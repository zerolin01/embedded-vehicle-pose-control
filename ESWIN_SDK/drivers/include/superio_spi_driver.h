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
 * @file superio_spi_driver.h
 * @brief SUPERIO spi driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __SUPERIO_SPI_DRIVER_H__
#define __SUPERIO_SPI_DRIVER_H__

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
 * @addtogroup SUPERIO_SPI_Driver SUPERIO_SPI Driver
 * @{
 */

/**
 * @brief Order in which the data bits are transferred.
 */
typedef enum {
    SUPERIO_SPI_TRANSFER_MSB_FIRST = 0U, /**< Transmit data starting with most significant bit. */
    SUPERIO_SPI_TRANSFER_LSB_FIRST = 1U, /**< Transmit data starting with least significant bit. */
} superio_spi_transfer_bit_order_t;

/**
 * @brief Size of transferred data in bytes.
 */
typedef enum {
    SUPERIO_SPI_TRANSFER_1BYTE = 1U, /**< Data size is 1-byte.  */
    SUPERIO_SPI_TRANSFER_2BYTE = 2U, /**< Data size is 2-bytes. */
    SUPERIO_SPI_TRANSFER_4BYTE = 4U, /**< Data size is 4-bytes. */
} superio_spi_transfer_size_t;

/**
 * @brief Master configuration structure.
 *
 * This structure is used to provide configuration parameters for the superio_spi master at initialization time.
 */
typedef struct {
    uint32_t baudRate;                /**< Baud rate in hertz. \R{2400UL,3000000UL} \D{115200UL} */
    superio_driver_type_t driverType; /**< Driver type: interrupts/polling/PDMA. \D{SUPERIO_DRIVER_TYPE_INTERRUPTS} */
    superio_spi_transfer_bit_order_t
        bitOrder;                             /**< Bit order: LSB-first / MSB-first. \D{SUPERIO_SPI_TRANSFER_MSB_FIRST} */
    superio_spi_transfer_size_t transferSize; /**< Transfer size in bytes: 1/2/4. \D{SUPERIO_SPI_TRANSFER_1BYTE} */
    uint8_t clockPolarity;                    /**< Clock Polarity (CPOL) 0 = active-high clock; 1 = active-low clock. \R{0,1} \D{0U} */
    uint8_t clockPhase;                       /**< Clock Phase (CPHA) 0 = sample on leading clock edge; 1 = sample on trailing clock edge.
                              \R{0,1} \D{0U} */
    uint8_t mosiPin;                          /**< superio pin to use as MOSI pin. \R{0,7} \D{1} */
    uint8_t misoPin;                          /**< superio pin to use as MISO pin. \R{0,7} \D{2} */
    uint8_t sckPin;                           /**< superio pin to use as SCK pin. \R{0,7} \D{3} */
    uint8_t ssPin;                            /**< superio pin to use as SS pin. \R{0,7} \D{4} */
    spi_callback_t callback;                  /**< User callback function. Note that this function will be
                                      called from the interrupt service routine, so its
                                      execution time should be as small as possible. It can be
                                      NULL if it is not needed. \D{NULL} */
    void *callbackParam;                      /**< Parameter for the callback function. \D{NULL} */
    uint8_t rxDMAChannel;                     /**< Rx PDMA channel number. Only used in PDMA mode. \R{0,32} \D{0} */
    uint8_t txDMAChannel;                     /**< Tx PDMA channel number. Only used in PDMA mode. \R{0,32} \D{2} */
} superio_spi_master_user_config_t;

/**
 * @brief Slave configuration structure.
 *
 * This structure is used to provide configuration parameters for the superio_spi slave at initialization time.
 */
typedef struct {
    superio_driver_type_t driverType; /**< Driver type: interrupts/polling/PDMA. \D{SUPERIO_DRIVER_TYPE_INTERRUPTS} */
    superio_spi_transfer_bit_order_t
        bitOrder;                             /**< Bit order: LSB-first / MSB-firs. \D{SUPERIO_SPI_TRANSFER_MSB_FIRST} */
    superio_spi_transfer_size_t transferSize; /**< Transfer size in bytes: 1/2/4. \D{SUPERIO_SPI_TRANSFER_1BYTE} */
    uint8_t clockPolarity;                    /**< Clock Polarity (CPOL) 0 = active-low clock; 1 = active-high clock. \R{0,1} \D{0U} */
    uint8_t clockPhase;                       /**< Clock Phase (CPHA) 0 = sample on leading clock edge; 1 = sample on trailing clock edge.
                              \R{0,1} \D{0U} */
    uint8_t mosiPin;                          /**< superio pin to use as MOSI pin. \R{0,7} \D{4} */
    uint8_t misoPin;                          /**< superio pin to use as MISO pin. \R{0,7} \D{5} */
    uint8_t sckPin;                           /**< superio pin to use as SCK pin. \R{0,7} \D{6} */
    uint8_t ssPin;                            /**< superio pin to use as SS pin. \R{0,7} \D{7} */
    spi_callback_t callback;                  /**< User callback function. Note that this function will be
                                      called from the interrupt service routine, so its
                                      execution time should be as small as possible. It can be
                                      NULL if it is not needed. \D{NULL} */
    void *callbackParam;                      /**< Parameter for the callback function. \D{NULL} */
    uint8_t rxDMAChannel;                     /**< Rx PDMA channel number. Only used in PDMA mode. \R{0,32} \D{0} */
    uint8_t txDMAChannel;                     /**< Tx PDMA channel number. Only used in PDMA mode. \R{0,32} \D{0} */
} superio_spi_slave_user_config_t;

/**
 * @brief Master internal context structure.
 *
 * This structure is used by the master-mode driver for its internal logic. It must
 * be provided by the application through the SUPERIO_SPI_DRV_MasterInit() function, then
 * it cannot be freed until the driver is de-initialized using SUPERIO_SPI_DRV_MasterDeinit().
 * The application should make no assumptions about the content of this structure.
 */
typedef struct {
    superio_common_state_t superioCommon;      /**< Common superio drivers structure. */
    const uint8_t *txData;                     /**< Transmit buffer. */
    uint8_t *rxData;                           /**< Receive buffer. */
    uint32_t txRemainingBytes;                 /**< Number of remaining bytes to be transmitted. */
    uint32_t rxRemainingBytes;                 /**< Number of remaining bytes to be received. */
    uint32_t dummyDmaData;                     /**< Dummy location for PDMA transfers. */
    uint8_t rxDMAChannel;                      /**< Rx PDMA channel number. */
    uint8_t txDMAChannel;                      /**< Tx PDMA channel number. */
    superio_driver_type_t driverType;          /**< Driver type: interrupts/polling/PDMA. */
    superio_spi_transfer_bit_order_t bitOrder; /**< Bit order: LSB-first / MSB-first. */
    superio_spi_transfer_size_t transferSize;  /**< Transfer size in bytes: 1/2/4. */
    status_t status;                           /**< Current status of the driver. */
    bool driverIdle;                           /**< Idle/busy state of the driver. */
    OS_SemaphoreId_t idleSemaphore;            /**< Semaphore used by blocking functions. */
    bool blocking;                             /**< Specifies if the current transfer is blocking. */
    bool master;                               /**< Specifies if the current instance was initialized as master. */
    spi_callback_t callback;                   /**< User callback function. */
    void *callbackParam;                       /**< Parameter for the callback function. */
} superio_spi_master_state_t;

/**
 * @brief Slave internal context structure.
 *
 * This structure is used by the slave-mode driver for its internal logic. It must
 * be provided by the application through the SUPERIO_SPI_DRV_SlaveInit() function, then
 * it cannot be freed until the driver is de-initialized using SUPERIO_SPI_DRV_SlaveDeinit().
 * The application should make no assumptions about the content of this structure.
 */
typedef superio_spi_master_state_t superio_spi_slave_state_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Initialize the SUPERIO_SPI master mode driver.
 *
 * This function initializes the SUPERIO_SPI driver in master mode.
 *
 * @param instance  SUPERIO peripheral instance number.
 * @param userConfigPtr    Pointer to the SUPERIO_SPI master user configuration structure. The function
 *                         reads configuration data from this structure and initializes the
 *                         driver accordingly. The application may free this structure after
 *                         the function returns.
 * @param master    Pointer to the SUPERIO_SPI master driver context structure. The driver uses
 *                  this memory area for its internal logic. The application must make no
 *                  assumptions about the content of this structure, and must not free this
 *                  memory until the driver is de-initialized using SUPERIO_SPI_DRV_MasterDeinit().
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_SPI_DRV_MasterInit(uint32_t instance, const superio_spi_master_user_config_t *userConfigPtr,
                                    superio_spi_master_state_t *master);

/**
 * @brief De-initialize the SUPERIO_SPI master mode driver.
 *
 * This function de-initializes the SUPERIO_SPI driver in master mode. The driver can't be used
 * again until reinitialized. The context structure is no longer needed by the driver and
 * can be freed after calling this function.
 *
 * @param master    Pointer to the SUPERIO_SPI master driver context structure.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_SPI_DRV_MasterDeinit(superio_spi_master_state_t *master);

/**
 * @brief Set the baud rate for any subsequent SPI communication.
 *
 * This function sets the baud rate for the SPI master.
 * Note that due to module limitation not any baud rate can be achieved. The driver
 * will set a baud rate as close as possible to the requested baud rate, but there may
 * still be substantial differences, for example if requesting a high baud rate while
 * using a low-frequency SuperIO clock. The application should call
 * SUPERIO_SPI_DRV_MasterGetBaudRate() after SUPERIO_SPI_DRV_MasterSetBaudRate() to check
 * what baud rate was actually set.
 *
 * @param master    Pointer to the SUPERIO_SPI master driver context structure.
 * @param baudRate  The desired baud rate in hertz.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_SPI_DRV_MasterSetBaudRate(superio_spi_master_state_t *master, uint32_t baudRate);

/**
 * @brief Get the currently configured baud rate.
 *
 * This function returns the currently configured SPI baud rate.
 *
 * @param master    Pointer to the SUPERIO_SPI master driver context structure.
 * @param baudRate  The current baud rate in hertz.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_SPI_DRV_MasterGetBaudRate(superio_spi_master_state_t *master, uint32_t *baudRate);

/**
 * @brief Perform a non-blocking SPI master transaction.
 *
 * This function performs an SPI full-duplex transaction, transmit and receive in parallel.
 * If only transmit or receive are required, it is possible to provide NULL pointers for
 * txData or rxData. The transfer is non-blocking, the function only initiates the transfer
 * and then returns, leaving the transfer to complete asynchronously).
 * SUPERIO_SPI_DRV_MasterGetStatus() can be called to check the status of the transfer.
 *
 * @param master    Pointer to the SUPERIO_SPI master driver context structure.
 * @param txData  Pointer to the data to be transmitted.
 * @param rxData  Pointer to the buffer where to store received data.
 * @param dataSize  Length in bytes of the data to be transferred.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_SPI_DRV_MasterTransfer(superio_spi_master_state_t *master, const uint8_t *txData, uint8_t *rxData,
                                        uint32_t dataSize);

/**
 * @brief Perform a blocking SPI master transaction.
 *
 * This function performs an SPI full-duplex transaction, transmit and receive in parallel.
 * If only transmit or receive are required, it is possible to provide NULL pointers for
 * txData or rxData. The transfer is blocking, the function only returns when the transfer
 * is complete.
 *
 * @param master    Pointer to the SUPERIO_SPI master driver context structure.
 * @param txData  Pointer to the data to be transmitted.
 * @param rxData  Pointer to the buffer where to store received data.
 * @param dataSize  Length in bytes of the data to be transferred.
 * @param timeout  Timeout for the transfer in milliseconds.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_SPI_DRV_MasterTransferBlocking(superio_spi_master_state_t *master, const uint8_t *txData,
                                                uint8_t *rxData, uint32_t dataSize, uint32_t timeout);

/**
 * @brief Aborts a non-blocking SPI master transaction.
 *
 * This function aborts a non-blocking SPI transfer.
 *
 * @param master    Pointer to the SUPERIO_SPI master driver context structure.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_SPI_DRV_MasterTransferAbort(superio_spi_master_state_t *master);

/**
 * @brief Get the status of the current non-blocking SPI master transaction.
 *
 * This function returns the current status of a non-blocking SPI master transaction.
 * A return code of STATUS_BUSY means the transfer is still in progress.
 * Otherwise the function returns a status reflecting the outcome of the last transfer.
 * When the driver is initialized in polling mode this function also advances the transfer
 * by checking and handling the transmit and receive events, so it must be called
 * frequently to avoid overflows or underflows.
 *
 * @param master    Pointer to the SUPERIO_SPI master driver context structure.
 * @param bytesRemaining  The remaining number of bytes to be transferred.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_SPI_DRV_MasterGetStatus(superio_spi_master_state_t *master, uint32_t *bytesRemaining);

/**
 * @brief Initialize the SUPERIO_SPI slave mode driver.
 *
 * This function initializes the SUPERIO_SPI driver in slave mode.
 *
 * @param instance  SUPERIO peripheral instance number.
 * @param userConfigPtr    Pointer to the SUPERIO_SPI slave user configuration structure. The function
 *                         reads configuration data from this structure and initializes the
 *                         driver accordingly. The application may free this structure after
 *                         the function returns.
 * @param slave     Pointer to the SUPERIO_SPI slave driver context structure. The driver uses
 *                  this memory area for its internal logic. The application must make no
 *                  assumptions about the content of this structure, and must not free this
 *                  memory until the driver is de-initialized using SUPERIO_SPI_DRV_SlaveDeinit().
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_SPI_DRV_SlaveInit(uint32_t instance, const superio_spi_slave_user_config_t *userConfigPtr,
                                   superio_spi_slave_state_t *slave);

/**
 * @brief De-initialize the SUPERIO_SPI slave mode driver.
 *
 * This function de-initializes the SUPERIO_SPI driver in slave mode. The driver can't be used
 * again until reinitialized. The context structure is no longer needed by the driver and
 * can be freed after calling this function.
 *
 * @param slave    Pointer to the SUPERIO_SPI slave driver context structure.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_SPI_DRV_SlaveDeinit(superio_spi_slave_state_t *slave);

/**
 * @brief Perform a non-blocking SPI slave transaction.
 *
 * This function performs an SPI full-duplex transaction, transmit and receive in parallel.
 * If only transmit or receive are required, it is possible to provide NULL pointers for
 * txData or rxData. The transfer is non-blocking, the function only initiates the transfer
 * and then returns, leaving the transfer to complete asynchronously).
 * SUPERIO_SPI_DRV_SlaveGetStatus() can be called to check the status of the transfer.
 *
 * @param slave    Pointer to the SUPERIO_SPI slave driver context structure.
 * @param txData  Pointer to the data to be transmitted.
 * @param rxData  Pointer to the buffer where to store received data.
 * @param dataSize  Length in bytes of the data to be transferred.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_SPI_DRV_SlaveTransfer(superio_spi_slave_state_t *slave, const uint8_t *txData, uint8_t *rxData,
                                       uint32_t dataSize);

/**
 * @brief Perform a blocking SPI slave transaction.
 *
 * This function performs an SPI full-duplex transaction, transmit and receive in parallel.
 * If only transmit or receive are required, it is possible to provide NULL pointers for
 * txData or rxData. The transfer is blocking, the function only returns when the transfer
 * is complete.
 *
 * @param slave    Pointer to the SUPERIO_SPI slave driver context structure.
 * @param txData  Pointer to the data to be transmitted.
 * @param rxData  Pointer to the buffer where to store received data.
 * @param dataSize  Length in bytes of the data to be transferred.
 * @param timeout  Timeout for the transfer in milliseconds.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_SPI_DRV_SlaveTransferBlocking(superio_spi_slave_state_t *slave, const uint8_t *txData, uint8_t *rxData,
                                               uint32_t dataSize, uint32_t timeout);

/**
 * @brief Aborts a non-blocking SPI slave transaction.
 *
 * This function aborts a non-blocking SPI transfer.
 *
 * @param slave    Pointer to the SUPERIO_SPI slave driver context structure.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_SPI_DRV_SlaveTransferAbort(superio_spi_slave_state_t *slave);

/**
 * @brief Get the status of the current non-blocking SPI slave transaction.
 *
 * This function returns the current status of a non-blocking SPI slave transaction.
 * A return code of STATUS_BUSY means the transfer is still in progress.
 * Otherwise the function returns a status reflecting the outcome of the last transfer.
 * When the driver is initialized in polling mode this function also advances the transfer
 * by checking and handling the transmit and receive events, so it must be called
 * frequently to avoid overflows or underflows.
 *
 * @param slave    Pointer to the SUPERIO_SPI slave driver context structure.
 * @param bytesRemaining  The remaining number of bytes to be transferred.
 * @return status_t Error or success status returned by API.
 */
status_t SUPERIO_SPI_DRV_SlaveGetStatus(superio_spi_slave_state_t *slave, uint32_t *bytesRemaining);

/**
 * @brief Returns default configuration structure for SUPERIO_SPI master.
 *
 * @param userConfigPtr    Pointer to the SUPERIO_SPI user configuration structure.
 */
void SUPERIO_SPI_DRV_MasterGetDefaultConfig(superio_spi_master_user_config_t *userConfigPtr);

/**
 * @brief Returns default configuration structure for SUPERIO_SPI slave.
 *
 * @param userConfigPtr    Pointer to the SUPERIO_SPI user configuration structure.
 */
void SUPERIO_SPI_DRV_SlaveGetDefaultConfig(superio_spi_slave_user_config_t *userConfigPtr);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
/* end of group SUPERIO_SPI_Driver */

/**
 * @}
 */
/* end of group Peripheral_Driver_layer */

#endif /* __SUPERIO_SPI_DRIVER_H__ */
