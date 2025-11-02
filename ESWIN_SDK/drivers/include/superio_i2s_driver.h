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
 * @file superio_i2s_driver.h
 * @brief SUPERIO i2s driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __SUPERIO_I2S_DRIVER_H__
#define __SUPERIO_I2S_DRIVER_H__

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
 * @addtogroup SUPERIO_I2S_Driver SUPERIO_I2S Driver
 * @{
 */

/**
 * @brief Master configuration structure.
 *
 * This structure is used to provide configuration parameters for the superio_i2s master at initialization time.
 */
typedef struct {
    superio_driver_type_t driverType; /**< Driver type: interrupts/polling/PDMA. \D{SUPERIO_DRIVER_TYPE_INTERRUPTS} */
    uint32_t baudRate;                /**< Baud rate in hertz. \R{100,100000} \D{1000}*/
    uint8_t bitsWidth;                /**< Number of bits in a word - multiple of 8. \R{7,10} \D{8} */
    uint8_t txPin;                    /**< Superio pin to use for transmit. \R{0,7} \D{0} */
    uint8_t rxPin;                    /**< Superio pin to use for receive. \R{0,7} \D{1} */
    uint8_t sckPin;                   /**< Superio pin to use for serial clock. \R{0,7} \D{2} */
    uint8_t wsPin;                    /**< Superio pin to use for word select. \R{0,7} \D{3} */
    i2s_callback_t callback;          /**< User callback function. Note that this function will be
                                            called from the interrupt service routine, so its
                                            execution time should be as small as possible. It can be
                                            NULL if it is not needed. */
    void *callbackParam;              /**< Parameter for the callback function. */
    uint8_t rxDMAChannel;             /**< Rx PDMA channel number. Only used in PDMA mode. \R{0,32} \D{0} */
    uint8_t txDMAChannel;             /**< Tx PDMA channel number. Only used in PDMA mode. \R{0,32} \D{0} */
} superio_i2s_master_user_config_t;

/**
 * @brief Slave configuration structure.
 *
 * This structure is used to provide configuration parameters for the superio_i2s slave at initialization time.
 */
typedef struct {
    superio_driver_type_t driverType; /**< Driver type: interrupts/polling/PDMA. \D{SUPERIO_DRIVER_TYPE_INTERRUPTS} */
    uint8_t bitsWidth;                /**< Number of bits in a word - multiple of 8. \R{7,10} \D{8} */
    uint8_t txPin;                    /**< Superio pin to use for transmit. \R{0,7} \D{0} */
    uint8_t rxPin;                    /**< Superio pin to use for receive. \R{0,7} \D{1} */
    uint8_t sckPin;                   /**< Superio pin to use for serial clock. \R{0,7} \D{2} */
    uint8_t wsPin;                    /**< Superio pin to use for word select. \R{0,7} \D{3} */
    i2s_callback_t callback;          /**< User callback function. Note that this function will be
                                            called from the interrupt service routine, so its
                                            execution time should be as small as possible. It can be
                                            NULL if it is not needed. */
    void *callbackParam;              /**< Parameter for the callback function. */
    uint8_t rxDMAChannel;             /**< Rx PDMA channel number. Only used in PDMA mode. \R{0,32} \D{0} */
    uint8_t txDMAChannel;             /**< Tx PDMA channel number. Only used in PDMA mode. \R{0,32} \D{0} */
} superio_i2s_slave_user_config_t;

/**
 * @brief Master internal context structure.
 *
 * This structure is used by the driver for its internal logic. It must
 * be provided by the application through the SUPERIO_I2S_DRV_MasterInit() function, then
 * it cannot be freed until the driver is de-initialized using SUPERIO_I2S_DRV_MasterDeinit().
 * The application should make no assumptions about the content of this structure.
 */
typedef struct {
    superio_common_state_t superioCommon; /**< Common superio drivers structure. */
    const uint8_t *txData;                /**< Transmit buffer. */
    uint8_t *rxData;                      /**< Receive buffer. */
    uint32_t txRemainingBytes;            /**< Number of remaining bytes to be transmitted. */
    uint32_t rxRemainingBytes;            /**< Number of remaining bytes to be received. */
    uint32_t dummyDmaData;                /**< Dummy location for PDMA transfers. */
    uint8_t rxDMAChannel;                 /**< Rx PDMA channel number. */
    uint8_t txDMAChannel;                 /**< Tx PDMA channel number. */
    uint8_t bitsWidth;                    /**< Number of bits in a word. */
    uint8_t byteWidth;                    /**< Number of bytes in a word. */
    bool master;                          /**< Current instance is in master mode. */
    superio_driver_type_t driverType;     /**< Driver type: interrupts/polling/PDMA. */
    status_t status;                      /**< Current status of the driver. */
    bool driverIdle;                      /**< Idle/busy state of the driver. */
    OS_SemaphoreId_t idleSemaphore;       /**< Semaphore used by blocking functions. */
    bool blocking;                        /**< Specifies if the current transfer is blocking. */
    i2s_callback_t callback;              /**< User callback function. */
    void *callbackParam;                  /**< Parameter for the callback function. */
} superio_i2s_master_state_t;

/**
 * @brief Slave internal context structure.
 *
 * This structure is used by the driver for its internal logic. It must
 * be provided by the application through the SUPERIO_I2S_DRV_SlaveInit() function, then
 * it cannot be freed until the driver is de-initialized using SUPERIO_I2S_DRV_SlaveDeinit().
 * The application should make no assumptions about the content of this structure.
 */
typedef superio_i2s_master_state_t superio_i2s_slave_state_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Initialize the SUPERIO_I2S master mode driver.
 *
 * This function initializes the SUPERIO_I2S driver in master mode.
 *
 * @param instance  SUPERIO peripheral instance number.
 * @param userConfigPtr    Pointer to the SUPERIO_I2S master user configuration structure. The function
 *                         reads configuration data from this structure and initializes the
 *                         driver accordingly. The application may free this structure after
 *                         the function returns.
 * @param master    Pointer to the SUPERIO_I2S master driver context structure. The driver uses
 *                  this memory area for its internal logic. The application must make no
 *                  assumptions about the content of this structure, and must not free this
 *                  memory until the driver is de-initialized using SUPERIO_I2S_DRV_MasterDeinit().
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2S_DRV_MasterInit(uint32_t instance, const superio_i2s_master_user_config_t *userConfigPtr,
                                    superio_i2s_master_state_t *master);

/**
 * @brief De-initialize the SUPERIO_I2S master mode driver.
 *
 * This function de-initializes the SUPERIO_I2S driver in master mode. The driver can't be used
 * again until reinitialized. The context structure is no longer needed by the driver and
 * can be freed after calling this function.
 *
 * @param master    Pointer to the SUPERIO_I2S master driver context structure.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2S_DRV_MasterDeinit(superio_i2s_master_state_t *master);

/**
 * @brief Set the baud rate and bit width for any subsequent I2S communication.
 *
 * This function sets the baud rate (SCK frequency) and bit width for the I2S master.
 * Note that due to module limitation not any baud rate can be achieved. The driver
 * will set a baud rate as close as possible to the requested baud rate, but there may
 * still be substantial differences, for example if requesting a high baud rate while
 * using a low-frequency SuperIO clock. The application should call
 * SUPERIO_I2S_DRV_MasterGetBaudRate() after SUPERIO_I2S_DRV_MasterSetConfig() to check
 * what baud rate was actually set.
 *
 * @param master    Pointer to the SUPERIO_I2S master driver context structure.
 * @param baudRate  The desired baud rate in hertz.
 * @param bitsWidth  Number of bits per word.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2S_DRV_MasterSetConfig(superio_i2s_master_state_t *master, uint32_t baudRate, uint8_t bitsWidth);

/**
 * @brief Get the currently configured baud rate.
 *
 * This function returns the currently configured I2S baud rate.
 *
 * @param master    Pointer to the SUPERIO_I2S master driver context structure.
 * @param baudRate  The current baud rate in hertz.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2S_DRV_MasterGetBaudRate(superio_i2s_master_state_t *master, uint32_t *baudRate);

/**
 * @brief Perform a non-blocking send transaction on the I2S bus.
 *
 * This function starts the transmission of a block of data and returns immediately.
 * The rest of the transmission is handled by the interrupt service routine (if the driver
 * is initialized in interrupt mode) or by the SUPERIO_I2S_DRV_MasterGetStatus function (if
 * the driver is initialized in polling mode).
 * Use SUPERIO_I2S_DRV_MasterGetStatus() to check the progress of the transmission.
 *
 * @param master    Pointer to the SUPERIO_I2S master driver context structure.
 * @param txBuff    Pointer to the data to be transferred.
 * @param txSize    Length in bytes of the data to be transferred.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2S_DRV_MasterSendData(superio_i2s_master_state_t *master, const uint8_t *txBuff, uint32_t txSize);

/**
 * @brief Perform a blocking send transaction on the I2S bus.
 *
 * This function sends a block of data, and only returns when the transmission is complete.
 *
 * @param master    Pointer to the SUPERIO_I2S master driver context structure.
 * @param txBuff    Pointer to the data to be transferred.
 * @param txSize    Length in bytes of the data to be transferred.
 * @param timeout   Timeout for the transfer in milliseconds.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2S_DRV_MasterSendDataBlocking(superio_i2s_master_state_t *master, const uint8_t *txBuff,
                                                uint32_t txSize, uint32_t timeout);

/**
 * @brief Perform a non-blocking receive transaction on the I2S bus.
 *
 * This function starts the reception of a block of data and returns immediately.
 * The rest of the reception is handled by the interrupt service routine (if the driver
 * is initialized in interrupt mode) or by the SUPERIO_I2S_DRV_MasterGetStatus function (if
 * the driver is initialized in polling mode).
 * Use SUPERIO_I2S_DRV_MasterGetStatus() to check the progress of the reception.
 *
 * @param master    Pointer to the SUPERIO_I2S master driver context structure.
 * @param rxBuff    Pointer to the buffer where to store received data.
 * @param rxSize    Length in bytes of the data to be transferred.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2S_DRV_MasterReceiveData(superio_i2s_master_state_t *master, uint8_t *rxBuff, uint32_t rxSize);

/**
 * @brief Perform a blocking receive transaction on the I2S bus.
 *
 * This function receives a block of data and only returns when the reception is complete.
 *
 * @param master    Pointer to the SUPERIO_I2S master driver context structure.
 * @param rxBuff    Pointer to the buffer where to store received data.
 * @param rxSize    Length in bytes of the data to be transferred.
 * @param timeout   Timeout for the transfer in milliseconds.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2S_DRV_MasterReceiveDataBlocking(superio_i2s_master_state_t *master, uint8_t *rxBuff, uint32_t rxSize,
                                                   uint32_t timeout);

/**
 * @brief Aborts a non-blocking I2S master transaction.
 *
 * This function aborts a non-blocking I2S transfer.
 *
 * @param master    Pointer to the SUPERIO_I2S master driver context structure.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2S_DRV_MasterTransferAbort(superio_i2s_master_state_t *master);

/**
 * @brief Get the status of the current non-blocking I2S master transaction.
 *
 * This function returns the current status of a non-blocking I2S master transaction.
 * A return code of STATUS_BUSY means the transfer is still in progress.
 * Otherwise the function returns a status reflecting the outcome of the last transfer.
 * When the driver is initialized in polling mode this function also advances the transfer
 * by checking and handling the transmit and receive events, so it must be called
 * frequently to avoid overflows or underflows.
 *
 * @param master    Pointer to the SUPERIO_I2S master driver context structure.
 * @param bytesRemaining  The remaining number of bytes to be transferred.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2S_DRV_MasterGetStatus(superio_i2s_master_state_t *master, uint32_t *bytesRemaining);

/**
 * @brief Provide a buffer for receiving data.
 *
 * This function can be used to provide a new buffer for receiving data to the driver.
 * It can be called from the user callback when event STATUS_I2S_RX_OVERRUN is reported.
 * This way the reception will continue without interruption.
 *
 * @param master    Pointer to the SUPERIO_I2S master driver context structure.
 * @param rxBuff    Pointer to the buffer where to store received data.
 * @param rxSize    Length in bytes of the data to be transferred.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2S_DRV_MasterSetRxBuffer(superio_i2s_master_state_t *master, uint8_t *rxBuff, uint32_t rxSize);

/**
 * @brief Provide a buffer for transmitting data.
 *
 * This function can be used to provide a new buffer for transmitting data to the driver.
 * It can be called from the user callback when event STATUS_I2S_TX_UNDERRUN is reported.
 * This way the transmission will continue without interruption.
 *
 * @param master    Pointer to the SUPERIO_I2S master driver context structure.
 * @param txBuff    Pointer to the buffer containing transmit data.
 * @param txSize    Length in bytes of the data to be transferred.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2S_DRV_MasterSetTxBuffer(superio_i2s_master_state_t *master, const uint8_t *txBuff, uint32_t txSize);

/**
 * @brief Initialize the SUPERIO_I2S slave mode driver.
 *
 * This function initializes the SUPERIO_I2S driver in slave mode.
 *
 * @param instance  SUPERIO peripheral instance number.
 * @param userConfigPtr    Pointer to the SUPERIO_I2S slave user configuration structure. The function
 *                         reads configuration data from this structure and initializes the
 *                         driver accordingly. The application may free this structure after
 *                         the function returns.
 * @param slave     Pointer to the SUPERIO_I2S slave driver context structure. The driver uses
 *                  this memory area for its internal logic. The application must make no
 *                  assumptions about the content of this structure, and must not free this
 *                  memory until the driver is de-initialized using SUPERIO_I2S_DRV_SlaveDeinit().
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2S_DRV_SlaveInit(uint32_t instance, const superio_i2s_slave_user_config_t *userConfigPtr,
                                   superio_i2s_slave_state_t *slave);

/**
 * @brief De-initialize the SUPERIO_I2S slave mode driver.
 *
 * This function de-initializes the SUPERIO_I2S driver in slave mode. The driver can't be used
 * again until reinitialized. The context structure is no longer needed by the driver and
 * can be freed after calling this function.
 *
 * @param slave     Pointer to the SUPERIO_I2S slave driver context structure.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2S_DRV_SlaveDeinit(superio_i2s_slave_state_t *slave);

/**
 * @brief Set the bit width for any subsequent I2S communication.
 *
 * This function sets the bit width for the I2S slave.
 *
 * @param slave     Pointer to the SUPERIO_I2S slave driver context structure.
 * @param bitsWidth Number of bits per word.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2S_DRV_SlaveSetConfig(superio_i2s_slave_state_t *slave, uint8_t bitsWidth);

/**
 * @brief Perform a non-blocking send transaction on the I2S bus.
 *
 * This function starts the transmission of a block of data and returns immediately.
 * The rest of the transmission is handled by the interrupt service routine (if the driver
 * is initialized in interrupt mode) or by the SUPERIO_I2S_DRV_SlaveGetStatus function (if
 * the driver is initialized in polling mode).
 * Use SUPERIO_I2S_DRV_SlaveGetStatus() to check the progress of the transmission.
 *
 * @param slave     Pointer to the SUPERIO_I2S slave driver context structure.
 * @param txBuff    Pointer to the data to be transferred.
 * @param txSize    Length in bytes of the data to be transferred.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2S_DRV_SlaveSendData(superio_i2s_slave_state_t *slave, const uint8_t *txBuff, uint32_t txSize);

/**
 * @brief Perform a blocking send transaction on the I2S bus.
 *
 * This function sends a block of data, and
 * only returns when the transmission is complete.
 *
 * @param slave     Pointer to the SUPERIO_I2S slave driver context structure.
 * @param txBuff    Pointer to the data to be transferred.
 * @param txSize    Length in bytes of the data to be transferred.
 * @param timeout   Timeout for the transfer in milliseconds.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2S_DRV_SlaveSendDataBlocking(superio_i2s_slave_state_t *slave, const uint8_t *txBuff, uint32_t txSize,
                                               uint32_t timeout);

/**
 * @brief Perform a non-blocking receive transaction on the I2S bus.
 *
 * This function starts the reception of a block of data and returns immediately.
 * The rest of the reception is handled by the interrupt service routine (if the driver
 * is initialized in interrupt mode) or by the SUPERIO_I2S_DRV_SlaveGetStatus function (if
 * the driver is initialized in polling mode).
 * Use SUPERIO_I2S_DRV_SlaveGetStatus() to check the progress of the reception.
 *
 * @param slave     Pointer to the SUPERIO_I2S slave driver context structure.
 * @param rxBuff    Pointer to the buffer where to store received data.
 * @param rxSize    Length in bytes of the data to be transferred.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2S_DRV_SlaveReceiveData(superio_i2s_slave_state_t *slave, uint8_t *rxBuff, uint32_t rxSize);

/**
 * @brief Perform a blocking receive transaction on the I2S bus.
 *
 * This function receives a block of data and only returns when the reception is complete.
 *
 * @param slave     Pointer to the SUPERIO_I2S slave driver context structure.
 * @param rxBuff    Pointer to the buffer where to store received data.
 * @param rxSize    Length in bytes of the data to be transferred.
 * @param timeout   Timeout for the transfer in milliseconds.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2S_DRV_SlaveReceiveDataBlocking(superio_i2s_slave_state_t *slave, uint8_t *rxBuff, uint32_t rxSize,
                                                  uint32_t timeout);

/**
 * @brief Aborts a non-blocking I2S slave transaction.
 *
 * This function aborts a non-blocking I2S transfer.
 *
 * @param slave     Pointer to the SUPERIO_I2S slave driver context structure.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2S_DRV_SlaveTransferAbort(superio_i2s_slave_state_t *slave);

/**
 * @brief Get the status of the current non-blocking I2S slave transaction.
 *
 * This function returns the current status of a non-blocking I2S slave transaction.
 * A return code of STATUS_BUSY means the transfer is still in progress.
 * Otherwise the function returns a status reflecting the outcome of the last transfer.
 * When the driver is initialized in polling mode this function also advances the transfer
 * by checking and handling the transmit and receive events, so it must be called
 * frequently to avoid overflows or underflows.
 *
 * @param slave    Pointer to the SUPERIO_I2S slave driver context structure.
 * @param bytesRemaining  The remaining number of bytes to be transferred.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2S_DRV_SlaveGetStatus(superio_i2s_slave_state_t *slave, uint32_t *bytesRemaining);

/**
 * @brief Provide a buffer for receiving data.
 *
 * This function can be used to provide a driver with a new buffer for receiving data.
 * It can be called from the user callback when event STATUS_I2S_RX_OVERRUN is reported.
 * This way the reception will continue without interruption.
 *
 * @param slave     Pointer to the SUPERIO_I2S slave driver context structure.
 * @param rxBuff    Pointer to the buffer where to store received data.
 * @param rxSize    Length in bytes of the data to be transferred.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2S_DRV_SlaveSetRxBuffer(superio_i2s_slave_state_t *slave, uint8_t *rxBuff, uint32_t rxSize);

/**
 * @brief Provide a buffer for transmitting data.
 *
 * This function can be used to provide a driver with a new buffer for transmitting data.
 * It can be called from the user callback when event STATUS_I2S_TX_UNDERRUN is reported.
 * This way the transmission will continue without interruption.
 *
 * @param slave     Pointer to the SUPERIO_I2S slave driver context structure.
 * @param txBuff    Pointer to the buffer containing transmit data.
 * @param txSize    Length in bytes of the data to be transferred.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2S_DRV_SlaveSetTxBuffer(superio_i2s_slave_state_t *slave, const uint8_t *txBuff, uint32_t txSize);

/**
 * @brief Returns default configuration structure for SUPERIO_I2S master.
 *
 * @param userConfigPtr    Pointer to the SUPERIO_I2S user configuration structure.
 */
void SUPERIO_I2S_DRV_MasterGetDefaultConfig(superio_i2s_master_user_config_t *userConfigPtr);

/**
 * @brief Returns default configuration structure for SUPERIO_I2S slave.
 *
 * @param userConfigPtr    Pointer to the SUPERIO_I2S user configuration structure.
 */
void SUPERIO_I2S_DRV_SlaveGetDefaultConfig(superio_i2s_slave_user_config_t *userConfigPtr);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */ /* end of group SUPERIO_I2S_Driver */

/**
 * @}
 */ /* end of group Peripheral_Driver_layer */

#endif /* __SUPERIO_I2S_DRIVER_H__ */
