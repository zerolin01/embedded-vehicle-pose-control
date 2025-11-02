/*
 * FreeRTOS Kernel V10.4.6
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

/*
 * The FreeRTOS kernel's RISC-V port is split between the the code that is
 * common across all currently supported RISC-V chips (implementations of the
 * RISC-V ISA), and code which tailors the port to a specific RISC-V chip:
 *
 * + The code that is common to all RISC-V chips is implemented in
 *   FreeRTOS\Source\portable\GCC\RISC-V-RV32\portASM.S.  There is only one
 *   portASM.S file because the same file is used no matter which RISC-V chip is
 *   in use.
 *
 * + The code that tailors the kernel's RISC-V port to a specific RISC-V
 *   chip is implemented in freertos_risc_v_chip_specific_extensions.h.  There
 *   is one freertos_risc_v_chip_specific_extensions.h that can be used with any
 *   RISC-V chip that both includes a standard CLINT and does not add to the
 *   base set of RISC-V registers.  There are additional
 *   freertos_risc_v_chip_specific_extensions.h files for RISC-V implementations
 *   that do not include a standard CLINT or do add to the base set of RISC-V
 *   registers.
 *
 * CARE MUST BE TAKEN TO INCLDUE THE CORRECT
 * freertos_risc_v_chip_specific_extensions.h HEADER FILE FOR THE CHIP
 * IN USE.  To include the correct freertos_risc_v_chip_specific_extensions.h
 * header file ensure the path to the correct header file is in the assembler's
 * include path.
 *
 * This freertos_risc_v_chip_specific_extensions.h is for use on RISC-V chips
 * that include a standard CLINT and do not add to the base set of RISC-V
 * registers.
 *
 */

#include "portContext.h"

/* CSR definitions. */
#define CSR_MSTATUS         0x300
#define CSR_MTVEC	    0x305
#define CSR_MEPC            0x341
#define CSR_MCAUSE          0x342


	PUBLIC xPortStartFirstTask
        PUBLIC clic_csip_handler
      
        EXTERN xPortTaskSwitch
/*-----------------------------------------------------------*/

	SECTION `.text`:CODE:NOROOT(2)
	CODE
/*-----------------------------------------------------------*/
#define tskSTACK_FILL_BYTE    ( 0xa5a5a5a5U )

RESTORE_FPU_CONTEXT MACRO
    fp_load_x f0, 29*portWORD_SIZE(sp)
    fp_load_x f1, 30*portWORD_SIZE(sp)
    fp_load_x f2, 31*portWORD_SIZE(sp)
    fp_load_x f3, 32*portWORD_SIZE(sp)
    fp_load_x f4, 33*portWORD_SIZE(sp)
    fp_load_x f5, 34*portWORD_SIZE(sp)
    fp_load_x f6, 35*portWORD_SIZE(sp)
    fp_load_x f7, 36*portWORD_SIZE(sp)
    fp_load_x f10, 37*portWORD_SIZE(sp)
    fp_load_x f11, 38*portWORD_SIZE(sp)
    fp_load_x f12, 39*portWORD_SIZE(sp)
    fp_load_x f13, 40*portWORD_SIZE(sp)
    fp_load_x f14, 41*portWORD_SIZE(sp)
    fp_load_x f15, 42*portWORD_SIZE(sp)
    fp_load_x f16, 43*portWORD_SIZE(sp)
    fp_load_x f17, 44*portWORD_SIZE(sp)
    fp_load_x f28, 45*portWORD_SIZE(sp)
    fp_load_x f29, 46*portWORD_SIZE(sp)
    fp_load_x f30, 47*portWORD_SIZE(sp)
    fp_load_x f31, 48*portWORD_SIZE(sp)
    load_x   t1,  49*portWORD_SIZE(sp)
    jal fcsrCheck
    fscsr  x0, t1
    ENDM
/*-----------------------------------------------------------*/
    
fcsrCheck:
    li  t2, tskSTACK_FILL_BYTE
    bne t1, t2, judgefcsr
    li  t1, 0
judgefcsr:
    ret
/*-----------------------------------------------------------*/
portcontextRESTORE_CONTEXT MACRO
#ifdef CONFIG_RV_FPU_PRESENT 
    RESTORE_FPU_CONTEXT
#endif
    /* Restore mepc & mstatus state */ 
    load_x  x1, 1 * portWORD_SIZE( sp )
    load_x  x5, 2 * portWORD_SIZE( sp )
    load_x  x6, 3 * portWORD_SIZE( sp )
    load_x  x7, 4 * portWORD_SIZE( sp )
    load_x  x8, 5 * portWORD_SIZE( sp )
    load_x  x9, 6 * portWORD_SIZE( sp )
    load_x  x10, 7 * portWORD_SIZE( sp )
    load_x  x11, 8 * portWORD_SIZE( sp )
    load_x  x12, 9 * portWORD_SIZE( sp )
    load_x  x13, 10 * portWORD_SIZE( sp )
    load_x  x14, 11 * portWORD_SIZE( sp )
    load_x  x15, 12 * portWORD_SIZE( sp )
    load_x  x16, 13 * portWORD_SIZE( sp )
    load_x  x17, 14 * portWORD_SIZE( sp )
    load_x  x18, 15 * portWORD_SIZE( sp )
    load_x  x19, 16 * portWORD_SIZE( sp )
    load_x  x20, 17 * portWORD_SIZE( sp )
    load_x  x21, 18 * portWORD_SIZE( sp )
    load_x  x22, 19 * portWORD_SIZE( sp )
    load_x  x23, 20 * portWORD_SIZE( sp )
    load_x  x24, 21 * portWORD_SIZE( sp )
    load_x  x25, 22 * portWORD_SIZE( sp )
    load_x  x26, 23 * portWORD_SIZE( sp )
    load_x  x27, 24 * portWORD_SIZE( sp )
    load_x  x28, 25 * portWORD_SIZE( sp )
    load_x  x29, 26 * portWORD_SIZE( sp )
    load_x  x30, 27 * portWORD_SIZE( sp )
    load_x  x31, 28 * portWORD_SIZE( sp )
    addi sp, sp, portCONTEXT_SIZE
    ENDM

/*-----------------------------------------------------------*/

xPortStartFirstTask:
    load_x  sp, pxCurrentTCB            /* Load pxCurrentTCB. */
    load_x  sp, 0x0( sp )                 /* Read sp from first TCB member. */

     portasmRESTORE_ADDITIONAL_REGISTERS /* Defined in freertos_risc_v_chip_specific_extensions.h to restore any registers unique to the RISC-V implementation. */
    /* Pop PC from stack and set MEPC */
     load_x  t0, 0 * portWORD_SIZE(sp)
     csrw  CSR_MEPC, t0
    /* Pop mstatus from stack and set it */
     load_x t0,  (portRegNum - 1)  * portWORD_SIZE(sp)
     csrw CSR_MSTATUS, t0
    /* Interrupt still disable here */
    /* Restore Registers from Stack */
     portcontextRESTORE_CONTEXT
     
     mret
/*-----------------------------------------------------------*/
clic_csip_handler:
    STORE_CONTEXT

    /* Push mstatus to stack */
    csrr t0, CSR_MSTATUS
    store_x t0,  (portRegNum - 1)  * portWORD_SIZE(sp)

    /* Push additional registers */

    /* Store sp to task stack */
    load_x t0, pxCurrentTCB
    store_x sp, 0(t0)

    csrr t0, CSR_MEPC
    store_x t0, 0(sp)
    jal xPortTaskSwitch

    /* Switch task context */
    load_x t0, pxCurrentTCB           /* Load pxCurrentTCB. */
    load_x sp, 0x0(t0)                /* Read sp from first TCB member */

    /* Pop PC from stack and set MEPC */
    load_x t0,  0  * portWORD_SIZE(sp)
    csrw CSR_MEPC, t0
    /* Pop additional registers */

    /* Pop mstatus from stack and set it */
    load_x t0,  (portRegNum - 1)  * portWORD_SIZE(sp)
    csrw CSR_MSTATUS, t0
    /* Interrupt still disable here */
    /* Restore Registers from Stack */
    portcontextRESTORE_CONTEXT
    mret


/*-----------------------------------------------------------*/
xPortRestoreBeforeFirstTask:
        portRESTORE_BaseReg
        load_x  x2, 2*portWORD_SIZE(sp)
        ret
        
        PUBLIC xPortMoveISRStackTop

xPortMoveISRStackTop:
        load_x  t0, 0(a0)
        beqz t0, xPortInit_register_a0
        addi t1, x0, portasmREGISTER_CONTEXT_WORDSIZE
        bgtu t1, t0, xPortInit_register_a0
        sub     t1, t0, t1
        store_x t1, 0(a0)
        mv a0, t1
        ret

xPortInit_register_a0:
        li      a0, 0
        
        END