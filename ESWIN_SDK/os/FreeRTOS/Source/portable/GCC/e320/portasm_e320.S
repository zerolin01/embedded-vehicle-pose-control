
/*
 * FreeRTOS Kernel V10.2.1
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#include "riscv_encoding.h"
#include "e320.h"

/*
#ifndef __riscv_32e
#define portRegNum          30
#else
#define portRegNum          14
#endif
*/

#if CONFIG_RV_FPU_PRESENT
#define fpuRegNum           21
#else
#define fpuRegNum           0
#endif

#ifndef __riscv_32e
#define portRegNum          (30 + fpuRegNum)
#else
#define portRegNum          (14 + fpuRegNum)
#endif

#if __riscv_xlen == 32
    #define store_x sw
    #define load_x lw
#elif __riscv_xlen == 64
    #define store_x sd
    #define load_x ld
#endif

#define portWORD_SIZE                                   (__riscv_xlen / 8)

/* used in assembler, as byte offsets from the start of the context */
#define PORT_CONTEXT_xIDX(X)            (X) /* index into "raw" for register x? */
#define PORT_CONTEXT_mcauseIDX          (32)
#define PORT_CONTEXT_mepcIDX        (33)
#define PORT_CONTEXT_mstatusIDX     (34)

/* used in assembler, as byte offsets from the start of the context */
#define PORT_CONTEXT_xOFFSET( X )   (PORT_CONTEXT_xIDX(X)    * portWORD_SIZE)
#define PORT_CONTEXT_mcauseOFFSET   (PORT_CONTEXT_mcauseIDX  * portWORD_SIZE)
#define PORT_CONTEXT_mepcOFFSET     (PORT_CONTEXT_mepcIDX    * portWORD_SIZE)
#define PORT_CONTEXT_mstatusOFFSET  (PORT_CONTEXT_mstatusIDX * portWORD_SIZE)
/* total size of the structure usable in ASM. */
#define portasmREGISTER_CONTEXT_WORDSIZE                ((36) * portWORD_SIZE)


#define portCONTEXT_SIZE    ( portRegNum * REGBYTES )


/*************************************************************/


.section    .text.entry
.align 8

.extern xPortTaskSwitch
.extern pxCurrentTCB
.extern xISRStackTop
.global prvPortStartFirstTask
.global xPortRestoreBeforeFirstTask
.global xPortMoveISRStackTop

.global reg_store
.global reg_restore

/* add for pmu test */
/* a0:store address a1:saved general registers's addr  */
reg_store:
    csrrw sp, mscratch, sp

    /* restore register from pointer */
    addi a1, a1, 4
    lw gp, 8(a1)
    lw tp, 12(a1)
    lw t0, 16(a1)
    lw t1, 20(a1)
    lw t2, 24(a1)
    lw s0, 28(a1)
    lw s1, 32(a1)
//    lw a0, 36(a1)
//    lw a1, 40(a1)
    lw a2, 44(a1)
    lw a3, 48(a1)
    lw a4, 52(a1)
    lw a5, 56(a1)
    lw a6, 60(a1)
    lw a7, 64(a1)
    lw s2, 68(a1)
    lw s3, 72(a1)
    lw s4, 76(a1)
    lw s5, 80(a1)
    lw s6, 84(a1)
    lw s7, 88(a1)
    lw s8, 92(a1)
    lw s9, 96(a1)
    lw s10, 100(a1)
    lw s11, 104(a1)
    lw t3, 108(a1)
    lw t4, 112(a1)
    lw t5, 116(a1)
    lw t6, 120(a1)

    /* store general purpose registers */
//    csrr sp, mscratch
    mv sp, a0

    sw ra, 0(sp)
//    sw sp, 4(sp)
    sw gp, 8(sp)
    sw tp, 12(sp)
    sw t0, 16(sp)
    sw t1, 20(sp)
    sw t2, 24(sp)
    sw s0, 28(sp)
    sw s1, 32(sp)
//    sw a0, 36(sp)

//    lw a1, 40(a1)
//    sw a1, 40(sp)

    sw a2, 44(sp)
    sw a3, 48(sp)
    sw a4, 52(sp)
    sw a5, 56(sp)
    sw a6, 60(sp)
    sw a7, 64(sp)
    sw s2, 68(sp)
    sw s3, 72(sp)
    sw s4, 76(sp)
    sw s5, 80(sp)
    sw s6, 84(sp)
    sw s7, 88(sp)
    sw s8, 92(sp)
    sw s9, 96(sp)
    sw s10, 100(sp)
    sw s11, 104(sp)
    sw t3, 108(sp)
    sw t4, 112(sp)
    sw t5, 116(sp)

    /* store float registers */
    fsw ft0, 120(sp)
    fsw ft1, 124(sp)
    fsw ft2, 128(sp)
    fsw ft3, 132(sp)
    fsw ft4, 136(sp)
    fsw ft5, 140(sp)
    fsw ft6, 144(sp)
    fsw ft7, 148(sp)
    fsw fs0, 152(sp)
    fsw fs1, 156(sp)
    fsw fa0, 160(sp)
    fsw fa1, 164(sp)
    fsw fa2, 168(sp)
    fsw fa3, 172(sp)
    fsw fa4, 176(sp)
    fsw fa5, 180(sp)
    fsw fa6, 184(sp)
    fsw fa7, 188(sp)
    fsw fs2, 192(sp)
    fsw fs3, 196(sp)
    fsw fs4, 200(sp)
    fsw fs5, 204(sp)
    csrr t0, fcsr
    sw t0, 248(sp)
    /* store control registers */
    csrr t0, mepc
    sw t0, 252(sp)
    csrr t0, mstatus
    sw t0, 256(sp)
    csrr t0, mtvec
    sw t0, 260(sp)

    // save a1 and sp register at the last, a1 saved in 40(a1), sp saved in 4(a1)
    lw t0, 36(a1)        // a0
    sw t0, 36(sp)
    lw t0, 40(a1)        // a1
    sw t0, 40(sp)
    lw t0, 4(a1)        // sp
    sw t0, 4(sp)

    csrr sp, mscratch

    ret

reg_restore:
    mv sp, a0
    /* restore general purpose registers */
    lw ra, 0(sp)
    lw gp, 8(sp)
    lw tp, 12(sp)
    // lw t0, 16(sp)    // t0 will be used as temp, so restore in the last.
    lw t1, 20(sp)
    lw t2, 24(sp)
    lw s0, 28(sp)
    lw s1, 32(sp)
    lw a0, 36(sp)
    lw a1, 40(sp)
    lw a2, 44(sp)
    lw a3, 48(sp)
    lw a4, 52(sp)
    lw a5, 56(sp)
    lw a6, 60(sp)
    lw a7, 64(sp)
    lw s2, 68(sp)
    lw s3, 72(sp)
    lw s4, 76(sp)
    lw s5, 80(sp)
    lw s6, 84(sp)
    lw s7, 88(sp)
    lw s8, 92(sp)
    lw s9, 96(sp)
    lw s10, 100(sp)
    lw s11, 104(sp)
    lw t3, 108(sp)
    lw t4, 112(sp)
    lw t5, 116(sp)

    /* restore float registers */
    flw ft0, 120(sp)
    flw ft1, 124(sp)
    flw ft2, 128(sp)
    flw ft3, 132(sp)
    flw ft4, 136(sp)
    flw ft5, 140(sp)
    flw ft6, 144(sp)
    flw ft7, 148(sp)
    flw fs0, 152(sp)
    flw fs1, 156(sp)
    flw fa0, 160(sp)
    flw fa1, 164(sp)
    flw fa2, 168(sp)
    flw fa3, 172(sp)
    // return to ecall
    flw fa4, 176(sp)
    flw fa5, 180(sp)
    flw fa6, 184(sp)
    flw fa7, 188(sp)
    flw fs2, 192(sp)
    flw fs3, 196(sp)
    flw fs4, 200(sp)
    flw fs5, 204(sp)
    flw fs6, 208(sp)
    flw fs7, 212(sp)
    flw fs8, 216(sp)
    flw fs9, 220(sp)
    flw fs10, 224(sp)
    flw fs11, 228(sp)
    flw ft8, 232(sp)
    flw ft9, 236(sp)
    flw ft10, 240(sp)
    flw ft11, 244(sp)

    /* restore control registers */
    lw t0, 248(sp)
    csrw fcsr, t0
    lw t0, 252(sp)
    addi t0, t0, 4
    csrw mepc, t0
    lw t0, 256(sp)
    csrw mstatus, t0
    lw t0, 260(sp)
    csrw mtvec, t0

    /* restore t0 register */
    lw t0, 16(sp)
    /* restore sp register */
    lw sp, 4(sp)
    addi sp, sp, 54*REGBYTES

    // return to ecall
    mret
/* add for pmu test */


/*************************************************************/

.macro portSAVE_BaseReg
        /* Make room for the registers. */
        addi    sp, sp, -portasmREGISTER_CONTEXT_WORDSIZE
        /* x1(ra)                       Return address */
        store_x  x1,  PORT_CONTEXT_xOFFSET(1)(sp)
        /* x2(sp) ***** Should be save ouside this macro */
        /* x3(gp)                       Global pointer */
        store_x  x3,  PORT_CONTEXT_xOFFSET(3)(sp)
        /* x4(tp)                       Thread pointer */
        store_x  x4,  PORT_CONTEXT_xOFFSET(4)(sp)
        /* x5-7(t0-2)           Temporaries */
        store_x  x5,  PORT_CONTEXT_xOFFSET(5)(sp)
        store_x  x6,  PORT_CONTEXT_xOFFSET(6)(sp)
        store_x  x7,  PORT_CONTEXT_xOFFSET(7)(sp)
        /* x8(s0/fp)            Saved register/Frame pointer */
        store_x  x8,  PORT_CONTEXT_xOFFSET(8)(sp)
        /* x9(s1)                       Saved register */
        store_x  x9,  PORT_CONTEXT_xOFFSET(9)(sp)
        /* x10-17(a0-7)         Function arguments */
        store_x  x10, PORT_CONTEXT_xOFFSET(10)(sp)
        store_x  x11, PORT_CONTEXT_xOFFSET(11)(sp)
        store_x  x12, PORT_CONTEXT_xOFFSET(12)(sp)
        store_x  x13, PORT_CONTEXT_xOFFSET(13)(sp)
        store_x  x14, PORT_CONTEXT_xOFFSET(14)(sp)
        store_x  x15, PORT_CONTEXT_xOFFSET(15)(sp)
        store_x  x16, PORT_CONTEXT_xOFFSET(16)(sp)
        store_x  x17, PORT_CONTEXT_xOFFSET(17)(sp)
        /* x18-27(s2-11)                Saved registers */
        store_x  x18, PORT_CONTEXT_xOFFSET(18)(sp)
        store_x  x19, PORT_CONTEXT_xOFFSET(19)(sp)
        store_x  x20, PORT_CONTEXT_xOFFSET(20)(sp)
        store_x  x21, PORT_CONTEXT_xOFFSET(21)(sp)
        store_x  x22, PORT_CONTEXT_xOFFSET(22)(sp)
        store_x  x23, PORT_CONTEXT_xOFFSET(23)(sp)
        store_x  x24, PORT_CONTEXT_xOFFSET(24)(sp)
        store_x  x25, PORT_CONTEXT_xOFFSET(25)(sp)
        store_x  x26, PORT_CONTEXT_xOFFSET(26)(sp)
        store_x  x27, PORT_CONTEXT_xOFFSET(27)(sp)
        /* x28-31(t3-6)         Temporaries */
        store_x  x28, PORT_CONTEXT_xOFFSET(28)(sp)
        store_x  x29, PORT_CONTEXT_xOFFSET(29)(sp)
        store_x  x30, PORT_CONTEXT_xOFFSET(30)(sp)
        store_x  x31, PORT_CONTEXT_xOFFSET(31)(sp)

        /* Save mcause, mepc & mstatus state */
        csrr s2, mcause
        csrr s3, mepc
        csrr s4, mstatus                /* Required for MPIE bit. */
        store_x s2, PORT_CONTEXT_mcauseOFFSET(sp)
        store_x s3, PORT_CONTEXT_mepcOFFSET(sp)
        store_x s4, PORT_CONTEXT_mstatusOFFSET(sp)

        .endm
/*************************************************************/

.macro portRESTORE_BaseReg
        /* Restore mepc & mstatus state */
        load_x  s3, PORT_CONTEXT_mepcOFFSET(sp)
        load_x  s4, PORT_CONTEXT_mstatusOFFSET(sp)
        csrw    mepc, s3
        csrw    mstatus, s4

        /* x1(ra)                       Return address */
        load_x  x1,  PORT_CONTEXT_xOFFSET(1)(sp)
        /* x2(sp) ***** Should be restore outside this macro */
        /* x3(gp)                       Global pointer */
        load_x  x3,  PORT_CONTEXT_xOFFSET(3)(sp)
        /* x4(tp)                       Thread pointer */
        load_x  x4,  PORT_CONTEXT_xOFFSET(4)(sp)
        /* x5-7(t0-2)           Temporaries */
        load_x  x5,  PORT_CONTEXT_xOFFSET(5)(sp)
        load_x  x6,  PORT_CONTEXT_xOFFSET(6)(sp)
        load_x  x7,  PORT_CONTEXT_xOFFSET(7)(sp)
        /* x8(s0/fp)            Saved register/Frame pointer */
        load_x  x8,  PORT_CONTEXT_xOFFSET(8)(sp)
        /* x9(s1)                       Saved register */
        load_x  x9,  PORT_CONTEXT_xOFFSET(9)(sp)
        /* x10-17(a0-7)         Function arguments */
        load_x  x10, PORT_CONTEXT_xOFFSET(10)(sp)
        load_x  x11, PORT_CONTEXT_xOFFSET(11)(sp)
        load_x  x12, PORT_CONTEXT_xOFFSET(12)(sp)
        load_x  x13, PORT_CONTEXT_xOFFSET(13)(sp)
        load_x  x14, PORT_CONTEXT_xOFFSET(14)(sp)
        load_x  x15, PORT_CONTEXT_xOFFSET(15)(sp)
        load_x  x16, PORT_CONTEXT_xOFFSET(16)(sp)
        load_x  x17, PORT_CONTEXT_xOFFSET(17)(sp)
        /* x18-27(s2-11)                Saved registers */
        load_x  x18, PORT_CONTEXT_xOFFSET(18)(sp)
        load_x  x19, PORT_CONTEXT_xOFFSET(19)(sp)
        load_x  x20, PORT_CONTEXT_xOFFSET(20)(sp)
        load_x  x21, PORT_CONTEXT_xOFFSET(21)(sp)
        load_x  x22, PORT_CONTEXT_xOFFSET(22)(sp)
        load_x  x23, PORT_CONTEXT_xOFFSET(23)(sp)
        load_x  x24, PORT_CONTEXT_xOFFSET(24)(sp)
            load_x  x25, PORT_CONTEXT_xOFFSET(25)(sp)
        load_x  x26, PORT_CONTEXT_xOFFSET(26)(sp)
        load_x  x27, PORT_CONTEXT_xOFFSET(27)(sp)
        /* x28-31(t3-6)         Temporaries */
        load_x  x28, PORT_CONTEXT_xOFFSET(28)(sp)
        load_x  x29, PORT_CONTEXT_xOFFSET(29)(sp)
        load_x  x30, PORT_CONTEXT_xOFFSET(30)(sp)
        load_x  x31, PORT_CONTEXT_xOFFSET(31)(sp)
        .endm
/*************************************************************/

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
    addi sp, sp, -54*REGBYTES
#else
    addi sp, sp, -32*REGBYTES
#endif
    /* save base register: 31 registers */
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
    /* save float register: 20 registers*/
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
    STORE   t0, 52*REGBYTES(sp)
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
    /* restore float register */
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
    csrw    fcsr, t0

    addi sp, sp, 54*REGBYTES
#else
    addi sp, sp, 32*REGBYTES
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
/* In CLIC mode, the exeception entry must be 64bytes aligned */
.align 6
.global exception_entry
exception_entry:
    /* Save the caller saving registers (context) */
    SAVE_CONTEXT
    /* Save the necessary CSR registers */
    // SAVE_CSR_CONTEXT

    /*
     * Set the exception handler function arguments
     * argument 1: mcause value
     * argument 2: current stack point(SP) value
     */
    csrr a0, mcause
    mv a1, sp
    /*
     * TODO: Call the exception handler function
     * By default, the function template is provided in
     * system_Device.c, you can adjust it as you want
     */
    // call _kittyhaps_exception_call
    call _e320_exception_call

    /* Restore the necessary CSR registers */
    // RESTORE_CSR_CONTEXT
    /* Restore the caller saving registers (context) */
    RESTORE_CONTEXT

    mret

    /* Return to regular code */
default_intexc_handler:
1:
    j 1b

/* Start the first task.  This also clears the bit that indicates the FPU is
    in use in case the FPU was used before the scheduler was started - which
    would otherwise result in the unnecessary leaving of space in the stack
    // return to ecall
    for lazy saving of FPU registers. */
.align 3
prvPortStartFirstTask:
    /* Setup Interrupt Stack using
       The stack that was used by main()
       before the scheduler is started is
       no longer required after the scheduler is started.
       Interrupt stack pointer is stored in CSR_MSCRATCH */
/*
    csrw CSR_MSCRATCH, sp

    load_x  sp, xISRStackTop
    addi sp, sp, portasmREGISTER_CONTEXT_WORDSIZE

    portSAVE_BaseReg


    csrrw       t0, CSR_MSCRATCH, t0
    // SP = X2, so save it
    store_x     t0, PORT_CONTEXT_xOFFSET(2)(sp)
    // x1       ra Return address
    store_x  x1,  PORT_CONTEXT_xOFFSET(1)(sp)

*/
    la t0, _stack_top
    csrw CSR_MSCRATCH, t0

    LOAD sp, pxCurrentTCB           /* Load pxCurrentTCB. */
    LOAD sp, 0x0(sp)                /* Read sp from first TCB member */

    /* Pop PC from stack and set MEPC */
    LOAD t0,  0  * REGBYTES(sp)
    csrw CSR_MEPC, t0
    /* Pop mstatus from stack and set it */
    LOAD t0,  (portRegNum - 1)  * REGBYTES(sp)
    csrw CSR_MSTATUS, t0
    /* Interrupt still disable here */
    /* Restore Registers from Stack */
    LOAD x1,  1  * REGBYTES(sp)    /* RA */
    LOAD x5,  2  * REGBYTES(sp)
    LOAD x6,  3  * REGBYTES(sp)
    LOAD x9,  6  * REGBYTES(sp)
    LOAD x10, 7  * REGBYTES(sp)
    LOAD x11, 8  * REGBYTES(sp)
    LOAD x12, 9  * REGBYTES(sp)
    LOAD x13, 10 * REGBYTES(sp)
    LOAD x14, 11 * REGBYTES(sp)
    LOAD x15, 12 * REGBYTES(sp)
#ifndef __riscv_32e
    LOAD x16, 13 * REGBYTES(sp)
    LOAD x17, 14 * REGBYTES(sp)
    LOAD x18, 15 * REGBYTES(sp)
    LOAD x19, 16 * REGBYTES(sp)
    LOAD x20, 17 * REGBYTES(sp)
    LOAD x21, 18 * REGBYTES(sp)
    LOAD x22, 19 * REGBYTES(sp)
    LOAD x23, 20 * REGBYTES(sp)
    LOAD x24, 21 * REGBYTES(sp)
    LOAD x25, 22 * REGBYTES(sp)
    LOAD x26, 23 * REGBYTES(sp)
    LOAD x27, 24 * REGBYTES(sp)
    LOAD x28, 25 * REGBYTES(sp)
    LOAD x29, 26 * REGBYTES(sp)
    LOAD x30, 27 * REGBYTES(sp)
    LOAD x31, 28 * REGBYTES(sp)
#endif
#if CONFIG_RV_FPU_PRESENT
    FPLOAD f0, 29*REGBYTES(sp)
    FPLOAD f1, 30*REGBYTES(sp)
    FPLOAD f2, 31*REGBYTES(sp)
    FPLOAD f3, 32*REGBYTES(sp)
    FPLOAD f4, 33*REGBYTES(sp)
    FPLOAD f5, 34*REGBYTES(sp)
    FPLOAD f6, 35*REGBYTES(sp)
    FPLOAD f7, 36*REGBYTES(sp)
    FPLOAD f10, 37*REGBYTES(sp)
    FPLOAD f11, 38*REGBYTES(sp)
    FPLOAD f12, 39*REGBYTES(sp)
    FPLOAD f13, 40*REGBYTES(sp)
    FPLOAD f14, 41*REGBYTES(sp)
    FPLOAD f15, 42*REGBYTES(sp)
    FPLOAD f16, 43*REGBYTES(sp)
    FPLOAD f17, 44*REGBYTES(sp)
    FPLOAD f28, 45*REGBYTES(sp)
    FPLOAD f29, 46*REGBYTES(sp)
    FPLOAD f30, 47*REGBYTES(sp)
    FPLOAD f31, 48*REGBYTES(sp)
    lw      t0, 49*REGBYTES(sp)
    csrw    fcsr, t0
#endif
    addi sp, sp, portCONTEXT_SIZE

    mret

.align 2
.global clic_csip_handler
clic_csip_handler:
//    SAVE_CONTEXT
    addi sp, sp, -portCONTEXT_SIZE
    STORE x1,  1  * REGBYTES(sp)
    STORE x5,  2  * REGBYTES(sp)
    STORE x6,  3  * REGBYTES(sp)
    STORE x7,  4  * REGBYTES(sp)
    STORE x8,  5  * REGBYTES(sp)
    STORE x9,  6  * REGBYTES(sp)
    STORE x10, 7  * REGBYTES(sp)
    STORE x11, 8  * REGBYTES(sp)
    STORE x12, 9  * REGBYTES(sp)
    STORE x13, 10 * REGBYTES(sp)
    STORE x14, 11 * REGBYTES(sp)
    STORE x15, 12 * REGBYTES(sp)
#ifndef __riscv_32e
    STORE x16, 13 * REGBYTES(sp)
    STORE x17, 14 * REGBYTES(sp)
    STORE x18, 15 * REGBYTES(sp)
    STORE x19, 16 * REGBYTES(sp)
    STORE x20, 17 * REGBYTES(sp)
    STORE x21, 18 * REGBYTES(sp)
    STORE x22, 19 * REGBYTES(sp)
    STORE x23, 20 * REGBYTES(sp)
    STORE x24, 21 * REGBYTES(sp)
    STORE x25, 22 * REGBYTES(sp)
    STORE x26, 23 * REGBYTES(sp)
    STORE x27, 24 * REGBYTES(sp)
    STORE x28, 25 * REGBYTES(sp)
    STORE x29, 26 * REGBYTES(sp)
    STORE x30, 27 * REGBYTES(sp)
    STORE x31, 28 * REGBYTES(sp)
#endif

#if CONFIG_RV_FPU_PRESENT
    FPSTORE f0, 29*REGBYTES(sp)
    FPSTORE f1, 30*REGBYTES(sp)
    FPSTORE f2, 31*REGBYTES(sp)
    FPSTORE f3, 32*REGBYTES(sp)
    FPSTORE f4, 33*REGBYTES(sp)
    FPSTORE f5, 34*REGBYTES(sp)
    FPSTORE f6, 35*REGBYTES(sp)
    FPSTORE f7, 36*REGBYTES(sp)
    FPSTORE f10, 37*REGBYTES(sp)
    FPSTORE f11, 38*REGBYTES(sp)
    FPSTORE f12, 39*REGBYTES(sp)
    FPSTORE f13, 40*REGBYTES(sp)
    FPSTORE f14, 41*REGBYTES(sp)
    FPSTORE f15, 42*REGBYTES(sp)
    FPSTORE f16, 43*REGBYTES(sp)
    FPSTORE f17, 44*REGBYTES(sp)
    FPSTORE f28, 45*REGBYTES(sp)
    FPSTORE f29, 46*REGBYTES(sp)
    FPSTORE f30, 47*REGBYTES(sp)
    FPSTORE f31, 48*REGBYTES(sp)
    csrr    t0, fcsr
    STORE   t0, 49*REGBYTES(sp)
#endif
    /* Push mstatus to stack */
    csrr t0, CSR_MSTATUS
    STORE t0,  (portRegNum - 1)  * REGBYTES(sp)

    /* Push additional registers */

    /* Store sp to task stack */
    LOAD t0, pxCurrentTCB
    STORE sp, 0(t0)

    csrr t0, CSR_MEPC
    STORE t0, 0(sp)
    jal xPortTaskSwitch

    /* Switch task context */
    LOAD t0, pxCurrentTCB           /* Load pxCurrentTCB. */
    LOAD sp, 0x0(t0)                /* Read sp from first TCB member */

    /* Pop PC from stack and set MEPC */
    LOAD t0,  0  * REGBYTES(sp)
    csrw CSR_MEPC, t0
    /* Pop additional registers */

    /* Pop mstatus from stack and set it */
    LOAD t0,  (portRegNum - 1)  * REGBYTES(sp)
    csrw CSR_MSTATUS, t0
    /* Interrupt still disable here */
    /* Restore Registers from Stack */
    LOAD x1,  1  * REGBYTES(sp)    /* RA */
    LOAD x5,  2  * REGBYTES(sp)
    LOAD x6,  3  * REGBYTES(sp)
    LOAD x7,  4  * REGBYTES(sp)
    LOAD x8,  5  * REGBYTES(sp)
    LOAD x9,  6  * REGBYTES(sp)
    LOAD x10, 7  * REGBYTES(sp)
    LOAD x11, 8  * REGBYTES(sp)
    LOAD x12, 9  * REGBYTES(sp)
    LOAD x13, 10 * REGBYTES(sp)
    LOAD x14, 11 * REGBYTES(sp)
    LOAD x15, 12 * REGBYTES(sp)
#ifndef __riscv_32e
    LOAD x16, 13 * REGBYTES(sp)
    LOAD x17, 14 * REGBYTES(sp)
    LOAD x18, 15 * REGBYTES(sp)
    LOAD x19, 16 * REGBYTES(sp)
    LOAD x20, 17 * REGBYTES(sp)
    LOAD x21, 18 * REGBYTES(sp)
    LOAD x22, 19 * REGBYTES(sp)
    LOAD x23, 20 * REGBYTES(sp)
    LOAD x24, 21 * REGBYTES(sp)
    LOAD x25, 22 * REGBYTES(sp)
    LOAD x26, 23 * REGBYTES(sp)
    LOAD x27, 24 * REGBYTES(sp)
    LOAD x28, 25 * REGBYTES(sp)
    LOAD x29, 26 * REGBYTES(sp)
    LOAD x30, 27 * REGBYTES(sp)
    LOAD x31, 28 * REGBYTES(sp)
#endif

#if CONFIG_RV_FPU_PRESENT
    FPLOAD f0, 29*REGBYTES(sp)
    FPLOAD f1, 30*REGBYTES(sp)
    FPLOAD f2, 31*REGBYTES(sp)
    FPLOAD f3, 32*REGBYTES(sp)
    FPLOAD f4, 33*REGBYTES(sp)
    FPLOAD f5, 34*REGBYTES(sp)
    FPLOAD f6, 35*REGBYTES(sp)
    FPLOAD f7, 36*REGBYTES(sp)
    FPLOAD f10, 37*REGBYTES(sp)
    FPLOAD f11, 38*REGBYTES(sp)
    FPLOAD f12, 39*REGBYTES(sp)
    FPLOAD f13, 40*REGBYTES(sp)
    FPLOAD f14, 41*REGBYTES(sp)
    FPLOAD f15, 42*REGBYTES(sp)
    FPLOAD f16, 43*REGBYTES(sp)
    FPLOAD f17, 44*REGBYTES(sp)
    FPLOAD f28, 45*REGBYTES(sp)
    FPLOAD f29, 46*REGBYTES(sp)
    FPLOAD f30, 47*REGBYTES(sp)
    FPLOAD f31, 48*REGBYTES(sp)
    lw      t0, 49*REGBYTES(sp)
    csrw    fcsr, t0
#endif
    addi sp, sp, portCONTEXT_SIZE

    // RESTORE_CONTEXT
    mret

/*-----------------------------------------------------------*/
/**
 * void xPortRestoreBeforeFirstTask(void)
 */
.align 8
.type   xPortRestoreBeforeFirstTask, @function
xPortRestoreBeforeFirstTask:
        load_x  sp, xISRStackTop
        portRESTORE_BaseReg
        load_x  x2, PORT_CONTEXT_xOFFSET(2)(sp)
        ret
/*-----------------------------------------------------------*/
/*-----------------------------------------------------------*/

/**
 * BaseType_t xPortMoveISRStackTop( StackType_t *xISRStackTop);
 */
.align 8
.type   xPortMoveISRStackTop, @function
xPortMoveISRStackTop:
        load_x  t0, 0(a0)
        beqz t0, 1f
        addi t1, x0, portasmREGISTER_CONTEXT_WORDSIZE
        bgtu t1, t0, 1f
        sub     t1, t0, t1
        store_x t1, 0(a0)
        mv a0, t1
        j 2f
1:
        li      a0, 0
2:
        ret
/*-----------------------------------------------------------*/
