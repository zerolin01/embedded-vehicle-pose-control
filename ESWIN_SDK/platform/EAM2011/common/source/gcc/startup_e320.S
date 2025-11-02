/*
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
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
/******************************************************************************
 * \file     startup_hbird.S
 * \brief    NMSIS Nuclei N/NX Class Core based Core Device Startup File for
 *  Nuclei HummingBird evaluation SoC which support Nuclei N/NX class cores
 * \version  V1.00
 * \date     17. Dec 2019
 *
 ******************************************************************************/

#include "e320.h"
#include "riscv_encoding.h"

/* -------------------------------------------------------------------------- */
// Stack definitions
/* -------------------------------------------------------------------------- */

// Stacks
// Memory layout:
//  +--------------------+ <- stack_top
//  |     stack(s)       |
//  +--------------------+ <- stack_bottom
//  |      heap          |
//  +--------------------+
//  | text, data, bss..  |
//  +--------------------+

// Reserved stack size
    .extern _e320_stack_top
    .extern _e320_stack_bottom
    .extern _e320_stack_core

#if (CONFIG_RV_CORE_NUM > 1)
    .extern _e320_stack_core
    .weak  secondary_main
secondary_main:
    call main
    ret
#endif

/*----------------------------------------------------- */
/* reset register
/*------------------------------------------------------*/
.macro RESET_REGISTER
    mv x1, x0
    mv x2, x0
    mv x3, x0
    mv x4, x0
    mv x5, x0
    mv x6, x0
    mv x7, x0
    mv x8, x0
    mv x9, x0
    mv x10, x0
    mv x11, x0
    mv x12, x0
    mv x13, x0
    mv x14, x0
    mv x15, x0
#ifndef __riscv_32e
    mv x16, x0
    mv x17, x0
    mv x18, x0
    mv x19, x0
    mv x20, x0
    mv x21, x0
    mv x22, x0
    mv x23, x0
    mv x24, x0
    mv x25, x0
    mv x26, x0
    mv x27, x0
    mv x28, x0
    mv x29, x0
    mv x30, x0
    mv x31, x0
#endif
.endm

#ifdef EMPS_FMC_INIT
.macro FMC_INIT
    li t0, 0x44044000 /* fmc_ctrl_cfg_type_t */
    li t1, 0xFF0000DC
    sw t1, (t0)
    li t0, 0x44044124 /* fmc_fmc_normal_int_msk */
    li t1, 0x07
    sw t1, (t0)
    li t0, 0x44044144 /* fmc_fmc_warning_int_msk */
    li t1, 0x3F
    sw t1, (t0)
.endm
#endif

.macro TIM1_INIT
    li a0, 0x1c010000
    li a1, 0x1c010000 + 0x10000
    bgeu a0, a1, 2f
1:
    sw zero, (a0)
    addi a0, a0, 4
    bltu a0, a1, 1b
2:
.endm

.macro INVAL_DCACHE OP_ADDR, OP_SIZE, OP_CACHE_LINE_SIZE
    li a0, \OP_ADDR
    li a1, \OP_ADDR + \OP_SIZE
    bgeu a0, a1, 2f
1:
    cbo.inval a0
    addi a0, a0, \OP_CACHE_LINE_SIZE
    bltu a0, a1, 1b
2:
.endm


/* ---------------------------------------------------- */
/* !Exception vectors                                   */
/* --------------------------------------------------   */
/*** Vector Table Code Section ***/
/* Put vector tables to DDR */
.macro DECLARE_CLIC_INT_HANDLER  INT_HDL_NAME
    j clic_\INT_HDL_NAME
.endm

.macro DECLARE_CLINT_INT_HANDLER  INT_HDL_NAME
    j clint_\INT_HDL_NAME
.endm

.macro DECLARE_CLIC_FAST_INT_HANDLER INT_NUM
    DECLARE_CLIC_INT_HANDLER default_intexc_handler_\INT_NUM
.endm

    .section .vtable
    .global clic_vector_base
    .global clic_vector_table
    .global _start

    .align 10

#ifndef CONFIG_RV_CLIC_VEC_ADDR
// #ifdef CONFIG_RV_CLIC_VEC_ADDR
clic_vector_table:
    j _start                                                   /* 0: Reserved, default handler for Flash download mode */
    DECLARE_CLIC_INT_HANDLER     default_intexc_handler_1      /* 1: Reserved */
    DECLARE_CLIC_INT_HANDLER     default_intexc_handler_2      /* 2: Reserved */
    DECLARE_CLIC_INT_HANDLER     msip_handler                  /* 3: Machine software interrupt */

    DECLARE_CLIC_INT_HANDLER     default_intexc_handler_4      /* 4: Reserved */
    DECLARE_CLIC_INT_HANDLER     default_intexc_handler_5      /* 5: Reserved */
    DECLARE_CLIC_INT_HANDLER     default_intexc_handler_6      /* 6: Reserved */
    DECLARE_CLIC_INT_HANDLER     mtip_handler                  /* 7: Machine timer interrupt */

    DECLARE_CLIC_INT_HANDLER     default_intexc_handler_8      /* 8: Reserved */
    DECLARE_CLIC_INT_HANDLER     default_intexc_handler_9      /* 9: Reserved */
    DECLARE_CLIC_INT_HANDLER     default_intexc_handler_10     /* 10: Reserved */
    DECLARE_CLIC_INT_HANDLER     meip_handler                  /* 11: Machine external interrupt */

    DECLARE_CLIC_INT_HANDLER     csip_handler                  /* 12: Clic Software Interrupt */
    DECLARE_CLIC_INT_HANDLER     default_intexc_handler_13     /* 13: Reserved */
    DECLARE_CLIC_INT_HANDLER     default_intexc_handler_14     /* 14: Reserved */
    DECLARE_CLIC_INT_HANDLER     default_intexc_handler_15     /* 15: Reserved */

    /* fast interrupts */
    .altmacro
    .set fast_int_no, 16
    .rept 113
    DECLARE_CLIC_FAST_INT_HANDLER %fast_int_no                 /* 16..: Reserved */
    .set fast_int_no, fast_int_no + 1
    .endr
#endif

    .global clint_vector_base
    .align 2
clint_vector_base:
    j 							  _start						  /* 0: Reserved */
    DECLARE_CLINT_INT_HANDLER     default_intexc_handler_1        /* 1: Reserved */
    DECLARE_CLINT_INT_HANDLER     default_intexc_handler_2        /* 2: Reserved */
    DECLARE_CLINT_INT_HANDLER     msip_handler                    /* 3: Machine software interrupt */
    DECLARE_CLINT_INT_HANDLER     default_intexc_handler_4        /* 4: Reserved */
    DECLARE_CLINT_INT_HANDLER     default_intexc_handler_5        /* 5: Reserved */
    DECLARE_CLINT_INT_HANDLER     default_intexc_handler_6        /* 6: Reserved */
    DECLARE_CLINT_INT_HANDLER     mtip_handler                    /* 7: Machine timer interrupt */
    DECLARE_CLINT_INT_HANDLER     default_intexc_handler_8        /* 8: Reserved */
    DECLARE_CLINT_INT_HANDLER     default_intexc_handler_9        /* 9: Reserved */
    DECLARE_CLINT_INT_HANDLER     default_intexc_handler_10       /* 10: Reserved */
    DECLARE_CLINT_INT_HANDLER     meip_handler                    /* 11: Machine external interrupt */
    DECLARE_CLINT_INT_HANDLER     default_intexc_handler_12       /* 12: CLINT software interrupt */
    DECLARE_CLINT_INT_HANDLER     default_intexc_handler_13       /* 13: Reserved */
    DECLARE_CLINT_INT_HANDLER     default_intexc_handler_14       /* 14: Reserved */
    DECLARE_CLINT_INT_HANDLER     default_intexc_handler_15       /* 15: Reserved */

/* -------------------------------------------------------------------------- */
/* !Main entry point */
/* -------------------------------------------------------------------------- */

    .section .init

    .extern _e320_board_init

    .globl _start
    .type _start,@function
/**
 * Reset Handler called on controller reset
 */
_start:
    /* ===== Startup Stage 1 ===== */
    /* clear all general registers */
#ifdef EMPS_SIMULATION
    RESET_REGISTER
#ifdef EMPS_FMC_INIT
    FMC_INIT
#endif
#endif
    /* Disable Global Interrupt */
    csrc CSR_MSTATUS, MSTATUS_MIE

    /* Invalid All Cache */
    INVAL_DCACHE 0x12100000, 0x00100000, 0x10

	li t0, 0x44044000 /* fmc_ctrl_cfg_type_t */
	li t1, 0xFFFF00DC
	sw t1, (t0)

    /* Initialize GP and Stack Pointer SP */
    .option push
    .option norelax
    la gp, __global_pointer$
    .option pop

    /* setup vector table */
    la t0, clic_vector_table
    csrw CSR_MTVT, t0
    la t0, exception_entry
    csrw CSR_MTVEC, t0

    /* set the interrupt processing mode to clint direct mode */
    li t0, 0x3f
    csrc CSR_MTVEC, t0
    csrs CSR_MTVEC, 0x0

#ifdef __riscv_flen
    /* Enable FPU */
    li t0, MSTATUS_FS
    csrs mstatus, t0
    csrw fcsr, x0
#endif

    /* Enable mcycle and minstret counter */
    csrci CSR_MCOUNTINHIBIT, 0x5

#if (CONFIG_RV_CORE_NUM > 1)
    csrr a0, CSR_MHARTID
    beqz a0, .Lcore0

    /* loop for core0 bootup */
    la t0, startup_sync
    li t1, 0
    li t2, 1
    sw t1, 0(t0)
.Lspin:
    lw t1, 0(t0)
    bne t1, t2, .Lspin

    /* Initialize core i stack */
    la t0, _e320_stack_core
    lw t0, 0(t0)
    /* generate offset for core i, a0 holds the hart id */
    slli t1, a0, 2
    /* add to array base */
    add t0, t0, t1
    /* load pointer to the stack top and set sp */
    lw t1, 0(t0)
    add sp, t1, x0

    /* the slave cores are done, jump to main part */
    j .Linit_done

    /* only core 0 executes the initialization code */
.Lcore0:
#endif

    /* Load code section if necessary */
    la a0, _ilm_lma
    la a1, _ilm
    /* If the ILM phy-address same as the logic-address, then quit */
    beq a0, a1, 2f
    la a2, _eilm
    bgeu a1, a2, 2f
1:
    /* Load code section if necessary */
    lw t0, (a0)
    sw t0, (a1)
    addi a0, a0, 4
    addi a1, a1, 4
    bltu a1, a2, 1b
2:
    /* Load timcode section if necessary */
    la a0, _ramcode_lma
    la a1, _ramcode_vma
    /* If the timcode phy-address same as the logic-address, then quit */
    beq a0, a1, 2f
    la a2, _ramcode_end
    bgeu a1, a2, 2f
1:
    /* Load code section if necessary */
    lw t0, (a0)
    sw t0, (a1)
    addi a0, a0, 4
    addi a1, a1, 4
    bltu a1, a2, 1b
2:
    /* Load data section */
    la a0, _data_lma
    la a1, _data
    beq a0, a1, 2f
    la a2, _edata
    bgeu a1, a2, 2f
1:
    lw t0, (a0)
    sw t0, (a1)
    addi a0, a0, 4
    addi a1, a1, 4
    bltu a1, a2, 1b
2:
    /* Clear bss section */
    la a0, __bss_start
    la a1, end
    bgeu a0, a1, 2f
1:
    sw zero, (a0)
    addi a0, a0, 4
    bltu a0, a1, 1b

#ifdef EMPS_SIMULATION
2:
    /* Clear stack section */
    la a0, _stack_end
    la a1, _stack_top
    bgeu a0, a1, 2f
1:
    sw zero, (a0)
    addi a0, a0, 4
    bltu a0, a1, 1b
#endif

2:
    /* load amo section */
    la a0, _amo_lma
    la a1, _amo
    la a2, _amo_end
    bgeu a1, a2, 2f
1:
    lw t0, (a0)
    sw t0, (a1)
    addi a0, a0, 4
    addi a1, a1, 4
    bltu a1, a2, 1b
2:
    /* TODO: setup stack top & exception top */
    /* calcualte the whole stack size */
    la t1, _stack_top
    la t2, _stack_end
    sw t1, 0(t1)
    sw t2, 0(t2)
    sub t0, t1, t2
    la t1, _e320_stack_size
    sw t0, 0(t1)
    addi t1, x0, CONFIG_RV_CORE_NUM
    mul t0, t0, t1

    /* set stack top and stack bottom */
    la t1, _stack_top
    la t2, _e320_stack_top
    sw t1, 0(t2)
    la t2, _e320_stack_bottom
    sub t1, t1, t0
    sw t1, 0(t2)

    /* setup stack for core 0 */
    la t0, _e320_stack_top
    lw t1, 0(t0)
    add sp, t1, x0

    /* Reinitialize the e320 support lib */
    call _e320_init_pre
#ifndef USE_NATIVE_INTERFACE
    /* Reinitialize the reentrancy structure */
    call _kitty_libc_impure_init

    /* Call global and static constructors */
    call __libc_init_array

    /* Call C/C++ constructor start up code */
    la a0, __libc_fini_array
    call atexit
#endif
    /* Call board init */
    call _e320_init_single

#if (CONFIG_RV_CORE_NUM > 1)
    la t0, startup_sync
.Lwait_zero:
    lw t1, 0(t0)
    bnez t1, .Lwait_zero
    li t2, 1
    sw t2, 0(t0)
#endif

.Linit_done:
    /* Give Core change to do multicore init */
    call _e320_init_multicore

    /* argc = argv = 0 */
    li a0, 0
    li a1, 0
#if (CONFIG_RV_CORE_NUM > 1)
    csrr t0, CSR_MHARTID
    beqz t0, 1f
    call secondary_main
    j .Lloop
1:
#endif
#ifdef RTOS_RTTHREAD
    // Call entry function when using RT-Thread
    call entry
#else
    call main
#endif

#if (CONFIG_RV_CORE_NUM > 1)
    /* core 0 used to deinit the lib */
    csrr a0, CSR_MHARTID
    bnez a0, .Lloop

.Lcheck_stop:
    /* check the other cores */
    addi t1, x0, CONFIG_RV_CORE_NUM
    lw t2, 0(t0)
    bne t1, t2, .Lcheck_stop
#endif
.Lloop:
    j .Lloop
