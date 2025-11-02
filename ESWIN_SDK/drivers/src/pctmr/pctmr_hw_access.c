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
 * @file pctmr_hw_access.c
 * @brief PCTMR driver hardware access source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-12
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "pctmr_hw_access.h"

/*
 *
 * Function Name : PCTMR_Reset.
 * Description   : This function configures all registers of the PCTMR instance to reset value.
 *
 */
void PCTMR_Reset(pctmr_type_t *const base)
{
    OS_ASSERT(base != NULL);

    /* First, disable the module so we can write the registers */
    uint32_t tmp = base->CSR;
    tmp &= ~(PCTMR_CSR_TEN_MASK | PCTMR_CSR_TCF_MASK);
    tmp |= PCTMR_CSR_TEN(0U);
    base->CSR = tmp;

    base->CSR = PCTMR_CSR_TEN(0U) | PCTMR_CSR_TMS(0U) | PCTMR_CSR_TFC(0U) | PCTMR_CSR_TPP(0U) | PCTMR_CSR_TPS(0U) |
                PCTMR_CSR_TIE(0U) | PCTMR_CSR_TCF(0U) | PCTMR_CSR_TDRE(0U);

    base->PSR = PCTMR_PSR_PCS(0U) | PCTMR_PSR_PBYP(0U) | PCTMR_PSR_PRESCALE(0U);

    base->CMR = PCTMR_CMR_COMPARE(0U);
}

/*
 *
 * Function Name : PCTMR_GetClkFreq.
 * Description   : Get the clock frequency for the selected clock source. If the
 * selected clock source is not enabled, a frequency of 0 is returned.
 *
 */
uint32_t PCTMR_GetClkFreq(const pctmr_clocksource_t clkSrc, const uint32_t instance)
{
    /* PCTMR PCC clock source names, for getting the input clock frequency */
    static const clock_names_t pctmrPccClockName[PCTMR_INSTANCE_COUNT] = FEATURE_PCTMR_CLOCKS_NAMES;
    clock_names_t inputClockName                                       = SROSC_CLK;
    uint32_t clkFreq;
    status_t clkStatus;

    /* Get input clock name */
    switch (clkSrc) {
    case PCTMR_CLOCKSOURCE_SROSC:
        inputClockName = SROSC_CLK;
        break;
    case PCTMR_CLOCKSOURCE_1KHZ_LPO:
        inputClockName = PCTMR_FUNC_CLK;
        break;
    case PCTMR_CLOCKSOURCE_AON:
        inputClockName = AON_CLK;
        break;
    case PCTMR_CLOCKSOURCE_ERCLK:
        inputClockName = pctmrPccClockName[instance];
        break;
    default:
        /* Invalid clock source */
        OS_ASSERT(false);
        break;
    }

    /* Get input clock frequency */
    if (inputClockName == SROSC_CLK) {
        clkStatus = CLOCK_SYS_GetFreq(SROSC_CLK, &clkFreq);
        OS_ASSERT(clkStatus == STATUS_SUCCESS);
        (void)clkStatus;
        OS_ASSERT(clkFreq != 0U); // If the GetFreq functions fails, clkFreq will be 0
    } else {
        clkStatus = CLOCK_SYS_GetFreq(inputClockName, &clkFreq);
        OS_ASSERT(clkStatus == STATUS_SUCCESS);
        (void)clkStatus;
        OS_ASSERT(clkFreq != 0U); // If the GetFreq functions fails, clkFreq will be 0
    }

    return clkFreq;
}

/*
 *
 * Function Name : PCTMR_Ticks2Us.
 * Description   : Transform timer ticks to microseconds using the given
 * prescaler settings. Clock frequency must be valid (different from 0).
 * @param  clkFreq system frequency
 * @param  pval    pctmr Prescaler Selection
 * @param  bypass  enable or disable by pass mode
 * @param  ticks   set count value
 * @param  us      the value after conversion
 */
bool PCTMR_Ticks2Us(const uint32_t clkFreq, const pctmr_prescaler_t pval, const bool bypass, const uint16_t ticks,
                    uint32_t *const us)
{
    bool success     = true;
    uint8_t p        = PCTMR_Cfg2P(pval, bypass);
    uint64_t nn      = ((uint64_t)ticks + 1U) << p;
    uint64_t us_real = (nn * 1000000U) / (clkFreq);
    uint32_t us_local;

    if (us_real <= (0xFFFFFFFFU)) {
        us_local = (uint32_t)us_real;
    } else {
        us_local = 0xFFFFFFFFU;
        success  = false;
    }

    *us = us_local;
    return success;
}

/*
 *
 * Function Name : PCTMR_ChooseClkConfig.
 * Description   :  Choose clocking configuration (prescaler value, timer ticks)
 * for the desired timeout period, given in microseconds. Input clock frequency,
 * clkFreq, must be greater than 0.
 * @param  clkFreq   System clock frequency
 * @param  us        Set timer count value
 * @param  prescval  Frequency division value
 * @param  bypass    bypass mode
 * @param  ticks     tick value
 */
bool PCTMR_ChooseClkConfig(const uint32_t clkFreq, const uint32_t us, pctmr_prescaler_t *const prescval,
                           bool *const bypass, uint16_t *const ticks)
{
    uint8_t p;
    uint64_t nticks = 0ULL;
    bool success;

    uint64_t nn = PCTMR_Us2Nn(clkFreq, us);

    /* Find the lowest prescaler value that allows the compare value in 16-bits */
    for (p = 0U; p <= PCTMR_MAX_PRESCALER; p++) {
        nticks = PCTMR_ComputeNticks(nn, p);

        if (nticks <= PCTMR_MAX_CMR_NTICKS) {
            /* Search finished, value will fit in the 16-bit register */
            break;
        }
    }

    success = PCTMR_Nticks2CompareTicks(nticks, ticks);

    /* Convert p to prescaler configuration */
    if (p == 0U) {
        /* Prescaler value of 1 */
        *bypass   = true;
        *prescval = PCTMR_PRESCALE_2;
    } else {
        *bypass = false;
        p--; // Decrement to match pctmr_prescaler_t.
        *prescval = (pctmr_prescaler_t)p;
    }

    return success;
}

/*
 *
 * Function Name : PCTMR_Us2Ticks.
 * Description   :  Transform microseconds to timer ticks using the given
 * prescaler settings. Input clock frequency, clkFreq, must be greater than 0.
 * @param  clkFreq  System clock frequency
 * @param  prescval Frequency division value
 * @param  bypass   enable or disable bypass mode
 * @param  us       value need to be converted
 * @param  ticks    converted value
 */
bool PCTMR_Us2Ticks(const uint32_t clkFreq, const pctmr_prescaler_t prescval, const bool bypass, const uint32_t us,
                    uint16_t *const ticks)
{
    bool success = true;
    /* Transform prescaler configuration to prescaler coefficient p */
    uint8_t p = PCTMR_Cfg2P(prescval, bypass);
    /* Compute nn, the number of ticks necessary for the period in microseconds
       without any prescaler */
    uint64_t nn = PCTMR_Us2Nn(clkFreq, us);
    /* Compute nticks, total number of ticks with prescaler */
    uint64_t nticks = PCTMR_ComputeNticks(nn, p);
    /* Transform nticks to value to be written to register */
    success = PCTMR_Nticks2CompareTicks(nticks, ticks);
    return success;
}
