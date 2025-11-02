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
 * @file pdma_hw_access.h
 * @brief PDMA driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __PDMA_HW_ACCESS_H__
#define __PDMA_HW_ACCESS_H__

#include "pdma_driver.h"
#include "pdma_register.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Specifies the number of cycles the PDMA Engine is stalled.
 */
typedef enum {
    PDMA_ENGINE_STALL_0_CYCLES = 0, /**< Stall for 0 cycles after each R/W */
    PDMA_ENGINE_STALL_4_CYCLES = 2, /**< Stall for 4 cycles after each R/W */
    PDMA_ENGINE_STALL_8_CYCLES = 3  /**< Stall for 8 cycles after each R/W */
} pdma_engine_stall_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @name pDMA hw access module level functions
 * @{
 */

/**
 * @brief  PDMA Error interrupt handling function
 *
 * @param  data  Parameter value passed
 */
void PDMA_ErrorIRQHandler(void *data);

/**
 * @brief Initializes pDMA module to known state.
 *
 * @param  base Register base address for pDMA module.
 */
void PDMA_Init(pdma_type_t *base);

/**
 * @brief  Cancels the remaining data transfer.
 *
 * The cancellation takes effect after the last write of the current
 * read/write sequence. The CX clears itself after the cancel has been
 * honored. This cancel retires the channel normally as if the minor loop
 * had completed.
 * 
 * @param  base Register base address for pDMA module.
 */
void PDMA_CancelTransfer(pdma_type_t *base);

/**
 * @brief Cancels the remaining data transfer and treats it as an error condition.
 *
 * his function stops the executing channel and forces the minor loop
 * to finish. The cancellation takes effect after the last write of the
 * current read/write sequence. The CX clears itself after the cancel has
 * been honoured. This cancel retires the channel normally as if the minor
 * loop had completed. Additional thing is to treat this operation as an error
 * condition.
 * 
 * @param  base Register base address for pDMA module.
 */
void PDMA_CancelTransferWithError(pdma_type_t *base);

/**
 * @brief Enables/Disables the error interrupt for channels.
 *
 * @param  base    Register base address for pDMA module.
 * @param  channel Channel indicator.
 * @param  enable  Enable(true) or Disable (false) error interrupt.
 */
void PDMA_SetErrorIntCmd(pdma_type_t *base, uint8_t channel, bool enable);

/**
 * @brief Configures PDMA group priorities.
 *
 * @param  base     Register base address for pDMA module.
 * @param  priority PDMA group priorities Value.
 */
void PDMA_SetGroupPriority(pdma_type_t *base, pdma_group_priority_t priority);

/**
 * @brief  Configures the nbytes for the pDMA channel.
 *
 * Note here that user need firstly configure the minor loop mapping feature and then call this function
 * @param  base    Register base address for pDMA module.
 * @param  channel pDMA channel number.
 * @param  nbytes  Number of bytes to be transferred in each service request of the channel
 */
void PDMA_TCDSetNbytes(pdma_type_t *base, uint8_t channel, uint32_t nbytes);

/**
 * @brief  Clears all registers to 0 for the hardware TCD.
 *
 * @param  base    Register base address for pDMA module.
 * @param  channel pDMA channel number.
 */
void PDMA_TCDClearReg(pdma_type_t *base, uint8_t channel);

/**
 * @brief  Configures the transfer attribute for the pDMA channel.
 *
 * enumeration type for an allowed source modulo. The value defines a specific address range
 * specified as the value after the SADDR + SOFF calculation is performed on the original register
 * value. Setting this field provides the ability to implement a circular data. For data queues
 * requiring power-of-2 size bytes, the queue should start at a 0-modulo-size address and the SMOD
 * field should be set to the appropriate value for the queue, freezing the desired number of upper
 * address bits. The value programmed into this field specifies the number of the lower address bits
 * allowed to change. For a circular queue application, the SOFF is typically set to the transfer
 * size to implement post-increment addressing with SMOD function restricting the addresses to a
 * 0-modulo-size range.
 * 
 * @param  base              Register base address for pDMA module.
 * @param  channel           pDMA channel number.
 * @param  srcModulo         enumeration type for an allowed source modulo.
 * @param  destModulo        Enum type for an allowed destination modulo.
 * @param  srcTransferSize   Enum type for source transfer size.
 * @param  destTransferSize  Enum type for destination transfer size.
 */
void PDMA_TCDSetAttribute(pdma_type_t *base, uint8_t channel, pdma_modulo_t srcModulo, pdma_modulo_t destModulo,
                          pdma_transfer_size_t srcTransferSize, pdma_transfer_size_t destTransferSize);

/**
 * @brief  Configures the minor loop offset for the TCD.
 *
 * @param  base      Register base address for pDMA module.
 * @param  channel   pDMA channel number.
 * @param  offset    Minor loop offset
 */
void PDMA_TCDSetMinorLoopOffset(pdma_type_t *base, uint8_t channel, int32_t offset);

/**
 * @brief  Sets the channel minor link for the TCD.
 *
 * @param  base        Register base address for pDMA module.
 * @param  channel     pDMA channel number.
 * @param  linkChannel Channel to be linked on minor loop complete.
 * @param  enable      Enable (true)/Disable (false) channel minor link.
 */
void PDMA_TCDSetChannelMinorLink(pdma_type_t *base, uint8_t channel, uint32_t linkChannel, bool enable);

/**
 * @brief Initializes the DMAMUX module to the reset state.
 *
 * @param  base  Register base address for DMAMUX module.
 */
void DMAMUX_Reset(dmamux_type_t *base);

/**
 * @brief  Enables/Disables the PDMA request for the channel or all channels.
 *
 * @param  base    Register base address for pDMA module.
 * @param  channel Channel indicator.
 * @param  enable  Enable(true) or Disable (false) PDMA request.
 */
void PDMA_SetDmaRequestCmd(pdma_type_t *base, uint8_t channel, bool enable);

/**
 * @brief  Sets the major iteration count according to minor loop channel link setting.
 *
 * @param  base      Register base address for pDMA module.
 * @param  channel   pDMA channel number.
 * @param  count     major loop count
 */
void PDMA_TCDSetMajorCount(pdma_type_t *base, uint8_t channel, uint32_t count);

/**
 * @brief  Returns the current major iteration count.
 *
 * @param  base     Register base address for pDMA module.
 * @param  channel  pDMA channel number.
 * @return uint32_t
 *         current iteration count
 */
uint32_t PDMA_TCDGetCurrentMajorCount(const pdma_type_t *base, uint8_t channel);

/**
 * @brief PDMA virtualChannel error interrupt handler
 *
 * @param  virtualChannel PDMA channel
 */
void PDMA_DRV_ErrorIRQHandler(uint8_t virtualChannel);

/**
 * @brief  Halts or does not halt the pDMA module when an error occurs.
 *
 * An error causes the HALT bit to be set. Subsequently, all service requests are ignored until the
 * HALT bit is cleared.
 *
 * @param  base         Register base address for pDMA module.
 * @param  haltOnError  Halts (true) or not halt (false) pDMA module when an error occurs.
 */
static inline void PDMA_SetHaltOnErrorCmd(pdma_type_t *base, bool haltOnError)
{
    uint32_t regValTemp;

    regValTemp = base->CR;
    regValTemp &= ~(DMA_CR_HOE_MASK);
    regValTemp |= DMA_CR_HOE(haltOnError ? 1UL : 0UL);
    base->CR = regValTemp;
}

/**
 * @}
 */

/**
 * @name pDMA HAL driver channel priority and arbitration configuration
 * @{
 */

/**
 * @brief   Sets the pDMA channel priority.
 *
 * @param  base     Register base address for pDMA module.
 * @param  channel  pDMA channel number.
 * @param  priority Priority of the PDMA channel. Different channels should have different priority
 * setting inside a group.
 */
static inline void PDMA_SetChannelPriority(pdma_type_t *base, uint8_t channel, pdma_channel_priority_t priority)
{
    OS_ASSERT(channel < DMA_CHANNELS);

    uint8_t regValTemp;
    uint8_t index = (uint8_t)FEATURE_DMA_CHN_TO_DCHPRI_INDEX(channel);
    regValTemp    = base->DCHPRI[index];
    regValTemp &= (uint8_t) ~(DMA_DCHPRI_CHPRI_MASK);
    regValTemp |= (uint8_t)DMA_DCHPRI_CHPRI(priority);
    base->DCHPRI[index] = regValTemp;
}

/**
 * @brief Configures PDMA group Arbitration.
 *
 * @param  base     Register base address for pDMA module.
 * @param  priority PDMA group Arbitration Value.
 */
static inline void PDMA_SetGroupArbitrationMode(pdma_type_t *base, pdma_arbitration_algorithm_t groupArbitration)
{
    uint32_t regValTemp;

    regValTemp = base->CR;
    regValTemp &= ~(DMA_CR_ERGA_MASK);
    regValTemp |= DMA_CR_ERGA(groupArbitration);
    base->CR = regValTemp;
}

/**
 * @brief  Sets the channel arbitration algorithm.
 *
 * @param  base   Register base address for pDMA module.
 * @param  channelArbitration Round-Robin way or fixed priority way.
 */
static inline void PDMA_SetChannelArbitrationMode(pdma_type_t *base, pdma_arbitration_algorithm_t channelArbitration)
{
    uint32_t regValTemp;

    regValTemp = base->CR;
    regValTemp &= ~(DMA_CR_ERCA_MASK);
    regValTemp |= DMA_CR_ERCA(channelArbitration);
    base->CR = regValTemp;
}

/**
 * @brief  Gets the channel arbitration algorithm.
 *
 * @param  base Register base address for pDMA module.
 * @return pdma_arbitration_algorithm_t
 *         variable indicating the selected channel arbitration: Round-Robin way or fixed priority way
 */
static inline pdma_arbitration_algorithm_t PDMA_GetChannelArbitrationMode(const pdma_type_t *base)
{
    pdma_arbitration_algorithm_t retVal;

    if (((base->CR >> DMA_CR_ERCA_SHIFT) & 1U) != 0U) {
        retVal = PDMA_ARBITRATION_ROUND_ROBIN;
    } else {
        retVal = PDMA_ARBITRATION_FIXED_PRIORITY;
    }
    return retVal;
}

/**
 * @}
 */

/**
 * @name pDMA HAL driver configuration and operation
 * @{
 */

/**
 * @brief  Enables/Disables the minor loop mapping.
 *
 * This function enables/disables the minor loop mapping feature.
 * If enabled, the NBYTES is redefined to include the individual enable fields and the NBYTES field. The
 * individual enable fields allow the minor loop offset to be applied to the source address, the
 * destination address, or both. The NBYTES field is reduced when either offset is enabled.
 *
 * @param  base      Register base address for pDMA module.
 * @param  enable    Enables (true) or Disable (false) minor loop mapping.
 */
static inline void PDMA_SetMinorLoopMappingCmd(pdma_type_t *base, bool enable)
{
    uint32_t regValTemp;
    regValTemp = base->CR;
    regValTemp &= ~(DMA_CR_EMLM_MASK);
    regValTemp |= DMA_CR_EMLM(enable ? 1UL : 0UL);
    base->CR = regValTemp;
}

/**
 * @brief  Gets the pDMA error interrupt status.
 *
 * @param  base  Register base address for pDMA module.
 * @return uint32_t
 *          32 bit variable indicating error channels. If error happens on pDMA channel n, the bit n
 *          of this variable is '1'. If not, the bit n of this variable is '0'.
 */
static inline uint32_t PDMA_GetErrorIntStatusFlag(const pdma_type_t *base)
{
    return base->ERR;
}

/**
 * @brief  Clears the error interrupt status for the pDMA channel or channels.
 *
 * @param  base    Register base address for pDMA module.
 * @param  channel Channel indicator.
 */
static inline void PDMA_ClearErrorIntStatusFlag(pdma_type_t *base, uint8_t channel)
{
    OS_ASSERT(channel < DMA_CHANNELS);
    base->CERR = (uint8_t)channel;
}

/**
 * @brief  Clears the done status for a channel or all channels.
 *
 * @param  base     Register base address for pDMA module.
 * @param  channel  Channel indicator.
 */
static inline void PDMA_ClearDoneStatusFlag(pdma_type_t *base, uint8_t channel)
{
    OS_ASSERT(channel < DMA_CHANNELS);
    base->CDNE = (uint8_t)channel;
}

/**
 * @brief  Triggers the pDMA channel.
 *
 * @param  base    Register base address for pDMA module.
 * @param  channel Channel indicator.
 */
static inline void PDMA_TriggerChannelStart(pdma_type_t *base, uint8_t channel)
{
    OS_ASSERT(channel < DMA_CHANNELS);
    base->SSRT = (uint8_t)channel;
}

/**
 * @brief  Clears the interrupt status for the pDMA channel or all channels.
 *
 * @param  base    Register base address for pDMA module.
 * @param  channel Channel indicator.
 */
static inline void PDMA_ClearIntStatusFlag(pdma_type_t *base, uint8_t channel)
{
    OS_ASSERT(channel < DMA_CHANNELS);
    base->CINT = (uint8_t)channel;
}

/**
 * @brief  Configures PDMA engine to stall for a number of cycles after each R/W.
 *
 * @param  base Register base address for pDMA module.
 * @param  channel Channel indicator.
 * @param  cycles  Number of cycles the PDMA engine is stalled after each R/W.
 */
static inline void PDMA_TCDSetEngineStall(pdma_type_t *base, uint8_t channel, pdma_engine_stall_t cycles)
{
    OS_ASSERT(channel < DMA_CHANNELS);
    uint16_t regValTemp;
    regValTemp = base->TCD[channel].CSR;
    regValTemp &= ~(DMA_TCD_CSR_BWC_MASK);
    regValTemp |= DMA_TCD_CSR_BWC(cycles);
    base->TCD[channel].CSR = regValTemp;
}

/**
 * @brief Configures the source address for the hardware TCD.
 *
 * @param  base    Register base address for pDMA module.
 * @param  channel pDMA channel number.
 * @param  address The pointer to the source memory address.
 */
static inline void PDMA_TCDSetSrcAddr(pdma_type_t *base, uint8_t channel, uint32_t address)
{
    OS_ASSERT(channel < DMA_CHANNELS);
    base->TCD[channel].SADDR = address;
}

/**
 * @brief  Configures the source address signed offset for the hardware TCD.
 *
 * Sign-extended offset applied to the current source address to form the next-state value as each
 * source read is complete.
 *
 * @param  base     Register base address for pDMA module.
 * @param  channel  pDMA channel number.
 * @param  offset   signed-offset for source address.
 */
static inline void PDMA_TCDSetSrcOffset(pdma_type_t *base, uint8_t channel, int16_t offset)
{
    OS_ASSERT(channel < DMA_CHANNELS);
    base->TCD[channel].SOFF = (uint16_t)offset;
}

/**
 * @brief Sets the source transfer size.
 *
 * Configures the source data read transfer size (1/2/4/16/32 bytes).
 *
 * @param  base      Register base address for pDMA module.
 * @param  channel   pDMA channel number.
 * @param  size      Source transfer size.
 */
static inline void PDMA_TCDSetSrcTransferSize(pdma_type_t *base, uint8_t channel, pdma_transfer_size_t size)
{

    OS_ASSERT(channel < DMA_CHANNELS);

    uint16_t regValTemp;
    regValTemp = base->TCD[channel].ATTR;
    regValTemp &= (uint16_t)(~(DMA_TCD_ATTR_SSIZE_MASK));
    regValTemp |= (uint16_t)(DMA_TCD_ATTR_SSIZE((uint16_t)size));
    base->TCD[channel].ATTR = regValTemp;
}

/**
 * @brief  Sets the destination transfer size.
 *
 * Configures the destination data write transfer size (1/2/4/16/32 bytes).
 *
 * @param  base    Register base address for pDMA module.
 * @param  channel pDMA channel number.
 * @param  size    Destination transfer size.
 */
static inline void PDMA_TCDSetDestTransferSize(pdma_type_t *base, uint8_t channel, pdma_transfer_size_t size)
{

    OS_ASSERT(channel < DMA_CHANNELS);

    uint16_t regValTemp;
    regValTemp = base->TCD[channel].ATTR;
    regValTemp &= (uint16_t)(~(DMA_TCD_ATTR_DSIZE_MASK));
    regValTemp |= (uint16_t)(DMA_TCD_ATTR_DSIZE((uint16_t)size));
    base->TCD[channel].ATTR = regValTemp;
}

/**
 * @brief  Enables/disables the source minor loop offset feature for the TCD.
 *
 * Configures whether the minor loop offset is applied to the source address
 * upon minor loop completion.
 * NOTE: EMLM bit needs to be enabled prior to calling this function, otherwise
 * it has no effect.
 *
 * @param  base     Register base address for pDMA module.
 * @param  channel  pDMA channel number.
 * @param  enable   Enables (true) or disables (false) source minor loop offset.
 */
static inline void PDMA_TCDSetSrcMinorLoopOffsetCmd(pdma_type_t *base, uint8_t channel, bool enable)
{

    OS_ASSERT(channel < DMA_CHANNELS);

    if (((base->CR >> DMA_CR_EMLM_SHIFT) & 1U) != 0U) {
        uint32_t regValTemp;
        regValTemp = base->TCD[channel].NBYTES.MLOFFYES;
        regValTemp &= ~(DMA_TCD_NBYTES_MLOFFYES_SMLOE_MASK);
        regValTemp |= DMA_TCD_NBYTES_MLOFFYES_SMLOE(enable ? 1UL : 0UL);
        base->TCD[channel].NBYTES.MLOFFYES = regValTemp;
    }
}

/**
 * @brief Enables/disables the destination minor loop offset feature for the TCD.
 *
 * Configures whether the minor loop offset is applied to the destination address
 * upon minor loop completion.
 * NOTE: EMLM bit needs to be enabled prior to calling this function, otherwise
 * it has no effect.
 *
 * @param  base     Register base address for pDMA module.
 * @param  channel  pDMA channel number.
 * @param  enable   Enables (true) or disables (false) destination minor loop offset.
 */
static inline void PDMA_TCDSetDestMinorLoopOffsetCmd(pdma_type_t *base, uint8_t channel, bool enable)
{

    OS_ASSERT(channel < DMA_CHANNELS);

    if (((base->CR >> DMA_CR_EMLM_SHIFT) & 1U) != 0U) {
        uint32_t regValTemp;
        regValTemp = base->TCD[channel].NBYTES.MLOFFYES;
        regValTemp &= ~(DMA_TCD_NBYTES_MLOFFYES_DMLOE_MASK);
        regValTemp |= DMA_TCD_NBYTES_MLOFFYES_DMLOE(enable ? 1UL : 0UL);
        base->TCD[channel].NBYTES.MLOFFYES = regValTemp;
    }
}

/**
 * @brief The pointer to the destination address.
 *
 * Adjustment value added to the source address at the completion of the major iteration count. This
 * value can be applied to restore the source address to the initial value, or adjust the address to
 * reference the next data structure.
 *
 * @param  base    Register base address for pDMA module.
 * @param  channel pDMA channel number.
 * @param  size    adjustment value
 */
static inline void PDMA_TCDSetSrcLastAdjust(pdma_type_t *base, uint8_t channel, int32_t size)
{

    OS_ASSERT(channel < DMA_CHANNELS);

    base->TCD[channel].SLAST = (uint32_t)size;
}

/**
 * @brief  Configures the destination address for the TCD.
 *
 * @param  base     Register base address for pDMA module.
 * @param  channel  pDMA channel number.
 * @param  address  The pointer to the destination address.
 */
static inline void PDMA_TCDSetDestAddr(pdma_type_t *base, uint8_t channel, uint32_t address)
{

    OS_ASSERT(channel < DMA_CHANNELS);

    base->TCD[channel].DADDR = address;
}

/**
 * @brief  Configures the destination address signed offset for the TCD.
 *
 * Sign-extended offset applied to the current source address to form the next-state value as each
 * destination write is complete.
 *
 * @param  base    Register base address for pDMA module.
 * @param  channel pDMA channel number.
 * @param  offset  signed-offset
 */
static inline void PDMA_TCDSetDestOffset(pdma_type_t *base, uint8_t channel, int16_t offset)
{

    OS_ASSERT(channel < DMA_CHANNELS);

    base->TCD[channel].DOFF = (uint16_t)offset;
}

/**
 * @brief  Configures the last source address adjustment.
 *
 * This function adds an adjustment value added to the source address at the completion of the major
 * iteration count. This value can be applied to restore the source address to the initial value, or
 * adjust the address to reference the next data structure.
 *
 * @param  base       Register base address for pDMA module.
 * @param  channel    pDMA channel number.
 * @param  adjust     adjustment value
 */
static inline void PDMA_TCDSetDestLastAdjust(pdma_type_t *base, uint8_t channel, int32_t adjust)
{

    OS_ASSERT(channel < DMA_CHANNELS);

    base->TCD[channel].DLASTSGA = (uint32_t)adjust;
}

/**
 * @brief  Configures the major channel link the TCD.
 *
 * If the major link is enabled, after the major loop counter is exhausted, the pDMA engine initiates a
 * channel service request at the channel defined by these six bits by setting that channel start
 * bits.
 *
 * @param  base     Register base address for pDMA module.
 * @param  channel  pDMA channel number.
 * @param  majorLinkChannel channel number for major link
 * @param  enable   Enables (true) or Disables (false) channel major link.
 */
static inline void PDMA_TCDSetChannelMajorLink(pdma_type_t *base, uint8_t channel, uint32_t majorLinkChannel,
                                               bool enable)
{

    OS_ASSERT(channel < DMA_CHANNELS);

    uint16_t regValTemp;
    regValTemp = base->TCD[channel].CSR;
    regValTemp &= (uint16_t) ~(DMA_TCD_CSR_MAJORLINKCH_MASK);
    regValTemp |= (uint16_t)DMA_TCD_CSR_MAJORLINKCH(majorLinkChannel);
    base->TCD[channel].CSR = regValTemp;
    regValTemp             = base->TCD[channel].CSR;
    regValTemp &= (uint16_t) ~(DMA_TCD_CSR_MAJORELINK_MASK);
    regValTemp |= (uint16_t)DMA_TCD_CSR_MAJORELINK(enable ? 1UL : 0UL);
    base->TCD[channel].CSR = regValTemp;
}

/**
 * @brief  Disables/Enables the PDMA request after the major loop completes for the TCD.
 *
 * If disabled, the pDMA hardware automatically clears the corresponding PDMA request when the
 * current major iteration count reaches zero.
 *
 * @param  base     Register base address for pDMA module.
 * @param  channel  pDMA channel number.
 * @param  disable  Disable (true)/Enable (false) PDMA request after TCD complete.
 */
static inline void PDMA_TCDSetDisableDmaRequestAfterTCDDoneCmd(pdma_type_t *base, uint8_t channel, bool disable)
{
    OS_ASSERT(channel < DMA_CHANNELS);
    uint16_t regValTemp;
    regValTemp = base->TCD[channel].CSR;
    regValTemp &= (uint16_t) ~(DMA_TCD_CSR_DREQ_MASK);
    regValTemp |= (uint16_t)DMA_TCD_CSR_DREQ(disable ? 1UL : 0UL);
    base->TCD[channel].CSR = regValTemp;
}

/**
 * @brief  Enables/Disables the half complete interrupt for the TCD.
 *
 * If set, the channel generates an interrupt request by setting the appropriate bit in the
 * interrupt register when the current major iteration count reaches the halfway point. Specifically,
 * the comparison performed by the pDMA engine is (CITER == (BITER >> 1)). This half-way point
 * interrupt request is provided to support the double-buffered schemes or other types of data movement
 * where the processor needs an early indication of the transfer's process.
 *
 * @param  base    Register base address for pDMA module.
 * @param  channel pDMA channel number.
 * @param  enable  Enable (true) /Disable (false) half complete interrupt.
 */
static inline void PDMA_TCDSetMajorHalfCompleteIntCmd(pdma_type_t *base, uint8_t channel, bool enable)
{

    OS_ASSERT(channel < DMA_CHANNELS);

    uint16_t regValTemp;
    regValTemp = base->TCD[channel].CSR;
    regValTemp &= (uint16_t) ~(DMA_TCD_CSR_INTHALF_MASK);
    regValTemp |= (uint16_t)DMA_TCD_CSR_INTHALF(enable ? 1UL : 0UL);
    base->TCD[channel].CSR = regValTemp;
}

/**
 * @brief  Enables/Disables the interrupt after the major loop completes for the TCD.
 *
 * If enabled, the channel generates an interrupt request by setting the appropriate bit in the
 * interrupt register when the current major iteration count reaches zero.
 *
 * @param  base     Register base address for pDMA module.
 * @param  channel  pDMA channel number.
 * @param  enable   Enable (true) /Disable (false) interrupt after TCD done.
 */
static inline void PDMA_TCDSetMajorCompleteIntCmd(pdma_type_t *base, uint8_t channel, bool enable)
{
    OS_ASSERT(channel < DMA_CHANNELS);

    uint16_t regValTemp;
    regValTemp = base->TCD[channel].CSR;
    regValTemp &= (uint16_t) ~(DMA_TCD_CSR_INTMAJOR_MASK);
    regValTemp |= (uint16_t)DMA_TCD_CSR_INTMAJOR(enable ? 1UL : 0UL);
    base->TCD[channel].CSR = regValTemp;
}

/**
 * @brief Enables/Disables the DMAMUX channel.
 *
 * Enables the hardware request. If enabled, the hardware request is  sent to
 * the corresponding PDMA channel.
 *
 * @param  base    Register base address for DMAMUX module.
 * @param  channel DMAMUX channel number.
 * @param  enable  Enables (true) or Disables (false) DMAMUX channel.
 */
static inline void DMAMUX_SetChannelCmd(dmamux_type_t *base, uint8_t channel, bool enable)
{
    OS_ASSERT(channel < DMAMUX_CHANNELS);

    uint32_t regIndex   = DMAMUX_CHN_REG_INDEX(channel);
    uint32_t regValTemp = base->CHCFG[regIndex];
    regValTemp &= (uint32_t) ~(DMAMUX_CHCFG_ENBL_MASK);
    regValTemp |= (uint32_t)DMAMUX_CHCFG_ENBL(enable ? 1U : 0U);
    base->CHCFG[regIndex] = regValTemp;
}

/**
 * @brief  Configure PDMA Channel Trigger bit in DMAMUX.
 *
 *  Enables/Disables PDMA Channel Trigger bit in DMAMUX.
 *
 * @param  base     Register base address for DMAMUX module.
 * @param  channel  DMAMUX channel number.
 * @param  enable   /disable command.
 */
static inline void DMAMUX_SetChannelTrigger(dmamux_type_t *base, uint8_t channel, bool enable)
{
    OS_ASSERT(channel < DMAMUX_CHANNELS);
    uint32_t regIndex = DMAMUX_CHN_REG_INDEX(channel);
    uint32_t regValTemp;
    regValTemp = base->CHCFG[regIndex];
    regValTemp &= (uint32_t) ~(DMAMUX_CHCFG_TRIG_MASK);
    regValTemp |= (uint32_t)DMAMUX_CHCFG_TRIG(enable ? 1U : 0U);
    base->CHCFG[regIndex] = regValTemp;
}

/**
 * @brief   Configures the PDMA request for the DMAMUX channel.
 *
 * Selects which PDMA source is routed to a PDMA channel. The PDMA sources are defined in the file
 * <MCU>_Features.h
 *
 * @param  base    Register base address for DMAMUX module.
 * @param  channel DMAMUX channel number.
 * @param  source  PDMA request source.
 */
static inline void DMAMUX_SetChannelSource(dmamux_type_t *base, uint8_t channel, uint8_t source)
{
    OS_ASSERT(channel < DMAMUX_CHANNELS);

    uint32_t regIndex = DMAMUX_CHN_REG_INDEX(channel);
    uint32_t regValTemp;
    regValTemp = base->CHCFG[regIndex];
    regValTemp &= (uint32_t) ~(DMAMUX_CHCFG_SOURCE_MASK);
    regValTemp |= (uint32_t)DMAMUX_CHCFG_SOURCE(source);
    base->CHCFG[regIndex] = regValTemp;
}

/**
 * @brief   Returns PDMA Register Base Address. Gets the address of the selected PDMA module.
 *
 * @param  instance  PDMA instance to be returned.
 * @return pdma_type_t* PDMA register base address
 */
pdma_type_t *PDMA_GetDmaRegBaseAddr(uint32_t instance);

/**
 * @brief Interrupt registration function
 *
 * @param  data Parameter values passed between functions
 */
void PDMA_IRQHandler(void *data);

/**
 * @}
 */

#if defined(__cplusplus)
}
#endif

#endif
