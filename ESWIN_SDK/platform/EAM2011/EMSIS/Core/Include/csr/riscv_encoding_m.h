/*
 * Copyright (c) 2020 ESWIN Limited. All rights reserved.
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

#ifndef __RISCV_ENCODING_M_H__
#define __RISCV_ENCODING_M_H__

#include "riscv_bits.h"

#ifdef __cplusplus
 extern "C" {
#endif
/**
 * \defgroup EMSIS_Core_CSR_Encoding    Core CSR Encodings
 * \ingroup  EMSIS_Core
 * \brief    EMSIS Core CSR Encodings
 * \details
 *
 * The following macros are used for CSR encodings
 *   @{
 */
#define MSTATUS_UIE         0x00000001
#define MSTATUS_SIE         0x00000002
#define MSTATUS_HIE         0x00000004
#define MSTATUS_MIE         0x00000008
#define MSTATUS_UPIE        0x00000010
#define MSTATUS_SPIE        0x00000020
#define MSTATUS_HPIE        0x00000040
#define MSTATUS_MPIE        0x00000080
#define MSTATUS_SPP         0x00000100
#define MSTATUS_MPP         0x00001800
#define MSTATUS_FS          0x00006000
#define MSTATUS_XS          0x00018000
#define MSTATUS_MPRV        0x00020000
#define MSTATUS_SUM         0x00040000
#define MSTATUS_MXR         0x00080000
#define MSTATUS_TVM         0x00100000
#define MSTATUS_TW          0x00200000
#define MSTATUS_TSR         0x00400000
#define MSTATUS32_SD        0x80000000
#define MSTATUS64_UXL       0x0000000300000000
#define MSTATUS64_SXL       0x0000000C00000000
#define MSTATUS64_SD        0x8000000000000000

#define MSTATUS_FS_OFF        0x00000000
#define MSTATUS_FS_INITIAL  0x00002000
#define MSTATUS_FS_CLEAN    0x00004000
#define MSTATUS_FS_DIRTY    0x00006000

#define SSTATUS_UIE         0x00000001
#define SSTATUS_SIE         0x00000002
#define SSTATUS_UPIE        0x00000010
#define SSTATUS_SPIE        0x00000020
#define SSTATUS_SPP         0x00000100
#define SSTATUS_FS          0x00006000
#define SSTATUS_XS          0x00018000
#define SSTATUS_SUM         0x00040000
#define SSTATUS_MXR         0x00080000
#define SSTATUS32_SD        0x80000000
#define SSTATUS64_UXL       0x0000000300000000
#define SSTATUS64_SD        0x8000000000000000

#define MIP_USIP            (1 << IRQ_U_SOFT)
#define MIP_SSIP            (1 << IRQ_S_SOFT)
#define MIP_HSIP            (1 << IRQ_H_SOFT)
#define MIP_MSIP            (1 << IRQ_M_SOFT)
#define MIP_UTIP            (1 << IRQ_U_TIMER)
#define MIP_STIP            (1 << IRQ_S_TIMER)
#define MIP_HTIP            (1 << IRQ_H_TIMER)
#define MIP_MTIP            (1 << IRQ_M_TIMER)
#define MIP_UEIP            (1 << IRQ_U_EXT)
#define MIP_SEIP            (1 << IRQ_S_EXT)
#define MIP_HEIP            (1 << IRQ_H_EXT)
#define MIP_MEIP            (1 << IRQ_M_EXT)

#define MIE_USIE            MIP_USIP
#define MIE_SSIE            MIP_SSIP
#define MIE_HSIE            MIP_HSIP
#define MIE_MSIE            MIP_MSIP
#define MIE_UTIE            MIP_UTIP
#define MIE_STIE            MIP_STIP
#define MIE_HTIE            MIP_HTIP
#define MIE_MTIE            MIP_MTIP
#define MIE_UEIE            MIP_UEIP
#define MIE_SEIE            MIP_SEIP
#define MIE_HEIE            MIP_HEIP
#define MIE_MEIE            MIP_MEIP

#define PRV_U 0
#define PRV_S 1
#define PRV_H 2
#define PRV_M 3

#define VM_MBARE 0
#define VM_SV32  1
#define VM_SV39  8
#define VM_SV48  9
#define VM_SV57  10
#define VM_SV64  11

#define IRQ_U_SOFT   0
#define IRQ_S_SOFT   1
#define IRQ_H_SOFT   2
#define IRQ_M_SOFT   3
#define IRQ_U_TIMER  4
#define IRQ_S_TIMER  5
#define IRQ_H_TIMER  6
#define IRQ_M_TIMER  7
#define IRQ_U_EXT    8
#define IRQ_S_EXT    9
#define IRQ_H_EXT    10
#define IRQ_M_EXT    11

/* === FPU FRM Rounding Mode === */
/** FPU Round to Nearest, ties to Even*/
#define FRM_RNDMODE_RNE     0x0
/** FPU Round Towards Zero */
#define FRM_RNDMODE_RTZ     0x1
/** FPU Round Down (towards -inf) */
#define FRM_RNDMODE_RDN     0x2
/** FPU Round Up (towards +inf) */
#define FRM_RNDMODE_RUP     0x3
/** FPU Round to nearest, ties to Max Magnitude */
#define FRM_RNDMODE_RMM     0x4
/**
 * In instruction's rm, selects dynamic rounding mode.
 * In Rounding Mode register, Invalid */
#define FRM_RNDMODE_DYN     0x7

/* === FPU FFLAGS Accrued Exceptions === */
/** FPU Inexact */
#define FFLAGS_AE_NX        (1<<0)
/** FPU Underflow */
#define FFLAGS_AE_UF        (1<<1)
/** FPU Overflow */
#define FFLAGS_AE_OF        (1<<2)
/** FPU Divide by Zero */
#define FFLAGS_AE_DZ        (1<<3)
/** FPU Invalid Operation */
#define FFLAGS_AE_NV        (1<<4)

/** Floating Point Register f0-f31, eg. f0 -> FREG(0) */
#define FREG(idx)           f##idx


/* === PMP CFG Bits === */
#define PMP_R                0x01
#define PMP_W                0x02
#define PMP_X                0x04
#define PMP_A                0x18
#define PMP_A_OFF           0x00
#define PMP_A_TOR            0x08
#define PMP_A_NA4            0x10
#define PMP_A_NAPOT            0x18
#define PMP_L                0x80

#define PMP_SHIFT            2
#define PMP_COUNT            16

#ifdef __riscv

#ifdef __riscv64
# define MSTATUS_UXL MSTATUS64_UXL
# define MSTATUS_SXL MSTATUS64_SXL
# define MSTATUS_SD MSTATUS64_SD
# define SSTATUS_SD SSTATUS64_SD
# define RISCV_PGLEVEL_BITS 9
#else
# define MSTATUS_UXL MSTATUS64_UXL
# define MSTATUS_SD MSTATUS32_SD
# define SSTATUS_SD SSTATUS32_SD
# define RISCV_PGLEVEL_BITS 10
#endif /* __riscv64 */

#define RISCV_PGSHIFT 12
#define RISCV_PGSIZE (1 << RISCV_PGSHIFT)

#endif /* __riscv */

/**
 * \defgroup EMSIS_Core_CSR_Registers    Core CSR Registers
 * \ingroup  EMSIS_Core
 * \brief    EMSIS Core CSR Register Definitions
 * \details  M Mode CSR Defination must support
 *
 * The following macros are used for CSR Register Defintions.
 *   @{
 */

/**
 * \brief  Machine Information Registers
 */
#define CSR_MVENDORID 0xf11
#define CSR_MARCHID 0xf12
#define CSR_MIMPID 0xf13
#define CSR_MHARTID 0xf14

/**
 * \brief  Machine Trap Setup
 */
#define CSR_MSTATUS 0x300
#define CSR_MISA 0x301
#define CSR_MEDELEG 0x302
#define CSR_MIDELEG 0x303
#define CSR_MIE 0x304
#define CSR_MTVEC 0x305
#define CSR_MCOUNTEREN 0x306
#ifdef CONFIG_RV_CLIC_PRESENT
#define CSR_MTVT 0x307
#endif

/**
 * \brief Machine Trap Handling
 */
#define CSR_MSCRATCH 0x340
#define CSR_MEPC 0x341
#define CSR_MCAUSE 0x342
#define CSR_MTVAL 0x343
#define CSR_MIP 0x344
#ifdef CONFIG_RV_CLIC_PRESENT
#define CSR_MNXTI 0x345
#define CSR_MINITSTATUS 0x346
#endif

/**
 * \brief  Machine Memory Protection
 */
#define CSR_PMPCFG0 0x3a0
#define CSR_PMPCFG1 0x3a1
#define CSR_PMPCFG2 0x3a2
#define CSR_PMPCFG3 0x3a3
#define CSR_PMPADDR0 0x3b0
#define CSR_PMPADDR1 0x3b1
#define CSR_PMPADDR2 0x3b2
#define CSR_PMPADDR3 0x3b3
#define CSR_PMPADDR4 0x3b4
#define CSR_PMPADDR5 0x3b5
#define CSR_PMPADDR6 0x3b6
#define CSR_PMPADDR7 0x3b7
#define CSR_PMPADDR8 0x3b8
#define CSR_PMPADDR9 0x3b9
#define CSR_PMPADDR10 0x3ba
#define CSR_PMPADDR11 0x3bb
#define CSR_PMPADDR12 0x3bc
#define CSR_PMPADDR13 0x3bd
#define CSR_PMPADDR14 0x3be
#define CSR_PMPADDR15 0x3bf

/**
 * \brief  Machine Counter/Timers
 */
#define CSR_MCYCLE 0xb00
#define CSR_MINSTRET 0xb02
#define CSR_MHPMCOUNTER3 0xb03
#define CSR_MHPMCOUNTER4 0xb04
#define CSR_MHPMCOUNTER5 0xb05
#define CSR_MHPMCOUNTER6 0xb06
#define CSR_MHPMCOUNTER7 0xb07
#define CSR_MHPMCOUNTER8 0xb08
#define CSR_MHPMCOUNTER9 0xb09
#define CSR_MHPMCOUNTER10 0xb0a
#define CSR_MHPMCOUNTER11 0xb0b
#define CSR_MHPMCOUNTER12 0xb0c
#define CSR_MHPMCOUNTER13 0xb0d
#define CSR_MHPMCOUNTER14 0xb0e
#define CSR_MHPMCOUNTER15 0xb0f
#define CSR_MHPMCOUNTER16 0xb10
#define CSR_MHPMCOUNTER17 0xb11
#define CSR_MHPMCOUNTER18 0xb12
#define CSR_MHPMCOUNTER19 0xb13
#define CSR_MHPMCOUNTER20 0xb14
#define CSR_MHPMCOUNTER21 0xb15
#define CSR_MHPMCOUNTER22 0xb16
#define CSR_MHPMCOUNTER23 0xb17
#define CSR_MHPMCOUNTER24 0xb18
#define CSR_MHPMCOUNTER25 0xb19
#define CSR_MHPMCOUNTER26 0xb1a
#define CSR_MHPMCOUNTER27 0xb1b
#define CSR_MHPMCOUNTER28 0xb1c
#define CSR_MHPMCOUNTER29 0xb1d
#define CSR_MHPMCOUNTER30 0xb1e
#define CSR_MHPMCOUNTER31 0xb1f
#define CSR_MCYCLEH 0xb80
#define CSR_MINSTRETH 0xb82
#define CSR_MHPMCOUNTER3H 0xb83
#define CSR_MHPMCOUNTER4H 0xb84
#define CSR_MHPMCOUNTER5H 0xb85
#define CSR_MHPMCOUNTER6H 0xb86
#define CSR_MHPMCOUNTER7H 0xb87
#define CSR_MHPMCOUNTER8H 0xb88
#define CSR_MHPMCOUNTER9H 0xb89
#define CSR_MHPMCOUNTER10H 0xb8a
#define CSR_MHPMCOUNTER11H 0xb8b
#define CSR_MHPMCOUNTER12H 0xb8c
#define CSR_MHPMCOUNTER13H 0xb8d
#define CSR_MHPMCOUNTER14H 0xb8e
#define CSR_MHPMCOUNTER15H 0xb8f
#define CSR_MHPMCOUNTER16H 0xb80
#define CSR_MHPMCOUNTER17H 0xb91
#define CSR_MHPMCOUNTER18H 0xb92
#define CSR_MHPMCOUNTER19H 0xb93
#define CSR_MHPMCOUNTER20H 0xb94
#define CSR_MHPMCOUNTER21H 0xb95
#define CSR_MHPMCOUNTER22H 0xb96
#define CSR_MHPMCOUNTER23H 0xb97
#define CSR_MHPMCOUNTER24H 0xb98
#define CSR_MHPMCOUNTER25H 0xb99
#define CSR_MHPMCOUNTER26H 0xb9a
#define CSR_MHPMCOUNTER27H 0xb9b
#define CSR_MHPMCOUNTER28H 0xb9c
#define CSR_MHPMCOUNTER29H 0xb9d
#define CSR_MHPMCOUNTER30H 0xb9e
#define CSR_MHPMCOUNTER31H 0xb9f

/**
 * \brief  Machine Counter Setup
 */
#define CSR_MCOUNTINHIBIT 0x320
#define CSR_MHPMEVENT3 0x323
#define CSR_MHPMEVENT4 0x324
#define CSR_MHPMEVENT5 0x325
#define CSR_MHPMEVENT6 0x326
#define CSR_MHPMEVENT7 0x327
#define CSR_MHPMEVENT8 0x328
#define CSR_MHPMEVENT9 0x329
#define CSR_MHPMEVENT10 0x32a
#define CSR_MHPMEVENT11 0x32b
#define CSR_MHPMEVENT12 0x32c
#define CSR_MHPMEVENT13 0x32d
#define CSR_MHPMEVENT14 0x32e
#define CSR_MHPMEVENT15 0x32f
#define CSR_MHPMEVENT16 0x330
#define CSR_MHPMEVENT17 0x331
#define CSR_MHPMEVENT18 0x332
#define CSR_MHPMEVENT19 0x333
#define CSR_MHPMEVENT20 0x334
#define CSR_MHPMEVENT21 0x335
#define CSR_MHPMEVENT22 0x336
#define CSR_MHPMEVENT23 0x337
#define CSR_MHPMEVENT24 0x338
#define CSR_MHPMEVENT25 0x339
#define CSR_MHPMEVENT26 0x33a
#define CSR_MHPMEVENT27 0x33b
#define CSR_MHPMEVENT28 0x33c
#define CSR_MHPMEVENT29 0x33d
#define CSR_MHPMEVENT30 0x33e
#define CSR_MHPMEVENT31 0x33f

/**
 * \brief  Debug/Trace Register(Shared with Debug Mode)
 */
#define CSR_TSELECT 0x7a0
#define CSR_TDATA1 0x7a1
#define CSR_TDATA2 0x7a2
#define CSR_TDATA3 0x7a3

/**
 * \brief  Debug Mode Register
 */
#define CSR_DCSR 0x7b0
#define CSR_DPC 0x7b1
#define CSR_DSCRATCH0 0x7b2
#define CSR_DSCRATCH1 0x7b3

/**
 * @}
 */ /** End of Doxygen Group EMSIS_Core_CSR_Registers **/

/* Interrupt Handler in MTVEC CSR */
#define MTVEC_BASE_Msk          ~(0x3U)

/* Interrupt Mode in MTVEC CSR */
#define MTVEC_MODE_Msk          0x3
#define MTVEC_MODE_CLINT_DIRECT 0x0
#define MTVEC_MODE_CLINT_VECTOR 0x1
#ifdef CONFIG_RV_CLIC_PRESENT
#define MTVEC_MODE_CLIC_DIRECT  0x2
#define MTVEC_MODE_CLIC_VECTOR  0x3
#endif

/* Exception Code in MCAUSE CSR */
#define MCAUSE_MISALIGNED_FETCH 0x0
#define MCAUSE_FAULT_FETCH 0x1
#define MCAUSE_ILLEGAL_INSTRUCTION 0x2
#define MCAUSE_BREAKPOINT 0x3
#define MCAUSE_MISALIGNED_LOAD 0x4
#define MCAUSE_FAULT_LOAD 0x5
#define MCAUSE_MISALIGNED_STORE 0x6
#define MCAUSE_FAULT_STORE 0x7
#define MCAUSE_USER_ECALL 0x8
#define MCAUSE_SUPERVISOR_ECALL 0x9
#define MCAUSE_HYPERVISOR_ECALL 0xa
#define MCAUSE_MACHINE_ECALL 0xb
#define MCAUSE_PAGE_FAULT_FETCH 0xc
#define MCAUSE_PAGE_FAULT_LOAD 0xd
#define MCAUSE_PAGE_FAULT_STORE 0xf

/* Counter Control mask in MCOUNTINHIBIT CSR */
#define MCOUNTINHIBIT_CY 0x1
#define MCOUNTINHIBIT_IR 0x4
#define MCOUNTINHIBIT_HPM(x) (1 << x)
#define MCOUNTINHIBIT_HPM3 MCOUNTINHIBIT_HPM(3)
#define MCOUNTINHIBIT_HPM4 MCOUNTINHIBIT_HPM(4)
#define MCOUNTINHIBIT_HPM5 MCOUNTINHIBIT_HPM(5)
#define MCOUNTINHIBIT_HPM6 MCOUNTINHIBIT_HPM(6)
#define MCOUNTINHIBIT_HPM7 MCOUNTINHIBIT_HPM(7)
#define MCOUNTINHIBIT_HPM8 MCOUNTINHIBIT_HPM(8)
#define MCOUNTINHIBIT_HPM9 MCOUNTINHIBIT_HPM(9)
#define MCOUNTINHIBIT_HPM10 MCOUNTINHIBIT_HPM(10)
#define MCOUNTINHIBIT_HPM11 MCOUNTINHIBIT_HPM(11)
#define MCOUNTINHIBIT_HPM12 MCOUNTINHIBIT_HPM(12)
#define MCOUNTINHIBIT_HPM13 MCOUNTINHIBIT_HPM(13)
#define MCOUNTINHIBIT_HPM14 MCOUNTINHIBIT_HPM(14)
#define MCOUNTINHIBIT_HPM15 MCOUNTINHIBIT_HPM(15)
#define MCOUNTINHIBIT_HPM16 MCOUNTINHIBIT_HPM(16)
#define MCOUNTINHIBIT_HPM17 MCOUNTINHIBIT_HPM(17)
#define MCOUNTINHIBIT_HPM18 MCOUNTINHIBIT_HPM(18)
#define MCOUNTINHIBIT_HPM19 MCOUNTINHIBIT_HPM(19)
#define MCOUNTINHIBIT_HPM20 MCOUNTINHIBIT_HPM(20)
#define MCOUNTINHIBIT_HPM21 MCOUNTINHIBIT_HPM(21)
#define MCOUNTINHIBIT_HPM22 MCOUNTINHIBIT_HPM(22)
#define MCOUNTINHIBIT_HPM23 MCOUNTINHIBIT_HPM(23)
#define MCOUNTINHIBIT_HPM24 MCOUNTINHIBIT_HPM(24)
#define MCOUNTINHIBIT_HPM25 MCOUNTINHIBIT_HPM(25)
#define MCOUNTINHIBIT_HPM26 MCOUNTINHIBIT_HPM(26)
#define MCOUNTINHIBIT_HPM27 MCOUNTINHIBIT_HPM(27)
#define MCOUNTINHIBIT_HPM28 MCOUNTINHIBIT_HPM(28)
#define MCOUNTINHIBIT_HPM29 MCOUNTINHIBIT_HPM(29)
#define MCOUNTINHIBIT_HPM30 MCOUNTINHIBIT_HPM(30)
#define MCOUNTINHIBIT_HPM31 MCOUNTINHIBIT_HPM(31)
/**
 * @}
 */ /** End of Doxygen Group EMSIS_Core_CSR_Encoding **/

#ifdef __cplusplus
}
#endif
#endif /* __RISCV_ENCODING_M_H__ */
