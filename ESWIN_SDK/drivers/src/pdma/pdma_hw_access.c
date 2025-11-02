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
 * @file pdma_hw_access.c
 * @brief PDMA driver hardware access source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "pdma_hw_access.h"

/*
 *
 * Function Name : PDMA_ErrorIRQHandler.
 * Description   : PDMA Error interrupt handling function.
 * 
 */
void PDMA_ErrorIRQHandler(void *data)
{
    uint32_t instance              = (size_t)data;
    const pdma_type_t *pdmaRegBase = PDMA_GetDmaRegBaseAddr(instance);
    uint32_t error                 = PDMA_GetErrorIntStatusFlag(pdmaRegBase);
    uint8_t virtualChannel         = 0U;

    for (virtualChannel = 0U; virtualChannel < DMA_VIRTUAL_CHANNELS; virtualChannel++) {
        if ((error & PDMA_ERR_LSB_MASK) != 0UL) {
            PDMA_DRV_ErrorIRQHandler(virtualChannel);
        }
        error = error >> 1U;
    }
}

/*
 *
 * Function Name : PDMA_Init.
 * Description   : Initializes pDMA module to known state.
 * 
 */
void PDMA_Init(pdma_type_t *base)
{
    uint8_t i;
    /* Clear the bit of CR register */

    uint32_t regValTemp;
    regValTemp = base->CR;
    regValTemp &= ~(DMA_CR_CLM_MASK);
    regValTemp &= ~(DMA_CR_CX_MASK);
    regValTemp &= ~(DMA_CR_ECX_MASK);
    regValTemp &= ~(DMA_CR_EDBG_MASK);
    regValTemp &= ~(DMA_CR_EMLM_MASK);
    regValTemp &= ~(DMA_CR_ERCA_MASK);
    base->CR = regValTemp;

    for (i = 0; i < DMA_CHANNELS; i++) {
        PDMA_TCDClearReg(base, i);
    }
}

/*
 *
 * Function Name : PDMA_CancelTransfer.
 * Description   : Cancels the remaining data transfer
 * 
 */
void PDMA_CancelTransfer(pdma_type_t *base)
{
    uint32_t regValTemp;

    regValTemp = base->CR;
    regValTemp &= ~(DMA_CR_CX_MASK);
    regValTemp |= DMA_CR_CX(1U);
    base->CR = regValTemp;
    while (((base->CR & DMA_CR_CX_MASK) >> DMA_CR_CX_SHIFT) != 0UL) {
    }
}

/*
 *
 * Function Name : PDMA_CancelTransferWithError.
 * Description   : Cancels the remaining data transfer
 * 
 */
void PDMA_CancelTransferWithError(pdma_type_t *base)
{
    uint32_t regValTemp;

    regValTemp = base->CR;
    regValTemp &= ~(DMA_CR_ECX_MASK);
    regValTemp |= DMA_CR_ECX(1U);
    base->CR = regValTemp;
    while (((base->CR & DMA_CR_ECX_MASK) >> DMA_CR_ECX_SHIFT) != 0UL) {
    }
}

/*
 *
 * Function Name : PDMA_SetErrorIntCmd.
 * Description   : Enables/Disables the error interrupt for channels.
 * 
 */
void PDMA_SetErrorIntCmd(pdma_type_t *base, uint8_t channel, bool enable)
{
    if (enable) {
        base->SEEI = channel;
    } else {
        base->CEEI = channel;
    }
}

/*
 *
 * Function Name : PDMA_SetDmaRequestCmd.
 * Description   :Enables/Disables the PDMA request for the channel or all channels.
 * 
 */
void PDMA_SetDmaRequestCmd(pdma_type_t *base, uint8_t channel, bool enable)
{
    if (enable) {
        base->SERQ = channel;
    } else {
        base->CERQ = channel;
    }
}

/*
 *
 * Function Name : PDMA_SetGroupPriority.
 * Description   : Configures PDMA group priorities.
 * 
 */
void PDMA_SetGroupPriority(pdma_type_t *base, pdma_group_priority_t priority)
{
    uint32_t regValTemp;
    uint32_t mask = DMA_CR_GRP0PRI_MASK | DMA_CR_GRP1PRI_MASK;
    regValTemp    = base->CR;
    regValTemp &= ~mask;
    if (PDMA_GRP0_PRIO_HIGH_GRP1_PRIO_LOW == priority) {
        regValTemp |= DMA_CR_GRP0PRI_MASK;
    } else {
        regValTemp |= DMA_CR_GRP1PRI_MASK;
    }

    base->CR = regValTemp;
}

/*
 *
 * Function Name : PDMA_TCDSetNbytes.
 * Description   : Configures the nbytes for the pDMA channel.
 * 
 */
void PDMA_TCDSetNbytes(pdma_type_t *base, uint8_t channel, uint32_t nbytes)
{
    OS_ASSERT(channel < DMA_CHANNELS);

    if (0UL != ((base->CR & DMA_CR_EMLM_MASK) >> DMA_CR_EMLM_SHIFT)) {
        bool mlOffNo = false;
        if (((base->TCD[channel].NBYTES.MLOFFNO & DMA_TCD_NBYTES_MLOFFNO_SMLOE_MASK) >>
             DMA_TCD_NBYTES_MLOFFNO_SMLOE_SHIFT) == 0UL) {
            if (((base->TCD[channel].NBYTES.MLOFFNO & DMA_TCD_NBYTES_MLOFFNO_DMLOE_MASK) >>
                 DMA_TCD_NBYTES_MLOFFNO_DMLOE_SHIFT) == 0UL) {
                base->TCD[channel].NBYTES.MLOFFNO = (nbytes & DMA_TCD_NBYTES_MLOFFNO_NBYTES_MASK);
                mlOffNo                           = true;
            }
        }
        if (!mlOffNo) {
            uint32_t regValTemp;
            regValTemp = base->TCD[channel].NBYTES.MLOFFYES;
            regValTemp &= ~(DMA_TCD_NBYTES_MLOFFYES_NBYTES_MASK);
            regValTemp |= DMA_TCD_NBYTES_MLOFFYES_NBYTES(nbytes);
            base->TCD[channel].NBYTES.MLOFFYES = regValTemp;
        }
    } else {
        base->TCD[channel].NBYTES.MLNO = nbytes;
    }
}

/*
 *
 * Function Name : PDMA_TCDClearReg.
 * Description   :  Clears all registers to 0 for the hardware TCD.
 * 
 */
void PDMA_TCDClearReg(pdma_type_t *base, uint8_t channel)
{

    base->TCD[channel].NBYTES.MLNO   = 0U;
    base->TCD[channel].SADDR         = 0U;
    base->TCD[channel].SOFF          = 0;
    base->TCD[channel].ATTR          = 0U;
    base->TCD[channel].SLAST         = 0;
    base->TCD[channel].DADDR         = 0U;
    base->TCD[channel].DOFF          = 0;
    base->TCD[channel].CITER.ELINKNO = 0U;
    base->TCD[channel].DLASTSGA      = 0;
    base->TCD[channel].CSR           = 0U;
    base->TCD[channel].BITER.ELINKNO = 0U;
}

/*
 *
 * Function Name : PDMA_TCDSetAttribute.
 * Description   : Configures the transfer attribute for the pDMA channel.
 * 
 */
void PDMA_TCDSetAttribute(pdma_type_t *base, uint8_t channel, pdma_modulo_t srcModulo, pdma_modulo_t destModulo,
                          pdma_transfer_size_t srcTransferSize, pdma_transfer_size_t destTransferSize)
{
    uint16_t regValTemp;
    regValTemp = (uint16_t)(DMA_TCD_ATTR_SMOD(srcModulo) | DMA_TCD_ATTR_SSIZE(srcTransferSize));
    regValTemp |= (uint16_t)(DMA_TCD_ATTR_DMOD(destModulo) | DMA_TCD_ATTR_DSIZE(destTransferSize));
    base->TCD[channel].ATTR = regValTemp;
}

/*
 *
 * Function Name : PDMA_TCDSetMinorLoopOffset.
 * Description   : Configures the minor loop offset for the TCD.
 * 
 * Configures the offset value. If neither source nor destination offset is enabled,
 * offset is not configured.
 * NOTE: EMLM bit needs to be enabled prior to calling this function, otherwise
 * it has no effect.
 * 
 */
void PDMA_TCDSetMinorLoopOffset(pdma_type_t *base, uint8_t channel, int32_t offset)
{
    OS_ASSERT(channel < DMA_CHANNELS);

    if (0UL != ((base->CR & DMA_CR_EMLM_MASK) >> DMA_CR_EMLM_SHIFT)) {
        bool mlOffNo = false;
        if (0UL != ((base->TCD[channel].NBYTES.MLOFFNO & DMA_TCD_NBYTES_MLOFFNO_SMLOE_MASK) >>
                    DMA_TCD_NBYTES_MLOFFNO_SMLOE_SHIFT)) {
            mlOffNo = true;
        }
        if (0UL != ((base->TCD[channel].NBYTES.MLOFFNO & DMA_TCD_NBYTES_MLOFFNO_DMLOE_MASK) >>
                    DMA_TCD_NBYTES_MLOFFNO_DMLOE_SHIFT)) {
            mlOffNo = true;
        }
        if (mlOffNo) {
            uint32_t regValTemp;
            regValTemp = base->TCD[channel].NBYTES.MLOFFYES;
            regValTemp &= ~(DMA_TCD_NBYTES_MLOFFYES_MLOFF_MASK);
            regValTemp |= DMA_TCD_NBYTES_MLOFFYES_MLOFF(offset);
            base->TCD[channel].NBYTES.MLOFFYES = regValTemp;
        }
    }
}

/*
 *
 * Function Name : PDMA_TCDSetChannelMinorLink.
 * Description   : Sets the channel minor link for the TCD.
 * 
 */
void PDMA_TCDSetChannelMinorLink(pdma_type_t *base, uint8_t channel, uint32_t linkChannel, bool enable)
{
    OS_ASSERT(channel < DMA_CHANNELS);

    if (enable) {
        OS_ASSERT(linkChannel < DMA_CHANNELS);
    } else {
    }

    uint16_t regValTemp;
    regValTemp = base->TCD[channel].BITER.ELINKYES;
    regValTemp &= (uint16_t) ~(DMA_TCD_BITER_ELINKYES_ELINK_MASK);
    regValTemp |= (uint16_t)DMA_TCD_BITER_ELINKYES_ELINK(enable ? 1UL : 0UL);
    base->TCD[channel].BITER.ELINKYES = regValTemp;

    regValTemp = base->TCD[channel].CITER.ELINKYES;
    regValTemp &= (uint16_t) ~(DMA_TCD_CITER_ELINKYES_ELINK_MASK);
    regValTemp |= (uint16_t)DMA_TCD_CITER_ELINKYES_ELINK(enable ? 1UL : 0UL);
    base->TCD[channel].CITER.ELINKYES = regValTemp;

    if (enable) {
        regValTemp = base->TCD[channel].BITER.ELINKYES;
        regValTemp &= (uint16_t) ~(DMA_TCD_BITER_ELINKYES_LINKCH_MASK);
        regValTemp |= (uint16_t)DMA_TCD_BITER_ELINKYES_LINKCH(linkChannel);
        base->TCD[channel].BITER.ELINKYES = regValTemp;

        regValTemp = base->TCD[channel].CITER.ELINKYES;
        regValTemp &= (uint16_t) ~(DMA_TCD_CITER_ELINKYES_LINKCH_MASK);
        regValTemp |= (uint16_t)DMA_TCD_CITER_ELINKYES_LINKCH(linkChannel);
        base->TCD[channel].CITER.ELINKYES = regValTemp;
    }
}

/*
 *
 * Function Name : PDMA_TCDSetMajorCount.
 * Description   : Sets the major iteration count according to minor loop channel link setting.
 * 
 * Note here that user need to first set the minor loop channel link and then call this function.
 * The execute flow inside this function is dependent on the minor loop channel link setting.
 * 
 */
void PDMA_TCDSetMajorCount(pdma_type_t *base, uint8_t channel, uint32_t count)
{
    OS_ASSERT(channel < DMA_CHANNELS);

    uint16_t regValTemp = 0;
    if (DMA_TCD_BITER_ELINKNO_ELINK_MASK == (base->TCD[channel].BITER.ELINKNO & DMA_TCD_BITER_ELINKNO_ELINK_MASK)) {
        regValTemp = base->TCD[channel].BITER.ELINKYES;
        regValTemp &= (uint16_t) ~(DMA_TCD_BITER_ELINKYES_BITER_MASK);
        regValTemp |= (uint16_t)DMA_TCD_BITER_ELINKYES_BITER(count);
        base->TCD[channel].BITER.ELINKYES = regValTemp;

        regValTemp = base->TCD[channel].CITER.ELINKYES;
        regValTemp &= (uint16_t) ~(DMA_TCD_CITER_ELINKYES_CITER_LE_MASK);
        regValTemp |= (uint16_t)DMA_TCD_CITER_ELINKYES_CITER_LE(count);
        base->TCD[channel].CITER.ELINKYES = regValTemp;
    } else {
        regValTemp = base->TCD[channel].BITER.ELINKNO;
        regValTemp &= (uint16_t) ~(DMA_TCD_BITER_ELINKNO_BITER_MASK);
        regValTemp |= (uint16_t)DMA_TCD_BITER_ELINKNO_BITER(count);
        base->TCD[channel].BITER.ELINKNO = regValTemp;

        regValTemp = base->TCD[channel].CITER.ELINKNO;
        regValTemp &= (uint16_t) ~(DMA_TCD_CITER_ELINKNO_CITER_MASK);
        regValTemp |= (uint16_t)DMA_TCD_CITER_ELINKNO_CITER(count);
        base->TCD[channel].CITER.ELINKNO = regValTemp;
    }
}

/*
 *
 * Function Name : PDMA_TCDGetCurrentMajorCount.
 * Description   : Returns the current major iteration count.
 * 
 * Number of iterations remaining in the main loop.
 * 
 */
uint32_t PDMA_TCDGetCurrentMajorCount(const pdma_type_t *base, uint8_t channel)
{
    OS_ASSERT(channel < DMA_CHANNELS);
    uint16_t result = 0U;
    if (DMA_TCD_BITER_ELINKNO_ELINK_MASK == (base->TCD[channel].BITER.ELINKNO & DMA_TCD_BITER_ELINKNO_ELINK_MASK)) {
        result = (uint16_t)((base->TCD[channel].CITER.ELINKYES & DMA_TCD_CITER_ELINKYES_CITER_LE_MASK) >>
                            DMA_TCD_CITER_ELINKYES_CITER_LE_SHIFT);
    } else {
        result = (uint16_t)((base->TCD[channel].CITER.ELINKNO & DMA_TCD_CITER_ELINKNO_CITER_MASK) >>
                            DMA_TCD_CITER_ELINKNO_CITER_SHIFT);
    }
    return (uint32_t)result;
}

/*
 *
 * Function Name : DMAMUX_Reset.
 * Description   : Initializes the DMAMUX module to the reset state.
 *  
 */
void DMAMUX_Reset(dmamux_type_t *base)
{
    uint8_t i;

    for (i = 0; i < DMAMUX_CHANNELS; i++) {
        base->CHCFG[i] = 0;
    }
}
