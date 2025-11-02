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
 * @file superio_uart_driver.h
 * @brief SUPERIO uart driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __SUPERIO_UART_DRIVER_H__
#define __SUPERIO_UART_DRIVER_H__

#include "callbacks.h"
#include "pdma_driver.h"
#include "superio_driver.h"
#include <stdbool.h>
#include <stddef.h>

/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup SUPERIO_UART_Driver SUPERIO_UART Driver
 * @{
 */

/**
 * @brief superio_uart driver direction (tx or rx).
 *
 * This structure describes the direction configuration options for the superio_uart driver.
 */
typedef enum {
    SUPERIO_UART_DIRECTION_TX = 0x01U, /**< Tx UART driver. */
    SUPERIO_UART_DIRECTION_RX = 0x00U, /**< Rx UART driver. */
} superio_uart_driver_direction_t;

/**
 * @brief Driver configuration structure.
 *
 * This structure is used to provide configuration parameters for the superio_uart driver at initialization time.
 */
typedef struct {
    superio_driver_type_t driverType; /**< Driver type: interrupts/polling/PDMA. \D{SUPERIO_DRIVER_TYPE_INTERRUPTS} */
    uint32_t baudRate;                /**< Baud rate in hertz. \R{100,3000000} \D{115200UL} */
    uint8_t bitCount;                 /**< Number of bits per word. \R{7,10} \D{8U} */
    superio_uart_driver_direction_t direction; /**< Driver direction: Tx or Rx. \D{SUPERIO_UART_DIRECTION_TX} */
    uint8_t dataPin;                           /**< SuperIO pin to use as Tx or Rx pin. \R{0,7} \D{2U} */
    uart_callback_t callback;                  /**< User callback function. Note that this function will be
                                                    called from the interrupt service routine, so its
                                                    execution time should be as small as possible. It can be
                                                    NULL if it is not needed. \D{NULL} */
    void *callbackParam;                       /**< Parameter for the callback function. \D{NULL} */
    uint8_t dmaChannel;                        /**< PDMA channel number. Only used in PDMA mode. \R{0,32} \D{0U} */
} superio_uart_user_config_t;

/**
 * @brief Driver internal context structure.
 *
 * This structure is used by the superio_uart driver for its internal logic. It must
 * be provided by the application through the SUPERIO_UART_DRV_Init() function, then
 * it cannot be freed until the driver is de-initialized using SUPERIO_UART_DRV_DeInit().
 * The application should make no assumptions about the content of this structure.
 */
typedef struct {
    superio_common_state_t superioCommon;      /**< Common superio drivers structure. */
    uint8_t *rxData;                           /**< Receive buffer. */
    const uint8_t *txData;                     /**< Transmit buffer. */
    uint32_t remainingBytes;                   /**< Number of remaining bytes to be transferred. */
    uint8_t dmaChannel;                        /**< PDMA channel number. */
    uart_callback_t callback;                  /**< User callback function. */
    void *callbackParam;                       /**< Parameter for the callback function. */
    superio_uart_driver_direction_t direction; /**< Driver direction: Tx or Rx. */
    superio_driver_type_t driverType;          /**< Driver type: interrupts/polling/PDMA. */
    status_t status;                           /**< Current status of the driver. */
    bool driverIdle;                           /**< Idle/busy state of the receiver. */
    OS_SemaphoreId_t idleSemaphore;            /**< Semaphore used by blocking functions. */
    bool blocking;                             /**< Specifies if the current transfer is blocking. */
    uint8_t bitCount;                          /**< Number of bits per word. */
    uint8_t txFlush;                           /**< Used for flushing Tx buffer before ending a transmission. */
} superio_uart_state_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Initialize the SUPERIO_UART driver.
 *
 * This function initializes the SUPERIO_UART driver.
 *
 * @param instance  SUPERIO peripheral instance number.
 * @param userConfigPtr    Pointer to the SUPERIO_UART user configuration structure. The function
 *                         reads configuration data from this structure and initializes the
 *                         driver accordingly. The application may free this structure after
 *                         the function returns.
 * @param state     Pointer to the SUPERIO_UART driver context structure. The driver uses
 *                  this memory area for its internal logic. The application must make no
 *                  assumptions about the content of this structure, and must not free this
 *                  memory until the driver is de-initialized using SUPERIO_UART_DRV_Deinit().
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_UART_DRV_Init(uint32_t instance, const superio_uart_user_config_t *userConfigPtr,
                               superio_uart_state_t *state);

/**
 * @brief De-initialize the SUPERIO_UART driver.
 *
 * This function de-initializes the SUPERIO_UART driver. The driver can't be used
 * again until reinitialized. The context structure is no longer needed by the driver and
 * can be freed after calling this function.
 *
 * @param state    Pointer to the SUPERIO_UART driver context structure.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_UART_DRV_Deinit(superio_uart_state_t *state);

/**
 * @brief Set the baud rate and bit width for any subsequent UART communication.
 *
 * This function sets the baud rate and bit width for the UART driver.
 * Note that due to module limitation not any baud rate can be achieved. The driver
 * will set a baud rate as close as possible to the requested baud rate, but there may
 * still be substantial differences, for example if requesting a high baud rate while
 * using a low-frequency SuperIO clock. The application should call
 * SUPERIO_UART_DRV_GetBaudRate() after SUPERIO_UART_DRV_SetConfig() to check
 * what baud rate was actually set.
 *
 * @param state    Pointer to the SUPERIO_UART driver context structure.
 * @param baudRate  the desired baud rate in hertz
 * @param bitCount  number of bits per word
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_UART_DRV_SetConfig(superio_uart_state_t *state, uint32_t baudRate, uint8_t bitCount);

/**
 * @brief Get the currently configured baud rate.
 *
 * This function returns the currently configured UART baud rate.
 *
 * @param state    Pointer to the SUPERIO_UART driver context structure.
 * @param baudRate  The current baud rate in hertz.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_UART_DRV_GetBaudRate(superio_uart_state_t *state, uint32_t *baudRate);

/**
 * @brief Perform a blocking UART transmission.
 *
 * This function sends a block of data and only returns when the transmission is complete.
 *
 * @param state     Pointer to the SUPERIO_UART driver context structure.
 * @param txBuff    Pointer to the data to be transferred.
 * @param txSize    Length in bytes of the data to be transferred.
 * @param timeout   Timeout for the transfer in milliseconds.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_UART_DRV_SendDataBlocking(superio_uart_state_t *state, const uint8_t *txBuff, uint32_t txSize,
                                           uint32_t timeout);

/**
 * @brief Perform a non-blocking UART transmission.
 *
 * This function sends a block of data and returns immediately.
 * The rest of the transmission is handled by the interrupt service routine (if the driver
 * is initialized in interrupt mode) or by the SUPERIO_UART_DRV_GetTransmitStatus() function (if
 * the driver is initialized in polling mode).
 *
 * @param state     Pointer to the SUPERIO_UART driver context structure.
 * @param txBuff    Pointer to the data to be transferred.
 * @param txSize    Length in bytes of the data to be transferred.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_UART_DRV_SendData(superio_uart_state_t *state, const uint8_t *txBuff, uint32_t txSize);

/**
 * @brief Perform a blocking UART reception.
 *
 * This function receives a block of data and
 * only returns when the transmission is complete.
 *
 * @param state     Pointer to the SUPERIO_UART driver context structure.
 * @param rxBuff    Pointer to the receive buffer.
 * @param rxSize    Length in bytes of the data to be received.
 * @param timeout   Timeout for the transfer in milliseconds.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_UART_DRV_ReceiveDataBlocking(superio_uart_state_t *state, uint8_t *rxBuff, uint32_t rxSize,
                                              uint32_t timeout);

/**
 * @brief Perform a non-blocking UART reception.
 *
 * This function receives a block of data and returns immediately.
 * The rest of the transmission is handled by the interrupt service routine (if the driver
 * is initialized in interrupt mode) or by the SUPERIO_UART_DRV_GetReceiveStatus() function (if
 * the driver is initialized in polling mode).
 *
 * @param state     Pointer to the SUPERIO_UART driver context structure.
 * @param rxBuff    Pointer to the receive buffer.
 * @param rxSize    Length in bytes of the data to be received.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_UART_DRV_ReceiveData(superio_uart_state_t *state, uint8_t *rxBuff, uint32_t rxSize);

/**
 * @brief Get the status of the current non-blocking UART transfer
 *
 * This function returns the current status of a non-blocking UART transfer.
 * A return code of STATUS_BUSY means the transfer is still in progress.
 * Otherwise the function returns a status reflecting the outcome of the last transfer.
 * When the driver is initialized in polling mode this function also advances the transfer
 * by checking and handling the transmit and receive events, so it must be called
 * frequently to avoid overflows or underflows.
 *
 * @param state     Pointer to the SUPERIO_UART driver context structure.
 * @param bytesRemaining  the remaining number of bytes to be transferred
 *        @note In PDMA mode, this parameter may not be accurate, in case the transfer completes
 *              right after calling this function; in this edge-case, the parameter will reflect
 *              the initial transfer size, due to automatic reloading of the major loop count
 *              in the PDMA transfer descriptor.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_UART_DRV_GetStatus(superio_uart_state_t *state, uint32_t *bytesRemaining);

/**
 * @brief Aborts a non-blocking UART transfer.
 *
 * This function aborts a non-blocking UART transfer.
 *
 * @param state     Pointer to the SUPERIO_UART driver context structure.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_UART_DRV_TransferAbort(superio_uart_state_t *state);

/**
 * @brief Provide a buffer for receiving data.
 *
 * This function can be used to provide a new buffer for receiving data to the driver.
 * It can be called from the user callback when event STATUS_UART_RX_OVERRUN is reported.
 * This way the reception will continue without interruption.
 *
 * @param state     Pointer to the SUPERIO_UART driver context structure.
 * @param rxBuff    Pointer to the buffer where to store received data.
 * @param rxSize    Length in bytes of the data to be transferred.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_UART_DRV_SetRxBuffer(superio_uart_state_t *state, uint8_t *rxBuff, uint32_t rxSize);

/**
 * @brief Provide a buffer for transmitting data.
 *
 * This function can be used to provide a new buffer for transmitting data to the driver.
 * It can be called from the user callback when event STATUS_UART_TX_UNDERRUN is reported.
 * This way the transmission will continue without interruption.
 *
 * @param state    Pointer to the SUPERIO_UART driver context structure.
 * @param txBuff    Pointer to the buffer containing transmit data.
 * @param txSize    Length in bytes of the data to be transferred.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_UART_DRV_SetTxBuffer(superio_uart_state_t *state, const uint8_t *txBuff, uint32_t txSize);

/**
 * @brief Returns default configuration structure for SUPERIO_UART.
 *
 * @param userConfigPtr    Pointer to the SUPERIO_UART user configuration structure.
 */
void SUPERIO_UART_DRV_GetDefaultConfig(superio_uart_user_config_t *userConfigPtr);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */ /* end of group SUPERIO_UART_Driver */

/**
 * @}
 */ /* end of group Peripheral_Driver_layer */

#endif /* __SUPERIO_UART_DRIVER_H__ */
