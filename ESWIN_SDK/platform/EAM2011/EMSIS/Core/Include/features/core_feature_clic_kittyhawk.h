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

#ifndef __CORE_FEATURE_CLIC_KITTYHAWK_H__
#define __CORE_FEATURE_CLIC_KITTYHAWK_H__
/**
 * @file     core_feature_clic_kittyhawk.h
 * @brief    CLIC feature API header file for Eswin Core Kittyhawk
 */

#include "EAM2011.h"
#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

/**
 * \defgroup EMSIS_Core_CLIC_Registers     Register Define and Type Definitions Of CLIC
 * \ingroup EMSIS_Core_Registers
 * \brief   Type definitions and defines for clic registers.
 *
 * @{
 */

/**
 * \brief  Union type to access CLICFG configure register.
 */
typedef union
{
    struct {
        uint8_t nvbits:1;                       /** bit: 0  when set, selective hardware vectoring is enabled */
        uint8_t nlbits:3;                       /** bit: 1..3  determines the number of level bits available in clicintcfg */
        uint8_t _reserved0:1;                   /** bit: 4  Reserved */
        uint8_t nmbits:2;                       /** bit: 5..6  determines the number of mode bits available in clicintcfg */
        uint8_t _reserved1:1;                   /** bit: 7  Reserved */
    } b;                                        /** Structure used for bit  access */
    uint8_t w;                                  /** Type      used for byte access */
} CLICCFG_Type;

/**
 * \brief  Union type to access CLICINTCFG configure register
 */
typedef union {
    struct {
        uint8_t shv:1;                          /** bit: 0  padding of clicintcfg */
        uint8_t level:7;                        /** bit: 1..7  sets the premption level and priority of a given interrupt */
    } b;                                        /** Structure  used for bit access */
    uint8_t w;                                  /** Type       used for byte access */
} CLICINTCFG_Type;

/**
 * \brief  Union type to access CLICINTIP status register
 */
typedef union {
    struct {
        uint8_t clicintip:1;                    /** bit: 0  indicate the corresponding interrupt id is pending */
        uint8_t _reserved0:7;                   /** bit: 1..7 Reserved */
    } b;                                        /** Structure  used for bit access */
    uint8_t w;                                  /** Type       used for byte access */
} CLICINTIP_Type;

/**
 * \brief  Union type to access CLICINTIE status register
 */
typedef union {
    struct {
        uint8_t clicintie:1;                    /** bit: 0  indicate the corresponding interrupt id is enabled */
        uint8_t _reserved0:7;                   /** bit: 1..7 Reserved */
    } b;                                        /** Structure  used for bit access */
    uint8_t w;                                  /** Type       used for byte access */
} CLICINTIE_Type;

#define CLIC_CLICCFG_NVBIT_Pos                 0U                                       /** CLIC CLICCFG: NVBIT Position */
#define CLIC_CLICCFG_NVBIT_Msk                 (0x1UL << CLIC_CLICCFG_NVBIT_Pos)        /** CLIC CLICCFG: NVBIT Mask */
#define CLIC_CLICCFG_NLBIT_Pos                 1U                                       /** CLIC CLICCFG: NLBIT Position */
#define CLIC_CLICCFG_NLBIT_Msk                 (0x7UL << CLIC_CLICCFG_NLBIT_Pos)        /** CLIC CLICCFG: NLBIT Mask */
#define CLIC_CLICCFG_NMBIT_Pos                 5U                                       /** CLIC CLICCFG: NMBIT Position */
#define CLIC_CLICCFG_NMBIT_Msk                 (0x3UL << CLIC_CLICCFG_NMBIT_Pos)        /** CLIC CLICCFG: NMBIT Mask */

#define CLIC_CLICINTCFG_SHV_Pos                0U                                       /** CLIC_CLICINTCFG: SHV Position */
#define CLIC_CLICINTCFG_SHV_Msk                (0x1UL << CLIC_CLICINTCFG_SHV_Pos)       /** CLIC_CLICINTCFG: SHV Mask */
#define CLIC_CLICINTCFG_CTL_Pos                1U                                       /** CLIC_CLICINTCFG: Control Position */
#define CLIC_CLICINTCFG_CTL_Msk                (0x7FUL << CLIC_CLICINTCFG_CTL_Pos)      /** CLIC_CLICINTCFG: Control Mask */

#define CLIC_INTIP_IP_Pos                      0U                                       /** CLIC INTIP: IP Position */
#define CLIC_INTIP_IP_Msk                      (0x1UL << CLIC_INTIP_IP_Pos)             /** CLIC INTIP: IP Mask */

#define CLIC_INTIE_IE_Pos                      0U                                       /** CLIC INTIE: IE Position */
#define CLIC_INTIE_IE_Msk                      (0x1UL << CLIC_INTIE_IE_Pos)             /** CLIC INTIE: IE Mask */

#define CLIC_MAX_NLBITS                        7U                                       /** Max nlbit of the CLICINTCTLBITS */

#define CLIC_DIRECT_INTERRUPT                  0x0                                      /** CLIC Direct Interrupt Mode */
#define CLIC_VECTORED_INTERRUPT                0x1                                      /** CLIC Vectored Interrupt Mode */

#define CLIC_MSIP_MSIP_Pos                     0U                                       /** CLIC MSIP: MSIP Position */
#define CLIC_MSIP_MSIP_Msk                     (0x1UL << CLIC_MSIP_MSIP_Pos)            /** CLIC MSIP: MSIP Mask */

/**\brief CLIC Trigger Enum for different Trigger Type */
typedef enum CLIC_TRIGGER {
    CLIC_LEVEL_TRIGGER = 0x0,          /** Level Triggered, trig[0] = 0 */
    CLIC_POSITIVE_EDGE_TRIGGER = 0x1,   /** Positive/Rising Edge Triggered, trig[1] = 1, trig[0] = 0 */
    CLIC_NEGATIVE_EDGE_TRIGGER = 0x3,   /** Negative/Falling Edge Triggered, trig[1] = 1, trig[0] = 0 */
    CLIC_MAX_TRIGGER = 0x3             /** MAX Supported Trigger Mode */
} CLIC_TRIGGER_Type;

/**
 * \brief Access to the structure of a vector interrupt controller
 */
typedef struct {
    __IOM uint8_t cicintip[512];                 /** Offset: 0x80_0000 - 0x80_01FF (R/W) interrupt pending status for cic interrupt */
    uint8_t _reserved0[512];                     /** Offset: 0x80_0200 - 0x80_03FF Reserved */
    __IOM uint8_t cicintie[512];                 /** Offset: 0x80_0400 - 0x80_05FF (R/W) interrupt enable control register for cic interrupt */
    uint8_t _reserved9[512];                     /** Offset: 0x80_0600 - 0x80_07FF Reserved */
    __IOM uint8_t cicintcfg[512];                /** Offset: 0x80_0800 - 0x80_09FF (R/W) interrupt config register for cic interrupt */
    uint8_t _reserved14[512];                    /** Offset: 0x80_0A00 - 0x80_0BFF Reserved */
    __IOM uint8_t ciccfg;                        /** Offset: 0x80_0C00 (R/W) interrupt config for cic */
} CLIC_Impl;


/**
 * \brief Access to the structure of software interrupt trigger
 */
typedef struct {
    __IOM uint8_t msip;                          /** Offset: 0x00_0000 (R/W) software interrupt trigger register */
} MSIP_Impl;

/**
 * @}
 */ /* end of group EMSIS_Core_CLIC_Registers */

/* ##########################   CLIC functions  #################################### */
/**
 * \defgroup   EMSIS_Core_IntExc        Interrupts and Exceptions
 * \brief Functions Implementation that manage interrupts and exceptions via the CLIC.
 *
 * @{
 */

/**
 * \brief  Set MSIP value
 * \details
 * This function set the msip value of CLIC_MSIP register.
 * \param [in]    msip      msip value
 * \remarks
 * - msip is used to trigger software interrupt in clint mode
 */
__STATIC_FORCEINLINE void __CLIC_SetMSIPBits(MSIP_Impl *impl, uint32_t msip)
{
    impl->msip &= ~CLIC_MSIP_MSIP_Msk;
    impl->msip |= (uint8_t)((msip << CLIC_MSIP_MSIP_Pos) & CLIC_MSIP_MSIP_Msk);
}

/**
 * \brief  Get MSIP value
 * \details
 * This function get the msip value of CLIC_MSIP register.
 * \param [in]    msip      msip value
 * \remarks
 * - msip is used to trigger software interrupt in clint mode
 */
__STATIC_FORCEINLINE uint32_t __CLIC_GetMSIPBits(MSIP_Impl *impl)
{
    return ((uint32_t)((impl->msip & CLIC_MSIP_MSIP_Msk) >> CLIC_MSIP_MSIP_Pos));
}

/**
 * \brief  Set nvbits value
 * \details
 * This function set the nvbits value of CLICCFG register.
 * \param [in]    nvbits    nvbits value
 * \remarks
 * - nvbits is used to enable the hardware vectoring
 */
__STATIC_FORCEINLINE void __CLIC_SetCfgNvbits(CLIC_Impl *impl, uint32_t nvbits)
{
    impl->ciccfg &= ~CLIC_CLICCFG_NVBIT_Msk;
    impl->ciccfg |= (uint8_t)((nvbits << CLIC_CLICCFG_NVBIT_Pos) & CLIC_CLICCFG_NVBIT_Msk);
}

/**
 * \brief  Get nvbits value
 * \details
 * This function get the nvbits value of CLICCFG register.
 * \param [in]    nvbits    nvbits value
 * \remarks
 * - nvbits is used to enable the hardware vectoring
 */
__STATIC_FORCEINLINE uint32_t __CLIC_GetCfgNvbits(CLIC_Impl *impl)
{
    return ((uint32_t)((impl->ciccfg & CLIC_CLICCFG_NVBIT_Msk) >> CLIC_CLICCFG_NVBIT_Pos));
}


/**
 * \brief  Set nlbits value
 * \details
 * This function set the nlbits value of CLICCFG register.
 * \param [in]    nlbits    nlbits value
 * \remarks
 * - nlbits is used to set the width of level in the CLICINTCTL[i].
 * \sa
 * - \ref CLIC_GetCfgNlbits
 */
__STATIC_FORCEINLINE void __CLIC_SetCfgNlbits(CLIC_Impl *impl, uint32_t nlbits)
{
    impl->ciccfg &= ~CLIC_CLICCFG_NLBIT_Msk;
    impl->ciccfg |= (uint8_t)((nlbits <<CLIC_CLICCFG_NLBIT_Pos) & CLIC_CLICCFG_NLBIT_Msk);
}

/**
 * \brief  Get nlbits value
 * \details
 * This function get the nlbits value of CLICCFG register.
 * \return   nlbits value of CLICCFG register
 * \remarks
 * - nlbits is used to set the width of level in the CLICINTCTL[i].
 * \sa
 * - \ref CLIC_SetCfgNlbits
 */
__STATIC_FORCEINLINE uint32_t __CLIC_GetCfgNlbits(CLIC_Impl *impl)
{
    return ((uint32_t)((impl->ciccfg & CLIC_CLICCFG_NLBIT_Msk) >> CLIC_CLICCFG_NLBIT_Pos));
}

/**
 * \brief  Get the CLIC version number
 * \details
 * This function gets the hardware version information from CLICINFO register.
 * \return   hardware version number in CLICINFO register.
 * \remarks
 * - This function gets hardware version information from CLICINFO register.
 * - Bit 20:17 for architecture version, bit 16:13 for implementation version.
 * \sa
 * - \ref CLIC_GetInfoNum
*/
__STATIC_FORCEINLINE uint32_t __CLIC_GetInfoVer(CLIC_Impl *impl)
{
    (void)impl;
    /* Kittyhawk doesn't implement CLICINFO register, return 0 as default */
    return 0;
}

/**
 * \brief  Get CLICINTCTLBITS
 * \details
 * This function gets CLICINTCTLBITS from CLICINFO register.
 * \return  CLICINTCTLBITS from CLICINFO register.
 * \remarks
 * - In the CLICINTCTL[i] registers, with 2 <= CLICINTCTLBITS <= 8.
 * - The implemented bits are kept left-justified in the most-significant bits of each 8-bit
 *   CLICINTCTL[I] register, with the lower unimplemented bits treated as hardwired to 1.
 * \sa
 * - \ref CLIC_GetInfoNum
 */
__STATIC_FORCEINLINE uint32_t __CLIC_GetInfoCtlbits(CLIC_Impl *impl)
{
    (void)impl;
    /* Kittyhawk doesn't implement CLICINFO register, return 7 as default */
    return 7;
}

/**
 * \brief  Get number of maximum interrupt inputs supported
 * \details
 * This function gets number of maximum interrupt inputs supported from CLICINFO register.
 * \return  number of maximum interrupt inputs supported from CLICINFO register.
 * \remarks
 * - This function gets number of maximum interrupt inputs supported from CLICINFO register.
 * - The num_interrupt field specifies the actual number of maximum interrupt inputs supported in this implementation.
 * \sa
 * - \ref CLIC_GetInfoCtlbits
 */
__STATIC_FORCEINLINE uint32_t __CLIC_GetInfoNum(CLIC_Impl *impl)
{
    (void)impl;
    /* Kittyhawk doesn't implement CLICINFO register, return 127 as default */
    return 128;
}

/**
 * \brief  Set Machine Mode Interrupt Level Threshold
 * \details
 * This function sets machine mode interrupt level threshold.
 * \param [in]  mth       Interrupt Level Threshold.
 * \sa
 * - \ref CLIC_GetMth
 */
__STATIC_FORCEINLINE void __CLIC_SetMth(CLIC_Impl *impl, uint8_t mth)
{
    (void) impl;
    (void) mth;
    /* not implement xintthresh register */
}

/**
 * \brief  Get Machine Mode Interrupt Level Threshold
 * \details
 * This function gets machine mode interrupt level threshold.
 * \return       Interrupt Level Threshold.
 * \sa
 * - \ref CLIC_SetMth
 */
__STATIC_FORCEINLINE uint8_t __CLIC_GetMth(CLIC_Impl *impl)
{
    (void)impl;
    /* not implement xintthresh register */
    return 0;
}


/**
 * \brief  Enable a specific interrupt
 * \details
 * This function enables the specific interrupt \em IRQn.
 * \param [in]  IRQn  Interrupt number
 * \remarks
 * - IRQn must not be negative.
 * \sa
 * - \ref CLIC_DisableIRQ
 */
__STATIC_FORCEINLINE void __CLIC_EnableIRQ(CLIC_Impl *impl, IRQn_Type IRQn)
{
    impl->cicintie[IRQn] |= (uint8_t)CLIC_INTIE_IE_Msk;
}

/**
 * \brief  Get a specific interrupt enable status
 * \details
 * This function returns the interrupt enable status for the specific interrupt \em IRQn.
 * \param [in]  IRQn  Interrupt number
 * \returns
 * - 0  Interrupt is not enabled
 * - 1  Interrupt is pending
 * \remarks
 * - IRQn must not be negative.
 * \sa
 * - \ref CLIC_EnableIRQ
 * - \ref CLIC_DisableIRQ
 */
__STATIC_FORCEINLINE uint32_t __CLIC_GetEnableIRQ(CLIC_Impl *impl, IRQn_Type IRQn)
{
    return (uint32_t)(impl->cicintie[IRQn] & CLIC_INTIE_IE_Msk);
}

/**
 * \brief  Disable a specific interrupt
 * \details
 * This function disables the specific interrupt \em IRQn.
 * \param [in]  IRQn  Number of the external interrupt to disable
 * \remarks
 * - IRQn must not be negative.
 * \sa
 * - \ref CLIC_EnableIRQ
 */
__STATIC_FORCEINLINE void __CLIC_DisableIRQ(CLIC_Impl *impl, IRQn_Type IRQn)
{
    impl->cicintie[IRQn] &= ~CLIC_INTIE_IE_Msk;
}

/**
 * \brief  Get the pending specific interrupt
 * \details
 * This function returns the pending status of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \returns
 * - 0  Interrupt is not pending
 * - 1  Interrupt is pending
 * \remarks
 * - IRQn must not be negative.
 * \sa
 * - \ref CLIC_SetPendingIRQ
 * - \ref CLIC_ClearPendingIRQ
 */
__STATIC_FORCEINLINE uint32_t __CLIC_GetPendingIRQ(CLIC_Impl *impl, IRQn_Type IRQn)
{
    return (uint32_t)(impl->cicintip[IRQn] &= CLIC_INTIP_IP_Msk);
}

/**
 * \brief  Set a specific interrupt to pending
 * \details
 * This function sets the pending bit for the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \remarks
 * - IRQn must not be negative.
 * \sa
 * - \ref CLIC_GetPendingIRQ
 * - \ref CLIC_ClearPendingIRQ
 */
__STATIC_FORCEINLINE void __CLIC_SetPendingIRQ(CLIC_Impl *impl, IRQn_Type IRQn)
{
       impl->cicintip[IRQn] |= (uint8_t)CLIC_INTIP_IP_Msk;
}

/**
 * \brief  Clear a specific interrupt from pending
 * \details
 * This function removes the pending state of the specific interrupt \em IRQn.
 * \em IRQn cannot be a negative number.
 * \param [in]      IRQn  Interrupt number
 * \remarks
 * - IRQn must not be negative.
 * \sa
 * - \ref CLIC_SetPendingIRQ
 * - \ref CLIC_GetPendingIRQ
 */
__STATIC_FORCEINLINE void __CLIC_ClearPendingIRQ(CLIC_Impl *impl, IRQn_Type IRQn)
{    
    impl->cicintip[IRQn] &= ~CLIC_INTIP_IP_Msk;
}

/**
 * \brief  Set trigger mode and polarity for a specific interrupt
 * \details
 * This function set trigger mode and polarity of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \param [in]      trig
 *                   - 00  level trigger, \ref CLIC_LEVEL_TRIGGER
 *                   - 01  positive edge trigger, \ref CLIC_POSITIVE_EDGE_TRIGGER
 *                   - 02  level trigger, \ref CLIC_LEVEL_TRIGGER
 *                   - 03  negative edge trigger, \ref CLIC_NEGATIVE_EDGE_TRIGGER
 * \remarks
 * - IRQn must not be negative.
 *
 * \sa
 * - \ref CLIC_GetTrigIRQ
 */
__STATIC_FORCEINLINE void __CLIC_SetTrigIRQ(CLIC_Impl *impl, IRQn_Type IRQn, uint32_t trig)
{
    (void)impl;
    (void)IRQn;
    (void)trig;
    /* Kittyhawk only support level trigger */
}

/**
 * \brief  Get trigger mode and polarity for a specific interrupt
 * \details
 * This function get trigger mode and polarity of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \return
 *                 - 00  level trigger, \ref CLIC_LEVEL_TRIGGER
 *                 - 01  positive edge trigger, \ref CLIC_POSITIVE_EDGE_TRIGGER
 *                 - 02  level trigger, \ref CLIC_LEVEL_TRIGGER
 *                 - 03  negative edge trigger, \ref CLIC_NEGATIVE_EDGE_TRIGGER
 * \remarks
 *     - IRQn must not be negative.
 * \sa
 *     - \ref CLIC_SetTrigIRQ
 */
__STATIC_FORCEINLINE uint32_t __CLIC_GetTrigIRQ(CLIC_Impl *impl, IRQn_Type IRQn)
{
    (void)impl;
    (void)IRQn;
    /* Kittyhawk only support level trigger */
    return CLIC_LEVEL_TRIGGER;
}

/**
 * \brief  Set interrupt working mode for a specific interrupt
 * \details
 * This function set selective hardware vector or non-vector working mode of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \param [in]      shv
 *                        - 0  non-vector mode, \ref CLIC_NON_VECTOR_INTERRUPT
 *                        - 1  vector mode, \ref CLIC_VECTOR_INTERRUPT
 * \remarks
 * - IRQn must not be negative.
 * \sa
 * - \ref CLIC_GetShvIRQ
 */
__STATIC_FORCEINLINE void __CLIC_SetShvIRQ(CLIC_Impl *impl, IRQn_Type IRQn, uint32_t shv)
{
    impl->cicintcfg[IRQn] &= ~CLIC_CLICINTCFG_SHV_Msk;
    impl->cicintcfg[IRQn] |= (uint8_t)(shv << CLIC_CLICINTCFG_SHV_Pos);
}

/**
 * \brief  Get interrupt working mode for a specific interrupt
 * \details
 * This function get selective hardware vector or non-vector working mode of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \return       shv
 *                        - 0  non-vector mode, \ref CLIC_NON_VECTOR_INTERRUPT
 *                        - 1  vector mode, \ref CLIC_VECTOR_INTERRUPT
 * \remarks
 * - IRQn must not be negative.
 * \sa
 * - \ref CLIC_SetShvIRQ
 */
__STATIC_FORCEINLINE uint32_t __CLIC_GetShvIRQ(CLIC_Impl *impl, IRQn_Type IRQn)
{
    return (uint32_t)((impl->cicintcfg[IRQn] & CLIC_CLICINTCFG_SHV_Msk) >> CLIC_CLICINTCFG_SHV_Pos);
}

/**
 * \brief  Modify CLIC Interrupt Input Control Register for a specific interrupt
 * \details
 * This function modify CLIC Interrupt Input Control(CLICINTCTL[i]) register of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \param [in]      intctrl  Set value for CLICINTCTL[i] register
 * \remarks
 * - IRQn must not be negative.
 * \sa
 * - \ref CLIC_GetCtrlIRQ
 */
__STATIC_FORCEINLINE void __CLIC_SetCtrlIRQ(CLIC_Impl *impl, IRQn_Type IRQn, uint8_t intctrl)
{
    impl->cicintcfg[IRQn] &= ~CLIC_CLICINTCFG_CTL_Msk;
    impl->cicintcfg[IRQn] |= (uint8_t)((intctrl << CLIC_CLICINTCFG_CTL_Pos) & CLIC_CLICINTCFG_CTL_Msk);
}

/**
 * \brief  Get CLIC Interrupt Input Control Register value for a specific interrupt
 * \details
 * This function modify CLIC Interrupt Input Control register of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \return       value of CLIC Interrupt Input Control register
 * \remarks
 * - IRQn must not be negative.
 * \sa
 * - \ref CLIC_SetCtrlIRQ
 */
__STATIC_FORCEINLINE uint8_t __CLIC_GetCtrlIRQ(CLIC_Impl *impl, IRQn_Type IRQn)
{
    return (uint8_t)((impl->cicintcfg[IRQn] & CLIC_CLICINTCFG_CTL_Msk) >> CLIC_CLICINTCFG_CTL_Pos);
}

/**
 * \brief  Set CLIC Interrupt level of a specific interrupt
 * \details
 * This function set interrupt level of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \param [in]      lvl_abs   Interrupt level
 * \remarks
 * - IRQn must not be negative.
 * - If lvl_abs to be set is larger than the max level allowed, it will be force to be max level.
 * - When you set level value you need use clciinfo.nlbits to get the width of level.
 *   Then we could know the maximum of level. CLICINTCTLBITS is how many total bits are
 *   present in the CLICINTCTL register.
 * \sa
 * - \ref CLIC_GetLevelIRQ
 */
__STATIC_FORCEINLINE void __CLIC_SetLevelIRQ(CLIC_Impl *impl, IRQn_Type IRQn, uint8_t lvl_abs)
{
    uint8_t nlbits = __CLIC_GetCfgNlbits(impl);
    uint8_t intctlbits = (uint8_t)__CLIC_GetInfoCtlbits(impl);
    uint8_t ctrVal = __CLIC_GetCtrlIRQ(impl, IRQn);
    uint8_t maxlvl = 0;

    if (nlbits == 0) {
        return;
    }

    if (nlbits > intctlbits) {
        nlbits = intctlbits;
    }

    maxlvl = ((1 << nlbits) - 1);
    if (lvl_abs > maxlvl) {
        lvl_abs = maxlvl;
    }

    ctrVal &= ~(maxlvl << (CLIC_MAX_NLBITS - nlbits));
    ctrVal |= (lvl_abs << (CLIC_MAX_NLBITS - nlbits));
    __CLIC_SetCtrlIRQ(impl, IRQn, ctrVal);
}

/**
 * \brief  Get CLIC Interrupt level of a specific interrupt
 * \details
 * This function get interrupt level of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \return         Interrupt level
 * \remarks
 * - IRQn must not be negative.
 * \sa
 * - \ref CLIC_SetLevelIRQ
 */
__STATIC_FORCEINLINE uint8_t __CLIC_GetLevelIRQ(CLIC_Impl *impl, IRQn_Type IRQn)
{
    uint8_t nlbits = __CLIC_GetCfgNlbits(impl);
    uint8_t intctlbits = (uint8_t)__CLIC_GetInfoCtlbits(impl);
    uint8_t ctrVal = __CLIC_GetCtrlIRQ(impl, IRQn);
    uint8_t lvl = 0, lvlMsk = 0;

    if (nlbits == 0) {
        return 0;
    }

    if (nlbits > intctlbits) {
        nlbits = intctlbits;
    }

    lvlMsk = ((1 << nlbits) - 1);
    lvl = ((ctrVal >> (CLIC_MAX_NLBITS - nlbits)) & lvlMsk);
    return lvl;
}

/**
 * \brief  Get CLIC Interrupt priority of a specific interrupt
 * \details
 * This function get interrupt priority of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \param [in]      pri   Interrupt priority
 * \remarks
 * - IRQn must not be negative.
 * - If pri to be set is larger than the max priority allowed, it will be force to be max priority.
 * - Priority width is CLICINTCTLBITS minus clciinfo.nlbits if clciinfo.nlbits
 *   is less than CLICINTCTLBITS. Otherwise priority width is 0.
 * \sa
 * - \ref CLIC_GetPriorityIRQ
 */
__STATIC_FORCEINLINE void __CLIC_SetPriorityIRQ(CLIC_Impl *impl, IRQn_Type IRQn, uint8_t pri)
{
    uint8_t nlbits = __CLIC_GetCfgNlbits(impl);
    uint8_t intctlbits = (uint8_t)__CLIC_GetInfoCtlbits(impl);
    uint8_t ctrVal = __CLIC_GetCtrlIRQ(impl, IRQn);
    uint8_t npbits = 0, maxPrio = 0;

    if (nlbits >= intctlbits) {
        npbits = 0;
    } else {
        npbits = intctlbits - nlbits;
    }

    if (npbits == 0) {
        return;
    }

    maxPrio =((1 << npbits) - 1);
    if (pri > maxPrio) {
        pri = maxPrio;
    }

    ctrVal &= ~maxPrio;
    ctrVal |= pri;
    __CLIC_SetCtrlIRQ(impl, IRQn, ctrVal);
}

/**
 * \brief  Get CLIC Interrupt priority of a specific interrupt
 * \details
 * This function get interrupt priority of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \return   Interrupt priority
 * \remarks
 * - IRQn must not be negative.
 * \sa
 * - \ref CLIC_SetPriorityIRQ
 */
__STATIC_FORCEINLINE uint8_t __CLIC_GetPriorityIRQ(CLIC_Impl *impl, IRQn_Type IRQn)
{
    uint8_t nlbits = __CLIC_GetCfgNlbits(impl);
    uint8_t intctlbits = (uint8_t)__CLIC_GetInfoCtlbits(impl);
    uint8_t ctrVal = __CLIC_GetCtrlIRQ(impl, IRQn);
    uint8_t npbits = 0, lvl = 0, lvlMsk = 0;

    if (nlbits >= intctlbits) {
        npbits = 0;
    } else {
        npbits = intctlbits - nlbits;
    }

    if (npbits == 0) {
        return 0;
    }

    lvlMsk = ((1 << npbits) - 1);
    lvl = ctrVal & lvlMsk;
    return lvl;
}
/**
 * @}
 */ /* End of Doxygen Group EMSIS_Core_IntExc */

#ifdef __cplusplus
}
#endif
#endif /** __CORE_FEATURE_CLIC_KITTYHAWK_H__ */
