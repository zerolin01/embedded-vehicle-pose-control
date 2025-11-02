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
 * @file pdu_hw_access.h
 * @brief PDU driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-12
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __PDU_HW_ACCESS_H__
#define __PDU_HW_ACCESS_H__
#include "pdu_driver.h"
#include "pdu_register.h"
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief  Resets the PDU registers to a known state.
 *
 * This function resets the PDU registers to a known state. This state is
 * defined in a reference manual and is power on reset value.
 *
 * @param  base  Register base address for the module.
 */
void PDU_Init(pdu_type_t *const base);

/**
 * @brief  Configure the PDU timer.
 *
 * This function configure the PDU's basic timer.
 *
 * @param  base  Register base address for the module.
 * @param  configPtr Pointer to configuration structure, see to "pdu_timer_config_t".
 */
void PDU_ConfigTimer(pdu_type_t *const base, const pdu_timer_config_t *const configPtr);

/**
 * @brief  Triggers the PDU by software if enabled.
 *
 * If enabled, this function triggers the PDU by using software.
 *
 * @param  base Register base address for the module.
 */
static inline void PDU_SetSoftTriggerCmd(pdu_type_t *const base)
{
    OS_ASSERT(base != NULL);
    OS_IoSetBit32(&(base->SC), PDU_SC_SWTRIG_MASK);
}

/**
 * @brief  Switches on to enable the PDU module.
 *
 * This function switches on to enable the PDU module.
 *
 * @param  base Register base address for the module.
 */
static inline void PDU_Enable(pdu_type_t *const base)
{
    OS_ASSERT(base != NULL);
    OS_IoSetBit32(&(base->SC), PDU_SC_PDUEN_MASK);
}

/**
 * @brief Switches to disable the PDU module.
 *
 * @param  base Register base address for the module.
 */
static inline void PDU_Disable(pdu_type_t *const base)
{
    OS_ASSERT(base != NULL);
    OS_IoClearBit32(&(base->SC), PDU_SC_PDUEN_MASK);
}

/**
 * @brief   Gets the PDU delay interrupt flag.
 *
 * This function gets the PDU delay interrupt flag.
 *
 * @param  base Register base address for the module.
 * @return true  if flag is set
 * @return false  if flag not set
 */
static inline bool PDU_GetTimerIntFlag(pdu_type_t const *const base)
{
    OS_ASSERT(base != NULL);

    return 1U == ((base->SC & PDU_SC_PDUIF_MASK) >> PDU_SC_PDUIF_SHIFT);
}

/**
 * @brief Clears the PDU delay interrupt flag.
 *
 * This function clears PDU delay interrupt flag.
 * @param  base Register base address for the module.
 */
static inline void PDU_ClearTimerIntFlag(pdu_type_t *const base)
{
    OS_ASSERT(base != NULL);
    volatile uint32_t dummy_read;
    OS_IoClearBit32(&(base->SC), PDU_SC_PDUIF_MASK);

    /* This read-after-write guarantees that the write to clear operation is completed,
     * for the case when memory write buffering is enabled. */
    dummy_read = OS_IoRead32(&(base->SC));
    (void)dummy_read;
}

/**
 * @brief   Loads the delay registers value for the PDU module.
 *
 * This function sets the LDOK bit and loads the delay registers value.
 * Writing one  to this bit updates the internal registers MOD, IDLY, CHnDLYm and
 * POyDLY with the values written to their buffers. The MOD, IDLY,
 * CHnDLYm and POyDLY take effect according to the load mode settings.
 *
 * After one is written to the LDOK bit, the values in the buffers of above mentioned registers
 * are not effective and cannot be written until the values in the
 * buffers are loaded into their internal registers.
 * The LDOK can be written only when the the PDU is enabled or as alone with it. It is
 * automatically cleared either when the values in the buffers are loaded into the
 * internal registers or when the PDU is disabled.
 *
 * @param  base Register base address for the module.
 */
static inline void PDU_SetLoadValuesCmd(pdu_type_t *const base)
{
    OS_ASSERT(base != NULL);
    OS_IoSetBit32(&(base->SC), PDU_SC_LDOK_MASK);
}

/**
 * @brief Sets the modulus value for the PDU module.
 *
 * This function sets the modulus value for the PDU module.
 * When the counter reaches the setting value, it is automatically reset to zero.
 * When in continuous mode, the counter begins to increase
 * again.
 *
 * @param  base   Register base address for the module.
 * @param  value  The setting value of upper limit for PDU counter.
 */
static inline void PDU_SetTimerModulusValue(pdu_type_t *const base, uint16_t value)
{
    OS_ASSERT(base != NULL);
    OS_IoRMW32(&(base->MOD), PDU_MOD_MOD_MASK, PDU_MOD_MOD(value));
}

/**
 * @brief  Gets the PDU counter value of PDU timer.
 *
 * This function gets the PDU counter value of PDU timer.
 *
 * @param  base  Register base address for the module.
 * @return uint32_t The current counter value
 */
static inline uint32_t PDU_GetTimerValue(pdu_type_t const *const base)
{
    OS_ASSERT(base != NULL);

    return ((base->CNT & PDU_CNT_CNT_MASK) >> PDU_CNT_CNT_SHIFT);
}

/**
 * @brief  Sets the interrupt delay milestone of the PDU counter.
 *
 * This function sets the interrupt delay milestone of the PDU counter.
 * If enabled, a PDU interrupt is generated when the counter is equal to the
 * setting value.
 *
 * @param  base   Register base address for the module.
 * @param  value  The setting value for interrupt delay milestone of PDU counter.
 */
static inline void PDU_SetValueForTimerInterrupt(pdu_type_t *const base, uint16_t value)
{
    OS_ASSERT(base != NULL);
    OS_IoRMW32(&(base->IDLY), PDU_IDLY_IDLY_MASK, PDU_IDLY_IDLY(value));
}

/**
 * @brief Switches to enable the pre-trigger back-to-back mode.
 *
 * This function switches to enable the pre-trigger back-to-back mode.
 *
 * @param  base   Register base address for the module.
 * @param  chn    PDU instance index for trigger.
 * @param  preChnMask ADC channel group index mask for trigger.
 * @param  enable   Switcher to assert the feature.
 */
void PDU_SetAdcPreTriggerBackToBackEnable(pdu_type_t *const base, uint32_t chn, uint32_t preChnMask, bool enable);

/**
 * @brief  Switches to enable the pre-trigger output.
 *
 * This function switches to enable pre-trigger output.
 *
 * @param  base Register base address for the module.
 * @param  chn  PDU instance index for trigger.
 * @param  preChnMask ADC channel group index mask for trigger.
 * @param  enable Switcher to assert the feature.
 */
void PDU_SetAdcPreTriggerOutputEnable(pdu_type_t *const base, uint32_t chn, uint32_t preChnMask, bool enable);

/**
 * @brief  Switches to enable the pre-trigger.
 *
 * This function switches to enable the pre-trigger.
 *
 * @param  base Register base address for the module.
 * @param  chn  PDU instance index for trigger.
 * @param  preChnMask ADC channel group index mask for trigger.
 * @param  enable  Switcher to assert the feature.
 */
void PDU_SetAdcPreTriggerEnable(pdu_type_t *const base, uint32_t chn, uint32_t preChnMask, bool enable);

/**
 * @brief  Gets the flag which indicates whether the PDU counter has reached the pre-trigger delay value.
 *
 * This function gets the flag which indicates the PDU counter has reached the
 * pre-trigger delay value.
 *
 * @param  base   Register base address for the module.
 * @param  chn    PDU instance index for trigger.
 * @param  preChnMask  ADC channel group index mask for trigger.
 * @return uint32_t Flag mask. Indicated bit would be 1 if the event is asserted.
 */
static inline uint32_t PDU_GetAdcPreTriggerFlags(pdu_type_t const *const base, uint32_t chn, uint32_t preChnMask)
{
    OS_ASSERT(base != NULL);
    OS_ASSERT(chn < PDU_CH_COUNT);

    return preChnMask & ((base->CH[chn].S & PDU_S_CF_MASK) >> PDU_S_CF_SHIFT);
}

/**
 * @brief  Clears the flag which indicates that the PDU counter has reached the pre-trigger delay value.
 *
 * This function clears the flag which indicates that the PDU counter has reached  the
 * pre-trigger delay value.
 *
 * @param  base Register base address for the module.
 * @param  chn  PDU instance index for trigger.
 * @param  preChnMask ADC channel group index mask for trigger.
 */
void PDU_ClearAdcPreTriggerFlags(pdu_type_t *const base, uint32_t chn, uint32_t preChnMask);

/**
 * @brief  Gets the flag which indicates whether a sequence error is detected.
 *
 * This function gets the flag which indicates whether a sequence error is detected.
 *
 * @param  base   Register base address for the module.
 * @param  chn    PDU instance index for trigger.
 * @param  preChnMask  ADC channel group index mask for trigger.
 * @return uint32_t
 *          Flag mask. Indicated bit would be 1 if the event is asserted.
 */
static inline uint32_t PDU_GetAdcPreTriggerSeqErrFlags(pdu_type_t const *const base, uint32_t chn, uint32_t preChnMask)
{
    OS_ASSERT(base != NULL);
    OS_ASSERT(chn < PDU_CH_COUNT);

    return (preChnMask & ((base->CH[chn].S & PDU_S_ERR_MASK) >> PDU_S_ERR_SHIFT));
}

/**
 * @brief   Clears the flag which indicates that a sequence error has been detected.
 *
 * This function clears the flag which indicates that the sequence error has been detected.
 *
 * @param  base Register base address fot the module.
 * @param  chn  PDU instance index for trigger
 * @param  preChnMask  ADC channel group index mask trigger.
 */
void PDU_ClearAdcPreTriggerSeqErrFlags(pdu_type_t *const base, uint32_t chn, uint32_t preChnMask);

/**
 * @brief  Sets the pre-trigger delay value.
 *
 * This function sets the pre-trigger delay value.
 *
 * @param  base    Register base address for the module.
 * @param  chn     PDU instance index for trigger.
 * @param  preChn  ADC channel group index for trigger.
 * @param  value   Setting value for pre-trigger's delay value.
 */
static inline void PDU_SetAdcPreTriggerDelayValue(pdu_type_t *const base, uint32_t chn, uint32_t preChn, uint32_t value)
{
    OS_ASSERT(base != NULL);
    OS_ASSERT(chn < PDU_CH_COUNT);
    OS_ASSERT(preChn < PDU_DLY_COUNT);

    base->CH[chn].DLY[preChn] = PDU_DLY_DLY(value);
}

/**
 * @brief  Switches to enable the pulse-out trigger
 *
 * this function switches to enable the pulse-out trigger
 *
 * @param  base Register base address for the module
 * @param  pulseChnMask Pulse-out channel index mask fot trigger
 * @param  enable switcher to assert the feature
 */
void PDU_SetCmpPulseOutEnable(pdu_type_t *const base, uint32_t pulseChnMask, bool enable);

/**
 * @brief  Sets the counter delay value for the pulse-out include high and low.
 *
 * @param  base        Register base address for the module
 * @param  pulseChn    pulse-out channel index for trigger
 * @param  HighValue   Setting High value for PDU delay
 * @param  LowValue    Setting Low value for PDU delay
 */
static inline void PDU_SetCmpPulseOutDelay(pdu_type_t *const base, uint32_t pulseChn, uint16_t HighValue,
                                           uint16_t LowValue)
{
    OS_ASSERT(base != NULL);
    OS_ASSERT(pulseChn < PDU_POnDLY_COUNT);
    uint32_t Value = 0;

    Value = HighValue;
    Value = Value << 16;
    Value |= LowValue;

    base->POnDLY[pulseChn].PODLY = Value;
}

/**
 * @brief Enable PDU instance back to back mode between PDU0 CH0 and PDU1 CH0 pre-triggers
 *
 * This function enables PDU instance back to back mode between PDU0 CH0 and PDU1 CH0 pre-triggers
 *
 */
static inline void PDU_TCSR_EnableInstanceBackToBack(void)
{
    /* instance back to back cannot be enabled simultaneously with inter-channel back to back on any PDU instance */
    OS_ASSERT((TCSR->ADCCONFIG & (TCSR_ADCCONFIG_PDUBBSEL_1_MASK | TCSR_ADCCONFIG_PDUBBSEL_2_MASK)) == 0U);

    TCSR->ADCCONFIG |= TCSR_ADCCONFIG_PDUBBSEL_MASK;
}

/**
 * @brief Disable PDU instance back to back mode between PDU0 CH0 and PDU1 CH0 pre-triggers
 *
 * This function disables PDU instance back to back mode between PDU0 CH0 and PDU1 CH0 pre-triggers
 *
 */
static inline void PDU_TCSR_DisableInstanceBackToBack(void)
{
    TCSR->ADCCONFIG &= ~TCSR_ADCCONFIG_PDUBBSEL_MASK;
}

/**
 * @brief Enable PDU inter-channel back to back mode between PDUx CH0 and PDUx CH1 pre-triggers
 *
 * @param  instance   PDU instance number
 */
static inline void PDU_TCSR_EnableInterchannelBackToBack(const uint32_t instance)
{
    /* instance back to back cannot be enabled simultaneously with inter-channel back to back */
    OS_ASSERT((TCSR->ADCCONFIG & TCSR_ADCCONFIG_PDUBBSEL_MASK) == 0U);

    switch (instance) {
    case 0U:
        TCSR->ADCCONFIG |= TCSR_ADCCONFIG_PDUBBSEL_1_MASK;
        break;
    case 1U:
        TCSR->ADCCONFIG |= TCSR_ADCCONFIG_PDUBBSEL_2_MASK;
        break;
    default:
        /* do nothing */
        break;
    }
}

/**
 * @brief  Disable PDU inter-channel back to back mode between PDUx CH0 and PDUx CH1 pre-triggers
 *
 * This function disables inter-channel back to back mode between PDUx CH0 and PDUx CH1 pre-triggers
 *
 * @param  instance PDU instance number
 */
static inline void PDU_TCSR_DisableInterchannelBackToBack(const uint32_t instance)
{
    switch (instance) {
    case 0U:
        TCSR->ADCCONFIG &= ~TCSR_ADCCONFIG_PDUBBSEL_1_MASK;
        break;
    case 1U:
        TCSR->ADCCONFIG &= ~TCSR_ADCCONFIG_PDUBBSEL_2_MASK;
        break;
    default:
        /* do nothing */
        break;
    }
}

#if defined(__cplusplus)
}
#endif

#endif
