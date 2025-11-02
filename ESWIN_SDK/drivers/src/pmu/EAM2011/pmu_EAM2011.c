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
 * @file pmu_EAM2011.c
 * @brief PMU EAM2011 driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "emps_platform.h"
#include "pmu_hw_access.h"
#include "pmu_register.h"
#include "power_manager.h"
#include <stddef.h>

#include "../clock/EAM2011/clock_EAM2011.h"
#include "../clock/EAM2011/crg_hw_access.h"
#include "../clock/EAM2011/crg_register.h"

static const IRQn_Type s_pmuPwrupIrqId      = PMU_PWRUP_IRQS;
static const IRQn_Type s_pmuBorIrqId        = PMU_BOR_IRQS;
static const IRQn_Type s_pmuPvdIrqId        = PMU_PVD_IRQS;
static const IRQn_Type s_pmuRebootFailIrqId = PMU_REBOOT_FAIL_IRQS;

/**
 * @brief Power manager internal structure.
 */
power_manager_state_t gPowerManagerState;

/*
 *
 * Function Name : PMU_SYS_BOR_IRQHandler
 * Description   : PMU BOR IRQ handler.
 *
 */
void PMU_SYS_BOR_IRQHandler(void *data)
{
    uint32_t intrStatus     = 0;
    pmu_bor_config_t *state = (pmu_bor_config_t *)data;
    if (state != NULL) {
        intrStatus = PMU_GetInterruptStatus(PMU);
        if (intrStatus & PMU_INTR_STATUS_BOR_MASK) {
            PMU_ClearBorIntStateFlag(PMU);
        }
        if (state->callback != NULL) {
            state->callback(state->callbackParam, intrStatus);
        }
    }
}

/*
 *
 * Function Name : PMU_SYS_PVD_IRQHandler
 * Description   : PMU PVD IRQ handler.
 *
 */
void PMU_SYS_PVD_IRQHandler(void *data)
{
    uint32_t intrStatus     = 0;
    pmu_pvd_config_t *state = (pmu_pvd_config_t *)data;
    if (state != NULL) {
        intrStatus = PMU_GetInterruptStatus(PMU);
        if (intrStatus & PMU_INTR_STATUS_PVD_MASK) {
            PMU_ClearPvdIntStateFlag(PMU);
        }
        if (state->callback != NULL) {
            state->callback(state->callbackParam, intrStatus);
        }
    }
}

/*
 *
 * Function Name : PMU_SYS_StandBy_Wakeup_IRQHandler
 * Description   : PMU StandBy Wakeup handler.
 *
 */
void PMU_SYS_StandBy_Wakeup_IRQHandler(void *data)
{
    uint32_t intrStatus         = 0;
    pmu_standby_config_t *state = (pmu_standby_config_t *)data;
    if (state != NULL) {
        intrStatus = PMU_GetInterruptStatus(PMU);
        if (intrStatus & PMU_INTR_STATUS_WAKEUP_MASK) {
            PMU_ClearPowerUpIntStateFlag(PMU);
        }
        if (state->callback != NULL) {
            state->callback(state->callbackParam, intrStatus);
        }
    }
}

/*
 *
 * Function Name : PMU_SYS_RebootFail_IRQHandler
 * Description   : PMU Reboot fail handler.
 *
 */
void PMU_SYS_RebootFail_IRQHandler(void *data)
{
    uint32_t intrStatus        = 0;
    pmu_reboot_config_t *state = (pmu_reboot_config_t *)data;
    if (state != NULL) {
        intrStatus = PMU_GetRebootFailInterruptStatus(PMU);
        if (intrStatus & PMU_REBOOT_FAIL_IRQ_MASK_MASK) {
            PMU_ClearRebootFailInterrupt(PMU);
        }
        if (state->callback != NULL) {
            state->callback(state->callbackParam, intrStatus);
        }
    }
}

/*
 *
 * Function Name : PMU_SYS_SetBorConfiguration
 * Description   : Configuration the PMU Bor.
 *
 */
status_t PMU_SYS_SetBorConfiguration(const pmu_bor_config_t *borConfig)
{
    status_t status  = STATUS_SUCCESS;
    uint32_t intMask = borConfig->interruptEnable ? (uint32_t)0U : (uint32_t)1U;

    PMU_SetBorState(PMU, false);
    PMU_SetBorRsntSource(PMU, borConfig->rstnSource);
    PMU_SetBorDetectionVol(PMU, borConfig->detectionVol);
    PMU_SetInterruptMask(PMU, intMask << PMU_INTR_MASK_BOR_SHIFT);
    if (borConfig->interruptEnable) {
        register_type_t type;
        type.lvl       = 1;
        type.priority  = 0;
        type.data_ptr  = (void *)borConfig;
        type.trig_mode = CLIC_LEVEL_TRIGGER;
        OS_RequestIrq(s_pmuBorIrqId, PMU_SYS_BOR_IRQHandler, &type);
        OS_EnableIrq(s_pmuBorIrqId);
    } else {
        OS_DisableIrq(s_pmuBorIrqId);
    }

    PMU_SetBorState(PMU, borConfig->enable);

    return status;
}

/*
 *
 * Function Name : PMU_SYS_SetPvdConfiguration
 * Description   : Configuration the PMU PVD.
 *
 */
status_t PMU_SYS_SetPvdConfiguration(const pmu_pvd_config_t *pvdConfig)
{
    status_t status  = STATUS_SUCCESS;
    uint32_t intMask = pvdConfig->interruptEnable ? (uint32_t)0U : (uint32_t)1U;

    PMU_SetPvdState(PMU, false);
    PMU_SetPvdDetectionVol(PMU, pvdConfig->detectionVol);
    PMU_SetInterruptMask(PMU, intMask << PMU_INTR_MASK_PVD_SHIFT);
    if (pvdConfig->interruptEnable) {
        register_type_t type;
        type.lvl       = 1;
        type.priority  = 0;
        type.data_ptr  = (void *)pvdConfig;
        type.trig_mode = CLIC_LEVEL_TRIGGER;
        OS_RequestIrq(s_pmuPvdIrqId, PMU_SYS_PVD_IRQHandler, &type);
    } else {
        OS_DisableIrq(s_pmuPvdIrqId);
    }

    PMU_SetPvdState(PMU, pvdConfig->enable);

    return status;
}

/*
 *
 * Function Name : PMU_SYS_SetMbgpConfiguration
 * Description   : Configuration the PMU MBGP.
 *
 */
status_t PMU_SYS_SetMbgpConfiguration(const pmu_mbgp_config_t *mbgpConfig)
{
    status_t status = STATUS_SUCCESS;

    PMU_SetMbgpReferenceVol(PMU, mbgpConfig->referenceVol);

    return status;
}

/*
 *
 * Function Name : PMU_SYS_SetLpbgpConfiguration
 * Description   : Configuration the PMU LPBGP.
 *
 */
status_t PMU_SYS_SetLpbgpConfiguration(const pmu_lpbgp_config_t *lpbgpConfig)
{
    status_t status = STATUS_SUCCESS;

    PMU_SetLpbgpReferenceVol(PMU, lpbgpConfig->referenceVol);

    return status;
}

/*
 *
 * Function Name : PMU_SYS_SetLdocConfiguration
 * Description   : Configuration the PMU LDO Core.
 *
 */
status_t PMU_SYS_SetLdocConfiguration(const pmu_ldoc_config_t *ldocConfig)
{
    status_t status = STATUS_SUCCESS;

    PMU_SetLdocReferenceVol(PMU, ldocConfig->v11CoreVol);
    PMU_SetVerf10OutputVol(PMU, ldocConfig->vref10AVol);
    PMU_SetBufferVerf10AState(PMU, ldocConfig->enableVref10A);
    PMU_SetBufferVerf12AState(PMU, ldocConfig->enableVref12A);
    PMU_SetLdocCurrentLimitState(PMU, ldocConfig->currentLimitEnable);

    return status;
}

/*
 *
 * Function Name : PMU_SYS_SetLdofConfiguration
 * Description   : Configuration the PMU LDO Flash.
 *
 */
status_t PMU_SYS_SetLdofConfiguration(const pmu_ldof_config_t *ldofConfig)
{
    status_t status = STATUS_SUCCESS;

    PMU_SetLdofCoreOutputVol(PMU, ldofConfig->v33FlashVol);
    PMU_SetLdofCurrentLimitState(PMU, ldofConfig->currentLimitEnable);

    return status;
}

/*
 *
 * Function Name : PMU_SYS_SetFsmConfiguration
 * Description   : Configuration the PMU FSM.
 *
 */
status_t PMU_SYS_SetFsmConfiguration(const pmu_fsm_config_t *fsmConfig)
{
    status_t status = STATUS_SUCCESS;

    PMU_SetFsmCountVal(PMU, fsmConfig->fsmCountVal);
    PMU_SetFsmTimeoutCntOverride(PMU, fsmConfig->fsmCntValOverride);
    PMU_SetFsmTimeoutCountingState(PMU, fsmConfig->fsmCountingEnable);

    return status;
}

/*
 *
 * Function Name : PMU_SYS_SetSleepModeConfiguration
 * Description   : Configuration the PMU FSM.
 *
 */
status_t PMU_SYS_SetSleepModeConfiguration(const pmu_sleep_config_t *sleepConfig)
{
    status_t status = STATUS_SUCCESS;
    PMU_SetSleepModeSource(PMU, sleepConfig->source);
    return status;
}

/*
 *
 * Function Name : PMU_SYS_SetStandbyConfiguration
 * Description   : Configuration the PMU FSM.
 *
 */
status_t PMU_SYS_SetStandbyConfiguration(const pmu_standby_config_t *standbyConfig)
{
    status_t status = STATUS_SUCCESS;

    PMU_DisableWakeupSource(PMU, PMU_WAKEUP_DISABLE_MASK);
    PMU_SetWakeupSource(PMU, standbyConfig->wakeupSource);
    PMU_SetInterruptMask(PMU, 0 << PMU_INTR_MASK_WAKEUP_SHIFT);

    register_type_t type;
    type.lvl       = 1;
    type.priority  = 0;
    type.data_ptr  = (void *)standbyConfig;
    type.trig_mode = CLIC_LEVEL_TRIGGER;
    OS_RequestIrq(s_pmuPwrupIrqId, PMU_SYS_StandBy_Wakeup_IRQHandler, &type);
    OS_EnableIrq(s_pmuPwrupIrqId);

    return status;
}

/*
 *
 * Function Name : PMU_SYS_SetRebootConfiguration
 * Description   : Configuration the PMU Reboot param.
 *
 */
status_t PMU_SYS_SetRebootConfiguration(const pmu_reboot_config_t *rebootConfig)
{
    status_t status  = STATUS_SUCCESS;
    uint32_t intMask = rebootConfig->interruptEnable ? (uint32_t)0U : (uint32_t)1U;

    PMU_SetRebootCounterState(PMU, rebootConfig->rebootCntEnable);
    PMU_SetRebootFailInterruptMask(PMU, intMask << PMU_REBOOT_FAIL_IRQ_MASK_SHIFT);
    if (rebootConfig->interruptEnable) {
        register_type_t type;
        type.lvl       = 1;
        type.priority  = 0;
        type.data_ptr  = (void *)rebootConfig;
        type.trig_mode = CLIC_LEVEL_TRIGGER;
        OS_RequestIrq(s_pmuRebootFailIrqId, PMU_SYS_RebootFail_IRQHandler, &type);
        OS_EnableIrq(s_pmuRebootFailIrqId);
    } else {
        OS_DisableIrq(s_pmuRebootFailIrqId);
    }

    return status;
}

/*
 *
 * Function Name : POWER_SYS_SwitchToSleepMode
 * Description   : Configuration the PMU from normal mode to sleep mode.
 *
 */
__TIM_CODE status_t POWER_SYS_SwitchToSleepMode(const power_manager_user_config_t *const config)
{
    /* 1. Switch root clock from PLL output to slow OSC */
    clock_names_t clkName;

    status_t status = CLOCK_SYS_GetParent(WORK_CLK, &clkName);
    if (status != STATUS_SUCCESS) {
        return status;
    }
    if (clkName != SROSC_CLK) {
        CLOCK_SYS_SetParent(WORK_CLK, SROSC_CLK);
    }

    /* 2. Disable PLL, FAST OSC */
    uint32_t spllPdStatus = CRG_GetSpllPowerCtrl(CRG);
    CRG_SetSpllPowerCtrl(CRG, CRG_SPLL_PD_MASK);
    uint32_t froscStatus = CRG_GetFrcoscEnableState(CRG);
    if (froscStatus) {
        CRG_SetFrcoscEnableState(CRG, false);
    }
    /* 3. Disable clock gating */
    /* 4. Configure sleep mode */

    if (PMU_GetSleepModeSource(PMU) == PMU_SLEEP_MODE_SW_TRG) {
        PMU_TriggerSleepMode(PMU);
    } else {
        __asm volatile("wfi");
    }

    if (froscStatus) {
        CRG_SetFrcoscEnableState(CRG, froscStatus);
    }
    if (spllPdStatus & (1 << CRG_SPLL_PD_BIASFUNC)) {
        CRG_SetSpllPowerCtrl(CRG, CRG_SPLL_PD_MASK & (~(1 << CRG_SPLL_PD_BIASFUNC)));
    }
    CRG_SetSpllPowerCtrl(CRG, spllPdStatus);

    uint32_t timeout = SPLL_STABILIZATION_TIMEOUT;
    while ((CRG_GetSpllLocked(CRG) == false) && (timeout > 0U)) {
        timeout--;
    }

    if (clkName != SROSC_CLK) {
        CLOCK_SYS_SetParent(WORK_CLK, clkName);
    }

    return status;
}

/*
 *
 * Function Name : POWER_SYS_SwitchToStandbyMode
 * Description   : Configuration the PMU from normal mode to standby mode.
 *
 */
static status_t POWER_SYS_SwitchToStandbyMode(const power_manager_user_config_t *const config)
{
    PMU_TriggerStandbyMode(PMU);

    return STATUS_SUCCESS;
}

/*
 *
 * Function Name : POWER_SYS_Doinit
 * Description   : This function performs the actual implementation-specific initialization.
 *
 *
 */
status_t POWER_SYS_DoInit(void)
{
    return STATUS_SUCCESS;
}

/*
 *
 * Function Name : POWER_SYS_DoDeinit
 * Description   : This function performs the actual implementation-specific de-initialization.
 *
 *
 */
status_t POWER_SYS_DoDeinit(void)
{
    return STATUS_SUCCESS;
}

/*
 *
 * Function Name : POWER_SYS_DoSetMode
 * Description   : Configuration the PMU mode (sleep or standby)
 *
 */
__TIM_CODE status_t POWER_SYS_DoSetMode(const power_manager_user_config_t *const config)
{
    if (config->powerMode == POWER_MANAGER_SLEEP) {
        POWER_SYS_SwitchToSleepMode(config);
    } else if (config->powerMode == POWER_MANAGER_STANDBY) {
        POWER_SYS_SwitchToStandbyMode(config);
    }

    return STATUS_SUCCESS;
}

#if 0
/*
 *
 * Function Name : PMU_SYS_Init
 * Description   : Initializes the PMU module.
 *
 */
status_t PMU_SYS_Init(pmu_user_config_t const * config, pmu_state_t * state)
{
    IRQn_Type irqNumber = NotAvail_IRQn;
    register_type_t type;

    if (config == NULL || state == NULL) {
        return STATUS_ERROR;
    }

	type.lvl = 1;
	type.priority = 0;
	type.data_ptr = state;
    type.trig_mode = CLIC_LEVEL_TRIGGER;
    irqNumber = s_pmuPwrupIrqId;
    OS_RequestIrq(irqNumber, PMU_SYS_IRQHandler, &type);
    irqNumber = s_pmuBorIrqId;
    OS_RequestIrq(irqNumber, PMU_SYS_IRQHandler, &type);
    irqNumber = s_pmuPvdIrqId;
    OS_RequestIrq(irqNumber, PMU_SYS_IRQHandler, &type);
    irqNumber = s_pmuRebootFailIrqId;
    OS_RequestIrq(irqNumber, PMU_SYS_IRQHandler, &type);


    PMU_SYS_SetMbgpConfiguration(&config->bgpConfig.mbgpConfig);
    PMU_SYS_SetLpbgpConfiguration(&config->bgpConfig.lpbgpConfig);
    PMU_SYS_SetLdocConfiguration(&config->ldoConfig.ldocConfig);
    PMU_SYS_SetLdofConfiguration(&config->ldoConfig.ldofConfig);
    PMU_SYS_SetFsmConfiguration(&config->fsmConfig);
    PMU_SYS_SetSleepModeConfiguration(&config->modeConfig.sleepConfig);
    PMU_SYS_SetStandbyConfiguration(&config->modeConfig.standbyConfig);
    PMU_SYS_SetCsrConfiguration(&config->csrConfig);
    PMU_SYS_SetBorConfiguration(&config->voldetConfig.borConfig);
    PMU_SYS_SetPvdConfiguration(&config->voldetConfig.pvdConfig);

    return STATUS_SUCCESS;
}
#endif

/*
 *
 * Function Name : PMU_SYS_SetBorDetectorVol
 * Description   : Configuration PMU BOR voltage
 *
 */
status_t PMU_SYS_SetBorDetectorVol(const uint32_t voltage)
{
    if (voltage > PMU_BOR_LV(PMU_BOR_LV_MASK)) {
        return STATUS_ERRORPAR;
    }

    PMU_SetBorDetectionVol(PMU, voltage);

    return STATUS_SUCCESS;
}

/*
 *
 * Function Name : PMU_SYS_SetBorRsntSourceState
 * Description   : Enable or disable RSNT_BOR as a reset source
 *
 */
status_t PMU_SYS_SetBorRsntSourceState(const bool state)
{
    PMU_SetBorRsntSource(PMU, state);

    return STATUS_SUCCESS;
}

/*
 *
 * Function Name : PMU_SYS_SetPvdDetectorVol
 * Description   : Configuration PMU PVD voltage
 *
 */
status_t PMU_SYS_SetPvdDetectorVol(const uint32_t voltage)
{
    if (voltage > PMU_PVD_LV(PMU_PVD_LV_MASK)) {
        return STATUS_ERRORPAR;
    }

    PMU_SetPvdDetectionVol(PMU, voltage);

    return STATUS_SUCCESS;
}

/*
 *
 * Function Name : PMU_SYS_SetWakeupSource
 * Description   : Enable the wakeup sourced for STANDBY mode
 *
 */
status_t PMU_SYS_SetSleepModeTriggerSource(const pmu_sleep_mode_trigger_t source)
{
    PMU_SetSleepModeSource(PMU, source);

    return STATUS_SUCCESS;
}

/*
 *
 * Function Name : PMU_SYS_TriggerSleepMode
 * Description   : Trigger the chip run into Sleep mode.
 *
 */
status_t PMU_SYS_TriggerSleepMode(void)
{
    PMU_TriggerSleepMode(PMU);

    return STATUS_SUCCESS;
}

/*
 *
 * Function Name : PMU_SYS_SetWakeupSource
 * Description   : Enable the wakeup sourced for STANDBY mode
 *
 */
status_t PMU_SYS_SetWakeupSource(const uint32_t sources)
{
    PMU_SetWakeupSource(PMU, sources);

    return STATUS_SUCCESS;
}

/*
 *
 * Function Name : PMU_SYS_DisableWakeupSource
 * Description   : Disable the wakeup source for STANDBY mode
 *
 */
status_t PMU_SYS_DisableWakeupSource(const uint32_t sources)
{
    PMU_DisableWakeupSource(PMU, sources);

    return STATUS_SUCCESS;
}

/*
 *
 * Function Name : PMU_SYS_TriggerStandbyMode
 * Description   : Trigger the chip run into STANDBY mode.
 *
 */
status_t PMU_SYS_TriggerStandbyMode(void)
{
    PMU_TriggerStandbyMode(PMU);

    return STATUS_SUCCESS;
}

/*
 *
 * Function Name : PMU_SYS_GetStandbyCount
 * Description   : Get STANDBY mode counter
 *
 */
status_t PMU_SYS_GetStandbyCount(uint32_t *count)
{
    if (count == NULL) {
        return STATUS_ERRORPAR;
    }
    *count = PMU_GetStandbyCount(PMU);

    return STATUS_SUCCESS;
}

/*
 *
 * Function Name : PMU_SYS_TriggerSoftwareReset
 * Description   : Trigger software reset
 *
 */
status_t PMU_SYS_TriggerSoftwareReset(void)
{
    PMU_TriggerSoftwareReset(PMU, true);

    return STATUS_SUCCESS;
}