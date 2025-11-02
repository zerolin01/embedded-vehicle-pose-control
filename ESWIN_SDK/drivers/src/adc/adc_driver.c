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
 * @file adc_driver.c
 * @brief ADC driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "adc_driver.h"
#include "adc_hw_access.h"
#include "clock_driver.h"
#include <stddef.h>

/**
 * @brief Table of base addresses for ADC instances.
 */
static adc_type_t *const s_adcBase[ADC_INSTANCE_COUNT] = ADC_BASE_PTRS;

/**
 * @brief Table of IRQs for DAC instances.
 */
static const IRQn_Type s_adcIrqId[ADC_INSTANCE_COUNT] = ADC_IRQS;

/**
 *
 * Function Name : ADC_DRV_IRQHandler
 * Description   : IRQ Handler.
 *
 */
static void ADC_DRV_IRQHandler(void *data)
{
    adc_state_t *state = (adc_state_t *)data;
    adc_type_t *const base = s_adcBase[state->instance];
    uint32_t intStatus = ADC_GetIntStatus(base);
    if (state->callback) {
        state->callback(state->callbackParam, intStatus);
    }
}

/**
 *
 * Function Name : ADC_DRV_PowerOnReset
 * Description   : ADC has a power-on sequence, and the software must meet
 * the power-on reset requirements of the ADC module.
 *
 */
void ADC_DRV_PowerOnReset(const uint32_t instance)
{
    adc_type_t *const base = s_adcBase[instance];
    ADC_SetModuleEnableFlag(base, true);
    OS_DelayUs(1);
    ADC_SetModuleSoftReset(base, true);
}

/**
 *
 * Function Name : ADC_GetSampleRate
 * Description   : This function returns the configured sample rate for the ADC.
 *
 */
static uint32_t ADC_GetSampleRate(const uint32_t instance)
{
    OS_ASSERT(instance < ADC_INSTANCE_COUNT);

    adc_type_t *const base = s_adcBase[instance];

    uint32_t adcFreq                            = 0U;
    clock_names_t adcClocks[ADC_INSTANCE_COUNT] = ADC_CLOCKS;
    status_t clkStatus                          = CLOCK_SYS_GetFreq(adcClocks[instance], &adcFreq);
    OS_ASSERT(clkStatus == STATUS_SUCCESS);
    (void)clkStatus;

    double frequencyWord = (double)base->FW;
    return (uint32_t)(frequencyWord * (double)adcFreq / (uint64_t)FEATURE_ADC_CALC_SAMPLERATE_CONST);
}

/**
 *
 * Function Name : ADC_DRV_SetSampleRate
 * Description   : This function sets the configured sample rate for the ADC.
 *
 */
void ADC_DRV_SetSampleRate(const uint32_t instance, const uint32_t sampleRate)
{
    OS_ASSERT(instance < ADC_INSTANCE_COUNT);

    adc_type_t *const base                      = s_adcBase[instance];
    uint32_t adcFreq                            = 0U;
    clock_names_t adcClocks[ADC_INSTANCE_COUNT] = ADC_CLOCKS;

    status_t clkStatus = CLOCK_SYS_GetFreq(adcClocks[instance], &adcFreq);
    OS_ASSERT(clkStatus == STATUS_SUCCESS);
    //OS_ASSERT((adcFreq >= ADC_CLOCK_FREQ_MIN_RUNTIME) && (adcFreq <= ADC_CLOCK_FREQ_MAX_RUNTIME));
    (void)clkStatus;

    double frequencyWord = (uint64_t)FEATURE_ADC_CALC_SAMPLERATE_CONST / (double)adcFreq * (double)sampleRate + 0.5;
    base->FW             = (uint32_t)frequencyWord;
    uint64_t FPH0        = 0;

    do {
        FPH0 += base->FW;
    } while (!(FPH0 >> 32));

    FPH0       = (FPH0 & 0xFFFFFFFF) + base->FW;
    base->FPH0 = (uint32_t)FPH0;
    base->FPH1 = (FPH0 >> 32) ? 1 : 0;
}

/*
 *
 * Function Name : ADC_DRV_Init
 * Description   : Initializes adc module.
 *
 */
void ADC_DRV_Init(const uint32_t instance, adc_state_t *state)
{
    OS_ASSERT(state != NULL);
    OS_ASSERT(instance < ADC_INSTANCE_COUNT);

    clock_names_t adcClocks[ADC_INSTANCE_COUNT] = ADC_CLOCKS;
    CLOCK_SYS_ClockEnable(adcClocks[instance], true);

    OS_RegisterType_t type;
    type.trig_mode = CLIC_LEVEL_TRIGGER;
    type.lvl       = 1;
    type.priority  = 0;
    type.data_ptr  = (void *)state;
    state->instance = instance;
    OS_RequestIrq(s_adcIrqId[instance], ADC_DRV_IRQHandler, &type);

    ADC_DRV_PowerOnReset(instance);
}

/*
 *
 * Function Name : ADC_DRV_Init
 * Description   : Initializes adc module.
 *
 */
void ADC_DRV_Deinit(const uint32_t instance)
{
    OS_ASSERT(instance < ADC_INSTANCE_COUNT);

    ADC_DRV_SetOperationState(instance, false);
    ADC_DRV_Reset(instance);
    clock_names_t adcClocks[ADC_INSTANCE_COUNT] = ADC_CLOCKS;
    CLOCK_SYS_ClockEnable(adcClocks[instance], false);
    OS_UnRequestIrq(s_adcIrqId[instance]);
}

/**
 *
 * Function Name : ADC_DRV_InitConverterStruct
 * Description   : This function initializes the members of the adc_converter_config_t
 * structure to default values (Reference Manual resets). This function should be called
 * on a structure before using it to configure the converter (ADC_DRV_ConfigConverter), otherwise all members
 * must be written (initialized) by the caller. This function insures that all members are written
 * with safe values, so the user can modify only the desired members.
 *
 */
void ADC_DRV_InitConverterStruct(adc_converter_config_t *const config)
{
    OS_ASSERT(config != NULL);

    config->sampleRate           = 100000;
    config->resolution           = ADC_RESOLUTION_12BIT;
    config->trigger              = ADC_TRIGGER_SOFTWARE;
    config->pretriggerSel        = ADC_PRETRIGGER_SEL_PDU;
    config->triggerSel           = ADC_TRIGGER_SEL_PDU;
    config->dmaEnable            = false;
    config->dataFmt              = ADC_ADMA_DATAFMT_MODE0;
    config->continuousConvEnable = false;
    config->scanningEnable       = false;
    config->scanningChannles     = 0;
}

/**
 *
 * Function Name : ADC_DRV_ConfigConverter
 * Description   : This function configures the ADC converter with the options
 * provided in the configuration structure.
 *
 */
void ADC_DRV_ConfigConverter(const uint32_t instance, const adc_converter_config_t *const config)
{
    OS_ASSERT(instance < ADC_INSTANCE_COUNT);
    OS_ASSERT(config != NULL);

    /* Some alternative clocks can be unavailable depending on the device */
    adc_type_t *const base                      = s_adcBase[instance];
    clock_names_t adcClocks[ADC_INSTANCE_COUNT] = ADC_CLOCKS;
    uint32_t adcFreq                            = 0U;
    status_t clkStatus                          = CLOCK_SYS_GetFreq(adcClocks[instance], &adcFreq);
    OS_ASSERT(clkStatus == STATUS_SUCCESS);
    //OS_ASSERT((adcFreq >= ADC_CLOCK_FREQ_MIN_RUNTIME) && (adcFreq <= ADC_CLOCK_FREQ_MAX_RUNTIME));
    (void)clkStatus;

    ADC_DRV_SetSampleRate(instance, config->sampleRate);
    ADC_SetDifferenceMode(base, config->differenceEnable);
    ADC_SetTriggerMode(base, config->trigger);
    ADC_SetPretriggerSelect(instance, config->pretriggerSel);
    ADC_SetTriggerSelect(instance, config->triggerSel);
    if (config->dmaEnable) {
        ADC_SetAdmaDataFormatMode(base, config->dataFmt);
    }
    ADC_SetDMAEnableFlag(base, config->dmaEnable);
    ADC_SetContinuousConvFlag(base, config->continuousConvEnable);
    ADC_SetScanningChannels(base, config->scanningChannles);
    ADC_SetScanningEnableFlag(base, config->scanningEnable);
}

/**
 *
 * Function Name : ADC_DRV_GetConverterConfig
 * Description   : This functions returns the current converter configuration in
 * the form of a configuration structure.
 *
 */
void ADC_DRV_GetConverterConfig(const uint32_t instance, adc_converter_config_t *const config)
{
    OS_ASSERT(instance < ADC_INSTANCE_COUNT);
    OS_ASSERT(config != NULL);

    const adc_type_t *const base = s_adcBase[instance];
    config->sampleRate           = ADC_GetSampleRate(instance);
    config->resolution           = ADC_GetResolution(base);
    config->trigger              = ADC_GetTriggerMode(base);
    config->triggerSel           = ADC_GetTriggerSelect(instance);
    config->pretriggerSel        = ADC_GetPretriggerSelect(instance);
    config->dmaEnable            = ADC_GetDMAEnableFlag(base);
    config->dataFmt              = ADC_GetAdmaDataFormatMode(base);
    config->continuousConvEnable = ADC_GetContinuousConvFlag(base);
    config->scanningEnable       = ADC_GetScanningEnableFlag(base);
    config->scanningChannles     = ADC_GetScanningChannels(base);
}

/**
 *
 * Function Name : ADC_DRV_Reset
 * Description   : This function writes all the internal ADC registers with
 * their Reference Manual reset values.
 *
 */
void ADC_DRV_Reset(const uint32_t instance)
{
    adc_type_t *const base = s_adcBase[instance];
    ADC_SetModuleSoftReset(base, true);
}

/**
 *
 * Function Name : ADC_DRV_InitHwCompareStruct
 * Description   : This function initializes the Hardware Compare configuration
 * structure to default values (Reference Manual resets). This function should be
 * called before configuring the Hardware Compare feature (ADC_DRV_ConfigHwCompare),
 * otherwise all members must be written by the caller. This function insures that all
 * members are written with safe values, so the user can modify the desired members.
 *
 */
void ADC_DRV_InitHwCompareStruct(adc_compare_config_t *const config)
{
    OS_ASSERT(config != NULL);

    config->compareEnable            = false;
    config->compareGreaterThanEnable = false;
    config->compareRangeFuncEnable   = false;
    config->compVal1                 = 0U;
    config->compVal2                 = 0U;
}

/**
 *
 * Function Name : ADC_DRV_ConfigHwCompare
 * Description   : This functions sets the configuration for the Hardware
 * Compare feature using the configuration structure.
 *
 */
void ADC_DRV_ConfigHwCompare(const uint32_t instance, const adc_compare_config_t *const config)
{
    OS_ASSERT(instance < ADC_INSTANCE_COUNT);
    OS_ASSERT(config != NULL);

    adc_type_t *const base = s_adcBase[instance];
    ADC_SetHwCompareEnableFlag(base, config->compareEnable);
    ADC_SetHwCompareGtEnableFlag(base, config->compareGreaterThanEnable);
    ADC_SetHwCompareRangeEnableFlag(base, config->compareRangeFuncEnable);
    ADC_SetHwCompareComp1Value(base, config->compVal1);
    ADC_SetHwCompareComp2Value(base, config->compVal2);
}

/**
 *
 * Function Name : ADC_DRV_GetHwCompareConfig
 * Description   : This function returns the configuration for the Hardware Compare feature.
 *
 */
void ADC_DRV_GetHwCompareConfig(const uint32_t instance, adc_compare_config_t *const config)
{
    OS_ASSERT(instance < ADC_INSTANCE_COUNT);
    OS_ASSERT(config != NULL);

    const adc_type_t *const base     = s_adcBase[instance];
    config->compareEnable            = ADC_GetHwCompareEnableFlag(base);
    config->compareGreaterThanEnable = ADC_GetHwCompareGtEnableFlag(base);
    config->compareRangeFuncEnable   = ADC_GetHwCompareRangeEnableFlag(base);
    config->compVal1                 = ADC_GetHwCompareComp1Value(base);
    config->compVal2                 = ADC_GetHwCompareComp2Value(base);
}

/**
 *
 * Function Name : ADC_DRV_InitHwAverageStruct
 * Description   : This function initializes the Hardware Average configuration
 * structure to default values (Reference Manual resets). This function should be
 * called before configuring the Hardware Average feature (ADC_DRV_ConfigHwAverage),
 * otherwise all members must be written by the caller. This function insures that all
 * members are written with safe values, so the user can modify the desired members.
 *
 */
void ADC_DRV_InitHwAverageStruct(adc_average_config_t *const config)
{
    OS_ASSERT(config != NULL);

    config->hwAvgEnable = false;
    config->hwAverage   = ADC_AVERAGE_4;
}

/**
 *
 * Function Name : ADC_DRV_ConfigHwAverage
 * Description   : This function sets the configuration for the Hardware Average feature.
 *
 */
void ADC_DRV_ConfigHwAverage(const uint32_t instance, const adc_average_config_t *const config)
{
    OS_ASSERT(instance < ADC_INSTANCE_COUNT);
    OS_ASSERT(config != NULL);

    adc_type_t *const base = s_adcBase[instance];
    ADC_SetHwAverageEnableFlag(base, config->hwAvgEnable);
    ADC_SetHwAverageMode(base, config->hwAverage);
}

/**
 *
 * Function Name : ADC_DRV_GetHwAverageConfig
 * Description   : This function returns the configuration for the Hardware Average feature.
 *
 */
void ADC_DRV_GetHwAverageConfig(const uint32_t instance, adc_average_config_t *const config)
{
    OS_ASSERT(instance < ADC_INSTANCE_COUNT);
    OS_ASSERT(config != NULL);

    const adc_type_t *const base = s_adcBase[instance];
    config->hwAvgEnable          = ADC_GetHwAverageEnableFlag(base);
    config->hwAverage            = ADC_GetHwAverageMode(base);
}

/**
 *
 * Function Name : ADC_DRV_InitChanStruct
 * Description   : This function initializes the control channel
 * configuration structure to default values (Reference Manual resets). This function should
 * be called on a structure before using it to configure a channel (ADC_DRV_ConfigChan), otherwise
 * all members must be written by the caller. This function insures that all members are written
 * with safe values, so the user can modify only the desired members.
 *
 */
void ADC_DRV_InitChanStruct(adc_chan_config_t *const config)
{
    OS_ASSERT(config != NULL);

    config->interruptEnable = false;
    config->channel         = ADC_INPUTCHAN_DISABLED;
}

/**
 *
 * Function Name : ADC_DRV_ConfigChan
 * Description   : This function sets a control channel configuration.
 *
 */
void ADC_DRV_ConfigChan(const uint32_t instance, const uint8_t chanIndex, const adc_chan_config_t *const config)
{
    OS_ASSERT(instance < ADC_INSTANCE_COUNT);
    OS_ASSERT(chanIndex < ADC_CTRL_CHANS_COUNT);
    OS_ASSERT(config != NULL);

    adc_type_t *const base = s_adcBase[instance];

    if (config->interruptEnable){
        OS_EnableIrq(s_adcIrqId[instance]);
    }
    ADC_SetInputChannel(base, chanIndex, config->channel, config->interruptEnable);
}

/**
 *
 * Function Name : ADC_DRV_GetChanConfig
 * Description   : This function returns the current configuration for a control channel.
 *
 */
void ADC_DRV_GetChanConfig(const uint32_t instance, const uint8_t chanIndex, adc_chan_config_t *const config)
{
    OS_ASSERT(instance < ADC_INSTANCE_COUNT);
    OS_ASSERT(chanIndex < ADC_CTRL_CHANS_COUNT);
    OS_ASSERT(config != NULL);

    const adc_type_t *const base = s_adcBase[instance];
    config->interruptEnable      = ADC_GetChanInterruptEnableFlag(base, chanIndex);
    config->channel              = ADC_GetInputChannel(base, chanIndex);
}

/**
 *
 * Function Name : ADC_DRV_SetSwPretrigger
 * Description   : This function sets the software pretrigger - affects only first 4 control channels.
 *
 */
void ADC_DRV_SetSwPretrigger(const uint32_t instance, const adc_sw_pretrigger_t swPretrigger)
{
    OS_ASSERT(instance < ADC_INSTANCE_COUNT);
    tcsr_type_t *const tcsrBase       = TCSR;
    uint32_t intermValue              = 0U;
    uint32_t mask[ADC_INSTANCE_COUNT] = {0U};
#if (ADC_INSTANCE_COUNT == 1U)
    mask[0] = TCSR_ADCOPT_ADC0SWPRETRG_MASK;
#elif (ADC_INSTANCE_COUNT == 2U)
    mask[0] = TCSR_ADCOPT_ADC0SWPRETRG_MASK;
    mask[1] = TCSR_ADCOPT_ADC1SWPRETRG_MASK;
#else
#error "Maximum supported value for ADC_INSTANCE_COUNT is 2."
#endif
    /* If SW Pretrigger Select is not enabled, the SW pretriggers will be ignored by ADC. */
    OS_ASSERT((ADC_GetPretriggerSelect(instance) == ADC_PRETRIGGER_SEL_SW) ||
              (swPretrigger == ADC_SW_PRETRIGGER_DISABLED));

    intermValue = tcsrBase->ADCCONFIG & (~mask[instance]);
    switch (instance) {
    case 0:
        intermValue |= TCSR_ADCOPT_ADC0SWPRETRG(swPretrigger);
        break;
    case 1:
        intermValue |= TCSR_ADCOPT_ADC1SWPRETRG(swPretrigger);
        break;
    default:
        OS_ASSERT(false);
        break;
    }

    tcsrBase->ADCCONFIG = intermValue;
}

/**
 *
 * Function Name : ADC_DRV_TriggerSwRequest
 * Description   : This function sets the software trigger.
 *
 */
void ADC_DRV_TriggerSwRequest(const uint32_t instance)
{
    OS_ASSERT(instance < ADC_INSTANCE_COUNT);
    adc_type_t *const base = s_adcBase[instance];

    uint32_t tmp = (uint32_t)base->CR;
    tmp &= (uint32_t)(~(ADC_CR_STR_MASK));
    tmp |= ADC_CR_STR(1U);
    base->CR = (uint32_t)tmp;
}

/**
 *
 * Function Name : ADC_DRV_SetOperationState
 * Description   : This function enable the adc conversion.
 *
 */
void ADC_DRV_SetOperationState(const uint32_t instance, bool state)
{
    OS_ASSERT(instance < ADC_INSTANCE_COUNT);
    adc_type_t *const base = s_adcBase[instance];

    ADC_SetOperationEnableFlag(base, state);
}

/**
 *
 * Function Name : ADC_DRV_WaitConvDone
 * Description   : This functions waits for a conversion to complete by
 * continuously polling the Conversion Active Flag.
 *
 */
void ADC_DRV_WaitConvDone(const uint32_t instance)
{
    OS_ASSERT(instance < ADC_INSTANCE_COUNT);

    const adc_type_t *const base = s_adcBase[instance];
    while (ADC_GetConvActiveFlag(base) == true) {
        /* Wait for conversion to finish */
    }
}

/**
 *
 * Function Name : ADC_DRV_GetConvCompleteFlag
 * Description   : This function returns the state of the Conversion Complete
 * flag for a control channel. This flag is set when a conversion is complete
 * or the condition generated by the Hardware Compare feature is evaluated to true.
 *
 */
bool ADC_DRV_GetConvCompleteFlag(const uint32_t instance, const uint8_t chanIndex)
{
    OS_ASSERT(instance < ADC_INSTANCE_COUNT);
    OS_ASSERT(chanIndex < ADC_CTRL_CHANS_COUNT);

    const adc_type_t *const base = s_adcBase[instance];

    uint32_t tmp = base->SC1[chanIndex];
    tmp          = (tmp & ADC_SC1_CCF_MASK) >> ADC_SC1_CCF_SHIFT;

    return (tmp != 0U) ? true : false;
}

/**
 *
 * Function Name : ADC_DRV_GetChanResult
 * Description   : This function returns the conversion result from a control channel.
 * 
 */
void ADC_DRV_GetChanResult(const uint32_t instance, const uint8_t chanIndex, uint16_t *const result)
{
    OS_ASSERT(instance < ADC_INSTANCE_COUNT);
    OS_ASSERT(result != NULL);

    const adc_type_t *const base = s_adcBase[instance];

    OS_ASSERT(chanIndex < ADC_R_COUNT);

    uint32_t tmp = base->R[chanIndex];
    tmp          = (tmp & ADC_R_D_MASK) >> ADC_R_D_SHIFT;

    *result = (uint16_t)tmp;
}

/**
 *
 * Function Name : ADC_DRV_AutoCalibration
 * Description   : This functions executes an Auto-Calibration sequence. 
 * It is recommended to run this sequence before using the ADC converter.
 *
 */
void ADC_DRV_AutoCalibration(const uint32_t instance)
{
    OS_ASSERT(instance < ADC_INSTANCE_COUNT);

    adc_type_t *const base = s_adcBase[instance];
    /* set hardware average to maximum and set software trigger*/
    bool hwavgen        = ADC_GetHwAverageEnableFlag(base);
    adc_average_t hwavg = ADC_GetHwAverageMode(base);
    adc_trigger_t trig  = ADC_GetTriggerMode(base);
    bool dmaen          = ADC_GetDMAEnableFlag(base);
    bool scanning       = ADC_GetScanningEnableFlag(base);
    bool intr           = ADC_GetChanInterruptEnableFlag(base, 0);
    ADC_SetChanInterruptEnableFlag(base, 0, false);
    ADC_SetHwAverageMode(base, ADC_AVERAGE_32);
    ADC_SetHwAverageEnableFlag(base, true);
    ADC_SetDMAEnableFlag(base, false);
    ADC_SetScanningEnableFlag(base, false);
    ADC_SetTriggerMode(base, ADC_TRIGGER_SOFTWARE);

    //base->FW   = 0xFFFFFFFF;
    //base->FPH0 = 0;
    //base->FPH1 = 0;

    /* start calibration */
    base->COR = 0;
    ADC_SetOperationEnableFlag(base, true);
    ADC_SetCalibrationActiveFlag(base, true);
    while (ADC_GetCalibrationActiveFlag(base)) {
        /* Wait for calibration to finish */
    }

    uint32_t tmp = base->R[0];
    if (tmp > FEATURE_ADC_CALIBRATION_VALUE) {
        tmp -= FEATURE_ADC_CALIBRATION_VALUE;
        tmp = ((~tmp + 0x00000001) & (ADC_COR_OFS_MASK | ADC_COR_OFS_SIGN_MASK));
    } else {
        tmp = (FEATURE_ADC_CALIBRATION_VALUE - tmp) & ADC_COR_OFS_MASK;
    }
    base->COR = tmp;

    /* restore hardware average and trigger settings*/
    ADC_SetChanInterruptEnableFlag(base, 0, intr);
    ADC_SetScanningEnableFlag(base, scanning);
    ADC_SetHwAverageEnableFlag(base, hwavgen);
    ADC_SetHwAverageMode(base, hwavg);
    ADC_SetDMAEnableFlag(base, dmaen);
    ADC_SetTriggerMode(base, trig);
}