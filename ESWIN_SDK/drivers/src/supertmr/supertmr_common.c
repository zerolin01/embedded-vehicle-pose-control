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
 * @file supertmr_common.c
 * @brief SUPERTMR common driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "supertmr_common.h"
#include "supertmr_hw_access.h"

/**
 * @brief Table of base addresses for SUPERTMR instances.
 */
supertmr_type_t *const g_pstSupertmrBase[SUPERTMR_INSTANCE_COUNT] = SUPERTMR_BASE_PTRS;

/**
 * @brief Interrupt vectors for the SUPERTMR peripheral.
 */
const IRQn_Type g_euSupertmrIrqId[SUPERTMR_INSTANCE_COUNT][SUPERTMR_CHANNEL_COUNT] = SUPERTMR_IRQS;
const IRQn_Type g_euSupertmrFaultIrqId[SUPERTMR_INSTANCE_COUNT]                    = SUPERTMR_Fault_IRQS;
const IRQn_Type g_euSupertmrOverflowIrqId[SUPERTMR_INSTANCE_COUNT]                 = SUPERTMR_Overflow_IRQS;
const IRQn_Type g_euSupertmrReloadIrqId[SUPERTMR_INSTANCE_COUNT]                   = SUPERTMR_Reload_IRQS;

/**
 * @brief Pointer to runtime state structure.
 */
supertmr_state_t *supertmrStatePtr[SUPERTMR_INSTANCE_COUNT] = {NULL};

/**
 * @brief  Select external clock pin or clock source for peripheral
 */
static const clock_names_t g_supertmrExtClockSel[SUPERTMR_INSTANCE_COUNT] = {
    SUPERTMR0_CLK,
    SUPERTMR1_CLK,
    SUPERTMR2_CLK,
    SUPERTMR3_CLK,
#if (defined(PLATFORM_EAM2011))
    SUPERTMR4_CLK,
    SUPERTMR5_CLK,
#endif
};

/**
 * @brief 
 * 
 */
static const clock_names_t g_supertmrClockSel[SUPERTMR_INSTANCE_COUNT] = {
    SUPERTMR0_CLK,
    SUPERTMR1_CLK,
    SUPERTMR2_CLK,
    SUPERTMR3_CLK,
#if (defined(PLATFORM_EAM2011))
    SUPERTMR4_CLK,
    SUPERTMR5_CLK,
#endif
};

extern void SUPERTMR_Ch0_7_IRQHandler(void *data);

/** Flex Timer Instance interrupt service routine
 *  - it will clear the timer overflow flag of the SUPERTMR instance
 */
void SUPERTMR_Ovf_Reload_IRQHandler(void *data)
{
    uint32_t instance = (size_t)data;
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);

    supertmr_state_t *state = supertmrStatePtr[instance];
    if (NULL != state) {
        if (state->supertmrCallback) {
            supertmr_callback_t cb = state->supertmrCallback;
            cb(SUPERTMR_EVENT_TIMER_OVERFLOW, state->supertmrCallbackParams);
        }
    }

    SUPERTMR_DRV_ClearStatusFlags(instance, (uint32_t)SUPERTMR_TIME_OVER_FLOW_FLAG);
}

void SUPERTMR_Falut_IRQHandler(void *data)
{
    uint32_t instance = (size_t)data;
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);

    supertmr_state_t *state = supertmrStatePtr[instance];
    if (NULL != state) {
        if (state->supertmrCallback) {
            supertmr_callback_t cb = state->supertmrCallback;
            cb(SUPERTMR_EVENT_FAULT, state->supertmrCallbackParams);
        }
    }

    SUPERTMR_DRV_ClearStatusFlags(instance, (uint32_t)SUPERTMR_FAULT_FLAG);
}

void SUPERTMR_Reload_IRQHandler(void *data)
{
    uint32_t instance = (size_t)data;
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);

    supertmr_state_t *state = supertmrStatePtr[instance];
    if (NULL != state) {
        if (state->supertmrCallback) {
            supertmr_callback_t cb = state->supertmrCallback;
            cb(SUPERTMR_EVENT_RELOAD_POINT, state->supertmrCallbackParams);
        }
    }
    SUPERTMR_DRV_ClearStatusFlags(instance, (uint32_t)SUPERTMR_RELOAD_FLAG);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_Init
 * Description   : Initializes the SUPERTMR driver and get the clock frequency value
 * which select one of three possible clock sources for the SUPERTMR counter.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_Init(uint32_t instance, const supertmr_user_config_t *info, supertmr_state_t *state)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    OS_ASSERT(NULL != info);
    OS_ASSERT(NULL != state);
    OS_ASSERT((SUPERTMR_CLOCK_SOURCE_SYSTEMCLK == info->supertmrClockSource) ||
              (SUPERTMR_CLOCK_SOURCE_FIXEDCLK == info->supertmrClockSource) ||
              (SUPERTMR_CLOCK_SOURCE_EXTERNALCLK == info->supertmrClockSource));
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    status_t status               = STATUS_SUCCESS;
    uint8_t index                 = 0U;
    OS_RegisterType_t uType       = {0};

    CLOCK_SYS_ClockEnable(g_supertmrClockSel[instance], true);

    /* Check if this instance is already initialized */
    if (NULL != supertmrStatePtr[instance]) {
        status = STATUS_ERROR;
    } else {
        /* Configure state structure. */
        state->supertmrClockSource = info->supertmrClockSource;
        state->supertmrMode        = SUPERTMR_MODE_NOT_INITIALIZED;
        state->supertmrPeriod      = 0U;
        supertmrStatePtr[instance] = state;

        for (index = 0U; index < SUPERTMR_CHANNEL_COUNT; index++) {
            state->measurementResults[index]      = 0U;
            state->channelsCallbacksParams[index] = NULL;
            state->channelsCallbacks[index]       = NULL;
            state->enableNotification[index]      = false;
        }

        state->supertmrCallback       = info->callback;
        state->supertmrCallbackParams = info->cbParams;

        /* The reset operation doesn't care about write protection. SUPERTMR_DRV_Reset will
         * disable this protection.*/
        SUPERTMR_DRV_Reset(supertmrBase);
        SUPERTMR_DRV_InitModule(supertmrBase, info->supertmrPrescaler);
        /* Get clock name used to configure the SuperTimer module */
        state->supertmrSourceClockFrequency = SUPERTMR_DRV_GetFrequency(instance);
        /* Check the functional clock is selected for SUPERTMR */
        OS_ASSERT(state->supertmrSourceClockFrequency > 0U);
    }

    if (STATUS_SUCCESS == status) {
        /* Check if the mode operation in PWM mode */
        if ((SUPERTMR_MODE_EDGE_ALIGNED_PWM == info->supertmrMode) ||
            (SUPERTMR_MODE_CEN_ALIGNED_PWM == info->supertmrMode) ||
            (SUPERTMR_MODE_OUTPUT_COMPARE == info->supertmrMode) ||
            (SUPERTMR_MODE_EDGE_ALIGNED_PWM_AND_INPUT_CAPTURE == info->supertmrMode)) {

            /* Configure sync for between registers and buffers */
            status = SUPERTMR_DRV_SetSync(instance, &(info->syncMethod));
        }

        /* Enable the generation of initialization trigger on chip module */
        SUPERTMR_DRV_SetInitTriggerCmd(supertmrBase, info->enableInitializationTrigger);
        SUPERTMR_DRV_SetBdmMode(supertmrBase, info->BDMMode);

        /* Check if enable interrupt in counter mode */
        if (info->isTofIsrEnabled) {
            /* Enable IRQ for SUPERTMR OVERFLOW */
            uType.trig_mode = CLIC_LEVEL_TRIGGER;
            uType.lvl       = 2;
            uType.priority  = 1;
            uType.data_ptr  = (void *)instance;
            OS_RequestIrq(g_euSupertmrOverflowIrqId[instance], SUPERTMR_Ovf_Reload_IRQHandler, &uType);
            SUPERTMR_DRV_SetTimerOverflowInt(supertmrBase, true);
            OS_EnableIrq(g_euSupertmrOverflowIrqId[instance]);
        } else {
            SUPERTMR_DRV_SetTimerOverflowInt(supertmrBase, false);
            OS_DisableIrq(g_euSupertmrOverflowIrqId[instance]);
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_Deinit
 * Description   : Shuts down the SUPERTMR driver.
 * First, SUPERTMR_DRV_Init must be called. Then this function will disables the SUPERTMR module.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_Deinit(uint32_t instance)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];

    /* Reset all SUPERTMR register */
    SUPERTMR_DRV_Reset(supertmrBase);
    supertmrStatePtr[instance] = NULL;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_GetDefaultConfig
 * Description   : This function will get the default configuration values
 * in the structure which is used as a common use-case.
 *
 *END**************************************************************************/
void SUPERTMR_DRV_GetDefaultConfig(supertmr_user_config_t *const config)
{
    OS_ASSERT(NULL != config);

    config->syncMethod.softwareSync     = true;
    config->syncMethod.hardwareSync0    = false;
    config->syncMethod.hardwareSync1    = false;
    config->syncMethod.hardwareSync2    = false;
    config->syncMethod.maxLoadingPoint  = false;
    config->syncMethod.minLoadingPoint  = false;
    config->syncMethod.inverterSync     = SUPERTMR_SYSTEM_CLOCK;
    config->syncMethod.outRegSync       = SUPERTMR_SYSTEM_CLOCK;
    config->syncMethod.maskRegSync      = SUPERTMR_SYSTEM_CLOCK;
    config->syncMethod.initCounterSync  = SUPERTMR_SYSTEM_CLOCK;
    config->syncMethod.autoClearTrigger = true;
    config->syncMethod.syncPoint        = SUPERTMR_UPDATE_NOW;

    config->supertmrMode                = SUPERTMR_MODE_EDGE_ALIGNED_PWM;
    config->supertmrPrescaler           = SUPERTMR_CLOCK_DIVID_BY_1;
    config->supertmrClockSource         = SUPERTMR_CLOCK_SOURCE_SYSTEMCLK;
    config->BDMMode                     = SUPERTMR_BDM_MODE_11;
    config->isTofIsrEnabled             = false;
    config->enableInitializationTrigger = false;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_MaskOutputChannels
 * Description   : This function will mask the output of the channels and at match
 * events will be ignored by the masked channels.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_MaskOutputChannels(uint32_t instance, uint32_t channelsMask, bool softwareTrigger)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];

    SUPERTMR_DRV_SetOutmaskReg(supertmrBase, channelsMask);
    SUPERTMR_DRV_SetSoftwareTriggerCmd(supertmrBase, softwareTrigger);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_SetInitialCounterValue
 * Description   : This function configure the initial counter value. The counter
 * will get this value after an overflow event.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_SetInitialCounterValue(uint32_t instance, uint16_t counterValue, bool softwareTrigger)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];

    SUPERTMR_DRV_SetCounterInitVal(supertmrBase, counterValue);
    SUPERTMR_DRV_SetSoftwareTriggerCmd(supertmrBase, softwareTrigger);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_SetHalfCycleReloadPoint
 * Description   : This function configure the value of the counter which will
 * generates an reload point.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_SetHalfCycleReloadPoint(uint32_t instance, uint16_t reloadPoint, bool softwareTrigger)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];

    SUPERTMR_DRV_SetHalfCycleValue(supertmrBase, reloadPoint);
    SUPERTMR_DRV_SetSoftwareTriggerCmd(supertmrBase, softwareTrigger);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_SetSoftOutChnValue
 * Description   : This function will force the output value of a channel to a specific value.
 * Before using this function it's mandatory to mask the match events using
 * SUPERTMR_DRV_MaskOutputChannels and to enable software output control using
 * SUPERTMR_DRV_SetSoftwareOutputChannelControl.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_SetSoftOutChnValue(uint32_t instance, uint8_t channelsValues, bool softwareTrigger)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    SUPERTMR_DRV_SetAllChnSoftwareCtrlVal(supertmrBase, channelsValues ^ (uint8_t)supertmrBase->POL);
    SUPERTMR_DRV_SetSoftwareTriggerCmd(supertmrBase, softwareTrigger);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_SetSoftwareOutputChannelControl
 * Description   : This function will configure which output channel can be
 * software controlled.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_SetSoftwareOutputChannelControl(uint32_t instance, uint8_t channelsMask, bool softwareTrigger)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    SUPERTMR_DRV_SetAllChnSoftwareCtrlCmd(supertmrBase, channelsMask);
    SUPERTMR_DRV_SetSoftwareTriggerCmd(supertmrBase, softwareTrigger);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_SetAllChnSoftwareOutputControl
 * Description   : This function will control list of channels by software to force
 * the output to specified value.
 * Despite the odd channels are configured as HIGH/LOW, they will be inverted in the following
 * configuration: COMP bit = 1 and CH(n)OCV and CH(n+1)OCV are HIGH. Please check Software
 * output control behavior chapter from RM.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_SetAllChnSoftwareOutputControl(uint32_t instance, uint8_t channelMask, uint8_t channelValueMask,
                                                     bool softwareTrigger)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    uint16_t value                = 0U;

    value = (uint16_t)(((uint16_t)channelValueMask ^ (uint16_t)supertmrBase->POL) << (uint16_t)8U) | (uint16_t)channelMask;

    /* Enable and force the software control of channels output */
    SUPERTMR_DRV_SoftwareOutputControl(supertmrBase, value);
    SUPERTMR_DRV_SetSoftwareTriggerCmd(supertmrBase, softwareTrigger);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_SetInvertingControl
 * Description   : This function will configure if the second channel of a pair
 * will be inverted or not.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_SetInvertingControl(uint32_t instance, uint8_t channelsPairMask, bool softwareTrigger)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];

    SUPERTMR_DRV_SetInvctrlReg(supertmrBase, channelsPairMask);
    SUPERTMR_DRV_SetSoftwareTriggerCmd(supertmrBase, softwareTrigger);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_SetModuloCounterValue
 * Description   : This function configure the maximum counter value.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_SetModuloCounterValue(uint32_t instance, uint16_t counterValue, bool softwareTrigger)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];

    SUPERTMR_DRV_SetMod(supertmrBase, counterValue);
    SUPERTMR_DRV_SetSoftwareTriggerCmd(supertmrBase, softwareTrigger);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_SetOutputLevel
 * Description   : This function will set the channel edge or level on the selection
 * of the channel mode.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_SetOutputLevel(uint32_t instance, uint8_t channel, uint8_t level)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];

    /* Sets the channel edge or level selection */
    SUPERTMR_DRV_SetChnEdgeLevel(supertmrBase, channel, level);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_SetSync
 * Description   : This function configure the synchronization for PWM register
 * (CnV, MOD, CINT, HCR, OUTMASK).If this function is used whit wrong parameters
 * it's possible to generate wrong waveform. Registers synchronization need to
 * be configured for PWM and output compare mode.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_SetSync(uint32_t instance, const supertmr_pwm_sync_t *param)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    OS_ASSERT(NULL != param);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    status_t retStatus            = STATUS_SUCCESS;
    bool hardwareSync             = param->hardwareSync0 || param->hardwareSync1 || param->hardwareSync2;

    /* Software and hardware triggers are not allowed in the same time */
    if ((param->softwareSync && hardwareSync) || (true != (param->softwareSync || hardwareSync))) {
        retStatus = STATUS_ERROR;
    } else if (param->softwareSync) {
        /* Configure sync for OUTMASK register */
        SUPERTMR_DRV_SetOutmaskSoftwareSyncModeCmd(supertmrBase, true);
        /* Configure sync for INVCTRL register */
        SUPERTMR_DRV_SetInvctrlSoftwareSyncModeCmd(supertmrBase, true);
        /* Configure sync for SWOCTRL register */
        SUPERTMR_DRV_SetSwoctrlSoftwareSyncModeCmd(supertmrBase, true);
        /* Configure sync for MOD, HCR, CNTIN, and CnV registers */
        SUPERTMR_DRV_SetModCntinCvSoftwareSyncModeCmd(supertmrBase, true);
        /* Configure synchronization method (waiting next loading point or now) */
        SUPERTMR_DRV_SetCounterSoftwareSyncModeCmd(supertmrBase, param->syncPoint);
    } else {
        /* Configure sync for OUTMASK register */
        SUPERTMR_DRV_SetOutmaskHardwareSyncModeCmd(supertmrBase, true);
        /* Configure sync for INVCTRL register */
        SUPERTMR_DRV_SetInvctrlHardwareSyncModeCmd(supertmrBase, true);
        /* Configure sync for SWOCTRL register */
        SUPERTMR_DRV_SetSwoctrlHardwareSyncModeCmd(supertmrBase, true);
        /* Configure sync for MOD, HCR, CNTIN, and CnV registers */
        SUPERTMR_DRV_SetModCntinCvHardwareSyncModeCmd(supertmrBase, true);
        /* Configure synchronization method (waiting next loading point or now) */
        SUPERTMR_DRV_SetCounterHardwareSyncModeCmd(supertmrBase, (bool)param->syncPoint);
    }

    if (STATUS_SUCCESS == retStatus) {
        /* Enhanced PWM sync is used */
        SUPERTMR_DRV_SetPwmSyncModeCmd(supertmrBase, true);
        /* Configure trigger source for sync */
        SUPERTMR_DRV_SetHardwareSyncTriggerSrc(supertmrBase, 0U, param->hardwareSync0);
        SUPERTMR_DRV_SetHardwareSyncTriggerSrc(supertmrBase, 1U, param->hardwareSync1);
        SUPERTMR_DRV_SetHardwareSyncTriggerSrc(supertmrBase, 2U, param->hardwareSync2);
        /* Configure loading points */
        SUPERTMR_DRV_SetMaxLoadingCmd(supertmrBase, param->maxLoadingPoint);
        SUPERTMR_DRV_SetMinLoadingCmd(supertmrBase, param->minLoadingPoint);
        /* Configure sync for OUTMASK register */
        SUPERTMR_DRV_SetOutmaskPwmSyncModeCmd(supertmrBase, (bool)param->maskRegSync);
        /* Configure sync for INVCTRL register */
        SUPERTMR_DRV_SetInvctrlPwmSyncModeCmd(supertmrBase, param->inverterSync);
        /* Configure sync for SWOCTRL register */
        SUPERTMR_DRV_SetSwoctrlPwmSyncModeCmd(supertmrBase, param->outRegSync);
        /* Configure sync for MOD, HCR, CNTIN, and CnV registers */
        SUPERTMR_DRV_SetCntinPwmSyncModeCmd(supertmrBase, param->initCounterSync);
        /* Configure if SUPERTMR clears TRIGj (j=0,1,2) when the hardware trigger j is detected. */
        SUPERTMR_DRV_SetHwTriggerSyncModeCmd(supertmrBase, param->autoClearTrigger);
    }

    return retStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_GenerateHardwareTrigger
 * Description   : This function is used to configure a trigger source for SUPERTMR instance.
 * This allow a hardware trigger input which can be used in PWM synchronization.
 * Note that the hardware trigger is implemented only on trigger 1 for each instance.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_GenerateHardwareTrigger(uint32_t instance)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    tcsr_type_t *simeBase = TCSR_BASE_PTRS;

    SUPERTMR_DRV_SyncBit(simeBase, instance, true);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_FaultSelectSourceTrgmux
 * Description   : This function is used to configure a trigger source for SUPERTMRx fault.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_FaultSelectSourceTrgmux(uint32_t instance, bool flt0, bool flt1, bool flt2)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    tcsr_type_t *simeBase = TCSR_BASE_PTRS;
    uint32_t value        = simeBase->SUPERTMROPT0;

    /* Clear the corresponding bits*/
    uint32_t mask = 0b111;
    mask          = mask << (instance * 4);
    value         = value & (~mask);

    /* Set the set bits */
    uint32_t ftmxFltSel    = (flt0 | flt1 << 1 | flt2 << 2) << (instance * 4);
    value                  = value | ftmxFltSel;
    simeBase->SUPERTMROPT0 = value;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_EnableInterrupts
 * Description   : This function will enable the generation a list of interrupts.
 * It includes the SUPERTMR overflow interrupts, the reload point interrupt, the fault
 * interrupt and the channel (n) interrupt.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_EnableInterrupts(uint32_t instance, uint32_t interruptMask)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    OS_RegisterType_t uType       = {0};
    uint32_t chnlInts             = (interruptMask & 0x000000FFU);
    uint8_t channel               = 0U;

    uType.trig_mode = CLIC_LEVEL_TRIGGER;
    uType.lvl       = 2;
    uType.priority  = 1;
    uType.data_ptr  = (void *)instance;

    /* Enable the timer overflow interrupt */
    if ((interruptMask & (uint32_t)SUPERTMR_TIME_OVER_FLOW_INT_ENABLE) != 0x0U) {
        SUPERTMR_DRV_SetTimerOverflowInt(supertmrBase, true);
        OS_RequestIrq(g_euSupertmrOverflowIrqId[instance], SUPERTMR_Ovf_Reload_IRQHandler, &uType);
        OS_EnableIrq(g_euSupertmrOverflowIrqId[instance]);
    }

    /* Enable the fault interrupt */
    if ((interruptMask & (uint32_t)SUPERTMR_FAULT_INT_ENABLE) != 0x0U) {
        SUPERTMR_DRV_SetFaultInt(supertmrBase, true);
        OS_RequestIrq(g_euSupertmrFaultIrqId[instance], SUPERTMR_Falut_IRQHandler, &uType);
        OS_EnableIrq(g_euSupertmrFaultIrqId[instance]);
    }

    /* Enable the reload interrupt */
    if ((interruptMask & (uint32_t)SUPERTMR_RELOAD_INT_ENABLE) != 0x0U) {
        SUPERTMR_DRV_SetReIntEnabledCmd(supertmrBase, true);
        OS_RequestIrq(g_euSupertmrReloadIrqId[instance], SUPERTMR_Reload_IRQHandler, &uType);
        OS_EnableIrq(g_euSupertmrReloadIrqId[instance]);
    }

    /* Enable the channel interrupts */
    while (0U != chnlInts) {
        if (0x0U != (chnlInts & 0x1U)) {
            SUPERTMR_DRV_EnableChnInt(supertmrBase, channel);
            OS_RequestIrq(g_euSupertmrIrqId[instance][channel], SUPERTMR_Ch0_7_IRQHandler, &uType);
            OS_EnableIrq(g_euSupertmrIrqId[instance][channel]);
        }
        channel++;
        chnlInts = chnlInts >> 1U;
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_DisableInterrupts
 * Description   : This function is used to disable some interrupts.
 *
 *END**************************************************************************/
void SUPERTMR_DRV_DisableInterrupts(uint32_t instance, uint32_t interruptMask)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    uint32_t chnlInts             = (interruptMask & 0x000000FFU);
    uint8_t channel               = 0U;

    /* Disable the timer overflow interrupt */
    if (0x0U != (interruptMask & (uint32_t)SUPERTMR_TIME_OVER_FLOW_INT_ENABLE)) {
        SUPERTMR_DRV_SetTimerOverflowInt(supertmrBase, false);
        OS_DisableIrq(g_euSupertmrOverflowIrqId[instance]);
    }

    /* Disable the fault interrupt */
    if (0x0U != (interruptMask & (uint32_t)SUPERTMR_FAULT_INT_ENABLE)) {
        SUPERTMR_DRV_SetFaultInt(supertmrBase, false);
        OS_DisableIrq(g_euSupertmrFaultIrqId[instance]);
    }

    /* Disable the reload interrupt */
    if (0x0U != (interruptMask & (uint32_t)SUPERTMR_RELOAD_INT_ENABLE)) {
        SUPERTMR_DRV_SetReIntEnabledCmd(supertmrBase, false);
        OS_DisableIrq(g_euSupertmrReloadIrqId[instance]);
    }

    /* Disable the channel interrupts */
    while (0U != chnlInts) {
        if (0x0U != (chnlInts & 0x1U)) {
            SUPERTMR_DRV_DisableChnInt(supertmrBase, channel);
            OS_DisableIrq(g_euSupertmrIrqId[instance][channel]);
        }
        channel++;
        chnlInts = chnlInts >> 1U;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_GetEnabledInterrupts
 * Description   : This function will get the enabled SUPERTMR interrupts.
 *
 *END**************************************************************************/
uint32_t SUPERTMR_DRV_GetEnabledInterrupts(uint32_t instance)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    supertmr_type_t const *supertmrBase = g_pstSupertmrBase[instance];
    uint32_t enabledInterrupts          = 0U;
    uint8_t channel                     = SUPERTMR_CHANNEL_COUNT;

    /* Check if timer overflow interrupt is enabled */
    if (true == SUPERTMR_DRV_IsOverflowIntEnabled(supertmrBase)) {
        enabledInterrupts |= (uint32_t)SUPERTMR_TIME_OVER_FLOW_INT_ENABLE;
    }

    /* Check if fault interrupt is enabled */
    if (true == SUPERTMR_DRV_IsFaultIntEnabled(supertmrBase)) {
        enabledInterrupts |= (uint32_t)SUPERTMR_FAULT_INT_ENABLE;
    }

    /* Check if the reload interrupt is enabled */
    if (true == SUPERTMR_DRV_IsReloadIntEnabled(supertmrBase)) {
        enabledInterrupts |= (uint32_t)SUPERTMR_RELOAD_INT_ENABLE;
    }

    /* Check if the channel interrupts are enabled */
    while (channel > 0U) {
        channel--;
        if (true == SUPERTMR_DRV_IsChnIntEnabled(supertmrBase, channel)) {
            enabledInterrupts |= (1UL << (uint32_t)channel);
        }
    }

    return enabledInterrupts;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_GetStatusFlags
 * Description   : This function will get the SUPERTMR status flags.
 *
 *END**************************************************************************/
uint32_t SUPERTMR_DRV_GetStatusFlags(uint32_t instance)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    supertmr_type_t const *supertmrBase = g_pstSupertmrBase[instance];
    uint8_t channel                     = 0U;
    uint32_t statusFlags                = 0U;

    /* Check the timer flag */
    if (true == SUPERTMR_DRV_HasTimerOverflowed(supertmrBase)) {
        statusFlags |= (uint32_t)SUPERTMR_TIME_OVER_FLOW_FLAG;
    }

    /* Check fault flag */
    if (true == SUPERTMR_DRV_GetDetectedFaultInput(supertmrBase)) {
        statusFlags |= (uint32_t)SUPERTMR_FAULT_FLAG;
    }

    /* Check reload flag */
    if (true == SUPERTMR_DRV_GetReloadFlag(supertmrBase)) {
        statusFlags |= (uint32_t)SUPERTMR_RELOAD_FLAG;
    }

    /* Check channel trigger flag */
    if (true == SUPERTMR_DRV_IsChnTriggerGenerated(supertmrBase)) {
        statusFlags |= (uint32_t)SUPERTMR_CHANNEL_TRIGGER_FLAG;
    }

    /* Lower 8 bits contain the channel status flags */
    for (channel = 0U; channel < SUPERTMR_CHANNEL_COUNT; channel++) {
        if (true == SUPERTMR_DRV_HasChnEventOccurred(supertmrBase, channel)) {
            statusFlags |= (1UL << (uint32_t)channel);
        }
    }

    return statusFlags;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_ClearStatusFlags
 * Description   : This function is used to clear the SUPERTMR status flags.
 *
 *END**************************************************************************/
void SUPERTMR_DRV_ClearStatusFlags(uint32_t instance, uint32_t flagMask)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    uint32_t chnlMask             = (flagMask & 0x000000FFU);
    uint8_t channel               = 0U;

    /* Clear the timer overflow flag by writing a 0 to the bit while it is set */
    if ((flagMask & (uint32_t)SUPERTMR_TIME_OVER_FLOW_FLAG) != 0x0U) {
        SUPERTMR_DRV_ClearTimerOverflow(supertmrBase);
    }

    /* Clear fault flag by writing a 0 to the bit while it is set */
    if ((flagMask & (uint32_t)SUPERTMR_FAULT_FLAG) != 0x0U) {
        SUPERTMR_DRV_ClearFaultsIsr(supertmrBase);
    }

    /* Check reload flag by writing a 0 to the bit while it is set */
    if ((flagMask & (uint32_t)SUPERTMR_RELOAD_FLAG) != 0x0U) {
        SUPERTMR_DRV_ClearReloadFlag(supertmrBase);
    }

    /* Clear channel trigger flag */
    if ((flagMask & (uint32_t)SUPERTMR_CHANNEL_TRIGGER_FLAG) != 0x0U) {
        SUPERTMR_DRV_ClearChnTriggerFlag(supertmrBase);
    }

    /* Clear the channel status flags by writing a 0 to the bit */
    for (channel = 0U; channel < SUPERTMR_CHANNEL_COUNT; channel++) {
        if ((chnlMask & 0x00000001U) != 0x0U) {
            SUPERTMR_DRV_ClearChnEventStatus(supertmrBase, channel);
        }
        chnlMask = chnlMask >> 1U;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_GetFrequency
 * Description   : Retrieves the frequency of the clock source feeding the SUPERTMR counter.
 * Function will return a 0 if no clock source is selected and the SUPERTMR counter is disabled.
 * The returned value is clock sources for the SUPERTMR counter.
 *
 *END**************************************************************************/
uint32_t SUPERTMR_DRV_GetFrequency(uint32_t instance)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    supertmr_type_t const *supertmrBase = g_pstSupertmrBase[instance];
    status_t returnCode                 = STATUS_SUCCESS;
    uint8_t clkPs;
    uint32_t frequency            = 0U;
    const supertmr_state_t *state = supertmrStatePtr[instance];
    OS_ASSERT(state != NULL);
    clkPs = (uint8_t)(1U << SUPERTMR_DRV_GetClockPs(supertmrBase));

    switch (state->supertmrClockSource) {
    case SUPERTMR_CLOCK_SOURCE_EXTERNALCLK:
        returnCode = CLOCK_SYS_GetFreq(g_supertmrExtClockSel[instance], &frequency);
        break;
    case SUPERTMR_CLOCK_SOURCE_FIXEDCLK:
        /* Get the clock frequency value */
        // returnCode = CLOCK_SYS_GetFreq(SIM_RTCCLK_CLK, &frequency);
        returnCode = STATUS_UNSUPPORTED;
        break;
    case SUPERTMR_CLOCK_SOURCE_SYSTEMCLK:
        /* Get the clock frequency value */
        returnCode = CLOCK_SYS_GetFreq(MAIN_BUS_MOTOR_CTRL_CLK, &frequency);
        break;
    default:
        /* Nothing to do */
        break;
    }

    /* Checks the functional clock of SUPERTMR module */
    (void)returnCode;
    OS_ASSERT(returnCode == STATUS_SUCCESS);

    return (uint32_t)(frequency / clkPs);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_ConvertFreqToPeriodTicks
 * Description   : This function converts the input parameters representing
 * frequency in Hz to a period value in ticks needed by the hardware timer.
 *
 *END**************************************************************************/
uint16_t SUPERTMR_DRV_ConvertFreqToPeriodTicks(uint32_t instance, uint32_t frequencyHz)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    OS_ASSERT(frequencyHz != 0U);
    uint32_t uSUPERTMRhz;
    const supertmr_state_t *state = supertmrStatePtr[instance];
    uSUPERTMRhz                   = state->supertmrSourceClockFrequency;

    return (uint16_t)(uSUPERTMRhz / frequencyHz);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_CounterReset
 * Description   : This function will allow the SUPERTMR to restart the counter to
 * its initial counting value in the register.
 * Note that the configuration is set in the SUPERTMR_DRV_SetSync() function to make
 * sure that the SUPERTMR registers are updated by software trigger or hardware trigger.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_CounterReset(uint32_t instance, bool softwareTrigger)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];

    /* Updates the counter with its initial value */
    SUPERTMR_DRV_SetCounter(supertmrBase, 0U);
    /* Set a software trigger or waiting a hardware trigger */
    SUPERTMR_DRV_SetSoftwareTriggerCmd(supertmrBase, softwareTrigger);

    return STATUS_SUCCESS;
}
