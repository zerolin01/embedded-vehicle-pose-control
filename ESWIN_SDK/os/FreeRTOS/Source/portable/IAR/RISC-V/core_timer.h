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

#ifndef __CORE_TIMER_H__
#define __CORE_TIMER_H__

#include "e320.h"
#if defined(__GNUC__)
#else
#include "emsis_iar.h"
#endif
#include "eswin_sdk_soc.h"
#ifdef __cplusplus
extern "C" {
#endif
#define TIMER_CTRL_ADDR  0x02000000
#define TIMER_MTIME_LOW  0xBFF8
#define TIMER_MTIME_HIGH 0xBFFC
#define TIMER_MTIME_CMP  0x4000
#define __read_csr       __RV_CSR_READ
uint64_t systimer_get_loadvalue(void);
void systimer_set_loadvalue(uint64_t value);
void systimer_set_comparevalue(uint64_t value);

uint64_t soc_timer_set_tick_interval(uint64_t tick);
uint32_t get_cpu_freq(void);
uint32_t sys_tick_reload(uint64_t ticks);

#ifdef __cplusplus
}
#endif
#endif /* __HAL_INTERRUPT_H__ */
