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
 * @file superio_uart_driver.c
 * @brief SUPERIO uart driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "superio_uart_driver.h"
#include "clock_driver.h"
#include "superio_common.h"
#include "superio_hw_access.h"

/* Constraints used for baud rate computation */
#define DIVIDER_MIN_VALUE (0)
#define DIVIDER_MAX_VALUE (0xFF)

/* Shifters/Timers used for UART simulation The parameter x represents the
   resourceIndex value for the current driver instance */
#define TX_SHIFTER(x) (x)
#define RX_SHIFTER(x) (x)
#define TX_TIMER(x)   (x)
#define RX_TIMER(x)   (x)

/*
 * Function Name : SUPERIO_UART_ComputeBaudRateDivider
 * Description   : Computes the baud rate divider for a target baud rate
 *
 */
static void SUPERIO_UART_ComputeBaudRateDivider(uint32_t baudRate, uint16_t *divider, uint32_t inputClock)
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
 * Function Name : SuperIO_UART_DRV_Configure
 * Description   : configures the SUPERIO module for UART
 *
 */
static void SUPERIO_UART_ConfigureTx(superio_uart_state_t *state, const superio_uart_user_config_t *userConfigPtr,
                                     uint32_t inputClock)
{
    superio_type_t *baseAddr;
    uint16_t divider;
    uint16_t bits;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)
    baseAddr      = g_superioBase[state->superioCommon.instance];
    resourceIndex = state->superioCommon.resourceIndex;

    /* Compute divider. */
    SUPERIO_UART_ComputeBaudRateDivider(userConfigPtr->baudRate, &divider, inputClock);
    bits = userConfigPtr->bitCount;

    /* Configure tx shifter */
    SUPERIO_SetShifterConfig(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_START_BIT_0,
                             SUPERIO_SHIFTER_STOP_BIT_1, SUPERIO_SHIFTER_SOURCE_PIN);
    SUPERIO_SetShifterControl(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_TRANSMIT,
                              userConfigPtr->dataPin, // Output on tx pin
                              SUPERIO_PIN_POLARITY_HIGH, SUPERIO_PIN_CONFIG_OUTPUT, TX_TIMER(resourceIndex),
                              SUPERIO_TIMER_POLARITY_POSEDGE);

    /* Configure tx timer */
    SUPERIO_SetTimerCompare(baseAddr, TX_TIMER(resourceIndex),
                            (uint16_t)((((uint16_t)(bits << 1U) - 1U) << 8U) + divider));
    SUPERIO_SetTimerConfig(baseAddr, TX_TIMER(resourceIndex), SUPERIO_TIMER_START_BIT_ENABLED,
                           SUPERIO_TIMER_STOP_BIT_TIM_DIS,
                           SUPERIO_TIMER_ENABLE_TRG_HIGH, // Enable when Tx data is available
                           SUPERIO_TIMER_DISABLE_TIM_CMP, SUPERIO_TIMER_RESET_NEVER,
                           SUPERIO_TIMER_DECREMENT_CLK_SHIFT_TMR, // Decrement on SuperIO clock
                           SUPERIO_TIMER_INITOUT_ONE);
    SUPERIO_SetTimerControl(
        baseAddr, TX_TIMER(resourceIndex),
        (uint8_t)((uint8_t)(TX_SHIFTER(resourceIndex) << 2U) + 1U),        // Trigger on tx shifter status flag
        SUPERIO_TRIGGER_POLARITY_LOW, SUPERIO_TRIGGER_SOURCE_INTERNAL, 0U, // Pin unused
        SUPERIO_PIN_POLARITY_HIGH, SUPERIO_PIN_CONFIG_DISABLED, SUPERIO_TIMER_MODE_DISABLED);
}

/*
 * Function Name : SuperIO_UART_DRV_Configure
 * Description   : configures the SUPERIO module for UART
 *
 */
static void SUPERIO_UART_ConfigureRx(superio_uart_state_t *state, const superio_uart_user_config_t *userConfigPtr,
                                     uint32_t inputClock)
{
    superio_type_t *baseAddr;
    uint16_t divider;
    uint16_t bits;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    baseAddr      = g_superioBase[state->superioCommon.instance];
    resourceIndex = state->superioCommon.resourceIndex;

    /* Compute divider. */
    SUPERIO_UART_ComputeBaudRateDivider(userConfigPtr->baudRate, &divider, inputClock);
    bits = userConfigPtr->bitCount;

    /* Configure rx shifter */
    SUPERIO_SetShifterConfig(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_START_BIT_0,
                             SUPERIO_SHIFTER_STOP_BIT_1, SUPERIO_SHIFTER_SOURCE_PIN);
    SUPERIO_SetShifterControl(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_DISABLED,
                              userConfigPtr->dataPin, // Input from rx pin
                              SUPERIO_PIN_POLARITY_HIGH, SUPERIO_PIN_CONFIG_DISABLED, RX_TIMER(resourceIndex),
                              SUPERIO_TIMER_POLARITY_NEGEDGE);

    /* Configure rx timer */
    SUPERIO_SetTimerCompare(baseAddr, RX_TIMER(resourceIndex),
                            (uint16_t)((((uint16_t)(bits << 1U) - 1U) << 8U) + divider));
    SUPERIO_SetTimerConfig(baseAddr, RX_TIMER(resourceIndex), SUPERIO_TIMER_START_BIT_ENABLED,
                           SUPERIO_TIMER_STOP_BIT_TIM_DIS,
                           SUPERIO_TIMER_ENABLE_PIN_POSEDGE, // Enable when data is available
                           SUPERIO_TIMER_DISABLE_TIM_CMP, SUPERIO_TIMER_RESET_PIN_RISING,
                           SUPERIO_TIMER_DECREMENT_CLK_SHIFT_TMR, // Decrement on SuperIO clock
                           SUPERIO_TIMER_INITOUT_ONE_RESET);
    SUPERIO_SetTimerControl(baseAddr, RX_TIMER(resourceIndex), 0U, // Trigger unused
                            SUPERIO_TRIGGER_POLARITY_HIGH, SUPERIO_TRIGGER_SOURCE_EXTERNAL,
                            userConfigPtr->dataPin, // Input from rx pin
                            SUPERIO_PIN_POLARITY_LOW, SUPERIO_PIN_CONFIG_DISABLED, SUPERIO_TIMER_MODE_DISABLED);
}

/*
 * Function Name : SUPERIO_UART_EndTransfer
 * Description   : End the current transfer
 *
 */
static void SUPERIO_UART_EndTransfer(superio_uart_state_t *state)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    baseAddr      = g_superioBase[state->superioCommon.instance];
    resourceIndex = state->superioCommon.resourceIndex;

    /* Disable transfer engine */
    switch (state->driverType) {
    case SUPERIO_DRIVER_TYPE_INTERRUPTS:
        /* Disable interrupts for Rx / Tx shifter */
        SUPERIO_SetShifterInterrupt(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), false);
        SUPERIO_SetShifterErrorInterrupt(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), false);
        /* Disable timer interrupt  */
        SUPERIO_SetTimerInterrupt(baseAddr, (uint8_t)(1U << TX_TIMER(resourceIndex)), false);
        break;
    case SUPERIO_DRIVER_TYPE_POLLING:
        /* Nothing to do here */
        break;
    case SUPERIO_DRIVER_TYPE_DMA:
        /* For Tx we need to disable timer interrupt */
        SUPERIO_SetTimerInterrupt(baseAddr, (uint8_t)(1U << TX_TIMER(resourceIndex)), false);
        /* Stop PDMA channels */
        (void)PDMA_DRV_StopChannel(state->dmaChannel);
        /* Disable the SuperIO PDMA request */
        SUPERIO_SetShifterDMARequest(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), false);
        break;
    default:
        /* Impossible type - do nothing */
        break;
    }

    state->remainingBytes = 0U;
    state->driverIdle     = true;
    /* Signal transfer end for blocking transfers */
    if (true == state->blocking) {
        (void)OS_SemaphoreRelease(state->idleSemaphore);
    }
}

/*
 * Function Name : SUPERIO_UART_EnableTransfer
 * Description   : Enables timers and shifters to start a transfer
 *
 */
static void SUPERIO_UART_EnableTransfer(superio_uart_state_t *state)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    resourceIndex = state->superioCommon.resourceIndex;
    baseAddr      = g_superioBase[state->superioCommon.instance];

    /* Enable timers and shifters */
    if (SUPERIO_UART_DIRECTION_RX == state->direction) {
        /* In rx mode, discard any leftover rx. data */
        SUPERIO_ClearShifterStatus(baseAddr, RX_SHIFTER(resourceIndex));
        SUPERIO_SetShifterMode(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_RECEIVE);
    }
    SUPERIO_SetTimerMode(baseAddr, TX_TIMER(resourceIndex), SUPERIO_TIMER_MODE_8BIT_BAUD);
}

/*
 * Function Name : SUPERIO_UART_StopTransfer
 * Description   : Forcefully stops the current transfer
 *
 */
static void SUPERIO_UART_StopTransfer(superio_uart_state_t *state)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    resourceIndex = state->superioCommon.resourceIndex;
    baseAddr      = g_superioBase[state->superioCommon.instance];

    /* Disable and re-enable timers and shifters to reset them */
    SUPERIO_SetTimerMode(baseAddr, TX_TIMER(resourceIndex), SUPERIO_TIMER_MODE_DISABLED);
    SUPERIO_SetShifterMode(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_DISABLED);

    /* Clear any leftover error flags */
    SUPERIO_ClearShifterErrorStatus(baseAddr, TX_SHIFTER(resourceIndex));

    /* End the transfer */
    SUPERIO_UART_EndTransfer(state);

    /* Re-enable shifter for Tx, to ensure correct idle state */
    if (SUPERIO_UART_DIRECTION_TX == state->direction) {
        /* In tx mode restore start bit in case it was changed for end of transmission detection */
        SUPERIO_SetShifterStartBit(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_START_BIT_0);
        SUPERIO_SetShifterMode(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_MODE_TRANSMIT);
    }
}

/*
 * Function Name : SUPERIO_UART_WaitTransferEnd
 * Description   : waits for the end of a blocking transfer
 *
 */
static status_t SUPERIO_UART_WaitTransferEnd(superio_uart_state_t *state, uint32_t timeout)
{
    OS_Status_t osifError = OS_OK;

    switch (state->driverType) {
    case SUPERIO_DRIVER_TYPE_INTERRUPTS:
        /* Wait for transfer to be completed by the IRQ */
        osifError = OS_SemaphoreAcquire(state->idleSemaphore, timeout);
        break;
    case SUPERIO_DRIVER_TYPE_POLLING:
        /* Call SUPERIO_UART_DRV_GetStatus() to do the transfer */
        while (SUPERIO_UART_DRV_GetStatus(state, NULL) == STATUS_BUSY) {
        }
        break;
    case SUPERIO_DRIVER_TYPE_DMA:
        /* Wait for transfer completion to be signaled by the PDMA or IRQ */
        osifError = OS_SemaphoreAcquire(state->idleSemaphore, timeout);
        break;
    default:
        /* Impossible type - do nothing */
        break;
    }

    /* Blocking transfer is over */
    state->blocking = false;
    if (OS_ErrorTimeout == osifError) {
        /* Abort current transfer */
        state->status = STATUS_TIMEOUT;
        SUPERIO_UART_StopTransfer(state);
    }

    return state->status;
}

/*
 * Function Name : SUPERIO_UART_ReadData
 * Description   : reads data received by the module
 *
 */
static void SUPERIO_UART_ReadData(superio_uart_state_t *state)
{
    const superio_type_t *baseAddr;
    uint32_t data;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    OS_ASSERT(state->remainingBytes > 0U);
    OS_ASSERT(state->rxData != NULL);

    baseAddr      = g_superioBase[state->superioCommon.instance];
    resourceIndex = state->superioCommon.resourceIndex;

    /* Read data from shifter buffer */
    data = SUPERIO_ReadShifterBuffer(baseAddr, RX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_RW_MODE_NORMAL);
    data >>= 32U - (uint32_t)(state->bitCount);

    if (state->bitCount <= 8U) {
        *(uint8_t *)state->rxData = (uint8_t)data;
        /* Update rx buffer pointer and remaining bytes count */
        state->rxData++;
        state->remainingBytes -= 1U;
    } else {
        /* For more than 8 bits per word 2 bytes are needed */
        *(uint16_t *)state->rxData = (uint16_t)data;
        /* Update rx buffer pointer and remaining bytes count */
        state->rxData = &state->rxData[2U];
        state->remainingBytes -= 2U;
    }
}

/*
 * Function Name : SUPERIO_UART_WriteData
 * Description   : writes data to be transmitted by the module
 *
 */
static void SUPERIO_UART_WriteData(superio_uart_state_t *state)
{
    superio_type_t *baseAddr;
    uint32_t data;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    OS_ASSERT(state->txData != NULL);

    baseAddr      = g_superioBase[state->superioCommon.instance];
    resourceIndex = state->superioCommon.resourceIndex;

    /* Read data from user buffer and update tx buffer pointer and remaining bytes count */
    if (state->bitCount <= 8U) {
        data = (uint32_t)(*(uint8_t *)state->txData);
        state->txData++;
        state->remainingBytes -= 1U;
    } else {
        /* For more than 8 bits per word 2 bytes are needed */
        data          = (uint32_t)(*(uint16_t *)state->txData);
        state->txData = &state->txData[2U];
        state->remainingBytes -= 2U;
    }

    SUPERIO_WriteShifterBuffer(baseAddr, TX_SHIFTER(resourceIndex), data, SUPERIO_SHIFTER_RW_MODE_NORMAL);
}

/*
 * Function Name : SUPERIO_UART_CheckStatusTx
 * Description   : Check status of the UART transmission. This function can be
 *                 called either in an interrupt routine or directly in polling
 *                 mode to advance the transfer.
 *
 */
static void SUPERIO_UART_CheckStatusTx(void *stateStruct)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)
    superio_uart_state_t *state;

    OS_ASSERT(stateStruct != NULL);

    state         = (superio_uart_state_t *)stateStruct;
    baseAddr      = g_superioBase[state->superioCommon.instance];
    resourceIndex = state->superioCommon.resourceIndex;

    /* No need to check for Tx underflow since timer is controlled by the shifter status flag */
    /* Check for transfer end */
    if (0U == state->remainingBytes) {
        if (SUPERIO_GetTimerStatus(baseAddr, TX_TIMER(resourceIndex))) {
            /* Clear timer status */
            SUPERIO_ClearTimerStatus(baseAddr, TX_TIMER(resourceIndex));
            state->txFlush--;
            if (0U == state->txFlush) {
                /* Done flushing the Tx buffer, end transfer */
                /* Record success if there was no error */
                if (STATUS_BUSY == state->status) {
                    state->status = STATUS_SUCCESS;
                }
                SUPERIO_UART_StopTransfer(state);
                /* Call callback to announce the end transfer event to the user */
                if (NULL != state->callback) {
                    state->callback(state, UART_EVENT_END_TRANSFER, state->callbackParam);
                }
            } else if (SUPERIO_GetShifterStatus(baseAddr, TX_SHIFTER(resourceIndex))) {
                /* txFlush == 1, but last byte was already transferred from buffer to shifter. There is a
                   danger that the transmission is over and we end up never reporting the end event.
                   To avoid this, send one extra dummy byte */
                /* Set start bit to 1 and send an 0xFF byte, this way the line will appear idle */
                SUPERIO_SetShifterStartBit(baseAddr, TX_SHIFTER(resourceIndex), SUPERIO_SHIFTER_START_BIT_1);
                SUPERIO_WriteShifterBuffer(baseAddr, TX_SHIFTER(resourceIndex), 0xFFFFFFFFUL,
                                           SUPERIO_SHIFTER_RW_MODE_NORMAL);
            } else {
                /* txFlush == 1, and last byte was not yet transferred from buffer to shifter.
                   No need to do anything, just wait for the next timer event. */
            }
        }
    }
    /* Check if transmitter needs more data */
    else if (SUPERIO_GetShifterStatus(baseAddr, TX_SHIFTER(resourceIndex)) && (state->remainingBytes > 0U)) {
        SUPERIO_UART_WriteData(state);
        if (0U == state->remainingBytes) {
            /* Out of data, call callback to allow user to provide a new buffer */
            if (NULL != state->callback) {
                state->callback(state, UART_EVENT_TX_EMPTY, state->callbackParam);
            }
        }
        if (0U == state->remainingBytes) {
            /* No more data, transmission will stop after the last bytes are sent.
               The timer event will indicate when the send is complete */
            /* Clear any previous timer events */
            SUPERIO_ClearTimerStatus(baseAddr, TX_TIMER(resourceIndex));
            if (SUPERIO_DRIVER_TYPE_INTERRUPTS == state->driverType) {
                /* Transmission completed; disable interrupt */
                SUPERIO_SetShifterInterrupt(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), false);
                /* Enable timer interrupt to ensure that transfer is completed */
                SUPERIO_SetTimerInterrupt(baseAddr, (uint8_t)(1U << TX_TIMER(resourceIndex)), true);
            }
        }
    } else {
        /* No relevant events - nothing to do */
    }
}

/*
 * Function Name : SUPERIO_UART_CheckStatusRx
 * Description   : Check status of the UART reception. This function can be
 *                 called either in an interrupt routine or directly in polling
 *                 mode to advance the transfer.
 *
 */
static void SUPERIO_UART_CheckStatusRx(void *stateStruct)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)
    superio_uart_state_t *state;

    OS_ASSERT(stateStruct != NULL);

    state         = (superio_uart_state_t *)stateStruct;
    baseAddr      = g_superioBase[state->superioCommon.instance];
    resourceIndex = state->superioCommon.resourceIndex;

    /* Check for errors */
    if (SUPERIO_GetShifterErrorStatus(baseAddr, RX_SHIFTER(resourceIndex))) {
        state->status = STATUS_UART_RX_OVERRUN;
        SUPERIO_ClearShifterErrorStatus(baseAddr, RX_SHIFTER(resourceIndex));
        state->remainingBytes = 0U;
        /* Continue processing events */
    }
    /* Check if data was received */
    else if (SUPERIO_GetShifterStatus(baseAddr, RX_SHIFTER(resourceIndex))) {
        SUPERIO_UART_ReadData(state);
        if (state->remainingBytes == 0U) {
            /* Out of data, call callback to allow user to provide a new buffer */
            if (state->callback != NULL) {
                state->callback(state, UART_EVENT_RX_FULL, state->callbackParam);
            }
        }
    } else {
        /* No events - nothing to do */
    }
    /* Check if transfer is over */
    if (0U == state->remainingBytes) {
        /* Record success if there was no error */
        if (STATUS_BUSY == state->status) {
            state->status = STATUS_SUCCESS;
        }
        /* Discard any leftover rx. data */
        SUPERIO_ClearShifterStatus(baseAddr, RX_SHIFTER(resourceIndex));
        /* End transfer */
        SUPERIO_UART_StopTransfer(state);
        /* Call callback to announce the event to the user */
        if (NULL != state->callback) {
            state->callback(state, UART_EVENT_END_TRANSFER, state->callbackParam);
        }
    }
}

/*
 * Function Name : SUPERIO_UART_CheckStatus
 * Description   : Check status of the UART transfer. This function can be
 *                 called either in an interrupt routine or directly in polling
 *                 mode to advance the transfer.
 *
 */
static void SUPERIO_UART_CheckStatus(void *stateStruct)
{
    const superio_uart_state_t *state;

    OS_ASSERT(stateStruct != NULL);

    state = (superio_uart_state_t *)stateStruct;
    if (SUPERIO_UART_DIRECTION_TX == state->direction) {
        SUPERIO_UART_CheckStatusTx(stateStruct);
    } else {
        SUPERIO_UART_CheckStatusRx(stateStruct);
    }
}

/*
 * Function Name : SUPERIO_UART_EndDmaTxTransfer
 * Description   : function called at the end of a PDMA Tx transfer
 *
 */
static void SUPERIO_UART_EndDmaTxTransfer(void *stateStruct, pdma_chn_status_t status)
{
    superio_uart_state_t *state;
    uint8_t dmaChn;
    uint32_t byteCount;
    superio_type_t *baseAddr;

    OS_ASSERT(stateStruct != NULL);

    state    = (superio_uart_state_t *)stateStruct;
    baseAddr = g_superioBase[state->superioCommon.instance];

    if (PDMA_CHN_ERROR == status) {
        /* PDMA error, stop transfer */
        state->status = STATUS_ERROR;
        SUPERIO_UART_StopTransfer(state);
        /* Call callback to announce the end transfer event to the user */
        if (NULL != state->callback) {
            state->callback(state, UART_EVENT_END_TRANSFER, state->callbackParam);
        }
    } else {
        /* Call callback to allow user to provide a new buffer */
        if (NULL != state->callback) {
            state->callback(state, UART_EVENT_TX_EMPTY, state->callbackParam);
        }
        if (0U == state->remainingBytes) {
            /* No more data to transmit, transmission will stop */
            /* Enable timer interrupt to let IRQ ensure that transfer is completed */
            SUPERIO_ClearTimerStatus(baseAddr, TX_TIMER(state->superioCommon.resourceIndex));
            SUPERIO_SetTimerInterrupt(baseAddr, (uint8_t)(1U << TX_TIMER(state->superioCommon.resourceIndex)), true);
        } else {
            /* There is more data to transfer, restart PDMA channel */
            /* Update buffer address and size */
            dmaChn = state->dmaChannel;
            if (state->bitCount <= 8U) {
                byteCount = 1U;
            } else {
                byteCount = 2U;
            }
            PDMA_DRV_SetSrcAddr(dmaChn, (uint32_t)(state->txData));
            PDMA_DRV_SetMajorLoopIterationCount(dmaChn, state->remainingBytes / byteCount);
            /* Now that this tx is set up, clear remaining bytes count */
            state->remainingBytes = 0U;
            /* Start the channel */
            (void)PDMA_DRV_StartChannel(dmaChn);
        }
    }
}

/*
 * Function Name : SUPERIO_UART_EndDmaRxTransfer
 * Description   : function called at the end of a PDMA Rx transfer
 *
 */
static void SUPERIO_UART_EndDmaRxTransfer(void *stateStruct, pdma_chn_status_t status)
{
    superio_uart_state_t *state;
    uint8_t dmaChn;
    uint32_t byteCount;

    OS_ASSERT(stateStruct != NULL);

    state = (superio_uart_state_t *)stateStruct;

    if (PDMA_CHN_ERROR == status) {
        /* PDMA error, stop transfer */
        state->status = STATUS_ERROR;
    } else {
        /* Call callback to allow user to provide a new buffer */
        if (NULL != state->callback) {
            state->callback(state, UART_EVENT_RX_FULL, state->callbackParam);
        }
    }
    if (0U == state->remainingBytes) {
        /* No more data to transmit, reception will stop */
        if (state->status == STATUS_BUSY) {
            state->status = STATUS_SUCCESS;
        }
        SUPERIO_UART_StopTransfer(state);
        /* Call callback to announce the event to the user */
        if (NULL != state->callback) {
            state->callback(state, UART_EVENT_END_TRANSFER, state->callbackParam);
        }
    } else {
        /* There is more data to transfer, restart PDMA channel */
        /* Update buffer address and size */
        dmaChn = state->dmaChannel;
        if (state->bitCount <= 8U) {
            byteCount = 1U;
        } else {
            byteCount = 2U;
        }
        PDMA_DRV_SetDestAddr(dmaChn, (uint32_t)(state->rxData));
        PDMA_DRV_SetMajorLoopIterationCount(dmaChn, state->remainingBytes / byteCount);
        /* Now that this tx is set up, clear remaining bytes count */
        state->remainingBytes = 0U;
        /* Start the channel */
        (void)PDMA_DRV_StartChannel(dmaChn);
    }
}

/*
 * Function Name : SUPERIO_UART_ComputeTxRegAddr
 * Description   : Computes the address of the register used for PDMA tx transfer
 *
 */
static uint32_t SUPERIO_UART_ComputeTxRegAddr(const superio_uart_state_t *state)
{
    uint32_t addr;
    const superio_type_t *baseAddr;
    uint8_t shifter;

    baseAddr = g_superioBase[state->superioCommon.instance];
    shifter  = TX_SHIFTER(state->superioCommon.resourceIndex);
    addr     = (uint32_t)(&(baseAddr->SHIFTBUF[shifter]));
    return addr;
}

/*
 * Function Name : SUPERIO_UART_ComputeRxRegAddr
 * Description   : Computes the address of the register used for PDMA rx transfer
 *
 */
static uint32_t SUPERIO_UART_ComputeRxRegAddr(const superio_uart_state_t *state)
{
    uint32_t addr;
    const superio_type_t *baseAddr;
    uint8_t shifter;
    uint32_t byteCount;

    if (state->bitCount <= 8U) {
        byteCount = 1U;
    } else {
        byteCount = 2U;
    }
    baseAddr = g_superioBase[state->superioCommon.instance];
    shifter  = RX_SHIFTER(state->superioCommon.resourceIndex);
    addr     = (uint32_t)(&(baseAddr->SHIFTBUF[shifter])) + (sizeof(uint32_t) - byteCount);
    return addr;
}

/*
 * Function Name : SUPERIO_UART_StartTxDmaTransfer
 * Description   : Starts a Tx PDMA transfer
 *
 */
static void SUPERIO_UART_StartTxDmaTransfer(superio_uart_state_t *state)
{
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)
    superio_type_t *baseAddr;
    pdma_transfer_size_t dmaTransferSize;
    uint32_t byteCount;

    baseAddr      = g_superioBase[state->superioCommon.instance];
    resourceIndex = state->superioCommon.resourceIndex;

    /* Configure the transfer control descriptor for the previously allocated channel */
    if (state->bitCount <= 8U) {
        dmaTransferSize = PDMA_TRANSFER_SIZE_1B;
        byteCount       = 1U;
    } else {
        dmaTransferSize = PDMA_TRANSFER_SIZE_2B;
        byteCount       = 2U;
    }
    (void)PDMA_DRV_ConfigMultiBlockTransfer(state->dmaChannel, PDMA_TRANSFER_MEM2PERIPH, (uint32_t)(state->txData),
                                            SUPERIO_UART_ComputeTxRegAddr(state), dmaTransferSize, byteCount,
                                            state->remainingBytes / byteCount, true);

    /* Now that this transfer is set up, clear remaining bytes count */
    state->remainingBytes = 0U;

    /* Setup callback for PDMA tx transfer end */
    (void)PDMA_DRV_InstallCallback(state->dmaChannel, (pdma_callback_t)(SUPERIO_UART_EndDmaTxTransfer),
                                   (void *)(state));
    /* Start tx PDMA channel */
    (void)PDMA_DRV_StartChannel(state->dmaChannel);

    /* Enable SuperIO PDMA requests */
    SUPERIO_SetShifterDMARequest(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), true);
}

/*
 * Function Name : SUPERIO_UART_StartRxDmaTransfer
 * Description   : Starts an Rx PDMA transfer
 *
 */
static void SUPERIO_UART_StartRxDmaTransfer(superio_uart_state_t *state)
{
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)
    superio_type_t *baseAddr;
    pdma_transfer_size_t dmaTransferSize;
    uint32_t byteCount;

    baseAddr      = g_superioBase[state->superioCommon.instance];
    resourceIndex = state->superioCommon.resourceIndex;

    /* Configure the transfer control descriptor for the previously allocated channel */
    if (state->bitCount <= 8U) {
        dmaTransferSize = PDMA_TRANSFER_SIZE_1B;
        byteCount       = 1U;
    } else {
        dmaTransferSize = PDMA_TRANSFER_SIZE_2B;
        byteCount       = 2U;
    }
    (void)PDMA_DRV_ConfigMultiBlockTransfer(state->dmaChannel, PDMA_TRANSFER_PERIPH2MEM,
                                            SUPERIO_UART_ComputeRxRegAddr(state), (uint32_t)(state->rxData),
                                            dmaTransferSize, byteCount, state->remainingBytes / byteCount, true);

    /* Now that this transfer is set up, clear remaining bytes count */
    state->remainingBytes = 0U;

    /* Setup callback for PDMA tx transfer end */
    (void)PDMA_DRV_InstallCallback(state->dmaChannel, (pdma_callback_t)(SUPERIO_UART_EndDmaRxTransfer),
                                   (void *)(state));
    /* Start tx PDMA channel */
    (void)PDMA_DRV_StartChannel(state->dmaChannel);

    /* Enable SuperIO PDMA requests */
    SUPERIO_SetShifterDMARequest(baseAddr, (uint8_t)(1U << RX_SHIFTER(resourceIndex)), true);
}

/*
 * Function Name : SUPERIO_UART_DRV_Init
 * Description   : Initialize the SuperIO_UART driver
 *
 */
status_t SUPERIO_UART_DRV_Init(uint32_t instance, const superio_uart_user_config_t *userConfigPtr,
                               superio_uart_state_t *state)
{
    uint32_t inputClock;
    status_t clkErr;
    status_t retCode;
    status_t osifError = STATUS_SUCCESS;
    uint8_t dmaReq;

    OS_ASSERT(state != NULL);
    OS_ASSERT(instance < SUPERIO_INSTANCE_COUNT);
    /* Check that device was initialized */
    OS_ASSERT(g_superioDeviceStatePtr[instance] != NULL);

    /* Get the protocol clock frequency */
    clkErr = CLOCK_SYS_GetFreq(g_superioClock[instance], &inputClock);
    OS_ASSERT(clkErr == STATUS_SUCCESS);
    OS_ASSERT(inputClock > 0U);

    /* Instruct the resource allocator that we need one shifter/timer */
    state->superioCommon.resourceCount = 1U;
    /* Common SuperIO driver initialization */
    retCode = SUPERIO_DRV_InitDriver(instance, (superio_common_state_t *)state);
    if (retCode != STATUS_SUCCESS) { /* Initialization failed, not enough resources */
        return retCode;
    }
    /* Initialize the semaphore */
    if (userConfigPtr->driverType != SUPERIO_DRIVER_TYPE_POLLING) {
        state->idleSemaphore = OS_SemaphoreNew(1U, 1U, NULL);
        OS_ASSERT(state->idleSemaphore != NULL);
    }

    /* Initialize driver-specific context structure */
    state->rxData         = NULL;
    state->txData         = NULL;
    state->remainingBytes = 0U;
    state->callback       = userConfigPtr->callback;
    state->callbackParam  = userConfigPtr->callbackParam;
    state->blocking       = false;
    state->driverType     = userConfigPtr->driverType;
    state->direction      = userConfigPtr->direction;
    state->status         = STATUS_SUCCESS;
    state->driverIdle     = true;
    state->bitCount       = userConfigPtr->bitCount;

    if (SUPERIO_UART_DIRECTION_TX == state->direction) {
        /* Configure device for UART Tx mode */
        SUPERIO_UART_ConfigureTx(state, userConfigPtr, inputClock);
    } else {
        /* Configure device for UART Rx mode */
        SUPERIO_UART_ConfigureRx(state, userConfigPtr, inputClock);
    }

    /* Set up transfer engine */
    switch (state->driverType) {
    case SUPERIO_DRIVER_TYPE_INTERRUPTS:
        if (SUPERIO_UART_DIRECTION_TX == state->direction) {
            state->superioCommon.isr = SUPERIO_UART_CheckStatusTx;
        } else {
            state->superioCommon.isr = SUPERIO_UART_CheckStatusRx;
        }
        break;
    case SUPERIO_DRIVER_TYPE_POLLING:
        /* Nothing to do here, SUPERIO_UART_DRV_GetStatus() will handle the transfer */
        break;
    case SUPERIO_DRIVER_TYPE_DMA:
        /* Store PDMA channel number */
        state->dmaChannel = userConfigPtr->dmaChannel;
        /* Configure PDMA request source */
        dmaReq = g_superioDMASrc[instance][TX_SHIFTER(state->superioCommon.resourceIndex)];
        (void)PDMA_DRV_SetChannelRequestAndTrigger(userConfigPtr->dmaChannel, dmaReq, false);
        /* For Tx we will still need interrupt to signal end of transfer */
        if (SUPERIO_UART_DIRECTION_TX == state->direction) {
            state->superioCommon.isr = SUPERIO_UART_CheckStatusTx;
        }
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
 * Function Name : SUPERIO_UART_DRV_Deinit
 * Description   : De-initialize the SuperIO_UART driver
 *
 */
status_t SUPERIO_UART_DRV_Deinit(superio_uart_state_t *state)
{
    OS_ASSERT(state != NULL);

    /* Check if driver is busy */
    if (!state->driverIdle) {
        return STATUS_BUSY;
    }

    /* Destroy the semaphore */
    if (SUPERIO_DRIVER_TYPE_POLLING != state->driverType) {
        (void)OS_SemaphoreDelete(state->idleSemaphore);
    }

    return SUPERIO_DRV_DeinitDriver((superio_common_state_t *)state);
}

/*
 * Function Name : SUPERIO_DRV_TcsrStopZoneMode
 * Description   : Enter/exit stop mode according the TCSR
 *
 */
void SUPERIO_UART_DRV_TcsrStopZoneMode(void *state, bool en)
{
    OS_ASSERT(state != NULL);

    uint32_t stop_enable = 1 << TCSR_STOP_SUPERIO_SHIFT;
    // uint32_t stopSel_enable = 1 << TCSR_STOPMODE_SUPERIO_SHIFT;

    tcsr_type_t *tcsr = (tcsr_type_t *)TCSR_BASEADDR;
    uint32_t val_stop = tcsr->STOP;
    // uint32_t val_stopMode = tcsr->STOPMODE;

    if (en) {
        val_stop = val_stop | stop_enable;
        // val_stopMode = val_stopMode | stopSel_enable;
    } else {
        val_stop = val_stop & (~stop_enable);
        // val_stopMode = val_stopMode & (~stopSel_enable);
    }

    tcsr->STOP = val_stop;
    // tcsr->STOPMODE = val_stopMode;
}

/*
 * Function Name : SUPERIO_UART_DRV_SetConfig
 * Description   : Set the baud rate and bit width for any subsequent UART transfer
 *
 */
status_t SUPERIO_UART_DRV_SetConfig(superio_uart_state_t *state, uint32_t baudRate, uint8_t bitCount)
{
    superio_type_t *baseAddr;
    uint16_t divider;
    uint32_t inputClock;
    status_t clkErr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    OS_ASSERT(state != NULL);
    OS_ASSERT(baudRate > 0U);
    OS_ASSERT(bitCount > 0U);
    OS_ASSERT(bitCount <= 16U);
    /* For PDMA transfers bitCount must 8 */
    OS_ASSERT(!((state->driverType == SUPERIO_DRIVER_TYPE_DMA) && (bitCount != 8U)));

    baseAddr      = g_superioBase[state->superioCommon.instance];
    resourceIndex = state->superioCommon.resourceIndex;

    /* Check if driver is busy */
    if (!state->driverIdle) {
        return STATUS_BUSY;
    }
    /* Get the protocol clock frequency */
    clkErr = CLOCK_SYS_GetFreq(g_superioClock[state->superioCommon.instance], &inputClock);
    OS_ASSERT(clkErr == STATUS_SUCCESS);
    OS_ASSERT(inputClock > 0U);

    /* Compute divider */
    SUPERIO_UART_ComputeBaudRateDivider(baudRate, &divider, inputClock);

    /* Configure tx/rx timer */
    SUPERIO_SetTimerCompare(baseAddr, TX_TIMER(resourceIndex),
                            (uint16_t)((uint16_t)(((uint16_t)((uint16_t)bitCount << 1U) - 1U) << 8U) + divider));

    state->bitCount = bitCount;

    (void)clkErr;
    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_UART_DRV_GetBaudRate
 * Description   : Get the currently configured baud rate
 *
 */
status_t SUPERIO_UART_DRV_GetBaudRate(superio_uart_state_t *state, uint32_t *baudRate)
{
    const superio_type_t *baseAddr;
    uint32_t inputClock;
    uint16_t divider;
    uint16_t timerCmp;
    status_t clkErr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    OS_ASSERT(state != NULL);
    OS_ASSERT(baudRate != NULL);

    baseAddr      = g_superioBase[state->superioCommon.instance];
    resourceIndex = state->superioCommon.resourceIndex;

    /* Get the protocol clock frequency */
    clkErr = CLOCK_SYS_GetFreq(g_superioClock[state->superioCommon.instance], &inputClock);
    OS_ASSERT(clkErr == STATUS_SUCCESS);
    OS_ASSERT(inputClock > 0U);

    /* Get the currently configured divider */
    timerCmp = SUPERIO_GetTimerCompare(baseAddr, TX_TIMER(resourceIndex));
    divider  = (uint16_t)(timerCmp & 0x00FFU);

    /* Compute baud rate: input_clock / (2 * (divider + 1)). Round to nearest integer */
    *baudRate = (inputClock + (uint32_t)divider + 1U) / (2U * ((uint32_t)divider + 1U));

    (void)clkErr;
    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_UART_DRV_SendData
 * Description   : Perform a non-blocking UART transmission
 *
 */
status_t SUPERIO_UART_DRV_SendData(superio_uart_state_t *state, const uint8_t *txBuff, uint32_t txSize)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    OS_ASSERT(state != NULL);
    OS_ASSERT(txBuff != NULL);
    OS_ASSERT(txSize > 0U);
    OS_ASSERT(state->direction == SUPERIO_UART_DIRECTION_TX);
    /* If 2 bytes per word are needed, then the size must be even */
    OS_ASSERT((state->bitCount <= 8U) || ((txSize & 1U) == 0U));

    baseAddr      = g_superioBase[state->superioCommon.instance];
    resourceIndex = state->superioCommon.resourceIndex;

    /* Check if driver is busy */
    if (!state->driverIdle) {
        return STATUS_BUSY;
    }

    state->txData         = (uint8_t *)txBuff;
    state->remainingBytes = txSize;
    state->status         = STATUS_BUSY;
    state->driverIdle     = false;
    /* Number of bytes to flush after the last byte is copied in the tx shifter buffer */
    state->txFlush = (uint8_t)((txSize == 1U) ? 1U : 2U);

    /* Enable timers and shifters */
    SUPERIO_UART_EnableTransfer(state);
    /* Enable transfer engine */
    switch (state->driverType) {
    case SUPERIO_DRIVER_TYPE_INTERRUPTS:
        /* Enable interrupts for Tx shifter */
        SUPERIO_SetShifterInterrupt(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), true);
        SUPERIO_SetShifterErrorInterrupt(baseAddr, (uint8_t)(1U << TX_SHIFTER(resourceIndex)), true);
        break;
    case SUPERIO_DRIVER_TYPE_POLLING:
        /* Call SUPERIO_UART_CheckStatus once to send the first byte */
        SUPERIO_UART_CheckStatus(state);
        break;
    case SUPERIO_DRIVER_TYPE_DMA:
        SUPERIO_UART_StartTxDmaTransfer(state);
        break;
    default:
        /* Impossible type - do nothing */
        break;
    }

    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_UART_DRV_SendDataBlocking
 * Description   : Perform a blocking UART transmission
 *
 */
status_t SUPERIO_UART_DRV_SendDataBlocking(superio_uart_state_t *state, const uint8_t *txBuff, uint32_t txSize,
                                           uint32_t timeout)
{
    /* Check if driver is busy */
    if (!state->driverIdle) {
        return STATUS_BUSY;
    }
    /* Mark transfer as blocking */
    if (SUPERIO_DRIVER_TYPE_POLLING != state->driverType) {
        state->blocking = true;
        /* Dummy wait to ensure the semaphore is 0, no need to check result */
        (void)OS_SemaphoreAcquire(state->idleSemaphore, 0);
    }
    /* Call SUPERIO_UART_DRV_SendData to start transfer */
    /* We already did the busy check so no need to check return code of SUPERIO_UART_DRV_SendData */
    (void)SUPERIO_UART_DRV_SendData(state, txBuff, txSize);

    /* Wait for transfer to end */
    return SUPERIO_UART_WaitTransferEnd(state, timeout);
}

/*
 * Function Name : SUPERIO_UART_DRV_ReceiveData
 * Description   : Perform a non-blocking UART reception
 *
 */
status_t SUPERIO_UART_DRV_ReceiveData(superio_uart_state_t *state, uint8_t *rxBuff, uint32_t rxSize)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)

    OS_ASSERT(state != NULL);
    OS_ASSERT(rxBuff != NULL);
    OS_ASSERT(rxSize > 0U);
    OS_ASSERT(state->direction == SUPERIO_UART_DIRECTION_RX);
    /* If 2 bytes per word are needed, then the size must be even */
    OS_ASSERT((state->bitCount <= 8U) || ((rxSize & 1U) == 0U));

    baseAddr      = g_superioBase[state->superioCommon.instance];
    resourceIndex = state->superioCommon.resourceIndex;

    /* Check if driver is busy */
    if (!state->driverIdle) {
        return STATUS_BUSY;
    }

    state->rxData         = rxBuff;
    state->remainingBytes = rxSize;
    state->status         = STATUS_BUSY;
    state->driverIdle     = false;

    /* Enable timers and shifters */
    SUPERIO_UART_EnableTransfer(state);
    /* Enable transfer engine */
    switch (state->driverType) {
    case SUPERIO_DRIVER_TYPE_INTERRUPTS:
        /* Enable interrupts for Rx shifter */
        SUPERIO_SetShifterInterrupt(baseAddr, (uint8_t)(1U << RX_SHIFTER(resourceIndex)), true);
        SUPERIO_SetShifterErrorInterrupt(baseAddr, (uint8_t)(1U << RX_SHIFTER(resourceIndex)), true);
        break;
    case SUPERIO_DRIVER_TYPE_POLLING:
        /* Call SUPERIO_UART_CheckStatus once to send the first byte */
        SUPERIO_UART_CheckStatus(state);
        break;
    case SUPERIO_DRIVER_TYPE_DMA:
        SUPERIO_UART_StartRxDmaTransfer(state);
        break;
    default:
        /* Impossible type - do nothing */
        break;
    }

    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_UART_DRV_ReceiveDataBlocking
 * Description   : Perform a blocking UART reception
 *
 */
status_t SUPERIO_UART_DRV_ReceiveDataBlocking(superio_uart_state_t *state, uint8_t *rxBuff, uint32_t rxSize,
                                              uint32_t timeout)
{
    /* Check if driver is busy */
    if (!state->driverIdle) {
        return STATUS_BUSY;
    }
    /* Mark transfer as blocking */
    if (SUPERIO_DRIVER_TYPE_POLLING != state->driverType) {
        state->blocking = true;
        /* Dummy wait to ensure the semaphore is 0, no need to check result */
        (void)OS_SemaphoreAcquire(state->idleSemaphore, 0);
    }
    /* Call SUPERIO_UART_DRV_ReceiveData to start transfer */
    /* We already did the busy check so no need to check return code of SUPERIO_UART_DRV_ReceiveData */
    (void)SUPERIO_UART_DRV_ReceiveData(state, rxBuff, rxSize);

    /* Wait for transfer to end */
    return SUPERIO_UART_WaitTransferEnd(state, timeout);
}

/*
 * Function Name : SUPERIO_UART_DRV_TransferAbort
 * Description   : Aborts a non-blocking UART transfer
 *
 */
status_t SUPERIO_UART_DRV_TransferAbort(superio_uart_state_t *state)
{
    OS_ASSERT(state != NULL);

    /* Check if driver is busy */
    if (state->driverIdle) {
        return STATUS_SUCCESS;
    }

    state->status = STATUS_UART_ABORTED;
    SUPERIO_UART_StopTransfer(state);

    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_UART_DRV_GetStatus
 * Description   : Get the status of the current non-blocking UART transaction
 *
 */
status_t SUPERIO_UART_DRV_GetStatus(superio_uart_state_t *state, uint32_t *bytesRemaining)
{
    uint32_t remainingBytes;

    OS_ASSERT(state != NULL);

    /* Initialize with the actual remaining bytes count */
    remainingBytes = state->remainingBytes;

    if (!state->driverIdle) {
        switch (state->driverType) {
        case SUPERIO_DRIVER_TYPE_POLLING:
            /* In polling mode advance the UART transfer here */
            SUPERIO_UART_CheckStatus(state);
            break;

        case SUPERIO_DRIVER_TYPE_DMA:
            /* In PDMA mode just update the remaining count.
            DO NOT write state->remainingBytes directly !!! */
            remainingBytes = PDMA_DRV_GetRemainingMajorIterationsCount(state->dmaChannel);
            break;

        default:
            /* Nothing */
            break;
        }
    }

    if (NULL != bytesRemaining) {
        *bytesRemaining = remainingBytes;
    }

    if (!state->driverIdle) {
        return STATUS_BUSY;
    } else {
        return state->status;
    }
}

/*
 * Function Name : SUPERIO_UART_DRV_SetRxBuffer
 * Description   : Provide a buffer for receiving data.
 *
 */
status_t SUPERIO_UART_DRV_SetRxBuffer(superio_uart_state_t *state, uint8_t *rxBuff, uint32_t rxSize)
{
    OS_ASSERT(state != NULL);
    OS_ASSERT(rxBuff != NULL);
    OS_ASSERT(rxSize > 0U);

    state->rxData         = rxBuff;
    state->remainingBytes = rxSize;

    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_UART_DRV_SetTxBuffer
 * Description   : Provide a buffer for transmitting data.
 *
 */
status_t SUPERIO_UART_DRV_SetTxBuffer(superio_uart_state_t *state, const uint8_t *txBuff, uint32_t txSize)
{
    OS_ASSERT(state != NULL);
    OS_ASSERT(txBuff != NULL);
    OS_ASSERT(txSize > 0U);

    state->txData         = (uint8_t *)txBuff;
    state->remainingBytes = txSize;
    /* In case of continuous transmission there are always 2 bytes to flush */
    state->txFlush = 2;

    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_UART_DRV_GetDefaultConfig
 * Description   : Returns default configuration structure for SuperIO_UART
 *
 */
void SUPERIO_UART_DRV_GetDefaultConfig(superio_uart_user_config_t *userConfigPtr)
{
    OS_ASSERT(userConfigPtr != NULL);

    userConfigPtr->driverType    = SUPERIO_DRIVER_TYPE_INTERRUPTS;
    userConfigPtr->baudRate      = 115200U;
    userConfigPtr->bitCount      = 8U;
    userConfigPtr->direction     = SUPERIO_UART_DIRECTION_TX;
    userConfigPtr->dataPin       = 0U;
    userConfigPtr->callback      = NULL;
    userConfigPtr->callbackParam = NULL;
    userConfigPtr->dmaChannel    = 0U;
}
