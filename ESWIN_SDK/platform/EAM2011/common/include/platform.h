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
 * Project:      EMSIS-HAL API
 * Title:        platform.h header file
 *
 * Version 0.0.0
 *    Initial Release
 *---------------------------------------------------------------------------
 */

#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "system-e320.h"

/*--------------------------------------------------------------------------*/
/* Interrupt */
/* Interrupt handler type declaration
 * Interrupt number definition. Except for software / timer / external,
 * other interrupt numbers are related to SOC design.
 * Please declare this enumeration type in each SoC platform
 */

/* Interrupt Register type*/
typedef struct register_type_s {
    uint8_t lvl;
    uint8_t priority;
    void *data_ptr;
    CLIC_TRIGGER_Type trig_mode;
} register_type_t;

/* Interrupt vector set*/
typedef void (*irq_handler)(void *);
typedef void *interrupt_handler_data_ptr;

typedef irq_handler kitty_interrupt_handler_table_t[SOC_INT_MAX];
typedef interrupt_handler_data_ptr kitty_interrupt_handler_data_ptr_table_t[SOC_INT_MAX];

/* The num of PMP */
#define PMP_ENTRY_NUM   4

#endif /* PLATFORM_H_  */
