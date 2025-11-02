/*
 * Copyright (c) 2020-2021 ESWIN Limited. All rights reserved.
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
 ******************************************************************************
 * @file     system_e320.h
 * @brief    EMSIS ESWIN Redkite Device Peripheral Access Layer Header File for
 *           Device <Device>
 * @version  V0.1.0
 * @date     4 Jan. 2021
 ******************************************************************************
 */

#ifndef __SYSTEM_E320_H__   /* ToDo: replace '<Device>' with your device name */
#define __SYSTEM_E320_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "core_emsis.h"
#include "e320.h"
#include <stdint.h>

/* ====================================================================== */
/* System clock setting                                                   */
/* ====================================================================== */

extern uint32_t SystemCoreClock;     /** System Clock Frequency (Core Clock)  */

/**
 * \brief Setup the micro controller system.
 * \details
 * Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit(void);

/**
 * \brief  Update SystemCoreClock variable.
 * \details
 * Updates the SystemCoreClock with current core Clock retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate(void);

/* ====================================================================== */
/* Exception & interrupt handler prototype definition                     */
/* ====================================================================== */

//typedef void (*exception_handler_fptr)(void);
typedef void (*exception_handler_fptr)(unsigned long *sp);

typedef exception_handler_fptr e320_exception_handler_table_t[SYSTEM_MAX_EXCEPTION_NUM];

/**
 * \brief  register the exception handler
 * \details
 * register the exception handler to exception handler table, it will
 * be called when the specific exception raised
 */
extern int register_exception_handler(EXCn_Type EXCn, exception_handler_fptr handler);

#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_E320_H__ */
