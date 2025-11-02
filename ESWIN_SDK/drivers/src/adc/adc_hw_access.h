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
 * @file adc_hw_access.h
 * @brief ADC driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __ADC_HW_ACCESS_H__
#define __ADC_HW_ACCESS_H__

#include <stdbool.h>
#include <stdint.h>

#include "adc_driver.h"
#include "adc_register.h"
#include "clock_driver.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Sets the ADC Enable Flag state
 *
 * This function configures the ADC Module Enable Flag.
 *
 *
 * @param baseAddr adc base pointer
 * @param state the new ADC Module Enable Flag state
 */
static inline void ADC_SetModuleEnableFlag(adc_type_t *const baseAddr, const bool state)
{
    uint32_t tmp = (uint32_t)baseAddr->SC2;
    tmp &= (uint32_t)(~(ADC_SC2_ADEN_MASK));
    tmp |= ADC_SC2_ADEN(state ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->SC2 = (uint32_t)tmp;
}

/**
 * @brief ADC Soft Rest
 *
 * This function configures the ADC Soft reset.
 * The ADC has a power-on sequence, and the software must meet the power-on
 * reset requirements of the ADC module.
 *
 *
 * @param baseAddr adc base pointer
 * @param state the new ADC Module Soft Reset state
 */
static inline void ADC_SetModuleSoftReset(adc_type_t *const baseAddr, const bool state)
{
    uint32_t tmp = (uint32_t)baseAddr->SC3;
    tmp &= (uint32_t)(~(ADC_SC3_ASR_MASK));
    tmp |= ADC_SC3_ASR(state ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->SC3 = (uint32_t)tmp;
}

/**
 * @brief Sets the ADC Operation Enable Flag state
 *
 * This function configures the ADC Operation Enable Flag.
 *
 *
 * @param baseAddr adc base pointer
 * @param state the new ADC Operation Enable Flag state
 */
static inline void ADC_SetOperationEnableFlag(adc_type_t *const baseAddr, const bool state)
{
    uint32_t tmp = (uint32_t)baseAddr->CR;
    tmp &= (uint32_t)(~(ADC_CR_OPEN_MASK));
    tmp |= ADC_CR_OPEN(state ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->CR = (uint32_t)tmp;
}

/**
 * @brief Sets the ADC Gain error calibration value
 *
 * This function configures the ADC Gain error calibration value.
 *
 *
 * @param baseAddr adc base pointer
 * @param value the new ADC Gain error calibration value
 */
static inline void ADC_SetGainCalibration(adc_type_t *const baseAddr, const uint32_t value)
{
    uint32_t tmp = (uint32_t)baseAddr->COR;
    tmp &= (uint32_t)(~(ADC_COR_GAIN_MASK));
    tmp |= ADC_COR_GAIN(value);
    baseAddr->COR = (uint32_t)tmp;
}

/**
 * @brief Gets the Conversion Active Flag
 *
 * This function checks whether a conversion is currently
 * taking place on the ADC module.
 *
 *
 * @param baseAddr adc base pointer
 * @return Conversion Active Flag state
 */
static inline bool ADC_GetConvActiveFlag(const adc_type_t *const baseAddr)
{
    uint32_t tmp = (uint32_t)baseAddr->SC2;
    tmp          = (tmp & ADC_SC2_ACT_MASK) >> ADC_SC2_ACT_SHIFT;

    return (tmp != 0U) ? true : false;
}

/**
 * @brief Gets the Resolution Mode configuration
 *
 * This function returns the configured resolution mode for
 * the ADC.
 *
 * @param baseAddr adc base pointer
 * @return the ADC resolution mode. Possible values:
 *        - ADC_RESOLUTION_12BIT : 12-bit resolution mode.
 */
static inline adc_resolution_t ADC_GetResolution(const adc_type_t *const baseAddr)
{
    (void)baseAddr;
    return ADC_RESOLUTION_12BIT;
}

/**
 * @brief Sets the Difference state
 *
 * This function configures the Difference Mode.
 *
 * @param baseAddr adc base pointer
 * @param state the new Difference Mode
 */
static inline void ADC_SetDifferenceMode(adc_type_t *const baseAddr, const bool state)
{
    uint32_t tmp = baseAddr->SC3;
    tmp &= ~(ADC_SC3_DIFF_MASK);
    tmp |= ADC_SC3_DIFF(state ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->SC3 = tmp;
}

/**
 * @brief Gets the ADC ADMA output data format
 *
 * This function returns the ADC ADMA output data format.
 *
 *
 * @param baseAddr adc base pointer
 * @param dataFmt the new data format
 */
static inline adc_adma_datafmt_t ADC_GetAdmaDataFormatMode(const adc_type_t *const baseAddr)
{
    uint32_t tmp = baseAddr->SC3;
    tmp          = (tmp & ADC_SC3_DMADF_MASK) >> ADC_SC3_DMADF_SHIFT;

    /* Enum defines all possible values, so casting is safe */
    return (adc_adma_datafmt_t)(tmp);
}

/**
 * @brief Gets the ADC interrupt status
 *
 * This function returns the channel interrupt status the ADC.
 *
 * @param baseAddr adc base pointer
 * @return the interrupt status
 */
static inline uint32_t ADC_GetIntStatus(const adc_type_t *const baseAddr)
{
    uint32_t tmp = baseAddr->INTS;
    tmp          = (tmp & ADC_INTS_INTS_MASK) >> ADC_INTS_INTS_SHIFT;

    /* Enum defines all possible values, so casting is safe */
    return tmp;
}


/**
 * @brief Sets the ADC ADMA output data format
 *
 * This function configures the ADC ADMA output data format.
 *
 *
 * @param baseAddr adc base pointer
 * @param dataFmt the new data format
 */
static inline void ADC_SetAdmaDataFormatMode(adc_type_t *const baseAddr, adc_adma_datafmt_t dataFmt)
{
    uint32_t tmp = baseAddr->SC3;
    /* Clear the affected bitfield */
    tmp &= ~(ADC_SC3_DMADF_MASK);
    tmp |= ADC_SC3_DMADF(dataFmt);
    baseAddr->SC3 = tmp;
}

/**
 * @brief Gets the ADC Trigger Mode
 *
 * This function returns the configured triggering mode
 * for the ADC. In Software Triggering Mode, the user can
 * start conversions by setting an input channel in the
 * ADC measurement channel A (index 0). When in Hardware
 * trigger mode, a conversion is started by another peripheral (
 * like PDU or TRGMUX).
 *
 * @param baseAddr adc base pointer
 * @return the current trigger mode. Possible values:
 *        - ADC_TRIGGER_SOFTWARE : Software triggering.
 *        - ADC_TRIGGER_HARDWARE : Hardware triggering.
 */
static inline adc_trigger_t ADC_GetTriggerMode(const adc_type_t *const baseAddr)
{
    uint32_t tmp = baseAddr->SC2;
    tmp          = (tmp & ADC_SC2_TRGS_MASK) >> ADC_SC2_TRGS_SHIFT;

    /* Enum defines all possible values, so casting is safe */
    return (adc_trigger_t)(tmp);
}

/**
 * @brief Sets the ADC Trigger Mode
 *
 * This function configures the ADC triggering mode. In
 * Software Triggering Mode, the user can start conversions
 * by setting an input channel in the ADC measurement channel
 * A (index 0). When in Hardware trigger mode, a conversion
 * is started by another peripheral (like PDU or TRGMUX).
 *
 * @param baseAddr adc base pointer
 * @param trigger the desired trigger mode
 *        - ADC_TRIGGER_SOFTWARE : Software triggering.
 *        - ADC_TRIGGER_HARDWARE : Hardware triggering.
 */
static inline void ADC_SetTriggerMode(adc_type_t *const baseAddr, const adc_trigger_t trigger)
{
    uint32_t tmp = baseAddr->SC2;
    tmp &= ~(ADC_SC2_TRGS_MASK);
    tmp |= ADC_SC2_TRGS(trigger);
    baseAddr->SC2 = tmp;
}

/**
 * @brief Gets the pretrigger source configured for an ADC instance
 *
 * This function gets the pretrigger source selected from
 * ADC Trigger Latching and Arbitration Unit,
 * affecting control channels 0-3.
 *
 * @param instance  the ADC instance
 * @return  pretrigger source selected. Possible values:
 *           - ADC_PRETRIGGER_SEL_PDU     - PDU pretrigger
 *           - ADC_PRETRIGGER_SEL_TRGMUX  - TRGMUX pretrigger
 *           - ADC_PRETRIGGER_SEL_SW      - Software pretrigger
 */
static inline adc_pretrigger_sel_t ADC_GetPretriggerSelect(const uint32_t instance)
{
    const tcsr_type_t *const tcsrBase  = TCSR;
    uint32_t currentVal                = 0U;
    adc_pretrigger_sel_t returnVal     = ADC_PRETRIGGER_SEL_PDU;
    uint32_t mask[ADC_INSTANCE_COUNT]  = {0U};
    uint32_t shift[ADC_INSTANCE_COUNT] = {0U};
#if (ADC_INSTANCE_COUNT == 1U)
    mask[0]  = TCSR_ADCOPT_ADC0PRETRGSEL_MASK;
    shift[0] = TCSR_ADCOPT_ADC0PRETRGSEL_SHIFT;
#elif (ADC_INSTANCE_COUNT == 2U)
    mask[0]      = TCSR_ADCOPT_ADC0PRETRGSEL_MASK;
    mask[1]      = TCSR_ADCOPT_ADC1PRETRGSEL_MASK;
    shift[0]     = TCSR_ADCOPT_ADC0PRETRGSEL_SHIFT;
    shift[1]     = TCSR_ADCOPT_ADC1PRETRGSEL_SHIFT;
#else
#error "Maximum supported value for ADC_INSTANCE_COUNT is 2."
#endif
    currentVal = (tcsrBase->ADCCONFIG & mask[instance]) >> shift[instance];
    switch (currentVal) {
    case ADC_PRETRIGGER_SEL_PDU:
        returnVal = ADC_PRETRIGGER_SEL_PDU;
        break;
    case ADC_PRETRIGGER_SEL_TRGMUX:
        returnVal = ADC_PRETRIGGER_SEL_TRGMUX;
        break;
    case ADC_PRETRIGGER_SEL_SW:
        returnVal = ADC_PRETRIGGER_SEL_SW;
        break;
    default:
        OS_ASSERT(false);
        break;
    }

    return returnVal;
}

/**
 * @brief Sets the pretrigger select for an ADC instance
 *
 * This function sets the pretrigger source selected from
 * ADC Trigger Latching and Arbitration Unit,
 * affecting control channels 0-3.
 *
 * @param instance  the ADC instance
 * @param pretriggerSel  the pretrigger source to be selected
 *           - ADC_PRETRIGGER_SEL_PDU     - PDU pretrigger
 *           - ADC_PRETRIGGER_SEL_TRGMUX  - TRGMUX pretrigger
 *           - ADC_PRETRIGGER_SEL_SW      - Software pretrigger
 */
static inline void ADC_SetPretriggerSelect(const uint32_t instance, const adc_pretrigger_sel_t pretriggerSel)
{
    tcsr_type_t *const tcsrBase       = TCSR;
    uint32_t mask[ADC_INSTANCE_COUNT] = {0U};
#if (ADC_INSTANCE_COUNT == 1U)
    mask[0] = TCSR_ADCOPT_ADC0PRETRGSEL_MASK;
#elif (ADC_INSTANCE_COUNT == 2U)
    mask[0]      = TCSR_ADCOPT_ADC0PRETRGSEL_MASK;
    mask[1]      = TCSR_ADCOPT_ADC1PRETRGSEL_MASK;
#else
#error "Maximum supported value for ADC_INSTANCE_COUNT is 2."
#endif
    uint32_t intermVal = 0U;

    intermVal = tcsrBase->ADCCONFIG & (~mask[instance]);

    switch (instance) {
    case 0:
        intermVal |= TCSR_ADCOPT_ADC0PRETRGSEL(pretriggerSel);
        break;
    case 1:
        intermVal |= TCSR_ADCOPT_ADC1PRETRGSEL(pretriggerSel);
        break;
    default:
        OS_ASSERT(false);
        break;
    }

    tcsrBase->ADCCONFIG = intermVal;
}

/**
 * @brief Gets the trigger source configured for an ADC instance
 *
 * This function gets the trigger source selected from
 * ADC Trigger Latching and Arbitration Unit.
 *
 * @param instance  the ADC instance
 * @param triggerSel  the trigger source to be selected
 *         - ADC_TRIGGER_SEL_PDU     - PDU trigger selected
 *         - ADC_TRIGGER_SEL_TRGMUX  - TRGMUX trigger
 */
static inline adc_trigger_sel_t ADC_GetTriggerSelect(const uint32_t instance)
{
    const tcsr_type_t *const tcsrBase  = TCSR;
    uint32_t currentVal                = 0U;
    adc_trigger_sel_t returnVal        = ADC_TRIGGER_SEL_PDU;
    uint32_t mask[ADC_INSTANCE_COUNT]  = {0U};
    uint32_t shift[ADC_INSTANCE_COUNT] = {0U};
#if (ADC_INSTANCE_COUNT == 1U)
    mask[0]  = TCSR_ADCOPT_ADC0TRGSEL_MASK;
    shift[0] = TCSR_ADCOPT_ADC0TRGSEL_SHIFT;
#elif (ADC_INSTANCE_COUNT == 2U)
    mask[0]      = TCSR_ADCOPT_ADC0TRGSEL_MASK;
    mask[1]      = TCSR_ADCOPT_ADC1TRGSEL_MASK;
    shift[0]     = TCSR_ADCOPT_ADC0TRGSEL_SHIFT;
    shift[1]     = TCSR_ADCOPT_ADC1TRGSEL_SHIFT;
#else
#error "Maximum supported value for ADC_INSTANCE_COUNT is 2."
#endif
    currentVal = (tcsrBase->ADCCONFIG & mask[instance]) >> shift[instance];
    switch (currentVal) {
    case ADC_TRIGGER_SEL_PDU:
        returnVal = ADC_TRIGGER_SEL_PDU;
        break;
    case ADC_TRIGGER_SEL_TRGMUX:
        returnVal = ADC_TRIGGER_SEL_TRGMUX;
        break;
    default:
        OS_ASSERT(false);
        break;
    }

    return returnVal;
}

/**
 * @brief Sets the trigger select for an ADC instance
 *
 * This function sets the trigger source selected from
 * ADC Trigger Latching and Arbitration Unit.
 *
 * @param instance  the ADC instance
 * @param triggerSel  the trigger source to be selected
 *         - ADC_TRIGGER_SEL_PDU     - PDU trigger selected
 *         - ADC_TRIGGER_SEL_TRGMUX  - TRGMUX trigger
 */
static inline void ADC_SetTriggerSelect(const uint32_t instance, const adc_trigger_sel_t triggerSel)
{
    tcsr_type_t *const tcsrBase       = TCSR;
    uint32_t mask[ADC_INSTANCE_COUNT] = {0U};
#if (ADC_INSTANCE_COUNT == 1U)
    mask[0] = TCSR_ADCOPT_ADC0TRGSEL_MASK;
#elif (ADC_INSTANCE_COUNT == 2U)
    mask[0]      = TCSR_ADCOPT_ADC0TRGSEL_MASK;
    mask[1]      = TCSR_ADCOPT_ADC1TRGSEL_MASK;
#else
#error "Maximum supported value for ADC_INSTANCE_COUNT is 2."
#endif
    uint32_t intermVal = 0U;

    intermVal = tcsrBase->ADCCONFIG & (~mask[instance]);

    switch (instance) {
    case 0:
        intermVal |= TCSR_ADCOPT_ADC0TRGSEL(triggerSel);
        break;
    case 1:
        intermVal |= TCSR_ADCOPT_ADC1TRGSEL(triggerSel);
        break;
    default:
        OS_ASSERT(false);
        break;
    }

    tcsrBase->ADCCONFIG = intermVal;
}

/**
 * @brief Gets the DMA Enable Flag state
 *
 * This function returns the state of the DMA Enable flag.
 * DMA can be used to transfer completed conversion values
 * from the result registers to RAM without CPU intervention.
 *
 * @param baseAddr adc base pointer
 * @return the DMA Enable Flag state
 */
static inline bool ADC_GetDMAEnableFlag(const adc_type_t *const baseAddr)
{
#if FEATURE_ADC_DMA_ADMA
    uint32_t tmp = (uint32_t)baseAddr->SC3;
    tmp          = (tmp & ADC_SC3_SDMAEN_MASK) >> ADC_SC3_SDMAEN_SHIFT;

    return (tmp != 0U) ? true : false;
#else
    uint32_t tmp = (uint32_t)baseAddr->SC2;
    tmp          = (tmp & ADC_SC2_DMAEN_MASK) >> ADC_SC2_DMAEN_SHIFT;

    return (tmp != 0U) ? true : false;
#endif
}

/**
 * @brief Sets the DMA Enable Flag state
 *
 * This function configures the DMA Enable Flag. DMA can be
 * used to transfer completed conversion values from the
 * result registers to RAM without CPU intervention.
 *
 * @param baseAddr adc base pointer
 * @param state the new DMA Enable Flag state
 */
static inline void ADC_SetDMAEnableFlag(adc_type_t *const baseAddr, const bool state)
{
#if FEATURE_ADC_DMA_ADMA
    uint32_t tmp = (uint32_t)baseAddr->SC3;
    tmp &= (uint32_t)(~(ADC_SC3_SDMAEN_MASK));
    tmp |= ADC_SC3_SDMAEN(state ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->SC3 = (uint32_t)tmp;
#else
    uint32_t tmp = (uint32_t)baseAddr->SC2;
    tmp &= (uint32_t)(~(ADC_SC2_DMAEN_MASK));
    tmp |= ADC_SC2_DMAEN(state ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->SC2 = (uint32_t)tmp;
#endif
}

/**
 * @brief Gets the Continuous Conversion Flag state
 *
 * This functions returns the state of the Continuous Conversion
 * Flag. This feature can be used to continuously sample a
 * single channel. When this is active, the channel cannot be
 * changed (by software or hardware trigger) until this feature
 * is turned off.
 *
 * @param baseAddr adc base pointer
 * @return the Continuous Conversion Flag state
 */
static inline bool ADC_GetContinuousConvFlag(const adc_type_t *const baseAddr)
{
    uint32_t tmp = (uint32_t)baseAddr->SC3;
    tmp          = (tmp & ADC_SC3_CCE_MASK) >> ADC_SC3_CCE_SHIFT;

    return (tmp != 0U) ? true : false;
}

/**
 * @brief Sets the Continuous Conversion Flag state
 *
 * This function configures the Continuous Conversion. This
 * feature can be used to continuously sample a single channel.
 * When this is active, the channel cannot be changed (by
 * software or hardware trigger) until this feature is turned
 * off.
 *
 * @param baseAddr adc base pointer
 * @param state the new Continuous Conversion Flag state
 */
static inline void ADC_SetContinuousConvFlag(adc_type_t *const baseAddr, const bool state)
{
    uint32_t tmp = (uint32_t)baseAddr->SC3;
    /* Clear the affected bitfield and write '0' to the w1c bits to avoid side-effects */
    tmp &= ~(ADC_SC3_CCE_MASK);
    tmp |= ADC_SC3_CCE(state ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->SC3 = (uint32_t)tmp;
}

/**
 * @brief Gets the Hardware Compare Enable Flag state
 *
 * This function returns the state of the Hardware Compare
 * Enable Flag. Hardware Compare can be used to check if the
 * ADC result is within or outside of a predefined range.
 *
 * @param baseAddr adc base pointer
 * @return the Hardware Compare Enable Flag state
 */
static inline bool ADC_GetHwCompareEnableFlag(const adc_type_t *const baseAddr)
{
    uint32_t tmp = (uint32_t)baseAddr->SC2;
    tmp          = (tmp & ADC_SC2_CFE_MASK) >> ADC_SC2_CFE_SHIFT;

    return (tmp != 0U) ? true : false;
}

/**
 * @brief Sets the Hardware Compare Enable Flag state
 *
 * This functions configures the Hardware Compare Enable Flag.
 * Hardware Compare can be used to check if the ADC result
 * is within or outside of a predefined range.
 *
 * @param baseAddr adc base pointer
 * @param state the new Hardware Compare Enable Flag state
 */
static inline void ADC_SetHwCompareEnableFlag(adc_type_t *const baseAddr, const bool state)
{
    uint32_t tmp = (uint32_t)baseAddr->SC2;
    tmp &= (uint32_t)(~(ADC_SC2_CFE_MASK));
    tmp |= ADC_SC2_CFE(state ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->SC2 = (uint32_t)tmp;
}

/**
 * @brief Gets the Hardware Compare Greater Than Enable Flag state
 *
 * This function returns the Hardware Compare Greater Than
 * Enable Flag. Using this feature, the ADC can be configured
 * to check if the measured value is within or outside of a
 * predefined range.
 *
 * @param baseAddr adc base pointer
 * @return the Hardware Compare Greater Than Enable Flag state
 */
static inline bool ADC_GetHwCompareGtEnableFlag(const adc_type_t *const baseAddr)
{
    uint32_t tmp = (uint32_t)baseAddr->SC2;
    tmp          = (tmp & ADC_SC2_CFGT_MASK) >> ADC_SC2_CFGT_SHIFT;

    return (tmp != 0U) ? true : false;
}

/**
 * @brief Sets the Hardware Compare Greater Than Enable Flag state
 *
 * This function configures the Hardware Compare Greater Than
 * Enable Flag. Using this feature, the ADC can be configured
 * to check if the measured value is within or outside of a
 * predefined range.
 *
 * @param baseAddr adc base pointer
 * @param state the new Hardware Compare Greater Than Enable Flag state
 */
static inline void ADC_SetHwCompareGtEnableFlag(adc_type_t *const baseAddr, const bool state)
{
    uint32_t tmp = (uint32_t)baseAddr->SC2;
    tmp &= (uint32_t)(~(ADC_SC2_CFGT_MASK));
    tmp |= ADC_SC2_CFGT(state ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->SC2 = (uint32_t)tmp;
}

/**
 * @brief Gets the Hardware Compare Range Enable state
 *
 * This function returns the state of the Hardware Compare
 * Range Enable Flag. This feature allows configuration
 * of a range with two non-zero values or with a non-zero
 * and zero value.
 *
 * @param baseAddr adc base pointer
 * @return the Hardware Compare Range Enable Flag state
 */
static inline bool ADC_GetHwCompareRangeEnableFlag(const adc_type_t *const baseAddr)
{
    uint32_t tmp = (uint32_t)baseAddr->SC2;
    tmp          = (tmp & ADC_SC2_CREN_MASK) >> ADC_SC2_CREN_SHIFT;

    return (tmp != 0U) ? true : false;
}

/**
 * @brief Sets the Hardware Compare Range Enable state
 *
 * This function configures the Hardware Compare Range
 * Enable Flag. This feature allows configuration
 * of a range with two non-zero values or with a non-zero
 * and zero value.
 *
 * @param baseAddr adc base pointer
 * @param state the new Hardware Compare Range Enable Flag state
 */
static inline void ADC_SetHwCompareRangeEnableFlag(adc_type_t *const baseAddr, const bool state)
{
    uint32_t tmp = (uint32_t)baseAddr->SC2;
    tmp &= (uint32_t)(~(ADC_SC2_CREN_MASK));
    tmp |= ADC_SC2_CREN(state ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->SC2 = (uint32_t)tmp;
}

/**
 * @brief Gets the Compare Register 1 value
 *
 * This function returns the value written in the Hardware
 * Compare Register 1. This value defines the upper or lower
 * limit for the Hardware Compare Range. This value is always
 * 12-bit resolution value (for lower resolution modes, internal
 * bit shifting will take place).
 *
 * @param baseAddr adc base pointer
 * @return the Compare Register 1 value
 */
static inline uint16_t ADC_GetHwCompareComp1Value(const adc_type_t *const baseAddr)
{
    return (uint16_t)baseAddr->CV[0U];
}

/**
 * @brief Sets the Compare Register 1 value
 *
 * This function writes a 12-bit value in the Hardware
 * Compare Register 1. This value defines the upper or lower
 * limit for the Hardware Compare Range. This value is always
 * 12-bit resolution (for lower resolution modes, internal
 * bit shifting will take place).
 *
 * @param baseAddr adc base pointer
 * @param value the new Compare Register 1 value
 */
static inline void ADC_SetHwCompareComp1Value(adc_type_t *const baseAddr, const uint16_t value)
{
    baseAddr->CV[0U] = ADC_CV_CV(value);
}

/**
 * @brief Gets the Compare Register 2 value
 *
 * This function returns the value written in the Hardware
 * Compare Register 2. This value defines the upper or lower
 * limit for the Hardware Compare Range. This value is always
 * 12-bit resolution (for lower resolution modes, internal
 * bit shifting will take place).
 *
 * @param baseAddr adc base pointer
 * @return the Compare Register 2 value
 */
static inline uint16_t ADC_GetHwCompareComp2Value(const adc_type_t *const baseAddr)
{
    return (uint16_t)baseAddr->CV[1U];
}

/**
 * @brief Sets the Compare Register 2 value
 *
 * This function writes a 12-bit value in the Hardware
 * Compare Register 2. This value defines the upper or lower
 * limit for the Hardware Compare Range. This value is always
 * 12-bit resolution value (for lower resolution modes, internal
 * bit shifting will take place).
 *
 * @param baseAddr adc base pointer
 * @param value the new Compare Register 2 value
 */
static inline void ADC_SetHwCompareComp2Value(adc_type_t *const baseAddr, const uint16_t value)
{
    baseAddr->CV[1U] = ADC_CV_CV(value);
}

/**
 * @brief Gets the Hardware Average Enable Flag state
 *
 * This function returns the state of the Hardware Average
 * Enable Flag. Hardware averaging can be used to obtain an
 * average value over multiple consecutive conversions on
 * the same channel.
 *
 * @param baseAddr adc base pointer
 * @return the Hardware Average Enable Flag state
 */
static inline bool ADC_GetHwAverageEnableFlag(const adc_type_t *const baseAddr)
{
    uint32_t tmp = (uint32_t)baseAddr->SC3;
    tmp          = (tmp & ADC_SC3_AVGE_MASK) >> ADC_SC3_AVGE_SHIFT;

    return (tmp != 0U) ? true : false;
}

/**
 * @brief Sets the Hardware Average Enable Flag state
 *
 * This function configures the Hardware Average Enable Flag.
 * Hardware averaging can be used to obtain an average value
 * over multiple consecutive conversions on the same channel.
 *
 * @param baseAddr adc base pointer
 * @param state the new Hardware Average Enable Flag state
 */
static inline void ADC_SetHwAverageEnableFlag(adc_type_t *const baseAddr, const bool state)
{
    uint32_t tmp = baseAddr->SC3;
    /* Clear the affected bitfield */
    tmp &= ~(ADC_SC3_AVGE_MASK);
    tmp |= ADC_SC3_AVGE(state ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->SC3 = tmp;
}

/**
 * @brief Gets the Hardware Average Mode
 *
 * This function returns the configured Hardware Average Mode.
 * The mode selects the number of samples to average: 4, 8, 16
 * or 32.
 *
 * @param baseAddr adc base pointer
 * @return the Hardware Average Mode selection. Possible values:
 *        - ADC_AVERAGE_4 : Hardware average of 4 samples..
 *        - ADC_AVERAGE_8 : Hardware average of 8 samples.
 *        - ADC_AVERAGE_16 : Hardware average of 16 samples.
 *        - ADC_AVERAGE_32 : Hardware average of 32 samples.
 */
static inline adc_average_t ADC_GetHwAverageMode(const adc_type_t *const baseAddr)
{
    uint32_t tmp = baseAddr->SC3;
    tmp          = (tmp & ADC_SC3_AVGS_MASK) >> ADC_SC3_AVGS_SHIFT;

    /* Enum defines all possible values, so casting is safe */
    return (adc_average_t)(tmp);
}

/**
 * @brief Sets the Hardware Average Mode
 *
 * This function configures the Hardware Average Mode. The
 * mode selects the number of samples to average: 4, 8, 16
 * or 32.
 *
 * @param baseAddr adc base pointer
 * @param averageMode the new Hardware Average Mode.
 *        - ADC_AVERAGE_4 : Hardware average of 4 samples..
 *        - ADC_AVERAGE_8 : Hardware average of 8 samples.
 *        - ADC_AVERAGE_16 : Hardware average of 16 samples.
 *        - ADC_AVERAGE_32 : Hardware average of 32 samples.
 */
static inline void ADC_SetHwAverageMode(adc_type_t *const baseAddr, const adc_average_t averageMode)
{
    uint32_t tmp = baseAddr->SC3;
    /* Clear the affected bitfield */
    tmp &= ~(ADC_SC3_AVGS_MASK);
    tmp |= ADC_SC3_AVGS(averageMode);
    baseAddr->SC3 = tmp;
}

/**
 * @brief Gets the ADC Scanning Mode state
 *
 * This function returns the ADC Scanning Mode.
 *
 * @param baseAddr adc base pointer
 * @return the ADC Scanning Mode state
 */
static inline bool ADC_GetScanningEnableFlag(const adc_type_t *const baseAddr)
{
    uint32_t tmp = (uint32_t)baseAddr->SC2;
    tmp          = (tmp & ADC_SC2_CSE_MASK) >> ADC_SC2_CSE_SHIFT;

    return (tmp != 0U) ? true : false;
}

/**
 * @brief Sets the ADC Scanning Mode state
 *
 * This function configures the ADC Scanning Mode.
 *
 * @param baseAddr adc base pointer
 * @param scanMode the new Scanning Mode state.
 *
 */
static inline void ADC_SetScanningEnableFlag(adc_type_t *const baseAddr, const bool scanMode)
{

    uint32_t tmp = (uint32_t)baseAddr->SC2;
    tmp &= (uint32_t)(~(ADC_SC2_CSE_MASK));
    tmp |= ADC_SC2_CSE(scanMode ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->SC2 = (uint32_t)tmp;
}

/**
 * @brief Gets the ADC Scanning Mode channels
 *
 * This function returns the effective san channel.
 * Channels corresponding to bits with value of '1' will be configured as effective scan channel
 * *
 * @param  base adc base pointer.
 * @return  Channel mask where each bit represents one channel. For each bit:
 *              - 0: corresponding channel is set to ineffective scan channel.
 *              - 1: corresponding channel is set to effective scan channel.
 */
static inline uint32_t ADC_GetScanningChannels(const adc_type_t *const baseAddr)
{
    uint32_t tmp = baseAddr->SC2;
    tmp          = (tmp & ADC_SC2_ESCN_MASK) >> ADC_SC2_ESCN_SHIFT;

    return (tmp);
}

/**
 * @brief Sets the ADC Scanning Mode channels
 *
 * This function sets the effective san channel.
 * Channels corresponding to bits with value of '1' will be configured as effective scan channel
 * *
 * @param  base adc base pointer.
 * @param  channels Channel mask where each bit represents one channel. For each bit:
 *              - 0: corresponding channel is set to ineffective scan channel.
 *              - 1: corresponding channel is set to effective scan channel.
 */
static inline void ADC_SetScanningChannels(adc_type_t *const baseAddr, uint32_t channels)
{
    uint32_t tmp = (uint32_t)baseAddr->SC2;
    tmp &= (uint32_t)(~(ADC_SC2_ESCN_MASK));
    tmp |= ADC_SC2_ESCN(channels);
    baseAddr->SC2 = (uint32_t)tmp;
}

/**
 * @brief Gets the Calibration Active Flag state
 *
 * This function returns the state of the Calibration Active
 * Flag. This flag is set if an Auto-Calibration sequence is
 * taking place.
 *
 * @param baseAddr adc base pointer
 * @return the Calibration Active Flag state
 */
static inline bool ADC_GetCalibrationActiveFlag(const adc_type_t *const baseAddr)
{
    uint32_t tmp = (uint32_t)baseAddr->CR;
    tmp          = (tmp & ADC_CR_CAL_MASK) >> ADC_CR_CAL_SHIFT;

    return (tmp != 0U) ? true : false;
}

/**
 * @brief Sets the Calibration Active Flag state
 *
 * This functions starts or aborts an Auto-Calibration
 * sequence. If this is set, it will remain set until the
 * sequence is finished.
 *
 * @param baseAddr adc base pointer
 * @param state the new Calibration Active Flag state
 */
static inline void ADC_SetCalibrationActiveFlag(adc_type_t *const baseAddr, const bool state)
{
    uint32_t tmp = baseAddr->CR;
    tmp &= ~(ADC_CR_CAL_MASK);
    tmp |= ADC_CR_CAL(state ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->CR = tmp;
}

/**
 * @brief Sets the Channel Interrupt Enable state
 *
 * This function set the state of the Channel Interrupt
 * Enable Flag. If the flag is set, an interrupt is generated
 * when the a conversion is completed for the channel.
 *
 * @param baseAddr adc base pointer
 * @param state the new Calibration Active Flag state
 */
static inline void ADC_SetChanInterruptEnableFlag(adc_type_t *const baseAddr, const uint8_t chanIndex, const bool state)
{
    uint32_t tmp = baseAddr->SC1[chanIndex];
    tmp &= ~(ADC_SC1_CIEN_MASK);
    tmp |= ADC_SC1_CIEN(state ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->SC1[chanIndex] = tmp;
}

/**
 * @brief Gets the Channel Interrupt Enable state
 *
 * This function returns the state of the Channel Interrupt
 * Enable Flag. If the flag is set, an interrupt is generated
 * when the a conversion is completed for the channel.
 *
 * @param baseAddr adc base pointer
 * @param chanIndex the adc measurement channel index
 * @return the Channel Interrupt Enable Flag state
 */
static inline bool ADC_GetChanInterruptEnableFlag(const adc_type_t *const baseAddr, const uint8_t chanIndex)
{
    uint32_t tmp = (uint32_t)baseAddr->SC1[chanIndex];
    tmp          = (tmp & ADC_SC1_CIEN_MASK) >> ADC_SC1_CIEN_SHIFT;

    return (tmp != 0U) ? true : false;
}

/**
 * @brief Gets the configured input channel for the selected measurement channel
 *
 * This function returns the configured input channel for a
 * measurement channel.
 *
 * @param baseAddr adc base pointer
 * @param chanIndex the adc measurement channel index
 * @return the Input Channel selected for the Measurement Channel.
 */
static inline adc_inputchannel_t ADC_GetInputChannel(const adc_type_t *const baseAddr, const uint8_t chanIndex)
{
    uint32_t tmp = baseAddr->SC1[chanIndex];
    tmp          = (tmp & ADC_SC1_ADCH_MASK) >> ADC_SC1_ADCH_SHIFT;

    /* Enum defines all possible values, so casting is safe */
    return (adc_inputchannel_t)(tmp);
}

/**
 * @brief Sets the input channel and Channel Interrupt Enable as configuration for the measurement channel.
 *
 * This function configures the input channel and state of the Interrupt Enable Flag for a measurement
 * channel. In software trigger mode, configuring channel
 * A (index 0) will start a new conversion immediately.
 * If the flag is set, an interrupt is generated when
 * the a conversion is completed for the channel.
 *
 * @param baseAddr adc base pointer
 * @param chanIndex the adc measurement channel index
 * @param inputChan the Input Channel selected for the Measurement Channel
 * @param state the new Channel Interrupt Enable Flag state
 */
static inline void ADC_SetInputChannel(adc_type_t *const baseAddr, const uint8_t chanIndex,
                                       const adc_inputchannel_t inputChan, const bool state)
{
    adc_inputchannel_t inputChanDemapped = inputChan;

    uint32_t tmp = baseAddr->SC1[chanIndex];
    tmp &= ~(ADC_SC1_ADCH_MASK);
    tmp |= ADC_SC1_ADCH(inputChanDemapped);
    tmp &= ~(ADC_SC1_CIEN_MASK);
    tmp |= ADC_SC1_CIEN(state ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->SC1[chanIndex] = tmp;
}

#if defined(__cplusplus)
}
#endif

#endif
