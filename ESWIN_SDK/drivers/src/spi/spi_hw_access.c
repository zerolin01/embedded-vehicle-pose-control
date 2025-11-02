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
 * @file spi_hw_access.c
 * @brief SPI driver hardware access source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:        Version:        Author:
 * Changes:
 *
 */
#include "spi_hw_access.h"

/**
 * 
 * Function Name : SPI_Init
 * Description   : Resets the SPI internal logic and registers to their default settings.
 *
 * This function first performs a software reset of the SPI module which resets the
 * internal SPI logic and most registers, then proceeds to manually reset all of the
 * SPI registers to their default setting to ensuring these registers at programmed to
 * their default value which includes disabling the module.
 *
 */
void SPI_Init(spi_type_t *pstBase)
{
    /* Software reset the internal logic */
    pstBase->CR = SPI_CR_RST_MASK;
    /* Now bring the SPI module out of reset and clear CR register */
    pstBase->CR = (uint32_t)0x0;
}

/**
 * 
 * Function Name : SPI_Disable
 * Description   : Disables the SPI module.
 *
 * @return status_t
 * This function returns STATUS_BUSY if it is detected that the Module Busy Flag
 * (MBF) is set, otherwise, if success, it returns STATUS_SUCCESS.
 */
status_t SPI_Disable(spi_type_t *pstBase)
{
    uint32_t spi_tmp = pstBase->SR;
    spi_tmp          = (spi_tmp & SPI_SR_MBF_MASK) >> SPI_SR_MBF_SHIFT;

    if (spi_tmp == (uint32_t)1) {
        return STATUS_BUSY;
    } else {
        pstBase->CR = pstBase->CR & (~(SPI_CR_MEN_MASK));
        return STATUS_SUCCESS;
    }
}

/**
 * 
 * Function Name : SPI_SetMasterSlaveMode
 * Description   : Configures the SPI for master or slave.
 *
 * @return status_t
 * This function returns the error condition STATUS_ERROR if the module is not
 *         disabled else it returns STATUS_SUCCESS.
 * Note that the SPI module must first be disabled before configuring this.
 *
 */
status_t SPI_SetMasterSlaveMode(spi_type_t *pstBase, spi_master_slave_mode_t euMode)
{
    pstBase->CFGR1 = (pstBase->CFGR1 & (~SPI_CFGR1_MASTER_MASK)) | ((uint32_t)euMode << SPI_CFGR1_MASTER_SHIFT);
    return STATUS_SUCCESS;
}

/**
 * 
 * Function Name : SPI_SetFlushFifoCmd
 * Description   : Flushes the SPI FIFOs.
 *
 */
void SPI_SetFlushFifoCmd(spi_type_t *pstBase, bool flushTxFifo, bool flushRxFifo)
{
    uint32_t crValue = 0;

    crValue = ((uint32_t)flushTxFifo << SPI_CR_RTF_SHIFT) | ((uint32_t)flushRxFifo << SPI_CR_RRF_SHIFT);

    pstBase->CR |= crValue;
}

/**
 * 
 * Function Name : SPI_ClearStatusFlag
 * Description   : Clears the SPI status flag.
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
 * @return status_t
 * STATUS_SUCCESS or SPI_STATUS_INVALID_PARAMETER
 */
status_t SPI_ClearStatusFlag(spi_type_t *pstBase, spi_status_flag_t euStatusFlag)
{
    if (euStatusFlag == SPI_ALL_STATUS) {
        pstBase->SR |= (uint32_t)SPI_ALL_STATUS;
    } else {
        pstBase->SR |= ((uint32_t)1U << (uint32_t)euStatusFlag);
    }
    return STATUS_SUCCESS;
}

/**
 * 
 * Function Name : SPI_SetPcsPolarityMode
 * Description   : Configures the desired SPI PCS polarity.
 *
 * This function allows the user to configure the polarity of a particular PCS signal.
 * Note that the SPI module must first be disabled before configuring this.
 *
 * @return status_t
 * This function returns the error condition STATUS_ERROR if the module is not
 *         disabled else it returns STATUS_SUCCESS.
 */
status_t SPI_SetPcsPolarityMode(spi_type_t *pstBase, spi_which_pcs_t euWhichPcs,
                                  spi_signal_polarity_t euPcsPolarity)
{
    uint32_t cfgr1Value = 0;

    /* Clear the PCS polarity bit */
    cfgr1Value = (pstBase->CFGR1) & (~((uint32_t)1U << (SPI_CFGR1_PCSPOL_SHIFT + (uint32_t)euWhichPcs)));

    /* Configure the PCS polarity bit according to the euPcsPolarity setting */
    cfgr1Value |= (uint32_t)euPcsPolarity << (SPI_CFGR1_PCSPOL_SHIFT + (uint32_t)euWhichPcs);

    pstBase->CFGR1 = cfgr1Value;

    return STATUS_SUCCESS;
}

/**
 * 
 * Function Name : SPI_SetPinConfigMode
 * Description   : Configures the SPI SDO/SDI pin configuration mode.
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
 * @return status_t
 * This function returns the error condition STATUS_ERROR if the module is not
 *         disabled else it returns STATUS_SUCCESS.
 */
status_t SPI_SetPinConfigMode(spi_type_t *pstBase, spi_pin_config_t euPinCfg,
                                spi_data_out_config_t euDataOutConfig, bool pcs3and2Enable)
{
    uint32_t cfgr1Value = 0;

    cfgr1Value = pstBase->CFGR1 & ~(SPI_CFGR1_PINCFG_MASK | SPI_CFGR1_OUTCFG_MASK | SPI_CFGR1_PCSCFG_MASK);

    cfgr1Value |= ((uint32_t)(euPinCfg) << SPI_CFGR1_PINCFG_SHIFT) |
                  ((uint32_t)(euDataOutConfig) << SPI_CFGR1_OUTCFG_SHIFT) |
                  ((uint32_t)(!pcs3and2Enable) << SPI_CFGR1_PCSCFG_SHIFT); // enable = 0

    pstBase->CFGR1 = cfgr1Value;

    return STATUS_SUCCESS;
}

/**
 * 
 * Function Name : SPI_AbsDif
 * Description   : This is a helper function which implements absolute difference between
 * two numbers.
 *
 * @param  a input
 * @param  b input
 * @return uint32_t
 */
static uint32_t SPI_AbsDif(uint32_t a, uint32_t b)
{
    if (a > b) {
        return (a - b);
    } else {
        return (b - a);
    }
}

/**
 * 
 * Function Name : SPI_SetBaudRate
 * Description   : Sets the SPI baud rate in bits per second.
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
 * @return uint32_t
 * The actual calculated baud rate. This function may also return a "0" if the
 *          SPI is not configured for master mode or if the SPI module is not disabled.
 */
uint32_t SPI_SetBaudRate(spi_type_t *pstBase, uint32_t bitsPerSec, uint32_t sourceClockInHz,
                           uint32_t *tcrPrescaleValue)
{

    uint32_t bestFreq      = 0xFFFFFFFFU;
    uint32_t bestScaler    = 0U;
    uint32_t bestPrescaler = 0U;
    uint32_t freq1         = 0U;
    uint32_t freq2         = 0U;
    uint8_t scaler         = 0U;
    uint8_t prescaler      = 0U;
    uint32_t low, high;
    uint32_t tempBestFreq   = 0U;
    uint32_t tempBestScaler = 0U;

    for (prescaler = 0; prescaler < 8U; prescaler++) {
        low  = 0U;
        high = 256U;

        /* Implement golden section search algorithm */
        do {
            scaler = (uint8_t)((low + high) / 2U);
            freq1  = sourceClockInHz / (s_baudratePrescaler[prescaler] * (scaler + (uint32_t)2U));

            if (SPI_AbsDif(bitsPerSec, bestFreq) > SPI_AbsDif(bitsPerSec, freq1)) {
                bestFreq = freq1;
            }
            if (freq1 < bitsPerSec) {
                high = scaler;
            } else {
                low = scaler;
            }
        } while ((high - low) > 1U);

        /* Evaluate last 2 scaler values */
        freq1 = sourceClockInHz / (s_baudratePrescaler[prescaler] * (low + (uint32_t)2U));
        freq2 = sourceClockInHz / (s_baudratePrescaler[prescaler] * (high + (uint32_t)2U));

        if (SPI_AbsDif(bitsPerSec, freq1) > SPI_AbsDif(bitsPerSec, freq2)) {
            tempBestFreq   = freq2;
            tempBestScaler = high;
        } else {
            tempBestFreq   = freq1;
            tempBestScaler = low;
        }

        if (SPI_AbsDif(bitsPerSec, bestFreq) >= SPI_AbsDif(bitsPerSec, tempBestFreq)) {
            bestFreq      = tempBestFreq;
            bestScaler    = tempBestScaler;
            bestPrescaler = prescaler;
        }

        /* If current frequency is equal to target frequency  stop the search */
        if (bestFreq == bitsPerSec) {
            break;
        }
    }

    /* Add default values for delay between transfers, delay between sck to pcs and between pcs to sck. */
    (void)SPI_SetDelay(pstBase, SPI_SCK_TO_PCS, bestScaler >> 2U);
    (void)SPI_SetDelay(pstBase, SPI_PCS_TO_SCK, bestScaler >> 2U);
    (void)SPI_SetDelay(pstBase, SPI_BETWEEN_TRANSFER, bestScaler >> 2U);

    /* Write the best baud rate scalar to the CCR.
     * Note, no need to check for error since we've already checked to make sure the module is
     * disabled and in master mode. Also, there is a limit on the maximum divider so we will not
     * exceed this.
     */
    (void)SPI_SetBaudRateDivisor(pstBase, bestScaler);

    /* return the best prescaler value for user to use later */
    *tcrPrescaleValue = bestPrescaler;

    /* return the actual calculated baud rate */
    return bestFreq;
}

/**
 * 
 * Function Name : SPI_SetBaudRateDivisor
 * Description   : Configures the baud rate divisor manually (only the SPI_CCR[SCKDIV]).
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
 * @return status_t
 * STATUS_SUCCESS or SPI_STATUS_OUT_OF_RANGE if divisor > 0xFF
 */
status_t SPI_SetBaudRateDivisor(spi_type_t *pstBase, uint32_t divisor)
{
    uint32_t spi_tmp;

    spi_tmp = pstBase->CCR;
    spi_tmp &= ~(SPI_CCR_SCKDIV_MASK);
    spi_tmp |= SPI_CCR_SCKDIV(divisor);
    pstBase->CCR = spi_tmp;

    return STATUS_SUCCESS;
}

/**
 * 
 * Function Name : SPI_RefillTCRTxmskReg
 * Description   : Refill the Transmit Command Register (TCR) parameters.
 *
 * Refill for clk out when txmsk set
 *
 */
void SPI_RefillTCRTxmskReg(spi_type_t *pstBase)
{
    uint32_t reg = pstBase->TCR;
    reg |= SPI_TCR_TXMSK_MASK;
    pstBase->TCR = reg;
    return;
}

/**
 * 
 * Function Name : SPI_SetTxCommandReg
 * Description   : Sets the Transmit Command Register (TCR) parameters.
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
 */
void SPI_SetTxCommandReg(spi_type_t *pstBase, const spi_tx_cmd_config_t *pstTxCmdCfgSet)
{
    pstBase->TCR = (((uint32_t)pstTxCmdCfgSet->euClkPolarity << SPI_TCR_CPOL_SHIFT) |
                    ((uint32_t)pstTxCmdCfgSet->euClkPhase << SPI_TCR_CPHA_SHIFT) |
                    ((uint32_t)pstTxCmdCfgSet->preDiv << SPI_TCR_PRESCALE_SHIFT) |
                    ((uint32_t)pstTxCmdCfgSet->euWhichPcs << SPI_TCR_PCS_SHIFT) |
                    ((uint32_t)pstTxCmdCfgSet->lsbFirst << SPI_TCR_LSBF_SHIFT) |
                    ((uint32_t)pstTxCmdCfgSet->byteSwap << SPI_TCR_BYSW_SHIFT) |
                    ((uint32_t)pstTxCmdCfgSet->contTransfer << SPI_TCR_CONT_SHIFT) |
                    ((uint32_t)pstTxCmdCfgSet->contCmd << SPI_TCR_CONTC_SHIFT) |
                    ((uint32_t)pstTxCmdCfgSet->rxMask << SPI_TCR_RXMSK_SHIFT) |
                    ((uint32_t)pstTxCmdCfgSet->txMask << SPI_TCR_TXMSK_SHIFT) |
                    ((uint32_t)pstTxCmdCfgSet->euWidth << SPI_TCR_WIDTH_SHIFT) |
                    ((uint32_t)(pstTxCmdCfgSet->frameSize - 1UL) << SPI_TCR_FRAMESZ_SHIFT));
}

/**
 * 
 * Function Name : SPI_GetTxCommandReg
 * Description   : Gets the Transmit Command Register (TCR) parameters.
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
 */
void SPI_GetTxCommandReg(spi_type_t *pstBase, spi_tx_cmd_config_t *pstTxCmdCfgSet)
{
    volatile uint32_t reg = pstBase->TCR;
    pstTxCmdCfgSet->euClkPolarity = (spi_sck_polarity_t)((reg & SPI_TCR_CPOL_MASK) >> SPI_TCR_CPOL_SHIFT);
    pstTxCmdCfgSet->euClkPhase = (spi_clock_phase_t)((reg & SPI_TCR_CPHA_MASK) >> SPI_TCR_CPHA_SHIFT);
    pstTxCmdCfgSet->preDiv = (reg & SPI_TCR_PRESCALE_MASK) >> SPI_TCR_PRESCALE_SHIFT;
    pstTxCmdCfgSet->euWhichPcs = (spi_which_pcs_t)((reg & SPI_TCR_PCS_MASK) >> SPI_TCR_PCS_SHIFT);
    pstTxCmdCfgSet->lsbFirst = (reg & SPI_TCR_LSBF_MASK) >> SPI_TCR_LSBF_SHIFT;
    pstTxCmdCfgSet->byteSwap = (reg & SPI_TCR_BYSW_MASK) >> SPI_TCR_BYSW_SHIFT;
    pstTxCmdCfgSet->contTransfer = (reg & SPI_TCR_CONT_MASK) >> SPI_TCR_CONT_SHIFT;
    pstTxCmdCfgSet->contCmd = (reg & SPI_TCR_CONTC_MASK) >> SPI_TCR_CONTC_SHIFT;
    pstTxCmdCfgSet->rxMask = (reg & SPI_TCR_RXMSK_MASK) >> SPI_TCR_RXMSK_SHIFT;
    pstTxCmdCfgSet->txMask = (reg & SPI_TCR_TXMSK_MASK) >> SPI_TCR_TXMSK_SHIFT;
    pstTxCmdCfgSet->euWidth = (spi_transfer_width_t)((reg & SPI_TCR_WIDTH_MASK) >> SPI_TCR_WIDTH_SHIFT);
    pstTxCmdCfgSet->frameSize = ((reg & SPI_TCR_FRAMESZ_MASK) >> SPI_TCR_FRAMESZ_SHIFT) + 1;
}

/**
 * 
 * Function Name : SPI_SetPcs
 * Description   : Sets the PCS flag to the value of the euWhichPcs parameter.
 *
 */
void SPI_SetPcs(spi_type_t *pstBase, spi_which_pcs_t euWhichPcs)
{
    uint32_t regVal;

    regVal = pstBase->TCR;
    regVal &= (uint32_t)(~(SPI_TCR_PCS_MASK));
    regVal |= (uint32_t)((uint32_t)euWhichPcs << SPI_TCR_PCS_SHIFT);
    pstBase->TCR = regVal;
}

/**
 * 
 * Function Name : SPI_SetDataMatch
 * Description   : Configuration Data Match
 *
 */
void SPI_SetDataMatch(spi_type_t *pstBase, const data_match_cfg_t *pstDataMatchCfg)
{
    uint32_t regVal;
    pstBase->DMR0 = pstDataMatchCfg->data0;
    pstBase->DMR1 = pstDataMatchCfg->data1;
    regVal        = pstBase->CFGR0;
    regVal &= (uint32_t)(~SPI_CFGR0_RDMO_MASK);
    if (pstDataMatchCfg->receiveDataMatchOnly) {
        regVal |= (uint32_t)((uint32_t)1 << SPI_CFGR0_RDMO_SHIFT);
    } else {
        regVal |= (uint32_t)((uint32_t)0 << SPI_CFGR0_RDMO_SHIFT);
    }
    pstBase->CFGR0 = regVal;

    regVal = pstBase->CFGR1;
    regVal &= (uint32_t)(~SPI_CFGR1_MATCFG_MASK);
    regVal |= (uint32_t)((uint32_t)pstDataMatchCfg->dataMatchMode << SPI_CFGR1_MATCFG_SHIFT);
    pstBase->CFGR1 = regVal;
    return;
}

/**
 * 
 * Function Name : SPI_SetTriggerEnable
 * Description   : Configures if enable Host Request.
 *
 */
void SPI_SetTriggerEnable(spi_type_t *pstBase, bool triggerEnable)
{
    uint32_t regVal;

    regVal = pstBase->CFGR0;
    regVal &= (uint32_t)(~(SPI_CFGR0_HREN_MASK));
    regVal |= (uint32_t)((uint32_t)triggerEnable << SPI_CFGR0_HREN_SHIFT);
    pstBase->CFGR0 = regVal;
}

/**
 * 
 * Function Name : SPI_SetTriggerSelect
 * Description   : Configures Host Request select.
 *
 */
void SPI_SetTriggerSelect(spi_type_t *pstBase, uint32_t selectType)
{
    uint32_t regVal;

    regVal = pstBase->CFGR0;
    regVal &= (uint32_t)(~(SPI_CFGR0_HRSEL_MASK));
    regVal |= (uint32_t)((uint32_t)selectType << SPI_CFGR0_HRSEL_SHIFT);
    pstBase->CFGR0 = regVal;
}

/**
 * 
 * Function Name : SPI_SetTriggerPolarity
 * Description   : Configures  Polarity of Host Request.
 *
 */
void SPI_SetTriggerPolarity(spi_type_t *pstBase, bool polarity)
{
    uint32_t regVal;

    regVal = pstBase->CFGR0;
    regVal &= (uint32_t)(~(SPI_CFGR0_HRPOL_MASK));
    regVal |= (uint32_t)((uint32_t)polarity << SPI_CFGR0_HRPOL_SHIFT);
    pstBase->CFGR0 = regVal;
}
