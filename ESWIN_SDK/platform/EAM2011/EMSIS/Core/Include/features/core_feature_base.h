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

#ifndef __CORE_FEATURE_BASE__
#define __CORE_FEATURE_BASE__
/**
 * @file     core_feature_base.h
 * @brief    Base core feature API for Eswin Cores
 */
#include "../riscv_encoding.h"
#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

/**
 * \defgroup EMSIS_Core_Registers     Register Define and Type Definitions
 * \brief   Type definitions and defines for core registers.
 *
 * @{
 */

#ifndef __RISCV_XLEN
  /** \brief Refer to the width of an integer register in bits(either 32 or 64) */
  #ifndef __riscv_xlen
    #define __RISCV_XLEN    32
  #else
    #define __RISCV_XLEN    __riscv_xlen
  #endif
#endif /* __RISCV_XLEN */

/** \brief Type of Control and Status Register(CSR), depends on the XLEN defined in RISC-V */
#if __RISCV_XLEN == 32
  typedef uint32_t rv_csr_t;
#elif __RISCV_XLEN == 64
  typedef uint64_t rv_csr_t;
#else
  typedef uint32_t rv_csr_t;
#endif
/**
 * @}
 */ /* End of Doxygen Group EMSIS_Core_Registers */

/**
 * \defgroup EMSIS_Core_Base_Registers     Base Register Define and Type Definitions
 * \ingroup EMSIS_Core_Registers
 * \brief   Type definitions and defines for base core registers.
 *
 * @{
 */
/**
 * \brief  Union type to access MISA register.
 */
typedef union {
    struct {
        rv_csr_t a:1;                           /** bit:     0  Atomic extension */
        rv_csr_t b:1;                           /** bit:     1  Tentatively reserved for Bit-Manipulation extension */
        rv_csr_t c:1;                           /** bit:     2  Compressed extension */
        rv_csr_t d:1;                           /** bit:     3  Double-precision floating-point extension */
        rv_csr_t e:1;                           /** bit:     4  RV32E base ISA */
        rv_csr_t f:1;                           /** bit:     5  Single-precision floating-point extension */
        rv_csr_t g:1;                           /** bit:     6  Additional standard extensions present */
        rv_csr_t h:1;                           /** bit:     7  Hypervisor extension */
        rv_csr_t i:1;                           /** bit:     8  RV32I/64I/128I base ISA */
        rv_csr_t j:1;                           /** bit:     9  Tentatively reserved for Dynamically Translated Languages extension */
        rv_csr_t _reserved1:1;                  /** bit:     10 Reserved  */
        rv_csr_t l:1;                           /** bit:     11 Tentatively reserved for Decimal Floating-Point extension  */
        rv_csr_t m:1;                           /** bit:     12 Integer Multiply/Divide extension */
        rv_csr_t n:1;                           /** bit:     13 User-level interrupts supported  */
        rv_csr_t _reserved2:1;                  /** bit:     14 Reserved  */
        rv_csr_t p:1;                           /** bit:     15 Tentatively reserved for Packed-SIMD extension  */
        rv_csr_t q:1;                           /** bit:     16 Quad-precision floating-point extension  */
        rv_csr_t _resreved3:1;                  /** bit:     17 Reserved  */
        rv_csr_t s:1;                           /** bit:     18 Supervisor mode implemented  */
        rv_csr_t t:1;                           /** bit:     19 Tentatively reserved for Transactional Memory extension  */
        rv_csr_t u:1;                           /** bit:     20 User mode implemented  */
        rv_csr_t v:1;                           /** bit:     21 Tentatively reserved for Vector extension  */
        rv_csr_t _reserved4:1;                  /** bit:     22 Reserved  */
        rv_csr_t x:1;                           /** bit:     23 Non-standard extensions present  */
#if defined(__RISCV_XLEN) && __RISCV_XLEN == 64
        rv_csr_t _reserved5:38;                 /** bit:     24..61 Reserved  */
        rv_csr_t mxl:2;                         /** bit:     62..63 Machine XLEN  */
#else
        rv_csr_t _reserved5:6;                  /** bit:     24..29 Reserved  */
        rv_csr_t mxl:2;                         /** bit:     30..31 Machine XLEN  */
#endif
    } b;                                        /** Structure used for bit  access */
    rv_csr_t d;                                 /** Type      used for csr data access */
} CSR_MISA_Type;

/**
 * \brief  Union type to access MSTATUS configure register.
 */
typedef union {
    struct {
#if defined(__RISCV_XLEN) && __RISCV_XLEN == 64
        rv_csr_t uie:1;                         /** bit:     0  User mode interrupt enable flag */
        rv_csr_t sie:1;                         /** bit:     1  Supervisor mode interrupt enable flag */
        rv_csr_t _reserved0:1                   /** bit:     2  Reserved */
        rv_csr_t mie:1;                         /** bit:     3  Machine mode interrupt enable flag */
        rv_csr_t upie:1;                        /** bit:     4  Mirror for UIE flag */
        rv_csr_t spie:1;                        /** bit:     5  Mirror for SIE flag */
        rv_csr_t _reserved1:1;                  /** bit:     6  Reserved */
        rv_csr_t mpie:1;                        /** bit:     7  Mirror of MIE flag */
        rv_csr_t spp:1;                         /** bit:     8  Mirror of of Privilege Mode for Supervisor */
        rv_csr_t _reserved2:2;                  /** bit:     9..10  Reserved */
        rv_csr_t mpp:2;                         /** bit:     11..12 Mirror of Privilege Mode */
        rv_csr_t fs:2;                          /** bit:     13..14 FS status flag */
        rv_csr_t xs:2;                          /** bit:     15..16 XS status flag */
        rv_csr_t mprv:1;                        /** bit:     17 Machine mode PMP */
        rv_csr_t sum:1;                         /** bit:     18 permit Supervisor User Memory access */
        rv_csr_t mxr:1;                         /** bit:     19 make executable readable */
        rv_csr_t tvm:1;                         /** bit:     20 Trap virtual memory */
        rv_csr_t tw:1;                          /** bit:     21 Timeout wait */
        rv_csr_t tsr:1;                         /** bit:     22 Trap SRET */
        rv_csr_t _reserved3:9;                  /** bit:     23..31 Reserved */
        rv_csr_t uxl:2;                         /** bit:     32..33 user mode xlen */
        rv_csr_t sxl:2;                         /** bit:     34..35 supervisor mode xlen */
        rv_csr_t _reserved4:27;                 /** bit:     36..62 Reserved  */
        rv_csr_t sd:1;                          /** bit:     Dirty status for XS or FS */
#else
        rv_csr_t uie:1;                         /** bit:     0  User mode interrupt enable flag */
        rv_csr_t sie:1;                         /** bit:     1  supervisor interrupt enable flag */
        rv_csr_t _reserved0:1;                  /** bit:     2  Reserved */
        rv_csr_t mie:1;                         /** bit:     3  Machine mode interrupt enable flag */
        rv_csr_t upie:1;                        /** bit:     4  Mirror for UIE flag */
        rv_csr_t spie:1;                        /** bit:     5  Mirror for SIE flag */
        rv_csr_t _reserved1:1;                  /** bit:     6  Reserved */
        rv_csr_t mpie:1;                        /** bit:     7  Mirror of MIE flag */
        rv_csr_t spp:1;                         /** bit:     8  Mirror of Privilege Mode for supervisor */
        rv_csr_t _reserved2:2;                  /** bit:     9..10  Reserved */
        rv_csr_t mpp:2;                         /** bit:     11..12 Mirror of Privilege Mode */
        rv_csr_t fs:2;                          /** bit:     13..14 FS status flag */
        rv_csr_t xs:2;                          /** bit:     15..16 XS status flag */
        rv_csr_t mprv:1;                        /** bit:     17  Machine mode PMP */
        rv_csr_t sum:1;                         /** bit:     18  Supervisor Mode load and store protection */
        rv_csr_t mxr:1;                         /** bit:     19  Make executable readeable */
        rv_csr_t tvm:1;                         /** bit:     20 Trap virtual memory */
        rv_csr_t tw:1;                          /** bit:     21 Timeout wait */
        rv_csr_t tsr:1;                         /** bit:     22 Trap SRET */
        rv_csr_t _reserved3:8;                  /** bit:     23..30 Reserved  */
        rv_csr_t sd:1;                          /** bit:     Dirty status for XS or FS */
#endif
    } b;                                        /** Structure used for bit  access */
    rv_csr_t d;                                 /** Type      used for csr data access */
} CSR_MSTATUS_Type;

/**
 * \brief  Union type to access MIE configure register.
 */
typedef union {
    struct {
        rv_csr_t usie:1;                        /** bit:    0  software interrupt enable bit for u mode */
        rv_csr_t ssie:1;                        /** bit:    1  software interrupt enable bit for s mode */
        rv_csr_t _reserved0:1;                  /** bit:    2  Reserved */
        rv_csr_t msie:1;                        /** bit:    3  software interrupt enable bit for m mode */
        rv_csr_t utie:1;                        /** bit:    4  timer interrupt enable bit for u mode */
        rv_csr_t stie:1;                        /** bit:    5  timer interrupt enable bit for s mode */
        rv_csr_t _reserved1:1;                  /** bit:    6  Reserved */
        rv_csr_t mtie:1;                        /** bit:    7  timer interrupt enable bit for m mode */
        rv_csr_t ueie:1;                        /** bit:    8  external interrupt enable bit for u mode */
        rv_csr_t seie:1;                        /** bit:    9  external interrupt enable bit for s mode */
        rv_csr_t _reserved2:1;                  /** bit:    10 Reserved */
        rv_csr_t meie:1;                        /** bit:    11 external interrupt enable bit for m mode */
#if defined(__RISCV_XLEN) && (__RISCV_XLEN == 64)
        rv_csr_t _reserved3:52;                 /** bit:    12..63 Reserved */
#else
        rv_csr_t _reserved3:20;                 /** bit:    12..31 Reserved */
#endif
    } b;                                        /** Structure used for bit access */
    rv_csr_t d;                                 /** Type      used for csr data access */
} CSR_MIE_Type;

/**
 * \brief  Union type to access MIP configure register.
 */
typedef union {
    struct {
        rv_csr_t usip:1;                        /** bit:    0  software interrupt pending bit for u mode */
        rv_csr_t ssip:1;                        /** bit:    1  software interrupt pending bit for s mode */
        rv_csr_t _reserved0:1;                  /** bit:    2  Reserved */
        rv_csr_t msip:1;                        /** bit:    3  software interrupt pending bit for m mode */
        rv_csr_t utip:1;                        /** bit:    4  timer interrupt pending bit for u mode */
        rv_csr_t stip:1;                        /** bit:    5  timer interrupt pending bit for s mode */
        rv_csr_t _reserved1:1;                  /** bit:    6  Reserved */
        rv_csr_t mtip:1;                        /** bit:    7  timer interrupt pending bit for m mode */
        rv_csr_t ueip:1;                        /** bit:    8  external interrupt pending bit for u mode */
        rv_csr_t seip:1;                        /** bit:    9  external interrupt pending bit for s mode */
        rv_csr_t _reserved2:1;                  /** bit:    10 Reserved */
        rv_csr_t meip:1;                        /** bit:    11 external interrupt pending bit for m mode */
#if defined(__RISCV_XLEN) && (__RISCV_XLEN == 64)
        rv_csr_t _reserved3:52;                 /** bit:    12..63 Reserved */
#else
        rv_csr_t _reserved3:20;                 /** bit:    12..31 Reserved */
#endif
    } b;                                        /** Structure used for bit access */
    rv_csr_t d;                                 /** Type      used for csr data access */
} CSR_MIP_Type;

/**
 * \brief  Union type to access MTVEC configure register.
 */
typedef union {
    struct {
        rv_csr_t mode:2;                        /** bit:     0..1   interrupt mode control */
#if defined(__RISCV_XLEN) && __RISCV_XLEN == 64
        rv_csr_t addr:62;                       /** bit:     2..63  mtvec address */
#else
        rv_csr_t addr:30;                       /** bit:     2..31  mtvec address */
#endif
    } b;                                        /** Structure used for bit  access */
    rv_csr_t d;                                 /** Type      used for csr data access */
} CSR_MTVEC_Type;

/**
 * \brief  Union type to access MCAUSE configure register.
 */
typedef union {
    struct {
        rv_csr_t exccode:10;                    /** bit:     0..9   exception or interrupt code */
        rv_csr_t _reserved0:13;                 /** bit:     10..22 Reserved */
#ifdef CONFIG_RV_CLIC_PRESENT
        rv_csr_t mpie:1;                        /** bit:     23  Interrupt enable flag before enter interrupt */
        rv_csr_t mpil:4;                        /** bit:     24..27 Previous interrupt level */
        rv_csr_t mpp:2;                         /** bit:     28..29 Privilede mode flag before enter interrupt */
        rv_csr_t minhv:1;                       /** bit:     30  Machine interrupt vector table */
#else
        rv_csr_t _reserved1:8;                  /** bit:     23..30 Reserved for Non-clic mode */
#endif
#if defined(__RISCV_XLEN) && __RISCV_XLEN == 64
        rv_csr_t _reserved2:32;                 /** bit:     31..62  Reserved */
        rv_csr_t interrupt:1;                   /** bit:     63  trap type. 0 means exception and 1 means interrupt */
#else
        rv_csr_t interrupt:1;                   /** bit:     31  trap type. 0 means exception and 1 means interrupt */
#endif
    } b;                                        /** Structure used for bit  access */
    rv_csr_t d;                                 /** Type      used for csr data access */
} CSR_MCAUSE_Type;

/**
 * \brief  Union type to access MCOUNTINHIBIT configure register.
 */
typedef union {
    struct {
        rv_csr_t cy:1;                          /** bit:     0  1 means disable mcycle counter */
        rv_csr_t _reserved0:1;                  /** bit:     1  Reserved */
        rv_csr_t ir:1;                          /** bit:     2  1 means disable minstret counter */
#if defined(__RISCV_XLEN) && __RISCV_XLEN == 64
        rv_csr_t _reserved1:61;                 /** bit:     3..63 Reserved */
#else
        rv_csr_t hmp3:1;                        /** bit:     3  1 means disable hpm3 */
        rv_csr_t hmp4:1;                        /** bit:     4  1 means disable hpm4 */
        rv_csr_t hmp5:1;                        /** bit:     5  1 means disable hpm5 */
        rv_csr_t hmp6:1;                        /** bit:     6  1 means disable hpm6 */
        rv_csr_t hmp7:1;                        /** bit:     7  1 means disable hpm7 */
        rv_csr_t hmp8:1;                        /** bit:     8  1 means disable hpm8 */
        rv_csr_t hmp9:1;                        /** bit:     9  1 means disable hpm9 */
        rv_csr_t hmp10:1;                        /** bit:    10 1 means disable hpm10 */
        rv_csr_t hmp11:1;                        /** bit:    11 1 means disable hpm11 */
        rv_csr_t hmp12:1;                        /** bit:    12 1 means disable hpm12 */
        rv_csr_t hmp13:1;                        /** bit:    13 1 means disable hpm13 */
        rv_csr_t hmp14:1;                        /** bit:    14 1 means disable hpm14 */
        rv_csr_t hmp15:1;                        /** bit:    15 1 means disable hpm15 */
        rv_csr_t hmp16:1;                        /** bit:    16 1 means disable hpm16 */
        rv_csr_t hmp17:1;                        /** bit:    17 1 means disable hpm17 */
        rv_csr_t hmp18:1;                        /** bit:    18 1 means disable hpm18 */
        rv_csr_t hmp19:1;                        /** bit:    19 1 means disable hpm19 */
        rv_csr_t hmp20:1;                        /** bit:    20 1 means disable hpm20 */
        rv_csr_t hmp21:1;                        /** bit:    21 1 means disable hpm21 */
        rv_csr_t hmp22:1;                        /** bit:    22 1 means disable hpm22 */
        rv_csr_t hmp23:1;                        /** bit:    23 1 means disable hpm23 */
        rv_csr_t hmp24:1;                        /** bit:    24 1 means disable hpm24 */
        rv_csr_t hmp25:1;                        /** bit:    25 1 means disable hpm25 */
        rv_csr_t hmp26:1;                        /** bit:    26 1 means disable hpm26 */
        rv_csr_t hmp27:1;                        /** bit:    27 1 means disable hpm27 */
        rv_csr_t hmp28:1;                        /** bit:    28 1 means disable hpm28 */
        rv_csr_t hmp29:1;                        /** bit:    29 1 means disable hpm29 */
        rv_csr_t hmp30:1;                        /** bit:    30 1 means disable hpm30 */
        rv_csr_t hmp31:1;                        /** bit:    31 1 means disable hpm31 */
#endif
    } b;                                        /** Structure used for bit  access */
    rv_csr_t d;                                 /** Type      used for csr data access */
} CSR_MCOUNTINHIBIT_Type;

/**
 * @}
 */ /* End of Doxygen Group EMSIS_Core_Base_Registers */

/* ###########################  Core Function Access  ########################### */
/**
 * \defgroup EMSIS_Core_Function_Access    Core Function Access
 * \ingroup  EMSIS_Core
 * \brief    Functions to access the Core Functional
 * \details
 *
 * The following functions or macros provide access to Core CSR registers.
 * - \ref EMSIS_Core_CSR_Encoding
 * - \ref EMSIS_Core_CSR_Registers
 *   @{
 */

/**
 * \brief   Enable IRQ Interrupts
 * \details Enables IRQ interrupts by setting the MIE-bit in the MSTATUS Register.
 * \remarks
 *          Can only be executed in Privileged modes.
 */
__STATIC_FORCEINLINE void __enable_irq(void)
{
    __RV_CSR_SET(CSR_MSTATUS, MSTATUS_MIE);
}

/**
 * \brief   Set Interrupt Mode
 * \details Set Interrupt Mode as direct & vectored by setting Mode field in the MTVEC register.
 * \remarks
 *          Can only be executed in Privileged modes.
 */
__STATIC_FORCEINLINE void __set_interrupt_mode(rv_csr_t mode)
{
    mode &= MTVEC_MODE_Msk;
    __RV_CSR_CLEAR(CSR_MTVEC, MTVEC_MODE_Msk);
    __RV_CSR_SET(CSR_MTVEC, mode);
}

/**
 * \brief   Get Interrupt mode
 * \details Get Interrupt mode by reading interrupt mode in MTVEC register.
 * \remarks
 *          Can only be executed in Privileged modes.
 */
__STATIC_FORCEINLINE rv_csr_t __get_interrupt_mode(void)
{
    return __RV_CSR_READ(CSR_MTVEC) & MTVEC_MODE_Msk;
}

/**
 * \brief   Set Interrupt Handler table
 * \details Set Interrupt Handler table in MTVEC Register.
 * \remarks
 *          Can only be executed in Privileged modes.
 */
__STATIC_FORCEINLINE void __set_exc_entry(rv_csr_t addr)
{
    addr &= MTVEC_BASE_Msk;
    __RV_CSR_CLEAR(CSR_MTVEC, ~(MTVEC_MODE_Msk));
    __RV_CSR_SET(CSR_MTVEC, addr);
}

/**
 * \brief   Get Interrupt Handler table
 * \details Get Interrupt Handler table by reading Base field in MTVEC register.
 * \remarks
 *          Can only be executed in Privileged modes.
 */
__STATIC_FORCEINLINE rv_csr_t __get_exec_entry(void)
{
    return __RV_CSR_READ(CSR_MTVEC) & MTVEC_BASE_Msk;
}

/**
 * \brief   Disable IRQ Interrupts
 * \details Disables IRQ interrupts by clearing the MIE-bit in the MSTATUS Register.
 * \remarks
 *          Can only be executed in Privileged modes.
 */
__STATIC_FORCEINLINE void __disable_irq(void)
{
    __RV_CSR_CLEAR(CSR_MSTATUS, MSTATUS_MIE);
}

/**
 * \brief    enable software interrupt for m mode
 * \deftails enable m-mode software interrupt by setting msie in mie register
 * \remarks
 *           can only be executed in privileged modes.
 */
__STATIC_FORCEINLINE void __enable_soft_irq(void)
{
    __RV_CSR_SET(CSR_MIE, MIE_MSIE);
}

/**
 * \brief   Disable Software interrupt for M mode
 * \details Disable M-Mode software interrupt by clearing MSIE in MIE register
 * \remarks
 *          Can only be executed in Privileged modes.
 */
__STATIC_FORCEINLINE void __disable_soft_irq(void)
{
    __RV_CSR_CLEAR(CSR_MIE, MIE_MSIE);
}

/**
 * \brief    Enable Timer interrupt for M mode
 * \deftails Enable M-Mode Timer interrupt by setting MTIE in MIE register
 * \remarks
 *           Can only be executed in Privileged modes.
 */
__STATIC_FORCEINLINE void __enable_timer_irq(void)
{
    __RV_CSR_SET(CSR_MIE, MIE_MTIE);
}

/**
 * \brief   Disable Timer interrupt for M mode
 * \details Disable M-Mode Timer interrupt by clearing MTIE in MIE register
 * \remarks
 *          Can only be executed in Privileged modes.
 */
__STATIC_FORCEINLINE void __disable_timer_irq(void)
{
    __RV_CSR_CLEAR(CSR_MIE, MIE_MTIE);
}

/**
 * \brief    Enable External interrupt for M mode
 * \deftails Enable M-Mode External interrupt by setting MEIE in MIE register
 * \remarks
 *           Can only be executed in Privileged modes.
 */
__STATIC_FORCEINLINE void __enable_external_irq(void)
{
    __RV_CSR_SET(CSR_MIE, MIE_MEIE);
}

/**
 * \brief   Disable External interrupt for M mode
 * \details Disable M-Mode External interrupt by clearing MEIE in MIE register
 * \remarks
 *          Can only be executed in Privileged modes.
 */
__STATIC_FORCEINLINE void __disable_external_irq(void)
{
    __RV_CSR_CLEAR(CSR_MIE, MIE_MEIE);
}

/**
 * \brief   Read whole 64 bits value of mcycle counter
 * \details This function will read the whole 64 bits of MCYCLE register
 * \return  The whole 64 bits value of MCYCLE
 * \remarks It will work for both RV32 and RV64 to get full 64bits value of MCYCLE
 */
__STATIC_FORCEINLINE uint64_t __get_rv_cycle(void)
{
#if __RISCV_XLEN == 32
    volatile uint32_t high0, low, high;
    uint64_t full;

    high0 = __RV_CSR_READ(CSR_MCYCLEH);
    low = __RV_CSR_READ(CSR_MCYCLE);
    high = __RV_CSR_READ(CSR_MCYCLEH);
    if (high0 != high) {
        low = __RV_CSR_READ(CSR_MCYCLE);
    }
    full = (((uint64_t)high) << 32) | low;
    return full;
#elif __RISCV_XLEN == 64
    return (uint64_t)__RV_CSR_READ(CSR_MCYCLE);
#else // TODO Need cover for XLEN=128 case in future
    return (uint64_t)__RV_CSR_READ(CSR_MCYCLE);
#endif
}

/**
 * \brief   Read whole 64 bits value of machine instruction-retired counter
 * \details This function will read the whole 64 bits of MINSTRET register
 * \return  The whole 64 bits value of MINSTRET
 * \remarks It will work for both RV32 and RV64 to get full 64bits value of MINSTRET
 */
__STATIC_FORCEINLINE uint64_t __get_rv_instret(void)
{
#if __RISCV_XLEN == 32
    volatile uint32_t high0, low, high;
    uint64_t full;

    high0 = __RV_CSR_READ(CSR_MINSTRETH);
    low = __RV_CSR_READ(CSR_MINSTRET);
    high = __RV_CSR_READ(CSR_MINSTRETH);
    if (high0 != high) {
        low = __RV_CSR_READ(CSR_MINSTRET);
    }
    full = (((uint64_t)high) << 32) | low;
    return full;
#elif __RISCV_XLEN == 64
    return (uint64_t)__RV_CSR_READ(CSR_MINSTRET);
#else // TODO Need cover for XLEN=128 case in future
    return (uint64_t)__RV_CSR_READ(CSR_MINSTRET);
#endif
}

#if defined(__PRIVILEGE_SMODE_SUPPORT) && (__PRIVILEGE_SMODE_SUPPORT == 1)
/**
 * \brief   Read whole 64 bits value of real-time clock
 * \details This function will read the whole 64 bits of TIME register
 * \return  The whole 64 bits value of TIME CSR
 * \remarks It will work for both RV32 and RV64 to get full 64bits value of TIME
 * \attention only available when user mode available
 */
__STATIC_FORCEINLINE uint64_t __get_rv_time(void)
{
#if __RISCV_XLEN == 32
    volatile uint32_t high0, low, high;
    uint64_t full;

    high0 = __RV_CSR_READ(CSR_TIMEH);
    low = __RV_CSR_READ(CSR_TIME);
    high = __RV_CSR_READ(CSR_TIMEH);
    if (high0 != high) {
        low = __RV_CSR_READ(CSR_TIME);
    }
    full = (((uint64_t)high) << 32) | low;
    return full;
#elif __RISCV_XLEN == 64
    return (uint64_t)__RV_CSR_READ(CSR_TIME);
#else // TODO Need cover for XLEN=128 case in future
    return (uint64_t)__RV_CSR_READ(CSR_TIME);
#endif
}
#endif

/**
 * \brief   Enable MCYCLE counter
 * \details
 * Clear the CY bit of MCOUNTINHIBIT to 0 to enable MCYCLE Counter
 */
__STATIC_FORCEINLINE void __enable_mcycle_counter(void)
{
    __RV_CSR_CLEAR(CSR_MCOUNTINHIBIT, MCOUNTINHIBIT_CY);
}

/**
 * \brief   Disable MCYCLE counter
 * \details
 * Set the CY bit of MCOUNTINHIBIT to 1 to disable MCYCLE Counter
 */
__STATIC_FORCEINLINE void __disable_mcycle_counter(void)
{
    __RV_CSR_SET(CSR_MCOUNTINHIBIT, MCOUNTINHIBIT_CY);
}

/**
 * \brief   Enable MINSTRET counter
 * \details
 * Clear the IR bit of MCOUNTINHIBIT to 0 to enable MINSTRET Counter
 */
__STATIC_FORCEINLINE void __enable_minstret_counter(void)
{
    __RV_CSR_CLEAR(CSR_MCOUNTINHIBIT, MCOUNTINHIBIT_IR);
}

/**
 * \brief   Disable MINSTRET counter
 * \details
 * Set the IR bit of MCOUNTINHIBIT to 1 to disable MINSTRET Counter
 */
__STATIC_FORCEINLINE void __disable_minstret_counter(void)
{
    __RV_CSR_SET(CSR_MCOUNTINHIBIT, MCOUNTINHIBIT_IR);
}

/**
 * \brief   Enable MCYCLE & MINSTRET counter
 * \details
 * Clear the IR and CY bit of MCOUNTINHIBIT to 1 to enable MINSTRET & MCYCLE Counter
 */
__STATIC_FORCEINLINE void __enable_all_counter(void)
{
    __RV_CSR_CLEAR(CSR_MCOUNTINHIBIT, MCOUNTINHIBIT_IR|MCOUNTINHIBIT_CY);
}

/**
 * \brief   Disable MCYCLE & MINSTRET counter
 * \details
 * Set the IR and CY bit of MCOUNTINHIBIT to 1 to disable MINSTRET & MCYCLE Counter
 */
__STATIC_FORCEINLINE void __disable_all_counter(void)
{
    __RV_CSR_SET(CSR_MCOUNTINHIBIT, MCOUNTINHIBIT_IR|MCOUNTINHIBIT_CY);
}

/**
 * \brief  Get Hartid
 * \details
 * Get Hartid from CSR_MHARTID
 */
__STATIC_FORCEINLINE uint32_t __get_hartid(void)
{
    return (uint32_t)__RV_CSR_READ(CSR_MHARTID);
}

/**
 * @}
 */ /* End of Doxygen Group EMSIS_Core_Function_Access */

#ifdef __cplusplus
}
#endif
#endif /* __CORE_FEATURE_BASE__ */
