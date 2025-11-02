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

#ifndef __CORE_FEATURE_TIMER_H__
#define __CORE_FEATURE_TIMER_H__
/**
 * @file     core_feature_timer.h
 * @brief    System Timer feature API header file for Eswin Core
 */

#ifdef __cplusplus
 extern "C" {
#endif

#ifdef CONFIG_RV_SYSTIMER_PRESENT

#define SYSTIMER_NUM	CONFIG_RV_SYSTIMER_NUM 

#ifdef CONFIG_RV_SYSTIMER_KITTYHAWK
#include "features/core_feature_timer_kittyhawk.h"
#else
#error "Non-supported System Timer"
#endif

/* System Timer Memory mapping of Device  */
#define SYSTIMER_BASE(x)                    (SysTimer_Impl *)CONFIG_RV_SYSTIMER_BASEADDR_##x    /** SysTick Base Address */

typedef struct {
    SysTimer_Impl *impl[SYSTIMER_NUM];
} SysTimer_Type;

extern SysTimer_Type *systimer;

/* ##################################    SysTimer function  ############################################ */
/**
 * \defgroup EMSIS_Core_SysTimer SysTimer Functions
 * \brief    Functions that configure the Core System Timer.
 * @{
 */

#define SYSTIMER_SET_OP(op, ...) \
    do { \
        op(systimer->impl[__get_hartid()], ##__VA_ARGS__); \
    } while (0)

#define SYSTIMER_GET_OP(op, ret_value, ...) \
    do { \
        return (ret_value) op(systimer->impl[__get_hartid()], ##__VA_ARGS__); \
    } while (0)

/**
 * \brief  Set system timer load value
 * \details
 * This function set the system timer load value in MTIMER register.
 * \param [in]  value   value to set system timer MTIMER register.
 * \remarks
 * - Load value is 64bits wide.
 * - \ref SysTimer_GetLoadValue
 */
__STATIC_FORCEINLINE void SysTimer_SetLoadValue(uint64_t value)
{
    SYSTIMER_SET_OP(__SysTimer_SetLoadValue, value);
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
__STATIC_FORCEINLINE uint64_t SysTimer_GetLoadValue(void)
{
    SYSTIMER_GET_OP(__SysTimer_GetLoadValue, uint64_t);
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
__STATIC_FORCEINLINE void SysTimer_SetCompareValue(uint64_t value)
{
    SYSTIMER_SET_OP(__SysTimer_SetCompareValue, value);
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
__STATIC_FORCEINLINE uint64_t SysTimer_GetCompareValue(void)
{
    SYSTIMER_GET_OP(__SysTimer_GetCompareValue, uint64_t);
}

#ifdef CONFIG_RV_CLIC_PRESENT
/**
 * \brief   System Tick Configuration
 * \details Initializes the System Timer and its non-vector interrupt, and starts the System Tick Timer.
 *
 *  In our default implementation, the timer counter will be set to zero, and it will start a timer compare non-vector interrupt
 *  when it matchs the ticks user set, during the timer interrupt user should reload the system tick using \ref SysTick_Reload function
 *  or similar function written by user, so it can produce period timer interrupt.
 * \param [in]  ticks  Number of ticks between two interrupts.
 * \return          0  Function succeeded.
 * \return          1  Function failed.
 * \remarks
 * - When the variable \ref __Vendor_SysTickConfig is set to 1, then the
 *   function \ref SysTick_Config is not included.
 * - In this case, the file <b><Device>.h</b> must contain a vendor-specific implementation
 *   of this function.
 * - If user need this function to start a period timer interrupt, then in timer interrupt handler
 *   routine code, user should call \ref SysTick_Reload with ticks to reload the timer.
 * - This function only available when __SYSTIMER_PRESENT == 1 and __CLIC_PRESENT == 1 and __Vendor_SysTickConfig == 0
 * \sa
 * - \ref SysTimer_SetCompareValue; SysTimer_SetLoadValue
 */
__STATIC_INLINE uint32_t SysTick_Config(uint64_t ticks)
{
    SysTimer_SetCompareValue(SysTimer_GetLoadValue() + ticks);
    CLIC_SetShvIRQ(Timer_IRQn, CLIC_VECTORED_INTERRUPT);
    CLIC_SetLevelIRQ(Timer_IRQn, 1);
    CLIC_EnableIRQ(Timer_IRQn);
    return (0UL);
}

/**
 * \brief   System Tick Reload
 * \details Reload the System Timer Tick when the MTIMECMP reached TIME value
 *
 * \param [in]  ticks  Number of ticks between two interrupts.
 * \return          0  Function succeeded.
 * \return          1  Function failed.
 * \remarks
 * - In interrupt handler function, user need to set the MTIMERCMP or MTIMER to reload
 *   the system tick, if vendor want to use this timer's auto clear feature, they can define
 *   \ref __Vendor_SysTickConfig to 1, and implement \ref SysTick_Config and \ref SysTick_Reload functions.
 * - When the variable \ref __Vendor_SysTickConfig is set to 1, then the
 *   function \ref SysTick_Reload is not included.
 * - In this case, the file <b><Device>.h</b> must contain a vendor-specific implementation
 *   of this function.
 * - This function only available when __SYSTIMER_PRESENT == 1 and __CLIC_PRESENT == 1 and __Vendor_SysTickConfig == 0
 * - Since the MTIMERCMP value might overflow, if overflowed, MTIMER will be set to 0, and MTIMERCMP set to ticks
 * \sa
 * - \ref SysTimer_SetCompareValue
 * - \ref SysTimer_SetLoadValue
 */
__STATIC_FORCEINLINE uint32_t SysTick_Reload(uint64_t ticks)
{
    uint64_t cur_ticks = SysTimer_GetLoadValue();
    uint64_t reload_ticks = ticks + cur_ticks;

    if (__USUALLY(reload_ticks > cur_ticks)) {
        SysTimer_SetCompareValue(reload_ticks);
    } else {
        /* When added the ticks value, then the MTIMERCMP < TIMER,
         * which means the MTIMERCMP is overflowed,
         * so we need to reset the counter to zero */
        SysTimer_SetCompareValue(reload_ticks);
    }

    return (0UL);
}

#endif /* defined(__Vendor_SysTickConfig) && (__Vendor_SysTickConfig == 0U) */
/**
 * @}
 */ /* End of Doxygen Group EMSIS_Core_SysTimer */

#endif /* CONFIG_RV_SYSTIMER_PRESENT */

#ifdef __cplusplus
}
#endif
#endif /** __CORE_FEATURE_TIMER_H__  */

