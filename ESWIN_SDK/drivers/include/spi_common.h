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
 * @file spi_common.h
 * @brief SPI driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:        Version:        Author:
 * Changes:
 *
 */

#ifndef __SPI_COMMON_H__
#define __SPI_COMMON_H__

#include "callbacks.h"
#include "clock_driver.h"
#include "osal.h"
#include "pdma_driver.h"
#include "status.h"
#include "clock_driver.h"
#include <stdbool.h>
/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup SPI_Common_Driver SPI Common Driver
 * @{
 */

/** Extern for the SPI master driver's interrupt handler. */
extern void SPI_DRV_MasterIRQHandler(uint32_t instance);

/** Extern for the SPI slave driver's interrupt handler. */
extern void SPI_DRV_SlaveIRQHandler(uint32_t instance);

/** @brief SPI clock*/
extern clock_names_t g_spiClock[];

/* @brief Clock names for UART. */
#define SPI_CLOCK_NAMES                                  \
    {                                                    \
        SPI0_CLK, SPI1_CLK, SPI2_CLK, SPI3_CLK           \
    }

/**
 * @brief SPI Peripheral Chip Select (PCS) configuration (which PCS to configure).
 */
typedef enum {
    SPI_PCS0 = 0U, /**< PCS[0] */
    SPI_PCS1 = 1U, /**< PCS[1] */
    SPI_PCS2 = 2U, /**< PCS[2] */
    SPI_PCS3 = 3U  /**< PCS[3] */
} spi_which_pcs_t;

/**
 * @brief SPI Signal (PCS and Host Request) Polarity configuration.
 */
typedef enum {
    SPI_ACTIVE_HIGH = 1U, /**< Signal is Active High (idles low). */
    SPI_ACTIVE_LOW  = 0U  /**< Signal is Active Low (idles high). */
} spi_signal_polarity_t;

/**
 * @brief SPI clock phase configuration.
 */
typedef enum {
    SPI_CLOCK_PHASE_1ST_EDGE = 0U, /**< Data captured on SCK 1st edge, changed on 2nd. */
    SPI_CLOCK_PHASE_2ND_EDGE = 1U  /**< Data changed on SCK 1st edge, captured on 2nd. */
} spi_clock_phase_t;

/**
 * @brief SPI Clock Signal (SCK) Polarity configuration.
 */
typedef enum {
    SPI_SCK_ACTIVE_HIGH = 0U, /**< Signal is Active High (idles low). */
    SPI_SCK_ACTIVE_LOW  = 1U  /**< Signal is Active Low (idles high). */
} spi_sck_polarity_t;

/**
 * @brief SPI transfer width configuration.
 */
typedef enum {
    SPI_SINGLE_BIT_XFER = 0U, /**< 1-bit shift at a time, data out on SDO, in on SDI (normal mode). */
    SPI_TWO_BIT_XFER    = 1U, /**< 2-bits shift out on SDO/SDI and in on SDO/SDI. */
    SPI_FOUR_BIT_XFER   = 2U, /**< 4-bits shift out on SDO/SDI/PCS[3:2] and in on SDO/SDI/PCS[3:2]. */
    SPI_SINGLE_BIT_XFER_HALF_DUPLEX =
        3U, /**< 1-bit shift at a time, data out on SDO data in on SDI (1bit half duplex). */
} spi_transfer_width_t;

/**
 * @brief Select the source of the host request input.
 */
typedef enum {
    SPI_TRIGGER_HREQ    = 0x0, /**< Host request input is SPI_HREQ pin. */
    SPI_TRIGGER_TRIGGER = 0x1  /**< Host request input is the input trigger. */
} spi_master_trigger_type;

/**
 * @brief Configure the polarity of the request pin.
 */
typedef enum {
    SPI_REQUEST_POLARITY_LOW  = 0x0, /**< Configure the polarity of the request pin is low. */
    SPI_REQUEST_POLARITY_HIGH = 0x1  /**< Configure the polarity of the request pin is High. */
} spi_master_request_hreq_polarity;

/**
 * @brief Type of SPI transfer (based on interrupts or PDMA).
 */
typedef enum {
    SPI_USING_DMA = 0,    /**< The driver will use PDMA to perform SPI transfer. */
    SPI_USING_INTERRUPTS, /**< The driver will use interrupts to perform SPI transfer. */
} spi_transfer_type;

/**
 * @brief Type of error reported by SPI.
 */
typedef enum {
    SPI_TRANSFER_OK = 0U, /**< Transfer OK. */
    SPI_TRANSMIT_FAIL,    /**< Error during transmission. */
    SPI_RECEIVE_FAIL      /**< Error during reception. */
} transfer_status_t;

/**
 * @brief Data match mode enum.
 */
typedef enum {
    SPI_DATA_MATCH_DISABLE                  = 0, /**< Disable match(default). */
    SPI_MATCH0_OR_MATCH1_1ST                = 2, /**< 1st data match match0 or match1. */
    SPI_MATCH0_OR_MATFH1_ANY                = 3, /**< Any data match match0 or match1. */
    SPI_MATCH0_1ST_AND_MATCH1_2ND           = 4, /**< 1st data match match0 2nd match match1. */
    SPI_MATCH0_ANY_AND_MATCH1_NEXT          = 5, /**< Any data match match0 next match match1. */
} data_match_mode_t;

/**
 * @brief Match data config.
 *
 * This structure holds data that is used by data match function
 */
typedef struct data_match_cfg {
    uint32_t data0;                  /**< Match0 data. \D{0} */
    uint32_t data1;                  /**< Match1 data. \D{0} */
    bool receiveDataMatchOnly;       /**< Receive data match only(RDMO), will discard data when Data Match Flag is not
                                        asserted. \D{false} */
    data_match_mode_t dataMatchMode; /**< Match mode. \D{SPI_DATA_MATCH_DISABLE} */
} data_match_cfg_t;

/**
 * @brief Runtime state structure for the SPI master driver.
 *
 * This structure holds data that is used by the SPI peripheral driver to
 * communicate between the transfer function and the interrupt handler. The
 * interrupt handler also uses this information to keep track of its progress.
 * The user must pass  the memory for this run-time state structure. The
 * SPI master driver populates the members.
 */
typedef struct {
    uint16_t bitsPerFrame;              /**< Number of bits per frame: 8- to 4096-bits; needed for
                                             TCR programming. */
    uint16_t bytesPerFrame;             /**< Number of bytes per frame: 1- to 512-bytes. */
    bool isPcsContinuous;               /**< Option to keep chip select asserted until transfer
                                             complete; needed for TCR programming. */
    bool isBlocking;                    /**< Save the transfer type. */
    uint32_t spiSrcClk;               /**< Module source clock. */
    volatile bool isTransferInProgress; /**< True if there is an active transfer. */
    const uint8_t *txBuff;              /**< The buffer from which transmitted bytes are taken. */
    uint8_t *rxBuff;                    /**< The buffer into which received bytes are placed. */
    volatile uint16_t txCount;          /**< Number of bytes remaining to send. */
    volatile uint16_t rxCount;          /**< Number of bytes remaining to receive. */
    volatile uint16_t txFrameCnt;       /**< Number of bytes from current frame which were already sent. */
    volatile uint16_t rxFrameCnt;       /**< Number of bytes from current frame which were already received. */
    volatile bool lsb;                  /**< True if first bit is LSB and false if first bit is MSB. */
    uint8_t fifoSize;                   /**< RX/TX fifo size. */
    uint8_t rxDMAChannel;               /**< Channel number for PDMA rx channel. */
    uint8_t txDMAChannel;               /**< Channel number for PDMA tx channel. */
    spi_transfer_type euTransferType; /**< Type of SPI transfer. */
    OS_SemaphoreId_t spiSemaphore;    /**< The semaphore used for blocking transfers. */
    transfer_status_t euStatus;         /**< The status of the current. */
    spi_callback_t callback;            /**< Select the callback to transfer complete. */
    void *callbackParam;                /**< Select additional callback parameters if it's necessary. */
    uint32_t dummy;                  /**< This field is used for the cases when TX is NULL and SPI is in PDMA mode. */
    data_match_cfg_t stDataMatchCfg; /**< Data Match Configuration. */
    spi_transfer_width_t euWidth;  /**< spi width. */
} spi_state_t;


/**
 * @brief SPI Transmit Command Register configuration structure.
 *
 * This structure contains the Transmit Command Register (TCR) settings. Any writes
 * to the TCR will cause the entire TCR contents to be pushed to the TX FIFO.
 * Therefore any updates to the TCR should include updates to all of the register
 * bit fields to form a 32-bit write to the TCR.
 */
typedef struct {
    uint32_t frameSize;                 /**< Number of bits/frame, minimum is 8-bits. */
    spi_transfer_width_t euWidth;       /**< Transfer width, single, 2-bit, or 4-bit transfer. */
    bool txMask;                        /**< Option to mask the transmit data (won't load to FIFO). */
    bool rxMask;                        /**< Option to mask the receive data (won't store in FIFO). */
    bool contCmd;                       /**< Master option to change cmd word within cont transfer. */
    bool contTransfer;                  /**< Master option for continuous transfer. */
    bool changeTransferType;            /**< Change transfer type. */
} spi_tx_cfg_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief SPI IRQ Handler that registered to original default IRQ Handler.
 *
 * @param  data parameters in IRQ handler.
 */
void SPI_IRQHandler(void *data);

/**
 * @brief The function SPI_DRV_IRQHandler passes IRQ control to either the master or
 * slave driver.
 *
 * The address of the IRQ handlers are checked to make sure they are non-zero before
 * they are called. If the IRQ handler's address is zero, it means that driver was
 * not present in the link (because the IRQ handlers are marked as weak). This would
 * actually be a program error, because it means the master/slave config for the IRQ
 * was set incorrectly.
 *
 * @param  instance The instance number of the SPI peripheral.
 */
void SPI_DRV_IRQHandler(uint32_t instance);

/**
 * @brief The function SPI_DRV_FillupTxBuffer writes data in TX hardware buffer
 * depending on driver state and number of bytes remained to send.
 *
 * @param  instance The instance number of the SPI peripheral.
 */
void SPI_DRV_FillupTxBuffer(uint32_t instance);

/**
 * @brief The function SPI_DRV_ReadRXBuffer reads data from RX hardware buffer and
 * writes this data in RX software buffer.
 *
 * @param  instance The instance number of the SPI peripheral.
 */
void SPI_DRV_ReadRXBuffer(uint32_t instance);

/**
 * @brief Disable the TEIE interrupts at the end of a transfer.
 * Disable the interrupts and clear the status for transmit/receive errors.
 *
 * @param  instance The instance number of the SPI peripheral.
 */
void SPI_DRV_DisableTEIEInterrupts(uint32_t instance);

/**
 * @brief Config TCSR SPI enter sleep mode
 *
 * @param  instance The instance number of the SPI peripheral.
 * @param sleep 1: enter sleep mode. 0: exit sleep mode
 */
void SPI_DRV_ConfigSleepMode(uint32_t instance, uint8_t sleep);

/**
 * @brief Config SPI module switch in/out sleep mode
 *
 * @param  instance The instance number of the SPI peripheral.
 * @param sleep 1: enter sleep mode. 0: exit sleep mode
 */
void SPI_DRV_SwitchSleepMode(uint32_t instance, uint8_t sleep);

/**
 * @brief Config SPI check if in sleep mode
 *
 * @param  instance The instance number of the SPI peripheral.
 *
 * @return return sleep mode. 1: in sleep mode 0: not in sleep mode
 */
int SPI_DRV_CheckSleepMode(uint32_t instance);

/**
 *
 * @brief Write data to TDR
 *
 */
status_t SPI_DRV_WriteData(uint32_t instance, uint8_t *data, uint32_t size);

/**
 *
 * @brief Write data to TDR
 *
 */
status_t SPI_DRV_WriteDataDma(uint32_t instance, uint8_t *data, uint32_t size);

/**
 *
 * @brief Write Cmd to TCR
 *
 */
status_t SPI_DRV_WriteCmd(uint32_t instance, const spi_tx_cfg_t *pstTxCfg);

/**
 *
 * @brief Recv data from RDR
 *
 */
void SPI_DRV_RecvDataDma(uint32_t instance, uint8_t *data, uint32_t size);

/**
 *
 * Function Name : SPI_DRV_RecvData
 * Description   : Write data to TDR
 *
 */
void SPI_DRV_RecvData(uint32_t instance, uint8_t *data, uint32_t size);

/**
 *
 * Function Name : SPI_DRV_WaitSendComplete
 * Description   : wait Send complete
 *
 */
void SPI_DRV_WaitSendComplete(uint32_t instance);

/**
 *
 * Function Name : SPI_DRV_WaitRecvComplete
 * Description   : wait Recv complete
 *
 */
void SPI_DRV_WaitRecvComplete(uint32_t instance);

/**
 *
 * Function Name : SPI_DRV_WaitRecvComplete
 * Description   : wait Recv complete
 *
 */
void SPI_DRV_WaitRecvCompleteDma(uint32_t instance);
#if defined(__cplusplus)
}
#endif

/**
 * @}
 */ /* end of group SPI_Common_Driver */

/**
 * @}
 */ /* end of group Peripheral_Driver_layer */

#endif
