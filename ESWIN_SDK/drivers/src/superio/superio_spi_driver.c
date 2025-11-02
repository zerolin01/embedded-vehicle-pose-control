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
 * @file superio_spi_driver.c
 * @brief SUPERIO spi driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "superio_spi_driver.h"
#include "clock_driver.h"
#include "superio_common.h"
#include "superio_hw_access.h"

/* Constraints used for baud rate computation */
#define DIVIDER_MIN_VALUE (1)
#define DIVIDER_MAX_VALUE (0xFF)

/* Shifters/Timers used for SPI simulation. The parameter x represents the
       resourceIndex value for the current driver instance */
#define TX_SHIFTER(x) (x)
#define RX_SHIFTER(x) (uint8_t)((x) + 1U)
#define SCK_TIMER(x)  (x)
#define SS_TIMER(x)   (uint8_t)((x) + 1U)

/* Dummy data to send when user provides no data */
#define SUPERIO_SPI_DUMMYDATA (0xFFFFFFFFU)

/*
 * Function Name : SUPERIO_SPI_MasterComputeBaudRateDivider
 * Description   : Computes the baud rate divider for a target baud rate
 *
 */
static void SUPERIO_SPI_MasterComputeBaudRateDivider(uint32_t baudRate, uint16_t *divider, uint32_t inputClock)
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
 * Function Name : SUPERIO_SPI_MasterConfigure
 * Description   : configures the SUPERIO module as SPI master
 *
 */
static void SUPERIO_SPI_MasterConfigure(const superio_spi_master_state_t *master,
                                        const superio_spi_master_user_config_t *userConfigPtr, uint32_t inputClock)
{
    superio_type_t *baseAddr;
    superio_pin_polarity_t clockPolarity;       /* Polarity of clock signal */
    superio_timer_polarity_t txShifterPolarity; /* Polarity of MOSI shifter */
    superio_timer_polarity_t rxShifterPolarity; /* Polarity of MISO shifter */
    uint16_t divider;
    superio_shifter_stop_t stopBit;
    superio_shifter_start_t startBit;
    uint8_t resourceIndex; /* Index of first used internal resource instance (shifter and timer) */

    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;
    /* Compute divider.*/
    SUPERIO_SPI_MasterComputeBaudRateDivider(userConfigPtr->baudRate, &divider, inputClock);
    /* add number of bits in the upper 8 bits of the divider. Formula is: TIMCMP[15:8] = (number of bits x 2) - 1 */
    divider += (uint16_t)((((uint16_t)(userConfigPtr->transferSize) * 8U * 2U) - 1U) << 8U);

    if (0U == userConfigPtr->clockPolarity) {
        /* CPOL = 0 */
        clockPolarity = SUPERIO_PIN_POLARITY_HIGH;
    } else {
        /* CPOL = 1 */
        clockPolarity = SUPERIO_PIN_POLARITY_LOW;
    }

    if (0U == userConfigPtr->clockPhase) {
        /* CPHA = 0 */
        txShifterPolarity = SUPERIO_TIMER_POLARITY_NEGEDGE;
        rxShifterPolarity = SUPERIO_TIMER_POLARITY_POSEDGE;
        stopBit           = SUPERIO_SHIFTER_STOP_BIT_DISABLED;
        startBit          = SUPERIO_SHIFTER_START_BIT_DISABLED;
    } else {
        /* CPHA = 1 */
        txShifterPolarity = SUPERIO_TIMER_POLARITY_POSEDGE;
        rxShifterPolarity = SUPERIO_TIMER_POLARITY_NEGEDGE;
        stopBit           = SUPERIO_SHIFTER_STOP_BIT_0;
        startBit          = SUPERIO_SHIFTER_START_BIT_DISABLED_SH;
    }

    /* Configure Tx shifter (MOSI) */
    SUPERIO_SetShifterControl(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_DISABLED,
                              userConfigPtr->mosiPin, SUPERIO_PIN_POLARITY_HIGH, SUPERIO_PIN_CONFIG_OUTPUT,
                              SCK_TIMER(resourceIndex), txShifterPolarity);
    SUPERIO_SetShifterConfig(baseAddr, TX_SHIFTER(resourceIndex), startBit, stopBit, SUPERIO_SHIFTER_SOURCE_PIN);

    /* Configure Rx shifter (MISO) */
    SUPERIO_SetShifterControl(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_DISABLED,
                              userConfigPtr->misoPin, SUPERIO_PIN_POLARITY_HIGH, SUPERIO_PIN_CONFIG_DISABLED,
                              SCK_TIMER(resourceIndex), rxShifterPolarity);
    SUPERIO_SetShifterConfig(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_START_BIT_DISABLED,
                             SUPERIO_SHIFTER_STOP_BIT_DISABLED, SUPERIO_SHIFTER_SOURCE_PIN);

    /* Configure sck timer */
    SUPERIO_SetTimerCompare(baseAddr, SCK_TIMER(resourceIndex), divider); // set baud rate, and number of bits
    SUPERIO_SetTimerConfig(baseAddr, SCK_TIMER(resourceIndex), SUPERIO_TIMER_START_BIT_ENABLED,
                           SUPERIO_TIMER_STOP_BIT_TIM_DIS, SUPERIO_TIMER_ENABLE_TRG_HIGH, SUPERIO_TIMER_DISABLE_TIM_CMP,
                           SUPERIO_TIMER_RESET_NEVER, SUPERIO_TIMER_DECREMENT_CLK_SHIFT_TMR,
                           SUPERIO_TIMER_INITOUT_ZERO);
    SUPERIO_SetTimerControl(
        baseAddr, SCK_TIMER(resourceIndex),
        (uint8_t)((uint8_t)(TX_SHIFTER(resourceIndex) << 2U) + 1U),                           // trigger on tx shifter status flag
        SUPERIO_TRIGGER_POLARITY_LOW, SUPERIO_TRIGGER_SOURCE_INTERNAL, userConfigPtr->sckPin, // output on clock pin
        clockPolarity,                                                                        // used configured polarity
        SUPERIO_PIN_CONFIG_OUTPUT, SUPERIO_TIMER_MODE_DISABLED);

    /* Configure SS timer */
    SUPERIO_SetTimerCompare(baseAddr, SS_TIMER(resourceIndex), 0xFFFFU);
    SUPERIO_SetTimerConfig(baseAddr, SS_TIMER(resourceIndex), SUPERIO_TIMER_START_BIT_DISABLED,
                           SUPERIO_TIMER_STOP_BIT_DISABLED,
                           SUPERIO_TIMER_ENABLE_TIM_ENABLE,   // enable when SCK timer is enabled
                           SUPERIO_TIMER_DISABLE_TIM_DISABLE, // disable when SCK timer is disabled
                           SUPERIO_TIMER_RESET_NEVER, SUPERIO_TIMER_DECREMENT_CLK_SHIFT_TMR, SUPERIO_TIMER_INITOUT_ONE);
    SUPERIO_SetTimerControl(baseAddr, SS_TIMER(resourceIndex), 0U, // trigger not used, using defaults
                            SUPERIO_TRIGGER_POLARITY_HIGH, SUPERIO_TRIGGER_SOURCE_EXTERNAL, userConfigPtr->ssPin,
                            SUPERIO_PIN_POLARITY_LOW, SUPERIO_PIN_CONFIG_OUTPUT, SUPERIO_TIMER_MODE_DISABLED);
}

/*
 * Function Name : SUPERIO_SPI_MasterEndTransfer
 * Description   : end a transfer
 *
 */
static void SUPERIO_SPI_MasterEndTransfer(superio_spi_master_state_t *master)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    OS_ASSERT(master != NULL);

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
        /* Disable SuperIO PDMA requests for both shifters */
        SUPERIO_SetShifterDMARequest(
            baseAddr, (uint8_t)((1U << TX_SHIFTER(resourceIndex)) | (1U << RX_SHIFTER(resourceIndex))), false);
        break;
    default:
        /* Impossible type - do nothing */
        break;
    }

    master->driverIdle       = true;
    master->txRemainingBytes = 0U;
    master->rxRemainingBytes = 0U;

    /* Signal transfer end for blocking transfers */
    if (master->blocking == true) {
        (void)OS_SemaphoreRelease(master->idleSemaphore);
    }
}

/*
 * Function Name : SUPERIO_SPI_MasterEnableTransfer
 * Description   : Enables timers and shifters to start a transfer
 *
 */
static void SUPERIO_SPI_MasterEnableTransfer(superio_spi_master_state_t *master)
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
        SUPERIO_SetTimerMode(baseAddr, SS_TIMER(resourceIndex), SUPERIO_TIMER_MODE_16BIT);
    } else {
        SUPERIO_SetTimerMode(baseAddr, SCK_TIMER(resourceIndex), SUPERIO_TIMER_MODE_16BIT);
    }
}

/*
 * Function Name : SUPERIO_SPI_MasterStopTransfer
 * Description   : Forcefully stops the current transfer
 *
 */
static void SUPERIO_SPI_MasterStopTransfer(superio_spi_master_state_t *master)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    resourceIndex = master->superioCommon.resourceIndex;
    baseAddr      = g_superioBase[master->superioCommon.instance];

    /* disable and re-enable timers and shifters to reset them */
    SUPERIO_SetShifterMode(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_DISABLED);
    SUPERIO_SetShifterMode(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_DISABLED);
    SUPERIO_SetTimerMode(baseAddr, SCK_TIMER(resourceIndex), SUPERIO_TIMER_MODE_DISABLED);
    SUPERIO_SetTimerMode(baseAddr, SS_TIMER(resourceIndex), SUPERIO_TIMER_MODE_DISABLED);

    /* clear any leftover error flags */
    SUPERIO_ClearShifterErrorStatus(baseAddr, TX_SHIFTER(resourceIndex));
    SUPERIO_ClearShifterErrorStatus(baseAddr, RX_SHIFTER(resourceIndex));
    /* discard any leftover rx. data */
    SUPERIO_ClearShifterStatus(baseAddr, RX_SHIFTER(resourceIndex));

    /* end the transfer */
    SUPERIO_SPI_MasterEndTransfer(master);
}

/*
 * Function Name : SUPERIO_SPI_MasterWaitTransferEnd
 * Description   : waits for the end of a blocking transfer
 *
 */
static status_t SUPERIO_SPI_MasterWaitTransferEnd(superio_spi_master_state_t *master, uint32_t timeout)
{
    OS_Status_t osifError = OS_OK;

    OS_ASSERT(master != NULL);

    switch (master->driverType) {
    case SUPERIO_DRIVER_TYPE_INTERRUPTS:
        /* Wait for transfer to be completed by the IRQ */
        osifError = OS_SemaphoreAcquire(master->idleSemaphore, timeout);
        break;
    case SUPERIO_DRIVER_TYPE_POLLING:
        /* Call SUPERIO_SPI_DRV_MasterGetStatus() to do the transfer */
        while (SUPERIO_SPI_DRV_MasterGetStatus(master, NULL) == STATUS_BUSY) {
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
    if (OS_ErrorTimeout == osifError) {
        /* abort current transfer */
        master->status = STATUS_TIMEOUT;
        SUPERIO_SPI_MasterStopTransfer(master);
    }

    return master->status;
}

/*
 * Function Name : SUPERIO_SPI_MasterEndDmaTransfer
 * Description   : end a PDMA transfer
 *
 */
static void SUPERIO_SPI_MasterEndDmaTransfer(void *stateStruct, pdma_chn_status_t status)
{
    superio_spi_master_state_t *master;

    OS_ASSERT(stateStruct != NULL);

    master = (superio_spi_master_state_t *)stateStruct;

    /* Record success if there was no error */
    if (status == PDMA_CHN_ERROR) {
        master->status = STATUS_ERROR;
    } else {
        master->status = STATUS_SUCCESS;
    }
    SUPERIO_SPI_MasterStopTransfer(master);
    /* Call callback to announce the event to the user */
    if (NULL != master->callback) {
        master->callback(master, SPI_EVENT_END_TRANSFER, master->callbackParam);
    }
}

/*
 * Function Name : SUPERIO_SPI_SlaveConfigure
 * Description   : configures the SUPERIO module as SPI slave
 *
 */
static void SUPERIO_SPI_SlaveConfigure(const superio_spi_slave_state_t *slave,
                                       const superio_spi_slave_user_config_t *userConfigPtr)
{
    superio_type_t *baseAddr;
    superio_pin_polarity_t clockPolarity;       // Polarity of clock signal
    superio_timer_polarity_t txShifterPolarity; // Polarity of MISO shifter
    superio_timer_polarity_t rxShifterPolarity; // Polarity of MOSI shifter
    superio_shifter_start_t startBit;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    baseAddr      = g_superioBase[slave->superioCommon.instance];
    resourceIndex = slave->superioCommon.resourceIndex;

    if (0U == userConfigPtr->clockPolarity) {
        /* CPOL = 0 */
        clockPolarity = SUPERIO_PIN_POLARITY_HIGH;
    } else {
        /* CPOL = 1 */
        clockPolarity = SUPERIO_PIN_POLARITY_LOW;
    }

    if (0U == userConfigPtr->clockPhase) {
        /* CPHA = 0 */
        txShifterPolarity = SUPERIO_TIMER_POLARITY_NEGEDGE;
        rxShifterPolarity = SUPERIO_TIMER_POLARITY_POSEDGE;
        startBit          = SUPERIO_SHIFTER_START_BIT_DISABLED;
    } else {
        /* CPHA = 1 */
        txShifterPolarity = SUPERIO_TIMER_POLARITY_POSEDGE;
        rxShifterPolarity = SUPERIO_TIMER_POLARITY_NEGEDGE;
        startBit          = SUPERIO_SHIFTER_START_BIT_DISABLED_SH;
    }

    /* Configure Slave Tx shifter (MISO) */
    SUPERIO_SetShifterControl(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_DISABLED,
                              userConfigPtr->misoPin, SUPERIO_PIN_POLARITY_HIGH, SUPERIO_PIN_CONFIG_OUTPUT,
                              SCK_TIMER(resourceIndex), txShifterPolarity);
    SUPERIO_SetShifterConfig(baseAddr, TX_SHIFTER(resourceIndex), startBit, SUPERIO_SHIFTER_STOP_BIT_DISABLED,
                             SUPERIO_SHIFTER_SOURCE_PIN);

    /* Configure Slave Rx shifter (MOSI) */
    SUPERIO_SetShifterControl(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_DISABLED,
                              userConfigPtr->mosiPin, SUPERIO_PIN_POLARITY_HIGH, SUPERIO_PIN_CONFIG_DISABLED,
                              SCK_TIMER(resourceIndex), rxShifterPolarity);
    SUPERIO_SetShifterConfig(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_START_BIT_DISABLED,
                             SUPERIO_SHIFTER_STOP_BIT_DISABLED, SUPERIO_SHIFTER_SOURCE_PIN);

    /* Configure sck timer */
    SUPERIO_SetTimerCompare(baseAddr, SCK_TIMER(resourceIndex), 0xFFFFU);
    SUPERIO_SetTimerConfig(
        baseAddr, SCK_TIMER(resourceIndex), SUPERIO_TIMER_START_BIT_DISABLED, SUPERIO_TIMER_STOP_BIT_DISABLED,
        SUPERIO_TIMER_ENABLE_TRG_POSEDGE,                                 // enable on SS pin rising edge
        SUPERIO_TIMER_DISABLE_TRG,                                        // disable on SS pin falling edge
        SUPERIO_TIMER_RESET_NEVER, SUPERIO_TIMER_DECREMENT_PIN_SHIFT_PIN, // decrement on input pin - SCK
        SUPERIO_TIMER_INITOUT_ZERO);
    SUPERIO_SetTimerControl(baseAddr, SCK_TIMER(resourceIndex),
                            (uint8_t)(userConfigPtr->ssPin << 1U), // trigger on SS pin edge
                            SUPERIO_TRIGGER_POLARITY_LOW, SUPERIO_TRIGGER_SOURCE_INTERNAL,
                            userConfigPtr->sckPin, // use SCK pin as input pin
                            clockPolarity, SUPERIO_PIN_CONFIG_DISABLED, SUPERIO_TIMER_MODE_DISABLED);
}

/*
 * Function Name : SUPERIO_SPI_ReadData
 * Description   : reads data received by the module
 *
 */
static void SUPERIO_SPI_ReadData(superio_spi_master_state_t *master)
{
    const superio_type_t *baseAddr;
    uint32_t data;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;

    /* Read data from shifter buffer */
    if (SUPERIO_SPI_TRANSFER_LSB_FIRST == master->bitOrder) {
        /* For data size < 4 bytes our data is in the upper part of the buffer and must be shifted */
        data = SUPERIO_ReadShifterBuffer(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_RW_MODE_NORMAL);
        data >>= (32U - (8U * (uint32_t)(master->transferSize)));
    } else {
        data = SUPERIO_ReadShifterBuffer(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_RW_MODE_BIT_SWAP);
    }

    if ((master->rxRemainingBytes > 0U) && (master->rxData != NULL)) {
        switch (master->transferSize) {
        case SUPERIO_SPI_TRANSFER_1BYTE:
            *(uint8_t *)master->rxData = (uint8_t)data;
            break;
        case SUPERIO_SPI_TRANSFER_2BYTE:
            *(uint16_t *)master->rxData = (uint16_t)data;
            break;
        case SUPERIO_SPI_TRANSFER_4BYTE:
            *(uint32_t *)master->rxData = (uint32_t)data;
            break;
        default:
            /* Not possible */
            break;
        }
        /* Update rx buffer pointer and remaining bytes count */
        master->rxData = &master->rxData[(master->transferSize)];
        master->rxRemainingBytes -= (uint32_t)(master->transferSize);
    } else {
        /* No data to receive, just ignore the read data */
    }
}

/*
 * Function Name : SUPERIO_SPI_WriteData
 * Description   : writes data to be transmitted by the module
 *
 */
static void SUPERIO_SPI_WriteData(superio_spi_master_state_t *master)
{
    superio_type_t *baseAddr;
    uint32_t data = SUPERIO_SPI_DUMMYDATA;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;

    if (0U == master->txRemainingBytes) {
        /* Done transmitting */
        return;
    }

    if ((master->txRemainingBytes > 0U) && (master->txData != NULL)) {
        /* Read data from user buffer */
        switch (master->transferSize) {
        case SUPERIO_SPI_TRANSFER_1BYTE:
            data = (uint32_t)(*(const uint8_t *)master->txData);
            break;
        case SUPERIO_SPI_TRANSFER_2BYTE:
            data = (uint32_t)(*(const uint16_t *)master->txData);
            break;
        case SUPERIO_SPI_TRANSFER_4BYTE:
            data = (uint32_t)(*(const uint32_t *)master->txData);
            break;
        default:
            /* Not possible */
            break;
        }
        /* Update tx buffer pointer and remaining bytes count */
        master->txData = &master->txData[master->transferSize];
        master->txRemainingBytes -= (uint32_t)(master->transferSize);
        /* Write data to shifter buffer */
        if (SUPERIO_SPI_TRANSFER_LSB_FIRST == master->bitOrder) {
            SUPERIO_WriteShifterBuffer(baseAddr, TX_SHIFTER(resourceIndex), data, SUPERIO_SHIFTER_RW_MODE_NORMAL);
        } else {
            /* Shift data before bit-swapping it to get the relevant bits in the lower part of the shifter */
            data <<= 32U - (8U * (uint32_t)(master->transferSize));
            SUPERIO_WriteShifterBuffer(baseAddr, TX_SHIFTER(resourceIndex), data, SUPERIO_SHIFTER_RW_MODE_BIT_SWAP);
        }
    } else {
        /* Nothing to send, write dummy data in buffer */
        SUPERIO_WriteShifterBuffer(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_SPI_DUMMYDATA,
                                   SUPERIO_SHIFTER_RW_MODE_NORMAL);
    }
}

/*
 * Function Name : SUPERIO_SPI_MasterCheckStatus
 * Description   : Check status of SPI master transfer. This function can be
 *                 called either in an interrupt routine or directly in polling
 *                 mode to advance the SPI transfer.
 *
 */
static void SUPERIO_SPI_MasterCheckStatus(void *stateStruct)
{
    superio_spi_master_state_t *master;
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    OS_ASSERT(stateStruct != NULL);

    master        = (superio_spi_master_state_t *)stateStruct;
    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;

    /* Check for errors */
    if (SUPERIO_GetShifterErrorStatus(baseAddr, TX_SHIFTER(resourceIndex))) {
        master->status = STATUS_SPI_TX_UNDERRUN;
        /* Force the transfer to stop */
        SUPERIO_SPI_MasterStopTransfer(master);
        /* Call callback to announce the event to the user */
        if (master->callback != NULL) {
            master->callback(master, SPI_EVENT_END_TRANSFER, master->callbackParam);
        }
        return;
    }
    if (SUPERIO_GetShifterErrorStatus(baseAddr, RX_SHIFTER(resourceIndex))) {
        master->status = STATUS_SPI_RX_OVERRUN;
        /* Force the transfer to stop */
        SUPERIO_SPI_MasterStopTransfer(master);
        /* Call callback to announce the event to the user */
        if (master->callback != NULL) {
            master->callback(master, SPI_EVENT_END_TRANSFER, master->callbackParam);
        }
        return;
    }
    /* Check if data was received */
    if (SUPERIO_GetShifterStatus(baseAddr, RX_SHIFTER(resourceIndex))) {
        SUPERIO_SPI_ReadData(master);
    }
    /* Check if transmitter needs more data */
    if (SUPERIO_GetShifterStatus(baseAddr, TX_SHIFTER(resourceIndex))) {
        SUPERIO_SPI_WriteData(master);
        if (0U == master->txRemainingBytes) {
            /* No more data to transmit, disable tx interrupts */
            SUPERIO_SetShifterInterrupt(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), false);
            SUPERIO_SetShifterErrorInterrupt(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), false);
        }
    }

    /* Check there is any data left */
    if ((0U == master->txRemainingBytes) && (0U == master->rxRemainingBytes)) {
        /* Record success if there was no error */
        if (STATUS_BUSY == master->status) {
            master->status = STATUS_SUCCESS;
        }

        /* End transfer */
        SUPERIO_SPI_MasterStopTransfer(master);
        /* Call callback to announce the event to the user */
        if (NULL != master->callback) {
            master->callback(master, SPI_EVENT_END_TRANSFER, master->callbackParam);
        }
    }
}

/*
 * Function Name : SUPERIO_SPI_MasterComputeTxRegAddr
 * Description   : Computes the address of the register used for PDMA tx transfer
 *
 */
static uint32_t SUPERIO_SPI_MasterComputeTxRegAddr(const superio_spi_master_state_t *master)
{
    uint32_t addr;
    const superio_type_t *baseAddr;
    uint8_t shifter;

    baseAddr = g_superioBase[master->superioCommon.instance];
    shifter  = TX_SHIFTER(master->superioCommon.resourceIndex);
    if (master->bitOrder == SUPERIO_SPI_TRANSFER_LSB_FIRST) {
        addr = (uint32_t)(&(baseAddr->SHIFTBUF[shifter]));
    } else {
        addr = (uint32_t)(&(baseAddr->SHIFTBUFBIS[shifter])) + (sizeof(uint32_t) - (uint32_t)master->transferSize);
    }
    return addr;
}

/*
 * Function Name : SUPERIO_SPI_MasterComputeRxRegAddr
 * Description   : Computes the address of the register used for PDMA rx transfer
 *
 */
static uint32_t SUPERIO_SPI_MasterComputeRxRegAddr(const superio_spi_master_state_t *master)
{
    uint32_t addr;
    const superio_type_t *baseAddr;
    uint8_t shifter;

    baseAddr = g_superioBase[master->superioCommon.instance];
    shifter  = RX_SHIFTER(master->superioCommon.resourceIndex);
    if (master->bitOrder == SUPERIO_SPI_TRANSFER_LSB_FIRST) {
        addr = (uint32_t)(&(baseAddr->SHIFTBUF[shifter])) + (sizeof(uint32_t) - (uint32_t)master->transferSize);
    } else {
        addr = (uint32_t)(&(baseAddr->SHIFTBUFBIS[shifter]));
    }

    return addr;
}

/*
 * Function Name : SUPERIO_SPI_MasterStartDmaTransfer
 * Description   : Starts a PDMA transfer
 *
 */
static void SUPERIO_SPI_MasterStartDmaTransfer(superio_spi_master_state_t *master)
{
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)
    superio_type_t *baseAddr;
    uint32_t addr;
    /* Table to map SuperIO_spi transfer sizes to PDMA transfer sizes */
    static const pdma_transfer_size_t dmaTransferSize[SUPERIO_SPI_TRANSFER_4BYTE] = {
        PDMA_TRANSFER_SIZE_1B, PDMA_TRANSFER_SIZE_2B, PDMA_TRANSFER_SIZE_4B, PDMA_TRANSFER_SIZE_4B};

    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;

    if (NULL != master->txData) {
        addr = (uint32_t)(master->txData);
    } else {
        /* if there is no data to transmit, use dummy data as source for PDMA transfer */
        master->dummyDmaData = SUPERIO_SPI_DUMMYDATA;
        addr                 = (uint32_t)(&(master->dummyDmaData));
    }

    /* Configure the transfer control descriptor for the Tx channel */
    (void)PDMA_DRV_ConfigMultiBlockTransfer(
        master->txDMAChannel, PDMA_TRANSFER_MEM2PERIPH, addr, SUPERIO_SPI_MasterComputeTxRegAddr(master),
        dmaTransferSize[(uint32_t)master->transferSize - 1U], (uint32_t)master->transferSize,
        master->txRemainingBytes / (uint32_t)master->transferSize, true);
    if (NULL == master->txData) {
        /* if there is no data to transmit, don't increment source offset */
        PDMA_DRV_SetSrcOffset(master->txDMAChannel, 0);
    }

    if (NULL != master->rxData) {
        addr = (uint32_t)(master->rxData);
    } else {
        /* if there is no data to receive, use dummy data as destination for PDMA transfer */
        addr = (uint32_t)(&(master->dummyDmaData));
    }

    /* Configure the transfer control descriptor for the Rx channel */
    (void)PDMA_DRV_ConfigMultiBlockTransfer(
        master->rxDMAChannel, PDMA_TRANSFER_PERIPH2MEM, SUPERIO_SPI_MasterComputeRxRegAddr(master), addr,
        dmaTransferSize[(uint32_t)master->transferSize - 1U], (uint32_t)master->transferSize,
        master->rxRemainingBytes / (uint32_t)master->transferSize, true);

    if (NULL == master->rxData) {
        /* if there is no data to receive, don't increment destination offset */
        PDMA_DRV_SetDestOffset(master->rxDMAChannel, 0);
    }
    /* Setup callback for PDMA transfer end */
    (void)PDMA_DRV_InstallCallback(master->rxDMAChannel, (pdma_callback_t)(SUPERIO_SPI_MasterEndDmaTransfer),
                                   (void *)(master));

    /* Start both PDMA channels */
    (void)PDMA_DRV_StartChannel(master->txDMAChannel);
    (void)PDMA_DRV_StartChannel(master->rxDMAChannel);

    /* Enable SuperIO PDMA requests for both shifters */
    SUPERIO_SetShifterDMARequest(
        baseAddr, (uint8_t)((1U << TX_SHIFTER(resourceIndex)) | (1U << RX_SHIFTER(resourceIndex))), true);
}

/*
 * Function Name : SUPERIO_SPI_DRV_MasterInit
 * Description   : Initialize the SuperIO_SPI master mode driver
 *
 */
status_t SUPERIO_SPI_DRV_MasterInit(uint32_t instance, const superio_spi_master_user_config_t *userConfigPtr,
                                    superio_spi_master_state_t *master)
{
    status_t retCode;
    uint32_t inputClock;
    status_t clkErr;
    status_t osifError = STATUS_SUCCESS;
    uint8_t dmaReqTx;
    uint8_t dmaReqRx;

    OS_ASSERT(master != NULL);
    OS_ASSERT(instance < SUPERIO_INSTANCE_COUNT);
    /* Check that device was initialized */
    OS_ASSERT(g_superioDeviceStatePtr[instance] != NULL);
    OS_ASSERT((userConfigPtr->transferSize == SUPERIO_SPI_TRANSFER_1BYTE) ||
              (userConfigPtr->transferSize == SUPERIO_SPI_TRANSFER_2BYTE) ||
              (userConfigPtr->transferSize == SUPERIO_SPI_TRANSFER_4BYTE));

    /* Get the protocol clock frequency */
    clkErr = CLOCK_SYS_GetFreq(g_superioClock[instance], &inputClock);
    OS_ASSERT(clkErr == STATUS_SUCCESS);
    OS_ASSERT(inputClock > 0U);

    /* Instruct the resource allocator that we need two shifters/timers */
    master->superioCommon.resourceCount = 2U;
    /* Common SuperIO driver initialization */
    retCode = SUPERIO_DRV_InitDriver(instance, &(master->superioCommon));
    if (STATUS_SUCCESS != retCode) { // Initialization failed, not enough resources
        return retCode;
    }
    /* Initialize the semaphore */
    if (SUPERIO_DRIVER_TYPE_POLLING != userConfigPtr->driverType) {
        master->idleSemaphore = OS_SemaphoreNew(1U, 1U, NULL);
        OS_ASSERT(master->idleSemaphore != NULL);
    }

    /* Initialize driver-specific context structure */
    master->driverType    = userConfigPtr->driverType;
    master->bitOrder      = userConfigPtr->bitOrder;
    master->transferSize  = userConfigPtr->transferSize;
    master->callback      = userConfigPtr->callback;
    master->callbackParam = userConfigPtr->callbackParam;
    master->blocking      = false;
    master->driverIdle    = true;
    master->master        = true;
    master->status        = STATUS_SUCCESS;

    /* Configure device for SPI mode */
    SUPERIO_SPI_MasterConfigure(master, userConfigPtr, inputClock);

    /* Set up transfer engine */
    switch (userConfigPtr->driverType) {
    case SUPERIO_DRIVER_TYPE_INTERRUPTS:
        master->superioCommon.isr = SUPERIO_SPI_MasterCheckStatus;
        break;
    case SUPERIO_DRIVER_TYPE_POLLING:
        /* Nothing to do here, SUPERIO_SPI_DRV_MasterGetStatus() will handle the transfer */
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
 * Function Name : SUPERIO_SPI_DRV_MasterDeinit
 * Description   : De-initialize the SuperIO_SPI master mode driver
 *
 */
status_t SUPERIO_SPI_DRV_MasterDeinit(superio_spi_master_state_t *master)
{
    OS_ASSERT(master != NULL);

    /* Check if driver is busy */
    if (!master->driverIdle) {
        return STATUS_BUSY;
    }

    /* Destroy the semaphore */
    if (SUPERIO_DRIVER_TYPE_POLLING != master->driverType) {
        (void)OS_SemaphoreDelete(master->idleSemaphore);
    }

    return SUPERIO_DRV_DeinitDriver(&(master->superioCommon));
}

/*
 * Function Name : SUPERIO_SPI_DRV_MasterSetBaudRate
 * Description   : Set the baud rate for any subsequent SPI communication
 *
 */
status_t SUPERIO_SPI_DRV_MasterSetBaudRate(superio_spi_master_state_t *master, uint32_t baudRate)
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
    SUPERIO_SPI_MasterComputeBaudRateDivider(baudRate, &divider, inputClock);

    /* Configure timer divider in the lower 8 bits of TIMCMP[CMP] */
    timerCmp = SUPERIO_GetTimerCompare(baseAddr, SCK_TIMER(resourceIndex));
    timerCmp = (uint16_t)((timerCmp & 0xFF00U) | divider);
    SUPERIO_SetTimerCompare(baseAddr, SCK_TIMER(resourceIndex), timerCmp);

    (void)clkErr;
    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_SPI_DRV_MasterGetBaudRate
 * Description   : Get the currently configured baud rate
 *
 */
status_t SUPERIO_SPI_DRV_MasterGetBaudRate(superio_spi_master_state_t *master, uint32_t *baudRate)
{
    const superio_type_t *baseAddr;
    uint16_t divider;
    uint16_t timerCmp;
    uint32_t inputClock;
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
    *baudRate = (inputClock + divider + 1U) / (2U * ((uint32_t)divider + 1U));

    (void)clkErr;
    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_SPI_DRV_MasterTransfer
 * Description   : Perform an SPI master non-blocking transaction
 *
 */
status_t SUPERIO_SPI_DRV_MasterTransfer(superio_spi_master_state_t *master, const uint8_t *txData, uint8_t *rxData,
                                        uint32_t dataSize)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    OS_ASSERT(master != NULL);
    OS_ASSERT((dataSize % (uint32_t)(master->transferSize)) == 0U);
    OS_ASSERT((txData != NULL) || (rxData != NULL));

    baseAddr      = g_superioBase[master->superioCommon.instance];
    resourceIndex = master->superioCommon.resourceIndex;

    /* Check if driver is busy */
    if (!master->driverIdle) {
        return STATUS_BUSY;
    }
    /* Initialize transfer data */
    master->txData           = txData;
    master->rxData           = rxData;
    master->txRemainingBytes = dataSize;
    master->rxRemainingBytes = dataSize;
    master->driverIdle       = false;
    master->status           = STATUS_BUSY;

    /* Enable timers and shifters */
    SUPERIO_SPI_MasterEnableTransfer(master);
    /* Enable transfer engine */
    switch (master->driverType) {
    case SUPERIO_DRIVER_TYPE_INTERRUPTS:
        /* Enable interrupts for Rx and Tx shifters */
        SUPERIO_SetShifterInterrupt(
            baseAddr, (uint8_t)((1U << TX_SHIFTER(resourceIndex)) | (1U << RX_SHIFTER(resourceIndex))), true);
        SUPERIO_SetShifterErrorInterrupt(
            baseAddr, (uint8_t)((1U << TX_SHIFTER(resourceIndex)) | (1U << RX_SHIFTER(resourceIndex))), true);
        break;
    case SUPERIO_DRIVER_TYPE_POLLING:
        /* Call SUPERIO_SPI_MasterCheckStatus once to send the first byte */
        SUPERIO_SPI_MasterCheckStatus(master);
        break;
    case SUPERIO_DRIVER_TYPE_DMA:
        SUPERIO_SPI_MasterStartDmaTransfer(master);
        break;
    default:
        /* Impossible type - do nothing */
        break;
    }

    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_SPI_DRV_MasterTransferBlocking
 * Description   : Perform an SPI master blocking transaction
 *
 */
status_t SUPERIO_SPI_DRV_MasterTransferBlocking(superio_spi_master_state_t *master, const uint8_t *txData,
                                                uint8_t *rxData, uint32_t dataSize, uint32_t timeout)
{
    status_t status;

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

    status = SUPERIO_SPI_DRV_MasterTransfer(master, txData, rxData, dataSize);
    if (STATUS_SUCCESS != status) {
        /* Transfer could not be started */
        master->blocking = false;
        return status;
    }

    /* Wait for transfer to end */
    return SUPERIO_SPI_MasterWaitTransferEnd(master, timeout);
}

/*
 * Function Name : SUPERIO_SPI_DRV_MasterTransferAbort
 * Description   : Aborts a non-blocking SPI master transaction
 *
 */
status_t SUPERIO_SPI_DRV_MasterTransferAbort(superio_spi_master_state_t *master)
{
    OS_ASSERT(master != NULL);

    /* Check if driver is busy */
    if (master->driverIdle) {
        return STATUS_SUCCESS;
    }

    master->status = STATUS_SPI_ABORTED;
    SUPERIO_SPI_MasterStopTransfer(master);

    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_SPI_DRV_MasterGetStatus
 * Description   : Get the status of the current non-blocking SPI master transaction
 *
 */
status_t SUPERIO_SPI_DRV_MasterGetStatus(superio_spi_master_state_t *master, uint32_t *bytesRemaining)
{
    uint32_t remainingBytes;

    OS_ASSERT(master != NULL);

    /* Initialize with the actual remaining bytes count */
    remainingBytes = master->rxRemainingBytes;

    if (!master->driverIdle) {
        switch (master->driverType) {
        case SUPERIO_DRIVER_TYPE_POLLING:
            /* In polling mode advance the SPI transfer here */
            SUPERIO_SPI_MasterCheckStatus(master);
            break;

        case SUPERIO_DRIVER_TYPE_DMA:
            /* In PDMA mode just update the remaining count.
            DO NOT write master->txRemainingBytes directly !!! */
            remainingBytes = PDMA_DRV_GetRemainingMajorIterationsCount(master->rxDMAChannel);
            break;

        default:
            /* Nothing */
            break;
        }
    }

    if (NULL != bytesRemaining) {
        *bytesRemaining = remainingBytes;
    }

    return master->status;
}

/*
 * Function Name : SUPERIO_SPI_DRV_SlaveInit
 * Description   : Initialize the SuperIO_SPI slave mode driver
 *
 */
status_t SUPERIO_SPI_DRV_SlaveInit(uint32_t instance, const superio_spi_slave_user_config_t *userConfigPtr,
                                   superio_spi_slave_state_t *slave)
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
    slave->superioCommon.resourceCount = 2U;
    /* Common SuperIO driver initialization */
    retCode = SUPERIO_DRV_InitDriver(instance, &(slave->superioCommon));
    if (retCode != STATUS_SUCCESS) { // Initialization failed, not enough resources
        return retCode;
    }

    /* Initialize the semaphore */
    if (SUPERIO_DRIVER_TYPE_POLLING != userConfigPtr->driverType) {
        slave->idleSemaphore = OS_SemaphoreNew(1U, 1U, NULL);
        OS_ASSERT(slave->idleSemaphore != NULL);
    }

    /* Initialize driver context structure */
    slave->driverType    = userConfigPtr->driverType;
    slave->bitOrder      = userConfigPtr->bitOrder;
    slave->transferSize  = userConfigPtr->transferSize;
    slave->callback      = userConfigPtr->callback;
    slave->callbackParam = userConfigPtr->callbackParam;
    slave->blocking      = false;
    slave->driverIdle    = true;
    slave->master        = false;
    slave->status        = STATUS_SUCCESS;

    /* Configure device for SPI mode */
    SUPERIO_SPI_SlaveConfigure(slave, userConfigPtr);

    /* Set up transfer engine */
    switch (userConfigPtr->driverType) {
    case SUPERIO_DRIVER_TYPE_INTERRUPTS:
        slave->superioCommon.isr = SUPERIO_SPI_MasterCheckStatus;
        break;
    case SUPERIO_DRIVER_TYPE_POLLING:
        /* Nothing to do here, SUPERIO_SPI_DRV_MasterGetStatus() will handle the transfer */
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
 * Function Name : SUPERIO_SPI_DRV_MasterGetDefaultConfig
 * Description   : Returns default configuration structure for SuperIO_SPI master
 *
 */
void SUPERIO_SPI_DRV_MasterGetDefaultConfig(superio_spi_master_user_config_t *userConfigPtr)
{
    OS_ASSERT(userConfigPtr != NULL);

    userConfigPtr->baudRate      = 5000000U;
    userConfigPtr->driverType    = SUPERIO_DRIVER_TYPE_DMA;
    userConfigPtr->bitOrder      = SUPERIO_SPI_TRANSFER_MSB_FIRST;
    userConfigPtr->transferSize  = SUPERIO_SPI_TRANSFER_1BYTE;
    userConfigPtr->clockPolarity = 0U;
    userConfigPtr->clockPhase    = 0U;
    userConfigPtr->mosiPin       = 0U;
    userConfigPtr->misoPin       = 1U;
    userConfigPtr->sckPin        = 2U;
    userConfigPtr->ssPin         = 3U;
    userConfigPtr->callback      = NULL;
    userConfigPtr->callbackParam = NULL;
    userConfigPtr->rxDMAChannel  = 0U;
    userConfigPtr->txDMAChannel  = 2U;
}

/*
 * Function Name : SUPERIO_SPI_DRV_SlaveGetDefaultConfig
 * Description   : Returns default configuration structure for SuperIO_SPI slave
 *
 */
void SUPERIO_SPI_DRV_SlaveGetDefaultConfig(superio_spi_slave_user_config_t *userConfigPtr)
{
    OS_ASSERT(userConfigPtr != NULL);

    userConfigPtr->driverType    = SUPERIO_DRIVER_TYPE_DMA;
    userConfigPtr->bitOrder      = SUPERIO_SPI_TRANSFER_MSB_FIRST;
    userConfigPtr->transferSize  = SUPERIO_SPI_TRANSFER_1BYTE;
    userConfigPtr->clockPolarity = 0U;
    userConfigPtr->clockPhase    = 0U;
    userConfigPtr->mosiPin       = 4U;
    userConfigPtr->misoPin       = 5U;
    userConfigPtr->sckPin        = 6U;
    userConfigPtr->ssPin         = 7U;
    userConfigPtr->callback      = NULL;
    userConfigPtr->callbackParam = NULL;
    userConfigPtr->rxDMAChannel  = 1U;
    userConfigPtr->txDMAChannel  = 3U;
}

/*
 * Function Name : SUPERIO_SPI_DRV_SlaveDeinit
 * Description   : This function de-initializes the SuperIO_SPI driver in slave mode. The driver can't be used
 * again until reinitialized. The context structure is no longer needed by the driver and
 * can be freed after calling this function.
 *
 */
status_t SUPERIO_SPI_DRV_SlaveDeinit(superio_spi_slave_state_t *slave)
{
    return SUPERIO_SPI_DRV_MasterDeinit(slave);
}

/*
 * Function Name : SUPERIO_SPI_DRV_SlaveTransfer
 * Description   : This function performs an SPI full-duplex transaction, transmit and receive in parallel.
 * If only transmit or receive are required, it is possible to provide NULL pointers for
 * txData or rxData. The transfer is non-blocking, the function only initiates the transfer
 * and then returns, leaving the transfer to complete asynchronously).
 * SUPERIO_SPI_DRV_SlaveGetStatus() can be called to check the status of the transfer.
 *
 */
status_t SUPERIO_SPI_DRV_SlaveTransfer(superio_spi_slave_state_t *slave, const uint8_t *txData, uint8_t *rxData,
                                       uint32_t dataSize)
{
    return SUPERIO_SPI_DRV_MasterTransfer(slave, txData, rxData, dataSize);
}

/*
 * Function Name : SUPERIO_SPI_DRV_SlaveTransferBlocking
 * Description   : This function performs an SPI full-duplex transaction, transmit and receive in parallel.
 * If only transmit or receive are required, it is possible to provide NULL pointers for
 * txData or rxData. The transfer is blocking, the function only returns when the transfer
 * is complete.
 *
 */
status_t SUPERIO_SPI_DRV_SlaveTransferBlocking(superio_spi_slave_state_t *slave, const uint8_t *txData, uint8_t *rxData,
                                               uint32_t dataSize, uint32_t timeout)
{
    return SUPERIO_SPI_DRV_MasterTransferBlocking(slave, txData, rxData, dataSize, timeout);
}

/*
 * Function Name : SUPERIO_SPI_DRV_SlaveTransferAbort
 * Description   : This function aborts a non-blocking SPI transfer.
 *
 */
status_t SUPERIO_SPI_DRV_SlaveTransferAbort(superio_spi_slave_state_t *slave)
{
    return SUPERIO_SPI_DRV_MasterTransferAbort(slave);
}

/*
 * Function Name : SUPERIO_SPI_DRV_SlaveGetStatus
 * Description   : This function returns the current status of a non-blocking SPI slave transaction.
 * A return code of STATUS_BUSY means the transfer is still in progress.
 * Otherwise the function returns a status reflecting the outcome of the last transfer.
 * When the driver is initialized in polling mode this function also advances the transfer
 * by checking and handling the transmit and receive events, so it must be called
 * frequently to avoid overflows or underflows.
 *
 */
status_t SUPERIO_SPI_DRV_SlaveGetStatus(superio_spi_slave_state_t *slave, uint32_t *bytesRemaining)
{
    return SUPERIO_SPI_DRV_MasterGetStatus(slave, bytesRemaining);
}
