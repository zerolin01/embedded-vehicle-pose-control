/*
 * Copyright (c) 2013-2020 Arm Limited. All rights reserved.
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
 *
 * ----------------------------------------------------------------------
 *
 * $Date:        19. July 2021
 * $Revision:    V0.0.0
 *
 * Project:      EMSIS-BASIC API
 * Title:        basic_tick.c file
 *
 * Version 0.0.0
 *    Initial Release
 *---------------------------------------------------------------------------
 */

#include "basic_common.h"

extern struct basic_platform_operations_s platform_ops;

/**
 * \brief  Function to measure CPU freq by n timers
 * \details
 * as we know timer freqency and can measure the cpu cycles by CSR_MCYCLE
 * register, the cpu frequency can be calculate by counting cpu cycle in n
 * timer cycles
 */
static uint32_t measure_cpu_freq(uint32_t n)
{
    uint32_t start_mcycle, delta_mcycle;
    uint32_t start_mtime, delta_mtime;
    uint32_t mtime_freq = platform_ops.get_timer_freq();

    // Don't start measuruing until we see an mtime tick
    uint32_t tmp = (uint32_t)SysTimer_GetLoadValue();

    do {
        start_mtime  = (uint32_t)SysTimer_GetLoadValue();
        start_mcycle = __RV_CSR_READ(CSR_MCYCLE);
    } while (start_mtime == tmp);

    do {
        delta_mtime  = (uint32_t)SysTimer_GetLoadValue() - start_mtime;
        delta_mcycle = __RV_CSR_READ(CSR_MCYCLE) - start_mcycle;
    } while (delta_mtime < n);

    return (delta_mcycle / delta_mtime) * mtime_freq + ((delta_mcycle % delta_mtime) * mtime_freq) / delta_mtime;
}

/*--------------------------------------------------------------------------*/
/* Tick */
/****************************************************************************
 * name: basic_get_tick
 *
 * description:
 *   get the number of system ticks.
 *
 * input parameters:
 *   none.
 *
 * returned value:
 *   the current tick number of the system increases monotonously from power on.
 *
 ****************************************************************************/
uint64_t basic_get_tick(void)
{
    return SysTimer_GetLoadValue();
}

/****************************************************************************
 * Name: basic_get_freq
 *
 * Description:
 *   Get tick count cycle.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   Number of ticks per second.
 *
 ****************************************************************************/
uint64_t basic_get_freq(void)
{
    uint64_t cpu_freq;

    // warm up
    measure_cpu_freq(1);
    // measure for real
    cpu_freq = measure_cpu_freq(100);

    return cpu_freq;
}

/****************************************************************************
 * Name: basic_set_tick
 *
 * Description:
 *   Set the number of ticks riggered by the next timer interrupt.
 *
 * Input Parameters:
 *   tick- The number of ticks to be set in timer.
 *
 * Returned Value:
 *   Number of ticks before setting.
 *
 ****************************************************************************/
uint64_t basic_set_tick(uint64_t tick)
{
    return platform_ops.set_tick(tick);
}

/****************************************************************************
 * Name: basic_set_tick_interval
 *
 * Description:
 *   Set the next timer interrupt trigger time to tick.
 *
 * Input Parameters:
 *   tick- Time interval to be set.
 *
 * Returned Value:
 *   Time interval from next timer interrupt trigger.
 *
 ****************************************************************************/
uint64_t basic_set_tick_interval(uint64_t tick)
{
    return platform_ops.set_tick_interval(tick);
}

/**
 * @brief BASIC_Msec2Tick
 * @details
 * 		The millisecond is converted to tick.
 * @param
 * 		millisecond - how many millisecond need to be converted.
 * @retval
 * 		return ticks.
 * */
uint64_t BASIC_Msec2Tick(uint32_t milliseconds)
{
    uint32_t mtime_freq = platform_ops.get_timer_freq();
    uint64_t ticks      = (mtime_freq * (uint64_t)milliseconds) / 1000;

    return ticks;
}

/**
 * @brief BASIC_TimerCountReset
 * @details
 * 		reset the counter to zero.
 * @retval
 * 		return void.
 * */
void BASIC_TimerCountReset(void)
{
    SysTimer_SetLoadValue(0);
}