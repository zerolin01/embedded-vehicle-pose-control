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
 * @file spi_hw_access.h
 * @brief SPI driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:        Version:        Author:
 * Changes:
 *
 */
#ifndef __SPI_HW_ACCESS_H__
#define __SPI_HW_ACCESS_H__

#include "spi_common.h"
#include "spi_register.h"
#include "status.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Prescaler values for SPI clock source.
 */
typedef enum {
    SPI_DIV_1   = 0U,
    SPI_DIV_2   = 1U,
    SPI_DIV_4   = 2U,
    SPI_DIV_8   = 3U,
    SPI_DIV_16  = 4U,
    SPI_DIV_32  = 5U,
    SPI_DIV_64  = 6U,
    SPI_DIV_128 = 7U,
} spi_prescaler_t;

/**
 * @brief SPI status flags.
 */
typedef enum {
    SPI_TX_DATA_FLAG      = SPI_SR_TDF_SHIFT, /**< TX data flag */
    SPI_RX_DATA_FLAG      = SPI_SR_RDF_SHIFT, /**< RX data flag */
    SPI_WORD_COMPLETE     = SPI_SR_WCF_SHIFT, /**< Word Complete flag */
    SPI_FRAME_COMPLETE    = SPI_SR_FCF_SHIFT, /**< Frame Complete flag */
    SPI_TRANSFER_COMPLETE = SPI_SR_TCF_SHIFT, /**< Transfer Complete flag */
    SPI_TRANSMIT_ERROR    = SPI_SR_TEF_SHIFT, /**< Transmit Error flag (FIFO underrun) */
    SPI_RECEIVE_ERROR     = SPI_SR_REF_SHIFT, /**< Receive Error flag (FIFO overrun) */
    SPI_DATA_MATCH        = SPI_SR_DMF_SHIFT, /**< Data Match flag */
    SPI_MODULE_BUSY       = SPI_SR_MBF_SHIFT, /**< Module Busy flag */
    SPI_ALL_STATUS        = 0x00003F00U         /**< Used for clearing all w1c status flags */
} spi_status_flag_t;

/**
 * @brief SPI master or slave configuration.
 */
typedef enum {
    SPI_MASTER = 1U, /**< SPI peripheral operates in master mode. */
    SPI_SLAVE  = 0U  /**< SPI peripheral operates in slave mode. */
} spi_master_slave_mode_t;

/**
 * @brief SPI pin (SDO and SDI) configuration.
 */
typedef enum {
    SPI_SDI_IN_SDO_OUT = 0U, /**< SPI SDI input, SDO output. */
    SPI_SDI_IN_OUT     = 1U, /**< SDI is used for both input and output data. */
    SPI_SDO_IN_OUT     = 2U, /**< SDO is used for both input and output data. */
    SPI_SDI_OUT_SDO_IN = 3U  /**< SPI SDO output input, SDI output input. */
} spi_pin_config_t;

/**
 * @brief SPI data output configuration.
 */
typedef enum {
    SPI_DATA_OUT_RETAINED = 0U, /**< Data out retains last value when chip select de-asserted */
    SPI_DATA_OUT_TRISTATE = 1U  /**< Data out is tri-stated when chip select de-asserted */
} spi_data_out_config_t;

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
    spi_transfer_width_t euWidth;     /**< Transfer width, single, 2-bit, or 4-bit transfer. */
    bool txMask;                        /**< Option to mask the transmit data (won't load to FIFO). */
    bool rxMask;                        /**< Option to mask the receive data (won't store in FIFO). */
    bool contCmd;                       /**< Master option to change cmd word within cont transfer. */
    bool contTransfer;                  /**< Master option for continuous transfer. */
    bool byteSwap;                      /**< Option to invoke the byte swap option in the FIFOs. */
    bool lsbFirst;                      /**< Option to transmit LSB first. */
    spi_which_pcs_t euWhichPcs;       /**< Selects which PCS to use. */
    uint32_t preDiv;                    /**< Selects the baud rate prescaler divider TCR bit setting. */
    spi_clock_phase_t euClkPhase;     /**< Selects which phase of clock to capture data. */
    spi_sck_polarity_t euClkPolarity; /**< Selects clock polarity. */
} spi_tx_cmd_config_t;

/**
 * @brief SPI delay type selection
 */
typedef enum {
    SPI_SCK_TO_PCS       = SPI_CCR_SCKPCS_SHIFT, /**< SCK to PCS Delay */
    SPI_PCS_TO_SCK       = SPI_CCR_PCSSCK_SHIFT, /**< PCS to SCK Delay */
    SPI_BETWEEN_TRANSFER = SPI_CCR_DBT_SHIFT     /**< Delay between transfers */
} spi_delay_type_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
/** Defines constant value arrays for the baud rate pre-scalar values.*/
static const uint32_t s_baudratePrescaler[] = {1, 2, 4, 8, 16, 32, 64, 128};

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Resets the SPI internal logic and registers to their default settings.
 *
 * This function first performs a software reset of the SPI module which resets the
 * internal SPI logic and most registers, then proceeds to manually reset all of the
 * SPI registers to their default setting to ensuring these registers at programmed to
 * their default value which includes disabling the module.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 */
void SPI_Init(spi_type_t *pstBase);

/**
 * @brief Enables the SPI module.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 */
static inline void SPI_Enable(spi_type_t *pstBase)
{
    (pstBase->CR) |= (uint32_t)1U << SPI_CR_MEN_SHIFT;
}

/**
 * @brief Disables the SPI module.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @return status_t
 * This function returns STATUS_BUSY if it is detected that the Module Busy Flag
 * (MBF) is set, otherwise, if success, it returns STATUS_SUCCESS.
 */
status_t SPI_Disable(spi_type_t *pstBase);

/**
 * @brief Configures the SPI for master or slave.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  euMode Mode setting (master or slave) of type spi_master_slave_mode_t
 * @return status_t
 * This function returns the error condition STATUS_ERROR if the module is not
- *         disabled else it returns STATUS_SUCCESS.
 */
status_t SPI_SetMasterSlaveMode(spi_type_t *pstBase, spi_master_slave_mode_t euMode);

/**
 * @brief Configuration Data Match
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  pstDataMatchCfg Data Match Configuration
 */
void SPI_SetDataMatch(spi_type_t *pstBase, const data_match_cfg_t *pstDataMatchCfg);

/**
 * @brief Returns whether the SPI module is in master mode.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @return true or false
 */
static inline bool SPI_IsMaster(const spi_type_t *pstBase)
{
    return (bool)((pstBase->CFGR1 >> SPI_CFGR1_MASTER_SHIFT) & 1U);
}

/**
 * @brief Gets FIFO sizes of the SPI module.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  pFifoSize fifoSize The FIFO size passed back to the user
 */
static inline void SPI_GetFifoSizes(const spi_type_t *pstBase, uint8_t *pFifoSize)
{
    if (pFifoSize != NULL) {
        *pFifoSize = (uint8_t)(1U << ((pstBase->PARAM & SPI_PARAM_TXFIFO_MASK) >> SPI_PARAM_TXFIFO_SHIFT));
    }
}

/**
 * @brief Flushes the SPI FIFOs.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  flushTxFifo Flushes (true) the Tx FIFO, else do not flush (false) the Tx FIFO
 * @param  flushRxFifo Flushes (true) the Rx FIFO, else do not flush (false) the Rx FIFO
 */
void SPI_SetFlushFifoCmd(spi_type_t *pstBase, bool flushTxFifo, bool flushRxFifo);

/**
 * @brief Sets the RX FIFO watermark values.
 *
 * This function allows the user to set the RX FIFO watermarks.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  rxWater The RX FIFO watermark value
 */
static inline void SPI_SetRxWatermarks(spi_type_t *pstBase, uint32_t rxWater)
{
    uint32_t spi_tmp = pstBase->FCR;
    spi_tmp &= ~(SPI_FCR_RXWATER_MASK);
    spi_tmp |= (rxWater << SPI_FCR_RXWATER_SHIFT);
    pstBase->FCR = spi_tmp;
}

/**
 * @brief Sets the TX FIFO watermark values.
 *
 * This function allows the user to set the TX FIFO watermarks.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  txWater The TX FIFO watermark value
 */
static inline void SPI_SetTxWatermarks(spi_type_t *pstBase, uint32_t txWater)
{
    uint32_t spi_tmp = pstBase->FCR;
    spi_tmp &= ~(SPI_FCR_TXWATER_MASK);
    spi_tmp |= (txWater << SPI_FCR_TXWATER_SHIFT);
    pstBase->FCR = spi_tmp;
}

/**
 * @brief Gets the SPI status flag state.
 *
 * This function returns the state of one of the SPI status flags as requested by
 * the user.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  euStatusFlag The status flag, of type spi_status_flag_t
 * @return true or false
 * State of the status flag: asserted (true) or not-asserted (false)
 */
static inline bool SPI_GetStatusFlag(const spi_type_t *pstBase, spi_status_flag_t euStatusFlag)
{
    return (bool)(((pstBase->SR) >> (uint8_t)euStatusFlag) & 1U);
}

/**
 * @brief Clears the SPI status flag.
 *
 * This function clears the state of one of the SPI status flags as requested by
 * the user. Note, the flag must be w1c capable, if not the function returns an error.
 * w1c capable flags are:
 *   SPI_WORD_COMPLETE
 *   SPI_FRAME_COMPLETE
 *   SPI_TRANSFER_COMPLETE
 *   SPI_TRANSMIT_ERROR
 *   SPI_RECEIVE_ERROR
 *   SPI_DATA_MATCH
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  euStatusFlag The status flag, of type spi_status_flag_t
 * @return status_t
 * STATUS_SUCCESS or SPI_STATUS_INVALID_PARAMETER
 */
status_t SPI_ClearStatusFlag(spi_type_t *pstBase, spi_status_flag_t euStatusFlag);

/**
 * @brief Configures the SPI interrupts.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  euInterruptSrc The interrupt source, of type spi_status_flag_t
 * @param  enable Enable (true) or disable (false) the interrupt source
 */
static inline void SPI_SetIntMode(spi_type_t *pstBase, spi_status_flag_t euInterruptSrc, bool enable)
{
    if (enable == true) {
        pstBase->IER |= (uint32_t)1U << (uint8_t)euInterruptSrc;
    } else {
        pstBase->IER &= ~((uint32_t)1U << (uint8_t)euInterruptSrc);
    }
}

/**
 * @brief Returns if the SPI interrupt request is enabled or disabled.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  euInterruptSrc The interrupt source, of type spi_status_flag_t
 * @return true or false
 * Returns if the interrupt source is enabled (true) or disabled (false)
 */
static inline bool SPI_GetIntMode(const spi_type_t *pstBase, spi_status_flag_t euInterruptSrc)
{
    return (bool)(((pstBase->IER) >> (uint8_t)euInterruptSrc) & 1U);
}

/**
 * @brief Sets the SPI Transmit Data PDMA configuration (enable or disable).
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  enable Enable (true) or disable (false) the TX PDMA request
 */
static inline void SPI_SetTxDmaCmd(spi_type_t *pstBase, bool enable)
{
    pstBase->DER = (pstBase->DER & (~SPI_DER_TDDE_MASK)) | ((uint32_t)enable << SPI_DER_TDDE_SHIFT);
}

/**
 * @brief Sets the SPI Receive Data PDMA configuration (enable or disable).
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  enable Enable (true) or disable (false) the RX PDMA request
 */
static inline void SPI_SetRxDmaCmd(spi_type_t *pstBase, bool enable)
{
    (pstBase->DER) = (pstBase->DER & (~SPI_DER_RDDE_MASK)) | ((uint32_t)enable << SPI_DER_RDDE_SHIFT);
}

/**
 * @brief Manually configures a specific SPI delay parameter (module must be disabled to
 *        change the delay values).
 *
 * This function configures the:
 * SCK to PCS delay, or
 * PCS to SCK delay, or
 * Between transfer delay.
 *
 * These delay names are available in type spi_delay_type_t.
 *
 * The user passes which delay they want to configure along with the delay value.
 * This allows the user to directly set the delay values if they have
 * pre-calculated them or if they simply wish to manually increment the value.
 *
 * Note that the SPI module must first be disabled before configuring this.
 * Note that the SPI module must be configure for master mode before configuring this.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  euWhichDelay The desired delay to configure, must be of type spi_delay_type_t
 * @param  delay The 8-bit delay value 0x00 to 0xFF (255). The delay is equal to:
 *             -delay + 1 cycles of the SPI baud rate clock (SCK to PCS and PCS to SCK)
 *             -delay + 2 cycles of the SPI baud rate clock (Between transfer delay)
 * @return status_t
 * STATUS_SUCCESS
 */
static inline status_t SPI_SetDelay(spi_type_t *pstBase, spi_delay_type_t euWhichDelay, uint32_t delay)
{
    uint32_t ccrValue = 0;

    ccrValue = pstBase->CCR & ~(0xFFUL << (uint32_t)euWhichDelay);
    ccrValue |= delay << (uint32_t)euWhichDelay;
    pstBase->CCR = ccrValue;
    return STATUS_SUCCESS;
}

/**
 * @brief Configures the desired SPI PCS polarity.
 *
 * This function allows the user to configure the polarity of a particular PCS signal.
 * Note that the SPI module must first be disabled before configuring this.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  euWhichPcs Select which PCS to program, of type spi_which_pcs_t
 * @param  euPcsPolarity Set PCS as active high or low, of type spi_signal_polarity_t
 * @return status_t
 * This function returns the error condition STATUS_ERROR if the module is not
 *         disabled else it returns STATUS_SUCCESS.
 */
status_t SPI_SetPcsPolarityMode(spi_type_t *pstBase, spi_which_pcs_t euWhichPcs,
                                  spi_signal_polarity_t euPcsPolarity);

/**
 * @brief Configures the SPI SDO/SDI pin configuration mode.
 *
 * This function configures the pin mode of the SPI.
 * For the SDI and SDO pins, the user can configure these pins as follows:
 *  SDI is used for input data and SDO for output data.
 *  SDO is used for input data and SDO for output data.
 *  SDI is used for input data and SDI for output data.
 *  SDO is used for input data and SDI for output data.
 *
 * The user has the option to configure the output data as:
 *  Output data retains last value when chip select is de-asserted (default setting).
 *  Output data is trisected when chip select is de-asserted.
 *
 * Finally, the user has the option to configure the PCS[3:2] pins as:
 *  Enabled for PCS operation (default setting).
 *  Disabled - this is need if the user wishes to configure the SPI mode for 4-bit transfers
 *             where these pins will be used as I/O data pins.
 *
 * Note that the SPI module must first be disabled before configuring this.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  euPinCfg Select configuration for the SDO/SDI pins (see spi_pin_config_t)
 * @param  euDataOutConfig Select data output config after chip select de-assertion
 * @param  pcs3and2Enable Enable or disable PCS[3:2]
 * @return status_t
 * This function returns the error condition STATUS_ERROR if the module is not
 *         disabled else it returns STATUS_SUCCESS.
 */
status_t SPI_SetPinConfigMode(spi_type_t *pstBase, spi_pin_config_t euPinCfg,
                                spi_data_out_config_t euDataOutConfig, bool pcs3and2Enable);

/**
 * @brief Sets the SPI baud rate in bits per second.
 *
 * This function takes in the desired bitsPerSec (baud rate) and calculates the nearest
 * possible baud rate without exceeding the desired baud rate, and returns the
 * calculated baud rate in bits-per-second. It requires that the caller also provide
 * the frequency of the module source clock (in Hertz). Also note that the baud rate
 * does not take into affect until the Transmit Control Register (TCR) is programmed
 * with the PRESCALE value. Hence, this function returns the PRESCALE tcrPrescaleValue
 * parameter for later programming in the TCR.  It is up to the higher level
 * peripheral driver to alert the user of an out of range baud rate input.
 *
 * Note that the SPI module must first be disabled before configuring this.
 * Note that the SPI module must be configure for master mode before configuring this.
 *
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  bitsPerSec The desired baud rate in bits per second
 * @param  sourceClockInHz Module source input clock in Hertz
 * @param  tcrPrescaleValue The TCR PRESCALE value, needed by user to program the TCR
 * @return uint32_t
 * The actual calculated baud rate. This function may also return a "0" if the
 *          SPI is not configured for master mode or if the SPI module is not disabled.
 */
uint32_t SPI_SetBaudRate(spi_type_t *pstBase, uint32_t bitsPerSec, uint32_t sourceClockInHz,
                           uint32_t *tcrPrescaleValue);

/**
 * @brief Configures the baud rate divisor manually (only the SPI_CCR[SCKDIV]).
 *
 * This function allows the caller to manually set the baud rate divisor in the event
 * that this divider is known and the caller does not wish to call the
 * SPI_SetBaudRate function. Note that this only affects the SPI_CCR[SCKDIV]).
 * The Transmit Control Register (TCR) is programmed separately with the PRESCALE value.
 * The valid range is 0x00 to 0xFF, if the user inputs outside of this range, an error
 * is returned.
 *
 * Note that the SPI module must first be disabled before configuring this.
 * Note that the SPI module must be configure for master mode before configuring this.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  divisor Desired baud rate divisor setting (0x00 to 0xFF)
 * @return status_t
 * STATUS_SUCCESS or SPI_STATUS_OUT_OF_RANGE if divisor > 0xFF
 */
status_t SPI_SetBaudRateDivisor(spi_type_t *pstBase, uint32_t divisor);

/**
 * @brief Sets the PCS flag to the value of the euWhichPcs parameter.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  euWhichPcs Desired chip
 */
void SPI_SetPcs(spi_type_t *pstBase, spi_which_pcs_t euWhichPcs);

/**
 * @brief Refill the Transmit Command Register (TCR) parameters.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 */
void SPI_RefillTCRTxmskReg(spi_type_t *pstBase);

/**
 * @brief Sets the Transmit Command Register (TCR) parameters.
 *
 * The Transmit Command Register (TCR) contains multiple parameters that affect
 * the transmission of data, such as clock phase and polarity, which PCS to use,
 * whether or not the PCS remains asserted at the completion of a frame, etc.
 * Any writes to this register results in an immediate push of the entire register
 * and its contents to the TX FIFO.  Hence, writes to this register should include
 * all of the desired parameters written to the register at once. Hence, the user
 * should fill in the members of the spi_tx_cmd_config_t data structure and pass
 * this to the function.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  pstTxCmdCfgSet Structure that contains the Transmit Command Register (TCR)
 *                    settings of type spi_tx_cmd_config_t
 */
void SPI_SetTxCommandReg(spi_type_t *pstBase, const spi_tx_cmd_config_t *pstTxCmdCfgSet);

/**
 * @brief Gets the Transmit Command Register (TCR) parameters.
 *
 * The Transmit Command Register (TCR) contains multiple parameters that affect
 * the transmission of data, such as clock phase and polarity, which PCS to use,
 * whether or not the PCS remains asserted at the completion of a frame, etc.
 * Any writes to this register results in an immediate push of the entire register
 * and its contents to the TX FIFO.  Hence, writes to this register should include
 * all of the desired parameters written to the register at once. Hence, the user
 * should fill in the members of the spi_tx_cmd_config_t data structure and pass
 * this to the function.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  pstTxCmdCfgSet Structure that contains the Transmit Command Register (TCR)
 *                    settings of type spi_tx_cmd_config_t
 */
void SPI_GetTxCommandReg(spi_type_t *pstBase, spi_tx_cmd_config_t *pstTxCmdCfgSet);

/**
 * @brief Writes data into the TX data buffer.
 *
 * This function writes data passed in by the user to the Transmit Data Register (TDR).
 * The user can pass up to 32-bits of data to load into the TDR. If the frame size exceeds 32-bits,
 * the user will have to manage sending the data one 32-bit word at a time.
 * Any writes to the TDR will result in an immediate push to the TX FIFO.
 * This function can be used for either master or slave mode.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  data The data word to be sent
 */
static inline void SPI_WriteData(spi_type_t *pstBase, uint32_t data)
{
    pstBase->TDR = data;
}

/**
 * @brief Reads data from the data buffer.
 *
 * This function reads the data from the Receive Data Register (RDR).
 * This function can be used for either master or slave mode.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @return uint32_t
 * The data read from the data buffer
 */
static inline uint32_t SPI_ReadData(const spi_type_t *pstBase)
{
    return (uint32_t)pstBase->RDR;
}

/**
 * @brief Reads TX COUNT form the FIFO Status Register.
 *
 * This function reads the TX COUNT field  from the FIFO Status Register (FSR).
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @return uint32_t
 * The data read from the FIFO Status Register
 */
static inline uint32_t SPI_ReadTxCount(const spi_type_t *pstBase)
{
    return (uint32_t)(((uint32_t)(pstBase->FSR & SPI_FSR_TXCOUNT_MASK)) >> SPI_FSR_TXCOUNT_SHIFT);
}

/**
 * @brief Reads RX COUNT form the FIFO Status Register.
 *
 * This function reads the RX COUNT field  from the FIFO Status Register (FSR).
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @return uint32_t
 * The data read from the FIFO Status Register
 */
static inline uint32_t SPI_ReadRxCount(const spi_type_t *pstBase)
{
    return (uint32_t)((((uint32_t)pstBase->FSR & (uint32_t)SPI_FSR_RXCOUNT_MASK)) >>
                      (uint32_t)SPI_FSR_RXCOUNT_SHIFT);
}

/**
 * @brief Clear RXMSK bit form TCR Register.
 *
 * This function clears the RXMSK bit from the Transmit Command Register (TCR).
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 */
static inline void SPI_ClearRxmaskBit(spi_type_t *pstBase)
{
    (pstBase->TCR) = ((pstBase->TCR) & (~SPI_TCR_RXMSK_MASK));
}

/**
 * @brief Set RXMSK bit form TCR Register.
 *
 * This function set the RXMSK bit from the Transmit Command Register (TCR).
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 */
static inline void SPI_SetRxmskBit(spi_type_t *pstBase)
{
    (pstBase->TCR) = ((pstBase->TCR) | (SPI_TCR_RXMSK_MASK));
}

/**
 * @brief Clear TXMSK bit form TCR Register.
 *
 * This function clears the TXMSK bit from the Transmit Command Register (TCR).
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 */
static inline void SPI_ClearTxMaskBit(spi_type_t *pstBase)
{
    (pstBase->TCR) = ((pstBase->TCR) & (~SPI_TCR_TXMSK_MASK));
}

/**
 * @brief Set TXMSK bit form TCR Register.
 *
 * This function set the TXMSK bit from the Transmit Command Register (TCR).
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 */
static inline void SPI_SetTxmskBit(spi_type_t *pstBase)
{
    (pstBase->TCR) = ((pstBase->TCR) | (SPI_TCR_TXMSK_MASK));
}

/**
 * @brief Clear CONTC bit form TCR Register.
 *
 * This function clears the CONTC bit from the Transmit Command Register (TCR).
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 */
static inline void SPI_ClearContCBit(spi_type_t *pstBase)
{
    (pstBase->TCR) = ((pstBase->TCR) & (~SPI_TCR_CONTC_MASK));
}

/**
 * @brief Set CONTC bit form TCR Register.
 *
 * This function set the CONTC bit from the Transmit Command Register (TCR).
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 */
static inline void SPI_SetContCBit(spi_type_t *pstBase)
{
    (pstBase->TCR) = ((pstBase->TCR) | (SPI_TCR_CONTC_MASK));
}

/**
 * @brief Configures the clock prescaler used for all SPI master logic.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  euPrescaler Prescaler value for master logic.
 */
static inline void SPI_SetClockPrescaler(spi_type_t *pstBase, spi_prescaler_t euPrescaler)
{
    uint32_t spi_tmp = pstBase->TCR;
    spi_tmp &= ~(SPI_TCR_PRESCALE_MASK);
    spi_tmp |= ((uint32_t)euPrescaler << SPI_TCR_PRESCALE_SHIFT);
    pstBase->TCR = spi_tmp;
}

/**
 * @brief Get the clock prescaler used for all SPI master logic.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @return spi_prescaler_t
 * Prescaler value for master logic.
 */
static inline spi_prescaler_t SPI_GetClockPrescaler(const spi_type_t *pstBase)
{
    return (spi_prescaler_t)(((uint32_t)((pstBase->TCR) & SPI_TCR_PRESCALE_MASK)) >> SPI_TCR_PRESCALE_SHIFT);
}

/**
 * @brief Configures if the sample point for master devices is delayed.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  isSamplingPointDelayed Configure if the sampling point is delayed for master devices
 */
static inline void SPI_SetSamplingPoint(spi_type_t *pstBase, bool isSamplingPointDelayed)
{
    uint32_t spi_tmp = pstBase->CFGR1;
    spi_tmp &= ~(SPI_CFGR1_SAMPLE_MASK);
    spi_tmp |= ((uint32_t)isSamplingPointDelayed << SPI_CFGR1_SAMPLE_SHIFT);
    pstBase->CFGR1 = spi_tmp;
}

/**
 * @brief Configures if enable Host Request.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  triggerEnable Enable the trigger
 */
void SPI_SetTriggerEnable(spi_type_t *pstBase, bool triggerEnable);

/**
 * @brief Configures Host Request select.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  selectType Select the source of the Host request
 */
void SPI_SetTriggerSelect(spi_type_t *pstBase, uint32_t selectType);

/**
 * @brief Configures  Polarity of Host Request.
 *
 * @param  pstBase Module pstBase pointer of type spi_type_t.
 * @param  polarity Configure Polarity of Host request
 */
void SPI_SetTriggerPolarity(spi_type_t *pstBase, bool polarity);

#if defined(__cplusplus)
}
#endif

#endif //__SPI_HW_ACCESS_H__
