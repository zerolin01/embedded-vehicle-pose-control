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

#ifndef __CORE_FEATURE_TIMER_KITTYHAWK_H__
#define __CORE_FEATURE_TIMER_KITTYHAWK_H__
/**
 * @file     core_feature_timer_kittyhawk.h
 * @brief    System Timer feature API header file for Eswin Kittyhawk core
 */

#ifdef __cplusplus
 extern "C" {
#endif

/**
 * \defgroup EMSIS_Core_SysTimer_Registers     Register Define and Type Definitions Of System Timer
 * \ingroup EMSIS_Core_Registers
 * \brief   Type definitions and defines for system timer registers.
 *
 * @{
 */
/**
 * \brief  Structure type to access the System Timer (SysTimer).
 * \details
 * Structure definition to access the system timer(SysTimer).
 */
typedef struct {
    __IOM uint64_t MTIMERCMP;              /** Offset: 0x4000 - 0x4007 (R/W)  System Timer compare Value 64bits Register */
    uint8_t RESERVED1[0x7FF0];             /** Offset: 0x4008 - 0xBFF7 Reserved */
    __IOM uint64_t MTIMER;                 /** Offset: 0xBFF8 - 0xBFFF (R/W)  System Timer current value 64bits Register */
} SysTimer_Impl;

/* Timer Control / Status Register Definitions */
#define SysTimer_MSIP_MSIP_Pos              0U                                          /** SysTick Timer MSIP: MSIP bit Position */
#define SysTimer_MSIP_MSIP_Msk              (1UL << SysTimer_MSIP_MSIP_Pos)             /** SysTick Timer MSIP: MSIP Mask */

#define SysTimer_MTIMER_Msk                 (0xFFFFFFFFFFFFFFFFULL)                     /** SysTick Timer MTIMER value Mask */
#define SysTimer_MTIMERCMP_Msk              (0xFFFFFFFFFFFFFFFFULL)                     /** SysTick Timer MTIMERCMP value Mask */

/**
 * @}
 */ /* end of group EMSIS_Core_SysTimer_Registers */

/* ##################################    SysTimer function  ############################################ */
/**
 * \defgroup EMSIS_Core_SysTimer SysTimer Functions
 * \brief    Functions that configure the Core System Timer.
 * @{
 */
/**
 * \brief  Set system timer load value
 * \details
 * This function set the system timer load value in MTIMER register.
 * \param [in]  value   value to set system timer MTIMER register.
 * \remarks
 * - Load value is 64bits wide.
 * - \ref SysTimer_GetLoadValue
 */
__STATIC_FORCEINLINE void __SysTimer_SetLoadValue(SysTimer_Impl *impl, uint64_t value)
{
    impl->MTIMER = value;
}

/**
 * \brief  Get system timer load value
 * \details
 * This function get the system timer current value in MTIMER register.
 * \return  current value(64bit) of system timer MTIMER register.
 * \remarks
 * - Load value is 64bits wide.
 * - \ref SysTimer_SetLoadValue
 */
__STATIC_FORCEINLINE uint64_t __SysTimer_GetLoadValue(SysTimer_Impl *impl)
{
    return impl->MTIMER;
}

/**
 * \brief  Set system timer compare value
 * \details
 * This function set the system Timer compare value in MTIMERCMP register.
 * \param [in]  value   compare value to set system timer MTIMERCMP register.
 * \remarks
 * - Compare value is 64bits wide.
 * - If compare value is larger than current value timer interrupt generate.
 * - Modify the load value or compare value less to clear the interrupt.
 * - \ref SysTimer_GetCompareValue
 */
__STATIC_FORCEINLINE void __SysTimer_SetCompareValue(SysTimer_Impl *impl, uint64_t value)
{
    impl->MTIMERCMP = value;
}

/**
 * \brief  Get system timer compare value
 * \details
 * This function get the system timer compare value in MTIMERCMP register.
 * \return  compare value of system timer MTIMERCMP register.
 * \remarks
 * - Compare value is 64bits wide.
 * - \ref SysTimer_SetCompareValue
 */
__STATIC_FORCEINLINE uint64_t __SysTimer_GetCompareValue(SysTimer_Impl *impl)
{
    return impl->MTIMERCMP;
}

/**
 * @}
 */ /* End of Doxygen Group EMSIS_Core_SysTimer */

#ifdef __cplusplus
}
#endif
#endif /** __CORE_FEATURE_TIMER_KITTYHAWK_H__  */

