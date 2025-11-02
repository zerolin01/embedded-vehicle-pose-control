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

#ifndef __HAL_INTERRUPT_H__
#define __HAL_INTERRUPT_H__

#include "e320.h"
#if defined(__GNUC__)
#else
#include "emsis_iar.h"
#endif
#include "eswin_sdk_soc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CLIC_ADDR_BASE      0x2800000
#define CLIC_INT_IP_OFFSET  0x000
#define CLIC_INT_IE_OFFSET  0x400
#define CLIC_INT_CFG_OFFSET 0x800
#define CLIC_CFG_OFFSET     0xC00

#define CLIC_CFG_NVBIT_LSB   0U
#define CLIC_CFG_NVBIT_MASK  (0x1UL)
#define CLIC_CFG_NLBITS_LSB  (1U)
#define CLIC_CFG_NLBITS_MASK (0x1EUL)

#define CLIC_INT_CFG_SHV_LSB   0U
#define CLIC_INT_CFG_SHV_MASK  (0x1UL)
#define CLIC_INT_CFG_CTRL_LSB  1U
#define CLIC_INT_CFG_CTRL_MASK (0x7FUL << CLIC_INT_CFG_CTRL_LSB)

#define CLIC_INTIP_IP_LSB       0U
#define CLIC_INTIP_IP_MASK      0x1UL
#define CLIC_MAX_NLBITS         7U  /*!< Max nlbit of the CLICINTCTLBITS */
#define CLIC_DIRECT_INTERRUPT   0x0 /*!< CLIC Direct Interrupt Mode */
#define CLIC_VECTORED_INTERRUPT 0x1

uint8_t clic_get_nlbits(void);
void clic_set_nlbits(uint8_t nlbits);

void clic_set_irq_pendings(uint32_t source);
void clic_clear_irq_pendings(uint32_t source);

int hal_intr_get_mode(void);
int hal_intr_enable(int enable);
int hal_intr_select_mode(inte_mode_t mode);
uint32_t hal_systick_config(uint64_t ticks);
int hal_intr_enable_irq(IRQn_Type irq, int enable);
int clic_interrupt_enable(IRQn_Type irq, uint8_t shv, uint8_t lvl, uint8_t priority);
irq_handler hal_intr_register_handler(IRQn_Type irq, irq_handler handler, register_type_t *type);

#ifdef __cplusplus
}
#endif
#endif /* __HAL_INTERRUPT_H__ */
