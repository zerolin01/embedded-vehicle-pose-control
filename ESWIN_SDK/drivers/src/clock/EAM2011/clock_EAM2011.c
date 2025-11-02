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
 * @file clock_EAM2011.c
 * @brief Clock EAM2011 driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */
#include "fmc_flash_driver.h"
#include "clock_EAM2011.h"
#include "crg_hw_access.h"
#include "crg_register.h"
#include "emps_platform.h"
#include <stddef.h>

/* Clock Sel  */
static clock_names_t spllSrcClkSel[]  = {FROSC_CLK, SOSC_CLK};
static clock_names_t rtcClkSel[]      = {LPOSC32K_CLK, RTCOSC_CLK};
static clock_names_t workClkSel[]     = {FROSC_CLK, SPLL_CLK, SOSC_CLK, SROSC_CLK};
static clock_names_t wdogClkSel[]     = {LPOSC_CLK, PCTMR_FUNC_CLK, PMU_AON_CLK, RTCOSC_CLK};
static clock_names_t supertmrClkSel[] = {TCLK0_CLK, TCLK1_CLK, TCLK2_CLK, SUPERTMR_FUNC_CLK};

/* Clock Arrary  */
clock_core_t *g_stCrgClocks[CLOCK_NAME_COUNT];

#if 0
clock_rcosc_t s_rcoscClocks[] = {
    CRG_RCOSC_CLK(SROSC_CLK),
    CRG_RCOSC_CLK(FROSC_CLK),
};
#endif

typedef enum {
    EXT_CLK_RTCOSC,
    EXT_CLK_SOSC,
    EXT_CLK_TCLK0,
    EXT_CLK_TCLK1,
    EXT_CLK_TCLK2,
} CRG_EXT_CLK_E;

/* Clock Arrary  */
static clock_extclk_t s_extClocks[] = {
    CRG_EXTCLK_CLK(RTCOSC_CLK, 0UL),
    CRG_EXTCLK_CLK(SOSC_CLK, 0UL),
    CRG_EXTCLK_CLK(TCLK0_CLK, 0UL),
    CRG_EXTCLK_CLK(TCLK1_CLK, 0UL),
    CRG_EXTCLK_CLK(TCLK2_CLK, 0UL),
};

static clock_fixed_rate_t s_fixedRateClocks[] = {
    CRG_FIXED_RATE_CLK(SROSC_CLK, 8000000UL),
#ifndef EMPS_SIMULATION
    CRG_FIXED_RATE_CLK(FROSC_CLK, 24000000UL),
#else
    CRG_FIXED_RATE_CLK(FROSC_CLK, 48000000UL),
#endif
    CRG_FIXED_RATE_CLK(LPOSC_CLK, 128000UL),
};

static clock_fixed_factor_t s_fixedFactorClocks[] = {
    CRG_FIXED_FACTOR_CLK(LPOSC32K_CLK, LPOSC_CLK, 4, 1),
    CRG_FIXED_FACTOR_CLK(GTMR_FUNC_CLK, SROSC_CLK, 8, 1),
    CRG_FIXED_FACTOR_CLK(TSENSOR_LV_CLK, SROSC_CLK, 16, 1),
};

static clock_spll_t s_spllClocks[] = {
    CRG_SPLL_CLK(SPLL_CLK, SPLL_SRC_CLK),
};

static clock_mux_t s_muxClocks[] = {
    CRG_MUX_CLK(SPLL_SRC_CLK, &CRG->PLL_SRCSEL, 0, 0x1, spllSrcClkSel),
    CRG_MUX_CLK(WORK_CLK, &CRG->WORK_CLKGFSEL, 0, 0x2, workClkSel),
    CRG_MUX_CLK(WDOG_CLK, &CRG->LPOCK_CLKSEL, 0, 0x2, wdogClkSel),
    CRG_MUX_CLK(SUPERTMR0_SELCLK, &CRG->SUPERTMR0_TCLKSEL, 0, 0x2, supertmrClkSel),
    CRG_MUX_CLK(SUPERTMR1_SELCLK, &CRG->SUPERTMR1_TCLKSEL, 0, 0x2, supertmrClkSel),
    CRG_MUX_CLK(SUPERTMR2_SELCLK, &CRG->SUPERTMR2_TCLKSEL, 0, 0x2, supertmrClkSel),
    CRG_MUX_CLK(SUPERTMR3_SELCLK, &CRG->SUPERTMR3_TCLKSEL, 0, 0x2, supertmrClkSel),
    CRG_MUX_CLK(SUPERTMR4_SELCLK, &CRG->SUPERTMR4_TCLKSEL, 0, 0x2, supertmrClkSel),
    CRG_MUX_CLK(SUPERTMR5_SELCLK, &CRG->SUPERTMR5_TCLKSEL, 0, 0x2, supertmrClkSel),
    CRG_MUX_CLK(AON_CLK, (void __IO *)0x4400004C, 0, 0x1, rtcClkSel),
};

static clk_div_table_t s_div8Table[] = {{0, 1}, {1, 2}, {2, 4}, {3, 8}, {4, 16}, {5, 32}, {6, 64}, {7, 128}, {0, 0}};

static clock_divider_t s_divClocks[] = {
    CRG_DIV_CLK(CPU_CORE_CLK, WORK_CLK, &CRG->MAINBUS_CLKDIV, 0, 0x3, s_div8Table),
    CRG_DIV_CLK(MAIN_BUS_MOTOR_CTRL_CLK, MAIN_BUS_CLK, &CRG->MAINBUS_MOTOR_CLKDIV, 0, 0x3, s_div8Table),
    CRG_DIV_CLK(MAIN_BUS_AUX_CLK, MAIN_BUS_CLK, &CRG->MAINBUS_AUX_CLKDIV, 0, 0x3, s_div8Table),
    CRG_DIV_CLK(PERIPH_BUS_CLK, WORK_CLK, &CRG->PERIPHBUS_CLKDIV, 0, 0x3, s_div8Table),
    CRG_DIV_CLK(SUPERTMR_FUNC_CLK, WORK_CLK, &CRG->SUPERTMR_CLKDIV, 0, 0x3, s_div8Table),
    CRG_DIV_CLK(MAIN_BUS_PLL_FUNC_CLK, WORK_CLK, &CRG->PLLFUNC_CLKDIV, 0, 0x3, s_div8Table),
    CRG_DIV_CLK(SUPERIO_CLK, WORK_CLK, &CRG->SUPERIO_CLKDIV, 0, 0x3, s_div8Table),
    CRG_DIV_CLK(PERIPH_BUS_PLL_FUNC_CLK, WORK_CLK, &CRG->PERIPH_PLL_CLKDIV, 0, 0x3, s_div8Table),
    CRG_DIV_CLK(PERIPH_BUS_PLL_AUX_FUNC_CLK, WORK_CLK, &CRG->PERIPH_PLL_AUX_CLKDIV, 0, 0x3, s_div8Table),
    CRG_DIV_CLK(FAST_AUX_CLK, WORK_CLK, &CRG->FASTAUX_CLKDIV, 0, 0x3, s_div8Table),
    CRG_DIV_CLK(ADC_FUNC_CLK, WORK_CLK, &CRG->ADCFUNC_CLKDIV, 0, 0x3, s_div8Table),
    CRG_DIV_CLK(DEB_CLK, SROSC_CLK, &CRG->DEBOUNCE_CLKDIV, 0, 0x3, s_div8Table),
    CRG_DIV_CLK(PCTMR_FUNC_CLK, AON_CLK, &CRG->LP32K_CLKDIV, 0, 0x3, s_div8Table),
    CRG_DIV_CLK(CPU_RTC_CLK, WORK_CLK, &CRG->CPURTC_CLKDIV, 0, 0x3, s_div8Table),
};

static clock_gate_t s_gateClocks[] = {
    CRG_GATE_CLK(DEBUG_CLK, MAIN_BUS_CLK, &CRG->DEBUG_CLKEN, 0, 0x1),
    CRG_GATE_CLK(TPIU_CLK, MAIN_BUS_AUX_CLK, &CRG->DEBUG_CLKEN, 1, 0x1),
    CRG_GATE_CLK(TIPU_TRACE_CLK, SUPERTMR_FUNC_CLK, &CRG->DEBUG_CLKEN, 2, 0x1),
    CRG_GATE_CLK(FMC_CLK, MAIN_BUS_CLK, &CRG->MEMORY_CLKEN, 0, 0x1),
    CRG_GATE_CLK(ADC0_CLK, ADC_FUNC_CLK, &CRG->MOTOR_CLKEN, 0, 0x2),
    CRG_GATE_CLK(ADC1_CLK, ADC_FUNC_CLK, &CRG->MOTOR_CLKEN, 2, 0x2),
    CRG_GATE_CLK(PDU0_CLK, MAIN_BUS_MOTOR_CTRL_CLK, &CRG->MOTOR_CLKEN, 4, 0x1),
    CRG_GATE_CLK(PDU1_CLK, MAIN_BUS_MOTOR_CTRL_CLK, &CRG->MOTOR_CLKEN, 5, 0x1),
    CRG_GATE_CLK(CMP_CLK, MAIN_BUS_MOTOR_CTRL_CLK, &CRG->MOTOR_CLKEN, 6, 0x1),
    CRG_GATE_CLK(DAC_CLK, SUPERTMR_FUNC_CLK, &CRG->MOTOR_CLKEN, 7, 0x2),
    CRG_GATE_CLK(PWM0_CLK, MAIN_BUS_AUX_CLK, &CRG->MOTOR_CLKEN, 9, 0x1),
    CRG_GATE_CLK(PWM1_CLK, MAIN_BUS_AUX_CLK, &CRG->MOTOR_CLKEN, 10, 0x1),
    CRG_GATE_CLK(PWM2_CLK, MAIN_BUS_AUX_CLK, &CRG->MOTOR_CLKEN, 11, 0x1),
    CRG_GATE_CLK(PWM3_CLK, MAIN_BUS_AUX_CLK, &CRG->MOTOR_CLKEN, 12, 0x1),
    CRG_GATE_CLK(PWM4_CLK, MAIN_BUS_AUX_CLK, &CRG->MOTOR_CLKEN, 13, 0x1),
    CRG_GATE_CLK(SUPERTMR0_CLK, SUPERTMR0_SELCLK, &CRG->MOTOR_CLKEN, 14, 0x1),
    CRG_GATE_CLK(SUPERTMR1_CLK, SUPERTMR1_SELCLK, &CRG->MOTOR_CLKEN, 15, 0x1),
    CRG_GATE_CLK(SUPERTMR2_CLK, SUPERTMR2_SELCLK, &CRG->MOTOR_CLKEN, 16, 0x1),
    CRG_GATE_CLK(SUPERTMR3_CLK, SUPERTMR3_SELCLK, &CRG->MOTOR_CLKEN, 17, 0x1),
    CRG_GATE_CLK(SUPERTMR4_CLK, SUPERTMR4_SELCLK, &CRG->MOTOR_CLKEN, 18, 0x1),
    CRG_GATE_CLK(SUPERTMR5_CLK, SUPERTMR5_SELCLK, &CRG->MOTOR_CLKEN, 19, 0x1),
    CRG_GATE_CLK(PITMR0_CLK, MAIN_BUS_MOTOR_CTRL_CLK, &CRG->MOTOR_CLKEN, 20, 0x2),
    CRG_GATE_CLK(PCTMR0_CLK, FAST_AUX_CLK, &CRG->MOTOR_CLKEN, 22, 0x1),
    CRG_GATE_CLK(PCTMR1_CLK, FAST_AUX_CLK, &CRG->MOTOR_CLKEN, 23, 0x1),
    CRG_GATE_CLK(PCTMR2_CLK, FAST_AUX_CLK, &CRG->MOTOR_CLKEN, 24, 0x1),
    CRG_GATE_CLK(PCTMR3_CLK, FAST_AUX_CLK, &CRG->MOTOR_CLKEN, 25, 0x1),
    CRG_GATE_CLK(PCTMR4_CLK, FAST_AUX_CLK, &CRG->MOTOR_CLKEN, 26, 0x1),
    CRG_GATE_CLK(PCTMR5_CLK, FAST_AUX_CLK, &CRG->MOTOR_CLKEN, 27, 0x1),
    CRG_GATE_CLK(PCTMR6_CLK, FAST_AUX_CLK, &CRG->MOTOR_CLKEN, 28, 0x1),
    CRG_GATE_CLK(PCTMR7_CLK, FAST_AUX_CLK, &CRG->MOTOR_CLKEN, 29, 0x1),
    CRG_GATE_CLK(PCTMR8_CLK, FAST_AUX_CLK, &CRG->MOTOR_CLKEN, 30, 0x1),
    CRG_GATE_CLK(EWM_CLK, AON_CLK, &CRG->MOTOR_CLKEN, 31, 0x1),
    CRG_GATE_CLK(CRC_CLK, MAIN_BUS_AUX_CLK, &CRG->OCC_CLKEN, 0, 0x1),
    CRG_GATE_CLK(TSENSOR_LV_GATE_CLK, TSENSOR_LV_CLK, &CRG->OCC_CLKEN, 2, 0x1),
    CRG_GATE_CLK(INTC_CLK, MAIN_BUS_AUX_CLK, &CRG->OCC_CLKEN, 4, 0x1),
    CRG_GATE_CLK(TRACE_PCLK, MAIN_BUS_AUX_CLK, &CRG->OCC_CLKEN, 5, 0x1),
    CRG_GATE_CLK(IOPMP_CLK, MAIN_BUS_AUX_CLK, &CRG->OCC_CLKEN, 6, 0x1),
    CRG_GATE_CLK(FRM_CLK, MAIN_BUS_AUX_CLK, &CRG->OCC_CLKEN, 8, 0x1),
    CRG_GATE_CLK(UART0_CLK, PERIPH_BUS_PLL_AUX_FUNC_CLK, &CRG->COM_CLKEN, 0, 0x2),
    CRG_GATE_CLK(UART1_CLK, PERIPH_BUS_PLL_AUX_FUNC_CLK, &CRG->COM_CLKEN, 2, 0x2),
    CRG_GATE_CLK(UART2_CLK, PERIPH_BUS_PLL_AUX_FUNC_CLK, &CRG->COM_CLKEN, 4, 0x2),
    CRG_GATE_CLK(UART3_CLK, PERIPH_BUS_PLL_AUX_FUNC_CLK, &CRG->COM_CLKEN, 6, 0x2),
    CRG_GATE_CLK(UART4_CLK, PERIPH_BUS_PLL_AUX_FUNC_CLK, &CRG->COM_CLKEN, 8, 0x2),
    CRG_GATE_CLK(UART5_CLK, PERIPH_BUS_PLL_AUX_FUNC_CLK, &CRG->COM_CLKEN, 10, 0x2),
    CRG_GATE_CLK(I2C0_CLK, PERIPH_BUS_PLL_AUX_FUNC_CLK, &CRG->COM_CLKEN, 12, 0x2),
    CRG_GATE_CLK(I2C1_CLK, PERIPH_BUS_PLL_AUX_FUNC_CLK, &CRG->COM_CLKEN, 14, 0x2),
    CRG_GATE_CLK(SUPERIO0_CLK, SUPERIO_CLK, &CRG->COM_CLKEN, 16, 0x2),
    CRG_GATE_CLK(SPI0_CLK, PERIPH_BUS_PLL_FUNC_CLK, &CRG->SPI_CLKEN, 0, 0x2),
    CRG_GATE_CLK(SPI1_CLK, PERIPH_BUS_PLL_FUNC_CLK, &CRG->SPI_CLKEN, 2, 0x2),
    CRG_GATE_CLK(SPI2_CLK, PERIPH_BUS_PLL_FUNC_CLK, &CRG->SPI_CLKEN, 4, 0x2),
    CRG_GATE_CLK(SPI3_CLK, PERIPH_BUS_PLL_FUNC_CLK, &CRG->SPI_CLKEN, 6, 0x2),
    CRG_GATE_CLK(CAN0_CLK, PERIPH_BUS_CLK, &CRG->CAN_CLKEN, 0, 0x3),
    CRG_GATE_CLK(CAN1_CLK, PERIPH_BUS_CLK, &CRG->CAN_CLKEN, 3, 0x2),
    CRG_GATE_CLK(CAN2_CLK, PERIPH_BUS_CLK, &CRG->CAN_CLKEN, 6, 0x3),
    CRG_GATE_CLK(CAN3_CLK, PERIPH_BUS_CLK, &CRG->CAN_CLKEN, 12, 0x3),
    CRG_GATE_CLK(GTMR_CLK, GTMR_FUNC_CLK, &CRG->GTMR_CLKEN, 0, 0x2),
    CRG_GATE_CLK(PITMR1_CLK, MAIN_BUS_MOTOR_CTRL_CLK, &CRG->PITMR1_CLKEN, 0, 0x2),
    CRG_GATE_CLK(PDMA_CLK, PERIPH_BUS_CLK, &CRG->PITMR1_CLKEN, 2, 0x1),
    CRG_GATE_CLK(TRGMUX_CLK, PERIPH_BUS_CLK, &CRG->PITMR1_CLKEN, 3, 0x1),
};

extern status_t FMC_DRV_UpdateClockMode(void);

void SystemCoreClockUpdate(void)
{
    uint32_t clkFreq;
    if (CLOCK_SYS_GetFreq(CPU_CORE_CLK, &clkFreq) == STATUS_SUCCESS) {
        SystemCoreClock = clkFreq;
    }

    if (CLOCK_SYS_GetFreq(CPU_RTC_CLK, &clkFreq) == STATUS_SUCCESS) {
        SystemTimerClock = clkFreq;
    }
}

__TIM_CODE status_t CLOCK_SYS_SetSpllSrcClkConfiguration(const crg_mux_config_t *muxConfig)
{
    uint32_t regval;
    status_t status = STATUS_SUCCESS;

    for (uint32_t i = 0; i < muxConfig->count && i < ARRAY_SIZE(s_muxClocks); i++) {
        if (muxConfig->muxClocks[i].clkName == SPLL_SRC_CLK) {
            clock_mux_t *muxClk = (clock_mux_t *)g_stCrgClocks[muxConfig->muxClocks[i].clkName];
            regval              = readl(muxClk->reg) & (~toClockMask(muxClk->shift, muxClk->width));
            regval              = (regval | (muxConfig->muxClocks[i].clkSel << muxClk->shift));
            writel(regval, muxClk->reg);
            muxClk->core.parent = s_muxClocks[i].parents[muxConfig->muxClocks[i].clkSel];

            return status;
        }
    }

    return status;
}

/*
 *
 * Function Name : CLOCK_SYS_SetSpllConfiguration
 * Description   : This function configures the spll clock
 *
 */
__TIM_CODE status_t CLOCK_SYS_SetSpllConfiguration(const crg_spll_config_t *spllConfig, const crg_mux_config_t *muxConfig)
{
    uint8_t pamask;
    uint32_t timeout;
    status_t status = STATUS_SUCCESS;

    crg_workclk_sel_t clkoutSel = (crg_workclk_sel_t)CRG_GetWorkClockSel(CRG);
    if (clkoutSel == CRG_WORK_SRC_SPLL_CLK) {
        CRG_SetWorkClockSel(CRG, CRG_WORK_SRC_FAST_CLK);
    }

    /* 1. Power down  */
    CRG_SetSpllPowerCtrl(CRG, CRG_SPLL_PD_MASK);
    /* 2. Configure spll clock source */
    //CRG_SetSpllClockSel(CRG, spllConfig->refClockSel);
    CLOCK_SYS_SetSpllSrcClkConfiguration(muxConfig);
    /* 3. Configure differential output clock */
    CRG_SetSpllDifferenceClko(CRG, spllConfig->refDiv, spllConfig->divSetInteger, spllConfig->divSetFractional);
    /* 4. Configure post divider 0 output clock */
    CRG_SetSpllPostClko(CRG, spllConfig->posDiv0, 0, 0);
    /* 5. Power up core function, post divider 0 and bias function */
    CRG_SetSpllPowerCtrl(CRG, CRG_SPLL_PD_MASK & (~(1 << CRG_SPLL_PD_BIASFUNC)));
    /* 6. Power up core function, post divider 0 and bias function */
    pamask = (1 << CRG_SPLL_PD_RESERVED) | (1 << CRG_SPLL_PD_POSDIV1) | (1 << CRG_SPLL_PD_POSDIV2);
    CRG_SetSpllPowerCtrl(CRG, pamask);

    timeout = SPLL_STABILIZATION_TIMEOUT;
    while ((CRG_GetSpllLocked(CRG) == false) && (timeout > 0U)) {
        timeout--;
    }

    if (timeout == 0U) {
        status = STATUS_TIMEOUT;
    }
    if ((status == STATUS_SUCCESS) && (clkoutSel == CRG_WORK_SRC_SPLL_CLK)) {
        CRG_SetWorkClockSel(CRG, CRG_WORK_SRC_SPLL_CLK);
    }

    return status;
}

/*
 *
 * Function Name : CLOCK_SYS_SetSoscConfiguration
 * Description   : This function configures the sosc clock
 *
 */
__TIM_CODE status_t CLOCK_SYS_SetSoscConfiguration(const crg_sosc_config_t *soscConfig)
{
    uint8_t xtalPyp = 0;
    status_t status = STATUS_SUCCESS;

    if (soscConfig == NULL) {
        return STATUS_ERRORPAR;
    }

    crg_workclk_sel_t clkoutSel  = (crg_workclk_sel_t)CRG_GetWorkClockSel(CRG);
    crg_pll_refclk_sel_t spllSel = (crg_pll_refclk_sel_t)CRG_GetSpllClockSel(CRG);
    if ((clkoutSel == CRG_WORK_SRC_SPLL_CLK) && (spllSel == CRG_SPLL_SRC_SOSC_CLK)) {
        CRG_SetWorkClockSel(CRG, CRG_WORK_SRC_FAST_CLK);
    }

    if (soscConfig->extRef == CRG_SOSC_REF_EXT) {
        xtalPyp = 1;
    }

    s_extClocks[EXT_CLK_SOSC].clockRate = soscConfig->freq;
    XTAL->xtal                          = PAD_XRAL_ACG(soscConfig->xtalAGC) | PAD_XRAL_BYP(xtalPyp) |
                 PAD_XRAL_XTALMF(soscConfig->current) | PAD_XRAL_ACG(soscConfig->xtalAGC) |
                 PAD_XRAL_AGC_BYPASS(soscConfig->xtalAGCBypass) | PAD_XRAL_RF(soscConfig->resistor);

    XTAL->xtal |= soscConfig->enable ? PAD_XRAL_EN(1) : PAD_XRAL_EN(0);

    if ((clkoutSel == CRG_WORK_SRC_SPLL_CLK) && (spllSel == CRG_SPLL_SRC_SOSC_CLK)) {
        CRG_SetWorkClockSel(CRG, clkoutSel);
    }

    return status;
}

#if 0
static bool CRG_GetSroscBusyStatus(const crg_type_t *const base)
{
    uint32_t clkoutSel = CRG_GetWorkClockSel(base);
    if (clkoutSel != CRG_WORK_SRC_LPCK_CLK) {
        return false;
    }

    clkoutSel = CRG_GetLpckClockSel(base);
    if (clkoutSel == CRG_LPCK_SRC_SROSC_CLK) {
        return true;
    }

    return false;
}

static status_t CLOCK_SYS_SetSroscConfiguration(const crg_rcosc_config_t *srcoscConfig)
{
    bool clkStatus;
    uint32_t timeout;
    status_t status = STATUS_SUCCESS;

    clkStatus = CRG_GetSroscBusyStatus(CRG);
    if (clkStatus) {
        CRG_SetWorkClockSel(CRG, CRG_WORK_SRC_SOSC_CLK);
    }

    CRG_SetSrcoscEnableState(CRG, false);
    CRG_SetSrcoscOutputFreq(CRG, srcoscConfig->oscclkSel, srcoscConfig->fmarginSel);
    if (srcoscConfig->autoTrimming) {

    } else {
        CRG_SetSrcoscFreqCtune(CRG, srcoscConfig->freqCtune);
        CRG_SetSrcoscFreqFtune(CRG, srcoscConfig->freqFtune);
    }

    CRG_SetSrcoscEnableState(CRG, true);
    timeout = SROSC_STABILIZATION_TIMEOUT;
    while ((CRG_GetSrcoscClkStableStatus(CRG) == false) && (timeout > 0U)) {
        timeout--;
    }

    if (timeout == 0U) {
        status = STATUS_TIMEOUT;
    }
    if (status == STATUS_SUCCESS && clkStatus) {
        CRG_SetWorkClockSel(CRG, CRG_WORK_SRC_LPCK_CLK);
    }

    return status;
}

static bool CRG_GetFroscBusyStatus(const crg_type_t *const base, uint32_t *workClkSel)
{
    uint32_t clkoutSel = CRG_GetWorkClockSel(base);
    if (workClkSel != NULL) {
        *workClkSel = clkoutSel;
    }

    if (clkoutSel == CRG_WORK_SRC_FAST_CLK) {
        return true;
    } else if (clkoutSel == CRG_WORK_SRC_SPLL_CLK) {
        clkoutSel = CRG_GetSpllClockSel(base);
        if (clkoutSel == CRG_SPLL_SRC_FROSC_CLK) {
            return true;
        }
    }

    return false;
}

static status_t CLOCK_SYS_SetFroscConfiguration(const crg_rcosc_config_t *frcoscConfig)
{
    bool clkStatus;
    uint32_t timeout;
    uint32_t clkoutSel;
    status_t status = STATUS_SUCCESS;

    clkStatus = CRG_GetFroscBusyStatus(CRG, &clkoutSel);
    if (clkStatus) {
        CRG_SetWorkClockSel(CRG, CRG_WORK_SRC_SOSC_CLK);
    }

    CRG_SetFrcoscEnableState(CRG, false);
    CRG_SetFrcoscOutputFreq(CRG, frcoscConfig->oscclkSel, frcoscConfig->fmarginSel);
    if (frcoscConfig->autoTrimming) {

    } else {
        CRG_SetFrcoscFreqCtune(CRG, frcoscConfig->freqCtune);
        CRG_SetFrcoscFreqFtune(CRG, frcoscConfig->freqFtune);
    }

    CRG_SetFrcoscEnableState(CRG, frcoscConfig->enable);
    timeout = FROSC_STABILIZATION_TIMEOUT;
    while ((CRG_GetFrcoscClkStableStatus(CRG) == false) && (timeout > 0U)) {
        timeout--;
    }

    if (timeout == 0U) {
        status = STATUS_TIMEOUT;
    }
    if (status == STATUS_SUCCESS && clkStatus) {
        CRG_SetWorkClockSel(CRG, clkoutSel);
    }

    return status;
}
#endif

/*
 *
 * Function Name : CLOCK_SYS_SetTckConfiguration
 * Description   : This function configures the rtc clock
 *
 */
#if 0
static status_t CLOCK_SYS_SetRtcConfiguration(const crg_rtc_config_t *rtcConfig)
{
    status_t status = STATUS_SUCCESS;

    CRG_SetRtcClockSel(PMU, rtcConfig->rtcClkSel);

    for (uint32_t i = 0; i < ARRAY_SIZE(s_fixedRateClocks); i++) {
        if (s_fixedRateClocks[i].core.name == RTCOSC_CLK) {
            s_fixedRateClocks[i].clockRate = rtcConfig->rtcClkInFreq;
        }
    }

    return status;
}
#endif

/*
 *
 * Function Name : CLOCK_SYS_SetTckConfiguration
 * Description   : This function configures the tck clocks
 *
 */
__TIM_CODE static status_t CLOCK_SYS_SetTckConfiguration(const crg_extclk_config_t *tclkConfig)
{
    status_t status = STATUS_SUCCESS;

    s_extClocks[EXT_CLK_RTCOSC].clockRate = tclkConfig->rtcoscFreq;
    for (uint32_t i = 0; i < NUMBER_OF_TCLK_INPUTS; i++) {
        s_extClocks[EXT_CLK_TCLK0 + i].clockRate = tclkConfig->tclkFreq[i];
    }

    return status;
}

/*
 *
 * Function Name : CLOCK_SYS_SetMuxClkConfiguration
 * Description   : This function configures the mux clocks
 *
 */
__TIM_CODE status_t CLOCK_SYS_SetMuxClkConfiguration(const crg_mux_config_t *muxConfig)
{
    uint32_t regval;
    status_t status = STATUS_SUCCESS;

    for (uint32_t i = 0; i < muxConfig->count && i < ARRAY_SIZE(s_muxClocks); i++) {
        if (muxConfig->muxClocks[i].clkName == SPLL_SRC_CLK) {
            continue;
        }
        clock_mux_t *muxClk = (clock_mux_t *)g_stCrgClocks[muxConfig->muxClocks[i].clkName];
        regval              = readl(muxClk->reg) & (~toClockMask(muxClk->shift, muxClk->width));
        regval              = (regval | (muxConfig->muxClocks[i].clkSel << muxClk->shift));
        writel(regval, muxClk->reg);
    }
    for (uint32_t i = 0; i < ARRAY_SIZE(s_muxClocks); i++) {
        clock_mux_t *muxClk = &s_muxClocks[i];
        regval              = readl(muxClk->reg) & (toClockMask(muxClk->shift, muxClk->width));
        muxClk->core.parent = muxClk->parents[regval >> muxClk->shift];
    }

    return status;
}

__TIM_CODE status_t CLOCK_SYS_SetClkDivider(clock_names_t name, crg_clock_divider_t divider)
{
    uint32_t regval;
    status_t status = STATUS_SUCCESS;

    clock_divider_t *divClk = (clock_divider_t *)g_stCrgClocks[name];
    regval                  = readl(divClk->reg) & (~toClockMask(divClk->shift, divClk->width));
    regval                  = (regval | (divider << divClk->shift));
    writel(regval, divClk->reg);
    return status;
}

/*
 *
 * Function Name : CLOCK_SYS_SetDivClkConfiguration
 * Description   : This function configures the divider clocks
 *
 */
__TIM_CODE status_t CLOCK_SYS_SetDivClkConfiguration(const crg_div_config_t *divConfig)
{
    status_t status = STATUS_SUCCESS;

    for (uint32_t i = 0; i < divConfig->count; i++) {
        status = CLOCK_SYS_SetClkDivider(divConfig->divClocks[i].clkName, (crg_clock_divider_t)divConfig->divClocks[i].clkDiv);
        if (status != STATUS_SUCCESS) {
            break;
        }
    }

    return status;
}

/*
 * Function Name : CLOCK_SYS_SetGateClkConfiguration
 * Description   : This function returns the clock's parent clock
 *
 */
#if 0
static status_t CLOCK_SYS_SetGateClkConfiguration(const crg_gate_config_t *gateConfig)
{
    uint32_t regval;
    status_t status = STATUS_SUCCESS;

    for (uint32_t i = 0; i < gateConfig->count; i++) {
        clock_gate_t *gateClk = (clock_gate_t *)g_stCrgClocks[gateConfig->gateClocks[i].clkName];
        regval                = readl(gateClk->reg) & (~toClockMask(gateClk->shift, gateClk->width));
        if (gateConfig->gateClocks[i].enable) {
            regval = regval | toClockMask(gateClk->shift, gateClk->width);
        }
        writel(regval, gateClk->reg);
    }

    return status;
}
#endif

/*
 * Function Name : CLOCK_SYS_GetParent
 * Description   : This function returns the clock's parent clock
 *
 */
__TIM_CODE status_t CLOCK_SYS_GetParent(clock_names_t clkName, clock_names_t *parentClkName)
{
    status_t status = STATUS_SUCCESS;

    if (clkName >= CLOCK_NAME_COUNT) {
        return STATUS_ERRORPAR;
    }

    clock_core_t *coreClk = (clock_core_t *)g_stCrgClocks[clkName];
    *parentClkName        = coreClk->parent;

    return status;
}

/*
 * Function Name : CLOCK_SYS_SetParent
 * Description   : This function sets the clock's parent clock
 *
 */
__TIM_CODE status_t CLOCK_SYS_SetParent(clock_names_t clkName, clock_names_t parentClkName)
{
    uint32_t regval;
    status_t status = STATUS_ERROR;

    if (clkName >= CLOCK_NAME_COUNT) {
        return STATUS_ERRORPAR;
    }

    clock_core_t *coreClk = (clock_core_t *)g_stCrgClocks[clkName];
    if (coreClk->flags != CRG_MUX_CLOCK) {
        return STATUS_ERRORPAR;
    }

    clock_mux_t *muxClk = (clock_mux_t *)coreClk;
    for (uint32_t i = 0; i < muxClk->parents_count; i++) {
        if (muxClk->parents[i] == parentClkName) {
            regval = readl(muxClk->reg) & (~toClockMask(muxClk->shift, muxClk->width));
            regval = (regval | (i << muxClk->shift));
            writel(regval, muxClk->reg);
            coreClk->parent = parentClkName;
            return STATUS_SUCCESS;
        }
    }

    return status;
}

/*
 *
 * Function Name : CLOCK_SYS_Init
 * Description   : Install pre-defined clock configurations.
 * This function installs the pre-defined clock configuration table to the
 * clock manager.
 *
 */
__TIM_CODE status_t CLOCK_DRV_Init(clock_user_config_t const *config)
{
    status_t result;

    /* Register tclk clocks */
    for (uint32_t i = 0; i < ARRAY_SIZE(s_extClocks); i++) {
        g_stCrgClocks[s_extClocks[i].core.name] = &s_extClocks[i].core;
    }

#if 0
    /* Register tclk clocks */
    for (uint32_t i = 0; i < ARRAY_SIZE(s_rcoscClocks); i++) {
        g_stCrgClocks[s_rcoscClocks[i].core.name] = &s_rcoscClocks[i].core;
        if (s_rcoscClocks[i].core.name == SROSC_CLK) {
            s_rcoscClocks[i].rcosc = &config->srcoscConfig;
        } else if (s_rcoscClocks[i].core.name == FROSC_CLK) {
            s_rcoscClocks[i].rcosc = &config->frcoscConfig;
        }
    }
#endif

    /* Register fixed rate clocks */
    for (uint32_t i = 0; i < ARRAY_SIZE(s_fixedRateClocks); i++) {
        g_stCrgClocks[s_fixedRateClocks[i].core.name] = &s_fixedRateClocks[i].core;
    }

    /* Register spll clocks */
    for (uint32_t i = 0; i < ARRAY_SIZE(s_spllClocks); i++) {
        g_stCrgClocks[s_spllClocks[i].core.name] = &s_spllClocks[i].core;
    }

    /* Register mux clocks */
    for (uint32_t i = 0; i < ARRAY_SIZE(s_muxClocks); i++) {
        g_stCrgClocks[s_muxClocks[i].core.name] = &s_muxClocks[i].core;
    }

    /* Register fixed factor clocks */
    for (uint32_t i = 0; i < ARRAY_SIZE(s_fixedFactorClocks); i++) {
        g_stCrgClocks[s_fixedFactorClocks[i].core.name] = &s_fixedFactorClocks[i].core;
    }

    /* Register div clocks */
    for (uint32_t i = 0; i < ARRAY_SIZE(s_divClocks); i++) {
        g_stCrgClocks[s_divClocks[i].core.name] = &s_divClocks[i].core;
    }

    for (uint32_t i = 0; i < ARRAY_SIZE(s_gateClocks); i++) {
        g_stCrgClocks[s_gateClocks[i].core.name] = &s_gateClocks[i].core;
    }

#if 0
    result = CLOCK_SYS_SetSroscConfiguration(&config->srcoscConfig);
    if (result != STATUS_SUCCESS) {
        return result;
    }

    result = CLOCK_SYS_SetFroscConfiguration(&config->frcoscConfig);
    if (result != STATUS_SUCCESS) {
        return result;
    }
#endif
    FMC_DRV_SetClockMode(FMC_CLOCKSOURCE_160M);
    result = CLOCK_SYS_SetSoscConfiguration(&config->soscConfig);
    if (result != STATUS_SUCCESS) {
        return result;
    }

    result = CLOCK_SYS_SetSpllConfiguration(&config->spllConfig, &config->muxConfig);
    if (result != STATUS_SUCCESS) {
        return result;
    }

#if 0
    result = CLOCK_SYS_SetRtcConfiguration(&config->rtcConfig);
    if (result != STATUS_SUCCESS) {
        return result;
    }
#endif

    result = CLOCK_SYS_SetTckConfiguration(&config->extClkConfig);
    if (result != STATUS_SUCCESS) {
        return result;
    }

    CLOCK_SYS_SetMuxClkConfiguration(&config->muxConfig);
    CLOCK_SYS_SetDivClkConfiguration(&config->divConfig);
    FMC_DRV_UpdateClockMode();
    //CLOCK_SYS_SetGateClkConfiguration(&config->gateConfig);
    CRG_SetIpgClkEnable(CRG);
    CRG_SetSubsysOccClkEnable(CRG);
    CRG_SetPeripheralsRest(CRG);

    SystemCoreClockUpdate();

    return result;
}

/*
 *
 * Function Name : CLOCK_SYS_Init
 * Description   : Wrapper over CLOCK_DRV_Init function. It's part of the old API.
 *
 */
__TIM_CODE status_t CLOCK_SYS_Init(clock_user_config_t const *config)
{
    return CLOCK_DRV_Init(config);
}


/*
 * Function Name : CLOCK_SYS_GetClkDivider
 * Description   : This function sets the clock division factor
 *
 */
__TIM_CODE status_t CLOCK_SYS_GetClkDivider(clock_names_t clockName, uint32_t *clkDiv)
{
    uint32_t regval;
    const clk_div_table_t *clkt;
    status_t status = STATUS_ERROR;

    clock_divider_t *divClk = (clock_divider_t *)g_stCrgClocks[clockName];
    regval                  = readl(divClk->reg) & (toClockMask(divClk->shift, divClk->width));
    regval                  = (regval >> divClk->shift);

    for (clkt = divClk->table; clkt->div; clkt++) {
        if (clkt->val == regval) {
            *clkDiv = clkt->div;
            status  = STATUS_SUCCESS;
        }
    }

    return status;
}

#if 0
status_t CLOCK_SYS_GetRcoscFreq(clock_names_t clockName,
                           uint32_t * frequency)
{
    //clock_rcosc_t *rcoscClk = &s_rcoscClocks[clockName];

    if (clockName == FROSC_CLK) {
        *frequency = 48000000;
    } else if (clockName == SROSC_CLK) {
        *frequency = 8000000;
    }

    return STATUS_SUCCESS;
}
#endif

/*
 * Function Name : CLOCK_SYS_GetSpllFreq
 * Description   : This function returns the frequency of spll
 *
 */
__TIM_CODE status_t CLOCK_SYS_GetSpllFreq(clock_names_t clockName, uint32_t parentFreq, uint32_t *frequency)
{
    (void)clockName;
    *frequency = CRG_GetSpllCko0Freq(CRG, parentFreq);

    return STATUS_SUCCESS;
}

/*
 * Function Name : CLOCK_DRV_GetFreq
 * Description   : This function returns the frequency of a given clock
 *
 */
#if 0
__TIM_CODE status_t CLOCK_DRV_GetFreq(clock_names_t clockName, uint32_t *frequency)
{

    uint32_t clkDiv;
    clock_extclk_t *tclk;
    uint32_t parentFreq;
    uint32_t _frequency;
    clock_fixed_rate_t *fixedClk;
    clock_fixed_factor_t *fixFactor;
    status_t status = STATUS_SUCCESS;

    if (frequency == NULL) {
        frequency = &_frequency;
    }

    clock_core_t *coreClk = g_stCrgClocks[clockName];
    switch (coreClk->flags) {
    case CRG_FIXED_RATE_CLOCK:
        fixedClk   = (clock_fixed_rate_t *)coreClk;
        *frequency = fixedClk->clockRate;
        break;
    case CRG_EXT_CLOCK:
        tclk       = (clock_extclk_t *)coreClk;
        *frequency = tclk->clockRate;
        break;
    case CRG_FIXED_FACTOR_CLOCK:
        fixFactor = (clock_fixed_factor_t *)coreClk;
        status    = CLOCK_DRV_GetFreq(coreClk->parent, &parentFreq);
        if (status == STATUS_SUCCESS) {
            *frequency = parentFreq * fixFactor->fixedMult / fixFactor->fixedFiv;
        }
        break;
    case CRG_DIV_CLOCK:
        status = CLOCK_DRV_GetFreq(coreClk->parent, &parentFreq);
        if (status == STATUS_SUCCESS) {
            CLOCK_SYS_GetClkDivider(coreClk->name, &clkDiv);
            *frequency = parentFreq / clkDiv;
        }
        break;
    case CRG_GATE_CLOCK:
    case CRG_MUX_CLOCK:
        status = CLOCK_DRV_GetFreq(coreClk->parent, &parentFreq);
        if (status == STATUS_SUCCESS) {
            if (clockName == SPLL_CLK) {
                status = CLOCK_SYS_GetSpllFreq(clockName, parentFreq, frequency);
            } else {
                *frequency = parentFreq;
            }
        }
        break;
#if 0
    case CRG_RCOSC_CLOCK:
        status = CLOCK_SYS_GetRcoscFreq(clockName, frequency);
        break;
#endif
    default:
        break;
    }

    return status;
}
#else
__TIM_CODE status_t CLOCK_DRV_GetFreq(clock_names_t clockName, uint32_t *frequency)
{

    uint32_t clkDiv;
    clock_extclk_t *tclk;
    uint32_t parentFreq;
    uint32_t _frequency;
    // clock_spll_t *spllClk;
    clock_fixed_rate_t *fixedClk;
    clock_fixed_factor_t *fixFactor;
    status_t status = STATUS_SUCCESS;

    if (frequency == NULL) {
        frequency = &_frequency;
    }

    clock_core_t *coreClk = g_stCrgClocks[clockName];
    if (coreClk->flags == CRG_FIXED_RATE_CLOCK) {
        fixedClk = (clock_fixed_rate_t *)coreClk;
        *frequency = fixedClk->clockRate;
    } else if (coreClk->flags == CRG_EXT_CLOCK) {
        tclk = (clock_extclk_t *)coreClk;
        *frequency = tclk->clockRate;
    } else if (coreClk->flags == CRG_FIXED_FACTOR_CLOCK) {
        fixFactor = (clock_fixed_factor_t *)coreClk;
        status = CLOCK_DRV_GetFreq(coreClk->parent, &parentFreq);
        if (status == STATUS_SUCCESS) {
            *frequency = parentFreq * fixFactor->fixedMult / fixFactor->fixedFiv;
        }
    } else if (coreClk->flags == CRG_SPLL_CLOCK) {
        status = CLOCK_DRV_GetFreq(coreClk->parent, &parentFreq);
        if (status == STATUS_SUCCESS) {
            status = CLOCK_SYS_GetSpllFreq(clockName, parentFreq, frequency);
        }
    } else if (coreClk->flags == CRG_DIV_CLOCK) {
        status = CLOCK_DRV_GetFreq(coreClk->parent, &parentFreq);
        if (status == STATUS_SUCCESS) {
            CLOCK_SYS_GetClkDivider(coreClk->name, &clkDiv);
            *frequency = parentFreq / clkDiv;
        }
    } else if ((coreClk->flags == CRG_GATE_CLOCK) || (coreClk->flags == CRG_MUX_CLOCK)) {
        status = CLOCK_DRV_GetFreq(coreClk->parent, &parentFreq);
        if (status == STATUS_SUCCESS) {
            *frequency = parentFreq;
        }
    }
    return status;
}
#endif

/*
 *
 * Function Name : CLOCK_SYS_GetFreq
 * Description   : Wrapper over CLOCK_DRV_GetFreq function. It's part of the old API.
 *
 */
__TIM_CODE status_t CLOCK_SYS_GetFreq(clock_names_t clockName, uint32_t *frequency)
{
#ifdef EMPS_FPGA
    if (clockName == GTMR_FUNC_CLK) {
        *frequency = 1000000;
    } else {
        *frequency = 20000000;
    }

    return STATUS_SUCCESS;
#else
    return CLOCK_DRV_GetFreq(clockName, frequency);
#endif
}

status_t CLOCK_DRV_ClockEnable(clock_names_t clockName, bool enable)
{
    uint32_t regval;
    status_t status = STATUS_ERROR;

    for (uint32_t i = 0; i < sizeof(s_gateClocks) / sizeof(clock_gate_t); i++) {
        if (clockName == s_gateClocks[i].core.name) {
            clock_gate_t *gateClk = &s_gateClocks[i];
            regval                = readl(gateClk->reg) & (~toClockMask(gateClk->shift, gateClk->width));
            if (enable) {
                regval = regval | toClockMask(gateClk->shift, gateClk->width);
            }
            writel(regval, gateClk->reg);
            status = STATUS_SUCCESS;
            break;
        }
    }
    return status;
}

status_t CLOCK_SYS_ClockEnable(clock_names_t clockName, bool enable)
{
#ifndef EMPS_FPGA
    return CLOCK_DRV_ClockEnable(clockName, enable);
#endif
}
