/*
 * Copyright (c) 2021 ESWIN Limited. All rights reserved.
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
 * \file	intexc_e320.S
 * \brief	EMSIS Interrupt and Exception Handling Template File
 *  for ESWIN HAPS Virtual Board which support ESWIN e320 cores
 * \version V0.1.0
 * \date	13 Jan. 2021
 *
 ******************************************************************************/

#include "riscv_encoding.h"

/**
 * \brief  Macro for context save
 * \details
 * This macro save ABI defined caller saved registers in the stack.
 * \remarks
 * - This Macro could use to save context when you enter to interrupt
 * or exception
*/
/* Save caller registers */
.macro SAVE_CONTEXT
    /* Allocate stack space for context saving */
#if CONFIG_RV_FPU_PRESENT
    addi sp, sp, -212
#else
    addi sp, sp, -128
#endif

    STORE x1, 1*REGBYTES(sp)
    STORE x2, 2*REGBYTES(sp)
    STORE x3, 3*REGBYTES(sp)
    STORE x4, 4*REGBYTES(sp)
    STORE x5, 5*REGBYTES(sp)
    STORE x6, 6*REGBYTES(sp)
    STORE x7, 7*REGBYTES(sp)
    STORE x8, 8*REGBYTES(sp)
    STORE x9, 9*REGBYTES(sp)
    STORE x10, 10*REGBYTES(sp)
    STORE x11, 11*REGBYTES(sp)
    STORE x12, 12*REGBYTES(sp)
    STORE x13, 13*REGBYTES(sp)
    STORE x14, 14*REGBYTES(sp)
    STORE x15, 15*REGBYTES(sp)
    STORE x16, 16*REGBYTES(sp)
    STORE x17, 17*REGBYTES(sp)
    STORE x18, 18*REGBYTES(sp)
    STORE x19, 19*REGBYTES(sp)
    STORE x20, 20*REGBYTES(sp)
    STORE x21, 21*REGBYTES(sp)
    STORE x22, 22*REGBYTES(sp)
    STORE x23, 23*REGBYTES(sp)
    STORE x24, 24*REGBYTES(sp)
    STORE x25, 25*REGBYTES(sp)
    STORE x26, 26*REGBYTES(sp)
    STORE x27, 27*REGBYTES(sp)
    STORE x28, 28*REGBYTES(sp)
    STORE x29, 29*REGBYTES(sp)
    STORE x30, 30*REGBYTES(sp)
    STORE x31, 31*REGBYTES(sp)

#if CONFIG_RV_FPU_PRESENT
    FPSTORE f0, 32*REGBYTES(sp)
    FPSTORE f1, 33*REGBYTES(sp)
    FPSTORE f2, 34*REGBYTES(sp)
    FPSTORE f3, 35*REGBYTES(sp)
    FPSTORE f4, 36*REGBYTES(sp)
    FPSTORE f5, 37*REGBYTES(sp)
    FPSTORE f6, 38*REGBYTES(sp)
    FPSTORE f7, 39*REGBYTES(sp)
    FPSTORE f10, 40*REGBYTES(sp)
    FPSTORE f11, 41*REGBYTES(sp)
    FPSTORE f12, 42*REGBYTES(sp)
    FPSTORE f13, 43*REGBYTES(sp)
    FPSTORE f14, 44*REGBYTES(sp)
    FPSTORE f15, 45*REGBYTES(sp)
    FPSTORE f16, 46*REGBYTES(sp)
    FPSTORE f17, 47*REGBYTES(sp)
    FPSTORE f28, 48*REGBYTES(sp)
    FPSTORE f29, 49*REGBYTES(sp)
    FPSTORE f30, 50*REGBYTES(sp)
    FPSTORE f31, 51*REGBYTES(sp)
    csrr    t0, fcsr
    STORE t0, 52*REGBYTES(sp)
#endif
.endm

/**
 * \brief  Macro for restore caller registers
 * \details
 * This macro restore ABI defined caller saved registers from stack.
 * \remarks
 * - You could use this macro to restore context before you want return
 * from interrupt or exeception
 */
/* Restore caller registers */
.macro RESTORE_CONTEXT
    LOAD x1, 1*REGBYTES(sp)
    LOAD x2, 2*REGBYTES(sp)
    LOAD x3, 3*REGBYTES(sp)
    LOAD x4, 4*REGBYTES(sp)
    LOAD x5, 5*REGBYTES(sp)
    LOAD x6, 6*REGBYTES(sp)
    LOAD x7, 7*REGBYTES(sp)
    LOAD x8, 8*REGBYTES(sp)
    LOAD x9, 9*REGBYTES(sp)
    LOAD x10, 10*REGBYTES(sp)
    LOAD x11, 11*REGBYTES(sp)
    LOAD x12, 12*REGBYTES(sp)
    LOAD x13, 13*REGBYTES(sp)
    LOAD x14, 14*REGBYTES(sp)
    LOAD x15, 15*REGBYTES(sp)
    LOAD x16, 16*REGBYTES(sp)
    LOAD x17, 17*REGBYTES(sp)
    LOAD x18, 18*REGBYTES(sp)
    LOAD x19, 19*REGBYTES(sp)
    LOAD x20, 20*REGBYTES(sp)
    LOAD x21, 21*REGBYTES(sp)
    LOAD x22, 22*REGBYTES(sp)
    LOAD x23, 23*REGBYTES(sp)
    LOAD x24, 24*REGBYTES(sp)
    LOAD x25, 25*REGBYTES(sp)
    LOAD x26, 26*REGBYTES(sp)
    LOAD x27, 27*REGBYTES(sp)
    LOAD x28, 28*REGBYTES(sp)
    LOAD x29, 29*REGBYTES(sp)
    LOAD x30, 30*REGBYTES(sp)
    LOAD x31, 31*REGBYTES(sp)

#if CONFIG_RV_FPU_PRESENT
    FPLOAD f0, 32*REGBYTES(sp)
    FPLOAD f1, 33*REGBYTES(sp)
    FPLOAD f2, 34*REGBYTES(sp)
    FPLOAD f3, 35*REGBYTES(sp)
    FPLOAD f4, 36*REGBYTES(sp)
    FPLOAD f5, 37*REGBYTES(sp)
    FPLOAD f6, 38*REGBYTES(sp)
    FPLOAD f7, 39*REGBYTES(sp)
    FPLOAD f10, 40*REGBYTES(sp)
    FPLOAD f11, 41*REGBYTES(sp)
    FPLOAD f12, 42*REGBYTES(sp)
    FPLOAD f13, 43*REGBYTES(sp)
    FPLOAD f14, 44*REGBYTES(sp)
    FPLOAD f15, 45*REGBYTES(sp)
    FPLOAD f16, 46*REGBYTES(sp)
    FPLOAD f17, 47*REGBYTES(sp)
    FPLOAD f28, 48*REGBYTES(sp)
    FPLOAD f29, 49*REGBYTES(sp)
    FPLOAD f30, 50*REGBYTES(sp)
    FPLOAD f31, 51*REGBYTES(sp)
    lw      t0, 52*REGBYTES(sp)
    csrs    fcsr, t0

    addi sp, sp, 212
#else
    addi sp, sp, 128
#endif
.endm

/**
 * \brief  Macro for save necessary CSRs to stack
 * \details
 * This macro store MCAUSE, MEPC, MSUBM to stack.
 */
.macro SAVE_CSR_CONTEXT
    /* Store CSR mcause to stack using pushmcause */
    csrr x5, CSR_MCAUSE
    STORE x5, 11*REGBYTES(sp)
    /* Store CSR mepc to stack using pushmepc */
    csrr x5, CSR_MEPC
    STORE x5, 12*REGBYTES(sp)
.endm

/**
 * \brief  Macro for restore necessary CSRs from stack
 * \details
 * This macro restore MSUBM, MEPC, MCAUSE from stack.
 */
.macro RESTORE_CSR_CONTEXT
    LOAD x5,  12*REGBYTES(sp)
    csrw CSR_MEPC, x5
    LOAD x5,  11*REGBYTES(sp)
    csrw CSR_MCAUSE, x5
.endm

/**
 * \brief  Exception/NMI Entry
 * \details
 * This function provide common entry functions for exception/nmi.
 * \remarks
 * This function provide a default exception/nmi entry.
 * ABI defined caller save register and some CSR registers
 * to be saved before enter interrupt handler and be restored before return.
 */
.section .text.trap
/* the exeception entry must be 64bytes aligned in e320 class cores */
.align 6
.global exception_entry
.weak exception_entry
exception_entry:
    /* Save the caller saving registers (context) */
    SAVE_CONTEXT
    /* Save the necessary CSR registers */
    SAVE_CSR_CONTEXT

    mv a1, sp

    /*
     * Set the exception handler function arguments
     * argument 0: mcause value
     */
    csrr a0, mcause
    /*
     * TODO: Call the exception handler function
     * By default, the function template is provided in
     * system_Device.c, you can adjust it as you want
     */

    call _e320_exception_call

    /* Restore the necessary CSR registers */
    // RESTORE_CSR_CONTEXT
    /* Restore the caller saving registers (context) */
    RESTORE_CONTEXT

    /* Return to regular code */
    mret

/* Default Handler for Exceptions / Interrupts */
.global clic_default_intexc_handler
.global clint_default_intexc_handler
.global clint_msip_handler
.global clint_mtip_handler
.global clint_meip_handler
.weak clic_default_intexc_handler
.weak clint_default_intexc_handler
.weak clint_msip_handler
.weak clint_meip_handler
.weak clint_mtip_handler
Undef_Handler:
1:
    j 1b

clic_default_intexc_handler:
1:
    j 1b

clint_default_intexc_handler:
1:
    j 1b

clint_msip_handler:
1:
    j 1b

clint_meip_handler:
1:
    j 1b

clint_mtip_handler:
1:
    j 1b

/* Default Handler for Fast Interrupt */
.macro DEFAULT_INT_HANDLER_DEFINE INT_NUM
.global clic_default_intexc_handler_\INT_NUM
.global clint_default_intexc_handler_\INT_NUM
.weak clic_default_intexc_handler_\INT_NUM
.weak clint_default_intexc_handler_\INT_NUM
clic_default_intexc_handler_\INT_NUM :
    j clic_default_intexc_handler

clint_default_intexc_handler_\INT_NUM :
    j clint_default_intexc_handler
.endm

.altmacro
.set int_no, 0
.rept 129
DEFAULT_INT_HANDLER_DEFINE %int_no
.set int_no, int_no + 1
.endr
