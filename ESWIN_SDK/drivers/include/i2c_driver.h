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
 * @file i2c_driver.h
 * @brief I2C driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __I2C_DRIVER_H__
#define __I2C_DRIVER_H__

#include "callbacks.h"
#include "emps_platform.h"
#include "osal.h"
#include "pdma_driver.h"
#include "status.h"
#include <stdbool.h>
#include <stddef.h>

/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup I2C_Driver I2C Driver
 * @{
 */

/** @brief Size of the master command queue. Worst case: 5 commands in High-Speed receive with 10-bit address:
   START + master code, REP START + addr_1 + tx, addr_2, REP START + addr_1 + rx, receive command. */
#define I2C_MASTER_CMD_QUEUE_SIZE 5U

/**
 * @brief I2C operating modes.
 */
typedef enum {
    I2C_STANDARD_MODE = 0x0U, /**< Standard-mode (Sm), bidirectional data transfers up to 100 Kbit/s. */
    I2C_FAST_MODE     = 0x1U, /**< Fast-mode (Fm), bidirectional data transfers up to 400 Kbit/s. */
    I2C_FASTPLUS_MODE = 0x2U, /**< Fast-mode Plus (Fm+), bidirectional data transfers up to 1 Mbit/s. */
#if (I2C_HAS_ULTRA_FAST_MODE)
    I2C_ULTRAFAST_MODE = 0x4U, /**< Ultra Fast Mode (UFm), unidirectional data transfers up to 5 Mbit/s. */
#endif
} i2c_mode_t;

/**
 * @brief Type of I2C transfer (based on interrupts or PDMA).
 */
typedef enum {
    I2C_USING_DMA        = 0, /**< The driver will use PDMA to perform I2C transfer. */
    I2C_USING_INTERRUPTS = 1, /**< The driver will use interrupts to perform I2C transfer. */
} i2c_transfer_type_t;

/**
 * @brief Master configuration structure.
 *
 * This structure is used to provide configuration parameters for the I2C master at initialization time.
 */
typedef struct {
    uint16_t slaveAddress;                /**< Slave address, 7-bit or 10-bit. \D{0x50U} */
    bool is10bitAddr;                     /**< Selects 7-bit or 10-bit slave address. \D{false} */
    i2c_mode_t operatingMode;             /**< I2C Operating mode. \D{I2C_STANDARD_MODE} */
    uint32_t baudRate;                    /**< The baud rate in hertz to use with current slave device. \D{100000UL} */
    i2c_transfer_type_t transferType;     /**< Type of I2C transfer. \D{I2C_USING_INTERRUPTS} */
    uint8_t dmaChannel;                   /**< Channel number for PDMA channel. If PDMA mode isn't used this field will be ignored. \D{0U} */
    i2c_master_callback_t masterCallback; /**< Master callback function. Note that this function will be called from the interrupt service routine at the
                                               end of a transfer, so its execution time should be as small as possible. It can be NULL if you want to check
                                               manually the status of the transfer. \D{NULL} */
    void *callbackParam;                  /**< Parameter for the master callback function. \D{NULL} */
} i2c_master_user_config_t;

/**
 * @brief Slave configuration structure.
 *
 * This structure is used to provide configuration parameters for the I2C slave at initialization time.
 */
typedef struct {
    uint16_t slaveAddress;              /**< Slave address, 7-bit or 10-bit. \D{0x50U} */
    bool is10bitAddr;                   /**< Selects 7-bit or 10-bit slave address. \D{false} */
    i2c_mode_t operatingMode;           /**< I2C Operating mode. \D{I2C_STANDARD_MODE} */
    bool slaveListening;                /**< Slave mode (always listening or on demand only). \D{true} */
    i2c_transfer_type_t transferType;   /**< Type of I2C transfer. \D{I2C_USING_INTERRUPTS} */
    uint8_t dmaChannel;                 /**< Channel number for PDMA rx channel. If PDMA mode isn't used this field will be ignored. \D{0U} */
    i2c_slave_callback_t slaveCallback; /**< Slave callback function. Note that this function will be
                                            called from the interrupt service routine, so its
                                            execution time should be as small as possible. It can be
                                            NULL if the slave is not in listening mode
                                            (slaveListening = false). \D{NULL} */
    void *callbackParam;                /**< Parameter for the slave callback function. \D{NULL} */
} i2c_slave_user_config_t;

/**
 * @brief Baud rate structure.
 *
 * This type is used for setting or getting the baud rate.
 */
typedef uint32_t i2c_baud_rate_params_t;

/**
 * @brief I2C master commands.
 */
typedef enum {
    I2C_MASTER_COMMAND_TRANSMIT        = 0U, /**< Transmit DATA[7:0]. */
    I2C_MASTER_COMMAND_RECEIVE         = 1U, /**< Receive (DATA[7:0] + 1) bytes. */
    I2C_MASTER_COMMAND_STOP            = 2U, /**< Generate STOP condition. */
    I2C_MASTER_COMMAND_RECEIVE_DISCARD = 3U, /**< Receive and discard (DATA[7:0] + 1) bytes. */
    I2C_MASTER_COMMAND_START           = 4U, /**< Generate START and transmit address in DATA[7:0]. */
    I2C_MASTER_COMMAND_START_NACK      = 5U, /**< Generate START and transmit address in DATA[7:0], expect a NACK to be returned. */
    I2C_MASTER_COMMAND_START_HS        = 6U, /**< Generate START and transmit address in DATA[7:0] in high speed mode. */
    I2C_MASTER_COMMAND_START_NACK_HS   = 7U, /**< Generate START and transmit address in DATA[7:0] in high speed mode, expect a NACK to be returned. */
} i2c_master_command_t;

/**
 * @brief Master software command queue for I2C MTDR register.
 */
typedef struct {
    i2c_master_command_t cmd[I2C_MASTER_CMD_QUEUE_SIZE]; /**< I2C master command queue. */
    uint8_t data[I2C_MASTER_CMD_QUEUE_SIZE];             /**< I2C transmit data queue corresponding to the master command queue. */
    uint8_t writeIdx;                                    /**< I2C master command queue write Idex. */
    uint8_t readIdx;                                     /**< I2C master command queue read Idex. */
} i2c_master_cmd_queue_t;

/**
 * @brief Master internal context structure.
 *
 * This structure is used by the master-mode driver for its internal logic. It must
 * be provided by the application through the I2C_DRV_MasterInit() function, then
 * it cannot be freed until the driver is de-initialized using I2C_DRV_MasterDeinit().
 * The application should make no assumptions about the content of this structure.
 */
typedef struct {
    i2c_master_cmd_queue_t cmdQueue;      /**< Software queue for commands, when I2C FIFO is not big enough. */
    uint8_t *rxBuff;                      /**< Pointer to receive data buffer. */
    uint32_t rxSize;                      /**< Size of receive data buffer. */
    const uint8_t *txBuff;                /**< Pointer to transmit data buffer. */
    uint32_t txSize;                      /**< Size of transmit data buffer. */
    volatile status_t status;             /**< Status of last driver operation. */
    i2c_mode_t operatingMode;             /**< I2C Operating mode. */
    uint16_t slaveAddress;                /**< Slave address. */
    volatile bool i2cIdle;                /**< Idle/busy state of the driver. */
    bool sendStop;                        /**< Specifies if STOP condition must be generated after current transfer. */
    bool is10bitAddr;                     /**< Selects 7-bit or 10-bit slave address. */
    OS_SemaphoreId_t idleSemaphore;       /**< Semaphore used by blocking functions. */
    bool blocking;                        /**< Specifies if the current transfer is blocking. */
    i2c_transfer_type_t transferType;     /**< Type of I2C transfer. */
    uint8_t dmaChannel;                   /**< Channel number for PDMA rx channel. */
    i2c_master_callback_t masterCallback; /**< Master callback function. */
    void *callbackParam;                  /**< Parameter for the master callback function. */
    bool abortedTransfer;                 /**< Specifies if master has aborted transfer. */
    uint32_t baudrate;                    /**< Baud rate in Hz.*/
} i2c_master_state_t;

/**
 * @brief Slave internal context structure.
 *
 * This structure is used by the slave-mode driver for its internal logic. It must
 * be provided by the application through the I2C_DRV_SlaveInit() function, then
 * it cannot be freed until the driver is de-initialized using I2C_DRV_SlaveDeinit().
 * The application should make no assumptions about the content of this structure.
 */
typedef struct {
    status_t status;                    /**< The I2C slave status. */
    volatile bool isTransferInProgress; /**< Slave is busy because of an ongoing transfer. */
    uint32_t txSize;                    /**< Size of the TX buffer. */
    uint32_t rxSize;                    /**< Size of the RX buffer. */
    const uint8_t *txBuff;              /**< Pointer to Tx Buffer. */
    uint8_t *rxBuff;                    /**< Pointer to Rx Buffer. */
    i2c_mode_t operatingMode;           /**< I2C Operating mode. */
    bool slaveListening;                /**< Slave mode (always listening or on demand only). */
    bool is10bitAddress;                /**< Specifies if 10-bit or 7-bit address. */
    uint8_t repeatedStarts;             /**< Specifies the number of repeated starts. */
    bool txUnderrunWarning;             /**< Possible slave tx underrun. */
    OS_SemaphoreId_t idleSemaphore;     /**< Semaphore used by blocking functions. */
    bool blocking;                      /**< Specifies if the current transfer is blocking. */
    i2c_transfer_type_t transferType;   /**< Type of I2C transfer. */
    uint8_t dmaChannel;                 /**< Channel number for PDMA channel. */
    i2c_slave_callback_t slaveCallback; /**< Slave callback function. */
    void *callbackParam;                /**< Parameter for the slave callback function. */
} i2c_slave_state_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Initialize the I2C master mode driver.
 *
 * @param  instance The I2C instance number(0-1).
 * @param  userConfigPtr   Pointer to the I2C master user configuration structure. The function
 *                         reads configuration data from this structure and initializes the
 *                         driver accordingly. The application may free this structure after
 *                         the function returns.
 * @param  master Master internal context structure.
 * @return status_t Error or success status returned by API.
 */
status_t I2C_DRV_MasterInit(uint32_t instance, const i2c_master_user_config_t *userConfigPtr,
                            i2c_master_state_t *master);

/**
 * @brief De-initialize the I2C master mode driver.
 *
 * This function de-initializes the I2C driver in master mode. The driver can't be used
 * again until reinitialized. The context structure is no longer needed by the driver and
 * can be freed after calling this function.
 *
 * @param  instance The I2C instance number(0-1).
 * @return status_t Error or success status returned by API.
 */
status_t I2C_DRV_MasterDeinit(uint32_t instance);

/**
 * @brief Get the currently configured baud rate.
 *
 * @param  instance The I2C instance number(0-1).
 * @param  baudRate structure that contains the current baud rate in hertz
 *                  and the baud rate in hertz for High-speed mode (unused
 *                  in other modes, can be NULL).
 */
void I2C_DRV_MasterGetBaudRate(uint32_t instance, i2c_baud_rate_params_t *baudRate);

/**
 * @brief  Set the baud rate for any subsequent I2C communication.
 *
 * This function sets the baud rate (SCL frequency) for the I2C master. It can also
 * change the operating mode. If the operating mode is High-Speed, a second baud rate
 * must be provided for high-speed communication.
 * Note that due to module limitation not any baud rate can be achieved. The driver
 * will set a baud rate as close as possible to the requested baud rate, but there may
 * still be substantial differences, for example if requesting a high baud rate while
 * using a low-frequency protocol clock for the I2C module. The application should
 * call I2C_DRV_MasterGetBaudRate() after I2C_DRV_MasterSetBaudRate() to check
 * what baud rate was actually set.
 *
 * @param  instance The I2C instance number(0-1).
 * @param  operatingMode I2C operating mode.
 * @param  baudRate structure that contains the baud rate in hertz to use by current slave device
 *                  and also the baud rate in hertz for High-speed mode (unused in other modes).
 * @return status_t Error or success status returned by API.
 */
status_t I2C_DRV_MasterSetBaudRate(uint32_t instance, const i2c_mode_t operatingMode,
                                   const i2c_baud_rate_params_t baudRate);

/**
 * @brief  Configures the I2C Host Request Enable status.
 *
 * @param  instance The I2C instance number(0-1).
 * @param  configuration true: Host Request input is enabled.
 *                       false: Host Request input is disabled.
 */
void I2C_DRV_SetMasterHostRequestEnable(uint32_t instance, bool configuration);

/**
 * @brief  Configures the I2C Host Request Polarity status.
 *
 * @param  instance The I2C instance number(0-1).
 * @param  configuration true: Host Request Polarity is Active high.
 *                       false: Host Request Polarity is Active low.
 */
void I2C_DRV_SetMasterHostHostRequestPolarity(uint32_t instance, bool configuration);

/**
 * @brief  Configures the source of the Host Request Select input.
 *
 * @param  instance The I2C instance number(0-1).
 * @param  configuration true: Host Request Select input is input trigger.
 *                       false: Host Request Select is pin HREQ.
 */
void I2C_DRV_SetMasterHostRequestSelect(uint32_t instance, bool configuration);

/**
 * @brief  Set the slave address for any subsequent I2C communication.
 *
 * This function sets the slave address which will be used for any future
 * transfer initiated by the I2C master.
 *
 * @param  instance The I2C instance number(0-1).
 * @param  address Slave address, 7-bit or 10-bit.
 * @param  is10bitAddr Specifies if provided address is 10-bit.
 */
void I2C_DRV_MasterSetSlaveAddr(uint32_t instance, const uint16_t address, const bool is10bitAddr);

/**
 * @brief Perform a non-blocking send transaction on the I2C bus.
 *
 * This function starts the transmission of a block of data to the currently
 * configured slave address and returns immediately.
 * The rest of the transmission is handled by the interrupt service routine.
 * Use I2C_DRV_MasterGetSendStatus() to check the progress of the transmission.
 *
 * @param  instance The I2C instance number(0-1).
 * @param  txBuff Pointer to the data to be transferred.
 * @param  txSize Length in bytes of the data to be transferred.
 * @param  sendStop Specifies whether or not to generate stop condition after the reception.
 * @return status_t Error or success status returned by API.
 */
status_t I2C_DRV_MasterSendData(uint32_t instance, const uint8_t *txBuff, uint32_t txSize, bool sendStop);

/**
 * @brief  Perform a blocking send transaction on the I2C bus.
 *
 * This function sends a block of data to the currently configured slave address, and
 * only returns when the transmission is complete.
 *
 * @param  instance The I2C instance number(0-1).
 * @param  txBuff Pointer to the data to be transferred.
 * @param  txSize Length in bytes of the data to be transferred.
 * @param  sendStop Specifies whether or not to generate stop condition after the reception.
 * @param  timeout Timeout for the transfer in milliseconds.
 * @return status_t Error or success status returned by API.
 */
status_t I2C_DRV_MasterSendDataBlocking(uint32_t instance, const uint8_t *txBuff, uint32_t txSize, bool sendStop,
                                        uint32_t timeout);

/**
 * @brief  Abort a non-blocking I2C Master transmission or reception.
 *
 * @param  instance The I2C instance number(0-1).
 * @return status_t Error or success status returned by API.
 */
status_t I2C_DRV_MasterAbortTransferData(uint32_t instance);

/**
 * @brief  Perform a non-blocking receive transaction on the I2C bus.
 *
 * This function starts the reception of a block of data from the currently
 * configured slave address and returns immediately.
 * The rest of the reception is handled by the interrupt service routine.
 * Use I2C_DRV_MasterGetReceiveStatus() to check the progress of the reception.
 *
 * @param  instance The I2C instance number(0-1).
 * @param  rxBuff Pointer to the buffer where to store received data.
 * @param  rxSize Length in bytes of the data to be transferred.
 * @param  sendStop Specifies whether or not to generate stop condition after the reception.
 * @return status_t Error or success status returned by API.
 */
status_t I2C_DRV_MasterReceiveData(uint32_t instance, uint8_t *rxBuff, uint32_t rxSize, bool sendStop);

/**
 * @brief  Perform a blocking receive transaction on the I2C bus.
 *
 * This function receives a block of data from the currently configured slave address,
 * and only returns when the transmission is complete.
 *
 * @param  instance The I2C instance number(0-1).
 * @param  rxBuff Pointer to the buffer where to store received data.
 * @param  rxSize Length in bytes of the data to be transferred.
 * @param  sendStop Specifies whether or not to generate stop condition after the reception.
 * @param  timeout Timeout for the transfer in milliseconds.
 * @return status_t Error or success status returned by API.
 */
status_t I2C_DRV_MasterReceiveDataBlocking(uint32_t instance, uint8_t *rxBuff, uint32_t rxSize, bool sendStop,
                                           uint32_t timeout);

/**
 * @brief  Return the current status of the I2C master transfer.
 *
 * This function can be called during a non-blocking transmission to check the
 * status of the transfer.
 *
 * @param  instance The I2C instance number(0-1).
 * @param  bytesRemaining The number of remaining bytes in the active I2C transfer.
 * @return status_t Error or success status returned by API.
 */
status_t I2C_DRV_MasterGetTransferStatus(uint32_t instance, uint32_t *bytesRemaining);

/**
 * @brief  Handle master operation when I2C interrupt occurs.
 *
 * This is the interrupt service routine for the I2C master mode driver. It
 * handles the rest of the transfer started by one of the send/receive functions.
 *
 * @param  data The I2C instance number(0-1), uint32_t instance = (uint32_t)data.
 */
void I2C_DRV_MasterIRQHandler(void *data);

/**
 * @brief  Initialize the I2C slave mode driver.
 *
 * @param  instance The I2C instance number(0-1).
 * @param  userConfigPtr  Pointer to the I2C slave user configuration structure. The function
 *                        reads configuration data from this structure and initializes the
 *                        driver accordingly. The application may free this structure after
 *                        the function returns.
 * @param  slave  Pointer to the I2C slave driver context structure. The driver uses
 *                this memory area for its internal logic. The application must make no
 *                assumptions about the content of this structure, and must not free this
 *                memory until the driver is de-initialized using I2C_DRV_SlaveDeinit().
 * @return status_t Error or success status returned by API.
 */
status_t I2C_DRV_SlaveInit(uint32_t instance, const i2c_slave_user_config_t *userConfigPtr, i2c_slave_state_t *slave);

/**
 * @brief  De-initialize the I2C slave mode driver.
 *
 * This function de-initializes the I2C driver in slave mode. The driver can't be used
 * again until reinitialized. The context structure is no longer needed by the driver and
 * can be freed after calling this function.
 *
 * @param  instance The I2C instance number(0-1).
 * @return status_t Error or success status returned by API.
 */
status_t I2C_DRV_SlaveDeinit(uint32_t instance);

/**
 * @brief  Provide a buffer for transmitting data.
 *
 * This function provides a buffer from which the I2C slave-mode driver can
 * transmit data. It can be called for example from the user callback provided at
 * initialization time, when the driver reports events I2C_SLAVE_EVENT_TX_REQ or
 * I2C_SLAVE_EVENT_TX_EMPTY.
 *
 * @param  instance The I2C instance number(0-1).
 * @param  txBuff Pointer to the data to be transferred.
 * @param  txSize Length in bytes of the data to be transferred.
 * @return status_t Error or success status returned by API.
 */
status_t I2C_DRV_SlaveSetTxBuffer(uint32_t instance, const uint8_t *txBuff, uint32_t txSize);

/**
 * @brief  Provide a buffer for receiving data.
 *
 * This function provides a buffer in which the I2C slave-mode driver can
 * store received data. It can be called for example from the user callback provided at
 * initialization time, when the driver reports events I2C_SLAVE_EVENT_RX_REQ or
 * I2C_SLAVE_EVENT_RX_FULL.
 *
 * @param  instance The I2C instance number(0-1).
 * @param  rxBuff Pointer to the buffer where to store received data.
 * @param  rxSize Length in bytes of the data to be transferred.
 * @return status_t Error or success status returned by API.
 */
status_t I2C_DRV_SlaveSetRxBuffer(uint32_t instance, uint8_t *rxBuff, uint32_t rxSize);

/**
 * @brief  Perform a non-blocking send transaction on the I2C bus.
 *
 * Performs a non-blocking send transaction on the I2C bus when the slave is
 * not in listening mode (initialized with slaveListening = false). It starts
 * the transmission and returns immediately. The rest of the transmission is
 * handled by the interrupt service routine.
 * Use I2C_DRV_SlaveGetTransmitStatus() to check the progress of the transmission.
 *
 * @param  instance The I2C instance number(0-1).
 * @param  txBuff Pointer to the data to be transferred.
 * @param  txSize Length in bytes of the data to be transferred.
 * @return status_t Error or success status returned by API.
 */
status_t I2C_DRV_SlaveSendData(uint32_t instance, const uint8_t *txBuff, uint32_t txSize);

/**
 * @brief  Perform a blocking send transaction on the I2C bus.
 *
 * Performs a blocking send transaction on the I2C bus when the slave is
 * not in listening mode (initialized with slaveListening = false). It sets
 * up the transmission and then waits for the transfer to complete before
 * returning.
 *
 * @param  instance The I2C instance number(0-1).
 * @param  txBuff Pointer to the data to be transferred.
 * @param  txSize Length in bytes of the data to be transferred.
 * @param  timeout Timeout for the transfer in milliseconds.
 * @return status_t Error or success status returned by API.
 */
status_t I2C_DRV_SlaveSendDataBlocking(uint32_t instance, const uint8_t *txBuff, uint32_t txSize, uint32_t timeout);

/**
 * @brief  Perform a non-blocking receive transaction on the I2C bus.
 *
 * Performs a non-blocking receive transaction on the I2C bus when the slave is
 * not in listening mode (initialized with slaveListening = false). It starts
 * the reception and returns immediately. The rest of the reception is
 * handled by the interrupt service routine.
 * Use I2C_DRV_SlaveGetReceiveStatus() to check the progress of the reception.
 *
 * @param  instance The I2C instance number(0-1).
 * @param  rxBuff Pointer to the buffer where to store received data.
 * @param  rxSize Length in bytes of the data to be transferred.
 * @return status_t Error or success status returned by API.
 */
status_t I2C_DRV_SlaveReceiveData(uint32_t instance, uint8_t *rxBuff, uint32_t rxSize);

/**
 * @brief  Perform a blocking receive transaction on the I2C bus.
 *
 * Performs a blocking receive transaction on the I2C bus when the slave is
 * not in listening mode (initialized with slaveListening = false). It sets
 * up the reception and then waits for the transfer to complete before returning.
 *
 * @param  instance The I2C instance number(0-1).
 * @param  rxBuff Pointer to the buffer where to store received data.
 * @param  rxSize Length in bytes of the data to be transferred.
 * @param  timeout Timeout for the transfer in milliseconds.
 * @return status_t Error or success status returned by API.
 */
status_t I2C_DRV_SlaveReceiveDataBlocking(uint32_t instance, uint8_t *rxBuff, uint32_t rxSize, uint32_t timeout);

/**
 * @brief  Return the current status of the I2C slave transfer.
 *
 * This function can be called during a non-blocking transmission to check the status of the transfer.
 *
 * @param  instance The I2C instance number(0-1).
 * @param  bytesRemaining The number of remaining bytes in the active I2C transfer.
 * @return status_t Error or success status returned by API.
 */
status_t I2C_DRV_SlaveGetTransferStatus(uint32_t instance, uint32_t *bytesRemaining);

/**
 * @brief  Abort a non-blocking I2C Master transmission or reception.
 *
 * @param  instance The I2C instance number(0-1).
 * @return status_t Error or success status returned by API.
 */
status_t I2C_DRV_SlaveAbortTransferData(uint32_t instance);

/**
 * @brief  Handle slave operation when I2C interrupt occurs.
 *
 * @param  data The I2C instance number(0-1), uint32_t instance = (uint32_t)data.
 */
void I2C_DRV_SlaveIRQHandler(void *data);

/**
 * @brief  Gets the default configuration structure for master.
 *
 * @param  config Pointer to configuration structure.
 */
void I2C_DRV_MasterGetDefaultConfig(i2c_master_user_config_t *config);

/**
 * @brief  Gets the default configuration structure for slave.
 *
 * @param  config Pointer to configuration structure.
 */
void I2C_DRV_SlaveGetDefaultConfig(i2c_slave_user_config_t *config);

/**
 * @brief  Set bus idle timeout for I2C.
 *
 * This function sets time out for bus idle for Master.If both SCL and SDA are high for longer than
 * Timeout cycles, then the I2C bus is assumed to be idle and the master can generate a START condition.
 *
 * @param  instance The I2C instance number(0-1).
 * @param  timeout Timeout for the transfer in milliseconds.
 */
void I2C_DRV_SetMasterBusIdleTimeout(uint32_t instance, uint16_t timeout);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
/* end of group I2C_Driver */

/**
 * @}
 */
/* end of group Peripheral_Driver_layer */

#endif /* __I2C_DRIVER_H__ */
