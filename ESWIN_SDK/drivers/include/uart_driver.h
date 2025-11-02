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
 * @file uart_driver.h
 * @brief UART driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __UART_DRIVER_H__
#define __UART_DRIVER_H__

#include "callbacks.h"
#include "clock_driver.h"
#include "pdma_driver.h"
#include "platform.h"

#include <stddef.h>

/**
 * @brief Type of UART transfer (based on interrupts or PDMA).
 */
typedef enum {
    UART_USING_DMA        = 0, /**< The driver will use PDMA to perform UART transfer */
    UART_USING_INTERRUPTS = 1, /**< The driver will use interrupts to perform UART transfer */
} uart_transfer_type_t;

/**
 * @brief Type of UART fifo use.
 */
typedef enum {
    UART_FIFO_DEPTH_1   = 0, /**< FIFO depth is    1. */
    UART_FIFO_DEPTH_4   = 1, /**< FIFO depth is    4. */
    UART_FIFO_DEPTH_8   = 2, /**< FIFO depth is    8. */
    UART_FIFO_DEPTH_16  = 3, /**< FIFO depth is   16. */
    UART_FIFO_DEPTH_32  = 4, /**< FIFO depth is   32. */
    UART_FIFO_DEPTH_64  = 5, /**< FIFO depth is   64. */
    UART_FIFO_DEPTH_128 = 6, /**< FIFO depth is  128. */
    UART_FIFO_DEPTH_256 = 7, /**< FIFO depth is  256. */
} uart_fifo_type_t;

/**
 * @brief Configure idle character length.
 */
typedef enum {
    UART_CHARS_LENGTH_1   = 0, /**< Idle character length is   1. */
    UART_CHARS_LENGTH_2   = 1, /**< Idle character length is   2. */
    UART_CHARS_LENGTH_4   = 2, /**< Idle character length is   4. */
    UART_CHARS_LENGTH_8   = 3, /**< Idle character length is   8. */
    UART_CHARS_LENGTH_16  = 4, /**< Idle character length is  16. */
    UART_CHARS_LENGTH_32  = 5, /**< Idle character length is  32. */
    UART_CHARS_LENGTH_64  = 6, /**< Idle character length is  64. */
    UART_CHARS_LENGTH_128 = 7, /**< Idle character length is 128. */
} uart_character_type_t;

/**
 * @brief Idle line type select.
 */
typedef enum {
    UART_LINE_AFTER_START = 0, /**< Idle character bit count starts after start bit. */
    UART_LINE_AFTER_STOP  = 1, /**< Idle character bit count starts after stop bit. */
} uart_idle_line_type_t;

/**
 * @brief Idle line interrupt enable.
 */
typedef enum {
    UART_IDLE_IR_DISABLE = 0, /**< Idle interrupt disble. */
    UART_IDLE_IR_ENABLE  = 1, /**< Idle interrupt enable. */
} uart_idle_interrupt_type_t;

/**
 * @brief Parity error interrupt enable.
 */
typedef enum {
    UART_PARITY_IR_DISABLE = 0, /**< Parity interrupt disble. */
    UART_PARITY_IR_ENABLE  = 1, /**< Parity interrupt enable. */
} uart_parity_interrupt_type_t;

/**
 * @brief Overrun interrupt enable.
 */
typedef enum {
    UART_OVERRUN_IR_DISABLE = 0, /**< Overflow interrupt disable. */
    UART_OVERRUN_IR_ENABLE  = 1, /**< Overflow interrupt enable. */
} uart_overrrun_interrupt_type_t;

/**
 * @brief UART number of bits in a character.
 */
typedef enum {
    UART_8_BITS_PER_CHAR  = 0x0U, /**<  8-bit data characters. */
    UART_9_BITS_PER_CHAR  = 0x1U, /**<  9-bit data characters. */
    UART_10_BITS_PER_CHAR = 0x2U, /**< 10-bit data characters. */
    UART_7_BITS_PER_CHAR  = 0x3U, /**< 7-bit data characters. */
} uart_bit_count_per_char_t;

/**
 * @brief UART parity mode.
 */
typedef enum {
    UART_PARITY_DISABLED = 0x0U, /**< Parity disabled. */
    UART_PARITY_EVEN     = 0x2U, /**< Parity enabled, type even, bit setting: PE|PT = 10. */
    UART_PARITY_ODD      = 0x3U, /**< Parity enabled, type odd,  bit setting: PE|PT = 11. */
} uart_parity_mode_t;

/**
 * @brief UART MSB or LSB mode.
 */
typedef enum {
    UART_LSB_FIRST = 0x0U, /**< LSB first mode. */
    UART_MSB_FIRST = 0x1U, /**< MSB first mode. */
} uart_msbf_mode_t;

typedef enum {
    UART_TX_RX_NORM = 0x0U, /**< Transmit data not inverted. */
    UART_TX_RX_INV  = 0x1U, /**< Transmit data inverted. */
} uart_tx_rx_inve_t;

/**
 * @brief UART number of stop bits.
 */
typedef enum {
    UART_ONE_STOP_BIT = 0x0U, /**< One stop bit. */
    UART_TWO_STOP_BIT = 0x1U, /**< Two stop bits. */
} uart_stop_bit_count_t;

/**
 * @brief Runtime state of the UART driver.
 *
 * Note that the caller provides memory for the driver state structures during
 * initialization because the driver does not statically allocate memory.
 */
typedef struct {
    int instance;                              /**< Uart instance number. */
    const uint8_t *txBuff;                     /**< The buffer of data being sent. */
    uint8_t *rxBuff;                           /**< The buffer of received data. */
    volatile uint32_t txSize;                  /**< The remaining number of bytes to be transmitted. */
    volatile uint32_t rxSize;                  /**< The remaining number of bytes to be received. */
    volatile bool isTxBusy;                    /**< True if there is an active transmit. */
    volatile bool isRxBusy;                    /**< True if there is an active receive. */
    volatile bool isTxBlocking;                /**< True if transmit is blocking transaction. */
    volatile bool isRxBlocking;                /**< True if receive is blocking transaction. */
    uart_bit_count_per_char_t bitCountPerChar; /**< number of bits in a char (8/9/10). */
    uart_callback_t rxCallback;                /**< Callback to invoke for data receive
                                                    Note: when the transmission is interrupt based, the callback
                                                    is being called upon receiving a byte; when PDMA transmission
                                                    is used, the bytes are copied to the rx buffer by the PDMA engine
                                                    and the callback is called when all the bytes have been transferred. */
    void *rxCallbackParam;                     /**< Receive callback parameter pointer. */
    uart_callback_t txCallback;                /**< Callback to invoke for data send
                                                    Note: when the transmission is interrupt based, the callback
                                                    is being called upon sending a byte; when PDMA transmission
                                                    is used, the bytes are copied to the tx buffer by the PDMA engine
                                                    and the callback is called when all the bytes have been transferred. */
    void *txCallbackParam;                     /**< Transmit callback parameter pointer. */
    uart_transfer_type_t transferType;         /**< Type of UART transfer (interrupt/dma based). */
    uint8_t rxDMAChannel;                      /**< PDMA channel number for PDMA-based rx. */
    uint8_t txDMAChannel;                      /**< PDMA channel number for PDMA-based tx. */
    OS_SemaphoreId_t rxComplete;               /**< Synchronization object for blocking Rx timeout condition. */
    OS_SemaphoreId_t txComplete;               /**< Synchronization object for blocking Tx timeout condition. */
    volatile status_t transmitStatus;          /**< Status of last driver transmit operation. */
    volatile status_t receiveStatus;           /**< Status of last driver receive operation. */
} uart_state_t;

/**
 * @brief UART configuration structure.
 */
typedef struct {
    uint32_t baudRate;                         /**< UART baud rate. \D{115200UL} */
    uart_parity_mode_t parityMode;             /**< Parity mode, disabled (default), even, odd. \D{UART_PARITY_DISABLED} */
    uart_stop_bit_count_t stopBitCount;        /**< Number of stop bits, 1 stop bit (default) or 2 stop bits. \D{UART_ONE_STOP_BIT} */
    uart_bit_count_per_char_t bitCountPerChar; /**< Number of bits in a character (8-default, 9 or 10):
                                                for 9/10 bits chars, users must provide appropriate buffers
                                                to the send/receive functions (bits 8/9 in subsequent bytes);
                                                for PDMA transmission only 8-bit char is supported. \D{UART_8_BITS_PER_CHAR} */
    uart_transfer_type_t transferType;         /**< Type of UART transfer (interrupt/dma based). \D{UART_USING_INTERRUPTS} */
    uart_fifo_type_t fifoType;                 /**< Type of UART fifo depth. \D{UART_FIFO_DEPTH_1} */
    uint8_t rxDMAChannel;                      /**< Channel number for PDMA rx channel.If PDMA mode isn't used this field will be ignored. \D{0} */
    uint8_t txDMAChannel;                      /**< Channel number for PDMA tx channel.If PDMA mode isn't used this field will be ignored. \D{1} */
} uart_user_config_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Initializes the UART configuration structure with default values.
 *
 * This function initializes a configuration structure received from the application
 * with default values.
 *
 * @param uartUserConfig User configuration structure of type #uart_user_config_t.
 */
void UART_DRV_GetDefaultConfig(uart_user_config_t *uartUserConfig);

/**
 * @brief Initializes an UART operation instance.
 *
 * The caller provides memory for the driver state structures during initialization.
 * The user must select the UART clock source in the application to initialize the UART.
 *
 * @param instance  The UART instance number.
 * @param uartUserConfig User configuration structure of type #uart_user_config_t.
 * @param uartStatePtr Pointer to the UART driver state structure.
 * @return status_t
 * STATUS_SUCCESS If successful.
 * STATUS_ERROR If an error occurred.
 */
status_t UART_DRV_Init(uint32_t instance, uart_state_t *uartStatePtr, const uart_user_config_t *uartUserConfig);

/**
 * @brief Initializes the UART fifo.
 *
 * @param instance UART base pointer.
 * @param water UART watermark
 */
void UART_DRV_FIFOWater_Init(uint32_t instance, uint16_t water);

/**
 * @brief Shuts down the UART by disabling interrupts and transmitter/receiver.
 *
 * @param instance  The UART instance number.
 * @return status_t
 * STATUS_SUCCESS If successful.
 * STATUS_ERROR If an error occurred.
 */
status_t UART_DRV_Deinit(uint32_t instance);

/**
 * @brief Installs callback function for the UART receive.
 *
 * @note After a callback is installed, it bypasses part of the UART IRQHandler logic.
 * Therefore, the callback needs to handle the indexes of txBuff and txSize.
 *
 * @param instance The UART instance number.
 * @param function The UART receive callback function.
 * @param callbackParam The UART receive callback parameter pointer.
 * @return uart_callback_t Former UART receive callback function pointer.
 */
uart_callback_t UART_DRV_InstallRxCallback(uint32_t instance, uart_callback_t function, void *callbackParam);

/**
 * @brief Installs callback function for the UART transmit.
 *
 * @note After a callback is installed, it bypasses part of the UART IRQHandler logic.
 * Therefore, the callback needs to handle the indexes of txBuff and txSize.
 *
 * @param instance The UART instance number.
 * @param function The UART transmit callback function.
 * @param callbackParam The UART transmit callback parameter pointer.
 * @return uart_callback_t Former UART transmit callback function pointer.
 */
uart_callback_t UART_DRV_InstallTxCallback(uint32_t instance, uart_callback_t function, void *callbackParam);

/**
 * @brief Sends data out through the UART module using a blocking method.
 *
 *  Blocking means that the function does not return until the transmission is complete.
 *
 * @param instance  The UART instance number.
 * @param txBuff  Source buffer containing 8-bit data chars to send.
 * @param txSize The number of bytes to send.
 * @param timeout Timeout value in milliseconds.
 * @return status_t
 * STATUS_SUCCESS If successful.
 * STATUS_TIMEOUT if the timeout was reached.
 * STATUS_BUSY If the resource is busy.
 * STATUS_ERROR if an error occurred.
 */
status_t UART_DRV_SendDataBlocking(uint32_t instance, const uint8_t *txBuff, uint32_t txSize, uint32_t timeout);

/**
 * @brief Send out multiple bytes of data using polling method.
 *
 * @param   instance  The UART instance number.
 * @param   txBuff The buffer pointer which saves the data to be sent.
 * @param   txSize Size of data to be sent in unit of byte.
 * @return status_t
 * STATUS_SUCCESS If successful.
 * STATUS_BUSY If the resource is busy.
 */
status_t UART_DRV_SendDataPolling(uint32_t instance, const uint8_t *txBuff, uint32_t txSize);

/**
 * @brief Send out multiple bytes of data using polling method.
 *
 * @param   instance  The UART instance number.
 * @param   txBuff The buffer pointer which saves the data to be sent.
 * @param   txSize Size of data to be sent in unit of byte.
 * @param   timeout timeout value in instruction cycles
 * @return status_t
 * STATUS_SUCCESS If successful.
 * STATUS_BUSY If the resource is busy.
 */
status_t UART_DRV_SendDataPollingWithTimeout(uint32_t instance, const uint8_t *txBuff, uint32_t txSize,
                                             uint32_t timeout);

/**
 * @brief Sends data out through the UART module using a non-blocking method.
 *  This enables an a-sync method for transmitting data. When used with
 *  a non-blocking receive, the UART can perform a full duplex operation.
 *  Non-blocking  means that the function returns immediately.
 *  The application has to get the transmit status to know when the transmit is complete.
 *
 * @param instance  The UART instance number.
 * @param txBuff  Source buffer containing 8-bit data chars to send.
 * @param txSize  the number of bytes to send
 * @return status_t
 * STATUS_SUCCESS If successful.
 * STATUS_BUSY If the resource is busy.
 */
status_t UART_DRV_SendData(uint32_t instance, const uint8_t *txBuff, uint32_t txSize);

/**
 * @brief Returns whether the previous transmit is complete.
 *
 * @param instance  The UART instance number.
 * @param bytesRemaining Pointer to value that is populated with the number of bytes that
 *        have been sent in the active transfer.
 *        @note In PDMA mode, this parameter may not be accurate, in case the transfer completes
 *              right after calling this function; in this edge-case, the parameter will reflect
 *              the initial transfer size, due to automatic reloading of the major loop count
 *              in the PDMA transfer descriptor.
 * @return status_t The transmit status.
 * STATUS_SUCCESS The transmit has completed successfully.
 * STATUS_BUSY The transmit is still in progress. A bytesRemaining will be filled with the number of bytes
 *             that are yet to be transmitted.
 * STATUS_UART_ABORTED The transmit was aborted.
 * STATUS_TIMEOUT A timeout was reached.
 * STATUS_ERROR An error occurred.
 */
status_t UART_DRV_GetTransmitStatus(uint32_t instance, uint32_t *bytesRemaining);

/**
 * @brief Terminates a non-blocking transmission early.
 *
 * @param instance  The UART instance number.
 * @return status_t Whether the aborting is successful or not.
 */
status_t UART_DRV_AbortSendingData(uint32_t instance);

/**
 * @brief Gets data from the UART module by using a blocking method.
 *  Blocking means that the function does not return until the
 *  receive is complete.
 *
 * @param instance  The UART instance number.
 * @param rxBuff  Buffer containing 8-bit read data chars received.
 * @param rxSize the number of bytes to receive
 * @param timeout Timeout value in milliseconds.
 * @return status_t
 * STATUS_SUCCESS If successful.
 * STATUS_TIMEOUT If the timeout was reached.
 * STATUS_BUSY If the resource is busy.
 * STATUS_UART_FRAMING_ERROR If a framing error occurred.
 * STATUS_UART_NOISE_ERROR If a noise error occurred.
 * STATUS_UART_PARITY_ERROR If a parity error occurred.
 * STATUS_UART_RX_OVERRUN If an overrun error occurred.
 * STATUS_ERROR If a PDMA error occurred.
 */
status_t UART_DRV_ReceiveDataBlocking(uint32_t instance, uint8_t *rxBuff, uint32_t rxSize, uint32_t timeout);

/**
 * @brief Receive multiple bytes of data using polling method.
 *
 *
 * @param   instance  The UART instance number.
 * @param   rxBuff The buffer pointer which saves the data to be received.
 * @param   rxSize Size of data need to be received in unit of byte.
 * @return status_t
 * STATUS_SUCCESS If the transaction is successful.
 * STATUS_BUSY If the resource is busy.
 * STATUS_UART_RX_OVERRUN if an overrun error occurred.
 */
status_t UART_DRV_ReceiveDataPolling(uint32_t instance, uint8_t *rxBuff, uint32_t rxSize);

/**
 * @brief Gets data from the UART module by using a non-blocking method.
 *  This enables an a-sync method for receiving data. When used with
 *  a non-blocking transmission, the UART can perform a full duplex operation.
 *  Non-blocking means that the function returns immediately.
 *  The application has to get the receive status to know when the receive is complete.
 *
 * @param instance  The UART instance number.
 * @param rxBuff  Buffer containing 8-bit read data chars received.
 * @param rxSize  The number of bytes to receive.
 * @return status_t
 * STATUS_SUCCESS If successful.
 * STATUS_BUSY If the resource is busy.
 */
status_t UART_DRV_ReceiveData(uint32_t instance, uint8_t *rxBuff, uint32_t rxSize);

/**
 * @brief Set uart loopback mode.
 *
 * @param instance  The UART instance number.
 * @param en Enable or disable.
 */
void UART_DRV_SetLoopback(uint32_t instance, bool en);

/**
 * @brief Set uart MSB or LSB first mode.
 *
 * @param instance  The UART instance number.
 * @param mode MSB or LSB first mode.
 */
void UART_DRV_SetMSBF(uint32_t instance, uart_msbf_mode_t mode);

/**
 * @brief  Configures the number of idle characters that must be received 
 * before the IDLE flag is set.
 *
 * @param instance  The UART instance number.
 * @param idleCharsType idle character length type.
 */
void UART_DRV_SetIdleCharsLen(uint32_t instance, uart_character_type_t idleCharsType);

/**
 * @brief Enables the idle line interrupt
 *
 * This function enables the idle line interrupt.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 * @param instance  The UART instance number.
 * @param uart_idle_interrupt_type_t  Enables or disables the idle interrupt. 
 */
void UART_DRV_SetIdleIREnable(uint32_t instance, uart_idle_interrupt_type_t idleIRType);

/**
 * @brief Overrun Interrupt Enable or not;
 *
 * This bit enables the overrun flag (OR) to generate hardware interrupt requests.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 * @param instance  The UART instance number.
 * @param OverrunIrqType  Enables or disanables the overrun error interrupt.
 */
void UART_DRV_OverrunIREnable(uint32_t instance, uart_overrrun_interrupt_type_t OverrunIrqType);

/**
 * @brief Configures receive data inverse in the UART controller.
 *
 * This function configures receive data inverse in the UART controller.
 * In some UART instances, the user should disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 * @param instance  The UART instance number.
 * @param invModeType  inverse mode(inverse the receive data or not)
 */
void UART_DRV_SetRxInv(uint32_t instance, uart_tx_rx_inve_t invModeType);

/**
 * @brief Configures transmit data inverse in the UART controller.
 *
 * This function configures transmit data inverse in the UART controller.
 * In some UART instances, the user should disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 * @param instance  The UART instance number.
 * @param invModeType  inverse mode(inverse the transmit data or not)
 */
void UART_DRV_SetTxInv(uint32_t instance, uart_tx_rx_inve_t invModeType);

/**
 * @brief Return whether there is a receiver overrun event.
 *
 * This function returns whether there is a receiver overrun event.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 * @param instance  The UART instance number.
 */
bool UART_DRV_IsRxOverrun(uint32_t instance);

/**
 * @brief Returns the transmit Buffer/FIFO empty or not.
 *
 * This function asserts when there is no data in the Transmit FIFO/buffer.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 * @param instance  The UART instance number.
 */
bool UART_DRV_IsTxFIFOEmpty(uint32_t instance);

/**
 * @brief Returns the transmit Buffer/FIFO empty or not.
 *
 * If the function returns 1, it indicates that there is an active edge on the RXD pin. 
 * If it is 0, it indicates that there is no active edge on the RXD pin. 
 *
 * @param instance  The UART instance number.
 */
bool UART_DRV_IsRxActiveEage(uint32_t instance);

uint32_t UART_DRV_RxTxFIFODepth(uint32_t instance, bool rxFIFO);

/**
 * @brief Returns whether the previous receive is complete.
 *
 * @param instance  The UART instance number.
 * @param bytesRemaining pointer to value that is filled  with the number of bytes that
 *        still need to be received in the active transfer.
 *        @note In PDMA mode, this parameter may not be accurate, in case the transfer completes
 *              right after calling this function; in this edge-case, the parameter will reflect
 *              the initial transfer size, due to automatic reloading of the major loop count
 *              in the PDMA transfer descriptor.
 * @return status_t The receive status.
 * STATUS_SUCCESS The receive has completed successfully.
 * STATUS_BUSY The receive is still in progress. A bytesReceived will be
 *             filled with the number of bytes that have been received so far.
 * STATUS_UART_ABORTED The receive was aborted.
 * STATUS_TIMEOUT A timeout was reached.
 * STATUS_UART_RX_OVERRUN, STATUS_UART_FRAMING_ERROR, STATUS_UART_PARITY_ERROR,
 * or STATUS_UART_NOISE_ERROR, STATUS_ERROR An error occurred during reception.
 */
status_t UART_DRV_GetReceiveStatus(uint32_t instance, uint32_t *bytesRemaining);

/**
 * @brief Terminates a non-blocking receive early.
 *
 * @param instance  The UART instance number.
 *
 * @return status_t Whether the receiving was successful or not.
 */
status_t UART_DRV_AbortReceivingData(uint32_t instance);

/**
 * @brief Configures the UART baud rate.
 *
 * This function configures the UART baud rate.
 * In some UART instances the user must disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all UARTs to ensure safe operation.
 *
 * @param instance  The UART instance number.
 * @param desiredBaudRate UART desired baud rate.
 * @return status_t
 * STATUS_BUSY If called during an on-going transfer.
 * STATUS_SUCCESS Otherwise.
 */
status_t UART_DRV_SetBaudRate(uint32_t instance, uint32_t desiredBaudRate);

/**
 * @brief Returns the UART baud rate.
 *
 * This function returns the UART configured baud rate.
 *
 * @param instance  The UART instance number.
 * @param configuredBaudRate UART configured baud rate.
 */
void UART_DRV_GetBaudRate(uint32_t instance, uint32_t *configuredBaudRate);

/**
 * @brief Sets the internal driver reference to the tx buffer.
 *
 * This function can be called from the tx callback to provide the driver
 * with a new buffer, for continuous transmission.
 *
 * @param instance  The UART instance number.
 * @param txBuff  Source buffer containing 8-bit data chars to send.
 * @param txSize  The number of bytes to send.
 * @return status_t STATUS_SUCCESS.
 */
status_t UART_DRV_SetTxBuffer(uint32_t instance, const uint8_t *txBuff, uint32_t txSize);

/**
 * @brief Sets the internal driver reference to the rx buffer.
 *
 * This function can be called from the rx callback to provide the driver
 * with a new buffer, for continuous reception.
 *
 * @param instance  The UART instance number.
 * @param rxBuff  Destination buffer containing 8-bit data chars to receive.
 * @param rxSize  The number of bytes to receive.
 * @return status_t STATUS_SUCCESS.
 */
status_t UART_DRV_SetRxBuffer(uint32_t instance, uint8_t *rxBuff, uint32_t rxSize);

/**
 * @brief Set pin Configuration.
 * @param instance  The UART instance number.
 * @param conf  Trigger Select
 * 00b - Input trigger is disabled.
 * 01b - Input trigger is used instead of RXD pin input.
 * 10b - Input trigger is used instead of CTS_B pin input.
 * 11b - Input trigger is used to modulate the TXD pin output. The TXD pin output (after TXINV
 *       configuration) is ANDed with the input trigger.
 */
void UART_DRV_SetPinCfg(uint32_t instance, uint8_t conf);

/**
 * @brief Set Transmitter clear-to-send enable or disable.
 * @param instance  The UART instance number.
 * @param enable Enable or disable CTS.
 */
void UART_DRV_SetCts(uint32_t instance, bool enable);

/**
 * @brief Set Receiver request-to-send enable
 * @param instance The UART instance number.
 * @param enable Enable or disable RTS.
 */
void UART_DRV_SetRts(uint32_t instance, bool enable);

/**
 * @brief Get uart version id
 * @param instance The UART instance number.
 * @return uint32_t Version ID
 */
uint32_t UART_DRV_GetVersionID(uint32_t instance);

#if defined(__cplusplus)
}
#endif

#endif // __UART_DRIVER_H__
