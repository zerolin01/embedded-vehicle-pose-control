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
 * @file pctmr_hw_access.h
 * @brief  PCTMR driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-12
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __PCTMR_HW_ACCESS_H__
#define __PCTMR_HW_ACCESS_H__

#include "clock_driver.h"
#include "pctmr_driver.h"
#include "pctmr_register.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PCTMR_MAX_CMR_NTICKS (PCTMR_CMR_COMPARE_MASK + 1U)
#define PCTMR_MAX_PRESCALER  (1U << PCTMR_PSR_PRESCALE_WIDTH)

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Initialize the PCTMR instance to reset values.
 *
 * This function initializes all registers of the PCTMR instance to a known state (the register
 * are written with their reset values from the Reference Manual).
 *
 * @param  base PCTMR base pointer
 */
void PCTMR_Reset(pctmr_type_t *const base);

/**
 * @brief Get the dominant frequency value under different frequency mode
 *
 * @param  clkSrc    PCTMR frequency mode
 * @param  instance  PCTMR instance number
 * @return uint32_t frequency value
 */
uint32_t PCTMR_GetClkFreq(const pctmr_clocksource_t clkSrc, const uint32_t instance);

/**
 * @brief Transform timer ticks to microseconds using the given
 * prescaler settings. Clock frequency must be valid (different from 0).
 *
 * @param  clkFreq system frequency
 * @param  pval    pctmr Prescaler Selection
 * @param  bypass  enable or disable by pass mode
 * @param  ticks   set count value
 * @param  us      the value after conversion
 * @return true    conversion success
 * @return false   conversion failed,result did not fit in 32-bit
 */
bool PCTMR_Ticks2Us(const uint32_t clkFreq, const pctmr_prescaler_t pval, const bool bypass, const uint16_t ticks,
                    uint32_t *const us);

/**
 * @brief Choose clocking configuration (prescaler value, timer ticks)
 * for the desired timeout period, given in microseconds. Input clock frequency,
 * clkFreq, must be greater than 0.
 *
 * @param  clkFreq   System clock frequency
 * @param  us        Set timer count value
 * @param  prescval  Frequency division value
 * @param  bypass    bypass mode
 * @param  ticks     tick value
 * @return true if configuration found
 * @return false configuration mismatch, desired timeout period is too small or too
 * big for the clock settings.
 */
bool PCTMR_ChooseClkConfig(const uint32_t clkFreq, const uint32_t us, pctmr_prescaler_t *const prescval,
                           bool *const bypass, uint16_t *const ticks);

/**
 * @brief Transform microseconds to timer ticks using the given
 * prescaler settings. Input clock frequency, clkFreq, must be greater than 0.
 *
 * @param  clkFreq  System clock frequency
 * @param  prescval Frequency division value
 * @param  bypass   enable or disable bypass mode
 * @param  us       value need to be converted
 * @param  ticks    converted value
 * @return true     conversion completed successfully
 * @return false    conversion failed,value did not fit in 16-bit
 */
bool PCTMR_Us2Ticks(const uint32_t clkFreq, const pctmr_prescaler_t prescval, const bool bypass, const uint32_t us,
                    uint16_t *const ticks);

/**
 * @brief  Get the PDMA Request Enable Flag
 *
 * This function checks whether a PDMA Request feature of the PCTMR is enabled.
 * The PDMA Request is issued when a Compare Match is asserted. If enabled, the
 * Compare Match/Interrupt Pending flag is cleared when the PDMA controller is
 * done.
 *
 * @param  base PCTMR base pointer
 * @return true  enable PDMA Request
 * @return false disable PDMA request
 */
static inline bool PCTMR_GetDmaRequest(const pctmr_type_t *const base)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CSR;

    tmp = (tmp & PCTMR_CSR_TDRE_MASK) >> PCTMR_CSR_TDRE_SHIFT;

    return ((tmp == 1U) ? true : false);
}

/**
 * @brief  Configure the PDMA Request Enable Flag state
 *
 * This function configures the PDMA Request feature of the PCTMR. If enabled,
 * a PDMA Request is issued when the Compare Match event occurs. If enabled, the
 * Compare Match/Interrupt Pending flag is cleared when the PDMA controller is
 * done.
 *
 * @param  base    PCTMR base pointer
 * @param  enable  The new state of the PDMA Request Enable Flag
 */
static inline void PCTMR_SetDmaRequest(pctmr_type_t *const base, bool enable)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CSR;
    /* Clear the affected bit-field and write '0' to the w1c bits to avoid side-effects */
    tmp &= ~(PCTMR_CSR_TDRE_MASK | PCTMR_CSR_TCF_MASK);
    tmp |= PCTMR_CSR_TDRE(enable ? (uint32_t)1U : (uint32_t)0U);
    base->CSR = tmp;
}

/**
 * @brief  Get the Compare Flag state
 *
 * This function checks whether a Compare Match event has occurred or if there is
 * an Interrupt Pending.
 *
 * @param  base PCTMR base pointer
 * @return true  Compare Match/Interrupt Pending asserted
 * @return false Compare Match/Interrupt Pending not asserted
 */
static inline bool PCTMR_GetIntFlag(const pctmr_type_t *const base)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CSR;
    tmp          = (tmp & PCTMR_CSR_TCF_MASK) >> PCTMR_CSR_TCF_SHIFT;

    return ((tmp == 1U) ? true : false);
}

/**
 * @brief  Clear the Compare Flag
 *
 * This function clears the Compare Flag/Interrupt Pending state.
 * @param  base PCTMR base pointer
 */
static inline void PCTMR_ClearIntFlag(pctmr_type_t *const base)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CSR;
    tmp |= (PCTMR_CSR_TCF_MASK);
    base->CSR = tmp;
}

/**
 * @brief  Get the Interrupt Enable state
 *
 * This function returns the Interrupt Enable state for the PCTMR. If enabled,
 * an interrupt is generated when a Compare Match event occurs.
 *
 * @param  base  PCTMR base pointer
 * @return true  Interrupt enabled
 * @return false Interrupt disabled
 */
static inline bool PCTMR_GetInterruptEnable(const pctmr_type_t *const base)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CSR;
    tmp          = (tmp & PCTMR_CSR_TIE_MASK) >> PCTMR_CSR_TIE_SHIFT;

    return ((tmp == 1U) ? true : false);
}

/**
 * @brief  Configure the Interrupt Enable state
 *
 * This function configures the Interrupt Enable state for the PCTMR. If enabled,
 * an interrupt is generated when a Compare Match event occurs.
 *
 * @param  base  PCTMR base pointer
 * @param  enable  enable or disable interrupt state
 */
static inline void PCTMR_SetInterrupt(pctmr_type_t *const base, bool enable)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CSR;
    /* Clear the affected bit-field and write '0' to the w1c bits to avoid side-effects */
    tmp &= ~(PCTMR_CSR_TIE_MASK | PCTMR_CSR_TCF_MASK);
    tmp |= PCTMR_CSR_TIE(enable ? (uint32_t)1U : (uint32_t)0U);
    base->CSR = tmp;
}

/**
 * @brief Get the Pin select for Counter Mode
 *
 * @param  base  PCTMR base pointer
 * @return pctmr_pinselect_t
 *         - PCTMR_PINSELECT_TRGMUX: count pulses from TRGMUX output
 *         - PCTMR_PINSELECT_ALT1: count pulses from pin alt 1
 *         - PCTMR_PINSELECT_ALT2: count pulses from pin alt 2
 *         - PCTMR_PINSELECT_ALT3: count pulses from pin alt 3
 */
static inline pctmr_pinselect_t PCTMR_GetPinSelect(const pctmr_type_t *const base)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CSR;
    tmp          = (tmp & PCTMR_CSR_TPS_MASK) >> PCTMR_CSR_TPS_SHIFT;
    return (pctmr_pinselect_t)(tmp);
}

/**
 * @brief  Configure the Pin selection for Pulse Counter Mode
 *
 * This function configures the input Pin selection for Pulse Counter Mode.
 * This feature can be configured only when the PCTMR is disabled.
 *
 * @param  base  PCTMR base pointer
 * @param  pinsel  Pin selection
 *          - PCTMR_PINSELECT_TRGMUX: count pulses from TRGMUX output
 *          - PCTMR_PINSELECT_ALT1: count pulses from pin alt 1
 *          - PCTMR_PINSELECT_ALT2: count pulses from pin alt 2
 *          - PCTMR_PINSELECT_ALT3: count pulses from pin alt 3
 *
 */
static inline void PCTMR_SetPinSelect(pctmr_type_t *const base, const pctmr_pinselect_t pinsel)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CSR;
    /* Clear the affected bit-field and write '0' to the w1c bits to avoid side-effects */
    tmp &= ~(PCTMR_CSR_TPS_MASK | PCTMR_CSR_TCF_MASK);
    tmp |= PCTMR_CSR_TPS(pinsel);
    base->CSR = tmp;
}

/**
 * @brief Get Pin Polarity for Pulse Counter Mode
 *
 * This function returns the configured pin polarity that triggers an increment
 * in Pulse Counter Mode.
 *
 * @param  base  PCTMR base pointer
 * @return pctmr_pinpolarity_t
 *        PCTMR_PINPOLARITY_RISING: count pulse on Rising Edge
 *        PCTMR_PINPOLARITY_FALLING: count pulse on Falling Edge
 */
static inline pctmr_pinpolarity_t PCTMR_GetPinPolarity(const pctmr_type_t *const base)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CSR;
    tmp          = (tmp & PCTMR_CSR_TPP_MASK) >> PCTMR_CSR_TPP_SHIFT;

    return (pctmr_pinpolarity_t)((tmp == 0U) ? PCTMR_PINPOLARITY_RISING : PCTMR_PINPOLARITY_FALLING);
}

/**
 * @brief Configure Pin Polarity for Pulse Counter Mode
 *
 * This function configures the pin polarity that triggers an increment in Pulse
 * Counter Mode. This feature can be configured only when the PCTMR is disabled.
 *
 * @param  base PCTMR base pointer
 * @param  pol  The pin polarity to count in Pulse Counter Mode
 *         - PCTMR_PINPOLARITY_RISING: count pulse on Rising Edge
 *         - PCTMR_PINPOLARITY_FALLING: count pulse on Falling Edge
 */
static inline void PCTMR_SetPinPolarity(pctmr_type_t *const base, const pctmr_pinpolarity_t pol)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CSR;
    /* Clear the affected bit-field and write '0' to the w1c bits to avoid side-effects */
    tmp &= ~(PCTMR_CSR_TPP_MASK | PCTMR_CSR_TCF_MASK);
    tmp |= PCTMR_CSR_TPP(pol);
    base->CSR = tmp;
}

/**
 * @brief Get the Free Running state
 *
 * This function checks whether the Free Running feature of the PCTMR is enabled
 * or disabled.
 *
 * @param  base PCTMR base pointer
 * @return true   Free Running Mode enabled. Reset counter on 16-bit overflow
 * @return false  Free Running Mode disabled. Reset counter on Compare Match.
 */
static inline bool PCTMR_GetFreeRunning(const pctmr_type_t *const base)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CSR;
    tmp          = (tmp & PCTMR_CSR_TFC_MASK) >> PCTMR_CSR_TFC_SHIFT;

    return ((tmp == 1U) ? true : false);
}

/**
 * @brief  Configure the Free Running state
 *
 * This function configures the Free Running feature of the PCTMR. This feature
 * can be configured only when the PCTMR is disabled.
 *
 * @param  base     PCTMR base pointer
 * @param  enable   The new Free Running state
 *         - true: Free Running Mode enabled. Reset counter on 16-bit overflow
 *         - false: Free Running Mode disabled. Reset counter on Compare Match.
 */
static inline void PCTMR_SetFreeRunning(pctmr_type_t *const base, const bool enable)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CSR;
    /* Clear the affected bit-field and write '0' to the w1c bits to avoid side-effects */
    tmp &= ~(PCTMR_CSR_TFC_MASK | PCTMR_CSR_TCF_MASK);
    tmp |= PCTMR_CSR_TFC(enable ? (uint32_t)1U : (uint32_t)0U);
    base->CSR = tmp;
}

/**
 * @brief  Get current Work Mode
 *
 * This function returns the currently configured Work Mode for the PCTMR.
 *
 * @param  base PCTMR base pointer
 * @return pctmr_workmode_t
 *         - PCTMR_WORKMODE_TIMER: PCTMR is in Timer Mode
 *         - PCTMR_WORKMODE_PULSECOUNTER: PCTMR is in Pulse Counter Mode
 */
static inline pctmr_workmode_t PCTMR_GetWorkMode(const pctmr_type_t *const base)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CSR;
    tmp          = (tmp & PCTMR_CSR_TMS_MASK) >> PCTMR_CSR_TMS_SHIFT;

    return (pctmr_workmode_t)((tmp == 0U) ? PCTMR_WORKMODE_TIMER : PCTMR_WORKMODE_PULSECOUNTER);
}

/**
 * @brief  Configure the Work Mode
 *
 * This function configures the PCTMR to either Timer Mode or Pulse Counter
 * Mode. This feature can be configured only when the PCTMR is disabled.
 *
 * @param  base PCTMR base pointer
 * @param  mode New Work Mode
 *          - PCTMR_WORKMODE_TIMER: PCTMR set to Timer Mode
 *          - PCTMR_WORKMODE_PULSECOUNTER: PCTMR set to Pulse Counter Mode
 */
static inline void PCTMR_SetWorkMode(pctmr_type_t *const base, const pctmr_workmode_t mode)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CSR;
    /* Clear the affected bit-field and write '0' to the w1c bits to avoid side-effects */
    tmp &= ~(PCTMR_CSR_TMS_MASK | PCTMR_CSR_TCF_MASK);
    tmp |= PCTMR_CSR_TMS(mode);
    base->CSR = tmp;
}

/**
 * @brief  Get the Enable state.
 *
 * Prior to reconfiguring the PCTMR, it is necessary to disable it.
 *
 * @param  base - PCTMR base pointer
 * @return true if PCTMR enabled
 * @return false if PCTMR disabled
 */
static inline bool PCTMR_GetEnable(const pctmr_type_t *const base)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CSR;

    tmp = (tmp & PCTMR_CSR_TEN_MASK) >> PCTMR_CSR_TEN_SHIFT;

    return ((tmp == 1U) ? true : false);
}

/**
 * @brief  Enable the PCTMR
 *
 * Enable the PCTMR. Starts the timer/counter.
 *
 * @param  base PCTMR base pointer
 */
static inline void PCTMR_Enable(pctmr_type_t *const base)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CSR;
    /* Clear the affected bit-field and write '0' to the w1c bits to avoid side-effects */
    tmp &= ~(PCTMR_CSR_TEN_MASK | PCTMR_CSR_TCF_MASK);
    tmp |= PCTMR_CSR_TEN(1U);
    base->CSR = tmp;
}

/**
 * @brief  Disable the PCTMR
 *
 * Disable the PCTMR. Stop the Counter/Timer and allow reconfiguration.
 *
 * @param  base  PCTMR base pointer
 */
static inline void PCTMR_Disable(pctmr_type_t *const base)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CSR;
    /* Clear the affected bit-field and write '0' to the w1c bits to avoid side-effects */
    tmp &= ~(PCTMR_CSR_TEN_MASK | PCTMR_CSR_TCF_MASK);

    tmp |= PCTMR_CSR_TEN(PCTMR_DISABLE);
    base->CSR = tmp;
}

/**
 * @brief Get Prescaler/Glitch Filter divider value
 *
 * This function returns the currently configured Prescaler/Glitch Filter divider
 * value.
 *
 * @param  base PCTMR base pointer
 * @return pctmr_prescaler_t The Prescaler/Glitch filter value
 *          - PCTMR_PRESCALE_2: Timer mode: prescaler 2, Glitch filter mode: invalid
 *          - PCTMR_PRESCALE_4_GLITCHFILTER_2: Timer mode: prescaler 4, Glitch filter mode: 2 clocks
 *          - PCTMR_PRESCALE_8_GLITCHFILTER_4: Timer mode: prescaler 8, Glitch filter mode: 4 clocks
 *          - PCTMR_PRESCALE_16_GLITCHFILTER_8: Timer mode: prescaler 16, Glitch filter mode: 8 clocks
 *          - PCTMR_PRESCALE_32_GLITCHFILTER_16: Timer mode: prescaler 32, Glitch filter mode: 16 clocks
 *          - PCTMR_PRESCALE_64_GLITCHFILTER_32: Timer mode: prescaler 64, Glitch filter mode: 32 clocks
 *          - PCTMR_PRESCALE_128_GLITCHFILTER_64: Timer mode: prescaler 128, Glitch filter mode: 64 clocks
 *          - PCTMR_PRESCALE_256_GLITCHFILTER_128: Timer mode: prescaler 256, Glitch filter mode: 128 clocks
 *          - PCTMR_PRESCALE_512_GLITCHFILTER_256: Timer mode: prescaler 512, Glitch filter mode: 256 clocks
 *          - PCTMR_PRESCALE_1024_GLITCHFILTER_512: Timer mode: prescaler 1024, Glitch filter mode: 512 clocks
 *          - PCTMR_PRESCALE_2048_GLITCHFILTER_1024: Timer mode: prescaler 2048, Glitch filter mode: 1024 clocks
 *          - PCTMR_PRESCALE_4096_GLITCHFILTER_2048: Timer mode: prescaler 4096, Glitch filter mode: 2048 clocks
 *          - PCTMR_PRESCALE_8192_GLITCHFILTER_4096: Timer mode: prescaler 8192, Glitch filter mode: 4096 clocks
 *          - PCTMR_PRESCALE_16384_GLITCHFILTER_8192: Timer mode: prescaler 16384, Glitch filter mode: 8192 clocks
 *          - PCTMR_PRESCALE_32768_GLITCHFILTER_16384: Timer mode: prescaler 32768, Glitch filter mode: 16384 clocks
 *          - PCTMR_PRESCALE_65536_GLITCHFILTER_32768: Timer mode: prescaler 65536, Glitch filter mode: 32768 clocks
 */
static inline pctmr_prescaler_t PCTMR_GetPrescaler(const pctmr_type_t *const base)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->PSR;
    tmp          = (tmp & PCTMR_PSR_PRESCALE_MASK) >> PCTMR_PSR_PRESCALE_SHIFT;
    return (pctmr_prescaler_t)(tmp);
}

/**
 * @brief  Configure the Prescaler/Glitch Filter divider value
 *
 * This function configures the value for the Prescaler/Glitch Filter. This
 * feature can be configured only when the PCTMR is disabled.
 *
 * @param  base   PCTMR base pointer
 * @param  presc  The new Prescaler value
 *         - PCTMR_PRESCALE_2: Timer mode: prescaler 2, Glitch filter mode: invalid
 *         - PCTMR_PRESCALE_4_GLITCHFILTER_2: Timer mode: prescaler 4, Glitch filter mode: 2 clocks
 *         - PCTMR_PRESCALE_8_GLITCHFILTER_4: Timer mode: prescaler 8, Glitch filter mode: 4 clocks
 *         - PCTMR_PRESCALE_16_GLITCHFILTER_8: Timer mode: prescaler 16, Glitch filter mode: 8 clocks
 *         - PCTMR_PRESCALE_32_GLITCHFILTER_16: Timer mode: prescaler 32, Glitch filter mode: 16 clocks
 *         - PCTMR_PRESCALE_64_GLITCHFILTER_32: Timer mode: prescaler 64, Glitch filter mode: 32 clocks
 *         - PCTMR_PRESCALE_128_GLITCHFILTER_64: Timer mode: prescaler 128, Glitch filter mode: 64 clocks
 *         - PCTMR_PRESCALE_256_GLITCHFILTER_128: Timer mode: prescaler 256, Glitch filter mode: 128 clocks
 *         - PCTMR_PRESCALE_512_GLITCHFILTER_256: Timer mode: prescaler 512, Glitch filter mode: 256 clocks
 *         - PCTMR_PRESCALE_1024_GLITCHFILTER_512: Timer mode: prescaler 1024, Glitch filter mode: 512 clocks
 *         - PCTMR_PRESCALE_2048_GLITCHFILTER_1024: Timer mode: prescaler 2048, Glitch filter mode: 1024 clocks
 *         - PCTMR_PRESCALE_4096_GLITCHFILTER_2048: Timer mode: prescaler 4096, Glitch filter mode: 2048 clocks
 *         - PCTMR_PRESCALE_8192_GLITCHFILTER_4096: Timer mode: prescaler 8192, Glitch filter mode: 4096 clocks
 *         - PCTMR_PRESCALE_16384_GLITCHFILTER_8192: Timer mode: prescaler 16384, Glitch filter mode: 8192 clocks
 *         - PCTMR_PRESCALE_32768_GLITCHFILTER_16384: Timer mode: prescaler 32768, Glitch filter mode: 16384 clocks
 *         - PCTMR_PRESCALE_65536_GLITCHFILTER_32768: Timer mode: prescaler 65536, Glitch filter mode: 32768 clocks
 */
static inline void PCTMR_SetPrescaler(pctmr_type_t *const base, const pctmr_prescaler_t presc)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->PSR;
    tmp &= ~(PCTMR_PSR_PRESCALE_MASK);
    tmp |= PCTMR_PSR_PRESCALE(presc);
    base->PSR = tmp;
}

/**
 * @brief Get the Prescaler/Glitch Filter Bypass enable state
 *
 * This function checks whether the Prescaler/Glitch Filter Bypass is enabled.
 *
 * @param  base   PCTMR base pointer
 * @return true if Prescaler/Glitch Filter Bypass enabled
 * @return false if Prescaler/Glitch Filter Bypass disabled
 */
static inline bool PCTMR_GetBypass(const pctmr_type_t *const base)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->PSR;
    tmp          = (tmp & PCTMR_PSR_PBYP_MASK) >> PCTMR_PSR_PBYP_SHIFT;

    return ((tmp == 1U) ? true : false);
}

/**
 * @brief  Configure the Prescaler/Glitch Filter Bypass enable state
 *
 * This function configures the Prescaler/Glitch filter Bypass. This feature
 * can be configured only when the PCTMR is disabled.
 *
 * @param  base    PCTMR base pointer
 * @param  enable   The new Prescaler/Glitch Filter Bypass state
 */
static inline void PCTMR_SetBypass(pctmr_type_t *const base, const bool enable)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->PSR;
    tmp &= ~(PCTMR_PSR_PBYP_MASK);
    tmp |= PCTMR_PSR_PBYP(enable ? (uint32_t)1U : (uint32_t)0U);
    base->PSR = tmp;
}

/**
 * @brief  Get the PCTMR input Clock selection
 *
 * This function returns the current configured input Clock for the PCTMR.
 *
 * @param  base  PCTMR base pointer
 * @return pctmr_clocksource_t
 *         - PCTMR_CLOCKSOURCE_SROSC: clock from SIRC DIV2
 *         - PCTMR_CLOCKSOURCE_1KHZ_LPO: clock from 1kHz LPO
 *         - PCTMR_CLOCKSOURCE_AON: clock from RTC
 *         - PCTMR_CLOCKSOURCE_ERCLK: clock from PCC
 */
static inline pctmr_clocksource_t PCTMR_GetClockSelect(const pctmr_type_t *const base)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->PSR;
    tmp          = (tmp & PCTMR_PSR_PCS_MASK) >> PCTMR_PSR_PCS_SHIFT;
    return (pctmr_clocksource_t)(tmp);
}

/**
 * @brief  Configure the PCTMR input Clock selection
 *
 * This function configures a clock source for the PCTMR. This feature can be
 * configured only when the PCTMR is disabled.
 *
 * @param  base        PCTMR base pointer
 * @param  clockSel    New Clock Source
 *          - PCTMR_CLOCKSOURCE_SROSC: clock from SIRC DIV2
 *          - PCTMR_CLOCKSOURCE_1KHZ_LPO: clock from 1kHz LPO
 *          - PCTMR_CLOCKSOURCE_AON: clock from RTC
 *          - PCTMR_CLOCKSOURCE_ERCLK: clock from PCC
 */
static inline void PCTMR_SetClockSelect(pctmr_type_t *const base, const pctmr_clocksource_t clockSel)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->PSR;
    tmp &= ~(PCTMR_PSR_PCS_MASK);
    tmp |= PCTMR_PSR_PCS(clockSel);
    base->PSR = tmp;
}

/**
 * @brief Get the Compare Value
 *
 * This function returns the current Compare Value.
 *
 * @param  base PCTMR base pointer
 * @return uint16_t The Compare Value
 *
 */
static inline uint16_t PCTMR_GetCompareValue(const pctmr_type_t *const base)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CMR;

    tmp = (tmp & PCTMR_CMR_COMPARE_MASK) >> PCTMR_CMR_COMPARE_SHIFT;

    return (uint16_t)(tmp);
}

/**
 * @brief  Configure the Compare Value
 *
 * This function configures the Compare Value. If set to 0, the Compare Match
 * event and the hardware trigger assert and remain asserted until the timer is
 * disabled.
 *
 * @param  base    PCTMR base pointer
 * @param  compVal The new Compare Value
 */
static inline void PCTMR_SetCompareValue(pctmr_type_t *const base, const uint16_t compVal)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->CMR;
    tmp &= ~(PCTMR_CMR_COMPARE_MASK);
    tmp |= PCTMR_CMR_COMPARE(compVal);
    base->CMR = tmp;
}

/**
 * @brief  Get the current Counter Value
 *
 * This function returns the Counter Value.
 * @param  base  PCTMR base pointer
 * @return uint16_t The Counter Value
 */
static inline uint16_t PCTMR_GetCounterValue(pctmr_type_t *const base)
{
    OS_ASSERT(base != NULL);

    /* Write dummy value before reading register */

    base->CNR    = PCTMR_CNR_COUNTER(PCTMR_DISABLE);
    uint16_t cnr = (uint16_t)base->CNR;
    return cnr;
}

/**
 * @brief  Compute total number of divided (prescaled) timer ticks
 *
 * @param  nn nn value
 * @param  p  p value
 * @return uint64_t the value of nticks.
 */
static inline uint64_t PCTMR_ComputeNticks(uint64_t nn, uint8_t p)
{
    uint64_t nPresc = (uint64_t)1U << p;
    OS_ASSERT(nPresc != 0U);

    /* integer division */
    uint64_t nticks = ((nn + (nPresc >> 1U)) / nPresc);

    return nticks;
}

/**
 * @brief Transform the value of divided (prescaled) timer ticks, nticks
 * to a 16-bit value to be written to the hardware register. Cap or underflow
 * cause an error.
 *
 * @param  nticks input tick value
 * @param  ticks  output tick value
 * @return true
 * @return false
 */
static inline bool PCTMR_Nticks2CompareTicks(uint64_t nticks, uint16_t *ticks)
{
    bool success = true;

    /* if nticks fits, write the value to ticks */
    if (nticks <= PCTMR_MAX_CMR_NTICKS) {
        if (nticks == 0U) {
            /* timeout period (us) too low for prescaler settings */
            *ticks  = 0U;
            success = false;
        } else {
            /* According to RM, the PCTMR compare events take place when "the CNR equals the value of the CMR and
             * increments". The additional increment is compensated here by decrementing the calculated compare value
             * with 1, before being written to CMR. */
            *ticks = (uint16_t)(nticks - 1U);
        }
    } else {
        /* timeout period (us) too high for prescaler settings */
        *ticks  = PCTMR_CMR_COMPARE_MASK;
        success = false;
    }

    return success;
}

/**
 * @brief  Transform microseconds to undivided (unPrescaled) timer units.
 *
 * @param  clkFreq  System frequency
 * @param  us       TImer setting value
 * @return uint64_t  the value of nn
 */
static inline uint64_t PCTMR_Us2Nn(const uint32_t clkFreq, const uint32_t us)
{
    /* Approximate the timeout in undivided (unPrescaled) timer ticks.
        - us is the timeout in microseconds (1/10^6 seconds)
        - clkFreq is the frequency in Hertz
        Operation:
        nn = (us/1000000) * clkFreq
        In C:
        For better precision, first to the multiplication (us * clkFreq)
        To overcome the truncation of the div operator in C, add half of the
        denominator before the division. Hence:
        nn = (us * clkFreq + 500000) / 1000000
    */
    /* There is no risk of overflow since us is 32-bit wide and clkFreq can be
       a theoretical maximum of ~100 MHz (platform maximum), which is over the
       maximum input of the PCTMR anyway
     */
    uint64_t nn = (uint64_t)((uint64_t)us * (uint64_t)clkFreq);
    nn          = (nn + 500000U) / 1000000U;
    return nn;
}

/** TIMER MODE CONFIGURATION******************************************************
 *
 * Timer Mode - Prescaler settings calculations
 * --------------------------------------------
 *
 * Timer Mode configuration takes a period (timeout) value expressed in
 * micro-seconds. To convert this to PCTMR prescaler (and compare value)
 * settings, the closest match must be found.
 * For best precision, the lowest prescaler that allows the corresponding
 * compare value to fit in the 16-bit register will be chosen.
 *
 * Algorithm for choosing prescaler and compare values:
 * =============================================================================
 * In: tper_us (period in microseconds), fclk (input clock frequency in Hertz)
 * Out: nticks (timer ticks), p (prescaler coefficient, 2^p = prescaler value)
 * ---
 * 1) Compute nn = tper_us * fclk / 1000000
 * 2) for p = 0..16
 *  2.1) nticks = nn / 2^p
 *  2.2) if nticks < 0x10000
 *      2.2.1) STOP, found nticks and p
 * 3) nticks = 0xFFFF, p = 16
 * =============================================================================
 *
 * A few names used throughout the static functions affecting Timer mode:
 *  nn - total number of timer ticks (undivided, unPrescaled) that is necessary
 *      for a particular timeout.
 *      nn = (tper_us * fclk) / 1000000 = nticks * nPresc
 *
 *  tper_us - a period (or timeout) expressed in microsecond units. In most
 *      functions will be denoted as 'us' for microseconds.
 *
 *  nticks - number of timer ticks that is necessary for a particular timeout,
 *      after preScaling
 *
 *  nPresc - prescaler value (1, 2, 4 ... 65536)
 *
 *  p - prescaler coefficient, 2^p = nPresc
 *
 *  fclk - input clock frequency, in Hertz. In most function will be denoted as
 *      'clkFreq'.
 *END**************************************************************************/

/**
 * @brief Transform prescaler settings (bypass on/off, prescaler value)
 *  to prescaler coefficient value (2's power), p.
 *
 * @param  prescval  Frequency selection
 * @param  bypass    enable or disable bypass
 * @return uint8_t the value of p.
 */
static inline uint8_t PCTMR_Cfg2P(const pctmr_prescaler_t prescval, const bool bypass)
{
    uint8_t p = 0U;

    if (!bypass) {
        p = (uint8_t)(((uint8_t)prescval) + 1U);
    }

    return p;
}

#if defined(__cplusplus)
}
#endif

#endif
