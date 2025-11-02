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
 * @file adma_hw_access.h
 * @brief ADMA driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __ADMA_HW_ACCESS_H__
#define __ADMA_HW_ACCESS_H__

#include "adma_driver.h"
#include "adma_register.h"
#include <stdbool.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Sets the core operation mode.
 *
 * @param base Register base address for ADMA module.
 * @param base Core indicator.
 * @param channelArbitration Round-Robin way or fixed priority way.
 */
static inline void ADMA_SetCoreOperationMode(adma_type_t *base, uint32_t core, adma_operation_mode_t operationMode)
{

#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    OS_ASSERT(core < sizeof(base->OPMODE) / sizeof(uint32_t));
#endif

    uint32_t regValTemp;
    regValTemp = base->OPMODE.CORE_JOINT_MODE[core];
    regValTemp &= ~(ADMA_CORE_JOINT_MODE_MODE_MASK);
    regValTemp |= ADMA_CORE_JOINT_MODE_MODE(operationMode);
    base->OPMODE.CORE_JOINT_MODE[core] = regValTemp;
}

/**
 * @brief Sets the Core 1 clock divider
 *
 * @param base Register base address for ADMA module.
 * @param base Core indicator.
 * @param channelArbitration Round-Robin way or fixed priority way.
 */
static inline void ADMA_SetCore1Clkdiv(adma_type_t *base, uint32_t ratio)
{

#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    OS_ASSERT(core < sizeof(base->OPMODE) / sizeof(uint32_t));
#endif

    uint32_t regValTemp;
    regValTemp = base->CORE1_CLKDIV;
    regValTemp &= ~(ADMA_CORE1_CLKDIV_RATIO_MASK);
    regValTemp |= ADMA_CORE1_CLKDIV_RATIO(ratio);
    base->CORE1_CLKDIV = regValTemp;
}

/**
 * @brief Allow to start multiple channels simultaneously.
 * Each bit set starts the corresponding channel.
 *
 * @param base Register base address for ADMA module.
 * @param base Core indicator.
 * @param base channel indicator.
 */
static inline void ADMA_SetChannelStart(adma_type_t *base, uint32_t core, uint32_t channel)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    OS_ASSERT(core < sizeof(base->OPMODE) / sizeof(uint32_t));
    OS_ASSERT(channel < FEATURE_ADMA_CHANNELS);
#endif

    uint32_t regValTemp;
    regValTemp = base->CHSTART.CORE_CH_START[core];
    regValTemp &= ~(ADMA_CORE_CH_START_MASK);
    regValTemp |= ADMA_CORE_CH_START(channel);
    base->CHSTART.CORE_CH_START[core] = regValTemp;
}

#if 0
/**
 * @brief Set Direct control of peripheral RX request.
 *
 * @param base Register base address for ADMA module.
 * @param base Core indicator.
 */
static inline void ADMA_SetPeripheralRxCtrl(adma_type_t *base, uint32_t value)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    OS_ASSERT(channel < FEATURE_ADMA_CHANNELS);
#endif
}

/**
 * @brief Set Direct control of peripheral Tx request.
 *
 * @param base Register base address for ADMA module.
 * @param base Core indicator.
 */
static inline void ADMA_SetPeripheralTxCtrl(adma_type_t *base, uint32_t value)
{
}
#endif

/**
 * @brief Enables/Disables the error interrupt for channels.
 *
 * @param base Register base address for ADMA module.
 * @param channel Channel indicator.
 * @param intEnable  ADMA error interrupts.
 */
static inline void ADMA_SetErrorIntCmd(adma_type_t *base, uint8_t channel, uint32_t intEnable)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    OS_ASSERT(channel < FEATURE_ADMA_CHANNELS);
#endif

    uint32_t regValTemp;
    regValTemp = base->CHREG[channel].INT_ENABLE_REG;
    regValTemp &= ~(ADMA_INT_ENABLE_REG_INT_ENABLE_ERROR_MASK);
    regValTemp |= ADMA_INT_ENABLE_REG_INT_ENABLE_ERROR(intEnable);
    base->CHREG[channel].INT_ENABLE_REG = regValTemp;
}

/**
 * @brief Enables/Disables the interrupt for channels.
 *
 * @param base Register base address for ADMA module.
 * @param channel Channel indicator.
 * @param intEnable  ADMA interrupts.
 */
static inline void ADMA_SetIntCmd(adma_type_t *base, uint8_t channel, uint32_t intEnable)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    OS_ASSERT(channel < FEATURE_ADMA_CHANNELS);
#endif

    uint32_t regValTemp;
    regValTemp = base->CHREG[channel].INT_ENABLE_REG;
    regValTemp &= ~(ADMA_INT_ENABLE_REG_INT_ENABLE_CH_END_MASK);
    regValTemp |= ADMA_INT_ENABLE_REG_INT_ENABLE_CH_END(intEnable);
    base->CHREG[channel].INT_ENABLE_REG = regValTemp;
}

/**
 * @brief Gets the ADMA interrupt status.
 *
 * @param base Register base address for ADMA module.
 * @return 32 bit variable indicating error channels. If error happens on ADMA channel n, the bit n
 * of this variable is '1'. If not, the bit n of this variable is '0'.
 */
static inline uint32_t ADMA_GetIntStatusFlag(const adma_type_t *base)
{
    return base->INT0_STATUS & ADMA_INT0_STATUS_INT_STATUS_MASK;
}

/**
 * @brief Gets the ADMA error interrupt status.
 *
 * @param base Register base address for ADMA module.
 * @return 32 bit variable indicating error channels. If error happens on ADMA channel n, the bit n
 * of this variable is '1'. If not, the bit n of this variable is '0'.
 */
static inline uint32_t ADMA_GetChannelErrorIntStatusFlag(const adma_type_t *base, uint8_t channel)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    OS_ASSERT(channel < FEATURE_ADMA_CHANNELS);
#endif

    return base->CHREG[channel].INT_STATUS_REG & ADMA_INT_STATUS_REG_INT_STATUS_ERROR_MASK;
}

/**
 * @brief Clears the error interrupt status for the ADMA channel or channels.
 *
 * @param base Register base address for ADMA module.
 * @param channel Channel indicator.
 */
static inline void ADMA_ClearChannelErrorIntStatusFlag(adma_type_t *base, uint8_t channel, uint32_t intState)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    OS_ASSERT(channel < FEATURE_ADMA_CHANNELS);
#endif
    base->CHREG[channel].INT_CLEAR_REG = ADMA_INT_CLEAR_REG_INT_CLR(intState);
}

/**
 * @brief Gets the ADMA interrupt status.
 *
 * @param base Register base address for ADMA module.
 * @return 32 bit variable indicating error channels. If error happens on ADMA channel n, the bit n
 * of this variable is '1'. If not, the bit n of this variable is '0'.
 */
static inline uint32_t ADMA_GetChannelIntStatusFlag(const adma_type_t *base, uint8_t channel)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    OS_ASSERT(channel < FEATURE_ADMA_CHANNELS);
#endif

    return base->CHREG[channel].INT_STATUS_REG & ADMA_INT_STATUS_REG_INT_STATUS_CH_END_MASK;
}

/**
 * @brief Clears the error interrupt status for the ADMA channel or channels.
 *
 * @param base Register base address for ADMA module.
 * @param channel Channel indicator.
 */
static inline void ADMA_ClearChannelIntStatusFlag(adma_type_t *base, uint8_t channel, uint32_t intState)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    OS_ASSERT(channel < FEATURE_ADMA_CHANNELS);
#endif
    base->CHREG[channel].INT_CLEAR_REG = ADMA_INT_CLEAR_REG_INT_CLR(intState);
}

/**
 * @brief Enables/Disables the ADMA request for the channel or all channels.
 *
 * @param base Register base address for ADMA module.
 * @param enable Enable(true) or Disable (false) ADMA request.
 * @param channel Channel indicator.
 */
static inline void ADMA_SetDmaChannelRequestCmd(adma_type_t *base, uint8_t channel, bool enable)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    OS_ASSERT(channel < FEATURE_ADMA_CHANNELS);
#endif
    base->CHREG[channel].CH_START_REG = ADMA_CH_START_REG_CH_START(enable);
}

/**
 * @brief Enables/Disables adma channel.
 *
 * @param base Register base address for ADMA module.
 * @param enable Enable(true) or Disable (false) ADMA request.
 * @param channel Channel indicator.
 */
static inline void ADMA_SetDmaChannelEnableCmd(adma_type_t *base, uint8_t channel, bool enable)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    OS_ASSERT(channel < FEATURE_ADMA_CHANNELS);
#endif

    uint32_t regValTemp;
    regValTemp = base->CHREG[channel].CH_ENABLE_REG;
    regValTemp &= ~(ADMA_CH_ENABLE_REG_CH_ENABLE_MASK);
    regValTemp |= ADMA_CH_ENABLE_REG_CH_ENABLE(enable);
    base->CHREG[channel].CH_ENABLE_REG = regValTemp;
}

/**
 * @brief Configures the source address for the hardware TCD.
 *
 * @param base Register base address for ADMA module.
 * @param channel ADMA channel number.
 * @param address The pointer to the source memory address.
 */
static inline void ADMA_SetSrcAddr(adma_type_t *base, uint8_t channel, uint32_t address)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    OS_ASSERT(channel < FEATURE_ADMA_CHANNELS);
#endif

    uint32_t regValTemp = base->CHREG[channel].CMD_REG0;
    regValTemp &= ~(ADMA_CMD_REG0_RD_START_ADDR_MASK);
    regValTemp |= ADMA_CMD_REG0_RD_START_ADDR(address);
    base->CHREG[channel].CMD_REG0 = regValTemp;
}

/**
 * @brief Configures the destination address for the hardware TCD.
 *
 * @param base Register base address for ADMA module.
 * @param channel ADMA channel number.
 * @param address The pointer to the destination memory address.
 */
static inline void ADMA_SetDestAddr(adma_type_t *base, uint8_t channel, uint32_t address)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    OS_ASSERT(channel < FEATURE_ADMA_CHANNELS);
#endif

    uint32_t regValTemp = base->CHREG[channel].CMD_REG1;
    regValTemp &= ~(ADMA_CMD_REG1_WR_START_ADDR_MASK);
    regValTemp |= ADMA_CMD_REG1_WR_START_ADDR(address);
    base->CHREG[channel].CMD_REG1 = regValTemp;
}

/**
 * @brief Configures the size of buffer to transfer.
 *
 * @param base Register base address for ADMA module.
 * @param channel ADMA channel number.
 * @param bufferSize Size of buffer to transfer.
 */
static inline void ADMA_SetBufferSize(adma_type_t *base, uint8_t channel, uint32_t bufferSize)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    OS_ASSERT(channel < FEATURE_ADMA_CHANNELS);
#endif

    uint32_t regValTemp = base->CHREG[channel].CMD_REG2;
    regValTemp &= ~(ADMA_CMD_REG2_BUFFER_SIZE_MASK);
    regValTemp |= ADMA_CMD_REG2_BUFFER_SIZE(bufferSize);
    base->CHREG[channel].CMD_REG2 = regValTemp;
}

/**
 * @brief Enables/Disables the transfer complete interrupt for channels.
 *
 * @param base Register base address for ADMA module.
 * @param channel ADMA channel number.
 * @param enable Enable(true) or Disable (false) interrupt.
 */
static inline void ADMA_SetCmdSetIntCmd(adma_type_t *base, uint8_t channel, bool enable)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    OS_ASSERT(channel < FEATURE_ADMA_CHANNELS);
#endif

    uint32_t regValTemp = base->CHREG[channel].CMD_REG3;
    regValTemp &= ~(ADMA_CMD_REG3_CMD_SET_INT_MASK);
    regValTemp |= ADMA_CMD_REG3_CMD_SET_INT(enable);
    base->CHREG[channel].CMD_REG3 = regValTemp;
}

/**
 * @brief Set ADMA CMD_LAST.
 *
 * If set the channel will stop once the entire buffer has been transferred,
 * if not the next command will be loaded from the address specified in the
 * CMD_NEXT_ADDR field.
 *
 * @param base Register base address for ADMA module.
 * @param channel ADMA channel number.
 * @param state CMD_LAST state.
 */
static inline void ADMA_SetCmdLast(adma_type_t *base, uint8_t channel, bool state)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    OS_ASSERT(channel < FEATURE_ADMA_CHANNELS);
#endif

    uint32_t regValTemp = base->CHREG[channel].CMD_REG3;
    regValTemp &= ~(ADMA_CMD_REG3_CMD_LAST_MASK);
    regValTemp |= ADMA_CMD_REG3_CMD_LAST(state);
    base->CHREG[channel].CMD_REG3 = regValTemp;
}

/**
 * @brief Set ADMA read address increment.
 *
 * If set the controller will increment the next burst address
 *
 * @param base Register base address for ADMA module.
 * @param channel ADMA channel number.
 * @param state ADMA increment the next burst address enable state.
 */
static inline void ADMA_SetRDIncr(adma_type_t *base, uint8_t channel, bool enable)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    OS_ASSERT(channel < FEATURE_ADMA_CHANNELS);
#endif

    uint32_t regValTemp = base->CHREG[channel].STATIC_REG0;
    regValTemp &= ~(ADMA_STATIC_REG0_RD_INCR_MASK);
    regValTemp |= ADMA_STATIC_REG0_RD_INCR(enable);
    base->CHREG[channel].STATIC_REG0 = regValTemp;
}

/**
 * @brief Set the number of read brust and read tokens
 *
 * Set Maximum number of bytes of an AHB read burst and
 * the Number of AHB read commands to issue before the channel is released.
 *
 * @param base Register base address for ADMA module.
 * @param channel ADMA channel number.
 * @param brust Maximum number of bytes of an AHB read burst.
 * @param tokens Number of AHB read commands to issue before the channel is released.
 */
static inline void ADMA_SetRdBustRdTokens(adma_type_t *base, uint8_t channel, uint32_t brust, uint32_t tokens)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    OS_ASSERT(channel < FEATURE_ADMA_CHANNELS);
#endif

    uint32_t regValTemp = base->CHREG[channel].STATIC_REG0;
    regValTemp &= ~(ADMA_STATIC_REG0_RD_BURST_MAX_SIZE_MASK | ADMA_STATIC_REG0_RD_TOKENS_MASK);
    regValTemp |= (ADMA_STATIC_REG0_RD_BURST_MAX_SIZE(brust) | ADMA_STATIC_REG0_RD_TOKENS(tokens));
    base->CHREG[channel].STATIC_REG0 = regValTemp;
}

/**
 * @brief Set ADMA write address increment.
 *
 * If set the controller will increment the next burst address
 *
 * @param base Register base address for ADMA module.
 * @param channel ADMA channel number.
 * @param state ADMA increment the next burst address enable state.
 */
static inline void ADMA_SetWRIncr(adma_type_t *base, uint8_t channel, bool enable)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    OS_ASSERT(channel < FEATURE_ADMA_CHANNELS);
#endif

    uint32_t regValTemp = base->CHREG[channel].STATIC_REG1;
    regValTemp &= ~(ADMA_STATIC_REG1_WR_INCR_MASK);
    regValTemp |= ADMA_STATIC_REG1_WR_INCR(enable);
    base->CHREG[channel].STATIC_REG1 = regValTemp;
}

/**
 * @brief Set the number of write brust and write tokens
 *
 * Set Maximum number of bytes of an AHB write burst and
 * the Number of AHB write commands to issue before the channel is released.
 *
 * @param base Register base address for ADMA module.
 * @param channel ADMA channel number.
 * @param brust Maximum number of bytes of an AHB write burst.
 * @param tokens Number of AHB write commands to issue before the channel is released.
 */
static inline void ADMA_SetWrBustWrTokens(adma_type_t *base, uint8_t channel, uint32_t brust, uint32_t tokens)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    OS_ASSERT(channel < FEATURE_ADMA_CHANNELS);
#endif

    uint32_t regValTemp = base->CHREG[channel].STATIC_REG1;
    regValTemp &= ~(ADMA_STATIC_REG1_WR_BURST_MAX_SIZE_MASK | ADMA_STATIC_REG1_WR_TOKENS_MASK);
    regValTemp |= (ADMA_STATIC_REG1_WR_BURST_MAX_SIZE(brust) | ADMA_STATIC_REG1_WR_TOKENS(tokens));
    base->CHREG[channel].STATIC_REG1 = regValTemp;
}

/**
 * @brief Set the ADMA periph param
 *
 * Set Maximum number of bytes of an AHB write burst and
 * the Number of AHB write commands to issue before the channel is released.
 *
 * @param base Register base address for ADMA module.
 * @param channel ADMA channel number.
 * @param config adma_peripheral_config .
 */
static inline void ADMA_SetPeriphConfig(adma_type_t *base, uint8_t channel, adma_peripheral_config_t *config)
{
#if defined(CUSTOM_DEVASSERT) || defined(DEV_ERROR_DETECT)
    OS_ASSERT(channel < FEATURE_ADMA_CHANNELS);
#endif

    uint32_t regValTemp = base->CHREG[channel].STATIC_REG4;
    regValTemp &= ~(ADMA_STATIC_REG4_RD_PERIPH_NUM_MASK | ADMA_STATIC_REG4_RD_PERIPH_DELAY_MASK |
                    ADMA_STATIC_REG4_WR_PERIPH_NUM_MASK | ADMA_STATIC_REG4_WR_PERIPH_DELAY_MASK);

    regValTemp |= (ADMA_STATIC_REG4_RD_PERIPH_NUM(config->rdPeriphNum) |
                   ADMA_STATIC_REG4_WR_PERIPH_NUM(config->wrPeriphNum) |
                   ADMA_STATIC_REG4_RD_PERIPH_DELAY(config->rdPeriphDelay) |
                   ADMA_STATIC_REG4_WR_PERIPH_DELAY(config->wrPeriphDelay));

    base->CHREG[channel].STATIC_REG4 = regValTemp;
}

/**
 * @brief Returns ADMA Register Base Address.
 *
 * Gets the address of the selected ADMA module.
 *
 * @param dmaInstance ADMA instance to be returned.
 * @return ADMA register base address
 */
adma_type_t *ADMA_DRV_GetDmaRegBaseAddr(uint32_t dmaInstance);

#if defined(__cplusplus)
}
#endif

#endif
