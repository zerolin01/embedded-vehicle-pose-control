/*
 * Copyright 2022 Beijing ESWIN Computing Technology Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include "core_timer.h"

/*----------------------------------------------------------------------------
 *Clock functions
 *----------------------------------------------------------------------------
 */

/****************************************************************************
 * Name: get_timer_freq
 *
 * Description:
 *   Function to get the timer frequencey
 * Timer frequency is device specific feature, so this function is weak
 * and can be override by SoC or Board functions
 *   
 * Input Parameters:
 *  
 * Returned Value:
 *  timer freq
 *
 ****************************************************************************/
__WEAK uint32_t get_timer_freq(void)
{
    return SystemTimerClock;
}

uint32_t mtime_lo(void)
{
    return *(volatile uint32_t *)(TIMER_CTRL_ADDR + TIMER_MTIME_LOW);
}

uint32_t mtime_hi(void)
{
    return *(volatile uint32_t *)(TIMER_CTRL_ADDR + TIMER_MTIME_HIGH);
}

void systimer_set_comparevalue(uint64_t value)
{
    *(volatile uint64_t *)(TIMER_CTRL_ADDR + TIMER_MTIME_CMP) = value;
}

void systimer_set_loadvalue(uint64_t value)
{
    *(volatile uint64_t *)(TIMER_CTRL_ADDR + TIMER_MTIME_LOW) = value;
}

uint64_t systimer_get_loadvalue(void)
{
  while (1) {
    uint32_t hi = mtime_hi();
    uint32_t lo = mtime_lo();
    if (hi == mtime_hi())
      return ((uint64_t)hi << 32) | lo;
  }
}

/****************************************************************************
 * Name: measure_cpu_freq
 *
 * Description:
 *   Function to measure CPU freq by n timers.
 *   As we know timer freqency and can measure the cpu cycles by CSR_MCYCLE
 *   register, the cpu frequency can be calculate by counting cpu cycle in n
 *   timer cycles
 * 
 * Input Parameters:
 *   n- timer count 
 *
 * Returned Value:
 *  cpu_freq
 *
 ****************************************************************************/
static uint32_t measure_cpu_freq(uint32_t n)
{
    uint32_t start_mcycle, delta_mcycle;
    uint32_t start_mtime, delta_mtime;
    uint32_t mtime_freq = get_timer_freq();

    // Don't start measuruing until we see an mtime tick
    uint32_t tmp = mtime_lo();

    do {
      start_mtime = mtime_lo();
    } while (start_mtime == tmp);

    start_mcycle = __read_csr(_CSR_MCYCLE);

      do {
        delta_mtime = mtime_lo() - start_mtime;
      } while (delta_mtime < n);

      delta_mcycle = __read_csr(_CSR_MCYCLE) - start_mcycle;

      return (delta_mcycle / delta_mtime) * mtime_freq
        + ((delta_mcycle % delta_mtime) * mtime_freq) / delta_mtime;
}

/****************************************************************************
 * Name: soc_timer_set_tick_interval
 *
 * Description:
 *   Set timer tick interval
 *
 * Input Parameters:
 *   tick- timer interval ticks 
 *
 * Returned Value:
 *  tick
 *
 ****************************************************************************/
uint64_t soc_timer_set_tick_interval(uint64_t tick)
{
    uint64_t cur_ticks = systimer_get_loadvalue();
    uint64_t reload_ticks = tick + cur_ticks;

    if (reload_ticks > cur_ticks) {
    	systimer_set_comparevalue(reload_ticks);
    } else {
        /*
         * When added the ticks value, then the MTIMERCMP < TIMER,
         * which means the MTIMERCMP is overflowed,
         * so we need to reset the counter to zero
         */
    	systimer_set_loadvalue(0);
    	systimer_set_comparevalue(tick);
    }

    return tick;
}

/****************************************************************************
 * Name: get_cpu_freq
 *
 * Description:
 *   Measure cpu frequency based on timer frequency
 *
 * Input Parameters:
 * 
 * Returned Value:
 *  cpu_freq   
 *
 ****************************************************************************/
uint32_t get_cpu_freq(void)
{
    uint32_t cpu_freq;

    // warm up
    measure_cpu_freq(1);
    // measure for real
    cpu_freq = measure_cpu_freq(100);

    return cpu_freq;
}

uint32_t sys_tick_reload(uint64_t ticks)
{
    uint64_t cur_ticks = systimer_get_loadvalue();
    uint64_t reload_ticks = ticks + cur_ticks;

    if (reload_ticks > cur_ticks) {
        systimer_set_comparevalue(reload_ticks);
    } else {
        /* When added the ticks value, then the MTIMERCMP < TIMER,
         * which means the MTIMERCMP is overflowed,
         * so we need to reset the counter to zero */
        systimer_set_loadvalue(0);
        systimer_set_comparevalue(ticks);
    }

    return (0UL);
}
#if 0
/****************************************************************************
 * Name: delay_1ms
 *
 * Description:
 *   delay a time in milliseconds
 *
 * Input Parameters:
 *   count: delay count 
 *
 * Returned Value:
 *  
 *
 ****************************************************************************/
void delay_1ms(uint32_t count)
{
    uint64_t start_mtime, delta_mtime;
    uint64_t delay_ticks = (SystemTimerClock * (uint64_t)count) / 1000;

    start_mtime = systimer_get_loadvalue();

    do {
        delta_mtime = systimer_get_loadvalue() - start_mtime;
    } while (delta_mtime < delay_ticks);
}
/** @} */ /* End of Doxygen Group EMSIS_Core_SystemAndClock */
#endif