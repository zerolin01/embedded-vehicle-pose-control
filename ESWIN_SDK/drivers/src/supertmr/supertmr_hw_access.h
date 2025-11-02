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
 * @file supertmr_hw_access.h
 * @brief SUPERTMR driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __SUPERTMR_HW_ACCESS_H__
#define __SUPERTMR_HW_ACCESS_H__

#include "clock_driver.h"
#include "status.h"
#include "supertmr_common.h"

#include <stdbool.h>
#include <stddef.h>

/*
 * SUPERTMR
 *
 * SuperTimer Module
 *
 * Registers defined in this header file:
 * - SUPERTMR_SC - Status And Control
 * - SUPERTMR_CNT - Counter
 * - SUPERTMR_MOD - Modulo
 * - SUPERTMR_C0SC - Channel (n) Status And Control
 * - SUPERTMR_C0V - Channel (n) Value
 * - SUPERTMR_C1SC - Channel (n) Status And Control
 * - SUPERTMR_C1V - Channel (n) Value
 * - SUPERTMR_C2SC - Channel (n) Status And Control
 * - SUPERTMR_C2V - Channel (n) Value
 * - SUPERTMR_C3SC - Channel (n) Status And Control
 * - SUPERTMR_C3V - Channel (n) Value
 * - SUPERTMR_C4SC - Channel (n) Status And Control
 * - SUPERTMR_C4V - Channel (n) Value
 * - SUPERTMR_C5SC - Channel (n) Status And Control
 * - SUPERTMR_C5V - Channel (n) Value
 * - SUPERTMR_C6SC - Channel (n) Status And Control
 * - SUPERTMR_C6V - Channel (n) Value
 * - SUPERTMR_C7SC - Channel (n) Status And Control
 * - SUPERTMR_C7V - Channel (n) Value
 * - SUPERTMR_CNTIN - Counter Initial Value
 * - SUPERTMR_STATUS - Capture And Compare Status
 * - SUPERTMR_MODE - Features Mode Selection
 * - SUPERTMR_SYNC - Synchronization
 * - SUPERTMR_OUTINIT - Initial State For Channels Output
 * - SUPERTMR_OUTMASK - Output Mask
 * - SUPERTMR_COMBINE - Function For Linked Channels
 * - SUPERTMR_DEADTIME - Dead-time Insertion Control
 * - SUPERTMR_EXTTRIG - SUPERTMR External Trigger
 * - SUPERTMR_POL - Channels Polarity
 * - SUPERTMR_FMS - Fault Mode Status
 * - SUPERTMR_FILTER - Input Capture Filter Control
 * - SUPERTMR_FLTCTRL - Fault Control
 * - SUPERTMR_QDCTRL - Quadrature Decoder Control And Status
 * - SUPERTMR_CONF - Configuration
 * - SUPERTMR_FLTPOL - SUPERTMR Fault Input Polarity
 * - SUPERTMR_SYNCONF - Synchronization Configuration
 * - SUPERTMR_INVCTRL - SUPERTMR Inverting Control
 * - SUPERTMR_SWOCTRL - SUPERTMR Software Output Control
 * - SUPERTMR_PWMLOAD - SUPERTMR PWM Load
 * - SUPERTMR_HCR - Half Cycle Register
 */

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Sets the value for the half cycle reload register.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param value The 16 bit counter value
 */
static inline void SUPERTMR_DRV_SetHalfCycleValue(supertmr_type_t *const supertmrBase, uint16_t value)
{
    ((supertmrBase)->HCR) = value;
}

/**
 * @brief Sets the SUPERTMR clock source.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param clock The SUPERTMR peripheral clock selection
 *            - 00: No clock
 *            - 01: system clock
 *            - 10: fixed clock
 *            - 11: External clock
 */
static inline void SUPERTMR_DRV_SetClockSource(supertmr_type_t *const supertmrBase, supertmr_clock_source_t clock)
{
    SUPERTMR_RMW_SC(supertmrBase, SUPERTMR_SC_CLKS_MASK, SUPERTMR_SC_CLKS(clock));
}

/**
 * @brief Sets the SUPERTMR clock divider.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param ps The SUPERTMR peripheral clock pre-scale divider
 */
static inline void SUPERTMR_DRV_SetClockPs(supertmr_type_t *const supertmrBase, supertmr_clock_ps_t ps)
{
    SUPERTMR_RMW_SC(supertmrBase, SUPERTMR_SC_PS_MASK, SUPERTMR_SC_PS(ps));
}

/**
 * @brief Reads the SUPERTMR clock divider.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 *
 * @return The SUPERTMR clock pre-scale divider
 */
static inline uint8_t SUPERTMR_DRV_GetClockPs(const supertmr_type_t *supertmrBase)
{
    return (uint8_t)((((supertmrBase)->SC) & SUPERTMR_SC_PS_MASK) >> SUPERTMR_SC_PS_SHIFT);
}

/**
 * @brief  Enables the SUPERTMR peripheral timer overflow interrupt.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param state - true : Overflow interrupt enabled
 *                  - false: Overflow interrupt disabled
 */
static inline void SUPERTMR_DRV_SetTimerOverflowInt(supertmr_type_t *const supertmrBase, bool state)
{
    SUPERTMR_RMW_SC(supertmrBase, SUPERTMR_SC_TOIE_MASK, SUPERTMR_SC_TOIE(state));
}

/**
 * @brief Enable PWM channel Outputs.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param channel The SUPERTMR channel

 */
static inline void SUPERTMR_DRV_EnablePwmChannelOutputs(supertmr_type_t *const supertmrBase, uint8_t channel)
{
    SUPERTMR_RMW_SC(supertmrBase, (1UL << (channel + SUPERTMR_OUTPUT_CHANNEL_OFFSET)),
                    (1UL << (channel + SUPERTMR_OUTPUT_CHANNEL_OFFSET)));
}

/**
 * @brief Disable PWM channel Outputs.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param channel The SUPERTMR channel
 */
static inline void SUPERTMR_DRV_DisablePwmChannelOutputs(supertmr_type_t *const supertmrBase, uint8_t channel)
{
    uint32_t regValue    = ((supertmrBase)->SC);
    regValue             = regValue & (~(1UL << (channel + SUPERTMR_OUTPUT_CHANNEL_OFFSET)));
    ((supertmrBase)->SC) = (regValue);
}

/**
 * @brief Clears the timer overflow interrupt flag.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 */
static inline void SUPERTMR_DRV_ClearTimerOverflow(supertmr_type_t *const supertmrBase)
{
    SUPERTMR_RMW_SC(supertmrBase, SUPERTMR_SC_TOF_MASK, SUPERTMR_SC_TOF(0U));
}

/**
 * @brief Reads the bit that controls enabling the SUPERTMR timer overflow interrupt.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 *
 * @return State of Timer Overflow Interrupt
 *         - true : Overflow interrupt is enabled
 *         - false: Overflow interrupt is disabled
 */
static inline bool SUPERTMR_DRV_IsOverflowIntEnabled(const supertmr_type_t *supertmrBase)
{
    return ((supertmrBase->SC & SUPERTMR_SC_TOIE_MASK) >> SUPERTMR_SC_TOIE_SHIFT) != 0U;
}

/**
 * @brief Returns the SUPERTMR peripheral timer overflow interrupt flag.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 *
 * @return State of Timer Overflow Flag
 *         - true : SUPERTMR counter has overflowed
 *         - false: SUPERTMR counter has not overflowed
 */
static inline bool SUPERTMR_DRV_HasTimerOverflowed(const supertmr_type_t *supertmrBase)
{
    return ((supertmrBase->SC & SUPERTMR_SC_TOF_MASK) >> SUPERTMR_SC_TOF_SHIFT) != 0U;
}

/**
 * @brief Sets the SUPERTMR count direction bit.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param mode The Center-Aligned PWM selection
 *                 - 1U: Up counting mode
 *                 - 0U: Up down counting mode
 */
static inline void SUPERTMR_DRV_SetCpwms(supertmr_type_t *const supertmrBase, bool mode)
{
    SUPERTMR_RMW_SC(supertmrBase, SUPERTMR_SC_CPWMS_MASK, SUPERTMR_SC_CPWMS(mode));
}

/**
 * @brief Gets the SUPERTMR count direction bit.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 *
 * @return The Center-Aligned PWM selection
 *         - 1U: Up counting mode
 *         - 0U: Up down counting mode
 */
static inline bool SUPERTMR_DRV_GetCpwms(const supertmr_type_t *supertmrBase)
{
    return ((supertmrBase->SC & SUPERTMR_SC_CPWMS_MASK) >> SUPERTMR_SC_CPWMS_SHIFT) != 0U;
}

/**
 * @brief Set the SUPERTMR reload interrupt enable.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param enable - true : Reload interrupt is  enabled
 *                   - false: Reload interrupt is disabled
 */
static inline void SUPERTMR_DRV_SetReIntEnabledCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    SUPERTMR_RMW_SC(supertmrBase, SUPERTMR_SC_RIE_MASK, SUPERTMR_SC_RIE(enable));
}

/**
 * @brief Get the state whether the SUPERTMR counter reached a reload point.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 *
 * @return State of reload point
 *         - true : SUPERTMR counter reached a reload point
 *         - false: SUPERTMR counter did not reach a reload point
 */
static inline bool SUPERTMR_DRV_GetReloadFlag(const supertmr_type_t *supertmrBase)
{
    return ((supertmrBase->SC & SUPERTMR_SC_RF_MASK) >> SUPERTMR_SC_RF_SHIFT) != 0U;
}

/**
 * @brief Clears the reload flag bit.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 */
static inline void SUPERTMR_DRV_ClearReloadFlag(supertmr_type_t *const supertmrBase)
{
    SUPERTMR_RMW_SC(supertmrBase, SUPERTMR_SC_RF_MASK, SUPERTMR_SC_RF(0U));
}

/**
 * @brief Return true/false whether the reload interrupt was enabled or not
 *
 * @param supertmrBase The SUPERTMR base address pointer
 */
static inline bool SUPERTMR_DRV_IsReloadIntEnabled(const supertmr_type_t *supertmrBase)
{
    return ((supertmrBase->SC & SUPERTMR_SC_RIE_MASK) >> SUPERTMR_SC_RIE_SHIFT) != 0U;
}

/**
 * @brief Reads the SUPERTMR clock source.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 *
 * @return The SUPERTMR clock source selection
 *          - 00: No clock
 *          - 01: system clock
 *          - 10: fixed clock
 *          - 11: External clock
 */
static inline uint8_t SUPERTMR_DRV_GetClockSource(const supertmr_type_t *supertmrBase)
{
    return (uint8_t)((((supertmrBase)->SC) & SUPERTMR_SC_CLKS_MASK) >> SUPERTMR_SC_CLKS_SHIFT);
}

/**
 * @brief Sets the SUPERTMR peripheral current counter value.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param value The SUPERTMR timer counter value to be set
 */
static inline void SUPERTMR_DRV_SetCounter(supertmr_type_t *const supertmrBase, uint16_t value)
{
    SUPERTMR_RMW_CNT(supertmrBase, SUPERTMR_CNT_COUNT_MASK, SUPERTMR_CNT_COUNT(value));
}

/**
 * @brief Sets the SUPERTMR peripheral timer modulo value.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param value The value to be set to the timer modulo
 */
static inline void SUPERTMR_DRV_SetMod(supertmr_type_t *const supertmrBase, uint16_t value)
{
    SUPERTMR_RMW_MOD(supertmrBase, SUPERTMR_MOD_MOD_MASK, SUPERTMR_MOD_MOD(value));
}

/**
 * @brief Sets the SUPERTMR peripheral timer counter initial value.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param value initial value to be set
 */
static inline void SUPERTMR_DRV_SetCounterInitVal(supertmr_type_t *const supertmrBase, uint16_t value)
{
    SUPERTMR_RMW_CNTIN(supertmrBase, SUPERTMR_CNTIN_INIT_MASK, SUPERTMR_CNTIN_INIT(value));
}

/**
 * @brief Sets the SUPERTMR peripheral timer channel mode.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param channel The SUPERTMR peripheral channel number
 * @param selection The mode to be set valid value MSnB:MSnA :00, 01, 10, 11
 */
static inline void SUPERTMR_DRV_SetChnMSnBAMode(supertmr_type_t *const supertmrBase, uint8_t channel, uint8_t selection)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    /* write MSA bit */
    SUPERTMR_RMW_CnSCV_REG(supertmrBase, channel, SUPERTMR_CnSC_MSA_MASK,
                           SUPERTMR_CnSC_MSA((uint32_t)selection & 0x01U));

    /* write MSB bit */
    SUPERTMR_RMW_CnSCV_REG(supertmrBase, channel, SUPERTMR_CnSC_MSB_MASK,
                           SUPERTMR_CnSC_MSB(((uint32_t)selection & 0x02U) >> 1U));
}

/**
 * @brief Sets the SUPERTMR peripheral timer channel edge level.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param channel The SUPERTMR peripheral channel number
 * @param level ELSnB:ELSnA :00, 01, 10, 11
 */
static inline void SUPERTMR_DRV_SetChnEdgeLevel(supertmr_type_t *const supertmrBase, uint8_t channel, uint8_t level)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    /* write ELSA bit */
    SUPERTMR_RMW_CnSCV_REG(supertmrBase, channel, SUPERTMR_CnSC_ELSA_MASK, SUPERTMR_CnSC_ELSA((uint32_t)level & 0x01U));

    /* write ELSB bit */
    SUPERTMR_RMW_CnSCV_REG(supertmrBase, channel, SUPERTMR_CnSC_ELSB_MASK,
                           SUPERTMR_CnSC_ELSB(((uint32_t)level & 0x02U) >> 1U));
}

/**
 * @brief Enables the SUPERTMR peripheral timer channel(n) interrupt.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param channel The SUPERTMR peripheral channel number
 */
static inline void SUPERTMR_DRV_EnableChnInt(supertmr_type_t *const supertmrBase, uint8_t channel)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    SUPERTMR_RMW_CnSCV_REG(supertmrBase, channel, SUPERTMR_CnSC_CHIE_MASK, SUPERTMR_CnSC_CHIE(1U));
}

/**
 * @brief Disables the SUPERTMR peripheral timer channel(n) interrupt.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param channel The SUPERTMR peripheral channel number
 */
static inline void SUPERTMR_DRV_DisableChnInt(supertmr_type_t *const supertmrBase, uint8_t channel)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    SUPERTMR_RMW_CnSCV_REG(supertmrBase, channel, SUPERTMR_CnSC_CHIE_MASK, SUPERTMR_CnSC_CHIE(0U));
}

/**
 * @brief Clear the channel flag by writing a 0 to the CHF bit.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param channel The SUPERTMR peripheral channel number
 */
static inline void SUPERTMR_DRV_ClearChnEventFlag(supertmr_type_t *const supertmrBase, uint8_t channel)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    SUPERTMR_RMW_CnSCV_REG(supertmrBase, channel, SUPERTMR_CnSC_CHF_MASK, SUPERTMR_CnSC_CHF(0U));
}

/** SUPERTMR channel control*/
/**
 * @brief Sets the SUPERTMR peripheral timer channel counter value.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param channel The SUPERTMR peripheral channel number
 * @param value Counter value to be set
 */
static inline void SUPERTMR_DRV_SetChnCountVal(supertmr_type_t *const supertmrBase, uint8_t channel, uint16_t value)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    ((supertmrBase)->CONTROLS[channel].CnV) = value;
}

/**
 * @brief Get SUPERTMR channel(n) interrupt enabled or not.
 * @param supertmrBase SUPERTMR module base address
 * @param channel The SUPERTMR peripheral channel number
 */
static inline bool SUPERTMR_DRV_IsChnIntEnabled(const supertmr_type_t *supertmrBase, uint8_t channel)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    return (((supertmrBase)->CONTROLS[channel].CnSC) & SUPERTMR_CnSC_CHIE_MASK) != 0U;
}

/**
 * @brief Returns whether any event for the SUPERTMR peripheral timer channel has occurred.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param channel The SUPERTMR peripheral channel number
 *
 * @return State of channel flag
 *         - true : Event occurred
 *         - false: No event occurred.
 */
static inline bool SUPERTMR_DRV_HasChnEventOccurred(const supertmr_type_t *supertmrBase, uint8_t channel)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    return (((supertmrBase)->CONTROLS[channel].CnSC) & SUPERTMR_CnSC_CHF_MASK) != 0U;
}

/**
 * @brief Gets the SUPERTMR peripheral timer channel mode.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param channel The SUPERTMR peripheral channel number
 *
 * @return The MSnB:MSnA mode value, will be 00, 01, 10, 11
 *
 */
static inline uint8_t SUPERTMR_DRV_GetChnMode(const supertmr_type_t *supertmrBase, uint8_t channel)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);
    uint8_t retValue;

    retValue = (uint8_t)((((supertmrBase)->CONTROLS[channel].CnSC) & SUPERTMR_CnSC_MSA_MASK) >> SUPERTMR_CnSC_MSA_SHIFT);

    retValue |= (uint8_t)(((((supertmrBase)->CONTROLS[channel].CnSC) & SUPERTMR_CnSC_MSB_MASK) >> SUPERTMR_CnSC_MSB_SHIFT) << 1U);

    return retValue;
}

/**
 * @brief Writes the provided value to the OUTMASK register.
 *
 * This function will mask/unmask multiple channels.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param regVal Value to be written to the register
 */
static inline void SUPERTMR_DRV_SetOutmaskReg(supertmr_type_t *const supertmrBase, uint32_t regVal)
{
    ((supertmrBase)->OUTMASK) = regVal;
}

/**
 * @brief Sets the SUPERTMR peripheral timer channel output polarity.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param channel The SUPERTMR peripheral channel number
 * @param polarity The polarity to be set
 *            - true : The channel polarity is active low
 *            - false  : The channel polarity is active high
 */
static inline void SUPERTMR_DRV_SetChnOutputPolarityCmd(supertmr_type_t *const supertmrBase, uint8_t channel,
                                                        bool polarity)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    if (true != polarity) {
        ((supertmrBase)->POL) &= ~(1UL << channel);
    } else {
        ((supertmrBase)->POL) |= (1UL << channel);
    }
}

/**
 * @brief Sets the SUPERTMR peripheral timer channel fault input polarity.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param fltChannel The SUPERTMR peripheral channel number
 * @param polarity The polarity to be set
 *                - false : The fault input polarity is active high
 *                - true: The fault input polarity is active low
 */
static inline void SUPERTMR_DRV_SetChnFaultInputPolarityCmd(supertmr_type_t *const supertmrBase, uint8_t fltChannel,
                                                            bool polarity)
{
    OS_ASSERT(fltChannel < SUPERTMR_FAULT_CHANNELS);

    if (true == polarity) {
        ((supertmrBase)->FLTPOL) &= ~(1UL << fltChannel);
    } else {
        ((supertmrBase)->FLTPOL) |= (1UL << fltChannel);
    }
}

/**
 * @brief Enables/disables the SUPERTMR peripheral timer fault interrupt.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param state Timer fault interrupt state
 *            - true : Fault control interrupt is enable
 *            - false: Fault control interrupt is disabled
 */
static inline void SUPERTMR_DRV_SetFaultInt(supertmr_type_t *const supertmrBase, bool state)
{
    SUPERTMR_RMW_MODE(supertmrBase, SUPERTMR_MODE_FAULTIE_MASK, SUPERTMR_MODE_FAULTIE(state));
}

/**
 * @brief Return true/false whether the Fault interrupt was enabled or not
 *
 * @param supertmrBase The SUPERTMR base address pointer
 */
static inline bool SUPERTMR_DRV_IsFaultIntEnabled(const supertmr_type_t *supertmrBase)
{
    return ((supertmrBase->MODE & SUPERTMR_MODE_FAULTIE_MASK) >> SUPERTMR_MODE_FAULTIE_SHIFT) != 0U;
}

/**
 * @brief Clears all fault interrupt flags that are active.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 */
static inline void SUPERTMR_DRV_ClearFaultsIsr(supertmr_type_t *const supertmrBase)
{
    SUPERTMR_RMW_FMS(supertmrBase, SUPERTMR_FMS_FAULTF0_MASK | SUPERTMR_FMS_FAULTF_MASK,
                     SUPERTMR_FMS_FAULTF0(0U) | SUPERTMR_FMS_FAULTF(0U));
    SUPERTMR_RMW_FMS(supertmrBase, SUPERTMR_FMS_FAULTF1_MASK | SUPERTMR_FMS_FAULTF_MASK,
                     SUPERTMR_FMS_FAULTF1(0U) | SUPERTMR_FMS_FAULTF(0U));
    SUPERTMR_RMW_FMS(supertmrBase, SUPERTMR_FMS_FAULTF2_MASK | SUPERTMR_FMS_FAULTF_MASK,
                     SUPERTMR_FMS_FAULTF2(0U) | SUPERTMR_FMS_FAULTF(0U));
    SUPERTMR_RMW_FMS(supertmrBase, SUPERTMR_FMS_FAULTF3_MASK | SUPERTMR_FMS_FAULTF_MASK,
                     SUPERTMR_FMS_FAULTF3(0U) | SUPERTMR_FMS_FAULTF(0U));
}

/**
 * @brief Defines the SUPERTMR fault control mode.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param mode Fault control mode value
 * - SUPERTMR_FAULT_CONTROL_DISABLED: Fault control disabled
 * - SUPERTMR_FAULT_CONTROL_MAN_EVEN: Fault control enabled for even channel (0, 2, 4, 6) and manual fault clearing.
 * - SUPERTMR_FAULT_CONTROL_MAN_ALL : Fault control enabled for all channels and manual fault clearing is enabled.
 * - SUPERTMR_FAULT_CONTROL_AUTO_ALL: Fault control enabled for all channels and automatic fault clearing is enabled.
 */
static inline void SUPERTMR_DRV_SetFaultControlMode(supertmr_type_t *const supertmrBase, uint32_t mode)
{
    SUPERTMR_RMW_MODE(supertmrBase, SUPERTMR_MODE_FAULTM_MASK, SUPERTMR_MODE_FAULTM(mode));
}

/**
 * @brief Initializes the channels output.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param enable Initialize the channels output
 *                   - true : The channels output is initialized according to the state of OUTINIT reg
 *                   - false: No effect
 */
static inline void SUPERTMR_DRV_SetInitChnOutputCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    SUPERTMR_RMW_MODE(supertmrBase, SUPERTMR_MODE_INIT_MASK, SUPERTMR_MODE_INIT(enable));
}

/**
 * @brief Gets the SUPERTMR detected fault input.
 *
 * This function reads the status for all fault inputs
 *
 * @param supertmrBase The SUPERTMR base address pointer
 *
 * @return The fault byte
 *         - 0 : No fault condition was detected.
 *         - 1 : A fault condition was detected.
 */
static inline bool SUPERTMR_DRV_GetDetectedFaultInput(const supertmr_type_t *supertmrBase)
{
    return (supertmrBase->FMS & SUPERTMR_FMS_FAULTF_MASK) != 0U;
}

/**
 * @brief Enables or disables the SUPERTMR write protection.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param enable The SUPERTMR write protection selection
 *                   - true : Write-protection is enabled
 *                   - false: Write-protection is disabled
 */
static inline void SUPERTMR_DRV_SetWriteProtectionCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    if (enable) {
        supertmrBase->FMS = (supertmrBase->FMS & ~SUPERTMR_FMS_WPEN_MASK) | SUPERTMR_FMS_WPEN(1U);
    } else {
        supertmrBase->MODE = (supertmrBase->MODE & ~SUPERTMR_MODE_WPDIS_MASK) | SUPERTMR_MODE_WPDIS(1U);
    }
}

/**
 * @brief Enables the SUPERTMR peripheral timer group.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param enable SUPERTMR mode selection
 *                   - true : All registers including SUPERTMR-specific registers are available
 *                   - false: Only the TPM-compatible registers are available
 */
static inline void SUPERTMR_DRV_Enable(supertmr_type_t *const supertmrBase, bool enable)
{
    SUPERTMR_RMW_MODE(supertmrBase, SUPERTMR_MODE_SUPERTMREN_MASK, SUPERTMR_MODE_SUPERTMREN(enable));
}

/**
 * @brief Sets the SUPERTMR peripheral timer sync mode.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param enable PWM synchronization mode
 *                   - false: No restriction both software and hardware triggers can be used
 *                   - true : Software trigger can only be used for MOD and CnV synchronization,
 *                            hardware trigger only for OUTMASK and SUPERTMR counter synchronization.
 */
static inline void SUPERTMR_DRV_SetPwmSyncMode(supertmr_type_t *const supertmrBase, bool enable)
{
    SUPERTMR_RMW_MODE(supertmrBase, SUPERTMR_MODE_PWMSYNC_MASK, SUPERTMR_MODE_PWMSYNC(enable));
}

/**
 * @brief Enables or disables the SUPERTMR peripheral timer software trigger.
 *
 * @param supertmrBase The SUPERTMR base address pointer.
 * @param enable Software trigger selection
 *                   - true : Software trigger is selected
 *                   - false: Software trigger is not selected
 */
static inline void SUPERTMR_DRV_SetSoftwareTriggerCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    SUPERTMR_RMW_SYNC(supertmrBase, SUPERTMR_SYNC_SWSYNC_MASK, SUPERTMR_SYNC_SWSYNC(enable));
}

/**
 * @brief Sets the SUPERTMR hardware synchronization trigger.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param trigger_num Number of trigger
 *                        - 0U: trigger 0
 *                        - 1U: trigger 1
 *                        - 2U: trigger 2
 * @param enable State of trigger
 *                   - true : Enable hardware trigger from field trigger_num for PWM synchronization
 *                   - false: Disable hardware trigger from field trigger_num for PWM synchronization
 */
static inline void SUPERTMR_DRV_SetHardwareSyncTriggerSrc(supertmr_type_t *const supertmrBase, uint8_t trigger_num,
                                                          bool enable)
{
    OS_ASSERT(trigger_num < 3U);

    if (enable) {
        ((supertmrBase)->SYNC) |= ((uint32_t)(SUPERTMR_SYNC_TRIG0_MASK) << trigger_num);
    } else {
        ((supertmrBase)->SYNC) &= ~((uint32_t)(SUPERTMR_SYNC_TRIG0_MASK) << trigger_num);
    }
}

/**
 * @brief Determines when the OUTMASK register is updated with the value of its buffer.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param enable Output Mask synchronization selection
 *                   - true : OUTMASK register is updated only by PWM synchronization
 *                   - false: OUTMASK register is updated in all rising edges of the system clock
 */
static inline void SUPERTMR_DRV_SetOutmaskPwmSyncModeCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    SUPERTMR_RMW_SYNC(supertmrBase, SUPERTMR_SYNC_SYNCHOM_MASK, SUPERTMR_SYNC_SYNCHOM(enable));
}

/**
 * @brief Enables or disables the SUPERTMR peripheral timer maximum loading points.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param enable Maximum loading point selection
 *                   - true : To enable maximum loading point
 *                   - false: To disable
 */
static inline void SUPERTMR_DRV_SetMaxLoadingCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    SUPERTMR_RMW_SYNC(supertmrBase, SUPERTMR_SYNC_CNTMAX_MASK, SUPERTMR_SYNC_CNTMAX(enable));
}

/**
 * @brief Enables or disables the SUPERTMR peripheral timer minimum loading points.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param enable Minimum loading point selection
 *                   - true : To enable minimum loading point
 *                   - false: To disable
 */
static inline void SUPERTMR_DRV_SetMinLoadingCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    SUPERTMR_RMW_SYNC(supertmrBase, SUPERTMR_SYNC_CNTMIN_MASK, SUPERTMR_SYNC_CNTMIN(enable));
}

/**
 * @brief Enables the SUPERTMR peripheral timer channel modified combine mode.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param chnlPairNum The SUPERTMR peripheral channel pair number
 * @param enable State of channel pair outputs modified combine
 *                   - true : To enable modified combine
 *                   - false: To disable modified combine
 */
static inline void SUPERTMR_DRV_SetDualChnMofCombineCmd(supertmr_type_t *const supertmrBase, uint8_t chnlPairNum,
                                                        bool enable)
{
    OS_ASSERT(chnlPairNum < (SUPERTMR_CHANNEL_COUNT >> 1U));

    if (enable) {
        ((supertmrBase)->COMBINE) |= ((uint32_t)SUPERTMR_COMBINE_MCOMBINE0_MASK << (chnlPairNum * SUPERTMR_COMBINE_CHAN_CTRL_WIDTH));
    } else {
        ((supertmrBase)->COMBINE) &= ~((uint32_t)SUPERTMR_COMBINE_MCOMBINE0_MASK << (chnlPairNum * SUPERTMR_COMBINE_CHAN_CTRL_WIDTH));
    }
}

/**
 * @brief Enables the SUPERTMR peripheral timer channel pair fault control.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param chnlPairNum The SUPERTMR peripheral channel pair number
 * @param enable State of channel pair fault control
 *                   - true : To enable fault control
 *                   - false: To disable
 */
static inline void SUPERTMR_DRV_SetDualChnFaultCmd(supertmr_type_t *const supertmrBase, uint8_t chnlPairNum,
                                                   bool enable)
{
    OS_ASSERT(chnlPairNum < (SUPERTMR_CHANNEL_COUNT >> 1U));

    if (enable) {
        ((supertmrBase)->COMBINE) |= ((uint32_t)SUPERTMR_COMBINE_FAULTEN0_MASK << (chnlPairNum * SUPERTMR_COMBINE_CHAN_CTRL_WIDTH));
    } else {
        ((supertmrBase)->COMBINE) &= ~((uint32_t)SUPERTMR_COMBINE_FAULTEN0_MASK << (chnlPairNum * SUPERTMR_COMBINE_CHAN_CTRL_WIDTH));
    }
}

/**
 * @brief Enables or disables the SUPERTMR peripheral timer channel pair counter PWM sync.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param chnlPairNum The SUPERTMR peripheral channel pair number
 * @param enable State of channel pair counter PWM sync
 *                   - true : To enable PWM synchronization
 *                   - false: To disable
 */
static inline void SUPERTMR_DRV_SetDualChnPwmSyncCmd(supertmr_type_t *const supertmrBase, uint8_t chnlPairNum,
                                                     bool enable)
{
    OS_ASSERT(chnlPairNum < (SUPERTMR_CHANNEL_COUNT >> 1U));

    if (enable) {
        ((supertmrBase)->COMBINE) |= ((uint32_t)SUPERTMR_COMBINE_SYNCEN0_MASK << (chnlPairNum * SUPERTMR_COMBINE_CHAN_CTRL_WIDTH));
    } else {
        ((supertmrBase)->COMBINE) &= ~((uint32_t)SUPERTMR_COMBINE_SYNCEN0_MASK << (chnlPairNum * SUPERTMR_COMBINE_CHAN_CTRL_WIDTH));
    }
}

/**
 * @brief Enables or disabled the SUPERTMR peripheral timer channel pair deadtime insertion.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param chnlPairNum The SUPERTMR peripheral channel pair number
 * @param enable State of channel pair deadtime insertion
 *                   - true : To enable deadtime insertion
 *                   - false: To disable
 */
static inline void SUPERTMR_DRV_SetDualChnDeadtimeCmd(supertmr_type_t *const supertmrBase, uint8_t chnlPairNum,
                                                      bool enable)
{
    OS_ASSERT(chnlPairNum < SUPERTMR_CHANNEL_COUNT);

    if (enable) {
        ((supertmrBase)->COMBINE) |= ((uint32_t)SUPERTMR_COMBINE_DTEN0_MASK << (chnlPairNum * SUPERTMR_COMBINE_CHAN_CTRL_WIDTH));
    } else {
        ((supertmrBase)->COMBINE) &= ~((uint32_t)SUPERTMR_COMBINE_DTEN0_MASK << (chnlPairNum * SUPERTMR_COMBINE_CHAN_CTRL_WIDTH));
    }
}

/**
 * @brief Enables or disables the SUPERTMR peripheral timer channel dual edge capture.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param chnlPairNum The SUPERTMR peripheral channel pair number
 * @param enable State of channel dual edge capture
 *                   - true : To enable dual edge capture mode
 *                   - false: To disable
 */
static inline void SUPERTMR_DRV_SetDualChnDecapCmd(supertmr_type_t *const supertmrBase, uint8_t chnlPairNum,
                                                   bool enable)
{
    OS_ASSERT(chnlPairNum < (SUPERTMR_CHANNEL_COUNT >> 1U));

    if (enable) {
        ((supertmrBase)->COMBINE) |= ((uint32_t)SUPERTMR_COMBINE_DECAP0_MASK << (chnlPairNum * SUPERTMR_COMBINE_CHAN_CTRL_WIDTH));
    } else {
        ((supertmrBase)->COMBINE) &= ~((uint32_t)SUPERTMR_COMBINE_DECAP0_MASK << (chnlPairNum * SUPERTMR_COMBINE_CHAN_CTRL_WIDTH));
    }
}

/**
 * @brief Enables the SUPERTMR peripheral timer dual edge capture mode.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param chnlPairNum The SUPERTMR peripheral channel pair number
 * @param enable State of dual edge capture mode
 *                   - true : To enable dual edge capture
 *                   - false: To disable
 */
static inline void SUPERTMR_DRV_SetDualEdgeCaptureCmd(supertmr_type_t *const supertmrBase, uint8_t chnlPairNum,
                                                      bool enable)
{
    OS_ASSERT(chnlPairNum < (SUPERTMR_CHANNEL_COUNT >> 1U));

    if (enable) {
        ((supertmrBase)->COMBINE) |= ((uint32_t)SUPERTMR_COMBINE_DECAPEN0_MASK << (chnlPairNum * SUPERTMR_COMBINE_CHAN_CTRL_WIDTH));
    } else {
        ((supertmrBase)->COMBINE) &= ~((uint32_t)SUPERTMR_COMBINE_DECAPEN0_MASK << (chnlPairNum * SUPERTMR_COMBINE_CHAN_CTRL_WIDTH));
    }
}

/**
 * @brief Enables the SUPERTMR peripheral timer dual edge capture mode.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param chnlPairNum The SUPERTMR peripheral channel pair number
 *
 * @return Dual edge capture mode status
 *         - true : To enable dual edge capture
 *         - false: To disable
 */
static inline bool SUPERTMR_DRV_GetDualEdgeCaptureBit(const supertmr_type_t *supertmrBase, uint8_t chnlPairNum)
{
    OS_ASSERT(chnlPairNum < (SUPERTMR_CHANNEL_COUNT >> 1U));

    return (((supertmrBase)->COMBINE) & ((uint32_t)SUPERTMR_COMBINE_DECAPEN0_MASK << (chnlPairNum * SUPERTMR_COMBINE_CHAN_CTRL_WIDTH))) != 0U;
}

/**
 * @brief Enables or disables the SUPERTMR peripheral timer channel pair output complement mode.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param chnlPairNum The SUPERTMR peripheral channel pair number
 * @param polarity State of channel pair output complement mode
 *            - true : The channel (n+1) output is the complement of the channel (n) output
 *            - false: The channel (n+1) output is the same as the channel (n) output
 */
static inline void SUPERTMR_DRV_SetDualChnCompCmd(supertmr_type_t *const supertmrBase, uint8_t chnlPairNum,
                                                  bool polarity)
{
    OS_ASSERT(chnlPairNum < (SUPERTMR_CHANNEL_COUNT >> 1U));

    if (true == polarity) {
        ((supertmrBase)->COMBINE) |= (SUPERTMR_COMBINE_COMP0_MASK << ((uint32_t)(chnlPairNum)*SUPERTMR_COMBINE_CHAN_CTRL_WIDTH));
    } else {
        ((supertmrBase)->COMBINE) &= ~(SUPERTMR_COMBINE_COMP0_MASK << ((uint32_t)(chnlPairNum)*SUPERTMR_COMBINE_CHAN_CTRL_WIDTH));
    }
}

/**
 * @brief Enables or disables the SUPERTMR peripheral timer channel pair output combine mode.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param chnlPairNum The SUPERTMR peripheral channel pair number
 * @param enable State of channel pair output combine mode
 *                   - true : Channels pair are combined
 *                   - false: Channels pair are independent
 */
static inline void SUPERTMR_DRV_SetDualChnCombineCmd(supertmr_type_t *const supertmrBase, uint8_t chnlPairNum,
                                                     bool enable)
{
    OS_ASSERT(chnlPairNum < (SUPERTMR_CHANNEL_COUNT >> 1U));

    if (enable) {
        ((supertmrBase)->COMBINE) |= (SUPERTMR_COMBINE_COMBINE0_MASK << ((uint32_t)(chnlPairNum)*SUPERTMR_COMBINE_CHAN_CTRL_WIDTH));
    } else {
        ((supertmrBase)->COMBINE) &= ~(SUPERTMR_COMBINE_COMBINE0_MASK << ((uint32_t)(chnlPairNum)*SUPERTMR_COMBINE_CHAN_CTRL_WIDTH));
    }
}

/**
 * @brief Verify if an channels pair is used in combine mode or not.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param chnlPairNum The SUPERTMR peripheral channel pair number
 *
 * @return Channel pair output combine mode status
 *         - true : Channels pair are combined
 *         - false: Channels pair are independent
 */
static inline bool SUPERTMR_DRV_GetDualChnCombineCmd(const supertmr_type_t *supertmrBase, uint8_t chnlPairNum)
{
    OS_ASSERT(chnlPairNum < (SUPERTMR_CHANNEL_COUNT >> 1U));

    return (((supertmrBase)->COMBINE) & (SUPERTMR_COMBINE_COMBINE0_MASK
                                         << ((uint32_t)(chnlPairNum)*SUPERTMR_COMBINE_CHAN_CTRL_WIDTH))) != 0U;
}

/**
 * @brief Verify if an channels pair is used in the modified combine mode or not.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param chnlPairNum The SUPERTMR peripheral channel pair number
 *
 * @return Channel pair output combine mode status
 *         - true : Channels pair are combined
 *         - false: Channels pair are independent
 */
static inline bool SUPERTMR_DRV_GetDualChnMofCombineCmd(const supertmr_type_t *supertmrBase, uint8_t chnlPairNum)
{
    OS_ASSERT(chnlPairNum < (SUPERTMR_CHANNEL_COUNT >> 1U));

    return (((supertmrBase)->COMBINE) & (SUPERTMR_COMBINE_MCOMBINE0_MASK
                                         << ((uint32_t)(chnlPairNum)*SUPERTMR_COMBINE_CHAN_CTRL_WIDTH))) != 0U;
}

/**
 * @brief Sets the SUPERTMR extended dead-time value.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param value The SUPERTMR peripheral extend pre-scale divider
 */
static inline void SUPERTMR_DRV_SetExtDeadtimeValue(supertmr_type_t *const supertmrBase, uint8_t value)
{
    OS_ASSERT(value < 16U);

    SUPERTMR_RMW_DEADTIME(supertmrBase, SUPERTMR_DEADTIME_DTVALEX_MASK, SUPERTMR_DEADTIME_DTVALEX(value));
}

/**
 * @brief Sets the SUPERTMR dead time divider.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param divider The SUPERTMR peripheral pre-scaler divider
 *                    - SUPERTMR_DEADTIME_DIVID_BY_1 : Divide by 1
 *                    - SUPERTMR_DEADTIME_DIVID_BY_4 : Divide by 4
 *                    - SUPERTMR_DEADTIME_DIVID_BY_16: Divide by 16
 */
static inline void SUPERTMR_DRV_SetDeadtimePrescale(supertmr_type_t *const supertmrBase, supertmr_deadtime_ps_t divider)
{
    SUPERTMR_RMW_DEADTIME(supertmrBase, SUPERTMR_DEADTIME_DTPS_MASK, SUPERTMR_DEADTIME_DTPS((uint8_t)divider));
}

/**
 * @brief Sets the SUPERTMR deadtime value.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param count The SUPERTMR peripheral pre-scaler divider
 *                  - 0U : no counts inserted
 *                  - 1U : 1 count is inserted
 *                  - 2U : 2 count is inserted
 *                  - ... up to a possible 63 counts
 */
static inline void SUPERTMR_DRV_SetDeadtimeCount(supertmr_type_t *const supertmrBase, uint8_t count)
{
    OS_ASSERT(count < 64U);

    SUPERTMR_RMW_DEADTIME(supertmrBase, SUPERTMR_DEADTIME_DTVAL_MASK, SUPERTMR_DEADTIME_DTVAL(count));
}

/** SUPERTMR external trigger */
/**
 * @brief Enables or disables the generation of the trigger when the SUPERTMR counter is equal
 * to the CNTIN register.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param enable State of initialization trigger
 *                   - true : To enable
 *                   - false: To disable
 */
static inline void SUPERTMR_DRV_SetInitTriggerCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    supertmrBase->EXTTRIG = (supertmrBase->EXTTRIG & ~SUPERTMR_EXTTRIG_INITTRIGEN_MASK) | SUPERTMR_EXTTRIG_INITTRIGEN(enable);
}

/**
 * @brief Checks whether any channel trigger event has occurred.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @return Channel trigger status
 *         - true : If there is a channel trigger event
 *         - false: If not.
 */
static inline bool SUPERTMR_DRV_IsChnTriggerGenerated(const supertmr_type_t *supertmrBase)
{
    return (supertmrBase->EXTTRIG & SUPERTMR_EXTTRIG_TRIGF_MASK) != 0U;
}

/**
 * @brief Clear the channel trigger flag.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 */
static inline void SUPERTMR_DRV_ClearChnTriggerFlag(supertmr_type_t *const supertmrBase)
{
    SUPERTMR_RMW_EXTTRIG_REG(supertmrBase, SUPERTMR_EXTTRIG_TRIGF_MASK, SUPERTMR_EXTTRIG_TRIGF(0UL));
}

/** Quadrature decoder control */
/**
 * @brief Enables the channel quadrature decoder.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param enable State of channel quadrature decoder
 *                  - true : To enable
 *                  - false: To disable
 */
static inline void SUPERTMR_DRV_SetQuadDecoderCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    if (enable) {
        ((supertmrBase)->QDCTRL) |= (1UL << SUPERTMR_QDCTRL_QUADEN_SHIFT);
    } else {
        ((supertmrBase)->QDCTRL) &= ~(1UL << SUPERTMR_QDCTRL_QUADEN_SHIFT);
    }
}

/**
 * @brief Enables or disables the phase A input filter.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param enable State of phase A input filter
 *                   - true : Enables the phase input filter
 *                   - false: Disables the filter
 */
static inline void SUPERTMR_DRV_SetQuadPhaseAFilterCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    if (enable) {
        ((supertmrBase)->QDCTRL) |= (1UL << SUPERTMR_QDCTRL_PHAFLTREN_SHIFT);
    } else {
        ((supertmrBase)->QDCTRL) &= ~(1UL << SUPERTMR_QDCTRL_PHAFLTREN_SHIFT);
    }
}

/**
 * @brief Selects polarity for the quadrature decode phase A input.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param mode Phase A input polarity selection
 *                - 0U: Normal polarity
 *                - 1U: Inverted polarity
 */
static inline void SUPERTMR_DRV_SetQuadPhaseAPolarity(supertmr_type_t *const supertmrBase, uint8_t mode)
{
    SUPERTMR_RMW_QDCTRL(supertmrBase, SUPERTMR_QDCTRL_PHAPOL_MASK, SUPERTMR_QDCTRL_PHAPOL(mode));
}

/**
 * @brief Selects polarity for the quadrature decode phase B input.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param mode Phase B input polarity selection
 *                - 0U: Normal polarity
 *                - 1U: Inverted polarity
 */
static inline void SUPERTMR_DRV_SetQuadPhaseBPolarity(supertmr_type_t *const supertmrBase, uint8_t mode)
{
    SUPERTMR_RMW_QDCTRL(supertmrBase, SUPERTMR_QDCTRL_PHBPOL_MASK, SUPERTMR_QDCTRL_PHBPOL(mode));
}

/**
 * @brief Sets the encoding mode used in quadrature decoding mode.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param quadMode Quadrature decoder mode selection
 *                     - 0U: Phase A and Phase B encoding mode
 *                     - 1U: Count and direction encoding mode
 */
static inline void SUPERTMR_DRV_SetQuadMode(supertmr_type_t *const supertmrBase, uint8_t quadMode)
{
    SUPERTMR_RMW_QDCTRL(supertmrBase, SUPERTMR_QDCTRL_QUADMODE_MASK, SUPERTMR_QDCTRL_QUADMODE(quadMode));
}

/**
 * @brief Gets the SUPERTMR counter direction in quadrature mode.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 *
 * @return The counting direction
 *         - 1U: if counting direction is increasing
 *         - 0U: if counting direction is decreasing
 */
static inline bool SUPERTMR_DRV_GetQuadDir(const supertmr_type_t *supertmrBase)
{
    return (supertmrBase->QDCTRL & SUPERTMR_QDCTRL_QUADIR_MASK) != 0U;
}

/**
 * @brief Gets the Timer overflow direction in quadrature mode.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 *
 * @return The timer overflow direction
 *         - 1U: if TOF bit was set on the top of counting
 *         - 0U: if TOF bit was set on the bottom of counting
 */
static inline bool SUPERTMR_DRV_GetQuadTimerOverflowDir(const supertmr_type_t *supertmrBase)
{
    return (supertmrBase->QDCTRL & SUPERTMR_QDCTRL_TOFDIR_MASK) != 0U;
}

/**
 * @brief Enables or disables the phase B input filter.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param enable State of phase B input filter
 *                   - true : Enables the phase input filter
 *                   - false: Disables the filter
 */
static inline void SUPERTMR_DRV_SetQuadPhaseBFilterCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    if (enable) {
        ((supertmrBase)->QDCTRL) |= (1UL << SUPERTMR_QDCTRL_PHBFLTREN_SHIFT);
    } else {
        ((supertmrBase)->QDCTRL) &= ~(1UL << SUPERTMR_QDCTRL_PHBFLTREN_SHIFT);
    }
}

/**
 * @brief Sets the fault input filter value.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param value Fault input filter value
 */
static inline void SUPERTMR_DRV_SetFaultInputFilterVal(supertmr_type_t *const supertmrBase, uint32_t value)
{
    SUPERTMR_RMW_FLTCTRL(supertmrBase, SUPERTMR_FLTCTRL_FFVAL_MASK, SUPERTMR_FLTCTRL_FFVAL(value));
}

/**
 * @brief Enables or disables the fault input filter.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param inputNum Fault input to be configured, valid value 0, 1, 2, 3
 * @param enable State of fault input filter
 *                   - true : To enable fault input filter
 *                   - false: To disable fault input filter
 */
static inline void SUPERTMR_DRV_SetFaultInputFilterCmd(supertmr_type_t *const supertmrBase, uint8_t inputNum,
                                                       bool enable)
{
    OS_ASSERT(inputNum < CHAN4_IDX);

    if (enable) {
        ((supertmrBase)->FLTCTRL) |= (1UL << (inputNum + SUPERTMR_FLTCTRL_FFLTR0EN_SHIFT));
    } else {
        ((supertmrBase)->FLTCTRL) &= ~(1UL << (inputNum + SUPERTMR_FLTCTRL_FFLTR0EN_SHIFT));
    }
}

/**
 * @brief Clears the entire content value of the Fault control register.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 */
static inline void SUPERTMR_DRV_ClearFaultControl(supertmr_type_t *const supertmrBase)
{
    ((supertmrBase)->FLTCTRL) = 0U;
}

/**
 * @brief Enables or disables the fault input.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param inputNum fault input to be configured, valid value 0, 1, 2, 3
 * @param enable State of fault input
 *                   - true : To enable fault input
 *                   - false: To disable fault input
 */
static inline void SUPERTMR_DRV_SetFaultInputCmd(supertmr_type_t *const supertmrBase, uint8_t inputNum, bool enable)
{
    OS_ASSERT(inputNum < CHAN4_IDX);

    if (enable) {
        ((supertmrBase)->FLTCTRL) |= (1UL << inputNum);
    } else {
        ((supertmrBase)->FLTCTRL) &= ~(1UL << inputNum);
    }
}

/**
 * @brief Configures the behavior of the PWM outputs when a fault is detected
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param enable State of fault output
 *                   - true : Output pins are set tri-state,
 *                   - false: Pins are set to a safe state determined by POL bits
 */
static inline void SUPERTMR_DRV_SetPwmFaultBehavior(supertmr_type_t *const supertmrBase, bool enable)
{
    if (enable) {
        ((supertmrBase)->FLTCTRL) |= (1UL << SUPERTMR_FLTCTRL_FSTATE_SHIFT);
    } else {
        ((supertmrBase)->FLTCTRL) &= ~(1UL << SUPERTMR_FLTCTRL_FSTATE_SHIFT);
    }
}

/**
 * @brief Writes the provided value to the Inverting control register.
 *
 * This function is enable/disable inverting control on multiple channel pairs.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param regVal Value to be written to the register
 */
static inline void SUPERTMR_DRV_SetInvctrlReg(supertmr_type_t *const supertmrBase, uint32_t regVal)
{
    ((supertmrBase)->INVCTRL) = regVal;
}

/** SUPERTMR software output control*/
/**
 * @brief Enables or disables the channel software output control.The
 * main difference between this function and SUPERTMR_HAL_SetChnSoftwareCtrlCmd
 * is that this can configure all the channels in the same time.
 * @param supertmrBase The SUPERTMR base address pointer
 * @param channelsMask Channels to be enabled or disabled
 */
static inline void SUPERTMR_DRV_SetAllChnSoftwareCtrlCmd(supertmr_type_t *const supertmrBase, uint8_t channelsMask)
{
    uint32_t mask = SUPERTMR_SWOCTRL_CH0OC_MASK | SUPERTMR_SWOCTRL_CH1OC_MASK | SUPERTMR_SWOCTRL_CH2OC_MASK |
                    SUPERTMR_SWOCTRL_CH3OC_MASK | SUPERTMR_SWOCTRL_CH4OC_MASK | SUPERTMR_SWOCTRL_CH5OC_MASK |
                    SUPERTMR_SWOCTRL_CH6OC_MASK | SUPERTMR_SWOCTRL_CH7OC_MASK;
    ((supertmrBase)->SWOCTRL) = (((supertmrBase)->SWOCTRL) & (~(mask))) | channelsMask;
}

/**
 * @brief Sets the channel software output control value.
 *
 * @param supertmrBase The SUPERTMR base address pointer.
 * @param channelsValues The values which will overwrite the output channels
 */
static inline void SUPERTMR_DRV_SetAllChnSoftwareCtrlVal(supertmr_type_t *const supertmrBase, uint8_t channelsValues)
{
    uint32_t mask = SUPERTMR_SWOCTRL_CH0OCV_MASK | SUPERTMR_SWOCTRL_CH1OCV_MASK | SUPERTMR_SWOCTRL_CH2OCV_MASK |
                    SUPERTMR_SWOCTRL_CH3OCV_MASK | SUPERTMR_SWOCTRL_CH4OCV_MASK | SUPERTMR_SWOCTRL_CH5OCV_MASK |
                    SUPERTMR_SWOCTRL_CH6OCV_MASK | SUPERTMR_SWOCTRL_CH7OCV_MASK;
    ((supertmrBase)->SWOCTRL) = (((supertmrBase)->SWOCTRL) & (~(mask))) | ((uint32_t)channelsValues << SUPERTMR_SWOCTRL_CH0OCV_SHIFT);
}

/**
 * @brief Enable and Force the software control of channels output at once.
 *
 * @param supertmrBase The SUPERTMR base address pointer.
 * @param value The values which enables and force the software control of channels output
 */
static inline void SUPERTMR_DRV_SoftwareOutputControl(supertmr_type_t *const supertmrBase, uint16_t value)
{
    ((supertmrBase)->SWOCTRL) = value;
}

/**
 * @brief Sets the BDM mode.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param val The SUPERTMR behavior in BDM mode
 *                - SUPERTMR_BDM_MODE_00: SUPERTMR counter stopped, CH(n)F bit can be set, SUPERTMR channels
 *                                   in functional mode, writes to MOD,CNTIN and C(n)V registers bypass
 *                                   the register buffers
 *                - SUPERTMR_BDM_MODE_01: SUPERTMR counter stopped, CH(n)F bit is not set, SUPERTMR channels
 *                                   outputs are forced to their safe value , writes to MOD,CNTIN and
 *                                   C(n)V registers bypass the register buffers
 *                - SUPERTMR_BDM_MODE_10: SUPERTMR counter stopped, CH(n)F bit is not set, SUPERTMR channels
 *                                   outputs are frozen when chip enters in BDM mode, writes to MOD,
 *                                   CNTIN and C(n)V registers bypass the register buffers
 *                - SUPERTMR_BDM_MODE_11: SUPERTMR counter in functional mode, CH(n)F bit can be set,
 *                                   SUPERTMR channels in functional mode, writes to MOD,CNTIN and C(n)V
 *                                   registers is in fully functional mode
 */
static inline void SUPERTMR_DRV_SetBdmMode(supertmr_type_t *const supertmrBase, supertmr_bdm_mode_t val)
{
    SUPERTMR_RMW_CONF(supertmrBase, SUPERTMR_CONF_BDMMODE_MASK, SUPERTMR_CONF_BDMMODE(val));
}

/** SUPERTMR Synchronization configuration*/
/**
 * @brief Sets the sync mode for the SUPERTMR SWOCTRL register when using a hardware trigger.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param enable State of software output control synchronization
 *                   - true : The hardware trigger activates SWOCTRL register sync
 *                   - false: The hardware trigger does not activate SWOCTRL register sync
 */
static inline void SUPERTMR_DRV_SetSwoctrlHardwareSyncModeCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    supertmrBase->SYNCONF = (supertmrBase->SYNCONF & ~SUPERTMR_SYNCONF_HWSOC_MASK) | SUPERTMR_SYNCONF_HWSOC(enable);
}

/**
 * @brief Sets sync mode for SUPERTMR INVCTRL register when using a hardware trigger.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param enable State of inverting control synchronization
 *                   - true : The hardware trigger activates INVCTRL register sync
 *                   - false: The hardware trigger does not activate INVCTRL register sync
 */
static inline void SUPERTMR_DRV_SetInvctrlHardwareSyncModeCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    supertmrBase->SYNCONF = (supertmrBase->SYNCONF & ~SUPERTMR_SYNCONF_HWINVC_MASK) | SUPERTMR_SYNCONF_HWINVC(enable);
}

/**
 * @brief Sets sync mode for SUPERTMR OUTMASK register when using a hardware trigger.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param enable State of output mask synchronization
 *                   - true : The hardware trigger activates OUTMASK register sync
 *                   - false: The hardware trigger does not activate OUTMASK register sync
 */
static inline void SUPERTMR_DRV_SetOutmaskHardwareSyncModeCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    supertmrBase->SYNCONF = (supertmrBase->SYNCONF & ~SUPERTMR_SYNCONF_HWOM_MASK) | SUPERTMR_SYNCONF_HWOM(enable);
}

/**
 * @brief Sets sync mode for SUPERTMR MOD, CNTIN and CV registers when using a hardware trigger.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param enable State of registers synchronization
 *                   - true : The hardware trigger activates  MOD, HCR, CNTIN, and CV registers sync
 *                   - false: The hardware trigger does not activate MOD, HCR, CNTIN, and CV registers sync
 */
static inline void SUPERTMR_DRV_SetModCntinCvHardwareSyncModeCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    supertmrBase->SYNCONF = (supertmrBase->SYNCONF & ~SUPERTMR_SYNCONF_HWWRBUF_MASK) | SUPERTMR_SYNCONF_HWWRBUF(enable);
}

/**
 * @brief Sets sync mode for SUPERTMR counter register when using a hardware trigger.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param enable State of SUPERTMR counter synchronization
 *                   - true : The hardware trigger activates SUPERTMR counter sync
 *                   - false: The hardware trigger does not activate SUPERTMR counter sync
 */
static inline void SUPERTMR_DRV_SetCounterHardwareSyncModeCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    supertmrBase->SYNCONF = (supertmrBase->SYNCONF & ~SUPERTMR_SYNCONF_HWRSTCNT_MASK) | SUPERTMR_SYNCONF_HWRSTCNT(enable);
}

/**
 * @brief Sets sync mode for SUPERTMR SWOCTRL register when using a software trigger.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param enable State of software output control synchronization
 *                   - true : The software trigger activates SWOCTRL register sync
 *                   - false: The software trigger does not activate SWOCTRL register sync
 */
static inline void SUPERTMR_DRV_SetSwoctrlSoftwareSyncModeCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    supertmrBase->SYNCONF = (supertmrBase->SYNCONF & ~SUPERTMR_SYNCONF_SWSOC_MASK) | SUPERTMR_SYNCONF_SWSOC(enable);
}

/**
 * @brief Sets sync mode for SUPERTMR INVCTRL register when using a software trigger.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param enable State of State of inverting control synchronization
 *                   - true : The software trigger activates INVCTRL register sync
 *                   - false: The software trigger does not activate INVCTRL register sync
 */
static inline void SUPERTMR_DRV_SetInvctrlSoftwareSyncModeCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    supertmrBase->SYNCONF = (supertmrBase->SYNCONF & ~SUPERTMR_SYNCONF_SWINVC_MASK) | SUPERTMR_SYNCONF_SWINVC(enable);
}

/**
 * @brief Sets sync mode for SUPERTMR OUTMASK register when using a software trigger.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param enable State of output mask synchronization
 *                   - true : The software trigger activates OUTMASK register sync
 *                   - false: The software trigger does not activate OUTMASK register sync
 */
static inline void SUPERTMR_DRV_SetOutmaskSoftwareSyncModeCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    supertmrBase->SYNCONF = (supertmrBase->SYNCONF & ~SUPERTMR_SYNCONF_SWOM_MASK) | SUPERTMR_SYNCONF_SWOM(enable);
}

/**
 * @brief Sets sync mode for SUPERTMR MOD, CNTIN and CV registers when using a software trigger.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param enable State of registers synchronization
 *                   - true : The software trigger activates SUPERTMR MOD, CNTIN and CV registers sync
 *                   - false: The software trigger does not activate SUPERTMR MOD, CNTIN and CV registers sync
 */
static inline void SUPERTMR_DRV_SetModCntinCvSoftwareSyncModeCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    supertmrBase->SYNCONF = (supertmrBase->SYNCONF & ~SUPERTMR_SYNCONF_SWWRBUF_MASK) | SUPERTMR_SYNCONF_SWWRBUF(enable);
}

/**
 * @brief Sets hardware trigger mode.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param enable State of hardware trigger mode
 *                   - true : SUPERTMR does not clear the TRIGx bit when the hardware trigger j is detected
 *                   - false: SUPERTMR clears the TRIGx bit when the hardware trigger j is detected
 */
static inline void SUPERTMR_DRV_SetHwTriggerSyncModeCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    supertmrBase->SYNCONF =
        (supertmrBase->SYNCONF & ~SUPERTMR_SYNCONF_HWTRIGMODE_MASK) | SUPERTMR_SYNCONF_HWTRIGMODE(enable);
}

/**
 * @brief Sets sync mode for SUPERTMR counter register when using a software trigger.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param update_mode State of SUPERTMR counter synchronization
 *                   - true : The software trigger activates SUPERTMR counter sync
 *                   - false: The software trigger does not activate SUPERTMR counter sync
 */
static inline void SUPERTMR_DRV_SetCounterSoftwareSyncModeCmd(supertmr_type_t *const supertmrBase,
                                                              supertmr_pwm_sync_mode_t update_mode)
{
    supertmrBase->SYNCONF =
        (supertmrBase->SYNCONF & ~SUPERTMR_SYNCONF_SWRSTCNT_MASK) | SUPERTMR_SYNCONF_SWRSTCNT(update_mode);
}

/**
 * @brief Sets the PWM synchronization mode to enhanced or legacy.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param mode State of synchronization mode
 *                   - true : Enhanced PWM synchronization is selected
 *                   - false: Legacy PWM synchronization is selected
 */
static inline void SUPERTMR_DRV_SetPwmSyncModeCmd(supertmr_type_t *const supertmrBase, bool mode)
{
    supertmrBase->SYNCONF = (supertmrBase->SYNCONF & ~SUPERTMR_SYNCONF_SYNCMODE_MASK) | SUPERTMR_SYNCONF_SYNCMODE(mode);
}

/**
 * @brief Sets the SWOCTRL register PWM synchronization mode.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param mode State of register synchronization
 *                   - true : SWOCTRL register is updated by PWM sync
 *                   - false: SWOCTRL register is updated at all rising edges of system clock
 */
static inline void SUPERTMR_DRV_SetSwoctrlPwmSyncModeCmd(supertmr_type_t *const supertmrBase,
                                                         supertmr_reg_update_t mode)
{
    supertmrBase->SYNCONF = (supertmrBase->SYNCONF & ~SUPERTMR_SYNCONF_SWOC_MASK) | SUPERTMR_SYNCONF_SWOC(mode);
}

/**
 * @brief Sets the INVCTRL register PWM synchronization mode.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param mode State of register synchronization
 *                   - true : INVCTRL register is updated by PWM sync
 *                   - false: INVCTRL register is updated at all rising edges of system clock
 */
static inline void SUPERTMR_DRV_SetInvctrlPwmSyncModeCmd(supertmr_type_t *const supertmrBase,
                                                         supertmr_reg_update_t mode)
{
    supertmrBase->SYNCONF = (supertmrBase->SYNCONF & ~SUPERTMR_SYNCONF_INVC_MASK) | SUPERTMR_SYNCONF_INVC(mode);
}

/**
 * @brief Sets the CNTIN register PWM synchronization mode.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param mode State of register synchronization
 *                   - true : CNTIN register is updated by PWM sync
 *                   - false: CNTIN register is updated at all rising edges of system clock
 */
static inline void SUPERTMR_DRV_SetCntinPwmSyncModeCmd(supertmr_type_t *const supertmrBase, supertmr_reg_update_t mode)
{
    supertmrBase->SYNCONF = (supertmrBase->SYNCONF & ~SUPERTMR_SYNCONF_CNTINC_MASK) | SUPERTMR_SYNCONF_CNTINC(mode);
}

/**
 * @brief Sets the modulo fractional value in the PWM dithering.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param value The value to be set to the fractional value for the modulo
 */
static inline void SUPERTMR_DRV_SetModFracVal(supertmr_type_t *const supertmrBase, uint8_t value)
{
    SUPERTMR_RMW_MOD_MIRROR(supertmrBase, SUPERTMR_MOD_MIRROR_FRACMOD_MASK, SUPERTMR_MOD_MIRROR_FRACMOD(value));
}

/**
 * @brief Sets the channel (n) match fractional value.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param channel The SUPERTMR peripheral channel (n)
 * @param value The value to be set to the channel (n) match fractional value
 */
static inline void SUPERTMR_DRV_SetChnMatchFracVal(supertmr_type_t *const supertmrBase, uint8_t channel, uint8_t value)
{
    SUPERTMR_RMW_CnV_MIRROR(supertmrBase, channel, SUPERTMR_CV_MIRROR_FRACVAL_MASK, SUPERTMR_CV_MIRROR_FRACVAL(value));
}

/**
 * @brief Sets the sync bit as a hardware trigger source for SUPERTMR instance.
 *
 * @param tcsrBase The SIM base address pointer.
 * @param instance The instance number of the SUPERTMR module.
 * @param enable Enable/Disable the sync bit.
 */
static inline void SUPERTMR_DRV_SyncBit(tcsr_type_t *const tcsrBase, uint32_t instance, bool enable)
{
    uint32_t instTemp = instance;

    if (instTemp > 3U) {
        instTemp = instance + 7U;
    }

    if (true == enable) {
        // TODO: zhoulei
        //       tcsrBase->SUPERTMROPT1 |= (1U << instTemp);
    } else {
        // TODO: zhoulei
        //        tcsrBase->SUPERTMROPT1 &= ~(1U << instTemp);
    }
}

/** DRV functionality */
/**
 * @brief Resets the SUPERTMR registers. All the register use in the driver should be
 * reset to default value of each register.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 */
void SUPERTMR_DRV_Reset(supertmr_type_t *const supertmrBase);

/**
 * @brief Initializes the SUPERTMR. This function will enable the superTimer module
 * and selects one pre-scale factor for the SUPERTMR clock source.
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param supertmrClockPrescaler The SUPERTMR peripheral clock pre-scale divider
 */
void SUPERTMR_DRV_InitModule(supertmr_type_t *const supertmrBase, supertmr_clock_ps_t supertmrClockPrescaler);

/**
 * @brief Enables or disables the generation of the SUPERTMR peripheral timer channel trigger when the
 * SUPERTMR counter is equal to its initial value
 *
 * @param supertmrBase The SUPERTMR base address pointer
 * @param channel The SUPERTMR peripheral channel number
 * @param enable Enables the generation of the channel trigger
 *                   - true : The generation of the channel trigger is enabled
 *                   - false: The generation of the channel trigger is disabled
 */
void SUPERTMR_DRV_SetChnTriggerCmd(supertmr_type_t *const supertmrBase, uint8_t channel, bool enable);

/**
 * @brief Sets the SUPERTMR peripheral timer channel input capture filter value.
 * @param supertmrBase The SUPERTMR base address pointer
 * @param channel The SUPERTMR peripheral channel number, only 0,1,2,3, channel 4, 5,6, 7 don't have
 * @param value Filter value to be set
 */
void SUPERTMR_DRV_SetChnInputCaptureFilter(supertmr_type_t *const supertmrBase, uint8_t channel, uint8_t value);

#if defined(__cplusplus)
}
#endif

#endif // __SUPERTMR_HW_ACCESS_H__
