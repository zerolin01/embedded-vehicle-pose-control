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
 * @file uart_driver.c
 * @brief UART driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "uart_hw_access.h"
#include <stdbool.h>
#include <stddef.h>

#ifdef BENCH_RTOS_BENCHMARK
static uint8_t semaphore_rx[128];
static uint8_t semaphore_tx[128];
#endif

/** Pointer to uart runtime state structure */
static uart_state_t *s_pstUartState[UART_INSTANCE_COUNT] = {NULL};

/** Table of base addresses for uart instances. */
static uart_type_t *const s_pstUartBase[UART_INSTANCE_COUNT] = UART_BASE_PTRS;

/** Table to save UART enum numbers defined in CMSIS files. */
static const IRQn_Type s_euUartRxIrqId[UART_INSTANCE_COUNT] = UART_RX_IRQS;
static const IRQn_Type s_euUartTxIrqId[UART_INSTANCE_COUNT] = UART_TX_IRQS;

/** Table to save UART clock names as defined in clock manager. */
static const clock_names_t s_euUartClkNames[UART_INSTANCE_COUNT] = UART_CLOCK_NAMES;

static status_t UART_DRV_StartSendDataUsingInt(uint32_t instance, const uint8_t *txBuff, uint32_t txSize);
static void UART_DRV_CompleteSendDataUsingInt(uint32_t instance);
static status_t UART_DRV_StartReceiveDataUsingInt(uint32_t instance, uint8_t *rxBuff, uint32_t rxSize);
static void UART_DRV_CompleteReceiveDataUsingInt(uint32_t instance);
static void UART_DRV_StopTxDma(uint32_t instance);
static void UART_DRV_StopRxDma(uint32_t instance);
static status_t UART_DRV_StartSendDataUsingDma(uint32_t instance, const uint8_t *txBuff, uint32_t txSize);
static void UART_DRV_TxDmaCallback(void *parameter, pdma_chn_status_t status);
static status_t UART_DRV_StartReceiveDataUsingDma(uint32_t instance, uint8_t *rxBuff, uint32_t rxSize);
static void UART_DRV_RxDmaCallback(void *parameter, pdma_chn_status_t status);
static void UART_DRV_PutData(uint32_t instance);
static void UART_DRV_GetData(uint32_t instance);
static void UART_DRV_RxIrqHandler(uint32_t instance);
static void UART_DRV_TxEmptyIrqHandler(uint32_t instance);
static void UART_DRV_TxCompleteIrqHandler(uint32_t instance);
static void UART_DRV_RxIdleIrqHandler(uint32_t instance);
static void UART_DRV_ErrIrqHandler(uint32_t instance);
static void UART_DRV_IRQHandler(void *data);

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_GetDefaultConfig
 * Description   : Initializes the UART configuration structure with
 *                 default values.
 *
 *END**************************************************************************/
void UART_DRV_GetDefaultConfig(uart_user_config_t *uartUserConfig)
{
    OS_ASSERT(NULL != uartUserConfig);

    uartUserConfig->transferType    = UART_USING_INTERRUPTS;
    uartUserConfig->baudRate        = 9600U;
    uartUserConfig->parityMode      = UART_PARITY_DISABLED;
    uartUserConfig->stopBitCount    = UART_ONE_STOP_BIT;
    uartUserConfig->bitCountPerChar = UART_8_BITS_PER_CHAR;
    uartUserConfig->rxDMAChannel    = 0U;
    uartUserConfig->txDMAChannel    = 0U;
}

/**
 * @brief Initializes the UART fifo water
 *
 * This function Initializes the UART fifo water
 * 
 */
void UART_DRV_FIFOWater_Init(uint32_t instance, uint16_t water)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);
    uart_type_t *base = s_pstUartBase[instance];

    UART_FIFOWater_Init(base, water);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_Init
 * Description   : This function initializes a UART instance for operation.
 * This function will initialize the run-time state structure to keep track of
 * the on-going transfers,  un-gates the clock to the UART module, initialize the
 * module to user defined settings and default settings, configure the IRQ state
 * structure and enable the module-level interrupt to the core, and enable the
 * UART module transmitter and receiver.
 * The following is an example of how to set up the uart_state_t and the
 * uart_user_config_t parameters and how to call the UART_DRV_Init function
 * by passing in these parameters:
 *    uart_user_config_t uartConfig;
 *    uartConfig.baudRate = 9600;
 *    uartConfig.bitCountPerChar = UART_8_BITS_PER_CHAR;
 *    uartConfig.parityMode = UART_PARITY_DISABLED;
 *    uartConfig.stopBitCount = UART_ONE_STOP_BIT;
 *    uartConfig.transferType = UART_USING_INTERRUPTS;
 *    uart_state_t uartState;
 *    UART_DRV_Init(instance, &uartState, &uartConfig);
 *
 *END**************************************************************************/
status_t UART_DRV_Init(uint32_t instance, uart_state_t *uartStatePtr, const uart_user_config_t *uartUserConfig)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);
    OS_ASSERT(NULL != uartStatePtr);
    OS_ASSERT(NULL != uartUserConfig);

    clock_names_t uartClocks[UART_INSTANCE_COUNT] = UART_CLOCK_NAMES;
    CLOCK_SYS_ClockEnable(uartClocks[instance], true);

    uart_type_t *base = s_pstUartBase[instance];
    uint32_t idx;
    uint32_t uartSourceClock;
    clock_names_t instanceClkName    = s_euUartClkNames[instance];
    OS_SemaphoreAttr_t semUartRxAttr = {0}, semUartTxAttr = {0};
    OS_RegisterType_t uType = {0};

    /* Get the UART clock as configured in the clock manager */
    (void)CLOCK_SYS_GetFreq(instanceClkName, &uartSourceClock);

    /* Check if current instance is clock gated off. */
    OS_ASSERT(uartSourceClock > 0U);

    /* Check if current instance is already initialized. */
    OS_ASSERT(NULL == s_pstUartState[instance]);

    /* In PDMA mode, only 8-bits chars are supported */
    OS_ASSERT((UART_USING_DMA != uartUserConfig->transferType) ||
              (UART_8_BITS_PER_CHAR == uartUserConfig->bitCountPerChar));

    /* For 10 bits per char, parity bit cannot be enabled */
    OS_ASSERT((UART_10_BITS_PER_CHAR != uartUserConfig->bitCountPerChar) ||
              (UART_PARITY_DISABLED == uartUserConfig->parityMode));

    /* Clear the state struct for this instance. */
    uint8_t *clearStructPtr = (uint8_t *)uartStatePtr;
    for (idx = 0; idx < sizeof(uart_state_t); idx++) {
        clearStructPtr[idx] = 0;
    }

    /* Save runtime structure pointer.*/
    s_pstUartState[instance] = uartStatePtr;

    /* Save the transfer information for runtime retrieval */
    uartStatePtr->transferType    = uartUserConfig->transferType;
    uartStatePtr->bitCountPerChar = uartUserConfig->bitCountPerChar;

    uartStatePtr->rxDMAChannel = uartUserConfig->rxDMAChannel;
    uartStatePtr->txDMAChannel = uartUserConfig->txDMAChannel;

    /* Save instance number */
    uartStatePtr->instance = instance;

    /* initialize the UART instance */
    UART_Init(base);

    /* initialize the parameters of the UART config structure with desired data */
    (void)UART_DRV_SetBaudRate(instance, uartUserConfig->baudRate);

    if (UART_PARITY_DISABLED != uartUserConfig->parityMode) {
        UART_SetBitCountPerChar(base, uartUserConfig->bitCountPerChar, true);
    } else {
        UART_SetBitCountPerChar(base, uartUserConfig->bitCountPerChar, false);
    }
    UART_SetParityMode(base, uartUserConfig->parityMode);
    UART_SetStopBitCount(base, uartUserConfig->stopBitCount);

    /* initialize last driver operation status */
    uartStatePtr->transmitStatus = STATUS_SUCCESS;
    uartStatePtr->receiveStatus  = STATUS_SUCCESS;

    semUartRxAttr.name   = "uart_rx";
#ifdef BENCH_RTOS_BENCHMARK
    semUartRxAttr.cb_mem = &semaphore_rx[0];
    semUartRxAttr.cb_size = 128;
#else
    semUartRxAttr.cb_mem = NULL;
#endif
    /* Create a Sema */
    uartStatePtr->rxComplete = OS_SemaphoreNew(1, 0, &semUartRxAttr);
    if (NULL == uartStatePtr->rxComplete)
        // log_err("rxComplete Semaphore create failed\r\n");

    semUartTxAttr.name   = "uart_tx";
#ifdef BENCH_RTOS_BENCHMARK
    semUartTxAttr.cb_mem = &semaphore_tx[0];
    semUartTxAttr.cb_size = 128;
#else
    semUartRxAttr.cb_mem = NULL;
#endif
    /* Create a Sema */
    uartStatePtr->txComplete = OS_SemaphoreNew(1, 0, &semUartTxAttr);
    if (NULL == uartStatePtr->txComplete)
        // log_err("txComplete Semaphore create failed\r\n");

    /* Install UART irq handler */
    uType.trig_mode = CLIC_LEVEL_TRIGGER;
    uType.lvl       = 2;
    uType.priority  = 1;
    uType.data_ptr  = (void *)instance;
    OS_RequestIrq(s_euUartRxIrqId[instance], UART_DRV_IRQHandler, &uType);
    OS_RequestIrq(s_euUartTxIrqId[instance], UART_DRV_IRQHandler, &uType);

    /* Enable UART interrupt. */
    OS_EnableIrq(s_euUartRxIrqId[instance]);
    OS_EnableIrq(s_euUartTxIrqId[instance]);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_Deinit
 * Description   : This function shuts down the UART by disabling interrupts and
 *                 transmitter/receiver.
 *
 *END**************************************************************************/
status_t UART_DRV_Deinit(uint32_t instance)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    clock_names_t instanceClkName = s_euUartClkNames[instance];
    uint32_t uartSourceClock;
    const uart_type_t *base       = s_pstUartBase[instance];
    const uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];

    (void)CLOCK_SYS_GetFreq(instanceClkName, &uartSourceClock);

    /* Check if current instance is already de-initialized or is gated.*/
    OS_ASSERT(NULL != s_pstUartState[instance]);
    OS_ASSERT(uartSourceClock > 0U);

    /* Wait until the data is completely shifted out of shift register */
    while (!UART_GetStatusFlag(base, UART_TX_COMPLETE)) {
    }

    /* Destroy the synchronization objects */
    (void)OS_SemaphoreDelete(uartState->rxComplete);
    (void)OS_SemaphoreDelete(uartState->txComplete);

    /* Disable UART interrupt. */
    OS_DisableIrq(s_euUartRxIrqId[instance]);
    OS_DisableIrq(s_euUartTxIrqId[instance]);

    /* Restore default handler. */
    OS_UnRequestIrq(s_euUartRxIrqId[instance]);
    OS_UnRequestIrq(s_euUartTxIrqId[instance]);

    /* Clear our saved pointer to the state structure */
    s_pstUartState[instance] = NULL;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_InstallRxCallback
 * Description   : Install receive data callback function.
 *
 *END**************************************************************************/
uart_callback_t UART_DRV_InstallRxCallback(uint32_t instance, uart_callback_t function, void *callbackParam)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];

    uart_callback_t currentCallback = uartState->rxCallback;
    uartState->rxCallback           = function;
    uartState->rxCallbackParam      = callbackParam;

    return currentCallback;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_InstallTxCallback
 * Description   : Install transmit data callback function, pass in NULL pointer
 * as callback will uninstall.
 *
 *END**************************************************************************/
uart_callback_t UART_DRV_InstallTxCallback(uint32_t instance, uart_callback_t function, void *callbackParam)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];

    uart_callback_t currentCallback = uartState->txCallback;
    uartState->txCallback           = function;
    uartState->txCallbackParam      = callbackParam;

    return currentCallback;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_SendDataBlocking
 * Description   : This function sends data out through the UART module using
 * blocking method. The function does not return until the transmit is complete.
 *
 *END**************************************************************************/
status_t UART_DRV_SendDataBlocking(uint32_t instance, const uint8_t *txBuff, uint32_t txSize, uint32_t timeout)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);
    OS_ASSERT(NULL != txBuff);

    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];
    status_t retVal         = STATUS_SUCCESS;
    OS_Status_t syncStatus;

    /* Indicates this is a blocking transaction. */
    uartState->isTxBlocking = true;

    OS_ASSERT((UART_USING_INTERRUPTS == uartState->transferType) || (UART_USING_DMA == uartState->transferType));

    if (UART_USING_INTERRUPTS == uartState->transferType) {
        /* Start the transmission process using interrupts */
        retVal = UART_DRV_StartSendDataUsingInt(instance, txBuff, txSize);
    } else {
        /* Start the transmission process using PDMA */
        retVal = UART_DRV_StartSendDataUsingDma(instance, txBuff, txSize);
    }

    if (STATUS_SUCCESS == retVal) {
        /* Wait until the transmit is complete. */
        syncStatus = OS_SemaphoreAcquire(uartState->txComplete, timeout);

        /* Finish the transmission if timeout expired */
        if (OS_ErrorTimeout == syncStatus) {
            uartState->isTxBlocking   = false;
            uartState->transmitStatus = STATUS_TIMEOUT;

            if (UART_USING_INTERRUPTS == uartState->transferType) {
                UART_DRV_CompleteSendDataUsingInt(instance);
            } else {
                UART_DRV_StopTxDma(instance);
            }
        }
    }

    return uartState->transmitStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_SendDataPolling
 * Description   : Send out multiple bytes of data using polling method.
 *
 *END**************************************************************************/
status_t UART_DRV_SendDataPolling(uint32_t instance, const uint8_t *txBuff, uint32_t txSize)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);
    OS_ASSERT(NULL != txBuff);

    uart_type_t *base       = s_pstUartBase[instance];
    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];

    /* Check the validity of the parameters */
    OS_ASSERT(txSize > 0U);
    OS_ASSERT((UART_8_BITS_PER_CHAR == uartState->bitCountPerChar) || (0U == (txSize & 1U)));

    /* Check driver is not busy transmitting data from a previous asynchronous call */
    if (uartState->isTxBusy) {
        return STATUS_BUSY;
    }

    /* Enable the UART transmitter */
    UART_SetTransmitterCmd(base, true);

    while (txSize > 0U) {
        while (!UART_GetStatusFlag(base, UART_TX_DATA_REG_EMPTY)) {
        }

        uartState->txBuff = txBuff;
        UART_DRV_PutData(instance);

        if (UART_8_BITS_PER_CHAR == uartState->bitCountPerChar) {
            ++txBuff;
            --txSize;
        } else {
            ++txBuff;
            ++txBuff;
            txSize -= 2U;
        }
    }
    /* fixed send failed when txsize = 1 */
    while (!UART_GetStatusFlag(base, UART_TX_DATA_REG_EMPTY)) {
    }

    /* Disable the UART transmitter */
    UART_SetTransmitterCmd(base, false);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_SendDataPollingWithTimeout
 * Description   : Send out multiple bytes of data using polling method.
 *
 *END**************************************************************************/
status_t UART_DRV_SendDataPollingWithTimeout(uint32_t instance, const uint8_t *txBuff, uint32_t txSize,
                                             uint32_t timeout)
{
    status_t status = STATUS_ERROR;
    OS_ASSERT(instance < UART_INSTANCE_COUNT);
    OS_ASSERT(NULL != txBuff);

    uart_type_t *base       = s_pstUartBase[instance];
    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];

    /* Check the validity of the parameters */
    OS_ASSERT(txSize > 0U);
    OS_ASSERT((UART_8_BITS_PER_CHAR == uartState->bitCountPerChar) || (0U == (txSize & 1U)));

    /* Check driver is not busy transmitting data from a previous asynchronous call */
    if (uartState->isTxBusy) {
        return STATUS_BUSY;
    }

    /* Enable the UART transmitter */
    UART_SetTransmitterCmd(base, true);

    while (txSize > 0U) {
        while (!UART_GetStatusFlag(base, UART_TX_DATA_REG_EMPTY)) {
            timeout--;
        }
        if (0 == timeout) {
            status = STATUS_TIMEOUT;
            break;
        }

        uartState->txBuff = txBuff;
        UART_DRV_PutData(instance);

        if (UART_8_BITS_PER_CHAR == uartState->bitCountPerChar) {
            ++txBuff;
            --txSize;
        } else {
            ++txBuff;
            ++txBuff;
            txSize -= 2U;
        }

        status = STATUS_SUCCESS;
    }

    /* fixed send failed when txsize = 1 */
    while (!UART_GetStatusFlag(base, UART_TX_DATA_REG_EMPTY)) {
    }

    /* Disable the UART transmitter */
    UART_SetTransmitterCmd(base, false);

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_SendData
 * Description   : This function sends data out through the UART module using
 * non-blocking method. The function will return immediately after calling this
 * function.
 *
 *END**************************************************************************/
status_t UART_DRV_SendData(uint32_t instance, const uint8_t *txBuff, uint32_t txSize)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);
    OS_ASSERT(NULL != txBuff);

    status_t retVal         = STATUS_SUCCESS;
    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];

    /* Indicates this is a non-blocking transaction. */
    uartState->isTxBlocking = false;

    OS_ASSERT((UART_USING_INTERRUPTS == uartState->transferType) || (UART_USING_DMA == uartState->transferType));

    if (UART_USING_INTERRUPTS == uartState->transferType) {
        /* Start the transmission process using interrupts */
        retVal = UART_DRV_StartSendDataUsingInt(instance, txBuff, txSize);
    } else {
        /* Start the transmission process using PDMA */
        retVal = UART_DRV_StartSendDataUsingDma(instance, txBuff, txSize);
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_GetTransmitStatus
 * Description   : This function returns whether the previous UART transmit has
 * finished. When performing non-blocking transmit, the user can call this
 * function to ascertain the state of the current transmission:
 * in progress (or busy) or complete (success). In addition, if the transmission
 * is still in progress, the user can obtain the number of words that have been
 * currently transferred.
 *
 *END**************************************************************************/
status_t UART_DRV_GetTransmitStatus(uint32_t instance, uint32_t *bytesRemaining)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    const uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];

    if (NULL != bytesRemaining) {
        if (uartState->isTxBusy) {
            /* Fill in the bytes not transferred yet. */
            if (UART_USING_INTERRUPTS == uartState->transferType) {
                /* In interrupt-based communication, the remaining bytes are retrieved
                 * from the state structure
                 */
                *bytesRemaining = uartState->txSize;
                ;
            } else {
                /* In PDMA-based communication, the remaining bytes are retrieved
                 * from the current PDMA major loop count
                 */
                *bytesRemaining = PDMA_DRV_GetRemainingMajorIterationsCount(uartState->txDMAChannel);
            }
        } else {
            *bytesRemaining = 0;
        }
    }

    return uartState->transmitStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_AbortSendingData
 * Description   : This function terminates an non-blocking UART transmission
 * early. During a non-blocking UART transmission, the user has the option to
 * terminate the transmission early if the transmission is still in progress.
 *
 *END**************************************************************************/
status_t UART_DRV_AbortSendingData(uint32_t instance)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];

    /* Check if a transfer is running. */
    if (!uartState->isTxBusy) {
        return STATUS_SUCCESS;
    }

    /* Update the tx status */
    uartState->transmitStatus = STATUS_UART_ABORTED;

    /* Stop the running transfer. */
    if (UART_USING_INTERRUPTS == uartState->transferType) {
        UART_DRV_CompleteSendDataUsingInt(instance);
    } else {
        UART_DRV_StopTxDma(instance);
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_ReceiveDataBlocking
 * Description   : This function receives data from UART module using blocking
 * method, the function does not return until the receive is complete.
 *
 *END**************************************************************************/
status_t UART_DRV_ReceiveDataBlocking(uint32_t instance, uint8_t *rxBuff, uint32_t rxSize, uint32_t timeout)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);
    OS_ASSERT(NULL != rxBuff);

    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];
    status_t retVal         = STATUS_SUCCESS;
    OS_Status_t syncStatus;

    /* Indicates this is a blocking transaction. */
    uartState->isRxBlocking = true;

    OS_ASSERT((UART_USING_INTERRUPTS == uartState->transferType) || (UART_USING_DMA == uartState->transferType));

    if (UART_USING_INTERRUPTS == uartState->transferType) {
        /* Start the reception process using interrupts */
        retVal = UART_DRV_StartReceiveDataUsingInt(instance, rxBuff, rxSize);
    } else {
        /* Start the reception process using PDMA */
        retVal = UART_DRV_StartReceiveDataUsingDma(instance, rxBuff, rxSize);
    }

    if (STATUS_SUCCESS == retVal) {
        /* Wait until the receive is complete. */
        syncStatus = OS_SemaphoreAcquire(uartState->rxComplete, timeout);

        /* Finish the reception if timeout expired */
        if (OS_ErrorTimeout == syncStatus) {
            uartState->isRxBlocking  = false;
            uartState->receiveStatus = STATUS_TIMEOUT;

            if (UART_USING_INTERRUPTS == uartState->transferType) {
                UART_DRV_CompleteReceiveDataUsingInt(instance);
            } else {
                UART_DRV_StopRxDma(instance);
            }
        }
    }

    return uartState->receiveStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_ReceiveDataPolling
 * Description   : Receive multiple bytes of data using polling method.
 *
 *END**************************************************************************/
status_t UART_DRV_ReceiveDataPolling(uint32_t instance, uint8_t *rxBuff, uint32_t rxSize)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);
    OS_ASSERT(NULL != rxBuff);

    uint8_t tmpByte;
    status_t retVal         = STATUS_SUCCESS;
    status_t tmpState       = STATUS_SUCCESS;
    uart_type_t *base       = s_pstUartBase[instance];
    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];

    /* Check the validity of the parameters */
    OS_ASSERT(rxSize > 0U);
    OS_ASSERT((UART_8_BITS_PER_CHAR == uartState->bitCountPerChar) || (0U == (rxSize & 1U)));

    /* Check driver is not busy receiving data from a previous asynchronous call */
    if (uartState->isRxBusy) {
        return STATUS_BUSY;
    }

    /* Enable the UART receiver */
    UART_SetReceiverCmd((uart_type_t *)base, true);

    while (rxSize > 0U) {
        while (!UART_GetStatusFlag(base, UART_RX_DATA_REG_FULL)) {
        }

        uartState->rxBuff = rxBuff;
        UART_DRV_GetData(instance);

        if (UART_8_BITS_PER_CHAR == uartState->bitCountPerChar) {
            ++rxBuff;
            --rxSize;
        } else {
            ++rxBuff;
            ++rxBuff;
            rxSize -= 2U;
        }

        /* Check for errors on received data */
        if (UART_GetStatusFlag(base, UART_FRAME_ERR)) {
            tmpState = STATUS_UART_FRAMING_ERROR;
            /* Disable the UART receiver */
            UART_SetReceiverCmd((uart_type_t *)base, false);
            /* Clear the flag */
            (void)UART_ClearStatusFlag(base, UART_FRAME_ERR);
            break;
        }
        if (UART_GetStatusFlag(base, UART_NOISE_DETECT)) {
            tmpState = STATUS_UART_NOISE_ERROR;
            /* Disable the UART receiver */
            UART_SetReceiverCmd((uart_type_t *)base, false);
            /* Clear the flag */
            (void)UART_ClearStatusFlag(base, UART_NOISE_DETECT);
            break;
        }
        if (UART_GetStatusFlag(base, UART_PARITY_ERR)) {
            tmpState = STATUS_UART_PARITY_ERROR;
            /* Disable the UART receiver */
            UART_SetReceiverCmd((uart_type_t *)base, false);
            /* Clear the flag */
            (void)UART_ClearStatusFlag(base, UART_PARITY_ERR);
            break;
        }
        if (UART_GetStatusFlag(base, UART_RX_OVERRUN)) {
            tmpState = STATUS_UART_RX_OVERRUN;
            /* Disable the UART receiver */
            UART_SetReceiverCmd((uart_type_t *)base, false);
            /* Clear the flag */
            (void)UART_ClearStatusFlag(base, UART_RX_OVERRUN);
            break;
        }
    }

    /* Update received status */
    if ((0U == rxSize) && (STATUS_UART_RX_OVERRUN == tmpState)) {
        retVal = STATUS_SUCCESS;
    } else {
        retVal = tmpState;
    }

    if (STATUS_SUCCESS == retVal) {
        /* Disable the UART receiver */
        UART_SetReceiverCmd((uart_type_t *)base, false);
    }

    /* Read dummy to clear RDRF flag */
    UART_Getchar(base, &tmpByte);

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_ReceiveData
 * Description   : This function receives data from UART module using
 * non-blocking method.  This function returns immediately after initiating the
 * receive function. The application has to get the receive status to see when
 * the receive is complete. In other words, after calling non-blocking get
 * function, the application must get the receive status to check if receive
 * is completed or not.
 *
 *END**************************************************************************/
status_t UART_DRV_ReceiveData(uint32_t instance, uint8_t *rxBuff, uint32_t rxSize)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);
    OS_ASSERT(NULL != rxBuff);

    status_t retVal         = STATUS_SUCCESS;
    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];

    /* Indicates this is a non-blocking transaction. */
    uartState->isRxBlocking = false;

    OS_ASSERT((UART_USING_INTERRUPTS == uartState->transferType) || (UART_USING_DMA == uartState->transferType));

    if (UART_USING_INTERRUPTS == uartState->transferType) {
        /* Start the reception process using interrupts */
        retVal = UART_DRV_StartReceiveDataUsingInt(instance, rxBuff, rxSize);
    } else {
        /* Start the reception process using PDMA */
        retVal = UART_DRV_StartReceiveDataUsingDma(instance, rxBuff, rxSize);
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_GetReceiveStatus
 * Description   : This function returns whether the previous UART receive is
 * complete. When performing a non-blocking receive, the user can call this
 * function to ascertain the state of the current receive progress: in progress
 * or complete. In addition, if the receive is still in progress, the user can
 * obtain the number of words that have been currently received.
 *
 *END**************************************************************************/
status_t UART_DRV_GetReceiveStatus(uint32_t instance, uint32_t *bytesRemaining)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    const uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];

    if (NULL != bytesRemaining) {
        if (uartState->isRxBusy) {
            /* Fill in the bytes transferred. */
            if (UART_USING_INTERRUPTS == uartState->transferType) {
                /* In interrupt-based communication, the remaining bytes are retrieved
                 * from the state structure
                 */
                *bytesRemaining = uartState->rxSize;
            } else {
                /* In PDMA-based communication, the remaining bytes are retrieved
                 * from the current PDMA major loop count
                 */
                *bytesRemaining = PDMA_DRV_GetRemainingMajorIterationsCount(uartState->rxDMAChannel);
            }
        } else {
            *bytesRemaining = 0;
        }
    }

    return uartState->receiveStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_AbortReceivingData
 * Description   : Terminates a non-blocking receive early.
 *
 *END**************************************************************************/
status_t UART_DRV_AbortReceivingData(uint32_t instance)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];

    /* Check if a transfer is running. */
    if (!uartState->isRxBusy) {
        return STATUS_SUCCESS;
    }

    /* Update the rx status */
    uartState->receiveStatus = STATUS_UART_ABORTED;

    /* Stop the running transfer. */
    if (UART_USING_INTERRUPTS == uartState->transferType) {
        UART_DRV_CompleteReceiveDataUsingInt(instance);
    } else {
        UART_DRV_StopRxDma(instance);
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_SetBaudRate
 * Description   : Configures the UART baud rate.
 * In some UART instances the user must disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
status_t UART_DRV_SetBaudRate(uint32_t instance, uint32_t desiredBaudRate)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    uint16_t sbr, sbrTemp, i;
    uint32_t osr, tempDiff, calculatedBaud, baudDiff, maxOsr;
    uint32_t uartSourceClock;
    clock_names_t instanceClkName = s_euUartClkNames[instance];
    uart_type_t *base             = s_pstUartBase[instance];
    const uart_state_t *uartState;
    uartState = (uart_state_t *)s_pstUartState[instance];

    if (NULL != uartState) {
        /* Check if there is an ongoing transfer */
        if (true == uartState->isTxBusy) {
            return STATUS_BUSY;
        }
        if (true == uartState->isRxBusy) {
            return STATUS_BUSY;
        }
    }

    /* Get the UART clock as configured in the clock manager */
    (void)CLOCK_SYS_GetFreq(instanceClkName, &uartSourceClock);

    /* Check if current instance is clock gated off. */
    OS_ASSERT(uartSourceClock > 0U);
    /* Check if the desired baud rate can be configured with the current protocol clock. */
    OS_ASSERT(uartSourceClock >= (desiredBaudRate * 4U));

    /* This uart instantiation uses a slightly different baud rate calculation
     * The idea is to use the best OSR (over-sampling rate) possible
     * Note, osr is typically hard-set to 16 in other uart instantiations
     * First calculate the baud rate using the minimum OSR possible (4) */
    osr            = 4;
    sbr            = (uint16_t)(uartSourceClock / (desiredBaudRate * osr));
    calculatedBaud = (uartSourceClock / (osr * sbr));
    if (calculatedBaud > desiredBaudRate) {
        baudDiff = calculatedBaud - desiredBaudRate;
    } else {
        baudDiff = desiredBaudRate - calculatedBaud;
    }
    /* find maximum osr */
    maxOsr = uartSourceClock / desiredBaudRate;
    if (maxOsr > 32U) {
        maxOsr = 32U;
    }
    /* loop to find the best osr value possible, one that generates minimum baudDiff
     * iterate through the rest of the supported values of osr */
    if (maxOsr >= 5U) {
        for (i = 5U; i <= maxOsr; i++) {
            /* calculate the temporary sbr value   */
            sbrTemp = (uint16_t)(uartSourceClock / (desiredBaudRate * i));
            /* calculate the baud rate based on the temporary osr and sbr values */
            calculatedBaud = (uartSourceClock / (i * sbrTemp));

            if (calculatedBaud > desiredBaudRate) {
                tempDiff = calculatedBaud - desiredBaudRate;
            } else {
                tempDiff = desiredBaudRate - calculatedBaud;
            }

            if (tempDiff <= baudDiff) {
                baudDiff = tempDiff;
                osr      = i;       // update and store the best osr value calculated
                sbr      = sbrTemp; // update store the best sbr value calculated
            }
        }
    }
    /* Check if osr is between 4x and 7x oversampling.
     * If so, then "BOTHEDGE" sampling must be turned on */
    if (osr < 8U) {
        UART_EnableBothEdgeSamplingCmd(base);
    }

    /* program the osr value (bit value is one less than actual value) */
    UART_SetOversamplingRatio(base, (osr - 1U));

    /* write the sbr value to the BAUD registers */
    UART_SetBaudRateDivisor(base, sbr);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_GetBaudRate
 * Description   : Returns the UART configured baud rate.
 *
 *END**************************************************************************/
void UART_DRV_GetBaudRate(uint32_t instance, uint32_t *configuredBaudRate)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);
    OS_ASSERT(configuredBaudRate != NULL);

    uint8_t osr;
    uint16_t sbr;
    uint32_t uartSourceClock;
    clock_names_t instanceClkName = s_euUartClkNames[instance];
    const uart_type_t *base       = s_pstUartBase[instance];

    /* Get the UART clock as configured in the clock manager */
    (void)CLOCK_SYS_GetFreq(instanceClkName, &uartSourceClock);

    osr = UART_GetOversamplingRatio(base);
    sbr = UART_GetBaudRateDivisor(base);

    *configuredBaudRate = (uartSourceClock / ((osr + 1UL) * sbr));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_SetTxBuffer
 * Description   : Sets the driver internal reference to the tx buffer.
 *                 Can be called from the tx callback to provide a different
 *                 buffer for continuous transmission.
 *
 *END**************************************************************************/
status_t UART_DRV_SetTxBuffer(uint32_t instance, const uint8_t *txBuff, uint32_t txSize)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);
    OS_ASSERT(txBuff != NULL);
    OS_ASSERT(txSize > 0U);

    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];
    uartState->txBuff       = txBuff;
    uartState->txSize       = txSize;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_SetRxBuffer
 * Description   : Sets the driver internal reference to the rx buffer.
 *                 Can be called from the rx callback to provide a different
 *                 buffer for continuous reception.
 *
 *END**************************************************************************/
status_t UART_DRV_SetRxBuffer(uint32_t instance, uint8_t *rxBuff, uint32_t rxSize)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);
    OS_ASSERT(rxBuff != NULL);
    OS_ASSERT(rxSize > 0U);

    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];
    uartState->rxBuff       = rxBuff;
    uartState->rxSize       = rxSize;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_IRQHandler
 * Description   : Interrupt handler for UART.
 * This handler uses the buffers stored in the uart_state_t structs to transfer
 * data. This is not a public API as it is called by IRQ whenever an interrupt
 * occurs.
 *
 *END**************************************************************************/
static void UART_DRV_IRQHandler(void *data)
{
    uint32_t instance = (uint32_t)data;
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    const uart_type_t *base = s_pstUartBase[instance];

    UART_DRV_ErrIrqHandler(instance);

    /* Handle receive data full interrupt */
    if (UART_GetIntMode(base, UART_INT_RX_DATA_REG_FULL)) {
        if (UART_GetStatusFlag(base, UART_RX_DATA_REG_FULL)) {
            UART_DRV_RxIrqHandler(instance);
        }
    }

    /* Handle transmitter data register empty interrupt */
    if (UART_GetIntMode(base, UART_INT_TX_DATA_REG_EMPTY)) {
        if (UART_GetStatusFlag(base, UART_TX_DATA_REG_EMPTY)) {
            UART_DRV_TxEmptyIrqHandler(instance);
        }
    }

    /* Handle transmission complete interrupt */
    if (UART_GetIntMode(base, UART_INT_TX_COMPLETE)) {
        if (UART_GetStatusFlag(base, UART_TX_COMPLETE)) {
            UART_DRV_TxCompleteIrqHandler(instance);
        }
    }

    /* Receive idle interrupt */
    if (UART_GetIntMode(base, UART_INT_IDLE_LINE)) {
        if (UART_GetStatusFlag(base, UART_IDLE_LINE_DETECT)) {
            UART_DRV_RxIdleIrqHandler(instance);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_SetPinCfg
 * Description   : set pin Configuration
 * 00b - Input trigger is disabled.
 * 01b - Input trigger is used instead of RXD pin input.
 * 10b - Input trigger is used instead of CTS_B pin input.
 * 11b - Input trigger is used to modulate the TXD pin output. The TXD pin output (after TXINV
 *       configuration) is ANDed with the input trigger.
 *
 *END**************************************************************************/
void UART_DRV_SetPinCfg(uint32_t instance, uint8_t conf)
{
    uart_type_t *base = s_pstUartBase[instance];
    base->PINCFG      = (uint32_t)conf;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_SetCts
 * Description   : Set Transmitter clear-to-send enable or disable
 *
 *END**************************************************************************/
void UART_DRV_SetCts(uint32_t instance, bool enable)
{
    uart_type_t *base = s_pstUartBase[instance];
    if (enable) {
        base->MODIR = base->MODIR | 0x01;
    } else {
        base->MODIR = base->MODIR & (~0x01);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_SetRts
 * Description   : set pin Configuration
 *
 *END**************************************************************************/
void UART_DRV_SetRts(uint32_t instance, bool enable)
{
    uart_type_t *base = s_pstUartBase[instance];
    if (enable) {
        base->MODIR = base->MODIR & (~0x02);
        base->MODIR = base->MODIR | 0x08;
    } else {
        base->MODIR = base->MODIR & (~0x04);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_RxIrqHandler
 * Description   : Rx Interrupt handler for UART.
 * This function treats the rx full interrupt.
 *
 *END**************************************************************************/
static void UART_DRV_RxIrqHandler(uint32_t instance)
{
    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];

    /* Get data and put in receive buffer  */
    UART_DRV_GetData(instance);

    /* Update the internal state */
    if (UART_8_BITS_PER_CHAR == uartState->bitCountPerChar) {
        ++uartState->rxBuff;
        --uartState->rxSize;
    } else {
        uartState->rxBuff = &uartState->rxBuff[2];
        uartState->rxSize -= 2U;
    }

    /* Check if this was the last byte in the current buffer */
    if (0U == uartState->rxSize) {
        /* Invoke callback if there is one (callback may reset the rx buffer for continuous reception) */
        if (NULL != uartState->rxCallback) {
            uartState->rxCallback(uartState, UART_EVENT_RX_FULL, uartState->rxCallbackParam);
        }
    }

    /* Finish reception if this was the last byte received */
    if (0U == uartState->rxSize) {
        /* Complete transfer (disable rx logic) */
        UART_DRV_CompleteReceiveDataUsingInt(instance);

        /* Invoke callback if there is one */
        if (NULL != uartState->rxCallback) {
            uartState->rxCallback(uartState, UART_EVENT_END_TRANSFER, uartState->rxCallbackParam);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_TxEmptyIrqHandler
 * Description   : Tx Empty Interrupt handler for UART.
 * This function treats the tx empty interrupt.
 *
 *END**************************************************************************/
static void UART_DRV_TxEmptyIrqHandler(uint32_t instance)
{
    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];
    uart_type_t *base       = s_pstUartBase[instance];

    /* Check if there are any more bytes to send */
    if (uartState->txSize > 0U) {
        /* Transmit the data */
        UART_DRV_PutData(instance);

        /* Update the internal state */
        if (UART_8_BITS_PER_CHAR == uartState->bitCountPerChar) {
            ++uartState->txBuff;
            --uartState->txSize;
        } else {
            uartState->txBuff = &uartState->txBuff[2];
            uartState->txSize -= 2U;
        }

        /* Check if this was the last byte in the current buffer */
        if (0U == uartState->txSize) {
            /* Invoke callback if there is one (callback may reset the tx buffer for continuous transmission)*/
            if (NULL != uartState->txCallback) {
                uartState->txCallback(uartState, UART_EVENT_TX_EMPTY, uartState->txCallbackParam);
            }

            /* If there's no new data, disable tx empty interrupt and enable transmission complete interrupt */
            if (0U == uartState->txSize) {
                UART_SetIntMode(base, UART_INT_TX_DATA_REG_EMPTY, false);
                UART_SetIntMode(base, UART_INT_TX_COMPLETE, true);
            }
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_TxCompleteIrqHandler
 * Description   : Tx Complete Interrupt handler for UART.
 * This function treats the tx complete interrupt.
 *
 *END**************************************************************************/
static void UART_DRV_TxCompleteIrqHandler(uint32_t instance)
{
    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];

    if (0U == uartState->txSize) {
        if (UART_USING_INTERRUPTS == uartState->transferType) {
            /* Complete the interrupt based transfer */
            UART_DRV_CompleteSendDataUsingInt(instance);
        } else {
            /* Complete the PDMA based transfer */
            UART_DRV_StopTxDma(instance);
        }
        /* Invoke callback if there is one */
        if (NULL != uartState->txCallback) {
            uartState->txCallback(uartState, UART_EVENT_END_TRANSFER, uartState->txCallbackParam);
        }
    }
}

static void UART_DRV_RxIdleIrqHandler(uint32_t instance)
{
    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];
    uart_type_t *base       = s_pstUartBase[instance];

    /* Invoke callback if there is one */
    UART_ClearStatusFlag(base, UART_IDLE_LINE_DETECT);
    if (NULL != uartState->rxCallback) {
        uartState->rxCallback(uartState, UART_EVENT_RX_IDLE, uartState->rxCallbackParam);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_ErrIrqHandler
 * Description   : Error Interrupt handler for UART.
 * This function treats the error interrupts.
 *
 *END**************************************************************************/
static void UART_DRV_ErrIrqHandler(uint32_t instance)
{
    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];
    uart_type_t *base       = s_pstUartBase[instance];

    /* Handle receive overrun interrupt */
    if (UART_GetStatusFlag(base, UART_RX_OVERRUN)) {
        /* Update the internal status */
        uartState->receiveStatus = STATUS_UART_RX_OVERRUN;
        if (UART_USING_INTERRUPTS == uartState->transferType) {
            /* Complete the transfer (disable rx logic) */
            UART_DRV_CompleteReceiveDataUsingInt(instance);
        } else {
            /* Complete the transfer (stop PDMA channel) */
            UART_DRV_StopRxDma(instance);
        }
        /* Invoke callback if there is one */
        if (NULL != uartState->rxCallback) {
            uartState->rxCallback(uartState, UART_EVENT_ERROR, uartState->rxCallbackParam);
        }

        /* Clear the flags */
        UART_DRV_ClearErrorFlags(base);
    }
    /* Handle framing error interrupt */
    if (UART_GetStatusFlag(base, UART_FRAME_ERR)) {
        /* Update the internal status */
        uartState->receiveStatus = STATUS_UART_FRAMING_ERROR;
        if (UART_USING_INTERRUPTS == uartState->transferType) {
            /* Complete the transfer (disable rx logic) */
            UART_DRV_CompleteReceiveDataUsingInt(instance);
        } else {
            /* Complete the transfer (stop PDMA channel) */
            UART_DRV_StopRxDma(instance);
        }
        /* Invoke callback if there is one */
        if (NULL != uartState->rxCallback) {
            uartState->rxCallback(uartState, UART_EVENT_ERROR, uartState->rxCallbackParam);
        }

        /* Clear the flags */
        UART_DRV_ClearErrorFlags(base);
    }
    /* Handle parity error interrupt */
    if (UART_GetStatusFlag(base, UART_PARITY_ERR)) {
        /* Update the internal status */
        uartState->receiveStatus = STATUS_UART_PARITY_ERROR;
        if (UART_USING_INTERRUPTS == uartState->transferType) {
            /* Complete the transfer (disable rx logic) */
            UART_DRV_CompleteReceiveDataUsingInt(instance);
        } else {
            /* Complete the transfer (stop PDMA channel) */
            UART_DRV_StopRxDma(instance);
        }
        /* Invoke callback if there is one */
        if (NULL != uartState->rxCallback) {
            uartState->rxCallback(uartState, UART_EVENT_ERROR, uartState->rxCallbackParam);
        }

        /* Clear the flags */
        UART_DRV_ClearErrorFlags(base);
    }
    /* Handle noise error interrupt */
    if (UART_GetStatusFlag(base, UART_NOISE_DETECT)) {
        /* Update the internal status */
        uartState->receiveStatus = STATUS_UART_NOISE_ERROR;
        if (UART_USING_INTERRUPTS == uartState->transferType) {
            /* Complete transfer (disable rx logic) */
            UART_DRV_CompleteReceiveDataUsingInt(instance);
        } else {
            /* Complete the transfer (stop PDMA channel) */
            UART_DRV_StopRxDma(instance);
        }
        /* Invoke callback if there is one */
        if (NULL != uartState->rxCallback) {
            uartState->rxCallback(uartState, UART_EVENT_ERROR, uartState->rxCallbackParam);
        }

        /* Clear the flags */
        UART_DRV_ClearErrorFlags(base);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_StartSendDataUsingInt
 * Description   : Initiate (start) a transmit by beginning the process of
 * sending data and enabling the interrupt.
 * This is not a public API as it is called from other driver functions.
 *
 *END**************************************************************************/
static status_t UART_DRV_StartSendDataUsingInt(uint32_t instance, const uint8_t *txBuff, uint32_t txSize)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);
    OS_ASSERT(NULL != txBuff);

    uart_type_t *base       = s_pstUartBase[instance];
    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];

    /* Check it's not busy transmitting data from a previous function call */
    if (uartState->isTxBusy) {
        return STATUS_BUSY;
    }

    /* Check the validity of the parameters */
    OS_ASSERT(txSize > 0U);
    OS_ASSERT((UART_8_BITS_PER_CHAR == uartState->bitCountPerChar) || (0U == (txSize & 1U)));

    /* initialize the module driver state structure */
    uartState->txBuff         = txBuff;
    uartState->txSize         = txSize;
    uartState->isTxBusy       = true;
    uartState->transmitStatus = STATUS_BUSY;

    /* Enable the UART transmitter */
    UART_SetTransmitterCmd(base, true);

    /* Enable tx empty interrupt */
    UART_SetIntMode(base, UART_INT_TX_DATA_REG_EMPTY, true);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_StartSendDataUsingDma
 * Description   : Initiate (start) a transmit by beginning the process of
 * sending data using PDMA transfers.
 * This is not a public API as it is called from other driver functions.
 *
 *END**************************************************************************/
static status_t UART_DRV_StartSendDataUsingDma(uint32_t instance, const uint8_t *txBuff, uint32_t txSize)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);
    OS_ASSERT(NULL != txBuff);

    uart_type_t *base       = s_pstUartBase[instance];
    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];

    /* Check it's not busy transmitting data from a previous function call */
    if (uartState->isTxBusy) {
        return STATUS_BUSY;
    }

    OS_ASSERT(txSize > 0U);

    /* Update state structure */
    uartState->txBuff         = txBuff;
    uartState->txSize         = 0U;
    uartState->isTxBusy       = true;
    uartState->transmitStatus = STATUS_BUSY;

    /* Configure the transfer control descriptor for the previously allocated channel */
    (void)PDMA_DRV_ConfigMultiBlockTransfer(uartState->txDMAChannel, PDMA_TRANSFER_MEM2PERIPH, (uint32_t)txBuff,
                                            (uint32_t)(&(base->DATA)), PDMA_TRANSFER_SIZE_1B, 1U, txSize, true);

    /* Call driver function to end the transmission when the PDMA transfer is done */
    (void)PDMA_DRV_InstallCallback(uartState->txDMAChannel, (pdma_callback_t)(UART_DRV_TxDmaCallback),
                                   (void *)(instance));

    /* Start the PDMA channel */
    (void)PDMA_DRV_StartChannel(uartState->txDMAChannel);

    /* Enable the UART transmitter */
    UART_SetTransmitterCmd(base, true);

    /* Enable tx PDMA requests for the current instance */
    UART_SetTxDmaCmd(base, true);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_CompleteSendDataUsingInt
 * Description   : Finish up a transmit by completing the process of sending
 * data and disabling the interrupt.
 * This is not a public API as it is called from other driver functions.
 *
 *END**************************************************************************/
static void UART_DRV_CompleteSendDataUsingInt(uint32_t instance)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    uart_type_t *base       = s_pstUartBase[instance];
    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];

    if (STATUS_BUSY == uartState->transmitStatus) {
        /* If the transfer is completed, update the transmit status */
        uartState->transmitStatus = STATUS_SUCCESS;
    } else {
        /* If the transfer is aborted or timed out, disable tx empty interrupt */
        UART_SetIntMode(base, UART_INT_TX_DATA_REG_EMPTY, false);
    }

    /* Disable transmission complete interrupt */
    UART_SetIntMode(base, UART_INT_TX_COMPLETE, false);

    /* Disable transmitter */
    UART_SetTransmitterCmd(base, false);

    /* Update the internal busy flag */
    uartState->isTxBusy = false;

    /* Signal the synchronous completion object. */
    if (uartState->isTxBlocking) {
        (void)OS_SemaphoreRelease(uartState->txComplete);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_TxDmaCallback
 * Description   : Finish up a transmit by completing the process of sending
 * data and disabling the PDMA requests. This is a callback for PDMA major loop
 * completion, so it must match the PDMA callback signature.
 * This is not a public API as it is called from other driver functions.
 *
 *END**************************************************************************/
static void UART_DRV_TxDmaCallback(void *parameter, pdma_chn_status_t status)
{
    uint32_t instance       = ((uint32_t)parameter);
    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];
    uart_type_t *base       = s_pstUartBase[instance];

    /* Check if the PDMA transfer completed with errors */
    if (PDMA_CHN_ERROR == status) {
        /* Update the status */
        uartState->transmitStatus = STATUS_ERROR;
        /* Stop the transfer */
        UART_DRV_StopTxDma(instance);
        /* Notify the application that an error occurred */
        if (NULL != uartState->txCallback) {
            uartState->txCallback(uartState, UART_EVENT_ERROR, uartState->txCallbackParam);
        }
    } else {
        /* Invoke callback if there is one */
        if (NULL != uartState->txCallback) {
            /* Allow the user to provide a new buffer, for continuous transmission */
            uartState->txCallback(uartState, UART_EVENT_TX_EMPTY, uartState->txCallbackParam);
        }

        /* If the callback has updated the tx buffer, update the PDMA descriptor to continue the transfer;
         * otherwise, stop the current transfer.
         */
        if (uartState->txSize > 0U) {
            /* Set the source address and the number of minor loops (bytes to be transferred) */
            PDMA_DRV_SetSrcAddr(uartState->txDMAChannel, (uint32_t)(uartState->txBuff));
            PDMA_DRV_SetMajorLoopIterationCount(uartState->txDMAChannel, uartState->txSize);

            /* Now that this tx is set up, clear remaining bytes count */
            uartState->txSize = 0U;

            /* Re-start the channel */
            (void)PDMA_DRV_StartChannel(uartState->txDMAChannel);
        } else {
            /* Enable transmission complete interrupt */
            UART_SetIntMode(base, UART_INT_TX_COMPLETE, true);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_StartReceiveDataUsingInt
 * Description   : Initiate (start) a receive by beginning the process of
 * receiving data and enabling the interrupt.
 * This is not a public API as it is called from other driver functions.
 *
 *END**************************************************************************/
static status_t UART_DRV_StartReceiveDataUsingInt(uint32_t instance, uint8_t *rxBuff, uint32_t rxSize)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);
    OS_ASSERT(NULL != rxBuff);

    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];
    uart_type_t *base       = s_pstUartBase[instance];

    /* Check it's not busy receiving data from a previous function call */
    if (uartState->isRxBusy) {
        return STATUS_BUSY;
    }

    /* Check the validity of the parameters */
    OS_ASSERT(rxSize > 0U);
    OS_ASSERT((UART_8_BITS_PER_CHAR == uartState->bitCountPerChar) || (0U == (rxSize & 1U)));

    /* Initialize the module driver state struct to indicate transfer in progress
     * and with the buffer and byte count data. */
    uartState->isRxBusy      = true;
    uartState->rxBuff        = rxBuff;
    uartState->rxSize        = rxSize;
    uartState->receiveStatus = STATUS_BUSY;

    /* Enable the receiver */
    UART_SetReceiverCmd(base, true);

    /* Enable error interrupts */
    UART_SetErrorInterrupts(base, true);

    /* Enable receive data full interrupt */
    UART_SetIntMode(base, UART_INT_RX_DATA_REG_FULL, true);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_StartReceiveDataUsingDma
 * Description   : Initiate (start) a receive by beginning the process of
 * receiving data using PDMA transfers.
 * This is not a public API as it is called from other driver functions.
 *
 *END**************************************************************************/
static status_t UART_DRV_StartReceiveDataUsingDma(uint32_t instance, uint8_t *rxBuff, uint32_t rxSize)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);
    OS_ASSERT(NULL != rxBuff);

    uart_type_t *base       = s_pstUartBase[instance];
    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];

    /* Check it's not busy transmitting data from a previous function call */
    if (uartState->isRxBusy) {
        return STATUS_BUSY;
    }

    OS_ASSERT(rxSize > 0U);

    /* Configure the transfer control descriptor for the previously allocated channel */
    (void)PDMA_DRV_ConfigMultiBlockTransfer(uartState->rxDMAChannel, PDMA_TRANSFER_PERIPH2MEM,
                                            (uint32_t)(&(base->DATA)), (uint32_t)rxBuff, PDMA_TRANSFER_SIZE_1B, 1U,
                                            rxSize, true);

    /* Call driver function to end the reception when the PDMA transfer is done */
    (void)PDMA_DRV_InstallCallback(uartState->rxDMAChannel, (pdma_callback_t)(UART_DRV_RxDmaCallback),
                                   (void *)(instance));

    /* Start the PDMA channel */
    (void)PDMA_DRV_StartChannel(uartState->rxDMAChannel);

    /* Update the state structure */
    uartState->rxBuff        = rxBuff;
    uartState->rxSize        = 0U;
    uartState->isRxBusy      = true;
    uartState->receiveStatus = STATUS_BUSY;

    /* Enable the receiver */
    UART_SetReceiverCmd(base, true);

    /* Enable error interrupts */
    // UART_SetErrorInterrupts(base, true); //TODO:

    /* Enable rx PDMA requests for the current instance */
    UART_SetRxDmaCmd(base, true);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_CompleteReceiveDataUsingInt
 * Description   : Finish up a receive by completing the process of receiving data
 * and disabling the interrupt.
 * This is not a public API as it is called from other driver functions.
 *
 *END**************************************************************************/
static void UART_DRV_CompleteReceiveDataUsingInt(uint32_t instance)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    uint8_t tmpByte;
    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];
    uart_type_t *base       = s_pstUartBase[instance];

    /* Disable receiver */
    UART_SetReceiverCmd(base, false);

    /* Disable error interrupts */
    UART_SetErrorInterrupts(base, false);

    /* Read dummy to clear RDRF flag */
    UART_Getchar(base, &tmpByte);

    /* Disable receive data full and rx overrun interrupt. */
    UART_SetIntMode(base, UART_INT_RX_DATA_REG_FULL, false);

    /* Signal the synchronous completion object. */
    if (uartState->isRxBlocking) {
        (void)OS_SemaphoreRelease(uartState->rxComplete);
        uartState->isRxBlocking = false;
    }

    /* Update the information of the module driver state */
    uartState->isRxBusy = false;
    if (STATUS_BUSY == uartState->receiveStatus) {
        uartState->receiveStatus = STATUS_SUCCESS;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_RxDmaCallback
 * Description   : Finish up a receive by completing the process of receiving data
 * and disabling the PDMA requests. This is a callback for PDMA major loop
 * completion, so it must match the PDMA callback signature.
 * This is not a public API as it is called from other driver functions.
 *
 *END**************************************************************************/
static void UART_DRV_RxDmaCallback(void *parameter, pdma_chn_status_t status)
{
    uint32_t instance       = ((uint32_t)parameter);
    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];
    uart_type_t *base       = s_pstUartBase[instance];

    if (PDMA_CHN_ERROR == status) {
        /* Update the status */
        uartState->receiveStatus = STATUS_ERROR;
        /* Stop the transfer */
        UART_DRV_StopRxDma(instance);
        /* Notify the application that an error occurred */
        if (NULL != uartState->rxCallback) {
            uartState->rxCallback(uartState, UART_EVENT_ERROR, uartState->rxCallbackParam);
        }

        /* Clear the flags */
        UART_DRV_ClearErrorFlags(base);
    }

    /* Return if an error occurred; error cases are treated by the interrupt handler */
    if (STATUS_BUSY != uartState->receiveStatus) {
        return;
    }

    /* Invoke callback if there is one */
    if (NULL != uartState->rxCallback) {
        /* Allow the user to provide a new buffer inside the callback, to continue the reception */
        uartState->rxCallback(uartState, UART_EVENT_RX_FULL, uartState->rxCallbackParam);
    }

    /* If the callback has updated the rx buffer, update the PDMA descriptor to continue the transfer;
     * otherwise, stop the current transfer.
     */
    if (uartState->rxSize > 0U) {
        /* Set the source address and the number of minor loops (bytes to be transferred) */
        PDMA_DRV_SetDestAddr(uartState->rxDMAChannel, (uint32_t)(uartState->rxBuff));
        PDMA_DRV_SetMajorLoopIterationCount(uartState->rxDMAChannel, uartState->rxSize);

        /* Now that this rx is set up, clear remaining bytes count */
        uartState->rxSize = 0U;

        /* Re-start the channel */
        (void)PDMA_DRV_StartChannel(uartState->rxDMAChannel);
    } else {
        /* Stop the reception */
        UART_DRV_StopRxDma(instance);

        /* Invoke the callback to notify the end of the transfer */
        if (NULL != uartState->rxCallback) {
            uartState->rxCallback(uartState, UART_EVENT_END_TRANSFER, uartState->rxCallbackParam);
        }

        /* Clear the flags */
        UART_DRV_ClearErrorFlags(base);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_PutData
 * Description   : set loop back mode
 *
 *END**************************************************************************/
void UART_DRV_SetLoopback(uint32_t instance, bool en)
{
    uart_type_t *base = s_pstUartBase[instance];
    UART_SetLoopback(base, en);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_SetIdleCharsLen
 * Description   : Configures the number of idle characters that must be received before the 
 * IDLE flag is set.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
void UART_DRV_SetIdleCharsLen(uint32_t instance, uart_character_type_t idleCharsType)
{
    uart_type_t *base = s_pstUartBase[instance];
    UART_SetIdleCharsLen(base, idleCharsType);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_SetMSBF
 * Description   : set MSB or LSB mode.
 *
 *END**************************************************************************/
void UART_DRV_SetMSBF(uint32_t instance, uart_msbf_mode_t mode)
{
    uart_type_t *base = s_pstUartBase[instance];
    UART_MSBFMode(base, mode);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_SetIdleIREnable
 * Description   : This function enables the idle line interrupt.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
void UART_DRV_SetIdleIREnable(uint32_t instance, uart_idle_interrupt_type_t idleIRType)
{
    uart_type_t *base = s_pstUartBase[instance];
    UART_SetIdleIREnable(base, idleIRType);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_OverrunIREnable
 * Description   : Enables overrun interupt.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
void UART_DRV_OverrunIREnable(uint32_t instance, uart_overrrun_interrupt_type_t OverrunIrqType)
{
    uart_type_t *base = s_pstUartBase[instance];
    UART_OverrunIREnable(base, OverrunIrqType);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_SetRxInv
 * Description   : Configures receive data inverse in the UART controller.
 * In some UART instances, the user should disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
void UART_DRV_SetRxInv(uint32_t instance, uart_tx_rx_inve_t invModeType)
{
    uart_type_t *base = s_pstUartBase[instance];
    UART_SetRxInv(base, invModeType);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_SetTxInv
 * Description   : Configures transmit data inverse in the UART controller.
 * In some UART instances, the user should disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
void UART_DRV_SetTxInv(uint32_t instance, uart_tx_rx_inve_t invModeType)
{
    uart_type_t *base = s_pstUartBase[instance];
    UART_SetTxInv(base, invModeType);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_IsRxOverrun
 * Description   : Indicates whether there is a receiver overrun event. 
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
bool UART_DRV_IsRxOverrun(uint32_t instance)
{
    uart_type_t *base = s_pstUartBase[instance];
    return UART_IsRxOverrun(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_IsTxFIFOEmpty
 * Description   : Asserts when there is no data in the Transmit FIFO/buffer. 
 * This field does not take into account data that is in the transmit shift register.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
bool UART_DRV_IsTxFIFOEmpty(uint32_t instance)
{
    uart_type_t *base = s_pstUartBase[instance];

    return UART_IsTxFIFOEmpty(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_IsRxActiveEage
 * Description   : If the function returns 1, it indicates that there is an active edge on the RXD pin. 
 * If it is 0, it indicates that there is no active edge on the RXD pin. 
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
bool UART_DRV_IsRxActiveEage(uint32_t instance)
{
    uart_type_t *base = s_pstUartBase[instance];
    ;
    return UART_IsRxActiveEage(base);
}

uint32_t UART_DRV_RxTxFIFODepth(uint32_t instance, bool rxFIFO)
{
    uart_type_t *base = s_pstUartBase[instance];
    if (rxFIFO) {
        return UART_RxFIFODepth(base);
    } else {
        return UART_TxFIFODepth(base);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_PutData
 * Description   : Write data to the buffer register, according to configured
 * word length.
 * This is not a public API as it is called from other driver functions.
 *
 *END**************************************************************************/
static void UART_DRV_PutData(uint32_t instance)
{
    const uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];
    uart_type_t *base             = s_pstUartBase[instance];
    uint16_t data;
    const uint8_t *txBuff = uartState->txBuff;

    if (UART_8_BITS_PER_CHAR == uartState->bitCountPerChar) {
        /* Transmit the data */
        UART_Putchar(base, *txBuff);
    } else if (UART_9_BITS_PER_CHAR == uartState->bitCountPerChar) {
        /* Create a 16-bits integer from two bytes */
        data = (uint16_t)(*txBuff);
        ++txBuff;
        data |= (uint16_t)(((uint16_t)(*txBuff)) << 8U);

        /* Transmit the data */
        UART_Putchar9(base, data);
    } else {
        /* Create a 16-bits integer from two bytes */
        data = (uint16_t)(*txBuff);
        ++txBuff;
        data |= (uint16_t)(((uint16_t)(*txBuff)) << 8U);

        /* Transmit the data */
        UART_Putchar10(base, data);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_GetData
 * Description   : Read data from the buffer register, according to configured
 * word length.
 * This is not a public API as it is called from other driver functions.
 *
 *END**************************************************************************/
static void UART_DRV_GetData(uint32_t instance)
{
    const uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];
    const uart_type_t *base       = s_pstUartBase[instance];
    uint16_t data;
    uint8_t *rxBuff = uartState->rxBuff;

    if (UART_8_BITS_PER_CHAR == uartState->bitCountPerChar) {
        /* Receive the data */
        UART_Getchar(base, rxBuff);
    } else if (UART_9_BITS_PER_CHAR == uartState->bitCountPerChar) {
        /* Receive the data */
        UART_Getchar9(base, &data);

        /* Write the least significant bits to the receive buffer */
        *rxBuff = (uint8_t)(data & 0xFFU);
        ++rxBuff;
        /* Write the ninth bit to the subsequent byte in the rx buffer */
        *rxBuff = (uint8_t)(data >> 8U);
    } else {
        /* Receive the data */
        UART_Getchar10(base, &data);

        /* Write the least significant bits to the receive buffer */
        *rxBuff = (uint8_t)(data & 0xFFU);
        ++rxBuff;
        /* Write the ninth and tenth bits to the subsequent byte in the rx buffer */
        *rxBuff = (uint8_t)(data >> 8U);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_StopTxDma
 * Description   : Finish up a PDMA transmission by disabling the PDMA requests,
 * transmission complete interrupt and tx logic. This function also resets the
 * internal driver state (busy flag/tx semaphore).
 * This is not a public API as it is called from other driver functions.
 *
 *END**************************************************************************/
static void UART_DRV_StopTxDma(uint32_t instance)
{
    uart_type_t *base       = s_pstUartBase[instance];
    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];

    /* Disable tx PDMA requests for the current instance */
    UART_SetTxDmaCmd(base, false);

    /* Stop the dma channel */
    (void)PDMA_DRV_StopChannel(uartState->txDMAChannel);

    /* Disable transmission complete interrupt */
    UART_SetIntMode(base, UART_INT_TX_COMPLETE, false);

    /* Disable transmitter */
    UART_SetTransmitterCmd(base, false);

    /* Signal the synchronous completion object. */
    if (uartState->isTxBlocking) {
        (void)OS_SemaphoreRelease(uartState->txComplete);
    }

    if (STATUS_BUSY == uartState->transmitStatus) {
        /* If the transfer is completed, update the transmit status */
        uartState->transmitStatus = STATUS_SUCCESS;
    }

    /* Update the internal busy flag */
    uartState->isTxBusy = false;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_StopRxDma
 * Description   : Finish up a PDMA reception by disabling the PDMA requests,
 * error interrupts and rx logic. This function also resets the internal driver
 * state (busy flag/rx semaphore).
 * This is not a public API as it is called from other driver functions.
 *
 *END**************************************************************************/
static void UART_DRV_StopRxDma(uint32_t instance)
{
    uart_type_t *base       = s_pstUartBase[instance];
    uart_state_t *uartState = (uart_state_t *)s_pstUartState[instance];
    uint8_t tmpByte;

    /* Disable receiver */
    UART_SetReceiverCmd(base, false);

    /* Disable error interrupts */
    UART_SetErrorInterrupts(base, false);

    /* Disable rx PDMA requests for the current instance */
    UART_SetRxDmaCmd(base, false);

    /* Read dummy to clear RDRF flag */
    UART_Getchar(base, &tmpByte);

    /* Stop the PDMA channel */
    (void)PDMA_DRV_StopChannel(uartState->rxDMAChannel);

    /* Signal the synchronous completion object. */
    if (uartState->isRxBlocking) {
        (void)OS_SemaphoreRelease(uartState->rxComplete);
        uartState->isRxBlocking = false;
    }

    /* Update the internal driver status */
    if (uartState->receiveStatus == STATUS_BUSY) {
        uartState->receiveStatus = STATUS_SUCCESS;
    }

    /* Update the information of the module driver state */
    uartState->isRxBusy = false;
}
/**
 * @brief Get uart version id
 * @param instance The UART instance number.
 * @return uint32_t Version ID
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_GetVersionID
 * Description   : Get the UART module version id
 *
 *END**************************************************************************/
uint32_t UART_DRV_GetVersionID(uint32_t instance)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);
    uart_type_t *base = s_pstUartBase[instance];

    return base->VERID;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_GetBasePointer
 * Description   : Get the UART instance base pointer
 * This is not a public API as it is called from other driver functions.
 *
 *END**************************************************************************/
uart_type_t *UART_GetBasePointer(uint32_t instance)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);
    uart_type_t *base = s_pstUartBase[instance];
    return base;
}
