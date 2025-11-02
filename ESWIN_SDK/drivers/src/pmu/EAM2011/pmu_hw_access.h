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
 * @file pmu_hw_access.h
 * @brief PMU driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __PMU_HW_ACCESS_H__
#define __PMU_HW_ACCESS_H__

#include <stdbool.h>
#include <stdint.h>

#include "pmu_EAM2011.h"
#include "pmu_register.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Set UPD Trigger
 *
 * This function configures UPD Trigger.
 *
 * @param baseAddr pmu base pointer
 */
static inline void PMU_SetWrUpd(pmu_type_t *const baseAddr)
{
    baseAddr->REG_WR_UPD = 0x1;
}

/**
 * @brief Sets the Brown-out rest enable state
 *
 * This function configures the Brown-out rest enable state.
 *
 * @param baseAddr pmu base pointer
 * @param state the new Brown-out rest state
 */
static inline void PMU_SetLbistClampState(pmu_type_t *const baseAddr, const bool state)
{
    uint32_t tmp = (uint32_t)baseAddr->SW_LBIST_CLAMP_EN;
    /* Clear the affected bitfield and write '0' to the w1c bits to avoid side-effects */
    tmp &= ~(PMU_SW_LBIST_CLAMP_EN_MASK);
    tmp |= PMU_SW_LBIST_CLAMP_EN(state ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->SW_LBIST_CLAMP_EN = (uint32_t)tmp;

    PMU_SetWrUpd(baseAddr);
}

/**
 * @brief Gets the Brown-out rest enable state
 *
 * This functions returns the enable state of the Brown-out
 *
 * @param baseAddr pmu base pointer
 * @return the Brown-out state
 */
static inline bool PMU_GetBorState(const pmu_type_t *const baseAddr)
{
    uint32_t tmp = (uint32_t)baseAddr->REG_BOR_EN_LV;
    tmp          = (tmp & PMU_BOR_EN_LV_MASK) >> PMU_BOR_EN_LV_SHIFT;

    return (tmp != 0U) ? true : false;
}

/**
 * @brief Sets the Brown-out rest enable state
 *
 * This function configures the Brown-out rest enable state.
 *
 * @param baseAddr pmu base pointer
 * @param state the new Brown-out rest state
 */
static inline void PMU_SetBorState(pmu_type_t *const baseAddr, const bool state)
{
    uint32_t tmp = (uint32_t)baseAddr->REG_BOR_EN_LV;
    /* Clear the affected bitfield and write '0' to the w1c bits to avoid side-effects */
    tmp &= ~(PMU_BOR_EN_LV_MASK);
    tmp |= PMU_BOR_EN_LV(state ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->REG_BOR_EN_LV = (uint32_t)tmp;

    PMU_SetWrUpd(baseAddr);
}

/**
 * @brief Gets the BOR Detection Voltage
 *
 * This functions returns the BOR Detection Voltage
 *
 * @param baseAddr pmu base pointer
 * @return the PVD Detection Voltage
 */
static inline float PMU_GetBorDetectionVol(const pmu_type_t *const baseAddr)
{
    uint32_t tmp = (uint32_t)baseAddr->REG_BOR_LV;
    tmp          = (tmp & PMU_BOR_LV_MASK) >> PMU_BOR_LV_SHIFT;

    return PMU_BOR_MIN_DETECTION_VOL + PMU_BOR_VOL_TRIMMING_STEP * tmp;
}

/**
 * @brief Configure Brown-out rest detection voltage
 *
 * This function configures the Brown-out rest detection voltage.
 *
 * @param baseAddr pmu base pointer
 * @param voltage the new detection voltage.
 *
 */
static inline void PMU_SetBorDetectionVol(pmu_type_t *const baseAddr, uint8_t voltage)
{
    uint32_t tmp = baseAddr->REG_BOR_LV;
    /* Clear the affected bitfield */
    tmp &= ~(PMU_BOR_LV_MASK);
    tmp |= PMU_BOR_LV(voltage);
    baseAddr->REG_BOR_LV = tmp;

    PMU_SetWrUpd(baseAddr);
}

/**
 * @brief Clean the timeout interrupt after enabling BOR.
 *
 * @param baseAddr pmu base pointer
 *
 */
static inline void PMU_ClearBorIntStateFlag(pmu_type_t *const baseAddr)
{
    uint32_t tmp = baseAddr->BOR_INT_CLEAN;
    /* Clear the affected bitfield */
    tmp &= ~(PMU_BOR_INT_CLEAN_MASK);
    tmp |= PMU_BOR_INT_CLEAN(1);
    baseAddr->BOR_INT_CLEAN = tmp;
}

/**
 * @brief Gets the Programmable Voltage Detector enable state
 *
 * This functions returns the enable state of the Programmable Voltage Detector
 *
 * @param baseAddr pmu base pointer
 * @return the Programmable Voltage Detector state
 */
static inline bool PMU_GetPvdState(const pmu_type_t *const baseAddr)
{
    uint32_t tmp = (uint32_t)baseAddr->REG_PVD_EN_LV;
    tmp          = (tmp & PMU_PVD_EN_LV_MASK) >> PMU_PVD_EN_LV_SHIFT;

    return (tmp != 0U) ? true : false;
}

/**
 * @brief Sets the Programmable Voltage Detector enable state
 *
 * This function configures the Programmable Voltage Detector enable state.
 *
 * @param baseAddr pmu base pointer
 * @param state the new Programmable Voltage Detector state
 */
static inline void PMU_SetPvdState(pmu_type_t *const baseAddr, const bool state)
{
    uint32_t tmp = (uint32_t)baseAddr->REG_PVD_EN_LV;
    /* Clear the affected bitfield and write '0' to the w1c bits to avoid side-effects */
    tmp &= ~(PMU_PVD_EN_LV_MASK);
    tmp |= PMU_PVD_EN_LV(state ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->REG_PVD_EN_LV = (uint32_t)tmp;

    PMU_SetWrUpd(baseAddr);
}

/**
 * @brief Gets the PVD Detection Voltage
 *
 * This functions returns the PVD Detection Voltage
 *
 * @param baseAddr pmu base pointer
 * @return the PVD Detection Voltage
 */
static inline float PMU_GetPvdDetectionVol(const pmu_type_t *const baseAddr)
{
    uint32_t tmp = (uint32_t)baseAddr->REG_PVD_LV;
    tmp          = (tmp & PMU_PVD_LV_MASK) >> PMU_PVD_LV_SHIFT;

    return PMU_PVD_MIN_DETECTION_VOL + PMU_PVD_VOL_TRIMMING_STEP * tmp;
}

/**
 * @brief Configure Programmable Voltage Detector detection voltage
 *
 * This function configures Programmable Voltage Detector detection voltage.
 *
 * @param baseAddr pmu base pointer
 * @param voltage the new detection voltage.
 *
 */
static inline void PMU_SetPvdDetectionVol(pmu_type_t *const baseAddr, uint8_t voltage)
{
    uint32_t tmp = baseAddr->REG_PVD_LV;
    /* Clear the affected bitfield */
    tmp &= ~(PMU_PVD_LV_MASK);
    tmp |= PMU_PVD_LV(voltage);
    baseAddr->REG_PVD_LV = tmp;

    PMU_SetWrUpd(baseAddr);
}

/**
 * @brief Clean the timeout interrupt after enabling PVD.
 *
 * @param baseAddr pmu base pointer
 *
 */
static inline void PMU_ClearPvdIntStateFlag(pmu_type_t *const baseAddr)
{
    uint32_t tmp = baseAddr->POR_INT_CLEAN;
    /* Clear the affected bitfield */
    tmp &= ~(PMU_POR_INT_CLEAN_MASK);
    tmp |= PMU_POR_INT_CLEAN(1);
    baseAddr->POR_INT_CLEAN = tmp;
}

/**
 * @brief Clean PMU powerup interrupt.
 *
 * This function clean PMU interrupts.
 * It will be used at the wakeup from the standby mode.
 *
 * @param baseAddr pmu base pointer
 *
 */
static inline void PMU_ClearPowerUpIntStateFlag(pmu_type_t *const baseAddr)
{
    uint32_t tmp = baseAddr->POWERUP_INT_CLEAN;
    /* Clear the affected bitfield */
    tmp &= ~(PMU_POWERUP_INT_CLEAN_MASK);
    tmp |= PMU_POWERUP_INT_CLEAN(1);
    baseAddr->POWERUP_INT_CLEAN = tmp;
}

/**
 * @brief Sets the VREF10A buffering state
 *
 * This function configures the VREF10A buffering state.
 *
 * @param baseAddr pmu base pointer
 * @param state enable
 */
static inline void PMU_SetBufferVerf10AState(pmu_type_t *const baseAddr, const bool state)
{
    uint32_t tmp = (uint32_t)baseAddr->REG_BUF_EN_LV;
    /* Clear the affected bitfield and write '0' to the w1c bits to avoid side-effects */
    tmp &= ~(PMU_BUF_EN_LV_VREF10A_MASK);
    tmp |= PMU_BUF_EN_LV_VREF10A(state ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->REG_BUF_EN_LV = (uint32_t)tmp;

    PMU_SetWrUpd(baseAddr);
}

/**
 * @brief Sets the VREF12A buffering state
 *
 * This function configures the VREF10A buffering state.
 *
 * @param baseAddr pmu base pointer
 * @param state enable
 */
static inline void PMU_SetBufferVerf12AState(pmu_type_t *const baseAddr, const bool state)
{
    uint32_t tmp = (uint32_t)baseAddr->REG_BUF_EN_LV;
    /* Clear the affected bitfield and write '0' to the w1c bits to avoid side-effects */
    tmp &= ~(PMU_BUF_EN_LV_VREF12A_MASK);
    tmp |= PMU_BUF_EN_LV_VREF12A(state ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->REG_BUF_EN_LV = (uint32_t)tmp;

    PMU_SetWrUpd(baseAddr);
}

/**
 * @brief Gets the MBGP state
 *
 * This functions returns the enable state of MBGP
 *
 * @param baseAddr pmu base pointer
 * @return the MBGP state
 */
static inline bool PMU_GetLdofState(const pmu_type_t *const baseAddr)
{
    uint32_t tmp = (uint32_t)baseAddr->REG_LDOF_EN_LV;
    tmp          = (tmp & PMU_LDOF_EN_LV_MASK) >> PMU_LDOF_EN_LV_SHIFT;

    return (tmp != 0U) ? true : false;
}

/**
 * @brief Configure MBGP reference voltage
 *
 * This function configures MBGP reference voltage.
 *
 * @param baseAddr pmu base pointer
 * @param voltage the new detection voltage.
 *
 */
static inline void PMU_SetMbgpReferenceVol(pmu_type_t *const baseAddr, uint8_t voltage)
{
    uint32_t tmp = baseAddr->REG_MBGP_LV;
    /* Clear the affected bitfield */
    tmp &= ~(PMU_MBGP_LV_MASK);
    tmp |= PMU_MBGP_LV(voltage);
    baseAddr->REG_MBGP_LV = tmp;

    PMU_SetWrUpd(baseAddr);
}

/**
 * @brief Configure LPBGP reference voltage
 *
 * This function configures LPBGP reference voltage.
 *
 * @param baseAddr pmu base pointer
 * @param voltage the new detection voltage.
 *
 */
static inline void PMU_SetLpbgpReferenceVol(pmu_type_t *const baseAddr, uint8_t voltage)
{
    uint32_t tmp = baseAddr->REG_LPBGP_LV;
    /* Clear the affected bitfield */
    tmp &= ~(PMU_LPBGP_LV_MASK);
    tmp |= PMU_LPBGP_LV(voltage);
    baseAddr->REG_LPBGP_LV = tmp;

    PMU_SetWrUpd(baseAddr);
}

/**
 * @brief Configure LPLDO reference voltage
 *
 * This function configures LPLDO reference voltage.
 *
 * @param baseAddr pmu base pointer
 * @param voltage the new detection voltage.
 *
 */
static inline void PMU_SetLpldoReferenceVol(pmu_type_t *const baseAddr, uint8_t voltage)
{
    uint32_t tmp = baseAddr->REG_LPLDO_LV;
    /* Clear the affected bitfield */
    tmp &= ~(PMU_LPLDO_LV_MASK);
    tmp |= PMU_LPLDO_LV(voltage);
    baseAddr->REG_LPLDO_LV = tmp;

    PMU_SetWrUpd(baseAddr);
}

/**
 * @brief Sets LDOC current limiting state
 *
 * This function configures the LDOC current limiting state.
 *
 * @param baseAddr pmu base pointer
 * @param state the new LDOC current limiting state
 */
static inline void PMU_SetLdocCurrentLimitState(pmu_type_t *const baseAddr, const bool state)
{
    uint32_t tmp = (uint32_t)baseAddr->REG_LDOC_LV;
    /* Clear the affected bitfield and write '0' to the w1c bits to avoid side-effects */
    tmp &= ~(PMU_LDOC_LV_CURRENT_LIMIT_MASK);
    tmp |= PMU_LDOC_LV_CURRENT_LIMIT(state ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->REG_LDOC_LV = (uint32_t)tmp;

    PMU_SetWrUpd(baseAddr);
}

/**
 * @brief Configure LDOC reference voltage
 *
 * This function configures LDOC reference voltage.
 *
 * @param baseAddr pmu base pointer
 * @param voltage the new detection voltage.
 *
 */
static inline void PMU_SetLdocReferenceVol(pmu_type_t *const baseAddr, uint8_t voltage)
{
    uint32_t tmp = baseAddr->REG_LDOC_LV;
    /* Clear the affected bitfield */
    tmp &= ~(PMU_LDOC_LV_V11_CORE_MASK);
    tmp |= PMU_LDOC_LV_V11_CORE(voltage);
    baseAddr->REG_LDOC_LV = tmp;

    PMU_SetWrUpd(baseAddr);
}

/**
 * @brief Configure VERF10 output voltage
 *
 * This function configures VERF10 output voltage.
 *
 * @param baseAddr pmu base pointer
 * @param voltage the new VERF10 output voltage.
 *
 */
static inline void PMU_SetVerf10OutputVol(pmu_type_t *const baseAddr, uint8_t voltage)
{
    uint32_t tmp = baseAddr->REG_VREF10_LV;
    /* Clear the affected bitfield */
    tmp &= ~(PMU_VREF10_LV_MASK);
    tmp |= PMU_VREF10_LV(voltage);
    baseAddr->REG_VREF10_LV = tmp;

    PMU_SetWrUpd(baseAddr);
}

/**
 * @brief Sets LDOF current limiting state
 *
 * This function configures the LDOF current limiting state.
 *
 * @param baseAddr pmu base pointer
 * @param state the new LDOF current limiting state
 */
static inline void PMU_SetLdofCurrentLimitState(pmu_type_t *const baseAddr, const bool state)
{
    uint32_t tmp = (uint32_t)baseAddr->REG_LDOF_LV;
    /* Clear the affected bitfield and write '0' to the w1c bits to avoid side-effects */
    tmp &= ~(PMU_LDOF_LV_CURRENT_LIMIT_MASK);
    tmp |= PMU_LDOF_LV_CURRENT_LIMIT(state ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->REG_LDOF_LV = (uint32_t)tmp;

    PMU_SetWrUpd(baseAddr);
}

/**
 * @brief Configure LDOF Core output voltage
 *
 * This function configures LDOF Core output voltage.
 *
 * @param baseAddr pmu base pointer
 * @param voltage the new detection voltage.
 *
 */
static inline void PMU_SetLdofCoreOutputVol(pmu_type_t *const baseAddr, uint8_t voltage)
{
    uint32_t tmp = baseAddr->REG_LDOF_LV;
    /* Clear the affected bitfield */
    tmp &= ~(PMU_LDOF_LV_LDOF_CORE_MASK);
    tmp |= PMU_LDOF_LV_LDOF_CORE(voltage);
    baseAddr->REG_LDOF_LV = tmp;

    PMU_SetWrUpd(baseAddr);
}

/**
 * @brief Use FSM_CNT_VAL overwrite the timeout value of the PMU.
 *
 * This function configures Use FSM_CNT_VAL overwrite the timeout value of the PMU.
 *
 * @param baseAddr pmu base pointer
 * @param state the new overwrite state
 */
static inline void PMU_SetFsmTimeoutCntOverride(pmu_type_t *const baseAddr, const bool state)
{
    uint32_t tmp = (uint32_t)baseAddr->FSM_CNT_CTRL;
    /* Clear the affected bitfield and write '0' to the w1c bits to avoid side-effects */
    tmp &= ~(PMU_FSM_CNT_CTRL_TIMEOUT_OV_MASK);
    tmp |= PMU_FSM_CNT_CTRL_TIMEOUT_OV(state ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->FSM_CNT_CTRL = (uint32_t)tmp;
}

/**
 * @brief Set FSM timeout counting state.
 *
 * This function configures FSM timeout counting state.
 *
 * @param baseAddr pmu base pointer
 * @param state the new timeout counting state
 */
static inline void PMU_SetFsmTimeoutCountingState(pmu_type_t *const baseAddr, const bool state)
{
    uint32_t tmp = (uint32_t)baseAddr->FSM_CNT_CTRL;
    /* Clear the affected bitfield and write '0' to the w1c bits to avoid side-effects */
    tmp &= ~(PMU_FSM_CNT_CTRL_TIMEOUT_EN_MASK);
    tmp |= PMU_FSM_CNT_CTRL_TIMEOUT_EN(state ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->FSM_CNT_CTRL = (uint32_t)tmp;
}

/**
 * @brief Set the timeout value for PMU’s FSM
 *
 * This function set the timeout value for PMU’s FSM.
 *
 * @param baseAddr pmu base pointer
 * @param count the new timeout count.
 *
 */
static inline void PMU_SetFsmCountVal(pmu_type_t *const baseAddr, uint8_t count)
{
    uint32_t tmp = baseAddr->FSM_CNT_VAL;
    /* Clear the affected bitfield */
    tmp &= ~(PMU_FSM_CNT_VAL_MASK);
    tmp |= PMU_FSM_CNT_VAL(count);
    baseAddr->FSM_CNT_VAL = tmp;
}

/**
 * @brief Trigger the chip run into STANDBY mode
 *
 * This function trigger the chip run into STANDBY mode.
 *
 * @param baseAddr pmu base pointer
 *
 */
static inline void PMU_TriggerStandbyMode(pmu_type_t *const baseAddr)
{
    baseAddr->STANDBY_CTRL = 0x1;
}

/**
 * @brief Set STANDBY mode counter
 *
 * This function set STANDBY mode counter.
 *
 * @param baseAddr pmu base pointer
 * @param count the new count.
 *
 */
static inline int32_t PMU_GetStandbyCount(const pmu_type_t *const baseAddr)
{
    uint32_t tmp = baseAddr->STANDBY_MODE_CNT;
    /* Clear the affected bitfield */
    tmp &= PMU_STANDBY_MODE_CNT_MASK;

    return (tmp >> PMU_STANDBY_MODE_CNT_SHIFT);
}

/**
 * @brief Enable the wakeup sourced for STANDBY mode
 *
 * This function enable the wakeup sourced for STANDBY mode.
 *
 * @param baseAddr pmu base pointer
 * @param sources the wakeup sources.
 *
 */
static inline void PMU_SetWakeupSource(pmu_type_t *const baseAddr, const uint32_t sources)
{
    uint32_t tmp = baseAddr->WAKEUP_CTRL;
    /* Clear the affected bitfield */
    tmp &= ~(PMU_WAKEUP_CTRL_MASK);
    tmp |= PMU_WAKEUP_CTRL(sources);
    baseAddr->WAKEUP_CTRL = tmp;
}

/**
 * @brief Disable the wakeup sourced for STANDBY mode
 *
 * This function disable the wakeup sourced for STANDBY mode.
 *
 * @param baseAddr pmu base pointer
 * @param sources the wakeup sources.
 *
 */
static inline void PMU_DisableWakeupSource(pmu_type_t *const baseAddr, const uint32_t sources)
{
    uint32_t tmp = baseAddr->WAKEUP_DISABLE;
    /* Clear the affected bitfield */
    tmp &= ~(PMU_WAKEUP_DISABLE_MASK);
    tmp |= PMU_WAKEUP_DISABLE(sources);
    baseAddr->WAKEUP_DISABLE = tmp;
}

/**
 * @brief Trigger the chip run into Sleep mode
 *
 * This function trigger the chip run into Sleep mode.
 *
 * @param baseAddr pmu base pointer
 *
 */
__STATIC_FORCEINLINE void PMU_TriggerSleepMode(pmu_type_t *const baseAddr)
{
    uint32_t tmp = baseAddr->SLEEP_MODE;
    /* Clear the affected bitfield */
    tmp &= ~(PMU_SLEEP_MODE_TRG_MASK);
    tmp |= PMU_SLEEP_MODE_TRG(1U);
    baseAddr->SLEEP_MODE = tmp;
}

/**
 * @brief Get Source selection for the sleep mode.
 *
 * This function set PMU sleep mode source.
 *
 * @param baseAddr pmu base pointer
 * @param trg sleep mode trigger
 *
 */
__STATIC_FORCEINLINE pmu_sleep_mode_trigger_t PMU_GetSleepModeSource(pmu_type_t *const baseAddr)
{
    uint32_t tmp = baseAddr->SLEEP_MODE;

    tmp = (tmp & PMU_SLEEP_MODE_SOURCE_MASK) >> PMU_SLEEP_MODE_SOURCE_SHIFT;

    return (pmu_sleep_mode_trigger_t)tmp;
}

/**
 * @brief Source selection for the sleep mode.
 *
 * This function set PMU sleep mode source.
 *
 * @param baseAddr pmu base pointer
 * @param trg sleep mode trigger
 *
 */
static inline void PMU_SetSleepModeSource(pmu_type_t *const baseAddr, pmu_sleep_mode_trigger_t trg)
{
    uint32_t tmp = baseAddr->SLEEP_MODE;
    /* Clear the affected bitfield */
    tmp &= ~(PMU_SLEEP_MODE_SOURCE_MASK);
    tmp |= PMU_SLEEP_MODE_SOURCE(trg);
    baseAddr->SLEEP_MODE = tmp;
}

/**
 * @brief Enable or disable RSNT_BOR as a reset source
 *
 * This function enable or disable RSNT_BOR as a reset source.
 *
 * @param baseAddr pmu base pointer
 * @param state the new RSNT_BOR state
 *
 */
static inline void PMU_SetBorRsntSource(pmu_type_t *const baseAddr, bool state)
{
    uint32_t tmp = baseAddr->BOR_RSTN_MASK;
    /* Clear the affected bitfield */
    tmp &= ~(PMU_BOR_RSTN_MASK_MASK);
    /* 0 as reset source */
    tmp |= PMU_BOR_RSTN_MASK(state ? (uint32_t)0U : (uint32_t)1U);
    baseAddr->BOR_RSTN_MASK = tmp;
}

/**
 * @brief Set the PMU interrupt mask
 *
 * This function set the PMU interrupt mask.
 *
 * @param baseAddr pmu base pointer
 * @param trg sleep mode trigger
 *
 */
static inline void PMU_SetInterruptMask(pmu_type_t *const baseAddr, uint32_t intr)
{
    uint32_t tmp = baseAddr->REG_INTR_MASK;
    /* Clear the affected bitfield */
    tmp &= ~(PMU_INTR_MASK_MASK);
    tmp |= PMU_INTR_MASK(intr);
    baseAddr->REG_INTR_MASK = tmp;
}

/**
 * @brief Gets the PMU interrupt status
 *
 * This functions returns the PMU interrupt status
 *
 * @param baseAddr pmu base pointer
 * @return the PMU interrupt status
 */
static inline uint32_t PMU_GetInterruptStatus(const pmu_type_t *const baseAddr)
{
    return (uint32_t)baseAddr->INTR_STATUS & PMU_INTR_STATUS_MASK;
}

/**
 * @brief Set the PMU Oscillator state
 *
 * This function set the PMU Oscillator state.
 *
 * @param baseAddr pmu base pointer
 * @param state the new Oscillator state
 *
 */
static inline void PMU_SetOscillatorState(pmu_type_t *const baseAddr, bool state)
{
    uint32_t tmp = baseAddr->REG_EN_OSC_LV_ASYNC;
    /* Clear the affected bitfield */
    tmp &= ~(PMU_REG_EN_OSC_LV_ASYNC_MASK);
    tmp |= PMU_REG_EN_OSC_LV_ASYNC((state ? (uint32_t)1U : (uint32_t)0U));
    baseAddr->REG_EN_OSC_LV_ASYNC = tmp;
}

/**
 * @brief Set the PMU reboot counter state
 *
 * This function set the PMU reboot counter state.
 *
 * @param baseAddr pmu base pointer
 * @param state the new Reboot counter state
 *
 */
static inline void PMU_SetRebootCounterState(pmu_type_t *const baseAddr, bool state)
{
    uint32_t tmp = baseAddr->REBOOT_CNT_CTRL;
    /* Clear the affected bitfield */
    tmp &= ~(PMU_REBOOT_CNT_CTRL_MASK);
    tmp |= PMU_REBOOT_CNT_CTRL(state ? (uint32_t)1U : (uint32_t)0U);
    baseAddr->REBOOT_CNT_CTRL = tmp;
}

/**
 * @brief Set the PMU Reboot Fail interrupt mask
 *
 * This function set the PMU Reboot Fail interrupt mask.
 *
 * @param baseAddr pmu base pointer
 *
 */
static inline void PMU_SetRebootFailInterruptMask(pmu_type_t *const baseAddr, bool state)
{
    uint32_t tmp = baseAddr->REBOOT_FAIL_IRQ_MASK;
    /* Clear the affected bitfield */
    tmp &= ~(PMU_REBOOT_FAIL_IRQ_MASK_MASK);
    tmp |= PMU_REBOOT_FAIL_IRQ_MASK((state ? (uint32_t)1U : (uint32_t)0U));
    baseAddr->REBOOT_FAIL_IRQ_MASK = tmp;
}

/**
 * @brief Gets the PMU Reboot Fail interrupt state
 *
 * This functions returns the PMU Reboot Fail interrupt state.
 *
 * @param baseAddr pmu base pointer
 * @return the PMU interrupt status
 */
static inline uint32_t PMU_GetRebootFailInterruptStatus(const pmu_type_t *const baseAddr)
{
    return (uint32_t)baseAddr->REBOOT_FAIL_IRQ_STATUS & PMU_REBOOT_FAIL_IRQ_STATUS_MASK;
}

/**
 * @brief Clear the PMU Reboot Fail interrupt state
 *
 * This functions clear the PMU Reboot Fail interrupt state
 *
 * @param baseAddr pmu base pointer
 * @return the PMU interrupt status
 */
static inline void PMU_ClearRebootFailInterrupt(pmu_type_t *const baseAddr)
{
    uint32_t tmp = baseAddr->REBOOT_FAIL_IRQ_CLEAR;
    /* Clear the affected bitfield */
    tmp &= ~(PMU_REBOOT_FAIL_IRQ_CLEAR_MASK);
    tmp |= PMU_REBOOT_FAIL_IRQ_CLEAR(1U);
    baseAddr->REBOOT_FAIL_IRQ_CLEAR = tmp;
}

/**
 * @brief Software reset
 *
 * This functions trigger software reset
 *
 * @param baseAddr pmu base pointer
 * @return the PMU interrupt status
 * @param state software state
 */
static inline void PMU_TriggerSoftwareReset(pmu_type_t *const baseAddr, bool state)
{
    uint32_t tmp = baseAddr->SW_SYS_RSTN;
    tmp &= ~(PMU_SW_SYS_RSTN_MASK);
    tmp |= PMU_SW_SYS_RSTN((state ? (uint32_t)0U : (uint32_t)1U));
    baseAddr->SW_SYS_RSTN = tmp;
}

#if defined(__cplusplus)
}
#endif

#endif
