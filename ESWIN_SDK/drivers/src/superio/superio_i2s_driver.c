/**
 * Copyright Statement:
 * This software and related documentation (ESWIN SOFTWARE) are protected under relevant copyright laws.
 * The information contained herein is confidential and proprietary to
 * Beijing ESWIN Computing Technology Co., Ltd.(ESWIN)and/or its licensors.
 * Without the prior written permission of ESWIN and/or its licensors, any reproduction, modification,
 * use or disclosure Software, and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * Copyright ©[2023] [Beijing ESWIN Computing Technology Co., Ltd.]. All rights reserved.
 *
 * RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES THAT THE SOFTWARE
 * AND ITS DOCUMENTATIONS (ESWIN SOFTWARE) RECEIVED FROM ESWIN AND / OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. ESWIN EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON INFRINGEMENT.
 * NEITHER DOES ESWIN PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY
 * WHICH MAY BE USED BY,INCORPORATED IN, OR SUPPLIED WITH THE ESWIN SOFTWARE,
 * AND RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ESWIN BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @file superio_i2s_driver.c
 * @brief SUPERIO i2s driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "superio_i2s_driver.h"
#include "clock_driver.h"
#include "superio_common.h"
#include "superio_hw_access.h"

/* Constraints used for baud rate computation */
#define DIVIDER_MIN_VALUE (1)
#define DIVIDER_MAX_VALUE (0xFF)

/* Shifters/Timers used for I2S simulation The parameter x represents the
   resourceIndex value for the current driver instance */
#define TX_SHIFTER(x)          (x)
#define RX_SHIFTER(x)          (uint8_t)((x) + 1U)
#define SCK_TIMER(x)           (x)
#define WS_TIMER(x)            (uint8_t)((x) + 1U)
#define WS_TIMER_RISING_BLK(x) (uint8_t)((x) + 2U)

/*
 * Function Name : SUPERIO_I2S_ComputeByteWidth
 * Description   : Computes the baud rate divider for a target baud rate
 *
 */
static uint8_t SUPERIO_I2S_ComputeByteWidth(uint8_t bitsWidth)
{
    uint8_t byteWidth;

    if (bitsWidth <= 8U) {
        byteWidth = 1U;
    } else if (bitsWidth <= 16U) {
        byteWidth = 2U;
    } else {
        byteWidth = 4U;
    }

    return byteWidth;
}

/*
 * Function Name : SUPERIO_I2S_MasterComputeBaudRateDivider
 * Description   : Computes the baud rate divider for a target baud rate
 *
 */
static void SUPERIO_I2S_MasterComputeBaudRateDivider(uint32_t baudRate, uint16_t *divider, uint32_t inputClock)
{
    int32_t tmpDiv;

    /* Compute divider: ((input_clock / baud_rate) / 2) - 1. Round to nearest integer */
    tmpDiv = (((int32_t)inputClock + (int32_t)baudRate) / (2 * (int32_t)baudRate)) - 1;
    /* Enforce upper/lower limits */
    if (tmpDiv < DIVIDER_MIN_VALUE) {
        tmpDiv = DIVIDER_MIN_VALUE;
    }
    if (tmpDiv > DIVIDER_MAX_VALUE) {
        tmpDiv = DIVIDER_MAX_VALUE;
    }

    *divider = (uint16_t)tmpDiv;
}

/*
 * Function Name : SUPERIO_I2S_MasterConfigure
 * Description   : configures the SUPERIO module as I2S master
 *
 */
static void SUPERIO_I2S_MasterConfigure(superio_i2s_master_state_t *master,
                                        const superio_i2s_master_user_config_t *userConfigPtr, uint32_t inputClock)
{
    superio_type_t *baseAddr;
    uint16_t divider;
    uint16_t bits;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;

    /* Compute divider. */
    SUPERIO_I2S_MasterComputeBaudRateDivider(userConfigPtr->baudRate, &divider, inputClock);
    bits = userConfigPtr->bitsWidth;

    /* Configure tx shifter */
    SUPERIO_SetShifterConfig(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_START_BIT_DISABLED_SH,
                             SUPERIO_SHIFTER_STOP_BIT_DISABLED, SUPERIO_SHIFTER_SOURCE_PIN);
    SUPERIO_SetShifterControl(
        baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_DISABLED, userConfigPtr->txPin, // output on tx pin
        SUPERIO_PIN_POLARITY_HIGH, SUPERIO_PIN_CONFIG_DISABLED,                                   // disable for now, will enable on transmit
        SCK_TIMER(resourceIndex),                                                                 // use clock timer to drive the shifter
        SUPERIO_TIMER_POLARITY_POSEDGE);

    /* Configure rx shifter */
    SUPERIO_SetShifterConfig(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_START_BIT_DISABLED,
                             SUPERIO_SHIFTER_STOP_BIT_DISABLED, SUPERIO_SHIFTER_SOURCE_PIN);
    SUPERIO_SetShifterControl(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_DISABLED,
                              userConfigPtr->rxPin, // output to rx pin
                              SUPERIO_PIN_POLARITY_HIGH, SUPERIO_PIN_CONFIG_DISABLED,
                              SCK_TIMER(resourceIndex), // use control timer to drive the shifter
                              SUPERIO_TIMER_POLARITY_NEGEDGE);

    /* Configure SCK timer */
    SUPERIO_SetTimerCompare(baseAddr, SCK_TIMER(resourceIndex),
                            (uint16_t)((uint16_t)(((uint16_t)(bits << 1U) - 1U) << 8U) + divider));
    SUPERIO_SetTimerConfig(baseAddr, SCK_TIMER(resourceIndex), SUPERIO_TIMER_START_BIT_ENABLED,
                           SUPERIO_TIMER_STOP_BIT_DISABLED,
                           SUPERIO_TIMER_ENABLE_TRG_HIGH, // enable when Tx data is available
                           SUPERIO_TIMER_DISABLE_NEVER, SUPERIO_TIMER_RESET_NEVER,
                           SUPERIO_TIMER_DECREMENT_CLK_SHIFT_TMR, // decrement on SuperIO clock
                           SUPERIO_TIMER_INITOUT_ONE);
    SUPERIO_SetTimerControl(
        baseAddr, SCK_TIMER(resourceIndex),
        (uint8_t)((uint8_t)(TX_SHIFTER(resourceIndex) << 2U) + 1U),                           // trigger on tx shifter status flag
        SUPERIO_TRIGGER_POLARITY_LOW, SUPERIO_TRIGGER_SOURCE_INTERNAL, userConfigPtr->sckPin, // output on SCK pin
        SUPERIO_PIN_POLARITY_LOW, SUPERIO_PIN_CONFIG_OUTPUT,                                  // enable output
        SUPERIO_TIMER_MODE_DISABLED);

    /* Configure WS timer */

    SUPERIO_SetTimerCompare(baseAddr, WS_TIMER(resourceIndex), (uint16_t)((bits * ((divider + 1U) * 2U)) - 1U));
    SUPERIO_SetTimerConfig(baseAddr, WS_TIMER(resourceIndex), SUPERIO_TIMER_START_BIT_DISABLED,
                           SUPERIO_TIMER_STOP_BIT_DISABLED,
                           SUPERIO_TIMER_ENABLE_TIM_ENABLE, // enable when SCK timer is enabled
                           SUPERIO_TIMER_DISABLE_NEVER, SUPERIO_TIMER_RESET_NEVER,
                           SUPERIO_TIMER_DECREMENT_CLK_SHIFT_TMR, // decrement on SuperIO clock
                           SUPERIO_TIMER_INITOUT_ONE);
    SUPERIO_SetTimerControl(baseAddr, WS_TIMER(resourceIndex), 0U, // trigger not used
                            SUPERIO_TRIGGER_POLARITY_HIGH, SUPERIO_TRIGGER_SOURCE_EXTERNAL,
                            userConfigPtr->wsPin,                                // output on WS pin
                            SUPERIO_PIN_POLARITY_LOW, SUPERIO_PIN_CONFIG_OUTPUT, // enable output
                            SUPERIO_TIMER_MODE_DISABLED);
}

/*
 * Function Name : SUPERIO_I2S_SlaveConfigure
 * Description   : configures the SUPERIO module as I2S slave
 *
 */
static void SUPERIO_I2S_SlaveConfigure(const superio_i2s_slave_state_t *slave,
                                       const superio_i2s_slave_user_config_t *userConfigPtr)
{
    superio_type_t *baseAddr;
    uint16_t bits;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    baseAddr      = g_superioBase[slave->superioCommon.instance];
    resourceIndex = slave->superioCommon.resourceIndex;
    bits          = userConfigPtr->bitsWidth;

    /* Configure tx shifter */
    SUPERIO_SetShifterConfig(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_START_BIT_DISABLED,
                             SUPERIO_SHIFTER_STOP_BIT_DISABLED, SUPERIO_SHIFTER_SOURCE_PIN);
    SUPERIO_SetShifterControl(
        baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_DISABLED, userConfigPtr->txPin, // output on tx pin
        SUPERIO_PIN_POLARITY_HIGH, SUPERIO_PIN_CONFIG_DISABLED,                                   // disable for now, will enable on transmit
        WS_TIMER(resourceIndex),                                                                  // use clock timer to drive the shifter
        SUPERIO_TIMER_POLARITY_POSEDGE);

    /* Configure rx shifter */
    SUPERIO_SetShifterConfig(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_START_BIT_DISABLED,
                             SUPERIO_SHIFTER_STOP_BIT_DISABLED, SUPERIO_SHIFTER_SOURCE_PIN);
    SUPERIO_SetShifterControl(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_DISABLED,
                              userConfigPtr->rxPin, // output to rx pin
                              SUPERIO_PIN_POLARITY_HIGH, SUPERIO_PIN_CONFIG_DISABLED,
                              WS_TIMER(resourceIndex), // use control timer to drive the shifter
                              SUPERIO_TIMER_POLARITY_NEGEDGE);

    /* Configure SCK timer */
    SUPERIO_SetTimerCompare(baseAddr, SCK_TIMER(resourceIndex), (uint16_t)((uint16_t)(bits << 2U) - 3U));
    SUPERIO_SetTimerConfig(baseAddr, SCK_TIMER(resourceIndex), SUPERIO_TIMER_START_BIT_DISABLED,
                           SUPERIO_TIMER_STOP_BIT_DISABLED,
                           SUPERIO_TIMER_ENABLE_PIN_POSEDGE, // enable on WS positive edge
                           SUPERIO_TIMER_DISABLE_TIM_CMP, SUPERIO_TIMER_RESET_NEVER,
                           SUPERIO_TIMER_DECREMENT_TRG_SHIFT_TRG, // decrement on SCK input
                           SUPERIO_TIMER_INITOUT_ONE);
    SUPERIO_SetTimerControl(
        baseAddr, SCK_TIMER(resourceIndex), (uint8_t)(userConfigPtr->sckPin << 1U),          // trigger on SCK pin
        SUPERIO_TRIGGER_POLARITY_LOW, SUPERIO_TRIGGER_SOURCE_INTERNAL, userConfigPtr->wsPin, // use WS input pin
        SUPERIO_PIN_POLARITY_LOW, SUPERIO_PIN_CONFIG_DISABLED, SUPERIO_TIMER_MODE_DISABLED);

    /* Configure WS_TIMER_RISING_BLK timer */
    SUPERIO_SetTimerCompare(baseAddr, WS_TIMER_RISING_BLK(resourceIndex), (uint16_t)((uint16_t)(bits << 2U) - 4U));
    SUPERIO_SetTimerConfig(baseAddr, WS_TIMER_RISING_BLK(resourceIndex), SUPERIO_TIMER_START_BIT_DISABLED,
                           SUPERIO_TIMER_STOP_BIT_DISABLED, SUPERIO_TIMER_ENABLE_PIN_POSEDGE_TRG_HIGH,
                           SUPERIO_TIMER_DISABLE_TRG, SUPERIO_TIMER_RESET_NEVER, SUPERIO_TIMER_DECREMENT_PIN_SHIFT_PIN,
                           SUPERIO_TIMER_INITOUT_ONE);
    SUPERIO_SetTimerControl(baseAddr, WS_TIMER_RISING_BLK(resourceIndex),
                            (uint8_t)((uint8_t)(SCK_TIMER(resourceIndex) << 2U) + 3U), // SCK timer trigger output
                            SUPERIO_TRIGGER_POLARITY_HIGH, SUPERIO_TRIGGER_SOURCE_INTERNAL,
                            userConfigPtr->sckPin, // SCK input pin
                            SUPERIO_PIN_POLARITY_HIGH, SUPERIO_PIN_CONFIG_DISABLED, SUPERIO_TIMER_MODE_DISABLED);

    /* Configure WS timer */
    SUPERIO_SetTimerCompare(baseAddr, WS_TIMER(resourceIndex), (uint16_t)((uint16_t)(bits << 1U) - 1U));
    SUPERIO_SetTimerConfig(
        baseAddr, WS_TIMER(resourceIndex), SUPERIO_TIMER_START_BIT_DISABLED, SUPERIO_TIMER_STOP_BIT_DISABLED,
        SUPERIO_TIMER_ENABLE_PIN_POSEDGE_TRG_HIGH, // enable at first falling edge after first rising edge of clk
        SUPERIO_TIMER_DISABLE_TIM_CMP_TRG_LOW, SUPERIO_TIMER_RESET_NEVER, SUPERIO_TIMER_DECREMENT_PIN_SHIFT_PIN,
        SUPERIO_TIMER_INITOUT_ONE);
    SUPERIO_SetTimerControl(baseAddr, WS_TIMER(resourceIndex),
                            (uint8_t)((uint8_t)(WS_TIMER_RISING_BLK(resourceIndex) << 2U) +
                                      3U), // WS_TIMER_RISING_BLK timer trigger output
                            SUPERIO_TRIGGER_POLARITY_HIGH, SUPERIO_TRIGGER_SOURCE_INTERNAL,
                            userConfigPtr->sckPin,                                 // SCK input pin
                            SUPERIO_PIN_POLARITY_LOW, SUPERIO_PIN_CONFIG_DISABLED, // enable output
                            SUPERIO_TIMER_MODE_DISABLED);
}

/*
 * Function Name : SUPERIO_I2S_MasterEndTransfer
 * Description   : End the current transfer
 *
 */
static void SUPERIO_I2S_MasterEndTransfer(superio_i2s_master_state_t *master)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;

    /* Disable transfer engine */
    switch (master->driverType) {
    case SUPERIO_DRIVER_TYPE_INTERRUPTS:
        /* Disable interrupts for Rx and Tx shifters */
        SUPERIO_SetShifterInterrupt(
            baseAddr, (uint8_t)((1U << TX_SHIFTER(resourceIndex)) | (1U << RX_SHIFTER(resourceIndex))), false);
        SUPERIO_SetShifterErrorInterrupt(
            baseAddr, (uint8_t)((1U << TX_SHIFTER(resourceIndex)) | (1U << RX_SHIFTER(resourceIndex))), false);
        break;
    case SUPERIO_DRIVER_TYPE_POLLING:
        /* Nothing to do here */
        break;
    case SUPERIO_DRIVER_TYPE_DMA:
        /* Stop PDMA channels */
        (void)PDMA_DRV_StopChannel(master->txDMAChannel);
        (void)PDMA_DRV_StopChannel(master->rxDMAChannel);
        /* Disable SuperIO Tx and Rx PDMA requests */
        SUPERIO_SetShifterDMARequest(
            baseAddr, (uint8_t)((1U << TX_SHIFTER(resourceIndex)) | (1U << RX_SHIFTER(resourceIndex))), false);
        break;
    default:
        /* Impossible type - do nothing */
        break;
    }

    master->driverIdle       = true;
    master->txData           = NULL;
    master->rxData           = NULL;
    master->txRemainingBytes = 0U;
    master->rxRemainingBytes = 0U;

    /* Signal transfer end for blocking transfers */
    if (master->blocking == true) {
        (void)OS_SemaphoreRelease(master->idleSemaphore);
    }
}

/*
 * Function Name : SUPERIO_I2S_MasterEnableTransfer
 * Description   : Enables timers and shifters to start a transfer
 *
 */
static void SUPERIO_I2S_MasterEnableTransfer(superio_i2s_master_state_t *master)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    resourceIndex = master->superioCommon.resourceIndex;
    baseAddr      = g_superioBase[master->superioCommon.instance];

    /* enable timers and shifters */
    SUPERIO_SetShifterMode(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_TRANSMIT);
    SUPERIO_SetShifterMode(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_RECEIVE);
    if (master->master) {
        SUPERIO_SetTimerMode(baseAddr, SCK_TIMER(resourceIndex), SUPERIO_TIMER_MODE_8BIT_BAUD);
    } else {
        SUPERIO_SetTimerMode(baseAddr, SCK_TIMER(resourceIndex), SUPERIO_TIMER_MODE_16BIT);
        SUPERIO_SetTimerMode(baseAddr, WS_TIMER_RISING_BLK(resourceIndex), SUPERIO_TIMER_MODE_16BIT);
    }
    SUPERIO_SetTimerMode(baseAddr, WS_TIMER(resourceIndex), SUPERIO_TIMER_MODE_16BIT);
}

/*
 * Function Name : SUPERIO_I2S_MasterStopTransfer
 * Description   : Stops the current transfer
 *
 */
static void SUPERIO_I2S_MasterStopTransfer(superio_i2s_master_state_t *master)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    resourceIndex = master->superioCommon.resourceIndex;
    baseAddr      = g_superioBase[master->superioCommon.instance];

    /* disable timers and shifters */
    SUPERIO_SetShifterMode(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_DISABLED);
    SUPERIO_SetShifterMode(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_DISABLED);
    SUPERIO_SetTimerMode(baseAddr, SCK_TIMER(resourceIndex), SUPERIO_TIMER_MODE_DISABLED);
    if (!master->master) {
        SUPERIO_SetTimerMode(baseAddr, WS_TIMER_RISING_BLK(resourceIndex), SUPERIO_TIMER_MODE_DISABLED);
    }
    SUPERIO_SetTimerMode(baseAddr, WS_TIMER(resourceIndex), SUPERIO_TIMER_MODE_DISABLED);
    /* Disable pin output */
    SUPERIO_SetShifterPinConfig(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_PIN_CONFIG_DISABLED);

    /* clear any leftover error flags */
    SUPERIO_ClearShifterErrorStatus(baseAddr, TX_SHIFTER(resourceIndex));
    SUPERIO_ClearShifterErrorStatus(baseAddr, RX_SHIFTER(resourceIndex));
    /* discard any leftover rx. data */
    SUPERIO_ClearShifterStatus(baseAddr, RX_SHIFTER(resourceIndex));

    /* end the transfer */
    SUPERIO_I2S_MasterEndTransfer(master);
}

/*
 * Function Name : SUPERIO_I2S_MasterWaitTransferEnd
 * Description   : waits for the end of a blocking transfer
 *
 */
static status_t SUPERIO_I2S_MasterWaitTransferEnd(superio_i2s_master_state_t *master, uint32_t timeout)
{
    OS_Status_t osifError = OS_OK;

    switch (master->driverType) {
    case SUPERIO_DRIVER_TYPE_INTERRUPTS:
        /* Wait for transfer to be completed by the IRQ */
        osifError = OS_SemaphoreAcquire(master->idleSemaphore, timeout);
        break;
    case SUPERIO_DRIVER_TYPE_POLLING:
        /* Call SUPERIO_I2S_DRV_MasterGetStatus() to do the transfer */
        while (SUPERIO_I2S_DRV_MasterGetStatus(master, NULL) == STATUS_BUSY) {
        }
        break;
    case SUPERIO_DRIVER_TYPE_DMA:
        osifError = OS_SemaphoreAcquire(master->idleSemaphore, timeout);
        break;
    default:
        /* Impossible type - do nothing */
        break;
    }

    /* blocking transfer is over */
    master->blocking = false;
    if (osifError == OS_ErrorTimeout) {
        /* abort current transfer */
        master->status = STATUS_TIMEOUT;
        SUPERIO_I2S_MasterStopTransfer(master);
    }

    return master->status;
}

/*
 * Function Name : SUPERIO_I2S_ReadData
 * Description   : reads data received by the module
 *
 */
static void SUPERIO_I2S_ReadData(superio_i2s_master_state_t *master)
{
    const superio_type_t *baseAddr;
    uint32_t data;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;

    /* Read data from shifter buffer */
    data = SUPERIO_ReadShifterBuffer(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_RW_MODE_BIT_SWAP);

    if (master->rxRemainingBytes > 0U) {
        if (master->rxData != NULL) {
            switch (master->byteWidth) {
            case 1U:
                *(uint8_t *)master->rxData = (uint8_t)data;
                break;
            case 2U:
                *(uint16_t *)master->rxData = (uint16_t)data;
                break;
            default:
                *(uint32_t *)master->rxData = (uint32_t)data;
                break;
            }
            /* Update rx buffer pointer */
            master->rxData = &master->rxData[master->byteWidth];
        }
        /* Update remaining bytes count even if buffer is null */
        master->rxRemainingBytes -= (uint32_t)(master->byteWidth);
    } else {
        /* No data to receive, just ignore the read data */
    }
}

/*
 * Function Name : SUPERIO_I2S_WriteData
 * Description   : writes data to be transmitted by the module
 *
 */
static void SUPERIO_I2S_WriteData(superio_i2s_master_state_t *master)
{
    superio_type_t *baseAddr;
    uint32_t data;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;

    if (master->txRemainingBytes == 0U) {
        /* Done transmitting */
        return;
    }

    /* Read data from user buffer and update tx buffer pointer and remaining bytes count */
    switch (master->byteWidth) {
    case 1U:
        data = (uint32_t)(*(const uint8_t *)master->txData);
        break;
    case 2U:
        data = (uint32_t)(*(const uint16_t *)master->txData);
        break;
    default:
        data = (uint32_t)(*(const uint32_t *)master->txData);
        break;
    }
    master->txData = &master->txData[master->byteWidth];
    master->txRemainingBytes -= (uint32_t)(master->byteWidth);

    /* Write data to shifter buffer */
    /* Shift data before bit-swapping it to get the relevant bits in the lower part of the shifter */
    data <<= 32U - (uint32_t)(master->bitsWidth);
    SUPERIO_WriteShifterBuffer(baseAddr, TX_SHIFTER(resourceIndex), data, SUPERIO_SHIFTER_RW_MODE_BIT_SWAP);
}

/*
 * Function Name : SUPERIO_I2S_MasterCheckStatus
 * Description   : Check status of the I2S transfer. This function can be
 *                 called either in an interrupt routine or directly in polling
 *                 mode to advance the I2S transfer.
 *
 */
static void SUPERIO_I2S_MasterCheckStatus(void *stateStruct)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)
    superio_i2s_master_state_t *master;

    OS_ASSERT(stateStruct != NULL);

    master        = (superio_i2s_master_state_t *)stateStruct;
    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;

    /* Check for errors */
    if (master->txData != NULL) {
        if (SUPERIO_GetShifterErrorStatus(baseAddr, TX_SHIFTER(resourceIndex))) {
            master->status = STATUS_I2S_TX_UNDERRUN;
            SUPERIO_ClearShifterErrorStatus(baseAddr, TX_SHIFTER(resourceIndex));
            /* Continue processing events */
        }
    }
    if (master->rxData != NULL) {
        if (SUPERIO_GetShifterErrorStatus(baseAddr, RX_SHIFTER(resourceIndex))) {
            master->status = STATUS_I2S_RX_OVERRUN;
            SUPERIO_ClearShifterErrorStatus(baseAddr, RX_SHIFTER(resourceIndex));
            /* Continue processing events */
        }
    }
    /* Check if data was received */
    if (SUPERIO_GetShifterStatus(baseAddr, RX_SHIFTER(resourceIndex))) {
        SUPERIO_I2S_ReadData(master);
        if ((master->rxData != NULL) && (master->rxRemainingBytes == 0U)) {
            /* Out of rx space, call callback to allow user to provide a new buffer */
            if (master->callback != NULL) {
                master->callback(I2S_EVENT_RX_FULL, master->callbackParam);
            }
        }
    }
    /* Check if transmitter needs more data */
    if (master->txData != NULL) {
        if (SUPERIO_GetShifterStatus(baseAddr, TX_SHIFTER(resourceIndex))) {
            SUPERIO_I2S_WriteData(master);
            if (master->txRemainingBytes == 0U) {
                master->txData = NULL;
                /* Out of data, call callback to allow user to provide a new buffer */
                if (master->callback != NULL) {
                    master->callback(I2S_EVENT_TX_EMPTY, master->callbackParam);
                }
                if ((master->txRemainingBytes == 0U) && (master->driverType == SUPERIO_DRIVER_TYPE_INTERRUPTS)) {
                    /* Still no more data to transmit, transmission will stop */
                    /* disable tx interrupts */
                    SUPERIO_SetShifterInterrupt(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), false);
                    SUPERIO_SetShifterErrorInterrupt(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), false);
                    /* Enable rx interrupt to signal end of transfer */
                    SUPERIO_SetShifterInterrupt(baseAddr, (uint8_t)(1U << RX_SHIFTER(resourceIndex)), true);
                }
            }
        }
    }

    /* Check if transfer is over */
    if (master->rxRemainingBytes == 0U) {
        /* Record success if there was no error */
        if (master->status == STATUS_BUSY) {
            master->status = STATUS_SUCCESS;
        }
        /* End transfer */
        SUPERIO_I2S_MasterStopTransfer(master);
        /* Call callback to announce the event to the user */
        if (master->callback != NULL) {
            master->callback(I2S_EVENT_END_TRANSFER, master->callbackParam);
        }
    }
}

/*
 * Function Name : SUPERIO_I2S_MasterEndDmaTxTransfer
 * Description   : function called at the end of a PDMA Tx transfer
 *
 */
static void SUPERIO_I2S_MasterEndDmaTxTransfer(void *stateStruct, pdma_chn_status_t status)
{
    superio_i2s_master_state_t *master;
    uint8_t dmaChn;

    OS_ASSERT(stateStruct != NULL);

    master = (superio_i2s_master_state_t *)stateStruct;

    /* Call callback to allow user to provide a new buffer */
    if (status == PDMA_CHN_ERROR) {
        /* PDMA error, stop transfer */
        master->status = STATUS_ERROR;
        /* End transfer */
        SUPERIO_I2S_MasterStopTransfer(master);
        /* Call callback to announce the event to the user */
        if (master->callback != NULL) {
            master->callback(I2S_EVENT_END_TRANSFER, master->callbackParam);
        }
        return;
    } else {
        if (master->callback != NULL) {
            master->callback(I2S_EVENT_TX_EMPTY, master->callbackParam);
        }
    }
    if (master->txRemainingBytes == 0U) {
        /* No more data to transmit, transmission will stop */
        master->txData = NULL;
    } else {
        /* There is more data to transfer, restart PDMA channel */
        /* Update buffer address and size */
        dmaChn = master->txDMAChannel;
        PDMA_DRV_SetSrcAddr(dmaChn, (uint32_t)(master->txData));
        PDMA_DRV_SetMajorLoopIterationCount(dmaChn, master->txRemainingBytes / master->byteWidth);
        /* Now that this tx is set up, clear remaining bytes count */
        master->txRemainingBytes = 0U;
        /* Start the channel */
        (void)PDMA_DRV_StartChannel(dmaChn);
    }
}

/*
 * Function Name : SUPERIO_I2S_MasterEndDmaRxTransfer
 * Description   : function called at the end of a PDMA Tx transfer
 *
 */
static void SUPERIO_I2S_MasterEndDmaRxTransfer(void *stateStruct, pdma_chn_status_t status)
{
    superio_i2s_master_state_t *master;
    uint8_t dmaChn;
    uint32_t addr;

    OS_ASSERT(stateStruct != NULL);

    (void)status;
    master = (superio_i2s_master_state_t *)stateStruct;

    /* Check if this was a reception */
    if (master->rxData != NULL) {
        if (status == PDMA_CHN_ERROR) {
            /* PDMA error, stop transfer */
            master->status = STATUS_ERROR;
        } else {
            /* Call callback to allow user to provide a new buffer */
            if (master->callback != NULL) {
                master->callback(I2S_EVENT_RX_FULL, master->callbackParam);
            }
        }
    }
    if (master->rxRemainingBytes == 0U) {
        /* No more data, end transfer */
        if (master->status == STATUS_BUSY) {
            master->status = STATUS_SUCCESS;
        }
        /* End transfer */
        SUPERIO_I2S_MasterStopTransfer(master);
        /* Call callback to announce the event to the user */
        if (master->callback != NULL) {
            master->callback(I2S_EVENT_END_TRANSFER, master->callbackParam);
        }
    } else {
        /* There is more data to transfer, restart PDMA channel */
        /* Update buffer address and size */
        dmaChn = master->rxDMAChannel;
        if (master->rxData != NULL) {
            addr = (uint32_t)(master->rxData);
        } else {
            /* if there is no data to receive, use dummy data as destination for PDMA transfer */
            addr = (uint32_t)(&(master->dummyDmaData));
        }
        PDMA_DRV_SetDestAddr(dmaChn, addr);
        PDMA_DRV_SetMajorLoopIterationCount(dmaChn, master->rxRemainingBytes / master->byteWidth);
        /* Now that this rx is set up, clear remaining bytes count */
        master->rxRemainingBytes = 0U;
        /* Start the channel */
        (void)PDMA_DRV_StartChannel(dmaChn);
    }
}

/*
 * Function Name : SUPERIO_I2S_MasterComputeTxRegAddr
 * Description   : Computes the address of the register used for PDMA tx transfer
 *
 */
static uint32_t SUPERIO_I2S_MasterComputeTxRegAddr(const superio_i2s_master_state_t *master)
{
    uint32_t addr;
    const superio_type_t *baseAddr;
    uint8_t shifter;

    baseAddr = g_superioBase[master->superioCommon.instance];
    shifter  = TX_SHIFTER(master->superioCommon.resourceIndex);
    addr     = (uint32_t)(&(baseAddr->SHIFTBUFBIS[shifter])) + (sizeof(uint32_t) - master->byteWidth);
    return addr;
}

/*
 * Function Name : SUPERIO_I2S_MasterComputeRxRegAddr
 * Description   : Computes the address of the register used for PDMA rx transfer
 *
 */
static uint32_t SUPERIO_I2S_MasterComputeRxRegAddr(const superio_i2s_master_state_t *master)
{
    uint32_t addr;
    const superio_type_t *baseAddr;
    uint8_t shifter;

    baseAddr = g_superioBase[master->superioCommon.instance];
    shifter  = RX_SHIFTER(master->superioCommon.resourceIndex);
    addr     = (uint32_t)(&(baseAddr->SHIFTBUFBIS[shifter]));
    return addr;
}

/*
 * Function Name : SUPERIO_I2S_MasterStartDmaTransfer
 * Description   : Starts a PDMA transfer
 *
 */
static void SUPERIO_I2S_MasterStartDmaTransfer(superio_i2s_master_state_t *master)
{
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)
    superio_type_t *baseAddr;
    uint32_t addr;
    uint8_t requestMask = 0U;

    /* Table to map SuperIO_i2s transfer sizes to PDMA transfer sizes */
    static const pdma_transfer_size_t dmaTransferSize[4U] = {PDMA_TRANSFER_SIZE_1B, PDMA_TRANSFER_SIZE_2B,
                                                             PDMA_TRANSFER_SIZE_4B, PDMA_TRANSFER_SIZE_4B};

    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;

    /* Configure Tx channel if this is a transmission */
    if (master->txData != NULL) {
        /* Configure the transfer control descriptor for the previously allocated channel */
        (void)PDMA_DRV_ConfigMultiBlockTransfer(master->txDMAChannel, PDMA_TRANSFER_MEM2PERIPH,
                                                (uint32_t)(master->txData), SUPERIO_I2S_MasterComputeTxRegAddr(master),
                                                dmaTransferSize[master->byteWidth - 1U], master->byteWidth,
                                                master->txRemainingBytes / master->byteWidth, true);
        /* Now that this tx is set up, clear remaining bytes count */
        master->txRemainingBytes = 0U;

        /* Setup callback for PDMA tx transfer end */
        (void)PDMA_DRV_InstallCallback(master->txDMAChannel, (pdma_callback_t)(SUPERIO_I2S_MasterEndDmaTxTransfer),
                                       (void *)(master));
        /* Start tx PDMA channel */
        (void)PDMA_DRV_StartChannel(master->txDMAChannel);
        requestMask = (uint8_t)(1U << TX_SHIFTER(resourceIndex));
    }
    /* Configure Rx channel; if this is a transmission we still use Rx for the "end transfer" event */
    if (master->rxData != NULL) {
        addr = (uint32_t)(master->rxData);
    } else {
        /* if there is no data to receive, use dummy data as destination for PDMA transfer */
        addr = (uint32_t)(&(master->dummyDmaData));
    }
    /* Configure the transfer control descriptor for the previously allocated channel */
    (void)PDMA_DRV_ConfigMultiBlockTransfer(
        master->rxDMAChannel, PDMA_TRANSFER_PERIPH2MEM, SUPERIO_I2S_MasterComputeRxRegAddr(master), addr,
        dmaTransferSize[master->byteWidth - 1U], master->byteWidth, master->rxRemainingBytes / master->byteWidth, true);
    if (master->rxData == NULL) {
        /* if there is no data to receive, don't increment destination offset */
        PDMA_DRV_SetDestOffset(master->rxDMAChannel, 0);
    }
    /* Now that this rx is set up, clear remaining bytes count */
    master->rxRemainingBytes = 0U;

    /* Setup callback for PDMA rx transfer end */
    (void)PDMA_DRV_InstallCallback(master->rxDMAChannel, (pdma_callback_t)(SUPERIO_I2S_MasterEndDmaRxTransfer),
                                   (void *)(master));
    /* Start rx PDMA channel */
    (void)PDMA_DRV_StartChannel(master->rxDMAChannel);
    requestMask |= (uint8_t)(1U << RX_SHIFTER(resourceIndex));

    /* Enable SuperIO PDMA requests */
    SUPERIO_SetShifterDMARequest(baseAddr, requestMask, true);
}

/*
 * Function Name : SUPERIO_I2S_DRV_MasterInit
 * Description   : Initialize the SuperIO_I2S master mode driver
 *
 */
status_t SUPERIO_I2S_DRV_MasterInit(uint32_t instance, const superio_i2s_master_user_config_t *userConfigPtr,
                                    superio_i2s_master_state_t *master)
{
    uint32_t inputClock;
    status_t clkErr;
    status_t retCode;
    status_t osifError = STATUS_SUCCESS;
    uint8_t dmaReqTx;
    uint8_t dmaReqRx;

    OS_ASSERT(master != NULL);
    OS_ASSERT(instance < SUPERIO_INSTANCE_COUNT);
    /* Check that device was initialized */
    OS_ASSERT(g_superioDeviceStatePtr[instance] != NULL);

    /* Get the protocol clock frequency */
    clkErr = CLOCK_SYS_GetFreq(g_superioClock[instance], &inputClock);
    OS_ASSERT(clkErr == STATUS_SUCCESS);
    OS_ASSERT(inputClock > 0U);

    /* Instruct the resource allocator that we need two shifters/timers */
    master->superioCommon.resourceCount = 2U;
    /* Common SuperIO driver initialization */
    retCode = SUPERIO_DRV_InitDriver(instance, (superio_common_state_t *)master);
    if (retCode != STATUS_SUCCESS) { /* Initialization failed, not enough resources */
        return retCode;
    }
    /* Initialize the semaphore */
    if (userConfigPtr->driverType != SUPERIO_DRIVER_TYPE_POLLING) {
        master->idleSemaphore = OS_SemaphoreNew(1U, 1U, NULL);
        OS_ASSERT(osifError == STATUS_SUCCESS);
    }

    /* Initialize driver-specific context structure */
    master->driverType       = userConfigPtr->driverType;
    master->bitsWidth        = userConfigPtr->bitsWidth;
    master->byteWidth        = SUPERIO_I2S_ComputeByteWidth(userConfigPtr->bitsWidth);
    master->driverIdle       = true;
    master->callback         = userConfigPtr->callback;
    master->callbackParam    = userConfigPtr->callbackParam;
    master->blocking         = false;
    master->txData           = NULL;
    master->txRemainingBytes = 0U;
    master->rxData           = NULL;
    master->rxRemainingBytes = 0U;
    master->master           = true;
    master->status           = STATUS_SUCCESS;

    /* Configure device for I2S mode */
    SUPERIO_I2S_MasterConfigure(master, userConfigPtr, inputClock);

    /* Set up transfer engine */
    switch (master->driverType) {
    case SUPERIO_DRIVER_TYPE_INTERRUPTS:
        master->superioCommon.isr = SUPERIO_I2S_MasterCheckStatus;
        break;
    case SUPERIO_DRIVER_TYPE_POLLING:
        /* Nothing to do here, SUPERIO_I2S_DRV_MasterGetStatus() will handle the transfer */
        break;
    case SUPERIO_DRIVER_TYPE_DMA:
        /* Store PDMA channel numbers */
        master->rxDMAChannel = userConfigPtr->rxDMAChannel;
        master->txDMAChannel = userConfigPtr->txDMAChannel;
        /* Configure PDMA request sources */
        dmaReqTx = g_superioDMASrc[instance][TX_SHIFTER(master->superioCommon.resourceIndex)];
        dmaReqRx = g_superioDMASrc[instance][RX_SHIFTER(master->superioCommon.resourceIndex)];
        (void)PDMA_DRV_SetChannelRequestAndTrigger(userConfigPtr->txDMAChannel, dmaReqTx, false);
        (void)PDMA_DRV_SetChannelRequestAndTrigger(userConfigPtr->rxDMAChannel, dmaReqRx, false);
        break;
    default:
        /* Impossible type - do nothing */
        break;
    }

    (void)clkErr;
    (void)osifError;
    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_I2S_DRV_MasterDeinit
 * Description   : De-initialize the SuperIO_I2S master mode driver
 *
 */
status_t SUPERIO_I2S_DRV_MasterDeinit(superio_i2s_master_state_t *master)
{
    OS_ASSERT(master != NULL);

    /* Check if driver is busy */
    if (!master->driverIdle) {
        return STATUS_BUSY;
    }

    /* Destroy the semaphore */
    if (master->driverType != SUPERIO_DRIVER_TYPE_POLLING) {
        (void)OS_SemaphoreDelete(master->idleSemaphore);
    }
    return SUPERIO_DRV_DeinitDriver((superio_common_state_t *)master);
}

/*
 * Function Name : SUPERIO_I2S_DRV_MasterSetConfig
 * Description   : Set the baud rate and bit width for any subsequent I2S transfer
 *
 */
status_t SUPERIO_I2S_DRV_MasterSetConfig(superio_i2s_master_state_t *master, uint32_t baudRate, uint8_t bitsWidth)
{
    superio_type_t *baseAddr;
    uint16_t divider;
    uint32_t inputClock;
    status_t clkErr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    OS_ASSERT(master != NULL);
    OS_ASSERT(bitsWidth <= 32U);
    /* for PDMA transfers bitsWidth must 8, 16, or 32 */
    OS_ASSERT(!((master->driverType == SUPERIO_DRIVER_TYPE_DMA) && (bitsWidth != 8U) && (bitsWidth != 16U) &&
                (bitsWidth != 32U)));

    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;

    /* Check if driver is busy */
    if (!master->driverIdle) {
        return STATUS_BUSY;
    }
    /* Get the protocol clock frequency */
    clkErr = CLOCK_SYS_GetFreq(g_superioClock[master->superioCommon.instance], &inputClock);
    OS_ASSERT(clkErr == STATUS_SUCCESS);
    OS_ASSERT(inputClock > 0U);

    /* Compute divider */
    SUPERIO_I2S_MasterComputeBaudRateDivider(baudRate, &divider, inputClock);

    /* Configure SCK timer */
    SUPERIO_SetTimerCompare(baseAddr, SCK_TIMER(resourceIndex),
                            (uint16_t)((((uint16_t)((uint16_t)bitsWidth << 1U) - 1U) << 8U) + divider));
    /* Configure WS timer */
    SUPERIO_SetTimerCompare(baseAddr, WS_TIMER(resourceIndex), (uint16_t)((bitsWidth * ((divider + 1U) * 2U)) - 1U));

    master->bitsWidth = bitsWidth;
    master->byteWidth = SUPERIO_I2S_ComputeByteWidth(bitsWidth);

    (void)clkErr;
    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_I2S_DRV_MasterGetBaudRate
 * Description   : Get the currently configured baud rate
 *
 */
status_t SUPERIO_I2S_DRV_MasterGetBaudRate(superio_i2s_master_state_t *master, uint32_t *baudRate)
{
    const superio_type_t *baseAddr;
    uint32_t inputClock;
    uint16_t divider;
    uint16_t timerCmp;
    status_t clkErr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    OS_ASSERT(master != NULL);
    OS_ASSERT(baudRate != NULL);

    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;

    /* Get the protocol clock frequency */
    clkErr = CLOCK_SYS_GetFreq(g_superioClock[master->superioCommon.instance], &inputClock);
    OS_ASSERT(clkErr == STATUS_SUCCESS);
    OS_ASSERT(inputClock > 0U);

    /* Get the currently configured divider */
    timerCmp = SUPERIO_GetTimerCompare(baseAddr, SCK_TIMER(resourceIndex));
    divider  = (uint16_t)(timerCmp & 0x00FFU);

    /* Compute baud rate: input_clock / (2 * (divider + 1)). Round to nearest integer */
    *baudRate = (inputClock + (uint32_t)divider + 1U) / (2U * ((uint32_t)divider + 1U));

    (void)clkErr;
    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_I2S_DRV_MasterSendData
 * Description   : Perform a non-blocking send transaction on the I2S bus
 *
 */
status_t SUPERIO_I2S_DRV_MasterSendData(superio_i2s_master_state_t *master, const uint8_t *txBuff, uint32_t txSize)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    OS_ASSERT(master != NULL);
    OS_ASSERT(txBuff != NULL);
    OS_ASSERT(txSize > 0U);
    OS_ASSERT((txSize % (uint32_t)(master->byteWidth)) == 0U);

    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;

    /* Check if driver is busy */
    if (!master->driverIdle) {
        return STATUS_BUSY;
    }

    master->txData           = txBuff;
    master->txRemainingBytes = txSize;
    /* also count received data, it tells us when tx is actually completed */
    master->rxRemainingBytes = txSize;
    master->rxData           = NULL;
    master->status           = STATUS_BUSY;
    master->driverIdle       = false;

    /* Enable pin output */
    SUPERIO_SetShifterPinConfig(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_PIN_CONFIG_OUTPUT);
    /* Enable timers and shifters */
    SUPERIO_I2S_MasterEnableTransfer(master);
    /* Enable transfer engine */
    switch (master->driverType) {
    case SUPERIO_DRIVER_TYPE_INTERRUPTS:
        /* Enable interrupts for Tx shifter */
        SUPERIO_SetShifterInterrupt(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), true);
        SUPERIO_SetShifterErrorInterrupt(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), true);
        break;
    case SUPERIO_DRIVER_TYPE_POLLING:
        /* Call SUPERIO_I2S_MasterCheckStatus once to send the first byte */
        SUPERIO_I2S_MasterCheckStatus(master);
        break;
    case SUPERIO_DRIVER_TYPE_DMA:
        SUPERIO_I2S_MasterStartDmaTransfer(master);
        break;
    default:
        /* Impossible type - do nothing */
        break;
    }

    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_I2S_DRV_MasterSendDataBlocking
 * Description   : Perform a blocking send transaction on the I2S bus
 *
 */
status_t SUPERIO_I2S_DRV_MasterSendDataBlocking(superio_i2s_master_state_t *master, const uint8_t *txBuff,
                                                uint32_t txSize, uint32_t timeout)
{
    /* Check if driver is busy */
    if (!master->driverIdle) {
        return STATUS_BUSY;
    }

    /* mark transfer as blocking */
    if (master->driverType != SUPERIO_DRIVER_TYPE_POLLING) {
        master->blocking = true;
        /* Dummy wait to ensure the semaphore is 0, no need to check result */
        (void)OS_SemaphoreAcquire(master->idleSemaphore, 0);
    }
    /* Call SUPERIO_I2S_DRV_MasterSendData to start transfer */
    (void)SUPERIO_I2S_DRV_MasterSendData(master, txBuff, txSize);

    /* Wait for transfer to end */
    return SUPERIO_I2S_MasterWaitTransferEnd(master, timeout);
}

/*
 * Function Name : SUPERIO_I2S_DRV_MasterReceiveData
 * Description   : Perform a non-blocking receive transaction on the I2S bus
 *
 */
status_t SUPERIO_I2S_DRV_MasterReceiveData(superio_i2s_master_state_t *master, uint8_t *rxBuff, uint32_t rxSize)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    OS_ASSERT(master != NULL);
    OS_ASSERT(rxBuff != NULL);
    OS_ASSERT(rxSize > 0U);
    OS_ASSERT((rxSize % (uint32_t)(master->byteWidth)) == 0U);

    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;

    /* Check if driver is busy */
    if (!master->driverIdle) {
        return STATUS_BUSY;
    }

    master->rxData           = rxBuff;
    master->rxRemainingBytes = rxSize;
    master->txData           = NULL;
    master->txRemainingBytes = 0U;
    master->status           = STATUS_BUSY;
    master->driverIdle       = false;

    /* Enable timers and shifters */
    SUPERIO_I2S_MasterEnableTransfer(master);

    /* Enable transfer engine */
    switch (master->driverType) {
    case SUPERIO_DRIVER_TYPE_INTERRUPTS:
        /* Enable interrupts for Rx shifter */
        SUPERIO_SetShifterInterrupt(baseAddr, (uint8_t)(1U << RX_SHIFTER(resourceIndex)), true);
        SUPERIO_SetShifterErrorInterrupt(baseAddr, (uint8_t)(1U << RX_SHIFTER(resourceIndex)), true);
        break;
    case SUPERIO_DRIVER_TYPE_POLLING:
        /* Call SUPERIO_I2S_MasterCheckStatus once to send the first byte */
        SUPERIO_I2S_MasterCheckStatus(master);
        break;
    case SUPERIO_DRIVER_TYPE_DMA:
        SUPERIO_I2S_MasterStartDmaTransfer(master);
        break;
    default:
        /* Impossible type - do nothing */
        break;
    }

    /* For master we need to send a dummy char to start the clock.
       For slave just put a 0 in the buffer to keep the tx line clear while receiving. */
    SUPERIO_WriteShifterBuffer(baseAddr, TX_SHIFTER(resourceIndex), 0x0, SUPERIO_SHIFTER_RW_MODE_BIT_SWAP);

    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_I2S_DRV_MasterReceiveDataBlocking
 * Description   : Perform a blocking receive transaction on the I2S bus
 *
 */
status_t SUPERIO_I2S_DRV_MasterReceiveDataBlocking(superio_i2s_master_state_t *master, uint8_t *rxBuff, uint32_t rxSize,
                                                   uint32_t timeout)
{
    /* Check if driver is busy */
    if (!master->driverIdle) {
        return STATUS_BUSY;
    }

    /* mark transfer as blocking */
    if (master->driverType != SUPERIO_DRIVER_TYPE_POLLING) {
        master->blocking = true;
        /* Dummy wait to ensure the semaphore is 0, no need to check result */
        (void)OS_SemaphoreAcquire(master->idleSemaphore, 0);
    }
    /* Call SUPERIO_I2S_DRV_MasterReceiveData to start transfer */
    (void)SUPERIO_I2S_DRV_MasterReceiveData(master, rxBuff, rxSize);

    /* Wait for transfer to end */
    return SUPERIO_I2S_MasterWaitTransferEnd(master, timeout);
}

/*
 * Function Name : SUPERIO_I2S_DRV_MasterTransferAbort
 * Description   : Aborts a non-blocking I2S master transaction
 *
 */
status_t SUPERIO_I2S_DRV_MasterTransferAbort(superio_i2s_master_state_t *master)
{
    OS_ASSERT(master != NULL);

    /* Check if driver is busy */
    if (master->driverIdle) {
        return STATUS_SUCCESS;
    }

    master->status = STATUS_I2S_ABORTED;
    SUPERIO_I2S_MasterStopTransfer(master);

    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_I2S_DRV_MasterGetStatus
 * Description   : Get the status of the current non-blocking I2S master transaction
 *
 */
status_t SUPERIO_I2S_DRV_MasterGetStatus(superio_i2s_master_state_t *master, uint32_t *bytesRemaining)
{
    uint32_t remainingBytes;

    OS_ASSERT(master != NULL);

    /* Use rxRemainingBytes even for transmit; byte is not transmitted
    until rx shifter reports a receive event */
    remainingBytes = master->rxRemainingBytes;

    if (!master->driverIdle) {
        switch (master->driverType) {
        case SUPERIO_DRIVER_TYPE_POLLING:
            /* In polling mode advance the I2S transfer here */
            SUPERIO_I2S_MasterCheckStatus(master);
            break;

        case SUPERIO_DRIVER_TYPE_DMA:
            /* In PDMA mode just update the remaining count.
            DO NOT write master->rxRemainingBytes directly !!! */
            remainingBytes = PDMA_DRV_GetRemainingMajorIterationsCount(master->rxDMAChannel);
            break;

        default:
            /* Nothing */
            break;
        }
    }

    if (bytesRemaining != NULL) {
        *bytesRemaining = remainingBytes;
    }

    if (!master->driverIdle) {
        return STATUS_BUSY;
    } else {
        return master->status;
    }
}

/*
 * Function Name : SUPERIO_I2S_DRV_MasterSetRxBuffer
 * Description   : Provide a buffer for receiving data.
 *
 */
status_t SUPERIO_I2S_DRV_MasterSetRxBuffer(superio_i2s_master_state_t *master, uint8_t *rxBuff, uint32_t rxSize)
{
    OS_ASSERT(master != NULL);
    OS_ASSERT(rxBuff != NULL);
    OS_ASSERT(rxSize > 0U);

    master->rxData           = rxBuff;
    master->rxRemainingBytes = rxSize;

    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_I2S_DRV_MasterSetTxBuffer
 * Description   : Provide a buffer for transmitting data.
 *
 */
status_t SUPERIO_I2S_DRV_MasterSetTxBuffer(superio_i2s_master_state_t *master, const uint8_t *txBuff, uint32_t txSize)
{
    OS_ASSERT(master != NULL);
    OS_ASSERT(txBuff != NULL);
    OS_ASSERT(txSize > 0U);

    master->txData           = txBuff;
    master->txRemainingBytes = txSize;
    /* for transmit we also count received bytes for end condition */
    master->rxRemainingBytes += txSize;

    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_I2S_DRV_SlaveInit
 * Description   : Initialize the SuperIO_I2S slave mode driver
 *
 */
status_t SUPERIO_I2S_DRV_SlaveInit(uint32_t instance, const superio_i2s_slave_user_config_t *userConfigPtr,
                                   superio_i2s_slave_state_t *slave)
{
    status_t retCode;
    status_t osifError = STATUS_SUCCESS;
    uint8_t dmaReqTx;
    uint8_t dmaReqRx;

    OS_ASSERT(slave != NULL);
    OS_ASSERT(instance < SUPERIO_INSTANCE_COUNT);
    /* Check that device was initialized */
    OS_ASSERT(g_superioDeviceStatePtr[instance] != NULL);

    /* Instruct the resource allocator that we need two shifters/timers */
    slave->superioCommon.resourceCount = 3U;
    /* Common SuperIO driver initialization */
    retCode = SUPERIO_DRV_InitDriver(instance, (superio_common_state_t *)slave);
    if (retCode != STATUS_SUCCESS) { /* Initialization failed, not enough resources */
        return retCode;
    }
    /* Initialize the semaphore */
    if (userConfigPtr->driverType != SUPERIO_DRIVER_TYPE_POLLING) {
        slave->idleSemaphore = OS_SemaphoreNew(1U, 1U, NULL);
        OS_ASSERT(slave->idleSemaphore != NULL);
    }
    /* Initialize driver-specific context structure */
    slave->driverType       = userConfigPtr->driverType;
    slave->bitsWidth        = userConfigPtr->bitsWidth;
    slave->byteWidth        = SUPERIO_I2S_ComputeByteWidth(userConfigPtr->bitsWidth);
    slave->driverIdle       = true;
    slave->callback         = userConfigPtr->callback;
    slave->callbackParam    = userConfigPtr->callbackParam;
    slave->blocking         = false;
    slave->txData           = NULL;
    slave->txRemainingBytes = 0U;
    slave->rxData           = NULL;
    slave->rxRemainingBytes = 0U;
    slave->master           = false;
    slave->status           = STATUS_SUCCESS;

    /* Configure device for I2S mode */
    SUPERIO_I2S_SlaveConfigure(slave, userConfigPtr);

    /* Set up transfer engine */
    switch (slave->driverType) {
    case SUPERIO_DRIVER_TYPE_INTERRUPTS:
        slave->superioCommon.isr = SUPERIO_I2S_MasterCheckStatus;
        break;
    case SUPERIO_DRIVER_TYPE_POLLING:
        /* Nothing to do here, SUPERIO_I2S_DRV_MasterGetStatus() will handle the transfer */
        break;
    case SUPERIO_DRIVER_TYPE_DMA:
        /* Store PDMA channel numbers */
        slave->rxDMAChannel = userConfigPtr->rxDMAChannel;
        slave->txDMAChannel = userConfigPtr->txDMAChannel;
        /* Configure PDMA request sources */
        dmaReqTx = g_superioDMASrc[instance][TX_SHIFTER(slave->superioCommon.resourceIndex)];
        dmaReqRx = g_superioDMASrc[instance][RX_SHIFTER(slave->superioCommon.resourceIndex)];
        (void)PDMA_DRV_SetChannelRequestAndTrigger(userConfigPtr->txDMAChannel, dmaReqTx, false);
        (void)PDMA_DRV_SetChannelRequestAndTrigger(userConfigPtr->rxDMAChannel, dmaReqRx, false);
        break;
    default:
        /* Impossible type - do nothing */
        break;
    }

    (void)osifError;
    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_I2S_DRV_SlaveSetConfig
 * Description   : Set the baud rate and bit width for any subsequent I2S transfer
 *
 */
status_t SUPERIO_I2S_DRV_SlaveSetConfig(superio_i2s_slave_state_t *slave, uint8_t bitsWidth)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    OS_ASSERT(slave != NULL);
    OS_ASSERT(bitsWidth <= 32U);
    /* for PDMA transfers bitsWidth must 8, 16, or 32 */
    OS_ASSERT(!((slave->driverType == SUPERIO_DRIVER_TYPE_DMA) && (bitsWidth != 8U) && (bitsWidth != 16U) &&
                (bitsWidth != 32U)));

    baseAddr      = g_superioBase[slave->superioCommon.instance];
    resourceIndex = slave->superioCommon.resourceIndex;

    /* Check if driver is busy */
    if (!slave->driverIdle) {
        return STATUS_BUSY;
    }

    slave->bitsWidth = bitsWidth;
    slave->byteWidth = SUPERIO_I2S_ComputeByteWidth(bitsWidth);

    /* Configure SCK timer */
    SUPERIO_SetTimerCompare(baseAddr, SCK_TIMER(resourceIndex), (uint16_t)((uint16_t)((uint16_t)bitsWidth << 2U) - 3U));
    /* Configure WS_TIMER_RISING_BLK timer */
    SUPERIO_SetTimerCompare(baseAddr, WS_TIMER_RISING_BLK(resourceIndex),
                            (uint16_t)((uint16_t)((uint16_t)bitsWidth << 2U) - 4U));
    /* Configure WS timer */
    SUPERIO_SetTimerCompare(baseAddr, WS_TIMER(resourceIndex), (uint16_t)((uint16_t)((uint16_t)bitsWidth << 1U) - 1U));

    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_I2S_DRV_MasterGetDefaultConfig
 * Description   : Returns default configuration structure for SuperIO_I2S master
 *
 */
void SUPERIO_I2S_DRV_MasterGetDefaultConfig(superio_i2s_master_user_config_t *userConfigPtr)
{
    OS_ASSERT(userConfigPtr != NULL);

    userConfigPtr->driverType    = SUPERIO_DRIVER_TYPE_INTERRUPTS;
    userConfigPtr->baudRate      = 256000U; // 8 kHz sampling rate, 16 bits per sample
    userConfigPtr->bitsWidth     = 16U;
    userConfigPtr->txPin         = 0U;
    userConfigPtr->rxPin         = 1U;
    userConfigPtr->sckPin        = 2U;
    userConfigPtr->wsPin         = 3U;
    userConfigPtr->callback      = NULL;
    userConfigPtr->callbackParam = NULL;
    userConfigPtr->rxDMAChannel  = 255U;
    userConfigPtr->txDMAChannel  = 255U;
}

/*
 * Function Name : SUPERIO_I2S_DRV_SlaveGetDefaultConfig
 * Description   : Returns default configuration structure for SuperIO_I2S slave
 *
 */
void SUPERIO_I2S_DRV_SlaveGetDefaultConfig(superio_i2s_slave_user_config_t *userConfigPtr)
{
    OS_ASSERT(userConfigPtr != NULL);

    userConfigPtr->driverType    = SUPERIO_DRIVER_TYPE_INTERRUPTS;
    userConfigPtr->bitsWidth     = 16U;
    userConfigPtr->txPin         = 0U;
    userConfigPtr->rxPin         = 1U;
    userConfigPtr->sckPin        = 2U;
    userConfigPtr->wsPin         = 3U;
    userConfigPtr->callback      = NULL;
    userConfigPtr->callbackParam = NULL;
    userConfigPtr->rxDMAChannel  = 255U;
    userConfigPtr->txDMAChannel  = 255U;
}

/*
 * Function Name : SUPERIO_I2S_DRV_SlaveDeinit
 * Description   : This function de-initializes the SuperIO_I2S driver in slave mode. The driver can't be used
 * again until reinitialized. The context structure is no longer needed by the driver and
 * can be freed after calling this function.
 *
 */
status_t SUPERIO_I2S_DRV_SlaveDeinit(superio_i2s_slave_state_t *slave)
{
    return SUPERIO_I2S_DRV_MasterDeinit(slave);
}

/*
 * Function Name : SUPERIO_I2S_DRV_SlaveSendData
 * Description   : This function starts the transmission of a block of data and returns immediately.
 * The rest of the transmission is handled by the interrupt service routine (if the driver
 * is initialized in interrupt mode) or by the SUPERIO_I2S_DRV_SlaveGetStatus function (if
 * the driver is initialized in polling mode).
 * Use SUPERIO_I2S_DRV_SlaveGetStatus() to check the progress of the transmission.
 *
 */
status_t SUPERIO_I2S_DRV_SlaveSendData(superio_i2s_slave_state_t *slave, const uint8_t *txBuff, uint32_t txSize)
{
    return SUPERIO_I2S_DRV_MasterSendData(slave, txBuff, txSize);
}

/*
 * Function Name : SUPERIO_I2S_DRV_SlaveSendDataBlocking
 * Description   : This function sends a block of data, and only returns when the transmission is complete.
 *
 */
status_t SUPERIO_I2S_DRV_SlaveSendDataBlocking(superio_i2s_slave_state_t *slave, const uint8_t *txBuff, uint32_t txSize,
                                               uint32_t timeout)
{
    return SUPERIO_I2S_DRV_MasterSendDataBlocking(slave, txBuff, txSize, timeout);
}

/*
 * Function Name : SUPERIO_I2S_DRV_SlaveReceiveData
 * Description   : This function starts the reception of a block of data and returns immediately.
 * The rest of the reception is handled by the interrupt service routine (if the driver
 * is initialized in interrupt mode) or by the SUPERIO_I2S_DRV_SlaveGetStatus function (if
 * the driver is initialized in polling mode).
 * Use SUPERIO_I2S_DRV_SlaveGetStatus() to check the progress of the reception.
 *
 */
status_t SUPERIO_I2S_DRV_SlaveReceiveData(superio_i2s_slave_state_t *slave, uint8_t *rxBuff, uint32_t rxSize)
{
    return SUPERIO_I2S_DRV_MasterReceiveData(slave, rxBuff, rxSize);
}

/*
 * Function Name : SUPERIO_I2S_DRV_SlaveReceiveDataBlocking
 * Description   : This function receives a block of data and only returns when the reception is complete.
 *
 */
status_t SUPERIO_I2S_DRV_SlaveReceiveDataBlocking(superio_i2s_slave_state_t *slave, uint8_t *rxBuff, uint32_t rxSize,
                                                  uint32_t timeout)
{
    return SUPERIO_I2S_DRV_MasterReceiveDataBlocking(slave, rxBuff, rxSize, timeout);
}

/*
 * Function Name : SUPERIO_I2S_DRV_SlaveTransferAbort
 * Description   : This function aborts a non-blocking I2S transfer.
 *
 */
status_t SUPERIO_I2S_DRV_SlaveTransferAbort(superio_i2s_slave_state_t *slave)
{
    return SUPERIO_I2S_DRV_MasterTransferAbort(slave);
}

/*
 * Function Name : SUPERIO_I2S_DRV_SlaveGetStatus
 * Description   : This function returns the current status of a non-blocking I2S slave transaction.
 * A return code of STATUS_BUSY means the transfer is still in progress.
 * Otherwise the function returns a status reflecting the outcome of the last transfer.
 * When the driver is initialized in polling mode this function also advances the transfer
 * by checking and handling the transmit and receive events, so it must be called
 * frequently to avoid overflows or underflows.
 *
 */
status_t SUPERIO_I2S_DRV_SlaveGetStatus(superio_i2s_slave_state_t *slave, uint32_t *bytesRemaining)
{
    return SUPERIO_I2S_DRV_MasterGetStatus(slave, bytesRemaining);
}

/*
 * Function Name : SUPERIO_I2S_DRV_SlaveSetRxBuffer
 * Description   : This function can be used to provide a driver with a new buffer for receiving data.
 * It can be called from the user callback when event STATUS_I2S_RX_OVERRUN is reported.
 * This way the reception will continue without interruption.
 *
 */
status_t SUPERIO_I2S_DRV_SlaveSetRxBuffer(superio_i2s_slave_state_t *slave, uint8_t *rxBuff, uint32_t rxSize)
{
    return SUPERIO_I2S_DRV_MasterSetRxBuffer(slave, rxBuff, rxSize);
}

/*
 * Function Name : SUPERIO_I2S_DRV_SlaveSetTxBuffer
 * Description   : This function can be used to provide a driver with a new buffer for transmitting data.
 * It can be called from the user callback when event STATUS_I2S_TX_UNDERRUN is reported.
 * This way the transmission will continue without interruption.
 *
 */
status_t SUPERIO_I2S_DRV_SlaveSetTxBuffer(superio_i2s_slave_state_t *slave, const uint8_t *txBuff, uint32_t txSize)
{
    return SUPERIO_I2S_DRV_MasterSetTxBuffer(slave, txBuff, txSize);
}
