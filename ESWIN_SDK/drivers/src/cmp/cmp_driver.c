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
 * @file cmp_driver.c
 * @brief CMP driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "cmp_driver.h"
#include "cmp_hw_access.h"
#include "clock_driver.h"
#include "emps_platform.h"
#include <stddef.h>

/**
 * @brief Table of base addresses for CMP instances.
 */
static cmp_type_t *const g_cmpBase[] = CMP_BASE_PTRS;

/**
 * @brief Table of IRQs for CMP instances.
 */
static const IRQn_Type s_cmpIrqId[CMP_INSTANCE_COUNT] = CMP_IRQS;

/**
 *
 * Function Name : CMP_DRV_Reset
 * Description   : This function set all CMP registers to reset values.
 *
 */
status_t CMP_DRV_Reset(const uint32_t instance)
{
    status_t status = STATUS_SUCCESS;
    OS_ASSERT(instance < CMP_INSTANCE_COUNT);
    cmp_type_t *baseAddr = g_cmpBase[instance];
    baseAddr->C0         = 0;
    baseAddr->C1         = 0;
    baseAddr->C2         = 0;
    baseAddr->C3         = 0;
    return status;
}

/**
 *
 * Function Name : CMP_DRV_GetEnableState
 * Description   : This function get comparator module state.
 *
 */
bool CMP_DRV_GetEnableState(const uint32_t instance)
{
    OS_ASSERT(instance < CMP_INSTANCE_COUNT);

    cmp_type_t *base = g_cmpBase[instance];
    return CMP_GetEnableState(base);
}

/**
 *
 * Function Name : CMP_DRV_SetEnableState
 * Description   : This function enable or disable comparator module.
 *
 */
status_t CMP_DRV_SetEnableState(const uint32_t instance, bool enable)
{
    status_t status = STATUS_SUCCESS;
    OS_ASSERT(instance < CMP_INSTANCE_COUNT);

    cmp_type_t *base = g_cmpBase[instance];
    CMP_SetEnableState(base, enable);

    return status;
}

/**
 *
 * Function Name : CMP_DRV_IRQHandler
 * Description   : IRQ Handler.
 *
 */
static void CMP_DRV_IRQHandler(void *data)
{
    cmp_state_t *state = (cmp_state_t *)data;

    if (state->callback) {
        state->callback(state->callbackParam);
    }
    CMP_DRV_ClearOutputFlags(state->instance);
}

/**
 *
 * Function Name : CMP_DRV_Init
 * Description   : Configure all components from comparator module.
 *
 */
status_t CMP_DRV_Init(const uint32_t instance, const cmp_module_t *const config, cmp_state_t *state)
{
    status_t status = STATUS_SUCCESS;
    OS_ASSERT(config != NULL);
    OS_ASSERT(instance < CMP_INSTANCE_COUNT);
    cmp_type_t *base = g_cmpBase[instance];

    clock_names_t cmpClocks[CMP_INSTANCE_COUNT] = CMP_CLOCKS;
    CLOCK_SYS_ClockEnable(cmpClocks[instance], true);

    CMP_SetEnableState(base, false);
    CMP_SetDMATriggerState(base, (config->comparator).dmaTriggerState);
    CMP_SetOutputInterruptTrigger(base, (config->comparator).outputInterruptTrigger);
    CMP_SetFunctionalMode(base, (config->comparator).mode, (config->comparator).filterSampleCount,
                          (config->comparator).filterSamplePeriod);
    CMP_SetPowerMode(base, (config->comparator).powerMode);
    CMP_SetInverterState(base, (config->comparator).inverterState);
    CMP_SetComparatorOutputSource(base, (config->comparator).outputSelect);
    CMP_SetOutputPinState(base, (config->comparator).pinState);
    CMP_SetHysteresis(base, (config->comparator).hysteresisLevel);
    CMP_SetMinusMUXControl(base, (config->mux).negativeInputMux);
    CMP_SetPlusMuxControl(base, (config->mux).positiveInputMux);
    CMP_SetNegativePortInput(base, (config->mux).negativePortMux);
    CMP_SetPositivePortInput(base, (config->mux).positivePortMux);
    CMP_SetFixedPort(base, (config->triggerMode).fixedPort);
    CMP_SetFixedChannel(base, (config->triggerMode).fixedChannel);
    CMP_SetRoundRobinSamplesNumber(base, (config->triggerMode).samples);
    CMP_SetRoundRobinChannels(base, (config->triggerMode).roundRobinChannelsState);
    CMP_SetPresetState(base, (config->triggerMode).programedState);
    CMP_SetRoundRobinInterruptState(base, (config->triggerMode).roundRobinInterruptState);
    CMP_SetRoundRobinState(base, (config->triggerMode).roundRobinState);
    CMP_SetInitDelay(base, (config->triggerMode).initializationDelay);

    if ((config->comparator).outputInterruptTrigger) {
        /* Install Comparator interrupt handler */
        OS_RegisterType_t type;
        type.trig_mode  = CLIC_LEVEL_TRIGGER;
        type.lvl        = 1;
        type.priority   = 0;
        type.data_ptr   = state;
        state->instance = instance;
        OS_RequestIrq(s_cmpIrqId[instance], CMP_DRV_IRQHandler, &type);
        /* Enable Comparator interrupt */
        OS_EnableIrq(s_cmpIrqId[instance]);
    }

    /* Clear all flags*/
    (void)CMP_DRV_ClearOutputFlags(instance);
    (void)CMP_DRV_ClearInputFlags(instance);

    return status;
}
/**
 *
 * Function Name : CMP_DRV_GetConfigAll
 * Description   : This function returns the configuration for all components
 * from comparator module.
 *
 */
status_t CMP_DRV_GetConfigAll(const uint32_t instance, cmp_module_t *const config)
{
    status_t status = STATUS_SUCCESS;
    OS_ASSERT(config != NULL);
    OS_ASSERT(instance < CMP_INSTANCE_COUNT);

    const cmp_type_t *base                         = g_cmpBase[instance];
    (config->comparator).dmaTriggerState           = CMP_GetDMATriggerState(base);
    (config->comparator).outputInterruptTrigger    = CMP_GetOutputInterruptTrigger(base);
    (config->comparator).mode                      = CMP_GetFunctionalMode(base);
    (config->comparator).filterSampleCount         = CMP_GetFilterSampleCount(base);
    (config->comparator).filterSamplePeriod        = CMP_GetFilterSamplePeriod(base);
    (config->comparator).powerMode                 = CMP_GetPowerMode(base);
    (config->comparator).inverterState             = CMP_GetInverterState(base);
    (config->comparator).outputSelect              = CMP_GetComparatorOutputSource(base);
    (config->comparator).pinState                  = CMP_GetOutputPinState(base);
    (config->comparator).hysteresisLevel           = CMP_GetHysteresis(base);
    (config->mux).negativePortMux                  = CMP_GetNegativePortInput(base);
    (config->mux).positivePortMux                  = CMP_GetPositivePortInput(base);
    (config->mux).negativeInputMux                 = CMP_GetMinusMUXControl(base);
    (config->mux).positiveInputMux                 = CMP_GetPlusMUXControl(base);
    (config->triggerMode).roundRobinState          = CMP_GetRoundRobinState(base);
    (config->triggerMode).roundRobinInterruptState = CMP_GetRoundRobinInterruptState(base);
    (config->triggerMode).fixedPort                = CMP_GetFixedPort(base);
    (config->triggerMode).fixedChannel             = CMP_GetFixedChannel(base);
    (config->triggerMode).samples                  = CMP_GetRoundRobinSamplesNumber(base);
    (config->triggerMode).roundRobinChannelsState  = CMP_GetRoundRobinChannels(base);
    (config->triggerMode).programedState           = CMP_GetLastComparisonResult(base);
    (config->triggerMode).initializationDelay      = CMP_GetInitDelay(base);

    return status;
}

/**
 *
 * Function Name : CMP_DRV_ConfigMUX
 * Description   : Configure only MUX component from comparator module to select
 * source signals for comparator ports.
 *
 */
status_t CMP_DRV_ConfigMUX(const uint32_t instance, const cmp_anmux_t *config)
{
    status_t status = STATUS_SUCCESS;
    OS_ASSERT(config != NULL);
    OS_ASSERT(instance < CMP_INSTANCE_COUNT);
    cmp_type_t *base = g_cmpBase[instance];

    CMP_SetNegativePortInput(base, config->negativePortMux);
    CMP_SetPositivePortInput(base, config->positivePortMux);
    CMP_SetMinusMUXControl(base, config->negativeInputMux);
    CMP_SetPlusMuxControl(base, config->positiveInputMux);

    return status;
}
/**
 *
 * Function Name : CMP_DRV_GetMUX
 * Description   : Return configuration for the MUX component which select
 * source signals for comparator ports.
 *
 */
status_t CMP_DRV_GetMUXConfig(const uint32_t instance, cmp_anmux_t *const config)
{
    status_t status = STATUS_SUCCESS;
    OS_ASSERT(config != NULL);
    OS_ASSERT(instance < CMP_INSTANCE_COUNT);
    const cmp_type_t *base   = g_cmpBase[instance];
    config->negativePortMux  = CMP_GetNegativePortInput(base);
    config->positivePortMux  = CMP_GetPositivePortInput(base);
    config->negativeInputMux = CMP_GetMinusMUXControl(base);
    config->positiveInputMux = CMP_GetPlusMUXControl(base);

    return status;
}

/**
 *
 * Function Name : CMP_DRV_ConfigTriggerMode
 * Description   : Configure comparator in trigger mode.
 *
 */
status_t CMP_DRV_ConfigTriggerMode(const uint32_t instance, const cmp_trigger_mode_t *config)
{
    status_t status = STATUS_SUCCESS;
    OS_ASSERT(config != NULL);
    OS_ASSERT(instance < CMP_INSTANCE_COUNT);
    cmp_type_t *base = g_cmpBase[instance];
    CMP_SetRoundRobinState(base, config->roundRobinState);
    CMP_SetRoundRobinInterruptState(base, config->roundRobinInterruptState);
    CMP_SetFixedPort(base, config->fixedPort);
    CMP_SetFixedChannel(base, config->fixedChannel);
    CMP_SetRoundRobinSamplesNumber(base, config->samples);
    CMP_SetInitDelay(base, config->initializationDelay);
    CMP_SetRoundRobinChannels(base, config->roundRobinChannelsState);
    CMP_SetPresetState(base, config->programedState);

    return status;
}
/**
 *
 * Function Name : CMP_DRV_GetTriggerMode
 * Description   : Return configuration for the trigger mode.
 *
 */
status_t CMP_DRV_GetTriggerModeConfig(const uint32_t instance, cmp_trigger_mode_t *const config)
{
    status_t status = STATUS_SUCCESS;
    OS_ASSERT(config != NULL);
    OS_ASSERT(instance < CMP_INSTANCE_COUNT);

    const cmp_type_t *base           = g_cmpBase[instance];
    config->roundRobinState          = CMP_GetRoundRobinState(base);
    config->roundRobinInterruptState = CMP_GetRoundRobinInterruptState(base);
    config->fixedPort                = CMP_GetFixedPort(base);
    config->fixedChannel             = CMP_GetFixedChannel(base);
    config->samples                  = CMP_GetRoundRobinSamplesNumber(base);
    config->roundRobinChannelsState  = CMP_GetRoundRobinChannels(base);
    config->programedState           = CMP_GetLastComparisonResult(base);
    config->initializationDelay      = CMP_GetInitDelay(base);

    return status;
}
/**
 *
 * Function Name : CMP_DRV_GetOutputFlags
 * Description   : Return in flags comparator output flags(rising and falling edge on output).
 *
 */
status_t CMP_DRV_GetOutputFlags(const uint32_t instance, cmp_output_trigger_t *flags)
{
    status_t status = STATUS_SUCCESS;
    OS_ASSERT(flags != NULL);
    OS_ASSERT(instance < CMP_INSTANCE_COUNT);

    const cmp_type_t *baseAddr = g_cmpBase[instance];
    bool risingEnabled         = (bool)(((baseAddr->C0) >> CMP_C0_CRF_SHIFT) & (uint32_t)1U);
    bool fallingEnabled        = (bool)(((baseAddr->C0) >> CMP_C0_CFF_SHIFT) & (uint32_t)1U);
    *flags                     = (cmp_output_trigger_t)(((uint32_t)risingEnabled << (uint32_t)1U) | ((uint32_t)fallingEnabled));

    return status;
}
/**
 *
 * Function Name : CMP_DRV_ClearOutputFlags
 * Description   : Clear comparator output flags(rising and falling edge on output).
 *
 */
status_t CMP_DRV_ClearOutputFlags(const uint32_t instance)
{
    status_t status = STATUS_SUCCESS;
    OS_ASSERT(instance < CMP_INSTANCE_COUNT);
    cmp_type_t *baseAddr = g_cmpBase[instance];

    uint32_t tmp = baseAddr->C0;
    tmp &= ~(CMP_C0_CRF_MASK);
    tmp |= CMP_C0_CRF(1);
    tmp &= ~(CMP_C0_CFF_MASK);
    tmp |= CMP_C0_CFF(1);
    baseAddr->C0 = tmp;

    return status;
}

/**
 *
 * Function Name : CMP_DRV_GetInputFlags
 * Description   : Return all input change flags in flags.
 * flags format : Flag_Ch7 Flag_Ch6 ... Flag_Ch0
 *
 */
status_t CMP_DRV_GetInputFlags(const uint32_t instance, cmp_ch_list_t *flags)
{
    status_t status = STATUS_SUCCESS;
    OS_ASSERT(flags != NULL);
    OS_ASSERT(instance < CMP_INSTANCE_COUNT);

    const cmp_type_t *baseAddr = g_cmpBase[instance];
    uint32_t tmp               = baseAddr->C2;
    *flags                     = (cmp_ch_list_t)((tmp & (uint32_t)CMP_C2_CHF_MASK) >> CMP_C2_CHF_SHIFT);

    return status;
}
/**
 *
 * Function Name : CMP_DRV_ClearInputFlags
 * Description   : Clear all input change flags .
 *
 */
status_t CMP_DRV_ClearInputFlags(const uint32_t instance)
{
    status_t status = STATUS_SUCCESS;
    OS_ASSERT(instance < CMP_INSTANCE_COUNT);

    cmp_type_t *baseAddr = g_cmpBase[instance];
    uint32_t tmp         = baseAddr->C2;
    tmp |= (uint32_t)CMP_C2_CHF_MASK;
    baseAddr->C2 = tmp;

    return status;
}

/**
 *
 * Function Name : CMP_DRV_ConfigComparator
 * Description   : Configure only comparator features (functional mode, power mode,
 * inverter, hysteresis, offset, filter sampling period and samples count).
 *
 */
status_t CMP_DRV_ConfigComparator(const uint32_t instance, const cmp_comparator_t *config)
{
    status_t status = STATUS_SUCCESS;
    OS_ASSERT(config != NULL);
    OS_ASSERT(instance < CMP_INSTANCE_COUNT);

    cmp_type_t *base = g_cmpBase[instance];
    CMP_SetDMATriggerState(base, config->dmaTriggerState);
    CMP_SetOutputInterruptTrigger(base, config->outputInterruptTrigger);
    CMP_SetFunctionalMode(base, config->mode, config->filterSampleCount, config->filterSamplePeriod);
    CMP_SetFilterSamplePeriod(base, config->filterSamplePeriod);
    CMP_SetFilterSampleCount(base, config->filterSampleCount);
    CMP_SetPowerMode(base, config->powerMode);
    CMP_SetInverterState(base, config->inverterState);
    CMP_SetComparatorOutputSource(base, config->outputSelect);
    CMP_SetOutputPinState(base, config->pinState);
    CMP_SetHysteresis(base, config->hysteresisLevel);

    return status;
}
/**
 *
 * Function Name : CMP_DRV_GetComparator
 * Description   : Return configuration for comparator components from CMP module.
 *
 */
status_t CMP_DRV_GetComparatorConfig(const uint32_t instance, cmp_comparator_t *config)
{
    status_t status = STATUS_SUCCESS;
    OS_ASSERT(config != NULL);
    OS_ASSERT(instance < CMP_INSTANCE_COUNT);

    const cmp_type_t *base         = g_cmpBase[instance];
    config->dmaTriggerState        = CMP_GetDMATriggerState(base);
    config->outputInterruptTrigger = CMP_GetOutputInterruptTrigger(base);
    config->mode                   = CMP_GetFunctionalMode(base);
    config->filterSampleCount      = CMP_GetFilterSampleCount(base);
    config->filterSamplePeriod     = CMP_GetFilterSamplePeriod(base);
    config->powerMode              = CMP_GetPowerMode(base);
    config->inverterState          = CMP_GetInverterState(base);
    config->outputSelect           = CMP_GetComparatorOutputSource(base);
    config->pinState               = CMP_GetOutputPinState(base);
    config->hysteresisLevel        = CMP_GetHysteresis(base);

    return status;
}

/*
 * Function Name : CMP_DRV_TcsrStopMode
 * Description   : Enter/exit stop mode according the TCSR
 *
 */
void CMP_DRV_SetSleepModeState(const uint32_t instance, bool enable)
{
    uint32_t intermVal          = 0;
    tcsr_type_t *const tcsrBase = TCSR;

    switch (instance) {
    case 0:
        intermVal = tcsrBase->STOP & TCSR_STOP_CMP0_MASK;
        intermVal |= TCSR_STOP_CMP0(enable);
        break;
    default:
        OS_ASSERT(false);
        break;
    }

    tcsrBase->STOP = intermVal;
}
