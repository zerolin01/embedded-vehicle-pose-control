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
 * @file spi_slave_driver.h
 * @brief SPI slave driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:        Version:        Author:
 * Changes:
 *
 */

#ifndef __SPI_SLAVE_DRIVER_H__
#define __SPI_SLAVE_DRIVER_H__

#include "spi_common.h"

/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup SPI_Slave_Driver SPI Slave Driver
 * @{
 */

/**
 *  @brief User configuration structure for the SPI slave driver.
 */
typedef struct {
    spi_signal_polarity_t euPcsPolarity; /**< PCS polarity. \D{SPI_ACTIVE_HIGH} */
    uint16_t bitcount;                     /**< Number of bits/frame, minimum is 8-bits. \D{8U} */
    spi_clock_phase_t euClkPhase; /**< Selects which phase of clock to capture data. \D{SPI_CLOCK_PHASE_1ST_EDGE} */
    spi_which_pcs_t euWhichPcs;   /**< \D{SPI_PCS0} */
    spi_sck_polarity_t euClkPolarity; /**< Selects clock polarity. \D{SPI_SCK_ACTIVE_HIGH} */
    bool lsbFirst;                      /**< Option to transmit LSB first. \D{false} */
    spi_transfer_type euTransferType; /**< Type of SPI transfer. \D{SPI_USING_INTERRUPTS} */
    uint8_t rxDMAChannel;    /**< Channel number for PDMA rx channel. If PDMA mode isn't used this field will be ignored.
                                \D{0U} */
    uint8_t txDMAChannel;    /**< Channel number for PDMA tx channel. If PDMA mode isn't used this field will be ignored.
                                \D{0U} */
    spi_callback_t callback; /**< Select the callback to transfer complete. \D{NULL} */
    void *callbackParam;     /**< Select additional callback parameters if it's necessary. \D{NULL} */
    spi_transfer_width_t euWidth;  /**< Transfer bit width. \D{SPI_SINGLE_BIT_XFER} */
    data_match_cfg_t stDataMatchCfg; /**< Data Match Configuration. */
} spi_slave_config_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Return default configuration for SPI master.
 *
 * Initializes a structured provided by user with the configuration
 * of an interrupt based SPI transfer.
 *
 * @param  pstSpiConfig The pointer to the SPI slave driver user configuration structure.
 */
void SPI_DRV_SlaveGetDefaultConfig(spi_slave_config_t *pstSpiConfig);

/**
 * @brief Initializes a SPI instance for a slave mode operation, using interrupt mechanism.
 *
 * This function un-gates the clock to the SPI module, initializes the SPI for
 * slave mode. After it is  initialized, the SPI module is configured in slave mode and the
 * user can start transmitting and receiving data by calling send, receive, and transfer functions.
 * This function indicates SPI slave uses an interrupt mechanism.
 *
 * @param  instance The instance number of the SPI peripheral.
 * @param  pstSpiState The pointer to the SPI slave driver state structure.
 * @param  pstSlaveConfig The configuration structure spi_slave_user_config_t which
 * configures the data bus format.
 * @return status_t An error code or STATUS_SUCCESS.
 */
status_t SPI_DRV_SlaveInit(uint32_t instance, spi_state_t *pstSpiState,
                             const spi_slave_config_t *pstSlaveConfig);

/**
 * @brief Shuts down an SPI instance interrupt mechanism.
 *
 * Disables the SPI module, gates its clock, and changes the SPI slave driver state to NonInit for the
 * SPI slave module which is initialized with interrupt mechanism. After de-initialization, the
 * user can re-initialize the SPI slave module with other mechanisms.
 *
 * @param  instance The instance number of the SPI peripheral.
 * @return status_t
 * STATUS_SUCCESS If driver starts to send/receive data successfully.
 * STATUS_ERROR If driver is error and needs to clean error.
 * STATUS_BUSY If a transfer is in progress
 */
status_t SPI_DRV_SlaveDeinit(uint32_t instance);

/**
 * @brief Transfers data on SPI bus using a blocking call.
 *
 * This function checks the driver status and mechanism and transmits/receives data through the SPI
 * bus. If the sendBuffer is NULL, the transmit process is ignored. If the receiveBuffer is NULL, the
 * receive process is ignored. If both the receiveBuffer and the sendBuffer are available, the transmit and the
 * receive progress is processed. If only the receiveBuffer is available, the receive is
 * processed. Otherwise, the transmit is processed. This function only returns when the
 * processes are completed. This function uses an interrupt mechanism.
 * Depending on frame size sendBuffer and receiveBuffer must be aligned like this:
 * -1 byte if frame size <= 8 bits
 * -2 bytes if 8 bits < frame size <= 16 bits
 * -4 bytes if 16 bits < frame size
 *
 * @param  instance The instance number of SPI peripheral.
 * @param  sendBuffer The pointer to data that user wants to transmit.
 * @param  receiveBuffer The pointer to data that user wants to store received data.
 * @param  transferByteCount The number of bytes to send and receive which is equal to size of send or receive buffers.
 * @param  timeout The maximum number of milliseconds that function waits before.
 * @return status_t
 * STATUS_SUCCESS If driver starts to send/receive data successfully.
 * STATUS_ERROR If driver is error and needs to clean error.
 * STATUS_BUSY If a transfer is in progress.
 * STATUS_TIMEOUT if time out reached while transferring is in progress.
 */
status_t SPI_DRV_SlaveTransferBlocking(uint32_t instance, const uint8_t *sendBuffer, uint8_t *receiveBuffer,
                                         uint16_t transferByteCount, uint32_t timeout);

/**
 * @brief Starts the transfer data on SPI bus using a non-blocking call.
 *
 * This function checks the driver status and mechanism and transmits/receives data through the SPI
 * bus. If the sendBuffer is NULL, the transmit process is ignored. If the receiveBuffer is NULL, the
 * receive process is ignored. If both the receiveBuffer and the sendBuffer are available, the transmit and the
 * receive progress is processed. If only the receiveBuffer is available, the receive is
 * processed. Otherwise, the transmit is processed. This function only returns when the
 * processes are completed. This function uses an interrupt mechanism.
 * Depending on frame size sendBuffer and receiveBuffer must be aligned like this:
 * -1 byte if frame size <= 8 bits.
 * -2 bytes if 8 bits < frame size <= 16 bits.
 * -4 bytes if 16 bits < frame size.
 *
 * @param  instance The instance number of SPI peripheral.
 * @param  sendBuffer The pointer to data that user wants to transmit.
 * @param  receiveBuffer The pointer to data that user wants to store received data.
 * @param  transferByteCount The number of bytes to send and receive which is equal to size of send or receive buffers.
 * @return status_t
 * STATUS_SUCCESS If driver starts to send/receive data successfully.
 * STATUS_ERROR If driver is error and needs to clean error.
 * STATUS_BUSY If a transfer is in progress.
 */
status_t SPI_DRV_SlaveTransfer(uint32_t instance, const uint8_t *sendBuffer, uint8_t *receiveBuffer,
                                 uint16_t transferByteCount);

/**
 * @brief Aborts the transfer that started by a non-blocking call transfer function.
 *
 * This function stops the transfer which was started by the calling the SPI_DRV_SlaveTransfer() function.
 *
 * @param  instance The instance number of SPI peripheral.
 * @return status_t
 * STATUS_SUCCESS If everything is OK.
 */
status_t SPI_DRV_SlaveAbortTransfer(uint32_t instance);

/**
 * @brief Returns whether the previous transfer is finished.
 *
 * When performing an a-sync transfer, the user can call this function to ascertain
 * the state of the current transfer: in progress (or busy) or complete (success).
 * In addition, if the transfer is still in progress, the user can get the number
 * of bytes that have been transferred up to now.
 *
 * @param  instance The instance number of the SPI peripheral.
 * @param  bytesRemained Pointer to value that is filled in with the number of
 *                       frames that have been sent in the active transfer. A frame is defined as the
 *                       number of bits per frame.
 * @return status_t
 * STATUS_SUCCESS The transfer has completed successfully.
 * STATUS_BUSY The transfer is still in progress.
 * STATUS_ERROR If driver is error and needs to clean error.
 */
status_t SPI_DRV_SlaveGetTransferStatus(uint32_t instance, uint32_t *bytesRemained);

/**
 * @brief Interrupt handler for SPI slave mode.
 *
 * This handler uses the buffers stored in the spi_master_state_t structs to transfer data.
 *
 * @param  instance The instance number of the SPI peripheral.
 */
void SPI_DRV_SlaveIRQHandler(uint32_t instance);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */ /* end of group SPI_Slave_Driver */

/**
 * @}
 */ /* end of group Peripheral_Driver_layer */

#endif
