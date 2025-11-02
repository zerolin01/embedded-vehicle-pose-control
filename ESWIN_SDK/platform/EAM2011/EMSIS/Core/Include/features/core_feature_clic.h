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

#ifndef __CORE_FEATURE_CLIC_H__
#define __CORE_FEATURE_CLIC_H__

#include "e320_config.h"

/**
 * @file     core_feature_clic.h
 * @brief    CLIC feature API header file for Eswin Core
 */

#ifdef __cplusplus
 extern "C" {
#endif

#ifdef CONFIG_RV_CLIC_PRESENT

#define CLIC_NUM 	CONFIG_RV_CLIC_NUM

/* Include the special defines for device */
#include "core_feature_clic_kittyhawk.h"

/* CLIC Memory Mapping of Device */
#define CLIC_BASE(x)        (CLIC_Impl *)CONFIG_RV_CLIC_BASEADDR_##x                    /** CLIC Base Address */

/* MSIP Memory Mapping of Device */
#define MSIP_BASE(x)        (MSIP_Impl *)CONFIG_RV_CLIC_MSIP_BASEADDR_##x                /** MSIP Base Address */

typedef struct {
    CLIC_Impl *impl[CLIC_NUM];
} CLIC_Type;

typedef struct {
    MSIP_Impl *impl[CLIC_NUM];
} MSIP_Type;

extern CLIC_Type *clic;
extern MSIP_Type *msip;

/**
 * \brief  Definition of IRQn numbers
 * \details
 * The core interrupt enumeration names for IRQn values are defined in the file <b><Device>.h</b>.
 * - Interrupt ID(IRQn) from 0 to 15 are reserved for core internal interrupts.
 * - Interrupt ID(IRQn) start from 16 represent device-specific external interrupts.
 * - The first device-specific interrupt has the IRQn value 16.
 *
 * The table below describes the core interrupt names and their availability in various Eswin Cores.
 */
/* The following enum IRQn definition in this file
 * is only used for doxygen documentation generation,
 * The <Device>.h is the real file to define it by vendor
 */
#if defined(__ONLY_FOR_DOXYGEN_DOCUMENT_GENERATION__)
typedef enum IRQn {
    /* ========= Eswin Core Specific Interrupt Numbers  =========== */
    /* Core Internal Interrupt IRQn definitions */
    Reserved0_IRQn            =   0,              /**  Internal reserved */
    Reserved1_IRQn            =   1,              /**  Internal reserved */
    Reserved2_IRQn            =   2,              /**  Internal reserved */
    Software_IRQn             =   3,              /**  Software interrupt */
    Reserved3_IRQn            =   4,              /**  Internal reserved */
    Reserved4_IRQn            =   5,              /**  Internal reserved */
    Reserved5_IRQn            =   6,              /**  Internal reserved */
    Timer_IRQn                =   7,              /**  Timer Interrupt */
    Reserved6_IRQn            =   8,              /**  Internal reserved */
    Reserved7_IRQn            =   9,              /**  Internal reserved */
    Reserved8_IRQn            =  10,              /**  Internal reserved */
    External_IRQn             =  11,              /**  External reserved */
    Clicsoftware_IRQn         =  12,              /**  clic software reserved */
    Reserved11_IRQn           =  13,              /**  Internal reserved */
    Reserved12_IRQn           =  14,              /**  Internal reserved */
    Reserved13_IRQn           =  15,              /**  Internal reserved */

    /* ========= Device Specific Interrupt Numbers  =================== */
    /* ToDo: add here your device specific external interrupt numbers.
     * 16~max(NUM_INTERRUPT, 127) is reserved number for user.
     * Maxmum interrupt supported could get from clicinfo.NUM_INTERRUPT.
     * According the interrupt handlers defined in startup_Device.S
     * eg.: Interrupt for Timer#1       clic_tim1_handler   ->   TIM1_IRQn */
    FirstDeviceSpecificInterrupt_IRQn    = 16,    /** First Device Specific Interrupt */
    SOC_INT_MAX,                                  /** Number of total interrupts */
} IRQn_Type;
#endif /* __ONLY_FOR_DOXYGEN_DOCUMENT_GENERATION__ */

/* ############################# CLIC functions ################################# */
/**
 * defgroup        EMSIS_Core_IntExc        Interrupts and Exceptions
 * \brief  Functions that manage interrupts and exceptions via the CLIC
 *
 * @{
 */

#define MSIP_SET_OP(op, ...) \
    do { \
        op(msip->impl[__get_hartid()], ##__VA_ARGS__); \
    } while (0)

#define MSIP_GET_OP(op, ret_type, ...) \
    do { \
        return (ret_type) op(msip->impl[__get_hartid()], ##__VA_ARGS__); \
    } while (0)

#define CLIC_SET_OP(op, ...) \
    do { \
        op(clic->impl[__get_hartid()], ##__VA_ARGS__); \
    } while (0)

#define CLIC_GET_OP(op, ret_type, ...) \
    do { \
        return (ret_type) op(clic->impl[__get_hartid()], ##__VA_ARGS__); \
    } while (0)

/**
 * \brief  Set MSIP value
 * \details
 * This function set the msip value of CLIC_MSIP register.
 */
__STATIC_FORCEINLINE void CLIC_SetMSIPBits(uint32_t data)
{
    MSIP_SET_OP(__CLIC_SetMSIPBits, data);
}

/**
 * \brief  Get MSIP value
 * \details
 * This function get the msip value of CLIC_MSIP register.
 */
__STATIC_FORCEINLINE uint32_t CLIC_GetMSIPBits(void)
{
    MSIP_GET_OP(__CLIC_GetMSIPBits, uint32_t);
}

/**
 * \brief  Set nvbits value
 * \defails
 * This function set the nvbits value of CLICCFG register.
 */
__STATIC_FORCEINLINE void CLIC_SetCfgNvbits(uint32_t nvbits)
{
    CLIC_SET_OP(__CLIC_SetCfgNvbits, nvbits);
}

/**
 * \brief  Get nvbits value
 * \defails
 * This function get the nvbits value of CLICCFG register.
 */
__STATIC_FORCEINLINE uint32_t CLIC_GetCfgNvbits(void)
{
    CLIC_GET_OP(__CLIC_GetCfgNvbits, uint32_t);
}

/**
 * \brief  Set nlbits value
 * \details
 * This function set the nlbits value of CLICCFG register.
 */
__STATIC_FORCEINLINE void CLIC_SetCfgNlbits(uint32_t nlbits)
{
    CLIC_SET_OP(__CLIC_SetCfgNlbits, nlbits);
}

/**
 * \brief  Get nlbits value
 * \details
 * This function get the nlbits value of CLICCFG register.
 */
__STATIC_FORCEINLINE uint32_t CLIC_GetCfgNlbits(void)
{
    CLIC_GET_OP(__CLIC_GetCfgNlbits, uint32_t);
}

/**
 * \brief  Get the CLIC version number
 * \details
 * This function gets the hardware version information from CLICINFO register.
*/
__STATIC_FORCEINLINE uint32_t CLIC_GetInfoVer(void)
{
    CLIC_GET_OP(__CLIC_GetInfoVer, uint32_t);
}

/**
 * \brief  Get CLICINTCTLBITS
 * \details
 * This function gets CLICINTCTLBITS from CLICINFO register.
 */
__STATIC_FORCEINLINE uint32_t CLIC_GetInfoCtlbits(void)
{
    CLIC_GET_OP(__CLIC_GetInfoCtlbits, uint32_t);
}

/**
 * \brief  Get number of maximum interrupt inputs supported
 * \details
 * This function gets number of maximum interrupt inputs supported from CLICINFO register.
 */
__STATIC_FORCEINLINE uint32_t CLIC_GetInfoNum(void)
{
    CLIC_GET_OP(__CLIC_GetInfoNum, uint32_t);
}

/**
 * \brief  Set Machine Mode Interrupt Level Threshold
 * \details
 * This function sets machine mode interrupt level threshold.
 */
__STATIC_FORCEINLINE void CLIC_SetMth(uint8_t mth)
{
    CLIC_SET_OP(__CLIC_SetMth, mth);
}

/**
 * \brief  Get Machine Mode Interrupt Level Threshold
 * \details
 * This function gets machine mode interrupt level threshold.
 */
__STATIC_FORCEINLINE uint8_t CLIC_GetMth(void)
{
    CLIC_GET_OP(__CLIC_GetMth, uint8_t);
}

/**
 * \brief  Enable a specific interrupt
 * \details
 * This function enables the specific interrupt \em IRQn.
 */
__STATIC_FORCEINLINE void CLIC_EnableIRQ(IRQn_Type IRQn)
{
    CLIC_SET_OP(__CLIC_EnableIRQ, IRQn);
}

/**
 * \brief  Get a specific interrupt enable status
 * \details
 * This function returns the interrupt enable status for the specific interrupt \em IRQn.
 */
__STATIC_FORCEINLINE uint32_t CLIC_GetEnableIRQ(IRQn_Type IRQn)
{
    CLIC_GET_OP(__CLIC_GetEnableIRQ, uint32_t, IRQn);
}

/**
 * \brief  Disable a specific interrupt
 * \details
 * This function disables the specific interrupt \em IRQn.
 */
__STATIC_FORCEINLINE void CLIC_DisableIRQ(IRQn_Type IRQn)
{
    CLIC_SET_OP(__CLIC_DisableIRQ, IRQn);
}

/**
 * \brief  Get the pending specific interrupt
 * \details
 * This function returns the pending status of the specific interrupt \em IRQn.
 */
__STATIC_FORCEINLINE uint32_t CLIC_GetPendingIRQ(IRQn_Type IRQn)
{
    CLIC_GET_OP(__CLIC_GetPendingIRQ, uint32_t, IRQn);
}

/**
 * \brief  Set a specific interrupt to pending
 * \details
 * This function sets the pending bit for the specific interrupt \em IRQn.
 */
__STATIC_FORCEINLINE void CLIC_SetPendingIRQ(IRQn_Type IRQn)
{
    CLIC_SET_OP(__CLIC_SetPendingIRQ, IRQn);
}

__STATIC_FORCEINLINE void CLIC_SetPendingIRQ_Core(uint32_t core_id, IRQn_Type IRQn)
{
    __CLIC_SetPendingIRQ(clic->impl[core_id], IRQn);
}


/**
 * \brief  Clear a specific interrupt from pending
 * \details
 * This function removes the pending state of the specific interrupt \em IRQn.
 * \em IRQn cannot be a negative number.
 */
__STATIC_FORCEINLINE void CLIC_ClearPendingIRQ(IRQn_Type IRQn)
{    
    CLIC_SET_OP(__CLIC_ClearPendingIRQ, IRQn);
}

/**
 * \brief  Set trigger mode and polarity for a specific interrupt
 * \details
 * This function set trigger mode and polarity of the specific interrupt \em IRQn.
 */
__STATIC_FORCEINLINE void CLIC_SetTrigIRQ(IRQn_Type IRQn, uint32_t trig)
{
    CLIC_SET_OP(__CLIC_SetTrigIRQ, IRQn, trig);
}

/**
 * \brief  Get trigger mode and polarity for a specific interrupt
 * \details
 * This function get trigger mode and polarity of the specific interrupt \em IRQn.
 */
__STATIC_FORCEINLINE uint32_t CLIC_GetTrigIRQ(IRQn_Type IRQn)
{
    CLIC_GET_OP(__CLIC_GetTrigIRQ, uint32_t, IRQn);
}

/**
 * \brief  Set interrupt working mode for a specific interrupt
 * \details
 * This function set selective hardware vector or non-vector working mode of the specific interrupt \em IRQn.
 */
__STATIC_FORCEINLINE void CLIC_SetShvIRQ(IRQn_Type IRQn, uint32_t shv)
{
    CLIC_SET_OP(__CLIC_SetShvIRQ, IRQn, shv);
}

/**
 * \brief  Get interrupt working mode for a specific interrupt
 * \details
 * This function get selective hardware vector or non-vector working mode of the specific interrupt \em IRQn.
 */
__STATIC_FORCEINLINE uint32_t CLIC_GetShvIRQ(IRQn_Type IRQn)
{
    CLIC_GET_OP(__CLIC_GetShvIRQ, uint32_t, IRQn);
}

/**
 * \brief  Modify CLIC Interrupt Input Control Register for a specific interrupt
 * \details
 * This function modify CLIC Interrupt Input Control(CLICINTCTL[i]) register of the specific interrupt \em IRQn.
 */
__STATIC_FORCEINLINE void CLIC_SetCtrlIRQ(IRQn_Type IRQn, uint8_t intctrl)
{
    CLIC_SET_OP(__CLIC_SetCtrlIRQ, IRQn, intctrl);
}

/**
 * \brief  Get CLIC Interrupt Input Control Register value for a specific interrupt
 * \details
 * This function modify CLIC Interrupt Input Control register of the specific interrupt \em IRQn.
 */
__STATIC_FORCEINLINE uint8_t CLIC_GetCtrlIRQ(IRQn_Type IRQn)
{
    CLIC_GET_OP(__CLIC_GetCtrlIRQ, uint8_t, IRQn);
}
/**
 * \brief  Set CLIC Interrupt level of a specific interrupt
 * \details
 * This function set interrupt level of the specific interrupt \em IRQn.
 */
__STATIC_FORCEINLINE void CLIC_SetLevelIRQ(IRQn_Type IRQn, uint8_t lvl_abs)
{
    CLIC_SET_OP(__CLIC_SetLevelIRQ, IRQn, lvl_abs);
}
/**
 * \brief  Get CLIC Interrupt level of a specific interrupt
 * \details
 * This function get interrupt level of the specific interrupt \em IRQn.
 */
__STATIC_FORCEINLINE uint8_t CLIC_GetLevelIRQ(IRQn_Type IRQn)
{
    CLIC_GET_OP(__CLIC_GetLevelIRQ, uint8_t, IRQn);
}

/**
 * \brief  Set CLIC Interrupt priority of a specific interrupt
 * \details
 * This function Set interrupt priority of the specific interrupt \em IRQn.
 */
__STATIC_FORCEINLINE void CLIC_SetPriorityIRQ(IRQn_Type IRQn, uint8_t pri)
{
    CLIC_SET_OP(__CLIC_SetPriorityIRQ, IRQn, pri);
}

/**
 * \brief  Get CLIC Interrupt priority of a specific interrupt
 * \details
 * This function get interrupt priority of the specific interrupt \em IRQn.
 */
__STATIC_FORCEINLINE uint8_t CLIC_GetPriorityIRQ(IRQn_Type IRQn)
{
    CLIC_GET_OP(__CLIC_GetPriorityIRQ, uint8_t, IRQn);
}

/**
 * \brief  Set Interrupt Vector of a specific interrupt
 * \details
 * This function set interrupt handler address of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \param [in]      vector   Interrupt handler address
 * \remarks
 * - IRQn must not be negative.
 * - You can set the \ref CSR_CSR_MTVT to set interrupt vector table entry address.
 * - If your vector table is placed in readonly section, the vector for IRQn will not be modified.
 *   For this case, you need to use the correct irq handler name defined in your vector table as
 *   your irq handler function name.
 * - This function will only work correctly when the vector table is placed in an read-write enabled section.
 * \sa
 * - \ref CLIC_GetVector
 */
__STATIC_FORCEINLINE void CLIC_SetVector(IRQn_Type IRQn, rv_csr_t vector)
{
#if __RISCV_XLEN == 32
    volatile uint32_t vec_base;
    vec_base = ((uint32_t)__RV_CSR_READ(CSR_MTVT));
    (* (unsigned long *) (vec_base + ((int32_t)IRQn) * 4)) = vector;
#elif __RISCV_XLEN == 64
    volatile uint64_t vec_base;
    vec_base = ((uint64_t)__RV_CSR_READ(CSR_MTVT));
    (* (unsigned long *) (vec_base + ((int32_t)IRQn) * 8)) = vector;
#else // TODO Need cover for XLEN=128 case in future
    volatile uint64_t vec_base;
    vec_base = ((uint64_t)__RV_CSR_READ(CSR_MTVT));
    (* (unsigned long *) (vec_base + ((int32_t)IRQn) * 8)) = vector;
#endif
}

/**
 * \brief  Get Interrupt Vector of a specific interrupt
 * \details
 * This function get interrupt handler address of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \return        Interrupt handler address
 * \remarks
 * - IRQn must not be negative.
 * - You can read \ref CSR_CSR_MTVT to get interrupt vector table entry address.
 * \sa
 *     - \ref CLIC_SetVector
 */
__STATIC_FORCEINLINE rv_csr_t CLIC_GetVector(IRQn_Type IRQn)
{
#if __RISCV_XLEN == 32
    return (*(uint32_t *)(__RV_CSR_READ(CSR_MTVT)+IRQn*4));
#elif __RISCV_XLEN == 64
    return (*(uint64_t *)(__RV_CSR_READ(CSR_MTVT)+IRQn*8));
#else // TODO Need cover for XLEN=128 case in future
    return (*(uint64_t *)(__RV_CSR_READ(CSR_MTVT)+IRQn*8));
#endif
}

/**
 * \brief   Save necessary CSRs into variables for vector interrupt nesting
 * \details
 * This macro is used to declare variables which are used for saving
 * CSRs(MCAUSE, MEPC, MSTATUS), and it will read these CSR content into
 * these variables, it need to be used in a vector-interrupt if nesting is required.
 * \remarks
 * - Interrupt need be enabled manually after this macro is called
 * - It need to be used together with \ref RESTORE_IRQ_CSR_CONTEXT
 * - Don't use variable names __mcause, __mpec, __mstatus in your ISR code
 * - If you want to enable interrupt nesting feature for vector interrupt,
 * you can do it like this:
 * \code
 * // __INTERRUPT attribute will generates function entry and exit sequences suitable
 * // for use in an interrupt handler when this attribute is present
 * __INTERRUPT void irq_mtip_handler(void)
 * {
 *     // Must call this to save CSRs
 *     SAVE_IRQ_CSR_CONTEXT();
 *
 *       // Clear machine timer interrupt and then enable interrupt
 *       SysTimer_SetCompareValue(SysTimer_GetCompareValue() + SYSTEM_CLOCK);
 *       __enable_irq();
 *     // !!!Higher priority interrupt could nest it!!!
 *
 *     // put you own interrupt handling code here
 *
 *     // Must call this to restore CSRs
 *     RESTORE_IRQ_CSR_CONTEXT();
 * }
 * \endcode
 */
#define SAVE_IRQ_CSR_CONTEXT()                                              \
        rv_csr_t __mcause = __RV_CSR_READ(CSR_MCAUSE);                      \
        rv_csr_t __mepc = __RV_CSR_READ(CSR_MEPC);                            \
        rv_csr_t __mstatus = __RV_CSR_READ(CSR_MSTATUS);

/**
 * \brief   Restore necessary CSRs from variables for vector interrupt nesting
 * \details
 * This macro is used restore CSRs(MCAUSE, MEPC, MSTATUS.MPP) from pre-defined variables
 * in \ref SAVE_IRQ_CSR_CONTEXT macro.
 * \remarks
 * - Interrupt will be disabled manually before this macro is called
 * - It need to be used together with \ref SAVE_IRQ_CSR_CONTEXT
 */
#define RESTORE_IRQ_CSR_CONTEXT()                                           \
        __RV_CSR_READ_CLEAR(CSR_MSTATUS, MSTATUS_MPP);                      \
        __RV_CSR_READ_SET(CSR_MSTATUS, __mstatus & MSTATUS_MPP);            \
        __RV_CSR_WRITE(CSR_MEPC, __mepc);                                   \
        __RV_CSR_WRITE(CSR_MCAUSE, __mcause);

/**
 * @}
 */ /* End of Doxygen Group EMSIS_Core_IntExc */

#endif /* CONFIG_RV_CLIC_PRESENT */

#ifdef __cplusplus
}
#endif
#endif /* __CORE_FEATURE_CLIC_H__ */
