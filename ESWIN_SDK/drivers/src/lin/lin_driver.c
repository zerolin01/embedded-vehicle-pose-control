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
 * @file lin_driver.c
 * @brief LIN driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "lin_driver.h"
#include "lin_uart_driver.h"

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_Init
 * Description   : This function initializes a LIN Hardware Interface for operation.
 * This function will initialize the run-time state structure to keep track of
 * the on-going transfers,  un-gates the clock to LIN Hardware Interface, initialize the
 * module to user defined settings and default settings, configure the IRQ state
 * structure and enable the module-level interrupt to the core, and enable the
 * LIN Hardware Interface transmitter and receiver.
 * The following is an example of how to set up the lin_state_t and the
 * lin_user_config_t parameters and how to call the LIN_DRV_Init function
 * by passing in these parameters:
 *    lin_user_config_t linUserConfig
 *    linUserConfig.baudRate = 9600
 *    linUserConfig.nodeFunction = LIN_SLAVE
 *    linUserConfig.autobaudEnable = true
 *    linUserConfig.timerGetTimeIntervalCallback = (lin_timer_get_time_interval_t)
 * l_ifc_timerGetTimeIntervalCallbackHandler lin_state_t linState LIN_DRV_Init(instance, (lin_user_config_t *)
 * &linUserConfig, (lin_state_t *) &linState)
 *
 *END**************************************************************************/
status_t LIN_DRV_Init(uint32_t instance, lin_user_config_t *linUserConfig, lin_state_t *linCurrentState)
{
    status_t retVal = STATUS_UNSUPPORTED;

    retVal = LIN_UART_DRV_Init(instance, linUserConfig, linCurrentState);

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_Deinit
 * Description   : This function shuts down the LIN Hardware Interface by disabling interrupts and
 *                 transmitter/receiver.
 *
 *END**************************************************************************/
void LIN_DRV_Deinit(uint32_t instance)
{
    LIN_UART_DRV_Deinit(instance);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_GetDefaultConfig
 * Description   : This function initializes a configuration structure received
 * from the application with default values.
 *
 *END**************************************************************************/
void LIN_DRV_GetDefaultConfig(bool isMaster, lin_user_config_t *linUserConfig)
{
    LIN_UART_DRV_GetDefaultConfig(isMaster, linUserConfig);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_InstallCallback
 * Description   : This function installs the callback function that is used for LIN_DRV_IRQHandler.
 * Pass in Null pointer as callback will uninstall.
 *
 *END**************************************************************************/
lin_callback_t LIN_DRV_InstallCallback(uint32_t instance, lin_callback_t function)
{
    lin_callback_t retVal = NULL;

    retVal = LIN_UART_DRV_InstallCallback(instance, function);

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_SendFrameDataBlocking
 * Description   : This function sends data out through the LIN Hardware Interface using
 * blocking method. This function will calculate the checksum byte and send it with the
 * frame data. Blocking means that the function does not return until the transmission is
 * complete. This function checks if txSize is in range from 1 to 8.
 * If not, it will return STATUS_ERROR. This function also checks if the isBusBusy
 * is false, if not it will return STATUS_BUSY. The function does not return
 * until the transmission is complete. If the transmission is successful, it
 * will return STATUS_SUCCESS. If not, it will return STATUS_TIMEOUT.
 *
 *END**************************************************************************/
status_t LIN_DRV_SendFrameDataBlocking(uint32_t instance, const uint8_t *txBuff, uint8_t txSize, uint32_t timeoutMSec)
{
    status_t retVal = STATUS_UNSUPPORTED;

    retVal = LIN_UART_DRV_SendFrameDataBlocking(instance, txBuff, txSize, timeoutMSec);

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_SendFrameData
 * Description   : This function sends data out through the LIN Hardware Interface using
 * non-blocking method. This function will calculate the checksum byte and send it with the
 * frame data. The function will return immediately after calling this function. If txSize
 * is equal to 0 or greater than 8 then the function will return STATUS_ERROR. If isBusBusy is
 * currently true then the function will return STATUS_BUSY.
 *
 *END**************************************************************************/
status_t LIN_DRV_SendFrameData(uint32_t instance, const uint8_t *txBuff, uint8_t txSize)
{
    status_t retVal = STATUS_UNSUPPORTED;

    retVal = LIN_UART_DRV_SendFrameData(instance, txBuff, txSize);

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_GetTransmitStatus
 * Description   : This function returns whether the previous transmission has
 * finished. When performing non-blocking transmit, the user can call this
 * function to ascertain the state of the current transmission:
 * in progress (or busy that STATUS_BUSY) or timeout (STATUS_TIMEOUT) or complete (success that is STATUS_SUCCESS).
 * In addition, if the transmission is still in progress, the user can obtain the number
 * of bytes that still needed to transmit.
 *
 *END**************************************************************************/
status_t LIN_DRV_GetTransmitStatus(uint32_t instance, uint8_t *bytesRemaining)
{
    status_t retVal = STATUS_UNSUPPORTED;

    retVal = LIN_UART_DRV_GetTransmitStatus(instance, bytesRemaining);

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_ReceiveFrameDataBlocking
 * Description   : This function receives data from LIN Hardware Interface module using blocking
 * method. This function receives data from UART module using blocking
 * method, the function does not return until the receive is complete. The interrupt
 * handler LIN_UART_DRV_IRQHandler will check the checksum byte. If the checksum
 * is correct, it will receive the frame data. If the checksum is incorrect, this
 * function will return STATUS_TIMEOUT and data in rxBuff might be wrong. This function
 * also check if rxSize is in range from 1 to 8. If not, it will return STATUS_ERROR.
 * This function also checks if the isBusBusy is false, if not it will return
 * STATUS_BUSY.
 *
 *END**************************************************************************/
status_t LIN_DRV_ReceiveFrameDataBlocking(uint32_t instance, uint8_t *rxBuff, uint8_t rxSize, uint32_t timeoutMSec)
{
    status_t retVal = STATUS_UNSUPPORTED;

    retVal = LIN_UART_DRV_RecvFrmDataBlocking(instance, rxBuff, rxSize, timeoutMSec);

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_ReceiveFrameData
 * Description   : This function receives data from LIN Hardware Interface using
 * non-blocking method. This function will check the checksum byte. If the checksum is
 * correct, it will receive it with the frame data.
 * Non-blocking  means that the function returns immediately.
 * The application has to get the receive status to know when the reception is complete.
 * The interrupt handler LIN_UART_DRV_IRQHandler will check the checksum byte. If the
 * checksum is correct, it will receive the frame data. If the checksum is incorrect, this
 * function will return STATUS_TIMEOUT and data in rxBuff might be wrong. This function also
 * check if rxSize is in range from 1 to 8. If not, it will return STATUS_ERROR. This function
 * also checks if the isBusBusy is false, if not it will return STATUS_BUSY.
 *
 *END**************************************************************************/
status_t LIN_DRV_ReceiveFrameData(uint32_t instance, uint8_t *rxBuff, uint8_t rxSize)
{
    status_t retVal = STATUS_UNSUPPORTED;

    retVal = LIN_UART_DRV_RecvFrmData(instance, rxBuff, rxSize);

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_AbortTransferData
 * Description   : Aborts an on-going non-blocking transmission/reception.
 * While performing a non-blocking transferring data, users can call this
 * function to terminate immediately the transferring.
 *
 *END**************************************************************************/
status_t LIN_DRV_AbortTransferData(uint32_t instance)
{
    status_t retVal = STATUS_UNSUPPORTED;

    retVal = LIN_UART_DRV_AbortTransferData(instance);

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_GetReceiveStatus
 * Description   : This function returns whether the data reception is complete.
 * When performing non-blocking transmit, the user can call this function to
 * ascertain the state of the current receive progress:
 * in progress (STATUS_BUSY) or timeout (STATUS_TIMEOUT) or complete (STATUS_SUCCESS).
 * In addition, if the reception is still in progress, the user can obtain the
 * number of bytes that still needed to receive.
 *
 *END**************************************************************************/
status_t LIN_DRV_GetReceiveStatus(uint32_t instance, uint8_t *bytesRemaining)
{
    status_t retVal = STATUS_UNSUPPORTED;

    retVal = LIN_UART_DRV_GetReceiveStatus(instance, bytesRemaining);

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_GoToSleepMode
 * Description   : This function puts current LIN node to sleep mode.
 * This function changes current node state to LIN_NODE_STATE_SLEEP_MODE.
 *
 *END**************************************************************************/
status_t LIN_DRV_GoToSleepMode(uint32_t instance)
{
    status_t retVal = STATUS_UNSUPPORTED;

    retVal = LIN_UART_DRV_GoToSleepMode(instance);

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_GotoIdleState
 * Description   : This function puts current node to Idle state.
 *
 *END**************************************************************************/
status_t LIN_DRV_GotoIdleState(uint32_t instance)
{
    status_t retVal = STATUS_UNSUPPORTED;

    retVal = LIN_UART_DRV_GotoIdleState(instance);

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_SendWakeupSignal
 * Description   : This function sends a wakeup signal through the UART interface.
 *
 *END**************************************************************************/
status_t LIN_DRV_SendWakeupSignal(uint32_t instance)
{
    status_t retVal = STATUS_UNSUPPORTED;

    retVal = LIN_UART_DRV_SendWakeupSignal(instance);

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_GetCurrentNodeState
 * Description   : This function gets the current LIN node state.
 *
 *END**************************************************************************/
lin_node_state_t LIN_DRV_GetCurrentNodeState(uint32_t instance)
{
    lin_node_state_t retVal = LIN_NODE_STATE_UNINIT;

    retVal = LIN_UART_DRV_GetCurrentNodeState(instance);

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_TimeoutService
 * Description   : This is callback function for Timer Interrupt Handler.
 * Users shall initialize a timer (for example SUPERTMR) in Output compare mode
 * with period of 500 micro seconds. In timer IRQ handler, call this function.
 *
 *END**************************************************************************/
void LIN_DRV_TimeoutService(uint32_t instance)
{
    LIN_UART_DRV_TimeoutService(instance);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_SetTimeoutCounter
 * Description   : This function sets value for timeout counter that is used in
 * LIN_DRV_TimeoutService
 *
 *END**************************************************************************/
void LIN_DRV_SetTimeoutCounter(uint32_t instance, uint32_t timeoutValue)
{
    LIN_UART_DRV_SetTimeoutCounter(instance, timeoutValue);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_MasterSendHeader
 * Description   : This function sends frame header out through the LIN Hardware Interface
 * using a non-blocking method. Non-blocking  means that the function returns
 * immediately. This function sends LIN Break field, sync field then the ID with
 * correct parity. This function checks if the interface is Master, if not, it will
 * return STATUS_ERROR.This function checks if id is in range from 0 to 0x3F, if not
 * it will return STATUS_ERROR.
 *
 *END**************************************************************************/
status_t LIN_DRV_MasterSendHeader(uint32_t instance, uint8_t id)
{
    status_t retVal = STATUS_UNSUPPORTED;

    retVal = LIN_UART_DRV_MasterSendHeader(instance, id);

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_EnableIRQ
 * Description   : This function enables LIN hardware interrupts.
 *
 *END**************************************************************************/
status_t LIN_DRV_EnableIRQ(uint32_t instance)
{
    status_t retVal = STATUS_UNSUPPORTED;

    retVal = LIN_UART_DRV_EnableIRQ(instance);

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_DisableIRQ
 * Description   : This function disables LIN hardware interrupts.
 *
 *END**************************************************************************/
status_t LIN_DRV_DisableIRQ(uint32_t instance)
{
    status_t retVal = STATUS_UNSUPPORTED;

    retVal = LIN_UART_DRV_DisableIRQ(instance);

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_IRQHandler
 * Description   : Interrupt handler for LIN Hardware Interface.
 * This is not a public API as it is called by IRQ whenever an interrupt
 * occurs.
 *
 *END**************************************************************************/
void LIN_DRV_IRQHandler(void *data)
{
    uint32_t instance = (uint32_t)data;
    LIN_UART_DRV_IRQHandler(instance);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_DRV_AutoBaudCapture
 * Description   : This function capture bits time to detect break char, calculate
 * baudrate from sync bits and enable transceiver if autobaud successful.
 * This function should only be used in Slave.
 * The timer should be in mode input capture of both rising and falling edges.
 * The timer input capture pin should be externally connected to RXD pin.
 *
 *END**************************************************************************/
status_t LIN_DRV_AutoBaudCapture(uint32_t instance)
{
    status_t retVal = STATUS_UNSUPPORTED;

    retVal = LIN_UART_DRV_AutoBaudCapture(instance);

    return retVal;
}
