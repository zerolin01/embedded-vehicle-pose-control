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
 * @file superio_i2c_driver.h
 * @brief SUPERIO i2c driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __SUPERIO_I2C_DRIVER_H__
#define __SUPERIO_I2C_DRIVER_H__

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
 * @addtogroup SUPERIO_I2C_Driver SUPERIO_I2C Driver
 * @{
 */

/**
 * @brief Length of the PDMA scatter-gather chain for Tx. Two blocks are needed, but as the fist one is
   loaded directly in the TCD, only one block needs to be in RAM.
 */
#define SUPERIO_I2C_DMA_TX_CHAIN_LENGTH (1U)

/**
 * @brief Length of the PDMA scatter-gather chain for Rx. Six blocks are needed, but as the fist one is
   loaded directly in the TCD, only five blocks needs to be in RAM.
 */
#define SUPERIO_I2C_DMA_RX_CHAIN_LENGTH (5U)

/**
 * @brief Total size of the PDMA scatter-gather buffer. Add 1 because STCD_SIZE already compensates for the
   first stcd not being loaded in RAM.
 */
#define SUPERIO_I2C_DMA_BUF_SIZE (STCD_SIZE(SUPERIO_I2C_DMA_TX_CHAIN_LENGTH + SUPERIO_I2C_DMA_RX_CHAIN_LENGTH + 1U))

/**
 * @brief Master configuration structure.
 *
 * This structure is used to provide configuration parameters for the superio_i2c master at initialization time.
 */
typedef struct {
    uint16_t slaveAddress;            /**< Slave address, 7-bit. \R{0x0,0x7f} \D{0x0} */
    superio_driver_type_t driverType; /**< Driver type: interrupts/polling/PDMA. */
    uint32_t baudRate;                /**< Baud rate in hertz. \R{100,100000UL} \D{100UL} */
    uint8_t sdaPin;                   /**< SuperIO pin to use as I2C SDA pin. \R{0,7} \D{0} */
    uint8_t sclPin;                   /**< SuperIO pin to use as I2C SCL pin. \R{0,7} \D{1} */
    i2c_master_callback_t callback;   /**< User callback function. Note that this function will be
                                      called from the interrupt service routine, so its
                                      execution time should be as small as possible. It can be
                                      NULL if it is not needed. */
    void *callbackParam;              /**< Parameter for the callback function. */
    uint8_t rxDMAChannel;             /**< Rx PDMA channel number. Only used in PDMA mode. \R{0,32} \D{0} */
    uint8_t txDMAChannel;             /**< Tx PDMA channel number. Only used in PDMA mode. \R{0,32} \D{0} */
} superio_i2c_master_user_config_t;

/**
 * @brief Master internal context structure
 *
 * This structure is used by the driver for its internal logic. It must
 * be provided by the application through the SUPERIO_I2C_DRV_MasterInit() function, then
 * it cannot be freed until the driver is de-initialized using SUPERIO_I2C_DRV_MasterDeinit().
 * The application should make no assumptions about the content of this structure.
 */
typedef struct {
    superio_common_state_t superioCommon; /**< Common superio drivers structure. */
    const uint8_t *txData;                /**< Transmit buffer. */
    uint8_t *rxData;                      /**< Receive buffer. */
    uint32_t txRemainingBytes;            /**< Number of remaining bytes to be transmitted. */
    uint32_t rxRemainingBytes;            /**< Number of remaining bytes to be received. */
    uint8_t rxDMAChannel;                 /**< Rx PDMA channel number. */
    uint8_t txDMAChannel;                 /**< Tx PDMA channel number. */
    i2c_master_callback_t callback;       /**< User callback function. */
    void *callbackParam;                  /**< Parameter for the callback function. */
    uint16_t slaveAddress;                /**< Slave address. */
    uint16_t eventCount;                  /**< Number of timer events, needed for long transfers. */
    superio_driver_type_t driverType;     /**< Driver type: interrupts/polling/PDMA. */
    status_t status;                      /**< Current status of the driver. */
    bool receive;                         /**< Transfer direction, true = receive, false = transmit. */
    bool addrReceived;                    /**< Indicated start of receive (after address transmission). */
    bool driverIdle;                      /**< Idle/busy state of the driver. */
    OS_SemaphoreId_t idleSemaphore;       /**< Semaphore used by blocking functions. */
    bool blocking;                        /**< Specifies if the current transfer is blocking. */
    bool sendStop;                        /**< Specifies if STOP condition must be generated after current transfer. */
    uint8_t sdaPin;                       /**< SuperIO pin to use as I2C SDA pin. */
    uint8_t sclPin;                       /**< SuperIO pin to use as I2C SCL pin. */
    uint8_t dummyDmaIdle;                 /**< Dummy location for PDMA transfers. */
    uint8_t dummyDmaStop;                 /**< Dummy location for PDMA transfers. */
    uint8_t dummyDmaReceive;              /**< Dummy location for PDMA transfers. */
    uint8_t dmaReceiveTxStop0;            /**< Stores setting for setting Tx shifter stop bit to 0. */
    uint8_t dmaReceiveTxStop1;            /**< Stores setting for setting Tx shifter stop bit to 1. */
    uint8_t dmaReceiveRxStop1;            /**< Stores setting for setting Rx shifter stop bit to 1. */
    uint8_t lastReload;                   /**< Counter value for the last timer reload. */
    uint8_t stcd[SUPERIO_I2C_DMA_BUF_SIZE]; /**< Buffer for PDMA scatter-gather operations. */
} superio_i2c_master_state_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Initialize the SUPERIO_I2C master mode driver.
 *
 * This function initializes the SUPERIO_I2C driver in master mode.
 *
 * @param instance  SUPERIO peripheral instance number.
 * @param userConfigPtr    Pointer to the SUPERIO_I2C master user configuration structure. The function
 *                         reads configuration data from this structure and initializes the
 *                         driver accordingly. The application may free this structure after the function returns.
 * @param master    Pointer to the SUPERIO_I2C master driver context structure. The driver uses
 *                  this memory area for its internal logic. The application must make no
 *                  assumptions about the content of this structure, and must not free this
 *                  memory until the driver is de-initialized using SUPERIO_I2C_DRV_MasterDeinit().
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2C_DRV_MasterInit(uint32_t instance, const superio_i2c_master_user_config_t *userConfigPtr,
                                    superio_i2c_master_state_t *master);

/**
 * @brief De-initialize the SUPERIO_I2C master mode driver.
 *
 * This function de-initializes the SUPERIO_I2C driver in master mode. The driver can't be used
 * again until reinitialized. The context structure is no longer needed by the driver and
 * can be freed after calling this function.
 *
 * @param master    Pointer to the SUPERIO_I2C master driver context structure.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2C_DRV_MasterDeinit(superio_i2c_master_state_t *master);

/**
 * @brief Set the baud rate for any subsequent I2C communication.
 *
 * This function sets the baud rate (SCL frequency) for the I2C master.
 * Note that due to module limitation not any baud rate can be achieved. The driver
 * will set a baud rate as close as possible to the requested baud rate, but there may
 * still be substantial differences, for example if requesting a high baud rate while
 * using a low-frequency SUPERIO clock. The application should call
 * SUPERIO_I2C_DRV_MasterGetBaudRate() after SUPERIO_I2C_DRV_MasterSetBaudRate() to check
 * what baud rate was actually set.
 *
 * @param master    Pointer to the SUPERIO_I2C master driver context structure.
 * @param baudRate  The desired baud rate in hertz.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2C_DRV_MasterSetBaudRate(superio_i2c_master_state_t *master, uint32_t baudRate);

/**
 * @brief Get the currently configured baud rate.
 *
 * This function returns the currently configured I2C baud rate.
 *
 * @param master    Pointer to the SUPERIO_I2C master driver context structure.
 * @param baudRate  The current baud rate in hertz.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2C_DRV_MasterGetBaudRate(superio_i2c_master_state_t *master, uint32_t *baudRate);

/**
 * @brief Set the slave address for any subsequent I2C communication.
 *
 * This function sets the slave address which will be used for any future transfer.
 *
 * @param master    Pointer to the SUPERIO_I2C master driver context structure.
 * @param address   Slave address, 7-bit.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2C_DRV_MasterSetSlaveAddr(superio_i2c_master_state_t *master, const uint16_t address);

/**
 * @brief Perform a non-blocking send transaction on the I2C bus.
 *
 * This function starts the transmission of a block of data to the currently
 * configured slave address and returns immediately.
 * The rest of the transmission is handled by the interrupt service routine (if the driver
 * is initialized in interrupt mode) or by the SUPERIO_I2C_DRV_MasterGetStatus function (if
 * the driver is initialized in polling mode).
 * Use SUPERIO_I2C_DRV_MasterGetStatus() to check the progress of the transmission.
 *
 * @param master    Pointer to the SUPERIO_I2C master driver context structure.
 * @param txBuff    Pointer to the data to be transferred.
 * @param txSize    Length in bytes of the data to be transferred.
 * @param sendStop  Specifies whether or not to generate stop condition after the transmission.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2C_DRV_MasterSendData(superio_i2c_master_state_t *master, const uint8_t *txBuff, uint32_t txSize,
                                        bool sendStop);

/**
 * @brief Perform a blocking send transaction on the I2C bus.
 *
 * This function sends a block of data to the currently configured slave address, and
 * only returns when the transmission is complete.
 *
 * @param master    Pointer to the SUPERIO_I2C master driver context structure.
 * @param txBuff    Pointer to the data to be transferred.
 * @param txSize    Length in bytes of the data to be transferred.
 * @param sendStop  Specifies whether or not to generate stop condition after the transmission.
 * @param timeout   Timeout for the transfer in milliseconds.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2C_DRV_MasterSendDataBlocking(superio_i2c_master_state_t *master, const uint8_t *txBuff,
                                                uint32_t txSize, bool sendStop, uint32_t timeout);

/**
 * @brief Perform a non-blocking receive transaction on the I2C bus.
 *
 * This function starts the reception of a block of data from the currently
 * configured slave address and returns immediately.
 * The rest of the transmission is handled by the interrupt service routine (if the driver
 * is initialized in interrupt mode) or by the SUPERIO_I2C_DRV_MasterGetStatus function (if
 * the driver is initialized in polling mode).
 * Use SUPERIO_I2C_DRV_MasterGetStatus() to check the progress of the reception.
 *
 * @param master    Pointer to the SUPERIO_I2C master driver context structure.
 * @param rxBuff    Pointer to the buffer where to store received data.
 * @param rxSize    Length in bytes of the data to be transferred.
 * @param sendStop  Specifies whether or not to generate stop condition after the reception.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2C_DRV_MasterReceiveData(superio_i2c_master_state_t *master, uint8_t *rxBuff, uint32_t rxSize,
                                           bool sendStop);

/**
 * @brief Perform a blocking receive transaction on the I2C bus.
 *
 * This function receives a block of data from the currently configured slave address,
 * and only returns when the transmission is complete.
 *
 * @param master    Pointer to the SUPERIO_I2C master driver context structure.
 * @param rxBuff    Pointer to the buffer where to store received data.
 * @param rxSize    Length in bytes of the data to be transferred.
 * @param sendStop  Specifies whether or not to generate stop condition after the reception.
 * @param timeout   Timeout for the transfer in milliseconds.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2C_DRV_MasterReceiveDataBlocking(superio_i2c_master_state_t *master, uint8_t *rxBuff, uint32_t rxSize,
                                                   bool sendStop, uint32_t timeout);

/**
 * @brief Aborts a non-blocking I2C master transaction.
 *
 * This function aborts a non-blocking I2C transfer.
 *
 * @param master    Pointer to the SUPERIO_I2C master driver context structure.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2C_DRV_MasterTransferAbort(superio_i2c_master_state_t *master);

/**
 * @brief Get the status of the current non-blocking I2C master transaction.
 *
 * This function returns the current status of a non-blocking I2C master transaction.
 * A return code of STATUS_BUSY means the transfer is still in progress.
 * Otherwise the function returns a status reflecting the outcome of the last transfer.
 * When the driver is initialized in polling mode this function also advances the transfer
 * by checking and handling the transmit and receive events, so it must be called
 * frequently to avoid overflows or underflows.
 *
 * @param master          Pointer to the SUPERIO_I2C master driver context structure.
 * @param bytesRemaining  The remaining number of bytes to be transferred.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_I2C_DRV_MasterGetStatus(superio_i2c_master_state_t *master, uint32_t *bytesRemaining);

/**
 * @brief Returns default configuration structure for SUPERIO_I2C.
 *
 * @param userConfigPtr    Pointer to the SUPERIO_I2C user configuration structure.
 */
void SUPERIO_I2C_DRV_GetDefaultConfig(superio_i2c_master_user_config_t *userConfigPtr);

/**
 * @brief Generate nine clock on SCL line to free SDA line.
 *
 * This function should be called when SDA line be stuck in low.
 *
 * @param master    Pointer to the SUPERIO_I2C master driver context structure.
 * @return status_t
 *             STATUS_BUSY: Driver is transferring data.
 *             STATUS_SUCCESS: Function started generating clock.
 */
status_t SUPERIO_I2C_DRV_GenerateNineClock(superio_i2c_master_state_t *master);

/**
 *
 * @brief Indicate the generation nine clock is done or not.
 *
 * @param master    Pointer to the SUPERIO_I2C master driver context structure.
 * @return status_t
 *             STATUS_BUSY: Clock generation not done yet.
 *             STATUS_SUCCESS: Device finished generating nine clock.
 */
status_t SUPERIO_I2C_DRV_StatusGenerateNineClock(superio_i2c_master_state_t *master);

/**
 *
 * @brief Check status whether SDA or SCL line be low or high.
 *
 * @param master    Pointer to the SUPERIO_I2C master driver context structure.
 * @param sdaLine   true: Function return status of SDA line.
 *                  false: Function return status of SCL line.
 * @return bool
 *             true: Pin selected is high.
 *             false: Pin selected is low.
 */
bool SUPERIO_I2C_DRV_GetBusStatus(const superio_i2c_master_state_t *master, bool sdaLine);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */ /* end of group SUPERIO_I2C_Driver */

/**
 * @}
 */ /* end of group Peripheral_Driver_layer */

#endif /* __SUPERIO_I2C_DRIVER_H__ */
