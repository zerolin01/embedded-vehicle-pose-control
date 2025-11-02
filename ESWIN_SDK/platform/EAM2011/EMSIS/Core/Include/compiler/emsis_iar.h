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

#ifndef __EMSIS_IAR_H__
#define __EMSIS_IAR_H__
/**
 * @file     emsis_iar.h
 * @brief    EMSIS compiler GCC header file
 */

#include <intrinsics.h>

#ifdef __cplusplus
 extern "C" {
#endif

#ifndef __RISCV_XLEN
  /** \brief Refer to the width of an integer register in bits(either 32 or 64) */
  #ifndef __riscv_xlen
    #define __RISCV_XLEN    32
  #else
    #define __RISCV_XLEN    __riscv_xlen
  #endif
#endif /* __RISCV_XLEN */

/** \brief Pass information from the compiler to the assembler. */
#ifndef   __ASM
  #define __ASM                 __asm
#endif

/** \brief Barrier to prevent compiler from reordering instructions. */
#ifndef   __COMPILER_BARRIER
  #define __COMPILER_BARRIER()  __ASM volatile("":::"memory")
#endif

/** \brief Recommend that function should be inlined by the compiler. */
#ifndef __INLINE
  #define __INLINE              inline
#endif

/** \brief Define a static function that may be inlined by the compiler. */
#ifndef   __STATIC_INLINE
  #define __STATIC_INLINE       static inline
#endif

/** \brief Define a static function that may be inlined by the compiler. */
#ifndef   __FORCEINLINE
  #define __FORCEINLINE         _Pragma("inline = forced")
#endif

/** \brief Define a static function that should be always inlined by the compiler. */
#ifndef   __STATIC_FORCEINLINE
  #define __STATIC_FORCEINLINE  __FORCEINLINE __STATIC_INLINE
#endif

/** \brief restrict pointer qualifier to enable additional optimizations. */
#ifndef   __WEAK
  #define __WEAK                _Pragma("object_attribute = __weak")
  #define __WEAK_DEFS           1
#endif
#ifndef   __INTERRUPT
  #define __INTERRUPT           __interrupt
#endif

/** \brief provide the compiler with branch prediction information, the branch is usually true */
#ifndef   __USUALLY
  #define __USUALLY(exp)        (exp)
#endif

/** \brief provide the compiler with branch prediction information, the branch is rarely true */
#ifndef   __RARELY
  #define __RARELY(exp)         (exp)
#endif

/**
 * @}
 */ /* End of Doxygen Group EMSIS_Core_CompilerControl */

/* IO definitions (access restrictions to peripheral registers) */
/**
 * \defgroup EMSIS_Core_PeriphAccess     Peripheral Access
 * \brief  Naming conventions and optional features for accessing peripherals.
 *
 * The section below describes the naming conventions, requirements, and optional features
 * for accessing device specific peripherals.
 * Most of the rules also apply to the core peripherals.
 *
 * The **Device Header File <device.h>** contains typically these definition
 * and also includes the core specific header files.
 *
 * @{
 */
/** \brief Defines 'read only' permissions */
#ifdef __cplusplus
  #define   __I     volatile
#else
  #define   __I     volatile const
#endif
/** \brief Defines 'write only' permissions */
#define     __O     volatile
/** \brief Defines 'read / write' permissions */
#define     __IO    volatile

/* following defines should be used for structure members */
/** \brief Defines 'read only' structure member permissions */
#define     __IM     volatile const
/** \brief Defines 'write only' structure member permissions */
#define     __OM     volatile
/** \brief Defines 'read/write' structure member permissions */
#define     __IOM    volatile

/**
 * \brief   Mask and shift a bit field value for use in a register bit range.
 * \details The macro \ref _VAL2FLD uses the #define's _Pos and _Msk of the related bit
 * field to shift bit-field values for assigning to a register.
 *
 * **Example**:
 * \code
 * CLIC->CFG = _VAL2FLD(CLIC_CLICCFG_NLBIT, 3);
 * \endcode
 * \param[in] field  Name of the register bit field.
 * \param[in] value  Value of the bit field. This parameter is interpreted as an uint32_t type.
 * \return           Masked and shifted value.
 */
#define _VAL2FLD(field, value)    (((uint32_t)(value) << field ## _Pos) & field ## _Msk)

/**
 * \brief   Mask and shift a register value to extract a bit filed value.
 * \details The macro \ref _FLD2VAL uses the #define's _Pos and _Msk of the related bit
 * field to extract the value of a bit field from a register.
 *
 * **Example**:
 * \code
 * nlbits = _FLD2VAL(CLIC_CLICCFG_NLBIT, CLIC->CFG);
 * \endcode
 * \param[in] field  Name of the register bit field.
 * \param[in] value  Value of register. This parameter is interpreted as an uint32_t type.
 * \return           Masked and shifted bit field value.
 */
#define _FLD2VAL(field, value)    (((uint32_t)(value) & field ## _Msk) >> field ## _Pos)

/**
 * @}
 */ /* end of group EMSIS_Core_PeriphAccess */

/* ########################## CSR Register Access ############################## */
/**
 * \defgroup EMSIS_Core_CSR_Register_Access    Core CSR Register Access
 * \ingroup  EMSIS_Core
 * \brief    Functions to access CSR Registers
 * \details
 *   @{
 */

#ifndef __ASSEMBLY__

/**
 * \brief CSR operation Macro for csrrw instruction.
 * \details
 * Read the content of csr register to __v,
 * then write content of val into csr register, then return __v
 * \param csr   CSR macro definition defined in
 *              \ref EMSIS_Core_CSR_Registers, eg. \ref CSR_MSTATUS
 * \param val   value to store into the CSR register
 * \return the CSR register value before written
 */
#define __RV_CSR_SWAP(csr, val)                                 \
    ({                                                          \
        register rv_csr_t __v = (unsigned long)(0);             \
        __ASM volatile("csrrw %0, " STRINGIFY(csr) ", %1"       \
                     : "=r"(__v)                                \
                     : "r"(val)                                 \
                     : "memory");                               \
        __v;                                                    \
    })

/**
 * \brief CSR operation Macro for csrr instruction.
 * \details
 * Read the content of csr register to __v and return it
 * \param csr   CSR macro definition defined in
 *              \ref EMSIS_Core_CSR_Registers, eg. \ref CSR_MSTATUS
 * \return the CSR register value
 */
#define __RV_CSR_READ(csr)                                      \
    ({                                                          \
        register rv_csr_t __v;                                  \
        __ASM volatile("csrr %0, " STRINGIFY(csr)               \
                     : "=r"(__v)                                \
                     :                                          \
                     : "memory");                               \
        __v;                                                    \
    })

/**
 * \brief CSR operation Macro for csrw instruction.
 * \details
 * Write the content of val to csr register
 * \param csr   CSR macro definition defined in
 *              \ref EMSIS_Core_CSR_Registers, eg. \ref CSR_MSTATUS
 * \param val   value to store into the CSR register
 */
#define __RV_CSR_WRITE(csr, val)                                \
    ({                                                          \
        __ASM volatile("csrw " STRINGIFY(csr) ", %0"            \
                     :                                          \
                     : "r"(val)                                 \
                     : "memory");                               \
    })

/**
 * \brief CSR operation Macro for csrrs instruction.
 * \details
 * Read the content of csr register to __v,
 * then set csr register to be __v | val, then return __v
 * \param csr   CSR macro definition defined in
 *              \ref EMSIS_Core_CSR_Registers, eg. \ref CSR_MSTATUS
 * \param val   Mask value to be used wih csrrs instruction
 * \return the CSR register value before written
 */
#define __RV_CSR_READ_SET(csr, val)                             \
    ({                                                          \
        register rv_csr_t __v = (rv_csr_t)(0);                  \
        __ASM volatile("csrrs %0, " STRINGIFY(csr) ", %1"       \
                     : "=r"(__v)                                \
                     : "r"(val)                                 \
                     : "memory");                               \
        __v;                                                    \
    })

/**
 * \brief CSR operation Macro for csrs instruction.
 * \details
 * Set csr register to be csr_content | val
 * \param csr   CSR macro definition defined in
 *              \ref EMSIS_Core_CSR_Registers, eg. \ref CSR_MSTATUS
 * \param val   Mask value to be used wih csrs instruction
 */
#define __RV_CSR_SET(csr, val)                                  \
    ({                                                          \
        __ASM volatile("csrs " STRINGIFY(csr) ", %0"            \
                     :                                          \
                     : "r"(val)                                 \
                     : "memory");                               \
    })

/**
 * \brief CSR operation Macro for csrrc instruction.
 * \details
 * Read the content of csr register to __v,
 * then set csr register to be __v & ~val, then return __v
 * \param csr   CSR macro definition defined in
 *              \ref EMSIS_Core_CSR_Registers, eg. \ref CSR_MSTATUS
 * \param val   Mask value to be used wih csrrc instruction
 * \return the CSR register value before written
 */
#define __RV_CSR_READ_CLEAR(csr, val)                           \
    ({                                                          \
        register rv_csr_t __v = (rv_csr_t)(0);                  \
        __ASM volatile("csrrc %0, " STRINGIFY(csr) ", %1"       \
                     : "=r"(__v)                                \
                     : "r"(val)                                 \
                     : "memory");                               \
        __v;                                                    \
    })

/**
 * \brief CSR operation Macro for csrc instruction.
 * \details
 * Set csr register to be csr_content & ~val
 * \param csr   CSR macro definition defined in
 *              \ref EMSIS_Core_CSR_Registers, eg. \ref CSR_MSTATUS
 * \param val   Mask value to be used wih csrc instruction
 */
#define __RV_CSR_CLEAR(csr, val)                                \
    ({                                                          \
        __ASM volatile("csrc " STRINGIFY(csr) ", %0"            \
                     :                                          \
                     : "r"(val)                                 \
                     : "memory");                               \
    })
#endif /* __ASSEMBLY__ */

/**
 * @}
 */ /* End of Doxygen Group EMSIS_Core_CSR_Register_Access */

/* ###########################  CPU Intrinsic Functions ########################### */
/**
 * \defgroup EMSIS_Core_CPU_Intrinsic   Intrinsic Functions for CPU Instructions
 * \ingroup  EMSIS_Core
 * \brief    Functions that generate RISC-V CPU instructions.
 * \details
 *
 * The following functions generate specified RISC-V instructions that cannot be directly accessed by compiler.
 *   @{
 */

/**
 * \brief   NOP Instruction
 * \details
 * No Operation does nothing.
 * This instruction can be used for code alignment purposes.
 */
__STATIC_FORCEINLINE void __NOP(void)
{
    __ASM volatile("nop");
}

/**
 * \brief   Wait For Interrupt
 * \details
 * Wait For Interrupt is is executed using WFI instruction.
 * It will suspends execution until interrupt, NMI or Debug happened.
 * When Core is waked up by interrupt, if
 * 1. mstatus.MIE == 1(interrupt enabled), Core will enter ISR code
 * 2. mstatus.MIE == 0(interrupt disabled), Core will resume previous execution
 */
__STATIC_FORCEINLINE void __WFI(void)
{
    __ASM volatile("wfi");
}

/**
 * \brief   Breakpoint Instruction
 * \details
 * Causes the processor to enter Debug state.
 * Debug tools can use this to investigate system state
 * when the instruction at a particular address is reached.
 */
__STATIC_FORCEINLINE void __EBREAK(void)
{
    __ASM volatile("ebreak");
}

/**
 * \brief   Environment Call Instruction
 * \details
 * The ECALL instruction is used to make a service request to
 * the execution environment.
 */
__STATIC_FORCEINLINE void __ECALL(void)
{
    __ASM volatile("ecall");
}

/**
 * \brief Execute fence instruction, p -> pred, s -> succ
 * \details
 * the FENCE instruction ensures that all memory accesses from instructions preceding
 * the fence in program order (the `predecessor set`) appear earlier in the global memory order than
 * memory accesses from instructions appearing after the fence in program order (the `successor set`).
 * For details, please refer to The RISC-V Instruction Set Manual
 * \param p     predecessor set, such as iorw, rw, r, w
 * \param s     successor set, such as iorw, rw, r, w
 **/
#define __FENCE(p, s) __ASM volatile ("fence " #p "," #s : : : "memory")

/**
 * \brief   Fence.i Instruction
 * \details
 * The FENCE.I instruction is used to synchronize the instruction
 * and data streams.
 */
__STATIC_FORCEINLINE void __FENCE_I(void)
{
    __ASM volatile("fence.i");
}

/** \brief Read & Write Memory barrier */
#define __RWMB()        __FENCE(iorw,iorw)

/** \brief Read Memory barrier */
#define __RMB()         __FENCE(ir,ir)

/** \brief Write Memory barrier */
#define __WMB()         __FENCE(ow,ow)

/** \brief SMP Read & Write Memory barrier */
#define __SMP_RWMB()    __FENCE(rw,rw)

/** \brief SMP Read Memory barrier */
#define __SMP_RMB()     __FENCE(r,r)

/** \brief SMP Write Memory barrier */
#define __SMP_WMB()     __FENCE(w,w)

/** \brief CPU relax for busy loop */
#define __CPU_RELAX()   __ASM volatile ("" : : : "memory")

/* ===== Load/Store Operations ===== */
/**
 * \brief  Load 8bit value from address (8 bit)
 * \details Load 8 bit value.
 * \param [in]    addr  Address pointer to data
 * \return              value of type uint8_t at (*addr)
 */
__STATIC_FORCEINLINE uint8_t __LB(volatile void *addr)
{
    uint8_t result;

    __ASM volatile ("lb %0, 0(%1)" : "=r" (result) : "r" (addr));
    return result;
}

/**
 * \brief  Load 16bit value from address (16 bit)
 * \details Load 16 bit value.
 * \param [in]    addr  Address pointer to data
 * \return              value of type uint16_t at (*addr)
 */
__STATIC_FORCEINLINE uint16_t __LH(volatile void *addr)
{
    uint16_t result;

    __ASM volatile ("lh %0, 0(%1)" : "=r" (result) : "r" (addr));
    return result;
}

/**
 * \brief  Load 32bit value from address (32 bit)
 * \details Load 32 bit value.
 * \param [in]    addr  Address pointer to data
 * \return              value of type uint32_t at (*addr)
 */
__STATIC_FORCEINLINE uint32_t __LW(volatile void *addr)
{
    uint32_t result;

    __ASM volatile ("lw %0, 0(%1)" : "=r" (result) : "r" (addr));
    return result;
}

#if __RISCV_XLEN != 32
/**
 * \brief  Load 64bit value from address (64 bit)
 * \details Load 64 bit value.
 * \param [in]    addr  Address pointer to data
 * \return              value of type uint64_t at (*addr)
 * \remarks RV64 only macro
 */
__STATIC_FORCEINLINE uint64_t __LD(volatile void *addr)
{
    uint64_t result;
    __ASM volatile ("ld %0, 0(%1)" : "=r" (result) : "r" (addr));
    return result;
}
#endif

/**
 * \brief  Write 8bit value to address (8 bit)
 * \details Write 8 bit value.
 * \param [in]    addr  Address pointer to data
 * \param [in]    val   Value to set
 */
__STATIC_FORCEINLINE void __SB(volatile void *addr, uint8_t val)
{
    __ASM volatile ("sb %0, 0(%1)" : : "r" (val), "r" (addr));
}

/**
 * \brief  Write 16bit value to address (16 bit)
 * \details Write 16 bit value.
 * \param [in]    addr  Address pointer to data
 * \param [in]    val   Value to set
 */
__STATIC_FORCEINLINE void __SH(volatile void *addr, uint16_t val)
{
    __ASM volatile ("sh %0, 0(%1)" : : "r" (val), "r" (addr));
}

/**
 * \brief  Write 32bit value to address (32 bit)
 * \details Write 32 bit value.
 * \param [in]    addr  Address pointer to data
 * \param [in]    val   Value to set
 */
__STATIC_FORCEINLINE void __SW(volatile void *addr, uint32_t val)
{
    __ASM volatile ("sw %0, 0(%1)" : : "r" (val), "r" (addr));
}

#if __RISCV_XLEN != 32
/**
 * \brief  Write 64bit value to address (64 bit)
 * \details Write 64 bit value.
 * \param [in]    addr  Address pointer to data
 * \param [in]    val   Value to set
 */
__STATIC_FORCEINLINE void __SD(volatile void *addr, uint64_t val)
{
    __ASM volatile ("sd %0, 0(%1)" : : "r" (val), "r" (addr));
}
#endif

/**
 * \brief  Compare and Swap 32bit value using LR and SC
 * \details Compare old value with memory, if identical,
 * store new value in memory. Return the initial value in memory.
 * Success is indicated by comparing return value with OLD.
 * memory address, return 0 if successful, otherwise return !0
 * \param [in]    addr      Address pointer to data, address need to be 4byte aligned
 * \param [in]    oldval    Old value of the data in address
 * \param [in]    newval    New value to be stored into the address
 * \return  return the initial value in memory
 */
__STATIC_FORCEINLINE uint32_t __CAS_W(volatile uint32_t *addr, uint32_t oldval, uint32_t newval)
{
    register uint32_t result;
    register uint32_t rc;

    __ASM volatile (                                \
            "0:     lr.w %0, %2      \n"            \
            "       bne  %0, %z3, 1f \n"            \
            "       sc.w %1, %z4, %2 \n"            \
            "       bnez %1, 0b      \n"            \
            "1:\n"                                  \
            : "=&r"(result), "=&r"(rc), "+A"(*addr) \
            : "r"(oldval), "r"(newval)              \
            : "memory");
    return result;
}

/**
 * \brief  Atomic Swap 32bit value into memory
 * \details Atomically swap new 32bit value into memory using amoswap.d.
 * \param [in]    addr      Address pointer to data, address need to be 4byte aligned
 * \param [in]    newval    New value to be stored into the address
 * \return  return the original value in memory
 */
__STATIC_FORCEINLINE uint32_t __AMOSWAP_W(volatile uint32_t *addr, uint32_t newval)
{
    register uint32_t result;

    __ASM volatile ("amoswap.w %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(newval) : "memory");
    return result;
}

/**
 * \brief  Atomic Swap 32bit value into memory with aq
 * \details Atomically swap new 32bit value into memory using amoswap.d.
 * \param [in]    addr      Address pointer to data, address need to be 4byte aligned
 * \param [in]    newval    New value to be stored into the address
 * \return  return the original value in memory
 */
__STATIC_FORCEINLINE uint32_t __AMOSWAP_W_AQ(volatile uint32_t *addr, uint32_t newval)
{
    register uint32_t result;

    __ASM volatile ("amoswap.w.aq %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(newval) : "memory");
    return result;
}

/**
 * \brief  Atomic Swap 32bit value into memory with rl
 * \details Atomically swap new 32bit value into memory using amoswap.d.
 * \param [in]    addr      Address pointer to data, address need to be 4byte aligned
 * \param [in]    newval    New value to be stored into the address
 * \return  return the original value in memory
 */
__STATIC_FORCEINLINE uint32_t __AMOSWAP_W_RL(volatile uint32_t *addr, uint32_t newval)
{
    register uint32_t result;

    __ASM volatile ("amoswap.w.rl %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(newval) : "memory");
    return result;
}

#pragma diag_suppress=Pe550
/**
 * \brief  Atomic Add with 32bit value
 * \details Atomically ADD 32bit value with value in memory using amoadd.d.
 * \param [in]    addr   Address pointer to data, address need to be 4byte aligned
 * \param [in]    value  value to be ADDed
 * \return  return memory value + add value
 */
__STATIC_FORCEINLINE int32_t __AMOADD_W(volatile int32_t *addr, int32_t value)
{
    register int32_t result;

    __ASM volatile ("amoadd.w %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(value) : "memory");
    return *addr;
}

/**
 * \brief  Atomic And with 32bit value
 * \details Atomically AND 32bit value with value in memory using amoand.d.
 * \param [in]    addr   Address pointer to data, address need to be 4byte aligned
 * \param [in]    value  value to be ANDed
 * \return  return memory value & and value
 */
__STATIC_FORCEINLINE int32_t __AMOAND_W(volatile int32_t *addr, int32_t value)
{
    register int32_t result;

    __ASM volatile ("amoand.w %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(value) : "memory");
    return *addr;
}

/**
 * \brief  Atomic OR with 32bit value
 * \details Atomically OR 32bit value with value in memory using amoor.d.
 * \param [in]    addr   Address pointer to data, address need to be 4byte aligned
 * \param [in]    value  value to be ORed
 * \return  return memory value | and value
 */
__STATIC_FORCEINLINE int32_t __AMOOR_W(volatile int32_t *addr, int32_t value)
{
    register int32_t result;

    __ASM volatile ("amoor.w %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(value) : "memory");
    return *addr;
}

/**
 * \brief  Atomic XOR with 32bit value
 * \details Atomically XOR 32bit value with value in memory using amoxor.d.
 * \param [in]    addr   Address pointer to data, address need to be 4byte aligned
 * \param [in]    value  value to be XORed
 * \return  return memory value ^ and value
 */
__STATIC_FORCEINLINE int32_t __AMOXOR_W(volatile int32_t *addr, int32_t value)
{
    register int32_t result;

    __ASM volatile ("amoxor.w %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(value) : "memory");
    return *addr;
}

/**
 * \brief  Atomic unsigned MAX with 32bit value
 * \details Atomically unsigned max compare 32bit value with value in memory using amomaxu.d.
 * \param [in]    addr   Address pointer to data, address need to be 4byte aligned
 * \param [in]    value  value to be compared
 * \return  return the bigger value
 */
__STATIC_FORCEINLINE uint32_t __AMOMAXU_W(volatile uint32_t *addr, uint32_t value)
{
    register uint32_t result;

    __ASM volatile ("amomaxu.w %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(value) : "memory");
    return *addr;
}

/**
 * \brief  Atomic signed MAX with 32bit value
 * \details Atomically signed max compare 32bit value with value in memory using amomax.d.
 * \param [in]    addr   Address pointer to data, address need to be 4byte aligned
 * \param [in]    value  value to be compared
 * \return the bigger value
 */
__STATIC_FORCEINLINE int32_t __AMOMAX_W(volatile int32_t *addr, int32_t value)
{
    register int32_t result;

    __ASM volatile ("amomax.w %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(value) : "memory");
    return *addr;
}

/**
 * \brief  Atomic unsigned MIN with 32bit value
 * \details Atomically unsigned min compare 32bit value with value in memory using amominu.d.
 * \param [in]    addr   Address pointer to data, address need to be 4byte aligned
 * \param [in]    value  value to be compared
 * \return the smaller value
 */
__STATIC_FORCEINLINE uint32_t __AMOMINU_W(volatile uint32_t *addr, uint32_t value)
{
    register uint32_t result;

    __ASM volatile ("amominu.w %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(value) : "memory");
    return *addr;
}

/**
 * \brief  Atomic signed MIN with 32bit value
 * \details Atomically signed min compare 32bit value with value in memory using amomin.d.
 * \param [in]    addr   Address pointer to data, address need to be 4byte aligned
 * \param [in]    value  value to be compared
 * \return  the smaller value
 */
__STATIC_FORCEINLINE int32_t __AMOMIN_W(volatile int32_t *addr, int32_t value)
{
    register int32_t result;

    __ASM volatile ("amomin.w %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(value) : "memory");
    return *addr;
}
#pragma diag_default=Pe550

#if __RISCV_XLEN == 64
/**
 * \brief  Compare and Swap 64bit value using LR and SC
 * \details Compare old value with memory, if identical,
 * store new value in memory. Return the initial value in memory.
 * Success is indicated by comparing return value with OLD.
 * memory address, return 0 if successful, otherwise return !0
 * \param [in]    addr      Address pointer to data, address need to be 8byte aligned
 * \param [in]    oldval    Old value of the data in address
 * \param [in]    newval    New value to be stored into the address
 * \return  return the initial value in memory
 */
__STATIC_FORCEINLINE uint64_t __CAS_D(volatile uint64_t *addr, uint64_t oldval, uint64_t newval)
{
    register uint64_t result;
    register uint64_t rc;

    __ASM volatile (                                \
            "0:     lr.d %0, %2      \n"            \
            "       bne  %0, %z3, 1f \n"            \
            "       sc.d %1, %z4, %2 \n"            \
            "       bnez %1, 0b      \n"            \
            "1:\n"                                  \
            : "=&r"(result), "=&r"(rc), "+A"(*addr) \
            : "r"(oldval), "r"(newval)              \
            : "memory");
    return result;
}

/**
 * \brief  Atomic Swap 64bit value into memory
 * \details Atomically swap new 64bit value into memory using amoswap.d.
 * \param [in]    addr      Address pointer to data, address need to be 8byte aligned
 * \param [in]    newval    New value to be stored into the address
 * \return  return the original value in memory
 */
__STATIC_FORCEINLINE uint64_t __AMOSWAP_D(volatile uint64_t *addr, uint64_t newval)
{
    register uint64_t result;

    __ASM volatile ("amoswap.d %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(newval) : "memory");
    return result;
}

/**
 * \brief  Atomic Add with 64bit value
 * \details Atomically ADD 64bit value with value in memory using amoadd.d.
 * \param [in]    addr   Address pointer to data, address need to be 8byte aligned
 * \param [in]    value  value to be ADDed
 * \return  return memory value + add value
 */
__STATIC_FORCEINLINE int64_t __AMOADD_D(volatile int64_t *addr, int64_t value)
{
    register int64_t result;

    __ASM volatile ("amoadd.d %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(value) : "memory");
    return *addr;
}

/**
 * \brief  Atomic And with 64bit value
 * \details Atomically AND 64bit value with value in memory using amoand.d.
 * \param [in]    addr   Address pointer to data, address need to be 8byte aligned
 * \param [in]    value  value to be ANDed
 * \return  return memory value & and value
 */
__STATIC_FORCEINLINE int64_t __AMOAND_D(volatile int64_t *addr, int64_t value)
{
    register int64_t result;

    __ASM volatile ("amoand.d %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(value) : "memory");
    return *addr;
}

/**
 * \brief  Atomic OR with 64bit value
 * \details Atomically OR 64bit value with value in memory using amoor.d.
 * \param [in]    addr   Address pointer to data, address need to be 8byte aligned
 * \param [in]    value  value to be ORed
 * \return  return memory value | and value
 */
__STATIC_FORCEINLINE int64_t __AMOOR_D(volatile int64_t *addr, int64_t value)
{
    register int64_t result;

    __ASM volatile ("amoor.d %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(value) : "memory");
    return *addr;
}

/**
 * \brief  Atomic XOR with 64bit value
 * \details Atomically XOR 64bit value with value in memory using amoxor.d.
 * \param [in]    addr   Address pointer to data, address need to be 8byte aligned
 * \param [in]    value  value to be XORed
 * \return  return memory value ^ and value
 */
__STATIC_FORCEINLINE int64_t __AMOXOR_D(volatile int64_t *addr, int64_t value)
{
    register int64_t result;

    __ASM volatile ("amoxor.d %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(value) : "memory");
    return *addr;
}

/**
 * \brief  Atomic unsigned MAX with 64bit value
 * \details Atomically unsigned max compare 64bit value with value in memory using amomaxu.d.
 * \param [in]    addr   Address pointer to data, address need to be 8byte aligned
 * \param [in]    value  value to be compared
 * \return  return the bigger value
 */
__STATIC_FORCEINLINE uint64_t __AMOMAXU_D(volatile uint64_t *addr, uint64_t value)
{
    register uint64_t result;

    __ASM volatile ("amomaxu.d %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(value) : "memory");
    return *addr;
}

/**
 * \brief  Atomic signed MAX with 64bit value
 * \details Atomically signed max compare 64bit value with value in memory using amomax.d.
 * \param [in]    addr   Address pointer to data, address need to be 8byte aligned
 * \param [in]    value  value to be compared
 * \return the bigger value
 */
__STATIC_FORCEINLINE int64_t __AMOMAX_D(volatile int64_t *addr, int64_t value)
{
    register int64_t result;

    __ASM volatile ("amomax.d %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(value) : "memory");
    return *addr;
}

/**
 * \brief  Atomic unsigned MIN with 64bit value
 * \details Atomically unsigned min compare 64bit value with value in memory using amominu.d.
 * \param [in]    addr   Address pointer to data, address need to be 8byte aligned
 * \param [in]    value  value to be compared
 * \return the smaller value
 */
__STATIC_FORCEINLINE uint64_t __AMOMINU_D(volatile uint64_t *addr, uint64_t value)
{
    register uint64_t result;

    __ASM volatile ("amominu.d %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(value) : "memory");
    return *addr;
}

/**
 * \brief  Atomic signed MIN with 64bit value
 * \details Atomically signed min compare 64bit value with value in memory using amomin.d.
 * \param [in]    addr   Address pointer to data, address need to be 8byte aligned
 * \param [in]    value  value to be compared
 * \return  the smaller value
 */
__STATIC_FORCEINLINE int64_t __AMOMIN_D(volatile int64_t *addr, int64_t value)
{
    register int64_t result;

    __ASM volatile ("amomin.d %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(value) : "memory");
    return *addr;
}
#endif /* __RISCV_XLEN == 64  */

/**
 * @}
 */ /* End of Doxygen Group EMSIS_Core_CPU_Intrinsic */

#ifdef __cplusplus
}
#endif
#endif /* __EMSIS_GCC_H__ */
