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
 * @file lin_uart_driver.c
 * @brief LIN uart driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "lin_uart_driver.h"

/** Table of base addresses for UART instances. */
static uart_type_t *const s_pstLinUartBase[UART_INSTANCE_COUNT] = UART_BASE_PTRS;

/** Table to save UART IRQ enumeration numbers defined in the CMSIS header file */
static const IRQn_Type s_euLinUartRxIrqId[UART_INSTANCE_COUNT] = UART_RX_IRQS;
static const IRQn_Type s_euLinUartTxIrqId[UART_INSTANCE_COUNT] = UART_TX_IRQS;

#ifdef UART_ERR_IRQS
/** Table to save UART_ERR IRQ enumeration numbers defined in the device header file */
static const IRQn_Type s_linUartErrIrqId[UART_INSTANCE_COUNT] = UART_ERR_IRQS;
#endif

/** Table to save UART state structure pointers */
static lin_state_t *s_pstLinState[UART_INSTANCE_COUNT] = {NULL};

/** Table to save LIN user config structure pointers */
static lin_user_config_t *s_pstLinUserConfig[UART_INSTANCE_COUNT] = {NULL};

static uint32_t s_previousTwoBitTimeLength[UART_INSTANCE_COUNT]      = {0U};
static uint8_t s_wakeupSignal[UART_INSTANCE_COUNT]                   = {0U};
static uint8_t s_countMeasure[UART_INSTANCE_COUNT]                   = {0U};
static uint32_t s_timeMeasure[UART_INSTANCE_COUNT]                   = {0U};
static const clock_names_t s_stLinUartClkName[UART_INSTANCE_COUNT] = UART_CLOCK_NAMES;

static status_t LIN_UART_DRV_WaitComplete(uint32_t instance, uint32_t timeoutMSec);

static void LIN_UART_DRV_ProcessBreakDetect(uint32_t instance);

static void LIN_UART_DRV_CheckWakeupSignal(uint32_t instance);

static void LIN_UART_DRV_ProcessFrame(uint32_t instance, uint8_t tmpByte);

static void LIN_UART_DRV_ProcessFrameHeader(uint32_t instance, uint8_t tmpByte);

static void LIN_UART_DRV_ProcessReceiveFrameData(uint32_t instance, uint8_t tmpByte);

static void LIN_UART_DRV_ProcessSendFrameData(uint32_t instance, uint8_t tmpByte);

static void LIN_UART_DRV_AutobaudTimerValEval(uint32_t instance, uint32_t twoBitTimeLength);

static void LIN_UART_DRV_EvalTwoBitTimeLength(uint32_t instance, uint32_t twoBitTimeLength);

static uint8_t LIN_UART_DRV_MakeChecksumByte(uint32_t instance, const uint8_t *buffer, uint8_t sizeBuffer, uint8_t PID);

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_Init
 * Description   : This function initializes a UART instance for operation.
 * This function will initialize the run-time state structure to keep track of
 * the on-going transfers, initialize the module to user defined settings and
 * default settings, set break field length to be 13 bit times minimum, enable
 * the break detect interrupt, Rx complete interrupt, frame error detect interrupt,
 * and enable the UART module transmitter and receiver.
 *
 *END**************************************************************************/
status_t LIN_UART_DRV_Init(uint32_t instance, lin_user_config_t *linUserConfig, lin_state_t *linCurrentState)
{
    /* Assert parameters. */
    OS_ASSERT(linUserConfig && linCurrentState);
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    uint32_t linSourceClockFreq        = 0U;
    status_t retVal                    = STATUS_SUCCESS;
    OS_SemaphoreAttr_t semUartRxAttr = {0}, semUartTxAttr = {0};
    OS_RegisterType_t uType = {0};

    clock_names_t uartClocks[UART_INSTANCE_COUNT] = UART_CLOCK_NAMES;
    CLOCK_SYS_ClockEnable(uartClocks[instance], true);

    /* Get base address of the UART instance. */
    uart_type_t *base = s_pstLinUartBase[instance];

    /* Get the UART clock as configured in the clock manager */
    (void)CLOCK_SYS_GetFreq(s_stLinUartClkName[instance], &linSourceClockFreq);

    /* Check if current instance is clock gated off. */
    OS_ASSERT(linSourceClockFreq > 0U);

    /* Check if current instance is already initialized. */
    OS_ASSERT(NULL == s_pstLinState[instance]);

    /* Save runtime structure pointer. */
    s_pstLinState[instance] = linCurrentState;

    /* Save LIN user config structure pointer. */
    s_pstLinUserConfig[instance] = linUserConfig;

    /* Clear linSourceClockFreq value */
    linCurrentState->linSourceClockFreq = linSourceClockFreq;

    /* Init UART */
    UART_Init(base);

    /* Create txCompleted and rxCompleted semaphores as counting, with init value is 0 */

    semUartRxAttr.name   = "uart_rx";
    semUartRxAttr.cb_mem = NULL;
    /* Create a Sema */
    linCurrentState->rxCompleted = OS_SemaphoreNew(1, 0, &semUartRxAttr);

    semUartTxAttr.name   = "uart_tx";
    semUartTxAttr.cb_mem = NULL;
    /* Create a Sema */
    linCurrentState->txCompleted = OS_SemaphoreNew(1, 0, &semUartTxAttr);

    if (NULL == linCurrentState->rxCompleted) {
        // log_err("rxComplete Semaphore create failed\r\n");
        retVal = STATUS_ERROR;
    } else if (NULL == linCurrentState->txCompleted) {
        // log_err("txComplete Semaphore create failed\r\n");
        retVal = STATUS_ERROR;
    } else {
        /* if autobaud is enabled */
        if ((linUserConfig->autobaudEnable) && ((bool)LIN_SLAVE == linUserConfig->nodeFunction)) {
            /* Setting Slave's baudrate to 19200 will help Slave node */
            /* always detect LIN Break from Master */
            linUserConfig->baudRate                    = 19200U;
            linCurrentState->fallingEdgeInterruptCount = 0U;
            linCurrentState->baudrateEvalEnable        = true;
            s_previousTwoBitTimeLength[instance]       = 0U;
            s_countMeasure[instance]                   = 0U;
            s_timeMeasure[instance]                    = 0U;
        }

        /* Set baud rate to User's value */
        (void)UART_DRV_SetBaudRate(instance, linUserConfig->baudRate);

        /* Set 8 bit counts per char */
        UART_SetBitCountPerChar(base, UART_8_BITS_PER_CHAR, false);

        /* Set no parity mode */
        UART_SetParityMode(base, UART_PARITY_DISABLED);

        /* One stop bit */
        UART_SetStopBitCount(base, UART_ONE_STOP_BIT);

        /* Check if the current node is MASTER */
        if ((bool)LIN_MASTER == linUserConfig->nodeFunction) {
            /* Set Break char length as 13 bits minimum */
            UART_SetBreakCharTransmitLength(base, UART_BREAK_CHAR_13_BIT_MINIMUM);
        }

        /* Set Break char detect length as 13 bits minimum */
        UART_SetBreakCharDetectLength(base, UART_BREAK_CHAR_13_BIT_MINIMUM);

        /* Enable RX complete interrupt */
        UART_SetIntMode(base, UART_INT_RX_DATA_REG_FULL, true);

        /* Enable frame error interrupt */
        UART_SetIntMode(base, UART_INT_FRAME_ERR_FLAG, true);

        /* Enable LIN break detect interrupt */
        UART_SetIntMode(base, UART_INT_LIN_BREAK_DETECT, true);

        /* Install LIN_RxTx ISR for UART instance */
        uType.trig_mode = CLIC_LEVEL_TRIGGER;
        uType.lvl       = 2;
        uType.priority  = 1;
        uType.data_ptr  = (void *)instance;
        OS_RequestIrq(s_euLinUartRxIrqId[instance], LIN_DRV_IRQHandler, &uType);

        /* Enable UART interrupts. */
        OS_EnableIrq(s_euLinUartRxIrqId[instance]);

        OS_RequestIrq(s_euLinUartTxIrqId[instance], LIN_DRV_IRQHandler, &uType);

        /* Enable UART interrupts. */
        OS_EnableIrq(s_euLinUartTxIrqId[instance]);
#ifdef UART_ERR_IRQS
        /* Install LIN_ERR ISR for UART instance */
        OS_RequestIrq(s_linUartErrIrqId[instance], LIN_DRV_IRQHandler, &uType);
        /* Enable UART error interrupts */
        OS_EnableIrq(s_linUartErrIrqId[instance]);
#endif
        /* Change node's current state to IDLE */
        linCurrentState->currentNodeState = LIN_NODE_STATE_IDLE;

        /* Clear flags in current LIN state structure */
        linCurrentState->isTxBusy           = false;
        linCurrentState->isRxBusy           = false;
        linCurrentState->isBusBusy          = false;
        linCurrentState->isRxBlocking       = false;
        linCurrentState->isTxBlocking       = false;
        linCurrentState->timeoutCounterFlag = false;
        linCurrentState->timeoutCounter     = 0U;

        /* Assign wakeup signal to satisfy LIN Specifications specifies that
         * wakeup signal shall be in range from 250us to 5 ms.
         */
        if (linUserConfig->baudRate > 10000U) {
            /* Wakeup signal will be range from 400us to 800us depend on baudrate */
            s_wakeupSignal[instance] = 0x80U;
        } else {
            /* Wakeup signal will be range from 400us to 4ms depend on baudrate */
            s_wakeupSignal[instance] = 0xF8U;
        }

        if (!((linUserConfig->autobaudEnable) && ((bool)LIN_SLAVE == linUserConfig->nodeFunction))) {
            /* Enable the UART transmitter and receiver */
            UART_SetTransmitterCmd(base, true);
            UART_SetReceiverCmd(base, true);
        }
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_Deinit
 * Description   : This function shuts down the UART by disabling interrupts and
 *                 transmitter/receiver.
 *
 *END**************************************************************************/
void LIN_UART_DRV_Deinit(uint32_t instance)
{
    /* Assert parameters. */
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    /* Get base address of the UART instance. */
    uart_type_t *base = s_pstLinUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t *linCurrentState = s_pstLinState[instance];

    /* Check if current instance is already de-initialized or is gated.*/
    OS_ASSERT(NULL != linCurrentState);
    OS_ASSERT(linCurrentState->linSourceClockFreq > 0U);

    /* Wait until the data is completely shifted out of shift register */
    while (!UART_GetStatusFlag(base, UART_TX_COMPLETE)) {
        /* Do nothing */
    }

    /* Disable the UART transmitter and receiver */
    UART_SetTransmitterCmd(base, false);
    UART_SetReceiverCmd(base, false);

    /* Destroy TX and RX semaphores. */
    (void)OS_SemaphoreDelete(linCurrentState->txCompleted);
    (void)OS_SemaphoreDelete(linCurrentState->rxCompleted);

    /* Disable UART interrupts. */
    OS_DisableIrq(s_euLinUartRxIrqId[instance]);
    OS_DisableIrq(s_euLinUartTxIrqId[instance]);
#ifdef UART_ERR_IRQS
    OS_DisableIrq(s_linUartErrIrqId[instance]);
#endif

    OS_UnRequestIrq(s_euLinUartTxIrqId[instance]);
    OS_UnRequestIrq(s_euLinUartRxIrqId[instance]);

    /* Change node's current state to UNINIT */
    linCurrentState->currentNodeState = LIN_NODE_STATE_UNINIT;

    /* Clear our saved pointer to the LIN state structure */
    s_pstLinState[instance] = NULL;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_GetDefaultConfig
 * Description   : Initializes a configuration structure received
 * from the application with default values.
 *
 *END**************************************************************************/
void LIN_UART_DRV_GetDefaultConfig(bool isMaster, lin_user_config_t *linUserConfig)
{
    /* Assert parameters. */
    OS_ASSERT(NULL != linUserConfig);

    linUserConfig->baudRate                     = 19200U;
    linUserConfig->autobaudEnable               = false;
    linUserConfig->timerGetTimeIntervalCallback = NULL;
    if (true == isMaster) {
        linUserConfig->nodeFunction = (bool)LIN_MASTER;
    } else {
        linUserConfig->nodeFunction = (bool)LIN_SLAVE;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_InstallCallback
 * Description   : This function installs the callback function that is used for LIN_UART_DRV_IRQHandler.
 * Pass in Null pointer as callback will uninstall.
 *
 *END**************************************************************************/
lin_callback_t LIN_UART_DRV_InstallCallback(uint32_t instance, lin_callback_t function)
{
    /* Assert parameters. */
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    /* Get the current LIN state of this UART instance. */
    lin_state_t *linCurrentState = s_pstLinState[instance];

    /* Get the current callback function. */
    lin_callback_t currentCallback = linCurrentState->Callback;

    /* Install new callback function. */
    linCurrentState->Callback = function;

    return currentCallback;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_MakeChecksumByte
 * Description   : This function calculate checksum for a frame. This function
 * will return classic or enhanced checksum base on data in
 * s_pstLinUserConfig[instance] and input parameter.
 *
 *END**************************************************************************/
static uint8_t LIN_UART_DRV_MakeChecksumByte(uint32_t instance, const uint8_t *buffer, uint8_t sizeBuffer, uint8_t PID)
{
    /* Get list of PIDs use classic checksum. */
    const uint8_t *classicPID     = s_pstLinUserConfig[instance]->classicPID;
    const uint8_t numOfClassicPID = s_pstLinUserConfig[instance]->numOfClassicPID;
    uint8_t checkSum              = PID;
    uint8_t retVal                = 0U;

    if (255U == numOfClassicPID) {
        /* all frame use enhanced checksum */
        checkSum = 0U;
    } else {
        if (NULL != classicPID) {
            for (retVal = 0U; retVal < numOfClassicPID; retVal++) {
                if (checkSum == classicPID[retVal]) {
                    checkSum = 0U;
                    break;
                }
            }
        }
    }
    retVal = LIN_DRV_MakeChecksumByte(buffer, sizeBuffer, checkSum);
    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_SendFrameDataBlocking
 * Description   : This function sends data out through the UART module using
 * blocking method. This function checks if txSize is in range from 1 to 8.
 * If not, it will return STATUS_ERROR. This function also returns STATUS_ERROR
 * if node's current state is in SLEEP mode. This function checks if the
 * isBusBusy is false, if not it will return STATUS_BUSY. The function does not
 * return until the transmission is complete. If the transmission is successful,
 * it will return STATUS_SUCCESS. If not, it will return STATUS_TIMEOUT.
 *
 *END**************************************************************************/
status_t LIN_UART_DRV_SendFrameDataBlocking(uint32_t instance, const uint8_t *txBuff, uint8_t txSize,
                                            uint32_t timeoutMSec)
{
    /* Assert parameters. */
    OS_ASSERT(NULL != txBuff);
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    /* Get base address of the UART instance. */
    uart_type_t *base = s_pstLinUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t *linCurrentState = s_pstLinState[instance];

    status_t retVal = STATUS_SUCCESS;

    /* Check whether current mode is sleep mode */
    bool checkSleepMode = (LIN_NODE_STATE_SLEEP_MODE == linCurrentState->currentNodeState);

    /* Check if txSize > 8 or equal to 0 or node's current state
     * is in SLEEP mode then return STATUS_ERROR */
    if ((8U < txSize) || (0U == txSize) || checkSleepMode) {
        retVal = STATUS_ERROR;
    } else {
        /* Check if the LIN Bus is busy */
        if (linCurrentState->isBusBusy) {
            retVal = STATUS_BUSY;
        } else {
            /* Make the checksum byte. */
            linCurrentState->checkSum =
                LIN_UART_DRV_MakeChecksumByte(instance, txBuff, txSize, linCurrentState->currentPid);

            /* Update the LIN state structure. */
            linCurrentState->txBuff = txBuff;
            /* Add a place for checksum byte */
            linCurrentState->txSize         = (uint8_t)(txSize + 1U);
            linCurrentState->cntByte        = 0U;
            linCurrentState->currentEventId = LIN_NO_EVENT;
            linCurrentState->isBusBusy      = true;
            linCurrentState->isTxBusy       = true;
            linCurrentState->isTxBlocking   = true;

            /* Set Break char detect length as 10 bits minimum */
            UART_SetBreakCharDetectLength(base, UART_BREAK_CHAR_10_BIT_MINIMUM);

            /* Set node's current state to SEND_DATA */
            linCurrentState->currentNodeState = LIN_NODE_STATE_SEND_DATA;

            /* Start sending data */
            UART_Putchar(base, *linCurrentState->txBuff);

            /* Wait until the transmission is complete. */
            retVal = LIN_UART_DRV_WaitComplete(instance, timeoutMSec);

            /* Clear Tx Blocking flag */
            linCurrentState->isTxBlocking = false;

            /* Clear Tx Busy flag. */
            linCurrentState->isTxBusy = false;

            /* Change node's current state to IDLE */
            (void)LIN_UART_DRV_GotoIdleState(instance);
        }
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_SendFrameData
 * Description   : This function sends data out through the UART module using
 * non-blocking method. This function will calculate the checksum byte and send
 * it with the frame data. The function will return immediately after calling
 * this function. If txSize is equal to 0 or greater than 8  or node's current
 * state is in SLEEP mode then the function will return STATUS_ERROR. If
 * isBusBusy is currently true then the function will return STATUS_BUSY.
 *
 *END**************************************************************************/
status_t LIN_UART_DRV_SendFrameData(uint32_t instance, const uint8_t *txBuff, uint8_t txSize)
{
    /* Assert parameters. */
    OS_ASSERT(txBuff != NULL);
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    status_t retVal = STATUS_SUCCESS;

    /* Get base address of the UART instance. */
    uart_type_t *base = s_pstLinUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t *linCurrentState = s_pstLinState[instance];

    /* Check whether current mode is sleep mode */
    bool checkSleepMode = (LIN_NODE_STATE_SLEEP_MODE == linCurrentState->currentNodeState);

    /* Check if txSize > 8 or equal to 0 or node's current state
     * is in SLEEP mode then return STATUS_ERROR */
    if ((8U < txSize) || (0U == txSize) || checkSleepMode) {
        retVal = STATUS_ERROR;
    } else {
        /* Check if the LIN Bus is busy */
        if (linCurrentState->isBusBusy) {
            retVal = STATUS_BUSY;
        } else {
            /* Make the checksum byte. */
            linCurrentState->checkSum =
                LIN_UART_DRV_MakeChecksumByte(instance, txBuff, txSize, linCurrentState->currentPid);

            /* Update the LIN state structure. */
            linCurrentState->txBuff = txBuff;
            /* Add a place for checksum byte */
            linCurrentState->txSize           = (uint8_t)(txSize + 1U);
            linCurrentState->cntByte          = 0U;
            linCurrentState->currentNodeState = LIN_NODE_STATE_SEND_DATA;
            linCurrentState->currentEventId   = LIN_NO_EVENT;
            linCurrentState->isBusBusy        = true;
            linCurrentState->isTxBusy         = true;

            /* Set Break char detect length as 10 bits minimum */
            UART_SetBreakCharDetectLength(base, UART_BREAK_CHAR_10_BIT_MINIMUM);

            /* Start sending data */
            UART_Putchar(base, *linCurrentState->txBuff);
        }
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_GetTransmitStatus
 * Description   : This function returns whether the previous UART transmit has
 * finished. When performing non-blocking transmit, the user can call this
 * function to ascertain the state of the current transmission:
 * in progress (or busy that is STATUS_BUSY) or timeout (if timeout has occurred that is
 * STATUS_TIMEOUT) or complete (success that is STATUS_SUCCESS).
 * In addition, if the transmission is still in progress, the user can obtain the number
 * of bytes that still needed to transmit.
 *
 *END**************************************************************************/
status_t LIN_UART_DRV_GetTransmitStatus(uint32_t instance, uint8_t *bytesRemaining)
{
    /* Assert parameters. */
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    status_t retVal = STATUS_SUCCESS;

    /* Get the current LIN state of this UART instance. */
    const lin_state_t *linCurrentState = s_pstLinState[instance];

    /* Get the number of bytes that is still needed to transmit */
    *bytesRemaining = (uint8_t)(linCurrentState->txSize - linCurrentState->cntByte);

    /* Return status of the on-going transmission */
    if ((LIN_NO_EVENT == linCurrentState->currentEventId) && (0U != *bytesRemaining)) {
        if (false == linCurrentState->timeoutCounterFlag) {
            retVal = STATUS_BUSY;
        } else {
            retVal = STATUS_TIMEOUT;
        }
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_RecvFrmDataBlocking
 * Description   : This function receives data from UART module using blocking
 * method, the function does not return until the receive is complete. The interrupt
 * handler LIN_UART_DRV_IRQHandler will check the checksum byte. If the checksum
 * is correct, it will receive the frame data. If the checksum is incorrect, this
 * function will return STATUS_TIMEOUT and data in rxBuff might be wrong. This function
 * also check if rxSize is in range from 1 to 8. If not, it will return STATUS_ERROR.
 * This function also returns STATUS_ERROR if node's current state is in SLEEP mode.
 * This function checks if the isBusBusy is false, if not it will return
 * STATUS_BUSY.
 *
 *END**************************************************************************/
status_t LIN_UART_DRV_RecvFrmDataBlocking(uint32_t instance, uint8_t *rxBuff, uint8_t rxSize, uint32_t timeoutMSec)
{
    /* Assert parameters. */
    OS_ASSERT(NULL != rxBuff);
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    /* Get the current LIN state of this UART instance. */
    lin_state_t *linCurrentState = s_pstLinState[instance];
    status_t retVal              = STATUS_SUCCESS;

    /* Check whether current mode is sleep mode */
    bool checkSleepMode = (LIN_NODE_STATE_SLEEP_MODE == linCurrentState->currentNodeState);

    /* Check if rxSize > 8 or equal to 0 or node's current state
     * is in SLEEP mode then return STATUS_ERROR */
    if ((8U < rxSize) || (0U == rxSize) || checkSleepMode) {
        retVal = STATUS_ERROR;
    } else {
        /* Check if the LIN Rx is busy */
        if (linCurrentState->isRxBusy) {
            retVal = STATUS_BUSY;
        } else {
            /* Update the LIN state structure. */
            linCurrentState->rxBuff = rxBuff;
            /* Add a place for checksum byte */
            linCurrentState->rxSize  = (uint8_t)(rxSize + 1U);
            linCurrentState->cntByte = 0U;

            /* Setup receiving data blocking */
            linCurrentState->currentEventId = LIN_NO_EVENT;
            linCurrentState->isRxBlocking   = true;

            /* Wait until the reception is complete. */
            retVal = LIN_UART_DRV_WaitComplete(instance, timeoutMSec);

            /* In case of receiving a go to sleep request, after callback, node is in SLEEP MODE */
            /* In this case, node is in SLEEP MODE state */
            if (LIN_NODE_STATE_SLEEP_MODE != linCurrentState->currentNodeState) {
                /* Update node's current state to IDLE. */
                linCurrentState->currentNodeState = LIN_NODE_STATE_IDLE;
            }

            /* Clear Rx Blocking flag. */
            linCurrentState->isRxBlocking = false;

            /* Update Bus Busy flag. */
            linCurrentState->isBusBusy = false;
            linCurrentState->isRxBusy  = false;
        }
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_RecvFrmData
 * Description   : This function receives data from UART module using
 * non-blocking method. This function returns immediately after initiating the
 * receive function. The application has to get the receive status to see when
 * the receive is complete. In other words, after calling non-blocking get
 * function, the application must get the receive status to check if receive
 * is completed or not. The interrupt handler LIN_UART_DRV_IRQHandler will check
 * the checksum byte. If the checksum is correct, it will receive the frame data.
 * If the checksum is incorrect, this function will return STATUS_TIMEOUT and data in
 * rxBuff might be wrong. This function also check if rxSize is in range from 1 to 8.
 * If not, it will return STATUS_ERROR. This function also returns STATUS_ERROR if
 * node's current state is in SLEEP mode. This function checks if the
 * isBusBusy is false, if not it will return STATUS_BUSY.
 *
 *END**************************************************************************/
status_t LIN_UART_DRV_RecvFrmData(uint32_t instance, uint8_t *rxBuff, uint8_t rxSize)
{
    /* Assert parameters. */
    OS_ASSERT(NULL != rxBuff);
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    status_t retVal = STATUS_SUCCESS;

    /* Get base address of the UART instance. */
    uart_type_t *base = s_pstLinUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t *linCurrentState = s_pstLinState[instance];

    /* Check whether current mode is sleep mode */
    bool checkSleepMode = (LIN_NODE_STATE_SLEEP_MODE == linCurrentState->currentNodeState);

    /* Check if rxSize > 8 or equal to 0 or node's current state
     * is in SLEEP mode then return STATUS_ERROR */
    if ((8U < rxSize) || (0U == rxSize) || checkSleepMode) {
        retVal = STATUS_ERROR;
    } else {
        /* Check if the LIN Bus is busy */
        if (linCurrentState->isBusBusy) {
            retVal = STATUS_BUSY;
        } else {
            /* Update the LIN state structure. */
            linCurrentState->rxBuff = rxBuff;
            /* Add a place for checksum byte */
            linCurrentState->rxSize  = (uint8_t)(rxSize + 1U);
            linCurrentState->cntByte = 0U;

            /* Start receiving data */
            linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_DATA;
            linCurrentState->currentEventId   = LIN_NO_EVENT;
            linCurrentState->isBusBusy        = true;
            linCurrentState->isRxBusy         = true;
            linCurrentState->isRxBlocking     = false;

            /* Set Break char detect length as 10 bits minimum */
            UART_SetBreakCharDetectLength(base, UART_BREAK_CHAR_10_BIT_MINIMUM);
        }
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_AbortTransferData
 * Description   : Aborts an on-going non-blocking transmission/reception.
 * While performing a non-blocking transferring data, users can call this
 * function to terminate immediately the transferring.
 *
 *END**************************************************************************/
status_t LIN_UART_DRV_AbortTransferData(uint32_t instance)
{
    /* Assert parameters. */
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    status_t retVal = STATUS_SUCCESS;

    /* Get the current LIN state of this UART instance. */
    lin_state_t *linCurrentState = s_pstLinState[instance];

    /* Change node's current state to IDLE */
    (void)LIN_UART_DRV_GotoIdleState(instance);

    /* Clear LIN Tx and Rx Busy flag */
    linCurrentState->isTxBusy = false;
    linCurrentState->isRxBusy = false;

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_GetReceiveStatus
 * Description   : This function returns whether the previous UART reception is
 * complete. When performing a non-blocking receive, the user can call this
 * function to ascertain the state of the current receive progress: in progress
 * or complete. In addition, if the reception is still in progress, the user can
 * obtain the number of words that is still needed to receive.
 *
 *END**************************************************************************/
status_t LIN_UART_DRV_GetReceiveStatus(uint32_t instance, uint8_t *bytesRemaining)
{
    /* Assert parameters. */
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    status_t retVal = STATUS_SUCCESS;

    /* Get the current LIN state of this UART instance. */
    const lin_state_t *linCurrentState = s_pstLinState[instance];

    /* Get the number of bytes that is still needed to receive */
    *bytesRemaining = (uint8_t)(linCurrentState->rxSize - linCurrentState->cntByte);

    /* Return status of the on-going reception */
    if ((LIN_NO_EVENT == linCurrentState->currentEventId) && (0U != *bytesRemaining)) {
        if (false == linCurrentState->timeoutCounterFlag) {
            retVal = STATUS_BUSY;
        } else {
            retVal = STATUS_TIMEOUT;
        }
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_GoToSleepMode
 * Description   : This function puts current LIN node to sleep mode.
 * This function changes current node state to LIN_NODE_STATE_SLEEP_MODE.
 *
 *END**************************************************************************/
status_t LIN_UART_DRV_GoToSleepMode(uint32_t instance)
{
    /* Assert parameters. */
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    /* Get base address of the UART instance. */
    uart_type_t *base = s_pstLinUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t *linCurrentState = s_pstLinState[instance];

    /* Update node's current state to SLEEP_MODE. */
    linCurrentState->currentNodeState = LIN_NODE_STATE_SLEEP_MODE;

    /* Clear txBusy, rxBusy, busBusy flags */
    linCurrentState->isTxBusy  = false;
    linCurrentState->isRxBusy  = false;
    linCurrentState->isBusBusy = false;

    /* Clear UART_RX Pin Active Edge Interrupt Flag. */
    (void)UART_ClearStatusFlag(base, UART_RX_ACTIVE_EDGE_DETECT);

    /* Set Receive data not inverted */
    UART_SetRxDataPolarity(base, false);

    /* Disable RX complete interrupt */
    UART_SetIntMode(base, UART_INT_RX_DATA_REG_FULL, false);

    /* Enable RX Input Active Edge interrupt */
    UART_SetIntMode(base, UART_INT_RX_ACTIVE_EDGE, true);

    /* Disable frame error interrupt */
    UART_SetIntMode(base, UART_INT_FRAME_ERR_FLAG, false);

    /* Disable LIN break detect interrupt */
    UART_SetIntMode(base, UART_INT_LIN_BREAK_DETECT, false);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_SendWakeupSignal
 * Description   : This function sends a wakeup signal through the UART interface.
 *
 *END**************************************************************************/
status_t LIN_UART_DRV_SendWakeupSignal(uint32_t instance)
{
    /* OS_ASSERT parameters. */
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    /* Get base address of the UART instance. */
    uart_type_t *base = s_pstLinUartBase[instance];
    /* Get the current LIN state of this UART instance. */
    const lin_state_t *linCurrentState = s_pstLinState[instance];
    status_t retVal                    = STATUS_SUCCESS;

    /* Check if bus is not busy */
    if (false == linCurrentState->isBusBusy) {
        /* Send a wakeup signal */
        UART_Putchar(base, s_wakeupSignal[instance]);
    } else {
        retVal = STATUS_BUSY;
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_GetCurrentNodeState
 * Description   : This function gets the current LIN node state.
 *
 *END**************************************************************************/
lin_node_state_t LIN_UART_DRV_GetCurrentNodeState(uint32_t instance)
{
    /* Assert parameters. */
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    lin_node_state_t retVal = LIN_NODE_STATE_UNINIT;
    /* Get the current LIN state of this UART instance. */
    const lin_state_t *linCurrentState = s_pstLinState[instance];

    if (NULL != linCurrentState) {
        retVal = linCurrentState->currentNodeState;
    }

    /* Return LIN node's current state */
    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_TimeoutService
 * Description   : This is callback function for Timer Interrupt Handler.
 * Users shall initialize a timer (for example SUPERTMR) in Output compare mode
 * with period of about 500 micro seconds. In timer IRQ handler, call this function.
 *
 *END**************************************************************************/
void LIN_UART_DRV_TimeoutService(uint32_t instance)
{
    /* Assert parameters. */
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    /* Get the current LIN state of this UART instance. */
    lin_state_t *linCurrentState = s_pstLinState[instance];

    /* Get LIN node's current state */
    lin_node_state_t state = linCurrentState->currentNodeState;

    switch (state) {
    /* If the node is SENDING DATA */
    case LIN_NODE_STATE_SEND_DATA:
        /* Check if timeout Counter is 0 */
        if (0U == linCurrentState->timeoutCounter) {
            /* Set timeout Counter flag */
            linCurrentState->timeoutCounterFlag = true;

            if (false == linCurrentState->isTxBlocking) {
                /* Callback to handle timeout Counter flag */
                if (NULL != linCurrentState->Callback) {
                    linCurrentState->Callback(instance, linCurrentState);
                }

                /* Clear Tx busy flag */
                linCurrentState->isTxBusy = false;

                /* Change the node's current state to IDLE */
                (void)LIN_UART_DRV_GotoIdleState(instance);
            }
        } else /* If timeout Counter is not 0, then decrease timeout Counter by one */
        {
            linCurrentState->timeoutCounter--;
        }

        break;
    /* If the node is RECEIVING DATA */
    case LIN_NODE_STATE_RECV_DATA:
        /* Check if timeout Counter is 0 */
        if (0U == linCurrentState->timeoutCounter) {
            /* Set timeout Counter flag */
            linCurrentState->timeoutCounterFlag = true;

            /* Check if the reception is non-blocking */
            if (false == linCurrentState->isRxBlocking) {
                /* Callback to handle timeout Counter flag */
                if (NULL != linCurrentState->Callback) {
                    linCurrentState->Callback(instance, linCurrentState);
                }

                /* Clear Rx busy flag */
                linCurrentState->isRxBusy = false;

                /* Change the node's current state to IDLE */
                (void)LIN_UART_DRV_GotoIdleState(instance);
            }
        }
        /* If timeout Counter is not 0, then decrease timeout Counter by one */
        else {
            linCurrentState->timeoutCounter--;
        }

        break;
    default:
        /* The node state is not SENDING nor RECEIVING data */
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_SetTimeoutCounter
 * Description   : This function sets value for timeout counter that is used in
 * LIN_UART_DRV_TimeoutService
 *
 *END**************************************************************************/
void LIN_UART_DRV_SetTimeoutCounter(uint32_t instance, uint32_t timeoutValue)
{
    /* Assert parameters. */
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    /* Get the current LIN state of this UART instance. */
    lin_state_t *linCurrentState = s_pstLinState[instance];

    /* Clear Timeout Counter Flag */
    linCurrentState->timeoutCounterFlag = false;

    /* Set new value for Timeout Counter */
    linCurrentState->timeoutCounter = timeoutValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_MasterSendHeader
 * Description   : This function sends frame header out through the UART module
 * using a non-blocking method. Non-blocking  means that the function returns
 * immediately. This function sends LIN Break field, sync field then the ID with
 * correct parity. This function checks if the interface is Master, if not, it will
 * return STATUS_ERROR.This function checks if id is in range from 0 to 0x3F, if not
 * it will return STATUS_ERROR. This function also check node's current state is in
 * SLEEP mode then the function will return STATUS_ERROR. And check if isBusBusy is
 * currently true then the function will return STATUS_BUSY.
 *
 *END**************************************************************************/
status_t LIN_UART_DRV_MasterSendHeader(uint32_t instance, uint8_t id)
{
    /* Assert parameters. */
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    status_t retVal = STATUS_SUCCESS;

    /* Get the current LIN user config structure of this UART instance. */
    const lin_user_config_t *linUserConfig = s_pstLinUserConfig[instance];

    /* Get base address of the UART instance. */
    uart_type_t *base = s_pstLinUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t *linCurrentState = s_pstLinState[instance];

    /* Check whether current mode is sleep mode */
    bool checkSleepMode = (LIN_NODE_STATE_SLEEP_MODE == linCurrentState->currentNodeState);

    /* Check if the current node is slave or id is invalid or node's current
     * state is in SLEEP state */
    if (((bool)LIN_SLAVE) == linUserConfig->nodeFunction || (0x3FU < id) || checkSleepMode) {
        retVal = STATUS_ERROR;
    } else {
        /* Check if the LIN bus is busy */
        if (linCurrentState->isBusBusy) {
            retVal = STATUS_BUSY;
        } else {
            linCurrentState->currentId = id;

            /* Make parity for the current ID */
            linCurrentState->currentPid = LIN_DRV_ProcessParity(id, MAKE_PARITY);

            /* Set LIN current state to sending Break field */
            linCurrentState->currentNodeState = LIN_NODE_STATE_SEND_BREAK_FIELD;
            linCurrentState->currentEventId   = LIN_NO_EVENT;
            linCurrentState->isBusBusy        = true;

            /* Set Break char detect length as 13 bits minimum */
            UART_SetBreakCharDetectLength(base, UART_BREAK_CHAR_13_BIT_MINIMUM);
            UART_SetIntMode(base, UART_INT_LIN_BREAK_DETECT, true);

            /* Send break char by using queue mode */
            UART_QueueBreakField(base);
        }
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_EnableIRQ
 * Description   : This function enables UART hardware interrupts.
 *
 *END**************************************************************************/
status_t LIN_UART_DRV_EnableIRQ(uint32_t instance)
{
    /* Assert parameters. */
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    status_t retVal = STATUS_SUCCESS;

    /* Get base address of the UART instance. */
    uart_type_t *base = s_pstLinUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    const lin_state_t *linCurrentState = s_pstLinState[instance];

    if (LIN_NODE_STATE_SLEEP_MODE == linCurrentState->currentNodeState) {
        /* Enable RX Input Active Edge interrupt */
        UART_SetIntMode(base, UART_INT_RX_ACTIVE_EDGE, true);
    } else {
        /* Enable RX complete interrupt */
        UART_SetIntMode(base, UART_INT_RX_DATA_REG_FULL, true);

        /* Enable frame error interrupt */
        UART_SetIntMode(base, UART_INT_FRAME_ERR_FLAG, true);

        /* Enable LIN break detect interrupt */
        UART_SetIntMode(base, UART_INT_LIN_BREAK_DETECT, true);
    }

    /* Enable UART interrupts. */
    OS_EnableIrq(s_euLinUartRxIrqId[instance]);
    OS_EnableIrq(s_euLinUartTxIrqId[instance]);
#ifdef UART_ERR_IRQS
    OS_EnableIrq(s_linUartErrIrqId[instance]);
#endif

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_DisableIRQ
 * Description   : This function disables UART hardware interrupts.
 *
 *END**************************************************************************/
status_t LIN_UART_DRV_DisableIRQ(uint32_t instance)
{
    /* Assert parameters. */
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    status_t retVal = STATUS_SUCCESS;

    /* Get base address of the UART instance. */
    uart_type_t *base = s_pstLinUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    const lin_state_t *linCurrentState = s_pstLinState[instance];

    if (LIN_NODE_STATE_SLEEP_MODE == linCurrentState->currentNodeState) {
        /* Disable RX Input Active Edge interrupt */
        UART_SetIntMode(base, UART_INT_RX_ACTIVE_EDGE, false);
    } else {
        /* Disable RX complete interrupt */
        UART_SetIntMode(base, UART_INT_RX_DATA_REG_FULL, false);

        /* Disable frame error interrupt */
        UART_SetIntMode(base, UART_INT_FRAME_ERR_FLAG, false);

        /* Disable frame error interrupt */
        UART_SetIntMode(base, UART_INT_LIN_BREAK_DETECT, false);
    }

    /* Disable UART interrupts. */
    OS_DisableIrq(s_euLinUartRxIrqId[instance]);
    OS_DisableIrq(s_euLinUartTxIrqId[instance]);
#ifdef UART_ERR_IRQS
    OS_DisableIrq(s_linUartErrIrqId[instance]);
#endif

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_GotoIdleState
 * Description   : This function puts current node to Idle state.
 *
 *END**************************************************************************/
status_t LIN_UART_DRV_GotoIdleState(uint32_t instance)
{
    /* Get base address of the UART instance. */
    uart_type_t *base = s_pstLinUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t *linCurrentState = s_pstLinState[instance];

    linCurrentState->currentEventId = LIN_NO_EVENT;

    /* Set Break char detect length as 13 bits minimum */
    UART_SetBreakCharDetectLength(base, UART_BREAK_CHAR_13_BIT_MINIMUM);

    /* Set Receive data not inverted */
    UART_SetRxDataPolarity(base, false);

    /* Enable RX complete interrupt */
    UART_SetIntMode(base, UART_INT_RX_DATA_REG_FULL, true);

    /* Disable RXEDG interrupt */
    UART_SetIntMode(base, UART_INT_RX_ACTIVE_EDGE, false);

    /* Enable frame error interrupt */
    UART_SetIntMode(base, UART_INT_FRAME_ERR_FLAG, true);

    /* Enable LIN break detect interrupt */
    UART_SetIntMode(base, UART_INT_LIN_BREAK_DETECT, true);

    /* Change node's current state to IDLE */
    linCurrentState->currentNodeState = LIN_NODE_STATE_IDLE;

    /* Clear Bus busy Flag */
    linCurrentState->isBusBusy = false;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_WaitComplete
 * Description   : This function waits until transmission/reception is complete and
 * returns status of the transaction.
 *
 *END**************************************************************************/
static status_t LIN_UART_DRV_WaitComplete(uint32_t instance, uint32_t timeoutMSec)
{
    /* Get the current LIN state of this UART instance. */
    lin_state_t *linCurrentState = s_pstLinState[instance];
    status_t retVal              = STATUS_SUCCESS;

    if (true == linCurrentState->isTxBlocking) {
        if (OS_ErrorTimeout == OS_SemaphoreAcquire(linCurrentState->txCompleted, timeoutMSec)) {
            retVal = STATUS_TIMEOUT;
        }
    }

    if (true == linCurrentState->isRxBlocking) {
        if (OS_ErrorTimeout == OS_SemaphoreAcquire(linCurrentState->rxCompleted, timeoutMSec)) {
            retVal = STATUS_TIMEOUT;
        }
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_IRQHandler
 * Description   : Interrupt handler for UART.
 * This handler uses the buffers stored in the lin_state_t struct to transfer
 * data. This is not a public API as it is called by IRQ whenever an interrupt
 * occurs.
 *
 *END**************************************************************************/
void LIN_UART_DRV_IRQHandler(uint32_t instance)
{
    /* Assert parameters. */
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    uint8_t tmpByte = 0U;

    /* Get base address of the UART instance. */
    uart_type_t *base = s_pstLinUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t *linCurrentState = s_pstLinState[instance];

    /* Check RX Input Active Edge interrupt enable */
    bool activeEdgeIntState = UART_GetIntMode(base, UART_INT_RX_ACTIVE_EDGE);

    /* If LIN break character has been detected. */
    if (UART_GetStatusFlag(base, UART_LIN_BREAK_DETECT)) {
        LIN_UART_DRV_ProcessBreakDetect(instance);
    } else {
        /* If UART_RX Pin Active Edge has been detected. */
        if (UART_GetStatusFlag(base, UART_RX_ACTIVE_EDGE_DETECT) && activeEdgeIntState) {
            /* Clear UART_RX Pin Active Edge Interrupt Flag. */
            (void)UART_ClearStatusFlag(base, UART_RX_ACTIVE_EDGE_DETECT);

            /* Check if a wakeup signal has been received */
            LIN_UART_DRV_CheckWakeupSignal(instance);
        } else {
            /* If Framing Error has been detected */
            if (UART_GetStatusFlag(base, UART_FRAME_ERR)) {
                /* Clear Framing Error Interrupt Flag */
                (void)UART_ClearStatusFlag(base, UART_FRAME_ERR);

                /* Read dummy to clear UART_RX_DATA_REG_FULL flag */
                UART_Getchar(base, &tmpByte);

                /* Set current event id to LIN_FRAME_ERROR */
                linCurrentState->currentEventId = LIN_FRAME_ERROR;

                /* Check if LIN current node state is LIN_NODE_STATE_SEND_DATA */
                if (LIN_NODE_STATE_SEND_DATA == linCurrentState->currentNodeState) {
                    /* Callback function to handle Framing Error Event */
                    if (NULL != linCurrentState->Callback) {
                        linCurrentState->Callback(instance, linCurrentState);
                    }
                } else {
                    /* Check if LIN current node state is LIN_NODE_STATE_RECV_DATA */
                    if (LIN_NODE_STATE_RECV_DATA == linCurrentState->currentNodeState) {
                        /* Callback function to handle Framing Error Event */
                        if (NULL != linCurrentState->Callback) {
                            linCurrentState->Callback(instance, linCurrentState);
                        }
                    }
                }

                /* Change node's state to IDLE */
                (void)LIN_UART_DRV_GotoIdleState(instance);
            } else {
                if (UART_GetStatusFlag(base, UART_RX_DATA_REG_FULL)) {
                    /* Get data from Data Register & Clear UART_RX_DATA_REG_FULL flag */
                    UART_Getchar(base, &tmpByte);

                    /* Process data in Data Register while receive, send data */
                    LIN_UART_DRV_ProcessFrame(instance, tmpByte);
                }
            } /* End else: if (UART_GetStatusFlag(base, UART_FRAME_ERR) == 0) */
        }     /* End else: if (UART_GetStatusFlag(base, UART_RX_ACTIVE_EDGE_DETECT) == 0) */
    }         /* End else: if (UART_GetStatusFlag(base, UART_LIN_BREAK_DETECT) == 0) */

    /* Get status RX overrun flag */
    if (UART_GetStatusFlag(base, UART_RX_OVERRUN)) {
        /* Clear overrun flag */
        (void)UART_ClearStatusFlag(base, UART_RX_OVERRUN);

        /* Set current event id to LIN_RX_OVERRUN */
        linCurrentState->currentEventId = LIN_RX_OVERRUN;

        /* Callback function to handle RX Overrun Event */
        if (NULL != linCurrentState->Callback) {
            linCurrentState->Callback(instance, linCurrentState);
        }
    }
} /* End void LIN_UART_DRV_IRQHandler(uint32_t instance) */

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_ProcessBreakDetect
 * Description   : This function process break detect for LIN communication.
 *
 *END**************************************************************************/
static void LIN_UART_DRV_ProcessBreakDetect(uint32_t instance)
{
    /* Get the current LIN user configure structure of this UART instance. */
    const lin_user_config_t *linUserConfig = s_pstLinUserConfig[instance];

    /* Get base address of the UART instance. */
    uart_type_t *base = s_pstLinUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t *linCurrentState = s_pstLinState[instance];

    /* Clear LIN Break Detect Interrupt Flag */
    (void)UART_ClearStatusFlag(base, UART_LIN_BREAK_DETECT);

    /* Set Break char detect length as 10 bits minimum */
    UART_SetBreakCharDetectLength(base, UART_BREAK_CHAR_10_BIT_MINIMUM);
    /* Disable LIN Break Detect Interrupt */
    UART_SetIntMode(base, UART_INT_LIN_BREAK_DETECT, false);

    /* Check if the current node is MASTER */
    if ((bool)LIN_MASTER == linUserConfig->nodeFunction) {
        /* Check if LIN current node state is LIN_NODE_STATE_SEND_BREAK_FIELD */
        if (LIN_NODE_STATE_SEND_BREAK_FIELD == linCurrentState->currentNodeState) {
            /* Set flag LIN bus busy */
            linCurrentState->isBusBusy = true;
            /* Change the node's current state to SENDING PID to send PID after send SYNC */
            linCurrentState->currentNodeState = LIN_NODE_STATE_SEND_PID;
            /* Send Sync Field 0x55 */
            UART_Putchar(base, 0x55);
        }
    }
    /* If the current node is SLAVE */
    else {
        /* Set flag LIN bus busy */
        linCurrentState->isBusBusy = true;
        /* Change the node's current state to RECEIVED BREAK FIELD */
        linCurrentState->currentEventId = LIN_RECV_BREAK_FIELD_OK;
        /* Callback function */
        if (NULL != linCurrentState->Callback) {
            linCurrentState->Callback(instance, linCurrentState);
        }
        /* Change the node's current state to RECEIVING SYNC FIELD */
        linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_SYNC;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_CheckWakeupSignal
 * Description   : This function check if a dominant signal received is a wakeup
 *                signal.
 *
 *END**************************************************************************/
static void LIN_UART_DRV_CheckWakeupSignal(uint32_t instance)
{
    uint32_t wakeupSignalLength = 0U;

    /* Get the current LIN user config structure of this UART instance. */
    const lin_user_config_t *linUserConfig = s_pstLinUserConfig[instance];

    /* Get base address of the UART instance. */
    uart_type_t *base = s_pstLinUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t *linCurrentState = s_pstLinState[instance];

    /* if UART_GetRxDataPolarity is 0: Receive Data is not inverted */
    if (false == UART_GetRxDataPolarity(base)) {
        /* Start measure time */
        wakeupSignalLength = linUserConfig->timerGetTimeIntervalCallback(instance);

        /* Set Receive Data Inverted */
        UART_SetRxDataPolarity(base, true);
    } else {
        /* Set Receive Data is Not Inverted */
        UART_SetRxDataPolarity(base, false);

        /* Calculate time interval between the falling and rising edge */
        wakeupSignalLength = linUserConfig->timerGetTimeIntervalCallback(instance);

        /* If length of the dominant signal is longer than 150us, it is a wakeup signal */
        if (wakeupSignalLength >= 150000U) {
            linCurrentState->currentEventId = LIN_WAKEUP_SIGNAL;

            /* Callback to handle event: Received a wakeup signal */
            if (NULL != linCurrentState->Callback) {
                linCurrentState->Callback(instance, linCurrentState);
            }

            /* Change node's state to IDLE */
            (void)LIN_UART_DRV_GotoIdleState(instance);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_ProcessFrame
 * Description   : Part of Interrupt handler for receiving and sending data.
 * Receive Header, Data and Send Data.
 *
 *END**************************************************************************/
static void LIN_UART_DRV_ProcessFrame(uint32_t instance, uint8_t tmpByte)
{
    /* Get the current LIN state of this UART instance. */
    const lin_state_t *linCurrentState = s_pstLinState[instance];

    /* Check node's current state */
    switch (linCurrentState->currentNodeState) {
    /* if current state is RECEIVE SYNC FIELD */
    case LIN_NODE_STATE_RECV_SYNC:

    /* if current state is MASTER SENDING PID */
    case LIN_NODE_STATE_SEND_PID:

    /* if current state is RECEIVE PID */
    case LIN_NODE_STATE_RECV_PID:
        LIN_UART_DRV_ProcessFrameHeader(instance, tmpByte);
        break;
    /* if current state is RECEIVE DATA */
    case LIN_NODE_STATE_RECV_DATA:
        LIN_UART_DRV_ProcessReceiveFrameData(instance, tmpByte);
        break;
    /* if current state is SENDING DATA */
    case LIN_NODE_STATE_SEND_DATA:
        LIN_UART_DRV_ProcessSendFrameData(instance, tmpByte);
        break;

    default:
        /* Other node state */
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_ProcessFrameHeader
 * Description   : Part of Interrupt handler for receiving and sending data.
 * Receive Sync byte, PID and Send PID.
 *
 *END**************************************************************************/
static void LIN_UART_DRV_ProcessFrameHeader(uint32_t instance, uint8_t tmpByte)
{
    /* Get base address of the UART instance. */
    uart_type_t *base = s_pstLinUartBase[instance];

    /* Get the current LIN user config structure of this UART instance. */
    const lin_user_config_t *linUserConfig = s_pstLinUserConfig[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t *linCurrentState = s_pstLinState[instance];

    /* Check node's current state */
    switch (linCurrentState->currentNodeState) {
    /* If current state is RECEIVE SYNC FIELD */
    case LIN_NODE_STATE_RECV_SYNC:
        if (0x55U == tmpByte) {
            /* Set current event ID to Sync byte is correct */
            linCurrentState->currentEventId = LIN_SYNC_OK;
            /* Change node's current state to RECEIVE PID */
            linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_PID;
        } else {
            /* Set current event ID to Sync byte is incorrect */
            linCurrentState->currentEventId = LIN_SYNC_ERROR;
            /* Callback function to handle event RECEIVED SYNC FIELD ERROR */
            if (NULL != linCurrentState->Callback) {
                linCurrentState->Callback(instance, linCurrentState);
            }

            /* Change node's current state to IDLE */
            (void)LIN_UART_DRV_GotoIdleState(instance);
        }

        break;
    /* If current state is MASTER SENDING PID */
    case LIN_NODE_STATE_SEND_PID:
        /* Check if master node sent SYNC byte correctly before send PID */
        if (0x55U == tmpByte) {
            /* Change node's current state to RECEIVING PID */
            linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_PID;
            /* Send the current PID byte */
            UART_Putchar(base, linCurrentState->currentPid);
        }
        /* In case of errors during header transmission, it is up to the implementer
         * how to handle these errors (stop/continue transmission) and to decide if the
         * corresponding response is valid or not.
         * By default, LIN Driver set isBusBusy to false, and change node's state to IDLE.
         */
        else {
            /* Set current event ID to Sync byte is incorrect */
            linCurrentState->currentEventId = LIN_SYNC_ERROR;
            /* Clear Bus bus flag */
            linCurrentState->isBusBusy = false;
            /* Change node's current state to IDLE */
            linCurrentState->currentNodeState = LIN_NODE_STATE_IDLE;

            /* Callback function to handle event SENT SYNC BYTE ERROR */
            if (NULL != linCurrentState->Callback) {
                linCurrentState->Callback(instance, linCurrentState);
            }
        }

        break;
    /* If current state is RECEIVE PID */
    case LIN_NODE_STATE_RECV_PID:
        /* If the node is MASTER */
        if ((bool)LIN_MASTER == linUserConfig->nodeFunction) {
            /* Check if master node sent PID correctly */
            if (tmpByte == linCurrentState->currentPid) {
                /* Set current event ID to PID correct */
                linCurrentState->currentEventId = LIN_PID_OK;

                /* Check receiving data is blocking */
                if (true == linCurrentState->isRxBlocking) {
                    /* Starting receive data blocking */
                    linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_DATA;
                    linCurrentState->isBusBusy        = true;
                    linCurrentState->isRxBusy         = true;

                    /* Set Break char detect length as 10 bits minimum */
                    UART_SetBreakCharDetectLength(base, UART_BREAK_CHAR_10_BIT_MINIMUM);
                } else {
                    /* Clear Bus bus flag */
                    linCurrentState->isBusBusy = false;

                    /* Callback function to handle correct PID */
                    if (NULL != linCurrentState->Callback) {
                        linCurrentState->Callback(instance, linCurrentState);
                    }
                }
            }
            /* In case of errors during header transmission, it is up to the implementer
             * how to handle these errors (stop/continue transmission) and to decide if the
             * corresponding response is valid or not.
             * By default, LIN Driver set isBusBusy to false, and change node's state to IDLE.
             */
            else {
                /* Set current event ID to PID incorrect */
                linCurrentState->currentEventId = LIN_PID_ERROR;
                /* Clear bus busy flag */
                linCurrentState->isBusBusy = false;
                /* Change node's current state to IDLE */
                linCurrentState->currentNodeState = LIN_NODE_STATE_IDLE;
                /* Callback function to handle event MASTER SENT PID ERROR */
                if (NULL != linCurrentState->Callback) {
                    linCurrentState->Callback(instance, linCurrentState);
                }
            }
        }
        /* If the node is SLAVE */
        else {
            /* Check the received PID */
            linCurrentState->currentId  = LIN_DRV_ProcessParity(tmpByte, CHECK_PARITY);
            linCurrentState->currentPid = tmpByte;
            if (0xFFU != linCurrentState->currentId) {
                /* Set current event ID to PID correct */
                linCurrentState->currentEventId = LIN_PID_OK;

                /* Check receiving data is blocking */
                if (true == linCurrentState->isRxBlocking) {
                    /* Starting receive data blocking */
                    linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_DATA;
                    linCurrentState->isBusBusy        = true;
                    linCurrentState->isRxBusy         = true;

                    /* Set Break char detect length as 10 bits minimum */
                    UART_SetBreakCharDetectLength(base, UART_BREAK_CHAR_10_BIT_MINIMUM);
                } else {
                    /* Clear Bus bus flag */
                    linCurrentState->isBusBusy = false;

                    /* Callback function to handle event PID correct */
                    if (NULL != linCurrentState->Callback) {
                        linCurrentState->Callback(instance, linCurrentState);
                    }
                }
            } else {
                /* Set current event ID to PID ERROR */
                linCurrentState->currentEventId = LIN_PID_ERROR;

                /* Callback function to handle event PID incorrect */
                if (NULL != linCurrentState->Callback) {
                    linCurrentState->Callback(instance, linCurrentState);
                }

                /* Change node's current state to IDLE */
                (void)LIN_UART_DRV_GotoIdleState(instance);
            }
        }

        break;
    default:
        /* Other node state */
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_ProcessReceiveFrameData
 * Description   : Part of Interrupt handler for receiving.
 *
 *END**************************************************************************/
static void LIN_UART_DRV_ProcessReceiveFrameData(uint32_t instance, uint8_t tmpByte)
{
    /* Get the current LIN state of this UART instance. */
    lin_state_t *linCurrentState = s_pstLinState[instance];

    if (linCurrentState->rxSize > (linCurrentState->cntByte + 1U)) {
        *(linCurrentState->rxBuff) = tmpByte;
        linCurrentState->rxBuff++;
    } else {
        if (1U == (linCurrentState->rxSize - linCurrentState->cntByte)) {
            linCurrentState->checkSum = tmpByte;
        }
    }

    linCurrentState->cntByte++;
    if (linCurrentState->cntByte == linCurrentState->rxSize) {
        /* Restore rxBuffer pointer */
        linCurrentState->rxBuff -= linCurrentState->rxSize - 1U;
        if (LIN_UART_DRV_MakeChecksumByte(instance, linCurrentState->rxBuff, linCurrentState->rxSize - 1U,
                                          linCurrentState->currentPid) == linCurrentState->checkSum) {
            linCurrentState->currentEventId   = LIN_RX_COMPLETED;
            linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_DATA_COMPLETED;

            /* callback function to handle RX COMPLETED */
            if (NULL != linCurrentState->Callback) {
                linCurrentState->Callback(instance, linCurrentState);
            }

            /* Check if the reception is non-blocking */
            if (false == linCurrentState->isRxBlocking) {
                /* Clear Bus busy flag */
                linCurrentState->isBusBusy = false;

                /* Clear Rx busy flag */
                linCurrentState->isRxBusy = false;

                /* In case of receiving a go to sleep request, after callback, node is in SLEEP MODE */
                /* In this case, node is in SLEEP MODE state */
                if (LIN_NODE_STATE_SLEEP_MODE != linCurrentState->currentNodeState) {
                    (void)LIN_UART_DRV_GotoIdleState(instance);
                }
            } else {
                /* Post Semaphore to signal Rx Completed*/
                (void)OS_SemaphoreRelease(linCurrentState->rxCompleted);
            }
        } else {
            linCurrentState->currentEventId = LIN_CHECKSUM_ERROR;
            /* callback function to handle checksum error */
            if (NULL != linCurrentState->Callback) {
                linCurrentState->Callback(instance, linCurrentState);
            }

            /* Clear Rx busy flag */
            linCurrentState->isRxBusy = false;

            /* Change node's current state to IDLE */
            (void)LIN_UART_DRV_GotoIdleState(instance);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_ProcessSendFrameData
 * Description   : Part of Interrupt handler for sending data.
 *
 *END**************************************************************************/
static void LIN_UART_DRV_ProcessSendFrameData(uint32_t instance, uint8_t tmpByte)
{
    bool sendFlag = true;
    uint8_t tmpSize;
    bool tmpCheckSumAndSize;
    bool tmpBuffAndSize;

    /* Get base address of the UART instance. */
    uart_type_t *base = s_pstLinUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t *linCurrentState = s_pstLinState[instance];

    /* Check if Tx data register empty flag is false */
    if (false == UART_GetStatusFlag(base, UART_TX_DATA_REG_EMPTY)) {
        linCurrentState->currentEventId = LIN_READBACK_ERROR;
        /* callback function to handle Readback error */
        if (NULL != linCurrentState->Callback) {
            linCurrentState->Callback(instance, linCurrentState);
        }

        /* Check if the transmission is non-blocking */
        if (false == linCurrentState->isTxBlocking) {
            /* Clear Tx busy flag */
            linCurrentState->isTxBusy = false;

            /* Change node's current state to IDLE */
            (void)LIN_UART_DRV_GotoIdleState(instance);
        }

        sendFlag = false;
    } else {
        tmpSize            = (uint8_t)(linCurrentState->txSize - linCurrentState->cntByte);
        tmpCheckSumAndSize = (tmpSize == 1U) && (linCurrentState->checkSum != tmpByte);
        tmpBuffAndSize     = (*linCurrentState->txBuff != tmpByte) && (tmpSize != 1U);
        if (tmpBuffAndSize || tmpCheckSumAndSize) {
            linCurrentState->currentEventId = LIN_READBACK_ERROR;

            /* callback function to handle Readback error */
            if (NULL != linCurrentState->Callback) {
                linCurrentState->Callback(instance, linCurrentState);
            }

            /* Check if the transmission is non-blocking */
            if (false == linCurrentState->isTxBlocking) {
                /* Clear Tx busy flag */
                linCurrentState->isTxBusy = false;

                /* Change node's current state to IDLE */
                (void)LIN_UART_DRV_GotoIdleState(instance);
            }

            sendFlag = false;
        } else {
            linCurrentState->txBuff++;
            linCurrentState->cntByte++;
        }
    }

    if (sendFlag) {
        if (linCurrentState->cntByte < linCurrentState->txSize) {
            /* Send checksum byte */
            if (1U == (linCurrentState->txSize - linCurrentState->cntByte)) {
                UART_Putchar(base, linCurrentState->checkSum);
            }
            /* Send data bytes */
            else {
                UART_Putchar(base, *linCurrentState->txBuff);
            }
        } else {
            linCurrentState->currentEventId   = LIN_TX_COMPLETED;
            linCurrentState->currentNodeState = LIN_NODE_STATE_SEND_DATA_COMPLETED;

            UART_SetIntMode(base, UART_INT_RX_DATA_REG_FULL, false);
            /* callback function to handle event TX COMPLETED */
            if (NULL != linCurrentState->Callback) {
                linCurrentState->Callback(instance, linCurrentState);
            }

            /* Check if the transmission is non-blocking */
            if (false == linCurrentState->isTxBlocking) {
                /* Clear Tx busy flag */
                linCurrentState->isTxBusy = false;

                /* In this case, node is in SLEEP MODE state */
                if (LIN_NODE_STATE_SLEEP_MODE != linCurrentState->currentNodeState) {
                    /* Change node's current state to IDLE */
                    (void)LIN_UART_DRV_GotoIdleState(instance);
                }
            } else {
                /* Post Semaphore to signal Tx Completed*/
                (void)OS_SemaphoreRelease(linCurrentState->txCompleted);
            }
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_AutobaudTimerValEval
 * Description   : This function calculate LIN bus baudrate and set slave's baudrate accordingly.
 * Autobaud process runs only once after reset. After setting slave's baudrate to LIN bus baudrate,
 * slave does not evaluate LIN bus baudrate anymore.
 * This is not a public API as it is called from other driver functions.
 *
 *END**************************************************************************/
static void LIN_UART_DRV_AutobaudTimerValEval(uint32_t instance, uint32_t twoBitTimeLength)
{
    /* Assert parameters. */
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    uint32_t MasterBaudRate = 0U;

    /* Get the current LIN user config structure of this UART instance. */
    lin_user_config_t *linUserConfig = s_pstLinUserConfig[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t *linCurrentState = s_pstLinState[instance];

    /* Check whether current node state is receive sync */
    bool checkNodeState = (LIN_NODE_STATE_RECV_SYNC == linCurrentState->currentNodeState);

    /* Evaluate average value against baudrate */
    LIN_UART_DRV_EvalTwoBitTimeLength(instance, twoBitTimeLength);

    if ((linCurrentState->fallingEdgeInterruptCount > 4U) && checkNodeState) {
        if ((twoBitTimeLength >= TWO_BIT_DURATION_MIN_19200) && (twoBitTimeLength <= TWO_BIT_DURATION_MAX_19200)) {
            MasterBaudRate = 19200U;
        } else if ((twoBitTimeLength >= TWO_BIT_DURATION_MIN_14400) &&
                   (twoBitTimeLength <= TWO_BIT_DURATION_MAX_14400)) {
            MasterBaudRate = 14400U;
        } else if ((twoBitTimeLength >= TWO_BIT_DURATION_MIN_9600) && (twoBitTimeLength <= TWO_BIT_DURATION_MAX_9600)) {
            MasterBaudRate = 9600U;
        } else if ((twoBitTimeLength >= TWO_BIT_DURATION_MIN_4800) && (twoBitTimeLength <= TWO_BIT_DURATION_MAX_4800)) {
            MasterBaudRate = 4800U;
        } else if ((twoBitTimeLength >= TWO_BIT_DURATION_MIN_2400) && (twoBitTimeLength <= TWO_BIT_DURATION_MAX_2400)) {
            MasterBaudRate = 2400U;
        } else {
            /* Complete if-elseif-else block to avoid violating MISRA 2012 Rule 15.7 */
        }

        /* Check Master Baudrate against node's current baudrate */
        if ((0U != MasterBaudRate) && (linUserConfig->baudRate != MasterBaudRate)) {
            linUserConfig->baudRate = MasterBaudRate;

            /* Set new baud rate */
            (void)UART_DRV_SetBaudRate(instance, linUserConfig->baudRate);

            /* Assign wakeup signal to satisfy LIN Specifications specifies that
             * wakeup signal shall be in range from 250us to 5 ms.
             */
            if (linUserConfig->baudRate > 10000U) {
                /* Wakeup signal will be range from 400us to 800us depend on baudrate */
                s_wakeupSignal[instance] = 0x80U;
            } else {
                /* Wakeup signal will be range from 400us to 4ms depend on baudrate */
                s_wakeupSignal[instance] = 0xF8U;
            }
        }

        linCurrentState->currentEventId = LIN_BAUDRATE_ADJUSTED;
        /* Disable baudrate evaluation process */
        linCurrentState->baudrateEvalEnable = false;
        /* Callback function to handle this event */
        if (NULL != linCurrentState->Callback) {
            linCurrentState->Callback(instance, linCurrentState);
        }

        /* Update current state and current event */
        linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_PID;
        linCurrentState->currentEventId   = LIN_SYNC_OK;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_EvalTwoBitTimeLength
 * Description   : This function check time of double bit in sync byte for support autobaud.
 * This is not a public API as it is called from other driver functions.
 *
 *END**************************************************************************/
static void LIN_UART_DRV_EvalTwoBitTimeLength(uint32_t instance, uint32_t twoBitTimeLength)
{
    /* Assert parameters. */
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    /* Get the current LIN state of this UART instance. */
    lin_state_t *linCurrentState = s_pstLinState[instance];

    if (linCurrentState->fallingEdgeInterruptCount < 5U) {
        if (linCurrentState->fallingEdgeInterruptCount > 0U) {
            if ((twoBitTimeLength < TWO_BIT_DURATION_MIN_19200) ||
                ((twoBitTimeLength > TWO_BIT_DURATION_MAX_19200) && (twoBitTimeLength < TWO_BIT_DURATION_MIN_14400)) ||
                ((twoBitTimeLength > TWO_BIT_DURATION_MAX_14400) && (twoBitTimeLength < TWO_BIT_DURATION_MIN_9600)) ||
                ((twoBitTimeLength > TWO_BIT_DURATION_MAX_9600) && (twoBitTimeLength < TWO_BIT_DURATION_MIN_4800)) ||
                ((twoBitTimeLength > TWO_BIT_DURATION_MAX_4800) && (twoBitTimeLength < TWO_BIT_DURATION_MIN_2400)) ||
                (twoBitTimeLength > TWO_BIT_DURATION_MAX_2400)) {
                /* Change node's current state to IDLE */
                (void)LIN_UART_DRV_GotoIdleState(instance);
                linCurrentState->fallingEdgeInterruptCount = 0U;
            } else {
                if (linCurrentState->fallingEdgeInterruptCount > 1U) {
                    if ((twoBitTimeLength <
                         ((100U - BIT_RATE_TOLERANCE_UNSYNC) * s_previousTwoBitTimeLength[instance] / 100U)) ||
                        (twoBitTimeLength >
                         ((100U + BIT_RATE_TOLERANCE_UNSYNC) * s_previousTwoBitTimeLength[instance] / 100U))) {
                        /* Change node's current state to IDLE */
                        (void)LIN_UART_DRV_GotoIdleState(instance);
                        linCurrentState->fallingEdgeInterruptCount = 0U;
                    }
                }
            }

            s_previousTwoBitTimeLength[instance] = twoBitTimeLength;
        }
    }

    linCurrentState->fallingEdgeInterruptCount += 1U;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_AutoBaudCapture
 * Description   : This function capture bits time to detect break char, calculate
 * baudrate from sync bits and enable transceiver if autobaud successful.
 * This function should only be used in Slave.
 *
 *END**************************************************************************/
status_t LIN_UART_DRV_AutoBaudCapture(uint32_t instance)
{
    /* Assert parameters. */
    OS_ASSERT(instance < UART_INSTANCE_COUNT);

    /* Get base address of the UART instance. */
    uart_type_t *base = s_pstLinUartBase[instance];
    /* Get the current LIN user config structure of this UART instance. */
    const lin_user_config_t *linUserConfig = s_pstLinUserConfig[instance];
    /* Get the current LIN state of this UART instance. */
    lin_state_t *linCurrentState = s_pstLinState[instance];
    status_t retVal              = STATUS_BUSY;
    uint32_t tmpTime             = 0U;

    if (linCurrentState->baudrateEvalEnable) {
        /* Calculate time between two bit (for service autobaud) */
        tmpTime = linUserConfig->timerGetTimeIntervalCallback(instance);

        /* Get two bits time length */
        s_timeMeasure[instance] += tmpTime;
        s_countMeasure[instance]++;
        if ((s_countMeasure[instance] > 1U)) {
            switch (linCurrentState->currentNodeState) {
            /* If current state is SLEEP MODE */
            case LIN_NODE_STATE_SLEEP_MODE:
                /* If length of the dominant signal is longer than 150us, it is a wakeup signal */
                if (tmpTime >= 150000U) {
                    linCurrentState->currentEventId = LIN_WAKEUP_SIGNAL;

                    /* Callback to handle event: Received a wakeup signal */
                    if (NULL != linCurrentState->Callback) {
                        linCurrentState->Callback(instance, linCurrentState);
                    }

                    /* Change node's state to IDLE */
                    (void)LIN_UART_DRV_GotoIdleState(instance);
                } else {
                    retVal = STATUS_ERROR;
                }
                s_countMeasure[instance] = 0U;

                break;
            /* If current state is IDLE */
            case LIN_NODE_STATE_IDLE:
                /* Check break time minimum */
                if (tmpTime >= AUTOBAUD_BREAK_TIME_MIN) {
                    /* Set Break char detect length as 10 bits minimum */
                    UART_SetBreakCharDetectLength(base, UART_BREAK_CHAR_10_BIT_MINIMUM);

                    /* Disable LIN Break Detect Interrupt */
                    UART_SetIntMode(base, UART_INT_LIN_BREAK_DETECT, false);

                    /* Set flag LIN bus busy */
                    linCurrentState->isBusBusy = true;

                    /* Change the node's current state to RECEIVED BREAK FIELD */
                    linCurrentState->currentEventId = LIN_RECV_BREAK_FIELD_OK;

                    /* Callback function */
                    if (NULL != linCurrentState->Callback) {
                        linCurrentState->Callback(instance, linCurrentState);
                    }

                    /* Change the node's current state to RECEIVING SYNC FIELD */
                    linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_SYNC;

                    /* Start Autobaud Count(initialize number of measurements in sync byte) */
                    linCurrentState->fallingEdgeInterruptCount = 0U;

                    s_countMeasure[instance] = 1U;
                } else {
                    s_countMeasure[instance] = 0U;

                    retVal = STATUS_ERROR;
                }

                break;
            /* If current state is RECEIVE SYNC */
            default:
                /* Calculate baudrate */
                LIN_UART_DRV_AutobaudTimerValEval(instance, s_timeMeasure[instance]);

                /* Reset to measure in next times */
                s_countMeasure[instance] = 0U;
                s_timeMeasure[instance]  = 0U;

                if (LIN_NODE_STATE_IDLE == linCurrentState->currentNodeState) {
                    retVal = STATUS_ERROR;
                }

                break;
            }
        }
    } else {
        if (linCurrentState->fallingEdgeInterruptCount > 4U) {
            /* Enable the UART transmitter and receiver */
            UART_SetTransmitterCmd(base, true);
            UART_SetReceiverCmd(base, true);

            linCurrentState->fallingEdgeInterruptCount = 0U;
        }

        retVal = STATUS_SUCCESS;
    }

    return retVal;
}
