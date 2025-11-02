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
 * @file crg_hw_access.h
 * @brief CRG driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __CRG_HW_ACCESS_H__
#define __CRG_HW_ACCESS_H__

#include <stdbool.h>
#include <stddef.h>

#include "../pmu/EAM2011/pmu_register.h"
#include "clock_EAM2011.h"
#include "crg_register.h"

/**
 * @brief CRG ClockOut type.
 */
typedef enum {
    CRG_SPLL_PD_COREFUNC = 0U,    /**< Power down pll core function, high active. */
    CRG_SPLL_PD_RESERVED = 1U,    /**< Power down reserved. */
    CRG_SPLL_PD_POSDIV0  = 2U,    /**< Power down post divider 0, high active. */
    CRG_SPLL_PD_POSDIV1  = 3U,    /**< Power down post divider 1, high active. */
    CRG_SPLL_PD_POSDIV2  = 4U,    /**< Power down post divider 2, high active. */
    CRG_SPLL_PD_BIASFUNC = 5U,    /**< Power down bias function, high active. */
    CRG_SPLL_PD_MASK     = 0x3FU, /**< Power down bias function, high active. */
} crg_pll_pd_ctrl_t;

/**
 * @brief Gets the source clock for work clock.
 *
 * This function gets the source clock for work clock.
 *
 * @param base Register base address for the CRG instance.
 * @return uint32_t Work clock source
 */
__STATIC_FORCEINLINE uint32_t CRG_GetWorkClockSel(const crg_type_t *const base)
{
    return (uint32_t)base->WORK_CLKGFSEL & CRG_WORK_CLKSEL_MASK;
}

/**
 * @brief Sets CRG source clock for Work clock.
 *
 * This function sets source clock for Work clock.
 *
 * @param base Register base address for the CRG instance.
 * @param source for Work clock
 */
__STATIC_FORCEINLINE void CRG_SetWorkClockSel(crg_type_t *const base, uint32_t source)
{
    uint32_t value = base->WORK_CLKGFSEL;

    value &= ~CRG_WORK_CLKSEL_MASK;
    value |= CRG_WORK_CLKSEL(source);
    base->WORK_CLKGFSEL = value;
}

/**
 * @brief Gets the source clock for SPLL.
 *
 * This function gets the source clock for SPLL.
 *
 * @param base Register base address for the CRG instance.
 * @return uint32_t SPLL source
 */
__STATIC_FORCEINLINE uint32_t CRG_GetSpllClockSel(const crg_type_t *const base)
{
    return (uint32_t)base->PLL_SRCSEL & CRG_SPLL_SRCSEL_MASK;
}

/**
 * @brief Sets CRG source clock for SPLL.
 *
 * This function sets source clock for SPLL.
 *
 * @param base Register base address for the CRG instance.
 * @param source for spll
 */
__STATIC_FORCEINLINE void CRG_SetSpllClockSel(crg_type_t *base, uint32_t source)
{
    uint32_t value = base->PLL_SRCSEL;

    value &= ~CRG_SPLL_SRCSEL_MASK;
    value |= CRG_SPLL_SRCSEL(source);
    base->PLL_SRCSEL = value;
}

/**
 * @brief Sets CRG source clock for SPLL.
 *
 * This function sets source clock for SPLL.
 *
 * @param base Register base address for the CRG instance.
 * @param source for spll
 */
__STATIC_FORCEINLINE uint32_t CRG_GetSpllCko0Freq(const crg_type_t *const base, uint32_t refclkReq)
{
    uint32_t dev_set  = (uint32_t)((base->SPLL_DIVSET & (CRG_SPLL_DEVSET_INTEGER_MASK | CRG_SPLL_DEVSET_FRACT_MASK)) >>
                                  CRG_SPLL_DEVSET_FRACT_SHIFT);
    uint32_t ref_div  = (uint32_t)((base->SPLL_REFDIV & CRG_SPLL_REFDIV_MASK) >> CRG_SPLL_REFDIV_SHIFT);
    uint32_t pos_div0 = (uint32_t)((base->SPLL_POSDIV0 & CRG_SPLL_POSDIV_MASK) >> CRG_SPLL_POSDIV_SHIFT);
    uint32_t ckop     = (uint32_t)(4 * dev_set / (1 << CRG_SPLL_DEVSET_INTEGER_SHIFT) * refclkReq / (ref_div + 1));

    return (uint32_t)(ckop / (2 * (pos_div0 + 1)));
}

/**
 * @brief Sets SPLL Differential Output clock
 *
 * @param base        register base address for the crg instance.
 * @param refDiv        spll ref divider value
 * @param divSetInteger  div_set integer divider value
 * @param divSetFractional  div_set factional divider value
 */
__STATIC_FORCEINLINE void CRG_SetSpllDifferenceClko(crg_type_t *base, uint8_t refDiv, uint16_t divSetInteger,
                                             uint16_t divSetFractional)
{
    /* Set PLL configuration. */
    base->SPLL_REFDIV = CRG_SPLL_REFDIV(refDiv);
    base->SPLL_DIVSET = CRG_SPLL_DEVSET_INTEGER(divSetInteger) | CRG_SPLL_DEVSET_FRACT(divSetFractional);
}

/**
 * @brief Sets SPLL Post divider 0 output clock
 *
 * @param base      register base address for the crg instance.
 * @param posdiv0   post divider 0 value
 * @param posdiv1   post divider 1 value
 * @param posdiv2   post divider 2 value
 */
__STATIC_FORCEINLINE void CRG_SetSpllPostClko(crg_type_t *base, uint8_t posdiv0, uint8_t posdiv1, uint8_t posdiv2)
{
    /* Set PLL configuration. */
    base->SPLL_POSDIV0 = CRG_SPLL_POSDIV(posdiv0);
    base->SPLL_POSDIV1 = CRG_SPLL_POSDIV(posdiv1);
    base->SPLL_POSDIV2 = CRG_SPLL_POSDIV(posdiv2);
}

/**
 * @brief Get Power down control for PLL
 *
 * @param base  register base address for the crg instance.
 */
__STATIC_FORCEINLINE uint32_t CRG_GetSpllPowerCtrl(crg_type_t *base)
{
    /* Get PLL configuration. */
    uint32_t regValue = (uint32_t)base->SPLL_PDPLL;
    regValue          = (regValue & CRG_SPLL_PDPLL_MASK) >> CRG_SPLL_PDPLL_SHIFT;

    return regValue;
}

/**
 * @brief Sets Power down control for PLL
 *
 * @param base   register base address for the crg instance.
 * @param pdmask pd_pll mask
 */
__STATIC_FORCEINLINE void CRG_SetSpllPowerCtrl(crg_type_t *base, uint8_t pdmask)
{
    /* Set PLL configuration. */
    base->SPLL_PDPLL = CRG_SPLL_PDPLL(pdmask);
}

/**
 * @brief Get PLL lock indication
 *
 * This function get PLL lock indication.
 *
 * @param base Register base address for the CRG instance.
 * @return PLL lock indication
 *      - true : PLL locked.
 *      - false : true : PLL unlocked.
 */
__STATIC_FORCEINLINE bool CRG_GetSpllLocked(crg_type_t *base)
{
    uint32_t regValue = (uint32_t)base->SPLL_LK;
    regValue          = (regValue & CRG_SPLL_PLLLK_MASK) >> CRG_SPLL_PLLLK_SHIFT;
    return (regValue == 0U) ? false : true;
}

#if 0
/**
 * @brief Gets the source clock for LPCK clock.
 *
 * This function gets the source clock for LPCK clock.
 *
 * @param base Register base address for the CRG instance.
 * @return uint32_t LPCK clock source
 */
__STATIC_FORCEINLINE uint32_t CRG_GetLpckClockSel(const crg_type_t *const base)
{
    return (uint32_t)base->LPCK_SRCSEL;
}

/**
 * @brief Sets CRG source clock for LPCK clock.
 *
 * This function sets source clock for LPCK clock.
 *
 * @param base Register base address for the CRG instance.
 * @param source for LPCK clock
 */
__STATIC_FORCEINLINE void CRG_SetLpckClockSel(crg_type_t *const base, uint32_t source)
{
    uint32_t value = base->LPCK_SRCSEL;

    value &= ~CRG_LPCK_CLKSEL_MASK;
    value |= CRG_LPCK_CLKSEL(source);
    base->LPCK_SRCSEL = value;
}
#endif

#if 0
__STATIC_FORCEINLINE bool CRG_GetSrcoscEnableState(const crg_type_t *const base)
{
    uint32_t regValue = (uint32_t)base->SRCOSC_OSCEN;
    regValue          = (regValue & CRG_SRCOSC_OSCEN_MASK) >> CRG_SRCOSC_OSCEN_SHIFT;
    return (regValue == 0U) ? false : true;
}

__STATIC_FORCEINLINE void CRG_SetSrcoscEnableState(crg_type_t *base, bool enable)
{
    base->SRCOSC_OSCEN = CRG_SRCOSC_OSCEN(enable);
}

__STATIC_FORCEINLINE void CRG_SetSrcoscOutputFreq(crg_type_t *base, crg_rcosc_oscclk_sel_t oscclk,
                                           crg_rcosc_fmargin_sel_t fmargin)
{
    base->SRCOSC_OSCCLKSEL  = CRG_SRCOSC_OSCCLKSEL(oscclk);
    base->SRCOSC_FMARGINSEL = CRG_SRCOSC_FMARGINSEL(fmargin);
}

__STATIC_FORCEINLINE void CRG_SetSrcoscFreqCtune(crg_type_t *base, uint32_t ctune)
{
    base->SRCOSC_FREQCTUNE = CRG_SRCOSC_FREQ_CTUNE(ctune);
}

__STATIC_FORCEINLINE void CRG_SetSrcoscFreqFtune(crg_type_t *base, uint32_t ftune)
{
    base->SRCOSC_FREQFTUNE = CRG_SRCOSC_FREQ_CTUNE(ftune);
}

__STATIC_FORCEINLINE bool CRG_GetSrcoscClkStableStatus(const crg_type_t *const base)
{
    uint32_t regValue = (uint32_t)base->SRCOSC_CLKSTABLE;
    regValue          = (regValue & CRG_SRCOSC_CLK_STABLE_MASK) >> CRG_SRCOSC_CLK_STABLE_SHIFT;
    return (regValue == 0U) ? false : true;
}

__STATIC_FORCEINLINE bool CRG_GetSrcoscTrimStatus(const crg_type_t *const base)
{
    uint32_t regValue = (uint32_t)base->SRCOSC_TRIMDONE;
    regValue          = (regValue & CRG_SRCOSC_TRIMDONE_MASK) >> CRG_SRCOSC_TRIMDONE_SHIFT;
    return (regValue == 0U) ? false : true;
}
#endif

/**
 * @brief Gets the source clock for LPCK clock.
 *
 * This function gets the source clock for LPCK clock.
 *
 * @param base Register base address for the CRG instance.
 * @return uint32_t LPCK clock source
 */
__STATIC_FORCEINLINE bool CRG_GetFrcoscEnableState(const crg_type_t *const base)
{
    uint32_t regValue = (uint32_t)base->FRCOSC_OSCEN;
    regValue          = (regValue & CRG_FRCOSC_OSCEN_MASK) >> CRG_FRCOSC_OSCEN_SHIFT;
    return (regValue == 0U) ? false : true;
}

/**
 * @brief Sets the source clock for LPCK clock.
 *
 * This function gets the source clock for LPCK clock.
 *
 * @param base Register base address for the CRG instance.
 * @return uint32_t LPCK clock source
 */
__STATIC_FORCEINLINE void CRG_SetFrcoscEnableState(crg_type_t *base, bool enable)
{
    base->FRCOSC_OSCEN = CRG_FRCOSC_OSCEN(enable);
}

#if 0
__STATIC_FORCEINLINE void CRG_SetFrcoscOutputFreq(crg_type_t *base, crg_rcosc_oscclk_sel_t oscclk,
                                           crg_rcosc_fmargin_sel_t fmargin)
{
    base->FRCOSC_OSCCLKSEL  = CRG_FRCOSC_OSCCLKSEL(oscclk);
    base->FRCOSC_FMARGINSEL = CRG_FRCOSC_FMARGINSEL(fmargin);
}

__STATIC_FORCEINLINE void CRG_SetFrcoscFreqCtune(crg_type_t *base, uint32_t ctune)
{
    base->FRCOSC_FREQCTUNE = CRG_FRCOSC_FREQ_CTUNE(ctune);
}

__STATIC_FORCEINLINE void CRG_SetFrcoscFreqFtune(crg_type_t *base, uint32_t ftune)
{
    base->FRCOSC_FREQFTUNE = CRG_FRCOSC_FREQ_CTUNE(ftune);
}

__STATIC_FORCEINLINE bool CRG_GetFrcoscClkStableStatus(const crg_type_t *const base)
{
    uint32_t regValue = (uint32_t)base->FRCOSC_CLKSTABLE;
    regValue          = (regValue & CRG_FRCOSC_CLK_STABLE_MASK) >> CRG_FRCOSC_CLK_STABLE_SHIFT;
    return (regValue == 0U) ? false : true;
}

__STATIC_FORCEINLINE bool CRG_GetFrcoscTrimStatus(const crg_type_t *const base)
{
    uint32_t regValue = (uint32_t)base->FRCOSC_TRIMDONE;
    regValue          = (regValue & CRG_FRCOSC_TRIMDONE_MASK) >> CRG_FRCOSC_TRIMDONE_SHIFT;
    return (regValue == 0U) ? false : true;
}
#endif

/**
 * @brief Gets the source clock for rtc clock.
 *
 * This function gets the source clock for work clock.
 *
 * @param base Register base address for the CRG instance.
 * @return Work clock source
 */
__STATIC_FORCEINLINE uint32_t CRG_GetRtcClockSel(const pmu_type_t *const base)
{
    return (uint32_t)base->RTC_CK_GF_SEL & CRG_WORK_CLKSEL_MASK;
}

/**
 * @brief Sets CRG source clock for rtc clock.
 *
 * This function sets source clock for Work clock.
 *
 * @param base Register base address for the CRG instance.
 * @param source for Work clock
 */
__STATIC_FORCEINLINE void CRG_SetRtcClockSel(pmu_type_t *const base, uint32_t source)
{
#if 1
    uint32_t value = base->RTC_CK_GF_SEL;

    value &= ~PMU_RTC_CK_GF_SEL_MASK;
    value |= PMU_RTC_CK_GF_SEL(source);
    base->RTC_CK_GF_SEL = value;
#endif
}

/**
 * @brief Enable IPG clock.
 *
 * @param base Register base address for the CRG instance.
 */
__STATIC_FORCEINLINE void CRG_SetIpgClkEnable(crg_type_t *const base)
{
    base->IPG0_CLKEN = CRG_IPG0_CLKEN_MASK;
    base->IPG1_CLKEN = CRG_IPG1_CLKEN_MASK;
    base->IPG2_CLKEN = CRG_IPG2_CLKEN_MASK;
    base->IPG3_CLKEN = CRG_IPG3_CLKEN_MASK;
}

/**
 * @brief Enable occ clock.
 *
 * @param base Register base address for the CRG instance.
 */
__STATIC_FORCEINLINE void CRG_SetSubsysOccClkEnable(crg_type_t *const base)
{
    uint32_t value = base->OCC_CLKEN;

    value |= (CRG_OCC_CLKEN_ON);
    base->OCC_CLKEN = value;
}

/**
 * @brief Peripherals Rest.
 *
 * This function resets the peripherals.
 *
 * @param base Register base address for the CRG instance.
 */
__STATIC_FORCEINLINE void CRG_SetPeripheralsRest(crg_type_t *const base)
{
    base->SRST_N = CRG_SRST_N_DEF;
    base->SRST1_N = CRG_SRST1_N_DEF;
    base->SRST2_N = CRG_SRST2_N_DEF;
    __asm__ volatile ("fence rw, rw");
    __asm__ volatile ("nop");
    __asm__ volatile ("nop");
    __asm__ volatile ("nop");
    __asm__ volatile ("nop");
    base->SRST_N  = CRG_SRST_N_MASK;
    base->SRST1_N = CRG_SRST1_N_MASK;
    base->SRST2_N = CRG_SRST2_N_MASK;
}

#endif
