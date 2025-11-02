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
 * @file gtmr_hw_access.h
 * @brief GTMR driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-12
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __GTMR_HW_ACCESS_H__
#define __GTMR_HW_ACCESS_H__

#include "gtmr_driver.h"
#include "gtmr_register.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief  Initialize the GTMR instance to reset values.
 *
 * @param  base  GTMR base pointer
 *
 */
void GTMR_Reset(gtmr_type_t *const base);

/**
 * @brief  Get the Int(FULL) Flag state
 *
 * @param  base  GTMR base pointer
 * @return true if  FULL flag asserted
 * @return false if FULL flag not asserted
 */
static inline bool GTMR_GetIntFlag(const gtmr_type_t *const base)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->SR;
    tmp          = (tmp & GTMR_GSR_FULL_MASK) >> GTMR_GSR_FULL_SHIFT;

    return ((tmp == 1U) ? true : false);
}

/**
 * @brief  Enable or disable GTMR interrupt
 *
 * @param  base    GTMR base pointer
 * @param  enable  Enable or disable the interrupt
 */
static inline void GTMR_EnableInt(gtmr_type_t *const base, bool enable)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CTRL;
    /* Clear the affected bit-field and write '0' to the w1c bits to avoid side-effects */
    tmp &= ~GTMR_CTRL_INEN_MASK;
    tmp |= GTMR_CTRL_INEN(enable ? (uint32_t)1U : (uint32_t)0U);
    base->CTRL = tmp;
}

/**
 * @brief  Get GTMR Enable state.
 *
 * @param  base  GTMR base pointer
 * @return true  if GTMR enabled
 * @return false if GTMR disabled
 */
static inline bool GTMR_GetEnable(const gtmr_type_t *const base)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CTRL;
    tmp          = (tmp & GTMR_CTRL_EN_MASK) >> GTMR_CTRL_EN_SHIFT;

    return ((tmp == 1U) ? true : false);
}

/**
 * @brief Enable the GTMR
 *
 * @param  base  GTMR base pointer
 */
static inline void GTMR_Enable(gtmr_type_t *const base)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CTRL;
    /* Clear the affected bit-field and write '0' to the w1c bits to avoid side-effects */
    tmp &= ~GTMR_CTRL_EN_MASK;
    tmp |= GTMR_CTRL_EN(1U);
    base->CTRL = tmp;
}

/**
 * @brief GTMR software triggered output.
 *
 * @param  base    GTMR base pointer
 * @param  TriggerType software of trgmux(hardware)
 */
static inline void GTMR_GenSoftWareTrigger(gtmr_type_t *const base, gtmr_Triggermode_t TriggerType)
{
    OS_ASSERT(base != NULL);
    uint32_t tmp = base->CTRL;
    /* Clear the affected bit-field and write '0' to the w1c bits to avoid side-effects */
    tmp &= ~GTMR_CTRL_STRIG_MASK;

    if (TriggerType == GTMR_TRIGGERMODE_SOFT) {
        tmp |= GTMR_CTRL_STRIG(GTMR_TRIGGERMODE_SOFT);
    } else {
        tmp |= GTMR_CTRL_STRIG(GTMR_TRIGGERMODE_HARD);
    }
    base->CTRL = tmp;
}

/**
 * @brief Clear GTMR trigger flag
 *
 * @param  base  GTMR base pointer
 */
static inline void GTMR_ClearTriggerFlag(gtmr_type_t *const base)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CTRL;

    tmp &= ~GTMR_CTRL_TRIGF_MASK;
    tmp |= GTMR_CTRL_TRIGF(1U);
    base->CTRL = tmp;
}

/**
 * @brief  Stop the GTMR counter not stop the GTMR module.
 *
 * @param  base   GTMR base pointer
 * @param  enable enable or disable the gtmr stop mode
 */
static inline void GTMR_Stop(gtmr_type_t *const base, bool enable)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CTRL;
    tmp &= ~GTMR_CTRL_STOP_MASK;
    if (enable) {
        tmp |= GTMR_CTRL_STOP(1U);
    } else {
        tmp |= GTMR_CTRL_STOP(0U);
    }
    base->CTRL = tmp;
}

/**
 * @brief  Disable the GTMR
 *
 * @param  base     GTMR base pointer
 */
static inline void GTMR_Disable(gtmr_type_t *const base)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CTRL;
    /* Clear the affected bit-field and write '0' to the w1c bits to avoid side-effects */
    tmp &= ~GTMR_CTRL_EN_MASK;
    tmp |= GTMR_CTRL_EN(0U);
    base->CTRL = tmp;
}

/**
 * @brief  reset GTMR counter.GTMR value set to initial value that user set after counter reset.
 *
 * @param  base  GTMR base pointer
 */
static inline void GTMR_CounterReset(gtmr_type_t *const base)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CTRL;
    tmp &= ~GTMR_GSR_CRST_MASK;
    tmp |= GTMR_GSR_CRST(1U);
    base->SR = tmp;
}

/**
 * @brief  Get the current Counter Value
 *
 * @param  base  GTMR base pointer
 * @return uint32_t
 *         The Counter Value
 */
static inline uint32_t GTMR_GetCounterValue(gtmr_type_t *const base)
{
    OS_ASSERT(base != NULL);

    /* Write dummy value before reading register */
    // base->NCNT = GTMR_NCNT(0U);
    uint32_t cnr = (uint32_t)base->NCNT;
    return cnr;
}

/**
 * @brief  Configure the counter Value
 *
 * @param  base      GTMR base pointer
 * @param  compVal   The new counter Value
 */
static inline void GTMR_SetStartValue(gtmr_type_t *const base, const uint32_t compVal)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->SCNT;
    tmp &= ~GTMR_SCNT_MASK;
    tmp |= GTMR_SCNT(compVal);
    base->SCNT = tmp;
}

/**
 * @brief Config GTMR trigger mode by software or hardware(trgmux).The GTMR wile not start working until it is triggered.
 *
 * @param  base    GTMR base pointer
 * @param  TriggerType software of hardware(trgmux)
 */
static inline void GTMR_SetTriggerType(gtmr_type_t *const base, gtmr_Triggermode_t TriggerType)
{

    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CTRL;

    tmp &= ~GTMR_CTRL_TRIGS_MASK;
    if (TriggerType == GTMR_TRIGGERMODE_SOFT) {
        tmp |= GTMR_CTRL_TRIGS(GTMR_TRIGGERMODE_SOFT);

    } else {

        tmp |= GTMR_CTRL_TRIGS(GTMR_TRIGGERMODE_HARD);
    }
    base->CTRL = tmp;
}

/**
 * @brief  Configure GTMR Trigger Value.When the counter value equal to TriggerVal,GTMR generates an external trigger signal.
 *
 * @param  base       GTMR base pointer
 * @param  TriggerVal The new Trigger Value
 */
static inline void GTMR_SetTriggerValue(gtmr_type_t *const base, const uint32_t TriggerVal)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->TCNT;
    tmp &= ~GTMR_TCNT_MASK;
    tmp |= GTMR_TCNT(TriggerVal);
    base->TCNT = tmp;
}

/**
 * @brief Configure GTMR continuous mode,single operation mode or continuous operation mode.
 *
 * @param  base         GTMR base pointer
 * @param  ContinueMode  enable or disable Continuous mode
 */
static inline void GTMR_SetTriggerContinuous(gtmr_type_t *const base, bool ContinueMode)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CTRL;

    tmp &= ~GTMR_CTRL_TRIGM_MASK;
    if (ContinueMode) {
        tmp |= GTMR_CTRL_TRIGM(1);

    } else {

        tmp |= GTMR_CTRL_TRIGM(0);
    }
    base->CTRL = tmp;
}

/**
 * @brief Clear GTMR interrupt flag
 *
 * @param  base GTMR base pointer
 */
static inline void GTMR_ClearInt(gtmr_type_t *const base)
{

    OS_ASSERT(base != NULL);

    uint32_t tmp = base->SR;
    tmp &= ~GTMR_GSR_FULL_MASK;
    tmp |= GTMR_GSR_FULL(1U);
    base->SR = tmp;
}

/**
 * @brief Enable single software trigger
 *
 * @param  base   GTMR base pointer
 */
static inline void GTMR_GenOnceTrigger(gtmr_type_t *const base)
{

    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CTRL;
    /* Clear the affected bit-field and write '0' to the w1c bits to avoid side-effects */
    tmp &= ~GTMR_CTRL_TRIGR_MASK;
    tmp |= GTMR_CTRL_TRIGR(1U);
    base->CTRL = tmp;
}

#if defined(__cplusplus)
}
#endif

#endif
