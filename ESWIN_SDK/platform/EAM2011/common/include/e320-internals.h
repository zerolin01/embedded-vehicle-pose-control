/******************************************************************************
 * @file     e320-internals.h
 * @brief    EMSIS Core Peripheral Access Layer Header File for
 *           ESWIN e320 evaluation SoC which support ESWIN KittyHawk cores
 * @version  V0.1.0
 * @date     14 Jan. 2021
 ******************************************************************************/
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
 */

#ifndef __E320_INTERNALS_H__
#define __E320_INTERNALS_H__

#ifndef __ASSEMBLY__
#include <stdint.h>
#include <string.h>
// #include "e320.h"
#include "platform.h"
#endif

#ifndef __ASSEMBLY__
/* ===================================================================== */
/* Device Stack Declaration                                              */
/* ===================================================================== */

extern size_t _e320_stack_size;
extern uint8_t *_e320_stack_bottom;
#if (CONFIG_RV_CORE_NUM > 1)
extern uint8_t **_e320_stack_core;
#endif

/* ==================================================================== */
/* Device interrupt and exception declaration                           */
/* ==================================================================== */
/* Interrupt related */
#if (CONFIG_RV_CORE_NUM > 1)
extern e320_exception_handler_table_t *_e320_exception_handler_table;
#else
extern e320_exception_handler_table_t _e320_exceptin_handler_table;
#endif

#if defined(CONFIG_RV_CORE_NUM) && (CONFIG_RV_CORE_NUM > 1)
extern kitty_interrupt_handler_table_t *_kitty_interrupt_handler_table;
extern kitty_interrupt_handler_data_ptr_table_t *_kitty_interrupt_handler_data_ptr_table;
#else
extern kitty_interrupt_handler_table_t _kitty_interrupt_handler_table;
extern kitty_interrupt_handler_data_ptr_table_t _kitty_interrupt_handler_data_ptr_table;
#endif

/* ===================================================================== */
/* Device Reentrancy declaration                                         */
/* ===================================================================== */
#ifndef CONFIG_CUSTOM_REENT
enum {
    NORMAL_THREAD = 0,
    EXCEPTION_THREAD,
    MAX_THREAD,
};
#define SAVE_REENT_STRUCT(local) \
    do { \
        local = _kitty_libc_getreent(); \
        struct _reent *reent = _kitty_libc_getreent_by_type(EXCEPTION_THREAD); \
        _kitty_libc_setreent(local); \
    } while (0)

#define RESTORE_REENT_STRUCT(local) \
    do { \
        _kitty_libc_setreent(local); \
    } while (0)

void _kitty_libc_impure_init(void);
struct _reent *_kitty_libc_getreent(void);
void _kitty_libc_setreent(struct _reent *reent);
struct _reent *_kitty_libc_getreent_by_type(int type);
#else
#define SAVE_REENT_STRUCT(local)
#define RESTORE_REENT_STRUCT(local)
__WEAK void _kitty_libc_impure_init(void) {}
__WEAK struct _reent *_kitty_libc_getreent(void) {return NULL; }
__WEAK void _kitty_libc_setreent(struct _reent *reent) {(void)reent;}
__WEAK struct _reent *_kitty_libc_getreent_by_type(int type) {(void)type; return NULL; }
#endif

/* ===================================================================== */
/* Device internal functions                                             */
/* ===================================================================== */

/*
 * \brief  Function to init stack setting for multicore
 * \details
 * for multicore configuration, need to allocate stack for each
 * core thread
 */
void _e320_init_pre(void);

/**
 * \brief  Function to do something in boot core
 * \details
 * do something in boot core, w/o other cores
 */
void _e320_init_single(void);

/*
 * \brief  Function to do something in multicore
 * \details
 * eacho core will execute this function to do initialize
 */
void _e320_init_multicore(void);

/**
 * \brief  Function to init exception handler table
 * \details
 * setup exception handler table for both multicore and single core
 * configuration
 */
void _e320_exception_init(void);

/*
 * \brief  Function to setup interrupt handler table
 * \details
 * setup interrupt handler table for both multicore and single core
 * configuration
 */
void _e320_interrupt_init(void);

/*
 * \brief  the interface called when exception raised
 * \details
 * when any exception raised, this interface be called to handle
 * the exception
 */
void _e320_exception_call(unsigned long mcause, unsigned long sp);

/*
 * \brief  the interface called when interrupt raised
 * \details
 * when any interrupt raised, this interface be called to handle
 * the interrupt
 */
void _e320_interrupt_call(unsigned long mcuase);

#endif /*__ASM */
#endif /*__E320_INTERNALS_H__ */
