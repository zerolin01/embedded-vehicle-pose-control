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
 * @file spi_master_driver.h
 * @brief SPI master driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:        Version:        Author:
 * Changes:
 *
 */

#ifndef __SPI_MASTER_DRIVER_H__
#define __SPI_MASTER_DRIVER_H__

#include "spi_common.h"

/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup SPI_Master_Driver SPI Master Driver
 * @{
 */

/**
 * @brief Data structure containing information about a device on the SPI bus.
 *
 * The user must populate these members to set up the SPI master and
 * properly communicate with the SPI device.
 */
typedef struct {
    uint32_t bitsPerSec;                   /**< Baud rate in bits per second. \D{1000000UL} */
    spi_which_pcs_t euWhichPcs;          /**< Selects which PCS to use. \D{SPI_PCS0} */
    spi_signal_polarity_t euPcsPolarity; /**< PCS polarity. \D{SPI_ACTIVE_HIGH} */
    bool isPcsContinuous;                  /**< Keeps PCS asserted until transfer complete. \D{false} */
    uint16_t bitcount;                     /**< Number of bits/frame, minimum is 8-bits. \D{8U} */
    spi_clock_phase_t euClkPhase; /**< Selects which phase of clock to capture data. \D{SPI_CLOCK_PHASE_1ST_EDGE} */
    spi_sck_polarity_t euClkPolarity; /**< Selects clock polarity. \D{SPI_SCK_ACTIVE_HIGH} */
    bool lsbFirst;                      /**< Option to transmit LSB first. \D{false} */
    spi_transfer_type euTransferType; /**< Type of SPI transfer. \D{SPI_USING_INTERRUPTS} */
    uint8_t rxDMAChannel;    /**< Channel number for PDMA rx channel. If PDMA mode isn't used this field will be ignored.
                                \D{0U} */
    uint8_t txDMAChannel;    /**< Channel number for PDMA tx channel. If PDMA mode isn't used this field will be ignored.
                                \D{0U} */
    spi_callback_t callback; /**< Select the callback to transfer complete. \D{NULL} */
    void *callbackParam;     /**< Select additional callback parameters if it's necessary. \D{NULL} */
    spi_transfer_width_t euWidth;                       /**< Transfer bit width. \D{SPI_SINGLE_BIT_XFER} */
    data_match_cfg_t stDataMatchCfg;                      /**< Data Match Configuration. */
    spi_master_trigger_type euRequestSelect;            /**< Host request select. \D{SPI_TRIGGER_TRIGGER} */
    spi_master_request_hreq_polarity euRequestPolarity; /**< Hreq Request polarity. \D{SPI_REQUEST_POLARITY_LOW} */
    bool RequestEnable;                                   /**< trigger Enable. \D{false} */
} spi_master_config_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Return default configuration for SPI master.
 *
 * @param  pstSpiConfig Pointer to configuration structure which is filled with default configuration.
 */
void SPI_DRV_MasterGetDefaultConfig(spi_master_config_t *pstSpiConfig);

/**
 * @brief Initializes a SPI instance for interrupt driven master mode operation.
 *
 * @param  instance The instance number of the SPI peripheral.
 * @param  pstSpiState The pointer to the SPI master driver state structure. The user
 *  passes the memory for this run-time state structure. The SPI master driver
 *  populates the members. This run-time state structure keeps track of the
 *  transfer in progress.
 * @param  pstSpiConfig The data structure containing information about a device on the SPI bus.
 * @return status_t An error code or STATUS_SUCCESS.
 */
status_t SPI_DRV_MasterInit(uint32_t instance, spi_state_t *pstSpiState,
                              const spi_master_config_t *pstSpiConfig);

/**
 * @brief Shuts down a SPI instance.
 *
 * This function resets the SPI peripheral, gates its clock, and disables the interrupt to
 * the core.  It first checks to see if a transfer is in progress and if so returns an error
 * status.
 *
 * @param  instance The instance number of the SPI peripheral.
 * @return status_t
 * STATUS_SUCCESS The transfer has completed successfully.
 * STATUS_BUSY The transfer is still in progress.
 * STATUS_ERROR If driver is error and needs to clean error.
 */
status_t SPI_DRV_MasterDeinit(uint32_t instance);

/**
 * @brief Configures the SPI master mode bus timing delay options.
 *
 * This function involves the SPI module's delay options to
 * "fine tune" some of the signal timings and match the timing needs of a slower peripheral device.
 * This is an optional function that can be called after the SPI module has been initialized for
 * master mode. The timings are adjusted in terms of cycles of the baud rate clock.
 * The bus timing delays that can be adjusted are listed below:
 *
 * SCK to PCS Delay: Adjustable delay option between the last edge of SCK to the de-assertion
 *                   of the PCS signal.
 *
 * PCS to SCK Delay: Adjustable delay option between the assertion of the PCS signal to the
 *                   first SCK edge.
 *
 * Delay between Transfers: Adjustable delay option between the de-assertion of the PCS signal for
 *                          a frame to the assertion of the PCS signal for the next frame.
 *
 * @param  instance The instance number of the SPI peripheral.
 * @param  delayBetweenTransfers Minimum delay between 2 transfers in clock cycles.
 * @param  delaySCKtoPCS Minimum delay between SCK and PCS in clock cycles.
 * @param  delayPCStoSCK Minimum delay between PCS and SCK in clock cycles.
 * @return status_t
 * STATUS_SUCCESS The transfer has completed successfully.
 * STATUS_ERROR If driver is error and needs to clean error.
 */
status_t SPI_DRV_MasterSetDelay(uint32_t instance, uint32_t delayBetweenTransfers, uint32_t delaySCKtoPCS,
                                  uint32_t delayPCStoSCK);

/**
 * @brief Configures the SPI port physical parameters to access a device on the bus when the LSPI
 *        instance is configured for interrupt operation.
 *
 * In this function, the term "spiConfig" is used to indicate the SPI device for which the SPI
 * master is communicating. This is an optional function as the spiConfig parameters are
 * normally configured in the initialization function or the transfer functions, where these various
 * functions would call the configure bus function.
 * This is an example to set up the spi_master_config_t structure
 * to call the SPI_DRV_MasterConfigureBus function by passing in these parameters:
   @code
    spi_master_config_t spiConfig1;   You can also declare spiConfig2, spiConfig3, etc
    spiConfig.bitsPerSec = 500000;
    spiConfig.euWhichPcs = SPI_PCS0;
    spiConfig.euPcsPolarity = SPI_ACTIVE_LOW;
    spiConfig.isPcsContinuous = false;
    spiConfig.bitCount = 16;
    spiConfig.euClkPhase = SPI_CLOCK_PHASE_1ST_EDGE;
    spiConfig.euClkPolarity = SPI_ACTIVE_HIGH;
    spiConfig.lsbFirst= false;
    spiConfig.euTransferType = SPI_USING_INTERRUPTS;
   @endcode
 *
 * @param  instance The instance number of the SPI peripheral.
 * @param  pstSpiConfig Pointer to the pstSpiConfig structure. This structure contains the settings
 *  for the SPI bus configuration.  The SPI device parameters are the desired baud rate (in
 *  bits-per-sec), bits-per-frame, chip select attributes, clock attributes, and data shift
 *  direction.
 * @param  calculatedBaudRate The calculated baud rate passed back to the user to determine
 *  if the calculated baud rate is close enough to meet the needs. The baud rate never exceeds
 *  the desired baud rate.
 * @return status_t
 * STATUS_SUCCESS The transfer has completed successfully.
 * STATUS_ERROR If driver is error and needs to clean error.
 */
status_t SPI_DRV_MasterConfigureBus(uint32_t instance, const spi_master_config_t *pstSpiConfig,
                                      uint32_t *calculatedBaudRate);

/**
 * @brief Select the chip to communicate with.
 *
 * The main purpose of this function is to set the PCS and the appropriate polarity.
 *
 * @param  instance SPI instance
 * @param  euWhichPcs Selected chip.
 * @param  euPolarity Chip select line polarity.
 * @return status_t
 * STATUS_SUCCESS The transfer has completed successfully.
 * STATUS_ERROR If driver is error and needs to clean error.
 */
status_t SPI_DRV_SetPcs(uint32_t instance, spi_which_pcs_t euWhichPcs, spi_signal_polarity_t euPolarity);

/**
 * @brief Performs an interrupt driven blocking SPI master mode transfer.
 *
 * This function simultaneously sends and receives data on the SPI bus, as SPI is naturally
 * a full-duplex bus. The function does not return until the transfer is complete.
 * This function allows the user to optionally pass in a SPI configuration structure which
 * allows the user to change the SPI bus attributes in conjunction with initiating a SPI transfer.
 * The difference between passing in the SPI configuration structure here as opposed to the
 * configure bus function is that the configure bus function returns the calculated baud rate where
 * this function does not. The user can also call the configure bus function prior to the transfer
 * in which case the user would simply pass in a NULL to the transfer function's device structure
 * parameter.
 * Depending on frame size sendBuffer and receiveBuffer must be aligned like this:
 * -1 byte if frame size <= 8 bits.
 * -2 bytes if 8 bits < frame size <= 16 bits.
 * -4 bytes if 16 bits < frame size.
 *
 * @param  instance The instance number of the SPI peripheral.
 * @param  sendBuffer The pointer to the data buffer of the data to send. You may pass NULL for this
 *  parameter and  bytes with a value of 0 (zero) is sent.
 * @param  receiveBuffer Pointer to the buffer where the received bytes are stored. If you pass NULL
 *  for this parameter, the received bytes are ignored.
 * @param  transferByteCount The number of bytes to send and receive which is equal to size of send or receive buffers.
 * @param  timeout A timeout for the transfer in milliseconds. If the transfer takes longer than
 *  this amount of time, the transfer is aborted and a STATUS_TIMEOUT error.
 * @return status_t
 * STATUS_SUCCESS The transfer was successful.
 * STATUS_BUSY Cannot perform transfer because a transfer is already in progress.
 * STATUS_TIMEOUT The transfer timed out and was aborted.
 */
status_t SPI_DRV_MasterTransferBlocking(uint32_t instance, const uint8_t *sendBuffer, uint8_t *receiveBuffer,
                                          uint16_t transferByteCount, uint32_t timeout);

/**
 * @brief Performs an interrupt driven non-blocking SPI master mode transfer.
 *
 * This function simultaneously sends and receives data on the SPI bus, as SPI is naturally
 * a full-duplex bus. The function returns immediately after initiating the transfer. The user
 * needs to check whether the transfer is complete using the SPI_DRV_MasterGetTransferStatus
 * function.
 * This function allows the user to optionally pass in a SPI configuration structure which
 * allows the user to change the SPI bus attributes in conjunction with initiating a SPI transfer.
 * The difference between passing in the SPI configuration structure here as opposed to the
 * configure bus function is that the configure bus function returns the calculated baud rate where
 * this function does not. The user can also call the configure bus function prior to the transfer
 * in which case the user would simply pass in a NULL to the transfer function's device structure
 * parameter.
 * Depending on frame size sendBuffer and receiveBuffer must be aligned like this:
 * -1 byte if frame size <= 8 bits.
 * -2 bytes if 8 bits < frame size <= 16 bits.
 * -4 bytes if 16 bits < frame size.
 *
 * @param  instance The instance number of the SPI peripheral.
 * @param  sendBuffer The pointer to the data buffer of the data to send. You may pass NULL for this
 *  parameter and  bytes with a value of 0 (zero) is sent.
 * @param  receiveBuffer Pointer to the buffer where the received bytes are stored. If you pass NULL
 *  for this parameter, the received bytes are ignored.
 * @param  transferByteCount The number of bytes to send and receive which is equal to size of send or receive buffers
 * @return status_t
 * STATUS_SUCCESS The transfer was successful.
 * STATUS_BUSY Cannot perform transfer because a transfer is already in progress.
 */
status_t SPI_DRV_MasterTransfer(uint32_t instance, const uint8_t *sendBuffer, uint8_t *receiveBuffer,
                                  uint16_t transferByteCount);

/**
 * @brief Returns whether the previous interrupt driven transfer is completed.
 *
 * When performing an a-sync (non-blocking) transfer, the user can call this function to ascertain
 * the state of the current transfer: in progress (or busy) or complete (success).
 * In addition, if the transfer is still in progress, the user can get the number of words that
 * have been transferred up to now.
 *
 * @param  instance The instance number of the SPI peripheral.
 * @param  bytesRemained Pointer to a value that is filled in with the number of bytes that
 * must be received.
 * @return status_t
 * STATUS_SUCCESS The transfer has completed successfully.
 * STATUS_BUSY The transfer is still in progress. framesTransferred is filled
 * with the number of words that have been transferred so far.
 */
status_t SPI_DRV_MasterGetTransferStatus(uint32_t instance, uint32_t *bytesRemained);

/**
 * @brief Terminates an interrupt driven asynchronous transfer early.
 *
 * During an a-sync (non-blocking) transfer, the user has the option to terminate the transfer early
 * if the transfer is still in progress.
 *
 * @param  instance The instance number of the SPI peripheral.
 * @return status_t
 * STATUS_SUCCESS The transfer was successful.
 * SPI_STATUS_NO_TRANSFER_IN_PROGRESS No transfer is currently in progress.
 */
status_t SPI_DRV_MasterAbortTransfer(uint32_t instance);

/**
 * @brief Interrupt handler for SPI master mode.
 *
 * This handler uses the buffers stored in the spi_master_state_t structs to transfer data.
 *
 * @param  instance The instance number of the SPI peripheral.
 */
void SPI_DRV_MasterIRQHandler(uint32_t instance);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */ /* end of group SPI_Master_Driver */

/**
 * @}
 */ /* end of group Peripheral_Driver_layer */
#endif
