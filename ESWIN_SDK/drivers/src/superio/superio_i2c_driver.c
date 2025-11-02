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
 * @file superio_i2c_driver.c
 * @brief SUPERIO i2c driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "superio_i2c_driver.h"
#include "clock_driver.h"
#include "superio_common.h"
#include "superio_hw_access.h"

/* Constraints used for baud rate computation */
#define DIVIDER_MIN_VALUE (1U)
#define DIVIDER_MAX_VALUE (0xFF)

/* Shifters/Timers used for I2C simulation The parameter x represents the
   resourceIndex value for the current driver instance */
#define TX_SHIFTER(x)    (x)
#define RX_SHIFTER(x)    (uint8_t)((x) + 1U)
#define SCL_TIMER(x)     (x)
#define CONTROL_TIMER(x) (uint8_t)((x) + 1U)

/*
 *
 * Function Name : SUPERIO_I2C_MasterComputeBaudRateDivider
 * Description   : Computes the baud rate divider for a target baud rate
 *
 */
static void SUPERIO_I2C_MasterComputeBaudRateDivider(uint32_t baudRate, uint16_t *divider, uint32_t inputClock)
{
    int32_t tmpDiv;

    /* Compute divider: ((input_clock / baud_rate) / 2) - 1 - 1. The extra -1 is from the
       timer reset setting used for clock stretching. Round to nearest integer */
    tmpDiv = (((int32_t)inputClock + (int32_t)baudRate) / (2 * (int32_t)baudRate)) - 2;
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
 * Function Name : SUPERIO_I2C_MasterConfigure
 * Description   : configures the SUPERIO module as I2C master
 *
 */
static void SUPERIO_I2C_MasterConfigure(const superio_i2c_master_state_t *master, uint32_t inputClock,
                                        uint32_t baudRate)
{
    superio_type_t *baseAddr;
    uint16_t divider;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;

    /* Compute divider.*/
    SUPERIO_I2C_MasterComputeBaudRateDivider(baudRate, &divider, inputClock);

    /* Configure tx shifter */
    SUPERIO_SetShifterConfig(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_START_BIT_0,
                             SUPERIO_SHIFTER_STOP_BIT_1, SUPERIO_SHIFTER_SOURCE_PIN);
    /* Shifter disabled and pin enabled causes the pin to be held low.
       So disable pin too, will be enabled at transfer time.  */
    SUPERIO_SetShifterControl(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_DISABLED,
                              master->sdaPin, // output on SDA pin
                              SUPERIO_PIN_POLARITY_LOW, SUPERIO_PIN_CONFIG_DISABLED,
                              CONTROL_TIMER(resourceIndex), // use control timer to drive the shifter
                              SUPERIO_TIMER_POLARITY_POSEDGE);

    /* Configure rx shifter */
    SUPERIO_SetShifterConfig(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_START_BIT_DISABLED,
                             SUPERIO_SHIFTER_STOP_BIT_0, SUPERIO_SHIFTER_SOURCE_PIN);
    SUPERIO_SetShifterControl(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_DISABLED,
                              master->sdaPin, // input from SDA pin
                              SUPERIO_PIN_POLARITY_HIGH, SUPERIO_PIN_CONFIG_DISABLED,
                              CONTROL_TIMER(resourceIndex), // use control timer to drive the shifter
                              SUPERIO_TIMER_POLARITY_NEGEDGE);

    /* Configure SCL timer */
    SUPERIO_SetTimerCompare(baseAddr, SCL_TIMER(resourceIndex), divider);
    SUPERIO_SetTimerConfig(baseAddr, SCL_TIMER(resourceIndex), SUPERIO_TIMER_START_BIT_ENABLED,
                           SUPERIO_TIMER_STOP_BIT_TIM_DIS,
                           SUPERIO_TIMER_ENABLE_TRG_HIGH, // enable when Tx data is available
                           SUPERIO_TIMER_DISABLE_NEVER,
                           SUPERIO_TIMER_RESET_PIN_OUT,           // reset if output equals pin (for clock stretching)
                           SUPERIO_TIMER_DECREMENT_CLK_SHIFT_TMR, // decrement on SuperIO clock
                           SUPERIO_TIMER_INITOUT_ZERO);
    SUPERIO_SetTimerControl(
        baseAddr, SCL_TIMER(resourceIndex),
        (uint8_t)((uint8_t)(TX_SHIFTER(resourceIndex) << 2U) + 1U),                    // trigger on tx shifter status flag
        SUPERIO_TRIGGER_POLARITY_LOW, SUPERIO_TRIGGER_SOURCE_INTERNAL, master->sclPin, // output on SCL pin
        SUPERIO_PIN_POLARITY_HIGH, SUPERIO_PIN_CONFIG_OPEN_DRAIN,                      // enable output
        SUPERIO_TIMER_MODE_DISABLED);

    /* Configure control timer for shifters */
    SUPERIO_SetTimerCompare(baseAddr, CONTROL_TIMER(resourceIndex), 0x000FU);
    SUPERIO_SetTimerConfig(
        baseAddr, CONTROL_TIMER(resourceIndex), SUPERIO_TIMER_START_BIT_ENABLED, SUPERIO_TIMER_STOP_BIT_TIM_CMP,
        SUPERIO_TIMER_ENABLE_TIM_ENABLE,                                  // enable on SCL timer enable
        SUPERIO_TIMER_DISABLE_TIM_DISABLE,                                // disable on SCL timer disable
        SUPERIO_TIMER_RESET_NEVER, SUPERIO_TIMER_DECREMENT_PIN_SHIFT_PIN, // decrement on SCL pin input
        SUPERIO_TIMER_INITOUT_ONE);
    SUPERIO_SetTimerControl(
        baseAddr, CONTROL_TIMER(resourceIndex),
        (uint8_t)((uint8_t)(TX_SHIFTER(resourceIndex) << 2U) + 1U),                    // trigger on tx shifter status flag
        SUPERIO_TRIGGER_POLARITY_LOW, SUPERIO_TRIGGER_SOURCE_INTERNAL, master->sclPin, // use SCL pin as input
        SUPERIO_PIN_POLARITY_LOW, SUPERIO_PIN_CONFIG_DISABLED, SUPERIO_TIMER_MODE_DISABLED);
}

/*
 * Function Name : SUPERIO_I2C_MasterSetBytesNo
 * Description   : configures the number of SCL clocks needed for the entire transmission
 *
 */

static void SUPERIO_I2C_MasterSetBytesNo(superio_type_t *baseAddr, superio_i2c_master_state_t *master)
{
    uint16_t timerCmp;
    uint32_t edgeNo;       /* total number of clock edges */
    uint32_t counter;      /* edge counter per timer reload */
    uint32_t reloads;      /* number of timer reloads */
    uint8_t resourceIndex; /* Index of first used internal resource instance (shifter and timer) */

    resourceIndex = master->superioCommon.resourceIndex;
    /* Compute number of SCL edges, including address */
    edgeNo  = (master->txRemainingBytes * 18U) + 2U;
    reloads = (uint32_t)((edgeNo + 255U) / 256U);
    counter = (uint32_t)((uint32_t)(edgeNo + (reloads - 1)) / (uint32_t)reloads); // round up

    /* Set number of ticks in high part of timer compare register */
    timerCmp = SUPERIO_GetTimerCompare(baseAddr, SCL_TIMER(resourceIndex));
    timerCmp = (uint16_t)((timerCmp & 0x00FFU) | (uint16_t)(((counter - 1U) & 0xFFU) << 8U));
    SUPERIO_SetTimerCompare(baseAddr, SCL_TIMER(resourceIndex), timerCmp);

    /* Store reload information */
    master->eventCount = (uint16_t)reloads;
    master->lastReload = (uint8_t)(edgeNo - ((reloads - 1U) * counter) - 1U);
    /* Handle no reload case */
    if (reloads == 1U) {
        SUPERIO_SetTimerDisable(baseAddr, SCL_TIMER(resourceIndex), SUPERIO_TIMER_DISABLE_TIM_CMP);
    }
}

/*
 * Function Name : SUPERIO_I2C_MasterSendAddress
 * Description   : send address byte
 *
 */
static void SUPERIO_I2C_MasterSendAddress(superio_type_t *baseAddr, const superio_i2c_master_state_t *master)
{
    uint8_t addrByte;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    resourceIndex = master->superioCommon.resourceIndex;
    /* Address byte: slave 7-bit address + D = 0(transmit) or 1 (receive) */
    addrByte = (uint8_t)((uint8_t)(master->slaveAddress << 1U) + (uint8_t)(master->receive ? 1U : 0U));
    SUPERIO_WriteShifterBuffer(baseAddr, TX_SHIFTER(resourceIndex), (uint32_t)addrByte << 24U,
                               SUPERIO_SHIFTER_RW_MODE_BIT_SWAP);
}

/*
 * Function Name : SUPERIO_I2C_ReadData
 * Description   : Handles data reception
 *
 */
static void SUPERIO_I2C_ReadData(superio_i2c_master_state_t *master)
{
    superio_type_t *baseAddr;
    uint8_t data;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;

    /* Read data from rx shifter */
    data = (uint8_t)SUPERIO_ReadShifterBuffer(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_RW_MODE_BIT_SWAP);

    if (master->addrReceived == false) {
        /* This was the address byte */
        master->addrReceived = true;
        if (master->receive == true) {
            /* Send ACK from now on */
            SUPERIO_SetShifterStopBit(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_STOP_BIT_0);
        }
    } else {
        master->rxRemainingBytes--;
        if (master->receive == true) {
            /* Put data in user buffer */
            *(master->rxData) = data;
            master->rxData++;
        }
    }
    if ((master->receive == true) && (master->rxRemainingBytes == 1U)) {
        /* Send NACK for last byte */
        SUPERIO_SetShifterStopBit(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_STOP_BIT_1);
        /* Also instruct rx shifter to expect NACK */
        SUPERIO_SetShifterStopBit(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_STOP_BIT_1);
    }
}

/*
 * Function Name : SUPERIO_I2C_WriteData
 * Description   : Handles data transmission
 *
 */
static void SUPERIO_I2C_WriteData(superio_i2c_master_state_t *master)
{
    superio_type_t *baseAddr;
    uint32_t data;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;

    /* If txRemainingBytes == 0 the transmission is over */
    if (master->txRemainingBytes == 0U) {
        return;
    }

    master->txRemainingBytes--;

    if (master->txRemainingBytes == 0U) {
        /* Done transmitting */
        if (master->sendStop == true) {
            /* Transmit stop condition */
            data = 0x0;
        } else {
            /* Do not transmit stop condition */
            data = 0xFFU;
        }
    } else if (master->receive == true) {
        /* Transmit 0xFF to leave the line free while receiving */
        data = 0xFFU;
    } else {
        /* Read data from user buffer */
        data = *(master->txData);
        master->txData++;
    }

    /* Shift data before bit-swapping it to get the relevant bits in the lower part of the shifter */
    data <<= 24U;
    SUPERIO_WriteShifterBuffer(baseAddr, TX_SHIFTER(resourceIndex), data, SUPERIO_SHIFTER_RW_MODE_BIT_SWAP);
}

/*
 * Function Name : SUPERIO_I2C_MasterEndTransfer
 * Description   : End the current transfer
 *
 */
static void SUPERIO_I2C_MasterEndTransfer(superio_i2c_master_state_t *master)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;

    /* Restore Rx stop bit, in case it was changed by a receive */
    SUPERIO_SetShifterStopBit(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_STOP_BIT_0);
    /* Restore Tx stop bit, in case it was changed by a receive */
    SUPERIO_SetShifterStopBit(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_STOP_BIT_1);
    /* Clear Rx status in case there is a character left in the buffer */
    SUPERIO_ClearShifterStatus(baseAddr, RX_SHIFTER(resourceIndex));

    /* Disable transfer engine */
    switch (master->driverType) {
    case SUPERIO_DRIVER_TYPE_INTERRUPTS:
        /* Disable interrupts for Rx and Tx shifters */
        SUPERIO_SetShifterInterrupt(
            baseAddr, (uint8_t)((1U << TX_SHIFTER(resourceIndex)) | (1U << RX_SHIFTER(resourceIndex))), false);
        SUPERIO_SetShifterErrorInterrupt(
            baseAddr, (uint8_t)((1U << TX_SHIFTER(resourceIndex)) | (1U << RX_SHIFTER(resourceIndex))), false);
        /* Disable interrupt for SCL timer */
        SUPERIO_SetTimerInterrupt(baseAddr, (uint8_t)(1U << SCL_TIMER(resourceIndex)), false);
        break;
    case SUPERIO_DRIVER_TYPE_POLLING:
        /* Nothing to do here */
        break;
    case SUPERIO_DRIVER_TYPE_DMA:
        /* Disable error interrupt for Rx shifter */
        SUPERIO_SetShifterErrorInterrupt(baseAddr, (uint8_t)(1U << RX_SHIFTER(resourceIndex)), false);
        /* Disable interrupt for SCL timer */
        SUPERIO_SetTimerInterrupt(baseAddr, (uint8_t)(1U << SCL_TIMER(resourceIndex)), false);
        /* Stop PDMA channels */
        (void)PDMA_DRV_StopChannel(master->txDMAChannel);
        (void)PDMA_DRV_StopChannel(master->rxDMAChannel);
        /* Disable SuperIO PDMA requests for both shifters */
        SUPERIO_SetShifterDMARequest(
            baseAddr, (uint8_t)((1U << TX_SHIFTER(resourceIndex)) | (1U << RX_SHIFTER(resourceIndex))), false);
        break;
    default:
        /* Impossible type - do nothing */
        break;
    }

    master->driverIdle = true;

    /* Signal transfer end for blocking transfers */
    if (master->blocking == true) {
        (void)OS_SemaphoreRelease(master->idleSemaphore);
    }
}

/*
 * Function Name : SUPERIO_I2C_MasterEnableTransfer
 * Description   : Enables timers and shifters to start a transfer
 *
 */
static void SUPERIO_I2C_MasterEnableTransfer(superio_i2c_master_state_t *master)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    resourceIndex = master->superioCommon.resourceIndex;
    baseAddr      = g_superioBase[master->superioCommon.instance];

    /* enable timers and shifters */
    SUPERIO_SetShifterMode(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_TRANSMIT);
    SUPERIO_SetShifterMode(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_RECEIVE);
    SUPERIO_SetTimerMode(baseAddr, SCL_TIMER(resourceIndex), SUPERIO_TIMER_MODE_8BIT_BAUD);
    SUPERIO_SetTimerMode(baseAddr, CONTROL_TIMER(resourceIndex), SUPERIO_TIMER_MODE_16BIT);
    /* enable Tx pin */
    SUPERIO_SetShifterPinConfig(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_PIN_CONFIG_OPEN_DRAIN);
}

/*
 * Function Name : SUPERIO_I2C_MasterStopTransfer
 * Description   : Forcefully stops the current transfer
 *
 */
static void SUPERIO_I2C_MasterStopTransfer(superio_i2c_master_state_t *master)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    resourceIndex = master->superioCommon.resourceIndex;
    baseAddr      = g_superioBase[master->superioCommon.instance];

    /* disable Tx pin */
    SUPERIO_SetShifterPinConfig(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_PIN_CONFIG_DISABLED);
    /* disable and re-enable timers and shifters to reset them */
    SUPERIO_SetShifterMode(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_DISABLED);
    SUPERIO_SetShifterMode(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_DISABLED);
    SUPERIO_SetTimerMode(baseAddr, SCL_TIMER(resourceIndex), SUPERIO_TIMER_MODE_DISABLED);
    SUPERIO_SetTimerMode(baseAddr, CONTROL_TIMER(resourceIndex), SUPERIO_TIMER_MODE_DISABLED);

    /* clear any leftover error flags */
    SUPERIO_ClearShifterErrorStatus(baseAddr, TX_SHIFTER(resourceIndex));
    SUPERIO_ClearShifterErrorStatus(baseAddr, RX_SHIFTER(resourceIndex));
    /* discard any leftover rx. data */
    SUPERIO_ClearShifterStatus(baseAddr, RX_SHIFTER(resourceIndex));
    /* Clear timer status */
    SUPERIO_ClearTimerStatus(baseAddr, SCL_TIMER(resourceIndex));

    /* end the transfer */
    SUPERIO_I2C_MasterEndTransfer(master);

    /* Check receive overflow */
    if ((master->rxRemainingBytes != 0U) && (master->status == STATUS_SUCCESS) && (master->receive == true) &&
        (master->driverType != SUPERIO_DRIVER_TYPE_DMA)) {
        master->status = STATUS_I2C_RX_OVERRUN;
    }
}

/*
 * Function Name : SUPERIO_I2C_MasterWaitTransferEnd
 * Description   : waits for the end of a blocking transfer
 *
 */
static status_t SUPERIO_I2C_MasterWaitTransferEnd(superio_i2c_master_state_t *master, uint32_t timeout)
{
    OS_Status_t osifError = OS_OK;

    switch (master->driverType) {
    case SUPERIO_DRIVER_TYPE_INTERRUPTS:
        /* Wait for transfer to be completed by the IRQ */
        osifError = OS_SemaphoreAcquire(master->idleSemaphore, timeout);
        break;
    case SUPERIO_DRIVER_TYPE_POLLING:
        /* Call SUPERIO_I2C_DRV_MasterGetStatus() to do the transfer */
        while (SUPERIO_I2C_DRV_MasterGetStatus(master, NULL) == STATUS_BUSY) {
        }
        break;
    case SUPERIO_DRIVER_TYPE_DMA:
        /* Wait for transfer to be completed by the PDMA IRQ */
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
        SUPERIO_I2C_MasterStopTransfer(master);
    }

    return master->status;
}

/*
 * Function Name : SUPERIO_I2C_MasterCheckNack
 * Description   : Checks if the current Rx shifter error is NACK or RX_OVERFLOW
 *
 *         If there is a Tx event active it is an indication that module was not
 *         serviced for a long time - chances are this is an overflow.
 *         It is not certain, and it is even possible to have both NACK and overflow,
 *         but there is no way to tell, so we chose the safe option (if it is an
 *         overflow and we abort the transfer we may block the I2C bus).
 *
 *
 */
static inline bool SUPERIO_I2C_MasterCheckNack(const superio_type_t *baseAddr, const superio_i2c_master_state_t *master)
{
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    resourceIndex = master->superioCommon.resourceIndex;
    return !(SUPERIO_GetShifterStatus(baseAddr, TX_SHIFTER(resourceIndex)));
}

/*
 * Function Name : SUPERIO_I2C_MasterBusBusy
 * Description   : Check status of the I2C bus.
 *                 If either SDA or SCL is low, the bus is busy.
 *
 */
static inline bool SUPERIO_I2C_MasterBusBusy(const superio_type_t *baseAddr, const superio_i2c_master_state_t *master)
{
    uint8_t pinMask = (uint8_t)((1U << master->sdaPin) | (1U << master->sclPin));
    if ((SUPERIO_GetPinData(baseAddr) & pinMask) == pinMask) {
        /* both pins are high, bus is not busy */
        return false;
    } else {
        /* bus is busy */
        return true;
    }
}

/*
 * Function Name : SUPERIO_I2C_MasterCheckStatus
 * Description   : Check status of the I2C transfer. This function can be
 *                 called either in an interrupt routine or directly in polling
 *                 mode to advance the I2C transfer.
 *
 */
static void SUPERIO_I2C_MasterCheckStatus(void *stateStruct)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)
    superio_i2c_master_state_t *master;
    uint16_t timerCmp;

    OS_ASSERT(stateStruct != NULL);

    master        = (superio_i2c_master_state_t *)stateStruct;
    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;

    /* Check for errors */
    if (SUPERIO_GetShifterErrorStatus(baseAddr, TX_SHIFTER(resourceIndex))) {
        master->status = STATUS_I2C_TX_UNDERRUN;
        SUPERIO_ClearShifterErrorStatus(baseAddr, TX_SHIFTER(resourceIndex));
        /* don't stop the transfer, continue processing events */
    }
    if (SUPERIO_GetShifterErrorStatus(baseAddr, RX_SHIFTER(resourceIndex))) {
        /* Device limitation: not possible to tell the difference between NACK and receive overflow */
        if (SUPERIO_I2C_MasterCheckNack(baseAddr, master)) {
            master->status = STATUS_I2C_RECEIVED_NACK;
            /* Force the transfer to stop */
            SUPERIO_I2C_MasterStopTransfer(master);
            /* Call callback to announce the event to the user */
            if (master->callback != NULL) {
                master->callback(I2C_MASTER_EVENT_END_TRANSFER, master->callbackParam);
            }
            return;
        } else {
            master->status = STATUS_I2C_RX_OVERRUN;
            SUPERIO_ClearShifterErrorStatus(baseAddr, RX_SHIFTER(resourceIndex));
            /* don't stop the transfer, continue processing events */
        }
    }
    /* Check if data was received */
    if (SUPERIO_GetShifterStatus(baseAddr, RX_SHIFTER(resourceIndex))) {
        SUPERIO_I2C_ReadData(master);
    }
    /* Check if transmitter needs more data */
    if (SUPERIO_GetShifterStatus(baseAddr, TX_SHIFTER(resourceIndex))) {
        SUPERIO_I2C_WriteData(master);
        if (master->txRemainingBytes == 0U) {
            /* Done transmitting, disable Tx interrupt */
            SUPERIO_SetShifterInterrupt(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), false);
        }
    }
    /* Check if the transfer is over */
    if (SUPERIO_GetTimerStatus(baseAddr, SCL_TIMER(resourceIndex))) {
        /* Deinit when when generate nine clock */
        if (master->driverIdle == true) {
            /* Clear timer status */
            SUPERIO_SetTimerDisable(baseAddr, SCL_TIMER(resourceIndex), SUPERIO_TIMER_DISABLE_NEVER);
            SUPERIO_I2C_MasterStopTransfer(master);
        }
        /* General interrupt behavior */
        else {
            master->eventCount--;
            /* Clear timer status */
            SUPERIO_ClearTimerStatus(baseAddr, SCL_TIMER(resourceIndex));
            if (master->eventCount == 2U) {
                /* Adjust number of ticks in high part of timer compare register  for the last reload */
                timerCmp = SUPERIO_GetTimerCompare(baseAddr, SCL_TIMER(resourceIndex));
                timerCmp = (uint16_t)((uint16_t)((uint32_t)timerCmp & 0x00FFU) |
                                      (uint16_t)(((uint32_t)(master->lastReload) & 0xFFU) << 8U));
                SUPERIO_SetTimerCompare(baseAddr, SCL_TIMER(resourceIndex), timerCmp);
            }
            if (master->eventCount == 1U) {
                /* Timer will disable on the next countdown complete */
                SUPERIO_SetTimerDisable(baseAddr, SCL_TIMER(resourceIndex), SUPERIO_TIMER_DISABLE_TIM_CMP);
            }

            if (master->eventCount == 0U) {
                SUPERIO_SetTimerDisable(baseAddr, SCL_TIMER(resourceIndex), SUPERIO_TIMER_DISABLE_NEVER);

                /* Record success if there was no error */
                if (master->status == STATUS_BUSY) {
                    master->status = STATUS_SUCCESS;
                }
                /* End transfer. In case of race condition between Tx_shifter and timer_end events,
                   it is possible for the clock to be restarted. So we use forced stop to avoid this. */
                SUPERIO_I2C_MasterStopTransfer(master);
                /* Call callback to announce the event to the user */
                if (master->callback != NULL) {
                    master->callback(I2C_MASTER_EVENT_END_TRANSFER, master->callbackParam);
                }
            }
        }
    }
}

/*
 * Function Name : SUPERIO_I2C_MasterComputeTxRegAddr
 * Description   : Computes the address of the register used for PDMA tx transfer
 *
 */
/*
static inline uint32_t SUPERIO_I2C_MasterComputeTxRegAddr(const superio_i2c_master_state_t *master)
{
    uint32_t addr;
    const superio_type_t *baseAddr;
    uint8_t shifter;

    baseAddr = g_superioBase[master->superioCommon.instance];
    shifter  = TX_SHIFTER(master->superioCommon.resourceIndex);
    addr     = (uint32_t)(&(baseAddr->SHIFTBUFBIS[shifter])) + (sizeof(uint32_t) - 1U);
    return addr;
}
*/

/*
 * Function Name : SUPERIO_I2C_MasterComputeRxRegAddr
 * Description   : Computes the address of the register used for PDMA rx transfer
 *
 */
static inline uint32_t SUPERIO_I2C_MasterComputeRxRegAddr(const superio_i2c_master_state_t *master)
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
 * Function Name : SUPERIO_I2C_MasterDmaBlockImmediate
 * Description   : Configures a PDMA transfer to trigger immediately
 *
 */
static inline void SUPERIO_I2C_MasterDmaBlockImmediate(pdma_software_tcd_t *stcdBase, uint8_t blockNo)
{
    /* Set the START bit for this TCD to 1 */
    stcdBase[blockNo].CSR |= (uint16_t)DMA_TCD_CSR_START(1U);
}

/*
 * Function Name : SUPERIO_I2C_MasterDmaBlockTerminate
 * Description   : Configures a PDMA transfer to disable the PDMA request upon transfer completion
 *
 */
static inline void SUPERIO_I2C_MasterDmaBlockTerminate(pdma_software_tcd_t *stcdBase, uint8_t blockNo)
{
    /* Set the DREQ bit for this TCD to 1 */
    stcdBase[blockNo].CSR |= (uint16_t)DMA_TCD_CSR_DREQ(1U);
}

/*
 * Function Name : SUPERIO_I2C_MasterDmaConfigTx
 * Description   : Configures PDMA transfer for Tx
 *
 */
static inline void SUPERIO_I2C_MasterDmaConfigTx(superio_i2c_master_state_t *master, pdma_software_tcd_t *stcdBase)
{
#if 0
    pdma_scatter_gather_list_t srcList[2U];
    pdma_scatter_gather_list_t destList[2U];
#endif

    // uint32_t addr;

    /* Configure Tx chain */
    /* First block: transmit data */
    if (master->receive == false) {
        // addr = (uint32_t)(master->txData);
    } else {
        /* if receiving, send 0xFF to keep the line clear */
        master->dummyDmaIdle = 0xFFU;
        // addr                 = (uint32_t)(&(master->dummyDmaIdle));
    }
#if 0
    srcList[0U].address  = addr;
    srcList[0U].length   = master->rxRemainingBytes;
    srcList[0U].type     = PDMA_TRANSFER_MEM2PERIPH;
    destList[0U].address = SUPERIO_I2C_MasterComputeTxRegAddr(master);
    destList[0U].length  = master->rxRemainingBytes;
    destList[0U].type    = PDMA_TRANSFER_MEM2PERIPH;
#endif

    /* Second block: transmit stop/repeated start */
    if (master->sendStop) {
        master->dummyDmaStop = 0U;
    } else {
        master->dummyDmaStop = 0xFFU;
    }

#if 0
    srcList[1U].address  = (uint32_t)(&(master->dummyDmaStop));
    srcList[1U].length   = 1U;
    srcList[1U].type     = PDMA_TRANSFER_MEM2PERIPH;
    destList[1U].address = SUPERIO_I2C_MasterComputeTxRegAddr(master);
    destList[1U].length  = 1U;
    destList[1U].type    = PDMA_TRANSFER_MEM2PERIPH;
    /* use 2 STCDs for tx, transfer size: 1 byte, 1 byte per PDMA request */
    (void)PDMA_DRV_ConfigScatterGatherTransfer(master->txDMAChannel, stcdBase, PDMA_TRANSFER_SIZE_1B, 1U, srcList,
                                               destList, 2U);
#endif
    if (master->receive == true) {
        /* if there is no data to transmit, don't increment source offset */
        PDMA_DRV_SetSrcOffset(master->txDMAChannel, 0);
    }
    /* set DREQ bit for last block */
    SUPERIO_I2C_MasterDmaBlockTerminate(stcdBase, 0U);
}

/*
 * Function Name : SUPERIO_I2C_MasterDmaConfigRx
 * Description   : Configures PDMA transfer for Rx
 *
 */
static inline void SUPERIO_I2C_MasterDmaConfigRx(superio_i2c_master_state_t *master, pdma_software_tcd_t *stcdBase)
{
#if 0
    pdma_scatter_gather_list_t srcList[6U];
    dma_scatter_gather_list_t destList[6U];
#endif
    uint32_t tmp;
    uint8_t blockCnt = 0U;
    const superio_type_t *baseAddr;
    uint8_t shifter;
    uint8_t dmaChn;

    baseAddr = g_superioBase[master->superioCommon.instance];
    dmaChn   = master->rxDMAChannel;

    /* Configure Rx chain */
    if (master->receive == false) {
        /* when transmitting we don't need scatter-gather for receive, just move read data to dummy location */
        (void)PDMA_DRV_ConfigMultiBlockTransfer(
            dmaChn, PDMA_TRANSFER_PERIPH2MEM, SUPERIO_I2C_MasterComputeRxRegAddr(master),
            (uint32_t)(&(master->dummyDmaReceive)), PDMA_TRANSFER_SIZE_1B, 1U, master->rxRemainingBytes + 1U, true);
        /* no data to receive, don't increment destination offset */
        PDMA_DRV_SetDestOffset(dmaChn, 0);
    } else {
/* First block: receive address byte (dummy read) */
#if 0
        srcList[blockCnt].address  = SUPERIO_I2C_MasterComputeRxRegAddr(master);
        srcList[blockCnt].length   = 1U;
        srcList[blockCnt].type     = PDMA_TRANSFER_PERIPH2MEM;
        destList[blockCnt].address = (uint32_t)(&(master->dummyDmaReceive));
        destList[blockCnt].length  = 1U;
        destList[blockCnt].type    = PDMA_TRANSFER_PERIPH2MEM;
#endif
        blockCnt++;
        /* When receiving just 1 byte skip the "middle" part */
        if (master->rxRemainingBytes > 1U) {
            /* Second block: set tx shifter stop bit to 0 (transmit ACK) */
            shifter = TX_SHIFTER(master->superioCommon.resourceIndex);
            tmp     = baseAddr->SHIFTCFG[shifter];
            tmp &= ~(SUPERIO_SHIFTCFG_SSTOP_MASK);
            tmp |= SUPERIO_SHIFTCFG_SSTOP(SUPERIO_SHIFTER_STOP_BIT_0);
            master->dmaReceiveTxStop0 = (uint8_t)(tmp & 0xFFU);
#if 0
            srcList[blockCnt].address  = (uint32_t)(&(master->dmaReceiveTxStop0));
            srcList[blockCnt].length   = 1U;
            srcList[blockCnt].type     = PDMA_TRANSFER_PERIPH2MEM;
            destList[blockCnt].address = (uint32_t)(&(baseAddr->SHIFTCFG[shifter]));
            destList[blockCnt].length  = 1U;
            destList[blockCnt].type    = PDMA_TRANSFER_PERIPH2MEM;
#endif
            blockCnt++;
/* Third block: receive all but the last data byte */
#if 0
            srcList[blockCnt].address  = SUPERIO_I2C_MasterComputeRxRegAddr(master);
            srcList[blockCnt].length   = master->rxRemainingBytes - 1U;
            srcList[blockCnt].type     = PDMA_TRANSFER_PERIPH2MEM;
            destList[blockCnt].address = (uint32_t)(master->rxData);
            destList[blockCnt].length  = master->rxRemainingBytes - 1U;
            destList[blockCnt].type    = PDMA_TRANSFER_PERIPH2MEM;
#endif
            blockCnt++;
            /* Fourth block: set tx shifter stop bit to 1 (transmit NACK for last byte) */
            tmp = baseAddr->SHIFTCFG[shifter];
            tmp &= ~(SUPERIO_SHIFTCFG_SSTOP_MASK);
            tmp |= SUPERIO_SHIFTCFG_SSTOP(SUPERIO_SHIFTER_STOP_BIT_1);
            master->dmaReceiveTxStop1 = (uint8_t)(tmp & 0xFFU);
#if 0
            srcList[blockCnt].address  = (uint32_t)(&(master->dmaReceiveTxStop1));
            srcList[blockCnt].length   = 1U;
            srcList[blockCnt].type     = PDMA_TRANSFER_PERIPH2MEM;
            destList[blockCnt].address = (uint32_t)(&(baseAddr->SHIFTCFG[shifter]));
            destList[blockCnt].length  = 1U;
            destList[blockCnt].type    = PDMA_TRANSFER_PERIPH2MEM;
#endif
            blockCnt++;
        }
        /* Fifth block: set rx shifter stop bit to 1 (expect NACK) */
        shifter = RX_SHIFTER(master->superioCommon.resourceIndex);
        tmp     = baseAddr->SHIFTCFG[shifter];
        tmp &= ~(SUPERIO_SHIFTCFG_SSTOP_MASK);
        tmp |= SUPERIO_SHIFTCFG_SSTOP(SUPERIO_SHIFTER_STOP_BIT_1);
        master->dmaReceiveRxStop1 = (uint8_t)(tmp & 0xFFU);
#if 0
        srcList[blockCnt].address  = (uint32_t)(&(master->dmaReceiveRxStop1));
        srcList[blockCnt].length   = 1U;
        srcList[blockCnt].type     = PDMA_TRANSFER_PERIPH2MEM;
        destList[blockCnt].address = (uint32_t)(&(baseAddr->SHIFTCFG[shifter]));
        destList[blockCnt].length  = 1U;
        destList[blockCnt].type    = PDMA_TRANSFER_PERIPH2MEM;
#endif
        blockCnt++;
/* Sixth block: receive last byte */
#if 0
        srcList[blockCnt].address  = SUPERIO_I2C_MasterComputeRxRegAddr(master);
        srcList[blockCnt].length   = 1U;
        srcList[blockCnt].type     = PDMA_TRANSFER_PERIPH2MEM;
        destList[blockCnt].address = (uint32_t)(&(master->rxData[master->rxRemainingBytes - 1U]));
        destList[blockCnt].length  = 1U;
        destList[blockCnt].type    = PDMA_TRANSFER_PERIPH2MEM;
#endif
        blockCnt++;
#if 0
        /* use blockCnt (3 or 6) STCDs for rx, transfer size: 1 byte, 1 byte per PDMA request */
        (void)PDMA_DRV_ConfigScatterGatherTransfer(dmaChn, stcdBase, PDMA_TRANSFER_SIZE_1B, 1U, srcList, destList,
                                                   blockCnt);
#endif
        /* set all config transfers to SHIFTCFG registers to trigger immediately */
        SUPERIO_I2C_MasterDmaBlockImmediate(stcdBase, 0U);
        if (master->rxRemainingBytes > 1U) {
            SUPERIO_I2C_MasterDmaBlockImmediate(stcdBase, 2U);
            SUPERIO_I2C_MasterDmaBlockImmediate(stcdBase, 3U);
        }
        /* set DREQ bit for last block */
        SUPERIO_I2C_MasterDmaBlockTerminate(stcdBase, (uint8_t)(blockCnt - 2U));
    }
}

/*
 * Function Name : SUPERIO_I2C_MasterStartDmaTransfer
 * Description   : Starts a PDMA transfer
 *
 */
static void SUPERIO_I2C_MasterStartDmaTransfer(superio_i2c_master_state_t *master)
{
    uint32_t alignedStcd;
    pdma_software_tcd_t *stcdBase;

    /* get aligned address to use for software TCDs */
    alignedStcd = STCD_ADDR(master->stcd);
    stcdBase    = (pdma_software_tcd_t *)(alignedStcd);

    /* Configure Tx and Rx chains */
    SUPERIO_I2C_MasterDmaConfigTx(master, stcdBase);
    SUPERIO_I2C_MasterDmaConfigRx(master, &stcdBase[SUPERIO_I2C_DMA_TX_CHAIN_LENGTH]);

    /* Start both PDMA channels */
    (void)PDMA_DRV_StartChannel(master->txDMAChannel);
    (void)PDMA_DRV_StartChannel(master->rxDMAChannel);
}

/*
 * Function Name : SUPERIO_I2C_MasterEndDmaTransfer
 * Description   : Starts a PDMA transfer
 *
 */
static void SUPERIO_I2C_MasterEndDmaTransfer(void *stateStruct)
{
    superio_i2c_master_state_t *master;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)
    superio_type_t *baseAddr;
    uint16_t timerCmp;

    OS_ASSERT(stateStruct != NULL);

    master        = (superio_i2c_master_state_t *)stateStruct;
    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;

    /* Check for PDMA transfer errors */
    if ((PDMA_DRV_GetChannelStatus(master->txDMAChannel) == PDMA_CHN_ERROR) ||
        (PDMA_DRV_GetChannelStatus(master->rxDMAChannel) == PDMA_CHN_ERROR)) {
        master->status = STATUS_ERROR;
        /* Force the transfer to stop */
        SUPERIO_I2C_MasterStopTransfer(master);
        /* Call callback to announce the event to the user */
        if (master->callback != NULL) {
            master->callback(I2C_MASTER_EVENT_END_TRANSFER, master->callbackParam);
        }
        return;
    }
    /* Check for NACK */
    if (SUPERIO_GetShifterErrorStatus(baseAddr, RX_SHIFTER(resourceIndex))) {
        SUPERIO_ClearShifterErrorStatus(baseAddr, RX_SHIFTER(resourceIndex));
        master->status = STATUS_I2C_RECEIVED_NACK;
        /* Force the transfer to stop */
        SUPERIO_I2C_MasterStopTransfer(master);
        /* Call callback to announce the event to the user */
        if (master->callback != NULL) {
            master->callback(I2C_MASTER_EVENT_END_TRANSFER, master->callbackParam);
        }
        return;
    }
    /* Check if the transfer is over */
    if (SUPERIO_GetTimerStatus(baseAddr, SCL_TIMER(resourceIndex))) {
        master->eventCount--;
        /* Clear timer status */
        SUPERIO_ClearTimerStatus(baseAddr, SCL_TIMER(resourceIndex));
        if (master->eventCount == 2U) {
            /* Adjust number of ticks in high part of timer compare register  for the last reload */
            timerCmp = SUPERIO_GetTimerCompare(baseAddr, SCL_TIMER(resourceIndex));
            timerCmp =
                (uint16_t)((uint32_t)timerCmp & 0x00FFU) | (uint16_t)(((uint32_t)(master->lastReload) & 0xFFU) << 8U);
            SUPERIO_SetTimerCompare(baseAddr, SCL_TIMER(resourceIndex), timerCmp);
        }
        if (master->eventCount == 1U) {
            /* Timer will disable on the next countdown complete */
            SUPERIO_SetTimerDisable(baseAddr, SCL_TIMER(resourceIndex), SUPERIO_TIMER_DISABLE_TIM_CMP);
        }

        if (master->eventCount == 0U) {
            SUPERIO_SetTimerDisable(baseAddr, SCL_TIMER(resourceIndex), SUPERIO_TIMER_DISABLE_NEVER);
            /* Record success if there was no error */
            if (master->status == STATUS_BUSY) {
                master->status = STATUS_SUCCESS;
            }
            /* End transfer */
            SUPERIO_I2C_MasterStopTransfer(master);
            /* Call callback to announce the event to the user */
            if (master->callback != NULL) {
                master->callback(I2C_MASTER_EVENT_END_TRANSFER, master->callbackParam);
            }
        }
    }
}

/*
 * Function Name : SUPERIO_I2C_MasterStartTransfer
 * Description   : Perform a send or receive transaction on the I2C bus
 *
 */
static status_t SUPERIO_I2C_MasterStartTransfer(superio_i2c_master_state_t *master, uint32_t size, bool sendStop,
                                                bool receive)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    OS_ASSERT(master != NULL);

    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;

    /* Check if bus is busy */
    if (SUPERIO_I2C_MasterBusBusy(baseAddr, master)) {
        return STATUS_I2C_BUS_BUSY;
    }

    /* Tx - one extra byte for stop condition */
    master->txRemainingBytes = size + 1U;
    master->rxRemainingBytes = size;
    master->status           = STATUS_BUSY;
    master->driverIdle       = false;
    master->sendStop         = sendStop;
    master->receive          = receive;
    master->addrReceived     = false;

    /* Configure device for requested number of bytes, keeping the existing baud rate */
    SUPERIO_I2C_MasterSetBytesNo(baseAddr, master);
    /* Enable timers and shifters */
    SUPERIO_I2C_MasterEnableTransfer(master);
    /* Enable transfer engine */
    switch (master->driverType) {
    case SUPERIO_DRIVER_TYPE_INTERRUPTS:
        /* Send address to start transfer */
        SUPERIO_I2C_MasterSendAddress(baseAddr, master);
        /* Enable interrupt for Tx and Rx shifters */
        SUPERIO_SetShifterInterrupt(
            baseAddr, (uint8_t)((1U << TX_SHIFTER(resourceIndex)) | (1U << RX_SHIFTER(resourceIndex))), true);
        SUPERIO_SetShifterErrorInterrupt(
            baseAddr, (uint8_t)((1U << TX_SHIFTER(resourceIndex)) | (1U << RX_SHIFTER(resourceIndex))), true);
        /* Enable interrupt for SCL timer */
        SUPERIO_SetTimerInterrupt(baseAddr, (uint8_t)(1U << SCL_TIMER(resourceIndex)), true);
        break;
    case SUPERIO_DRIVER_TYPE_POLLING:
        /* Send address to start transfer */
        SUPERIO_I2C_MasterSendAddress(baseAddr, master);
        /* Nothing to do here, SUPERIO_I2C_DRV_MasterGetStatus() will handle the transfer */
        break;
    case SUPERIO_DRIVER_TYPE_DMA:
        SUPERIO_I2C_MasterStartDmaTransfer(master);
        /* Enable error interrupt for Rx shifter - for NACK detection */
        SUPERIO_SetShifterErrorInterrupt(baseAddr, (uint8_t)(1U << RX_SHIFTER(resourceIndex)), true);
        /* Enable interrupt for SCL timer - for end of transfer detection */
        SUPERIO_SetTimerInterrupt(baseAddr, (uint8_t)(1U << SCL_TIMER(resourceIndex)), true);
        /* Disable system interrupt*/
        OS_DisableInterrupt();
        /* Send address to start transfer */
        SUPERIO_I2C_MasterSendAddress(baseAddr, master);
        /* Enable SuperIO PDMA requests for both shifters */
        SUPERIO_SetShifterDMARequest(
            baseAddr, (uint8_t)((1U << TX_SHIFTER(resourceIndex)) | (1U << RX_SHIFTER(resourceIndex))), true);
        /* Enable system interrupt*/
        OS_EnableInterrupt();
        break;
    default:
        /* Impossible type - do nothing */
        break;
    }

    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_I2C_DRV_MasterInit
 * Description   : Initialize the SuperIO_I2C master mode driver
 *
 */
status_t SUPERIO_I2C_DRV_MasterInit(uint32_t instance, const superio_i2c_master_user_config_t *userConfigPtr,
                                    superio_i2c_master_state_t *master)
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
    retCode = SUPERIO_DRV_InitDriver(instance, &(master->superioCommon));
    if (retCode != STATUS_SUCCESS) { /* Initialization failed, not enough resources */
        return retCode;
    }

    /* Initialize the semaphore */
    if (userConfigPtr->driverType != SUPERIO_DRIVER_TYPE_POLLING) {
        master->idleSemaphore = OS_SemaphoreNew(1, 1, NULL);
        OS_ASSERT(master->idleSemaphore != NULL);
    }

    /* Initialize driver-specific context structure */
    master->driverType    = userConfigPtr->driverType;
    master->slaveAddress  = userConfigPtr->slaveAddress;
    master->sdaPin        = userConfigPtr->sdaPin;
    master->sclPin        = userConfigPtr->sclPin;
    master->callback      = userConfigPtr->callback;
    master->callbackParam = userConfigPtr->callbackParam;
    master->blocking      = false;
    master->driverIdle    = true;
    master->status        = STATUS_SUCCESS;

    /* Configure device for I2C mode */
    SUPERIO_I2C_MasterConfigure(master, inputClock, userConfigPtr->baudRate);

    /* Set up transfer engine */
    switch (master->driverType) {
    case SUPERIO_DRIVER_TYPE_INTERRUPTS:
        master->superioCommon.isr = SUPERIO_I2C_MasterCheckStatus;
        break;
    case SUPERIO_DRIVER_TYPE_POLLING:
        /* Nothing to do here, SUPERIO_I2C_DRV_MasterGetStatus() will handle the transfer */
        break;
    case SUPERIO_DRIVER_TYPE_DMA:
        /* Store PDMA channel numbers */
        master->txDMAChannel = userConfigPtr->txDMAChannel;
        master->rxDMAChannel = userConfigPtr->rxDMAChannel;
        /* Configure PDMA request sources */
        dmaReqTx = g_superioDMASrc[instance][TX_SHIFTER(master->superioCommon.resourceIndex)];
        dmaReqRx = g_superioDMASrc[instance][RX_SHIFTER(master->superioCommon.resourceIndex)];
        (void)PDMA_DRV_SetChannelRequestAndTrigger(userConfigPtr->txDMAChannel, dmaReqTx, false);
        (void)PDMA_DRV_SetChannelRequestAndTrigger(userConfigPtr->rxDMAChannel, dmaReqRx, false);
        /* For PDMA transfers we use timer interrupts to signal transfer end */
        master->superioCommon.isr = SUPERIO_I2C_MasterEndDmaTransfer;
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
 * Function Name : SUPERIO_I2C_DRV_MasterDeinit
 * Description   : De-initialize the SuperIO_I2C master mode driver
 *
 */
status_t SUPERIO_I2C_DRV_MasterDeinit(superio_i2c_master_state_t *master)
{
    OS_ASSERT(master != NULL);

    /* Check if driver is busy */
    if (!master->driverIdle) {
        return STATUS_BUSY;
    }

    /* Destroy the semaphore */
    if (master->driverType != SUPERIO_DRIVER_TYPE_POLLING) {
        (void)OS_SemaphoreDelete(master->idleSemaphore); // Remove the semaphore
    }
    return SUPERIO_DRV_DeinitDriver(&(master->superioCommon));
}

/*
 * Function Name : SUPERIO_I2C_DRV_MasterSetBaudRate
 * Description   : Set the baud rate for any subsequent I2C communication
 *
 */
status_t SUPERIO_I2C_DRV_MasterSetBaudRate(superio_i2c_master_state_t *master, uint32_t baudRate)
{
    superio_type_t *baseAddr;
    uint16_t divider;
    uint16_t timerCmp;
    uint32_t inputClock;
    status_t clkErr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    OS_ASSERT(master != NULL);

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
    SUPERIO_I2C_MasterComputeBaudRateDivider(baudRate, &divider, inputClock);

    /* Configure timer divider in the lower 8 bits of TIMCMP[CMP] */
    timerCmp = SUPERIO_GetTimerCompare(baseAddr, SCL_TIMER(resourceIndex));
    timerCmp = (uint16_t)((timerCmp & 0xFF00U) | divider);
    SUPERIO_SetTimerCompare(baseAddr, SCL_TIMER(resourceIndex), timerCmp);

    (void)clkErr;
    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_I2C_DRV_MasterGetBaudRate
 * Description   : Get the currently configured baud rate
 *
 */
status_t SUPERIO_I2C_DRV_MasterGetBaudRate(superio_i2c_master_state_t *master, uint32_t *baudRate)
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
    timerCmp = SUPERIO_GetTimerCompare(baseAddr, SCL_TIMER(resourceIndex));
    divider  = (uint16_t)(timerCmp & 0x00FFU);

    /* Compute baud rate: input_clock / (2 * (divider + 2)). Round to nearest integer */
    *baudRate = (inputClock + divider + 2U) / (2U * ((uint32_t)divider + 2U));

    (void)clkErr;
    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_I2C_DRV_MasterSetSlaveAddr
 * Description   : Set the slave address for any subsequent I2C communication
 *
 */
status_t SUPERIO_I2C_DRV_MasterSetSlaveAddr(superio_i2c_master_state_t *master, uint16_t address)
{
    OS_ASSERT(master != NULL);

    /* Check if driver is busy */
    if (!master->driverIdle) {
        return STATUS_BUSY;
    }

    master->slaveAddress = address;
    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_I2C_DRV_MasterSendData
 * Description   : Perform a non-blocking send transaction on the I2C bus
 *
 */
status_t SUPERIO_I2C_DRV_MasterSendData(superio_i2c_master_state_t *master, const uint8_t *txBuff, uint32_t txSize,
                                        bool sendStop)
{
    OS_ASSERT(master != NULL);
    OS_ASSERT(txBuff != NULL);
    OS_ASSERT(txSize > 0U);

    /* Check if driver is busy */
    if (!master->driverIdle) {
        return STATUS_BUSY;
    }

    /* Initialize transfer data */
    master->txData = txBuff;
    /* Start the transfer */
    return SUPERIO_I2C_MasterStartTransfer(master, txSize, sendStop, false);
}

/*
 * Function Name : SUPERIO_I2C_DRV_MasterSendDataBlocking
 * Description   : Perform a blocking send transaction on the I2C bus
 *
 */
status_t SUPERIO_I2C_DRV_MasterSendDataBlocking(superio_i2c_master_state_t *master, const uint8_t *txBuff,
                                                uint32_t txSize, bool sendStop, uint32_t timeout)
{
    status_t status;

    OS_ASSERT(master != NULL);
    OS_ASSERT(txBuff != NULL);
    OS_ASSERT(txSize > 0U);

    /* Check if driver is busy */
    if (!master->driverIdle) {
        return STATUS_BUSY;
    }

    /* Mark transfer as blocking */
    if (master->driverType != SUPERIO_DRIVER_TYPE_POLLING) {
        master->blocking = true;
        /* Dummy wait to ensure the semaphore is 0, no need to check result */
        (void)OS_SemaphoreAcquire(master->idleSemaphore, 0);
    }
    /* Initialize transfer data */
    master->txData = txBuff;
    /* Start the transfer */
    status = SUPERIO_I2C_MasterStartTransfer(master, txSize, sendStop, false);
    if (status != STATUS_SUCCESS) {
        /* Transfer could not be started */
        master->blocking = false;
        return status;
    }

    /* Wait for transfer to end */
    return SUPERIO_I2C_MasterWaitTransferEnd(master, timeout);
}

/*
 * Function Name : SUPERIO_I2C_DRV_MasterReceiveData
 * Description   : Perform a non-blocking receive transaction on the I2C bus
 *
 */
status_t SUPERIO_I2C_DRV_MasterReceiveData(superio_i2c_master_state_t *master, uint8_t *rxBuff, uint32_t rxSize,
                                           bool sendStop)
{
    OS_ASSERT(master != NULL);
    OS_ASSERT(rxBuff != NULL);
    OS_ASSERT(rxSize > 0U);

    /* Check if driver is busy */
    if (!master->driverIdle) {
        return STATUS_BUSY;
    }

    /* Initialize transfer data */
    master->rxData = rxBuff;
    /* Start the transfer */
    return SUPERIO_I2C_MasterStartTransfer(master, rxSize, sendStop, true);
}

/*
 * Function Name : SUPERIO_I2C_DRV_MasterReceiveDataBlocking
 * Description   : Perform a blocking receive transaction on the I2C bus
 *
 */
status_t SUPERIO_I2C_DRV_MasterReceiveDataBlocking(superio_i2c_master_state_t *master, uint8_t *rxBuff, uint32_t rxSize,
                                                   bool sendStop, uint32_t timeout)
{
    status_t status;

    OS_ASSERT(master != NULL);
    OS_ASSERT(rxBuff != NULL);
    OS_ASSERT(rxSize > 0U);

    /* Check if driver is busy */
    if (!master->driverIdle) {
        return STATUS_BUSY;
    }

    /* Mark transfer as blocking */
    if (master->driverType != SUPERIO_DRIVER_TYPE_POLLING) {
        master->blocking = true;
        /* Dummy wait to ensure the semaphore is 0, no need to check result */
        (void)OS_SemaphoreAcquire(master->idleSemaphore, 0);
    }
    /* Initialize transfer data */
    master->rxData = rxBuff;
    /* Start the transfer */
    status = SUPERIO_I2C_MasterStartTransfer(master, rxSize, sendStop, true);
    if (status != STATUS_SUCCESS) {
        /* Transfer could not be started */
        master->blocking = false;
        return status;
    }

    /* Wait for transfer to end */
    return SUPERIO_I2C_MasterWaitTransferEnd(master, timeout);
}

/*
 * Function Name : SUPERIO_I2C_DRV_MasterTransferAbort
 * Description   : Aborts a non-blocking I2C master transaction
 *
 */
status_t SUPERIO_I2C_DRV_MasterTransferAbort(superio_i2c_master_state_t *master)
{
    OS_ASSERT(master != NULL);

    /* Warning: an ongoing transfer can't be aborted safely due to device limitation;
       there is no way to know the exact stage of the transfer, and if we disable the module
       during the ACK bit (transmit) or during a 0 data bit (receive) the slave will hold
       the SDA line low forever and block the I2C bus. NACK reception is the only exception,
       as there is no slave to hold the line low. Therefore this function should only be used
       in extreme circumstances, and the application must have a way to reset the I2C slave
    */

    /* Check if driver is busy */
    if (master->driverIdle) {
        return STATUS_SUCCESS;
    }

    master->status = STATUS_I2C_ABORTED;
    SUPERIO_I2C_MasterStopTransfer(master);

    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_I2C_DRV_MasterGetStatus
 * Description   : Get the status of the current non-blocking I2C master transaction
 *
 */
status_t SUPERIO_I2C_DRV_MasterGetStatus(superio_i2c_master_state_t *master, uint32_t *bytesRemaining)
{
    uint32_t remainingBytes;

    OS_ASSERT(master != NULL);

    /* Use rxRemainingBytes even for transmit; byte is not transmitted
       until rx shifter confirms the ACK */
    remainingBytes = master->rxRemainingBytes;

    if (!master->driverIdle) {
        switch (master->driverType) {
        case SUPERIO_DRIVER_TYPE_POLLING:
            /* In polling mode advance the I2C transfer here */
            SUPERIO_I2C_MasterCheckStatus(master);
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
 * Function Name : SUPERIO_I2C_DRV_GetDefaultConfig
 * Description   : Returns default configuration structure for SuperIO_I2C
 *
 */
void SUPERIO_I2C_DRV_GetDefaultConfig(superio_i2c_master_user_config_t *userConfigPtr)
{
    OS_ASSERT(userConfigPtr != NULL);

    userConfigPtr->slaveAddress  = 32U;
    userConfigPtr->driverType    = SUPERIO_DRIVER_TYPE_INTERRUPTS;
    userConfigPtr->baudRate      = 100000U;
    userConfigPtr->sdaPin        = 0U;
    userConfigPtr->sclPin        = 1U;
    userConfigPtr->callback      = NULL;
    userConfigPtr->callbackParam = NULL;
    userConfigPtr->rxDMAChannel  = 255U;
    userConfigPtr->txDMAChannel  = 255U;
}

/*
 * Function Name : SUPERIO_I2C_DRV_GenerateNineClock
 * Description   : Generate nine clock on SCL line to free SDA line
 *
 */
status_t SUPERIO_I2C_DRV_GenerateNineClock(superio_i2c_master_state_t *master)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)
    uint16_t timerCmp;
    status_t status;

    OS_ASSERT(master != NULL);

    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;

    if (master->driverIdle == true) {
        /* Set number of ticks in high part of timer compare register */
        timerCmp = SUPERIO_GetTimerCompare(baseAddr, SCL_TIMER(resourceIndex));
        /* Set ticks in high part of timer compare register to generated nine clock*/
        timerCmp = (uint16_t)((timerCmp & 0x00FFU) | (uint16_t)(((16U) & 0xFFU) << 8U));
        SUPERIO_SetTimerCompare(baseAddr, SCL_TIMER(resourceIndex), timerCmp);
        /* Disable timer on Timer compare */
        SUPERIO_SetTimerDisable(baseAddr, SCL_TIMER(resourceIndex), SUPERIO_TIMER_DISABLE_TIM_CMP);
        SUPERIO_SetTimerStop(baseAddr, SCL_TIMER(resourceIndex), SUPERIO_TIMER_STOP_BIT_DISABLED);
        /* Enable timers and shifters */
        SUPERIO_SetShifterMode(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_TRANSMIT);
        SUPERIO_SetTimerMode(baseAddr, SCL_TIMER(resourceIndex), SUPERIO_TIMER_MODE_8BIT_BAUD);
        /* Enable Interrupt SLC timer */
        SUPERIO_SetTimerInterrupt(baseAddr, (uint8_t)(1U << SCL_TIMER(resourceIndex)), true);

        /* Write any value to trigger timer */
        SUPERIO_WriteShifterBuffer(baseAddr, TX_SHIFTER(resourceIndex), (uint32_t)0x00U,
                                   SUPERIO_SHIFTER_RW_MODE_BIT_SWAP);

        status = STATUS_SUCCESS;
    } else {
        status = STATUS_BUSY;
    }

    return status;
}

/*
 * Function Name : SUPERIO_I2C_DRV_StatusGenerateNineClock
 * Description   : Check status of SCL timer be disable or not.
 *
 */
status_t SUPERIO_I2C_DRV_StatusGenerateNineClock(superio_i2c_master_state_t *master)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)
    uint32_t interruptMask;
    uint32_t tmp;
    status_t status;

    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;

    /* Check Enable SCL timer */
    interruptMask = (1UL << (uint32_t)SCL_TIMER(resourceIndex));
    tmp           = SUPERIO_GetAllTimerInterrupt(baseAddr);
    if ((tmp & interruptMask) != 0U) {
        /* The generation nine clock have not done yet*/
        status = STATUS_BUSY;
    } else {
        /* The generation nine clock is done*/
        status = STATUS_SUCCESS;
    }

    return status;
}

/*
 * Function Name : SUPERIO_I2C_DRV_GetBusStatus
 * Description   : Check status of the SDA line.
 *                 If either SDA or SCL is low, the bus is busy.
 *
 */
bool SUPERIO_I2C_DRV_GetBusStatus(const superio_i2c_master_state_t *master, bool sdaLine)
{

    superio_type_t *baseAddr;
    uint8_t pinMask;

    baseAddr = g_superioBase[master->superioCommon.instance];

    /* Select Pin */
    if (sdaLine) {
        /* Select SDA line */
        pinMask = (uint8_t)((1U << master->sdaPin));
    } else {
        /* Select SCL line */
        pinMask = (uint8_t)((1U << master->sclPin));
    }

    /* Check Pin */
    if ((SUPERIO_GetPinData(baseAddr) & pinMask) == pinMask) {
        /* SDA/SCL line be high */
        return true;
    } else {
        /* SDA/SCL line be low */
        return false;
    }
}
