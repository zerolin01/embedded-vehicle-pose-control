/*
 * FreeRTOS Kernel V10.5.1
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
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
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#ifndef PORTCONTEXT_H
#define PORTCONTEXT_H

#if __riscv_xlen == 64
    #define portWORD_SIZE 8
    #define store_x sd
    #define load_x ld
    #define fp_load_x  fsd
    #define fp_store_x fsd
#elif __riscv_xlen == 32
    #define store_x sw
    #define load_x lw
    #define portWORD_SIZE 4
    #define fp_load_x flw
    #define fp_store_x fsw
#else
    #error Assembler did not define __riscv_xlen
#endif

#ifndef CONFIG_RV_FPU_PRESENT
#define CONFIG_RV_FPU_PRESENT  1
#endif

#ifdef CONFIG_RV_FPU_PRESENT
#define fpuRegNum           21
#else
#define fpuRegNum           0
#endif

#ifndef __riscv_32e
#define portRegNum          (30 + fpuRegNum)
#else
#define portRegNum          (14 + fpuRegNum)
#endif

#include "freertos_risc_v_chip_specific_extensions.h"

/* Only the standard core registers are stored by default.  Any additional
 * registers must be saved by the portasmSAVE_ADDITIONAL_REGISTERS and
 * portasmRESTORE_ADDITIONAL_REGISTERS macros - which can be defined in a chip
 * specific version of freertos_risc_v_chip_specific_extensions.h.  See the
 * notes at the top of portASM.S file. */

/* used in assembler, as byte offsets from the start of the context */
#define PORT_CONTEXT_mcauseIDX      (32)
#define PORT_CONTEXT_mepcIDX        (33)
#define PORT_CONTEXT_mstatusIDX     (34)

/* used in assembler, as byte offsets from the start of the context */
#define PORT_CONTEXT_mcauseOFFSET   (PORT_CONTEXT_mcauseIDX  * portWORD_SIZE)
#define PORT_CONTEXT_mepcOFFSET     (PORT_CONTEXT_mepcIDX    * portWORD_SIZE)
#define PORT_CONTEXT_mstatusOFFSET  (PORT_CONTEXT_mstatusIDX * portWORD_SIZE)
/* total size of the structure usable in ASM. */
#define portasmREGISTER_CONTEXT_WORDSIZE               ((36) * portWORD_SIZE)

#define portCONTEXT_SIZE ( portRegNum * portWORD_SIZE )

  EXTERN pxCurrentTCB
  EXTERN xISRStackTop
  EXTERN xCriticalNesting
  EXTERN pxCriticalNesting
/*-----------------------------------------------------------*/

STORE_FPU_CONTEXT  MACRO
    fp_store_x f0, 29*portWORD_SIZE(sp)
    fp_store_x f1, 30*portWORD_SIZE(sp)
    fp_store_x f2, 31*portWORD_SIZE(sp)
    fp_store_x f3, 32*portWORD_SIZE(sp)
    fp_store_x f4, 33*portWORD_SIZE(sp)
    fp_store_x f5, 34*portWORD_SIZE(sp)
    fp_store_x f6, 35*portWORD_SIZE(sp)
    fp_store_x f7, 36*portWORD_SIZE(sp)
    fp_store_x f10, 37*portWORD_SIZE(sp)
    fp_store_x f11, 38*portWORD_SIZE(sp)
    fp_store_x f12, 39*portWORD_SIZE(sp)
    fp_store_x f13, 40*portWORD_SIZE(sp)
    fp_store_x f14, 41*portWORD_SIZE(sp)
    fp_store_x f15, 42*portWORD_SIZE(sp)
    fp_store_x f16, 43*portWORD_SIZE(sp)
    fp_store_x f17, 44*portWORD_SIZE(sp)
    fp_store_x f28, 45*portWORD_SIZE(sp)
    fp_store_x f29, 46*portWORD_SIZE(sp)
    fp_store_x f30, 47*portWORD_SIZE(sp)
    fp_store_x f31, 48*portWORD_SIZE(sp)
    csrr      t0, fcsr
    store_x   t0, 49*portWORD_SIZE(sp)
    ENDM

STORE_CONTEXT  MACRO
    addi sp, sp, -portCONTEXT_SIZE
    store_x x1,  1  * portWORD_SIZE(sp)
    store_x x5,  2  * portWORD_SIZE(sp)
    store_x x6,  3  * portWORD_SIZE(sp)
    store_x x7,  4  * portWORD_SIZE(sp)
    store_x x8,  5  * portWORD_SIZE(sp)
    store_x x9,  6  * portWORD_SIZE(sp)
    store_x x10, 7  * portWORD_SIZE(sp)
    store_x x11, 8  * portWORD_SIZE(sp)
    store_x x12, 9  * portWORD_SIZE(sp)
    store_x x13, 10 * portWORD_SIZE(sp)
    store_x x14, 11 * portWORD_SIZE(sp)
    store_x x15, 12 * portWORD_SIZE(sp)
    store_x x16, 13 * portWORD_SIZE(sp)
    store_x x17, 14 * portWORD_SIZE(sp)
    store_x x18, 15 * portWORD_SIZE(sp)
    store_x x19, 16 * portWORD_SIZE(sp)
    store_x x20, 17 * portWORD_SIZE(sp)
    store_x x21, 18 * portWORD_SIZE(sp)
    store_x x22, 19 * portWORD_SIZE(sp)
    store_x x23, 20 * portWORD_SIZE(sp)
    store_x x24, 21 * portWORD_SIZE(sp)
    store_x x25, 22 * portWORD_SIZE(sp)
    store_x x26, 23 * portWORD_SIZE(sp)
    store_x x27, 24 * portWORD_SIZE(sp)
    store_x x28, 25 * portWORD_SIZE(sp)
    store_x x29, 26 * portWORD_SIZE(sp)
    store_x x30, 27 * portWORD_SIZE(sp)
    store_x x31, 28 * portWORD_SIZE(sp)
#ifdef CONFIG_RV_FPU_PRESENT
    STORE_FPU_CONTEXT
#endif
    ENDM
    

portRESTORE_BaseReg MACRO
    /* Restore mepc & mstatus state */
    load_x  s3, PORT_CONTEXT_mepcOFFSET(sp)
    load_x  s4, PORT_CONTEXT_mstatusOFFSET(sp)
    csrw    mepc, s3
    csrw    mstatus, s4

    /* x1(ra)                       Return address */
    load_x  x1,  1*portWORD_SIZE(sp)
    /* x2(sp) ***** Should be restore outside this macro */
    /* x3(gp)                       Global pointer */
    load_x  x3,  3*portWORD_SIZE(sp)
    /* x4(tp)                       Thread pointer */
    load_x  x4,  4*portWORD_SIZE(sp)
    /* x5-7(t0-2)           Temporaries */
    load_x  x5,  5*portWORD_SIZE(sp)
    load_x  x6,  6*portWORD_SIZE(sp)
    load_x  x7,  7*portWORD_SIZE(sp)
    /* x8(s0/fp)            Saved register/Frame pointer */
    load_x  x8,  8*portWORD_SIZE(sp)
    /* x9(s1)                       Saved register */
    load_x  x9,  9*portWORD_SIZE(sp)
    /* x10-17(a0-7)         Function arguments */
    load_x  x10, 10*portWORD_SIZE(sp)
    load_x  x11, 11*portWORD_SIZE(sp)
    load_x  x12, 12*portWORD_SIZE(sp)
    load_x  x13, 13*portWORD_SIZE(sp)
    load_x  x14, 14*portWORD_SIZE(sp)
    load_x  x15, 15*portWORD_SIZE(sp)
    load_x  x16, 16*portWORD_SIZE(sp)
    load_x  x17, 17*portWORD_SIZE(sp)
    /* x18-27(s2-11)                Saved registers */
    load_x  x18, 18*portWORD_SIZE(sp)
    load_x  x19, 19*portWORD_SIZE(sp)
    load_x  x20, 20*portWORD_SIZE(sp)
    load_x  x21, 21*portWORD_SIZE(sp)
    load_x  x22, 22*portWORD_SIZE(sp)
    load_x  x23, 23*portWORD_SIZE(sp)
    load_x  x24, 24*portWORD_SIZE(sp)
    load_x  x25, 25*portWORD_SIZE(sp)
    load_x  x26, 26*portWORD_SIZE(sp)
    load_x  x27, 27*portWORD_SIZE(sp)
    /* x28-31(t3-6)         Temporaries */
    load_x  x28, 28*portWORD_SIZE(sp)
    load_x  x29, 29*portWORD_SIZE(sp)
    load_x  x30, 30*portWORD_SIZE(sp)
    load_x  x31, 31*portWORD_SIZE(sp)
    ENDM
/*-----------------------------------------------------------*/

portcontextSAVE_CONTEXT_INTERNAL MACRO
    addi sp, sp, -portCONTEXT_SIZE
    store_x x1, 1 * portWORD_SIZE( sp )
    store_x x5, 2 * portWORD_SIZE( sp )
    store_x x6, 3 * portWORD_SIZE( sp )
    store_x x7, 4 * portWORD_SIZE( sp )
    store_x x8, 5 * portWORD_SIZE( sp )
    store_x x9, 6 * portWORD_SIZE( sp )
    store_x x10, 7 * portWORD_SIZE( sp )
    store_x x11, 8 * portWORD_SIZE( sp )
    store_x x12, 9 * portWORD_SIZE( sp )
    store_x x13, 10 * portWORD_SIZE( sp )
    store_x x14, 11 * portWORD_SIZE( sp )
    store_x x15, 12 * portWORD_SIZE( sp )
    store_x x16, 13 * portWORD_SIZE( sp )
    store_x x17, 14 * portWORD_SIZE( sp )
    store_x x18, 15 * portWORD_SIZE( sp )
    store_x x19, 16 * portWORD_SIZE( sp )
    store_x x20, 17 * portWORD_SIZE( sp )
    store_x x21, 18 * portWORD_SIZE( sp )
    store_x x22, 19 * portWORD_SIZE( sp )
    store_x x23, 20 * portWORD_SIZE( sp )
    store_x x24, 21 * portWORD_SIZE( sp )
    store_x x25, 22 * portWORD_SIZE( sp )
    store_x x26, 23 * portWORD_SIZE( sp )
    store_x x27, 24 * portWORD_SIZE( sp )
    store_x x28, 25 * portWORD_SIZE( sp )
    store_x x29, 26 * portWORD_SIZE( sp )
    store_x x30, 27 * portWORD_SIZE( sp )
    store_x x31, 28 * portWORD_SIZE( sp )

    load_x  t0, xCriticalNesting         /* Load the value of xCriticalNesting into t0. */
    store_x t0, 29 * portWORD_SIZE( sp ) /* Store the critical nesting value to the stack. */

    csrr t0, mstatus                     /* Required for MPIE bit. */
    store_x t0, 30 * portWORD_SIZE( sp )

    portasmSAVE_ADDITIONAL_REGISTERS     /* Defined in freertos_risc_v_chip_specific_extensions.h to save any registers unique to the RISC-V implementation. */

    load_x  t0, pxCurrentTCB             /* Load pxCurrentTCB. */
    store_x  sp, 0( t0 )                 /* Write sp to first TCB member. */

    ENDM
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

#endif /* PORTCONTEXT_H */
