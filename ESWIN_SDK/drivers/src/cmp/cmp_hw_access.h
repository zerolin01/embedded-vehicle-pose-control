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
 * @file cmp_hw_access.h
 * @brief CMP driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __CMP_HW_ACCESS_H__
#define __CMP_HW_ACCESS_H__

#include "cmp_driver.h"
#include "cmp_register.h"
#include <stdbool.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Verify if the CMP is enabled
 * @param baseAddr - cmp base pointer
 * @return - cmp state
 *  true - CMP is enabled
 *  false - CMP is disabled
 */
static inline bool CMP_GetEnableState(const cmp_type_t *baseAddr)
{
    return (bool)(((baseAddr->C0) >> CMP_C0_EN_SHIFT) & 1U);
}

/**
 * @brief Set the CMP state (enabled/disabled)
 * @param baseAddr - cmp base pointer
 * @param toSet - CMP state
 *  true - CMP is enabled
 *  false - CMP is disabled
 */
static inline void CMP_SetEnableState(cmp_type_t *baseAddr, bool toSet)
{
    (baseAddr->C0) = ((baseAddr->C0) & (~(CMP_C0_EN_MASK))) | ((uint32_t)toSet << CMP_C0_EN_SHIFT);
}

/**
 * @brief Gets the comparator functional mode. If you want to get filter count and filter period please use
 * CMP_GetFilterSamplePeriod and CMP_GetSamplingState.
 * @param baseAddr - cmp base pointer
 * @return cmp_mode_t - functional mode
 *   CMP_DISABLED
 *   CMP_CONTINUOUS
 *   CMP_SAMPLED_NONFILTRED_INT_CLK
 *   CMP_SAMPLED_NONFILTRED_EXT_CLK
 *   CMP_SAMPLED_FILTRED_INT_CLK
 *   CMP_SAMPLED_FILTRED_EXT_CLK
 *   CMP_WINDOWED
 *   CMP_WINDOWED_RESAMPLED
 *   CMP_WINDOWED_FILTRED
 */
cmp_mode_t CMP_GetFunctionalMode(const cmp_type_t *baseAddr);

/**
 * @brief Sets the comparator functional mode (mode, filter count, filter period)
 * @param baseAddr - cmp base pointer
 * @param mode - functional mode
 *  CMP_DISABLED
 *  CMP_CONTINUOUS
 *  CMP_SAMPLED_NONFILTRED_INT_CLK
 *  CMP_SAMPLED_NONFILTRED_EXT_CLK
 *  CMP_SAMPLED_FILTRED_INT_CLK
 *  CMP_SAMPLED_FILTRED_EXT_CLK
 *  CMP_WINDOWED
 *  CMP_WINDOWED_RESAMPLED
 *  CMP_WINDOWED_FILTRED
 * @param filterSampleCount - number of consecutive samples that must agree prior to the comparator ouput filter
 * accepting a new output state
 * @param filterSamplePeriod - sampling period
 */
void CMP_SetFunctionalMode(cmp_type_t *baseAddr, cmp_mode_t mode, uint8_t filterSampleCount,
                           uint8_t filterSamplePeriod);

/**
 * @brief Verify if the PDMA transfer trigger is enabled
 * @param baseAddr - cmp base pointer
 * @return - PDMA transfer trigger state
 *  true - PDMA trigger is enabled
 *  false - DAM trigger is disabled
 */
static inline bool CMP_GetDMATriggerState(const cmp_type_t *baseAddr)
{
    return (bool)(((baseAddr->C0) >> (CMP_C0_DMAEN_SHIFT)) & 1U);
}

/**
 * @brief Configure the PDMA transfer trigger
 * @param baseAddr - cmp base pointer
 * @param toSet - PDMA transfer trigger state
 *  true - PDMA trigger is enabled
 *  false - DAM trigger is disabled
 */
static inline void CMP_SetDMATriggerState(cmp_type_t *baseAddr, bool toSet)
{
    baseAddr->C0 = (baseAddr->C0 & (~(CMP_C0_DMAEN_MASK))) | ((uint32_t)toSet << CMP_C0_DMAEN_SHIFT);
}

/**
 * @brief Return the comparator output interrupts source configuration(none, rising edge, falling edge or both edges)
 * @param baseAddr - cmp base pointer
 * @return cmp_output_trigger_t - comparator output interrupts configuration
 *  CMP_NO_EVENT
 *  CMP_FALLING_EDGE
 *  CMP_RISING_EDGE
 *  CMP_BOTH_EDGES
 */
static inline cmp_output_trigger_t CMP_GetOutputInterruptTrigger(const cmp_type_t *baseAddr)
{
    bool risingEnabled  = (bool)(((baseAddr->C0) >> CMP_C0_IRE_SHIFT) & 1U);
    bool fallingEnabled = (bool)(((baseAddr->C0) >> CMP_C0_IFE_SHIFT) & 1U);
    return (cmp_output_trigger_t)(((uint32_t)risingEnabled << (uint32_t)1U) | ((uint32_t)fallingEnabled));
}

/**
 * @brief Set the comparator output interrupts source configuration(none, rising edge, falling edge or both edges)
 * @param baseAddr - cmp base pointer
 * @param toSet - comparator output interrupts configuration
 *  CMP_NO_EVENT
 *  CMP_FALLING_EDGE
 *  CMP_RISING_EDGE
 *  CMP_BOTH_EDGES
 */
static inline void CMP_SetOutputInterruptTrigger(cmp_type_t *baseAddr, cmp_output_trigger_t toSet)
{
    uint32_t tmp = baseAddr->C0;
    tmp &= ~(CMP_C0_IRE_MASK) & ~(CMP_C0_IFE_MASK);
    tmp |= CMP_C0_IRE((uint32_t)toSet >> 1) | CMP_C0_IFE((uint32_t)toSet & (uint32_t)0x01);
    baseAddr->C0 = tmp;
}

/**
 * @brief Return the sample period for filter(clock cycles)
 * @param baseAddr - cmp base pointer
 * @return uint8_t - sampling period(in bus cycles)
 */
static inline uint8_t CMP_GetFilterSamplePeriod(const cmp_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->C0;
    tmp          = (tmp & CMP_C0_FSP_MASK) >> CMP_C0_FSP_SHIFT;
    return (uint8_t)(tmp);
}

/**
 * @brief Set the filter sample period(clock cycles)
 * @param baseAddr -cmp base pointer
 * @param toSet - number of bus cycles
 */
static inline void CMP_SetFilterSamplePeriod(cmp_type_t *baseAddr, uint8_t toSet)
{
    uint32_t tmp = baseAddr->C0;
    baseAddr->C0 &= ~(CMP_C0_FSP_MASK);
    tmp &= ~(CMP_C0_FSP_MASK);
    tmp |= CMP_C0_FSP(toSet);
    baseAddr->C0 = tmp;
}

/**
 * @brief Return the current power mode
 * @param baseAddr - cmp base pointer
 * @return cmp_power_mode_t - current power mode
 *  CMP_LOW_SPEED
 *  CMP_HIGH_SPEED
 */
static inline cmp_power_mode_t CMP_GetPowerMode(const cmp_type_t *baseAddr)
{
    return (cmp_power_mode_t)(((baseAddr->C0) >> CMP_C0_PMODE_SHIFT) & 1U);
}

/**
 * @brief Set the power mode
 * @param baseAddr - cmp base pointer
 * @param toSet - power mode
 *  CMP_LOW_SPEED
 *  CMP_HIGH_SPEED
 */
static inline void CMP_SetPowerMode(cmp_type_t *baseAddr, cmp_power_mode_t toSet)
{
    (baseAddr->C0) = ((baseAddr->C0) & (~(CMP_C0_PMODE_MASK))) | ((uint32_t)toSet << CMP_C0_PMODE_SHIFT);
}

/**
 * @brief Return the current comparator output inverter
 * @param baseAddr cmp base pointer
 * @return cmp_inverter_t - inverter state
 *  CMP_NORMAL
 *  CMP_INVERT
 */
static inline cmp_inverter_t CMP_GetInverterState(const cmp_type_t *baseAddr)
{
    return (cmp_inverter_t)(((baseAddr->C0) >> CMP_C0_INVT_SHIFT) & 1U);
}

/**
 * @brief Configure the comparator output inverter mode
 * @param baseAddr - cmp base pointer
 * @param toSet - comparator output inverter mode
 *  CMP_NORMAL
 *  CMP_INVERT
 */
static inline void CMP_SetInverterState(cmp_type_t *baseAddr, cmp_inverter_t toSet)
{
    (baseAddr->C0) = ((baseAddr->C0) & (~(CMP_C0_INVT_MASK))) | ((uint32_t)toSet << CMP_C0_INVT_SHIFT);
}

/**
 * @brief Return the current comparator output selected
 * @param baseAddr - cmp base pointer
 * @return cmp_output_select_t - comparator output signal source
 *  CMP_COUT
 *  CMP_COUTA
 */
static inline cmp_output_select_t CMP_GetComparatorOutputSource(const cmp_type_t *baseAddr)
{
    return (cmp_output_select_t)(((baseAddr->C0) >> CMP_C0_COS_SHIFT) & 1U);
}

/**
 * @brief Select the comparator output signal source
 * @param baseAddr - cmp base pointer
 * @param toSet - comparator output signal source
 *  CMP_COUT
 *  CMP_COUTA
 */
static inline void CMP_SetComparatorOutputSource(cmp_type_t *baseAddr, cmp_output_select_t toSet)
{
    (baseAddr->C0) = ((baseAddr->C0) & (~(CMP_C0_COS_MASK))) | ((uint32_t)toSet << CMP_C0_COS_SHIFT);
}

/**
 * @brief Verify if the comparator output state(available/not available in a packaged pin)
 * @param baseAddr - cmp base pointer
 * @return cmp_output_enable_t - comparator output state
 *  CMP_UNAVAILABLE
 *  CMP_AVAILABLE
 */
static inline cmp_output_enable_t CMP_GetOutputPinState(const cmp_type_t *baseAddr)
{
    return (cmp_output_enable_t)(((baseAddr->C0) >> CMP_C0_COPE_SHIFT) & 1U);
}

/**
 * @brief Set the comparator output pin state(available/not available in a packaged pin)
 * @param baseAddr - cmp base pointer
 * @param toSet - comparator output state
 *  CMP_UNAVAILABLE
 *  CMP_AVAILABLE
 */
static inline void CMP_SetOutputPinState(cmp_type_t *baseAddr, cmp_output_enable_t toSet)
{

    (baseAddr->C0) = ((baseAddr->C0) & (~(CMP_C0_COPE_MASK))) | ((uint32_t)toSet << CMP_C0_COPE_SHIFT);
}

/**
 * @brief Return the number of consecutive samples that must agree prior to the comparator output filter
accepting a new output state
 * @param baseAddr - cmp base pointer
 * @return uint8_t - filter sample count
 */
static inline uint8_t CMP_GetFilterSampleCount(const cmp_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->C0;
    tmp          = (tmp & CMP_C0_FILTER_CNT_MASK) >> CMP_C0_FILTER_CNT_SHIFT;
    return (uint8_t)(tmp);
}

/**
 * @brief Set the number of consecutive samples that must agree prior to the comparator output filter
accepting a new output state
 * @param baseAddr - cmp base pointer
 * @param toSet - filter sample count(min value 0, max value 7)
 */
static inline void CMP_SetFilterSampleCount(cmp_type_t *baseAddr, uint8_t toSet)
{
    uint32_t tmp = baseAddr->C0;
    baseAddr->C0 &= ~(CMP_C0_FILTER_CNT_MASK);
    tmp &= ~(CMP_C0_FILTER_CNT_MASK);
    tmp |= CMP_C0_FILTER_CNT(toSet);
    baseAddr->C0 = tmp;
}

/**
 * @brief Return the current hysteresis level
 * @param baseAddr - cmp base pointer
 * @return cmp_hysteresis_t - current hysteresis level
 *  CMP_LEVEL_HYS_0
 *  CMP_LEVEL_HYS_1
 *  CMP_LEVEL_HYS_2
 *  CMP_LEVEL_HYS_3
 */
static inline cmp_hysteresis_t CMP_GetHysteresis(const cmp_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->C0;
    tmp          = (tmp & CMP_C0_HYSTCTR_MASK) >> CMP_C0_HYSTCTR_SHIFT;
    return (cmp_hysteresis_t)(tmp);
}

/**
 * @brief Set the hysteresis level
 * @param baseAddr - cmp base pointer
 * @param toSet - hysteresis level
 *  CMP_LEVEL_HYS_0
 *  CMP_LEVEL_HYS_1
 */
static inline void CMP_SetHysteresis(cmp_type_t *baseAddr, cmp_hysteresis_t toSet)
{
    uint32_t tmp = baseAddr->C0;
    tmp &= ~(CMP_C0_HYSTCTR_MASK);
    tmp |= CMP_C0_HYSTCTR(toSet);
    baseAddr->C0 = tmp;
}

/**
 * @brief Return the current source for positive port of the comparator
 * @param baseAddr - cmp base pointer
 * @return cmp_port_mux_t - signal source
 *  CMP_DAC
 *  CMP_MUX
 */
static inline cmp_port_mux_t CMP_GetPositivePortInput(const cmp_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->C1;
    tmp          = (tmp & CMP_C1_CPS_MASK) >> CMP_C1_CPS_SHIFT;
    return (cmp_port_mux_t)(tmp);
}

/**
 * @brief Set the source for positive port of the comparator
 * @param baseAddr cmp base pointer
 * @param toSet - signal source
 *  CMP_DAC
 *  CMP_MUX
 */
static inline void CMP_SetPositivePortInput(cmp_type_t *baseAddr, cmp_port_mux_t toSet)
{
    uint32_t tmp = baseAddr->C1;
    tmp &= ~(CMP_C1_CPS_MASK);
    tmp |= CMP_C1_CPS(toSet);
    baseAddr->C1 = tmp;
}

/**
 * @brief Return the current source for negative port of the comparator
 * @param baseAddr - cmp base pointer
 * @return cmp_port_mux_t - signal source
 *  CMP_DAC
 *  CMP_MUX
 */
static inline cmp_port_mux_t CMP_GetNegativePortInput(const cmp_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->C1;
    tmp          = (tmp & CMP_C1_CNS_MASK) >> CMP_C1_CNS_SHIFT;
    return (cmp_port_mux_t)(tmp);
}

/**
 * @brief Set the source for negative port of the comparator
 * @param baseAddr - cmp base pointer
 * @param toSet - signal source
 *  CMP_DAC
 *  CMP_MUX
 */
static inline void CMP_SetNegativePortInput(cmp_type_t *baseAddr, cmp_port_mux_t toSet)
{
    uint32_t tmp = baseAddr->C1;
    tmp &= ~(CMP_C1_CNS_MASK);
    tmp |= CMP_C1_CNS(toSet);
    baseAddr->C1 = tmp;
}

/**
 * @brief Return which channels are used for round-robin checker
 * @param baseAddr - cmp base pointer
 * @return cmp_ch_list_t - channels states, one bite for each channel state
 * |---------|---------|-----|---------|---------|
 * |CH7_state|CH6_state|.....|CH1_state|CH0_state|
 * |---------|---------|-----|---------|---------|
 */
static inline cmp_ch_list_t CMP_GetRoundRobinChannels(const cmp_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->C1;
    tmp          = tmp & (uint32_t)CMP_C1_CHN_MASK;
    return (cmp_ch_list_t)(tmp >> CMP_C1_CHN_SHIFT);
}

/**
 * @brief Set which channels are use for round-robin checker
 * @param baseAddr - cmp base pointer
 * @param toSet - channels states, one bite for each channel state
 * |---------|---------|-----|---------|---------|
 * |CH7_state|CH6_state|.....|CH1_state|CH0_state|
 * |---------|---------|-----|---------|---------|
 */
static inline void CMP_SetRoundRobinChannels(cmp_type_t *baseAddr, cmp_ch_list_t toSet)
{
    uint32_t tmp = baseAddr->C1;
    tmp &= ~(uint32_t)(CMP_C1_CHN_MASK);
    tmp |= (uint32_t)toSet << (uint32_t)CMP_C1_CHN_SHIFT;
    baseAddr->C1 = tmp;
}

/**
 * @brief Determine which input is selected for the plus mux
 * @param baseAddr - cmp base pointer
 * @return cmp_ch_number_t - channel for the plus mux
 */
static inline cmp_ch_number_t CMP_GetPlusMUXControl(const cmp_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->C1;
    tmp          = (tmp & CMP_C1_PS_MASK) >> CMP_C1_PS_SHIFT;
    return (cmp_ch_number_t)(tmp);
}

/**
 * @brief Select input for the plus mux
 * @param baseAddr cmp base pointer
 * @param toSet - channel for the plus mux
 */
static inline void CMP_SetPlusMuxControl(cmp_type_t *baseAddr, cmp_ch_number_t toSet)
{
    uint32_t tmp = baseAddr->C1;
    tmp &= ~(CMP_C1_PS_MASK);
    tmp |= CMP_C1_PS(toSet);
    baseAddr->C1 = tmp;
}

/**
 * @brief Determine which input is selected for the minus mux
 * @param baseAddr - cmp base pointer
 * @return cmp_ch_number_t - channel for the minus mux
 */
static inline cmp_ch_number_t CMP_GetMinusMUXControl(const cmp_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->C1;
    tmp          = (tmp & CMP_C1_NS_MASK) >> CMP_C1_NS_SHIFT;
    return (cmp_ch_number_t)(tmp);
}

/**
 * @brief Select input for the minus mux
 * @param baseAddr - cmp base pointer
 * @param toSet - channel for the minus mux
 */
static inline void CMP_SetMinusMUXControl(cmp_type_t *baseAddr, cmp_ch_number_t toSet)
{
    uint32_t tmp = baseAddr->C1;
    tmp &= ~(CMP_C1_NS_MASK);
    tmp |= CMP_C1_NS(toSet);
    baseAddr->C1 = tmp;
}

/**
 * @brief Verify if the round robin operation is enabled
 * @param baseAddr -cmp base pointer
 * @return - round-robin operation state
 *  true - round robin operation is enabled
 *  false - round robin operation is disabled
 */
static inline bool CMP_GetRoundRobinState(const cmp_type_t *baseAddr)
{
    return (bool)(((baseAddr->C2) >> CMP_C2_RRE_SHIFT) & 1U);
}

/**
 * @brief Set the round robin operation state
 * @param baseAddr cmp base pointer
 * @param toSet - round robin operation state
 *  true - round robin operation is enabled
 *  false - round robin operation is disabled
 */
static inline void CMP_SetRoundRobinState(cmp_type_t *baseAddr, bool toSet)
{
    (baseAddr->C2) = ((baseAddr->C2) & (~(CMP_C2_RRE_MASK))) | ((uint32_t)toSet << CMP_C2_RRE_SHIFT);
}

/**
 * @brief Verify if the round robin interrupt is enabled
 * @param baseAddr - cmp base pointer
 * @return - round-robin interrupt state
 *  true - round robin interrupt is enabled
 *  false - round robin interrupt is disabled
 */
static inline bool CMP_GetRoundRobinInterruptState(const cmp_type_t *baseAddr)
{
    return (bool)(((baseAddr->C2) >> CMP_C2_RRIE_SHIFT) & 1U);
}

/**
 * @brief Set the round robin interrupt state
 * @param baseAddr - cmp base pointer
 * @param toSet - round robin interrupt state
 *  true - round robin interrupt is enabled
 *  false - round robin interrupt is disabled
 */
static inline void CMP_SetRoundRobinInterruptState(cmp_type_t *baseAddr, bool toSet)
{
    (baseAddr->C2) = ((baseAddr->C2) & (~(CMP_C2_RRIE_SHIFT))) | ((uint32_t)toSet << CMP_C2_RRIE_SHIFT);
}

/**
 * @brief Return the port fixed for round-robin operation
 * @param baseAddr - cmp base pointer
 * @return cmp_fixed_port_t - fixed port
 */
static inline cmp_fixed_port_t CMP_GetFixedPort(const cmp_type_t *baseAddr)
{
    return (cmp_fixed_port_t)(((baseAddr->C2) >> CMP_C2_FMP_SHIFT) & 1U);
}

/**
 * @brief Set the fixed port for round-robin operation
 * @param baseAddr - cmp base pointer
 * @param toSet - fixed port
 *  CMP_PLUS_FIXED
 *  CMP_MINUS_FIXED
 */
static inline void CMP_SetFixedPort(cmp_type_t *baseAddr, cmp_fixed_port_t toSet)
{
    (baseAddr->C2) = ((baseAddr->C2) & (~(CMP_C2_FMP_MASK))) | ((uint32_t)toSet << CMP_C2_FMP_SHIFT);
}

/**
 * @brief Return which channel is selected for fixed mux port(as fixed reference)
 * @param baseAddr - cmp base pointer
 * @return cmp_ch_number_t - fixed channel
 *  CMP_PLUS_FIXED
 *  CMP_MINUS_FIXED
 */
static inline cmp_ch_number_t CMP_GetFixedChannel(const cmp_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->C2;
    tmp          = (tmp & CMP_C2_FCS_MASK) >> CMP_C2_FCS_SHIFT;
    return (cmp_ch_number_t)(tmp);
}

/**
 * @brief Set which channel is used as the fixed reference input for the fixed mux port
 * @param baseAddr - cmp base pointer
 * @param toSet - fixed channel
 */
static inline void CMP_SetFixedChannel(cmp_type_t *baseAddr, cmp_ch_number_t toSet)
{
    uint32_t tmp = baseAddr->C2;
    tmp &= ~(CMP_C2_FCS_MASK);
    tmp |= CMP_C2_FCS(toSet);
    baseAddr->C2 = tmp;
}

/**
 * @brief Return how many round-robin clock cycles takes sampling
 * @param baseAddr - cmp base pointer
 * @return uint8_t - number of sample clocks
 */
static inline uint8_t CMP_GetRoundRobinSamplesNumber(const cmp_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->C2;
    tmp          = (tmp & CMP_C2_NSCN_MASK) >> CMP_C2_NSCN_SHIFT;
    return (uint8_t)(tmp);
}

/**
 * @brief Set how many round-robin clock cycles takes sampling
 * @param baseAddr - cmp base pointer
 * @param toSet - number of sample clocks(min value 0, max value 3)
 */
static inline void CMP_SetRoundRobinSamplesNumber(cmp_type_t *baseAddr, uint8_t toSet)
{
    uint32_t tmp = baseAddr->C2;
    tmp &= ~(CMP_C2_NSCN_MASK);
    tmp |= CMP_C2_NSCN(toSet);
    baseAddr->C2 = tmp;
}

/**
 * @brief Return the comparator and DAC initialization delay
 * @param baseAddr - cmp base pointer
 * @return uint8_t - delay(round-robin clock period)
 */
static inline uint16_t CMP_GetInitDelay(const cmp_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->C3;
    tmp          = (tmp & CMP_C3_INITM_MASK) >> CMP_C3_INITM_SHIFT;
    return (uint16_t)(tmp);
}

/**
 * @brief Set the comparator and  DAC initialization delay
 * @param baseAddr - cmp base pointer
 * @param toSet - delay (min value 0, max value 63)
 */
static inline void CMP_SetInitDelay(cmp_type_t *baseAddr, uint16_t toSet)
{
    uint32_t tmp = baseAddr->C3;
    tmp &= ~(CMP_C3_INITM_MASK);
    tmp |= CMP_C3_INITM(toSet);
    baseAddr->C3 = tmp;
}

/**
 * @brief Return last input comparison results for all channels
 * @param baseAddr - cmp base pointer
 * @return cmp_ch_list_t - comparison results
 */
static inline cmp_ch_list_t CMP_GetLastComparisonResult(const cmp_type_t *baseAddr)
{
    uint32_t tmp = baseAddr->C2;
    tmp          = (tmp & CMP_C2_RESn_MASK) >> CMP_C2_RESn_SHIFT;
    return (cmp_ch_list_t)(tmp);
}

/**
 * @brief Defines the pre-set state of input channels.
 * @param baseAddr cmp base pointer
 * @param toSet - state
 */
static inline void CMP_SetPresetState(cmp_type_t *baseAddr, cmp_ch_list_t toSet)
{
    uint32_t tmp = baseAddr->C2;
    tmp &= ~(CMP_C2_RESn_MASK);
    tmp |= CMP_C2_RESn(toSet);
    baseAddr->C2 = tmp;
}

#if defined(__cplusplus)
}
#endif

#endif
