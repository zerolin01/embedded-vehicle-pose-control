/**
 * Copyright Statement:
 * This software and related documentation (ESWIN SOFTWARE) are protected under relevant copyright laws.
 * The information contained herein is confidential and proprietary to
 * Beijing ESWIN Computing Technology Co., Ltd.(ESWIN)and/or its licensors.
 * Without the prior written permission of ESWIN and/or its licensors, any reproduction, modification,
 * use or disclosure Software, and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * Copyright ©[2023] [Beijing ESWIN Computing Technology Co., Ltd.]. All rights reserved.
 *
 * RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES THAT THE SOFTWARE
 * AND ITS DOCUMENTATIONS (ESWIN SOFTWARE) RECEIVED FROM ESWIN AND / OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. ESWIN EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON INFRINGEMENT.
 * NEITHER DOES ESWIN PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY
 * WHICH MAY BE USED BY,INCORPORATED IN, OR SUPPLIED WITH THE ESWIN SOFTWARE,
 * AND RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ESWIN BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @file ewm_hw_access.h
 * @brief EWM driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-12
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __EWM_HW_ACCESS_H__
#define __EWM_HW_ACCESS_H__

#include "ewm_driver.h"
#include "ewm_register.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief  Refresh EWM
 *
 * This method needs to be called within the window period
 * specified by the Compare Low and Compare High registers
 *
 * @param  base  EWM base pointer
 */
static inline void EWM_Refresh(ewm_type_t *const base)
{
    /* Write first byte of the service key */
    base->SERV = EWM_KEY_FIRST_BYTE;
    /* Write second byte of the service key */
    base->SERV = EWM_KEY_SECOND_BYTE;
}

/**
 * @brief  Get EWM enablement status.
 *
 * @param  base  EWM base pointer
 * @return true  EWM enabled
 * @return false EWM disabled
 */
static inline bool EWM_IsEnabled(const ewm_type_t *const base)
{
    uint16_t regValue = 0;

    bool result = false;

    regValue = base->CTRL;

    regValue &= EWM_CTRL_EWMEN_MASK;

    regValue = regValue >> EWM_CTRL_EWMEN_SHIFT;
    if (regValue) {
        result = true;
    } else {
        result = false;
    }

    return result;
}

/**
 * @brief Enable EWM.
 *
 * @param  base    EWM base pointer
 */
static inline void EWM_Enable(ewm_type_t *base)
{
    base->CTRL = base->CTRL | EWM_CTRL_EWMEN(EWM_ENABLE);
}

/**
 * @brief  Set the Control Value.
 *
 * This register can be only written once after a CPU reset.
 *
 * @param  base    EWM base pointer
 * @param  value   Value to write into Control register
 */
static inline void EWM_SetControl(ewm_type_t *const base, uint8_t value)
{
    base->CTRL = value;
}

/**
 * @brief Get the Control register Value.
 *
 * @param  base EWM base pointer
 * @return uint8_t
 *         Value stored in Control register
 */
static inline uint8_t EWM_GetControl(const ewm_type_t *const base)
{
    return base->CTRL;
}

/**
 * @brief  Set the Compare Low Value.
 *
 * This register can be only written once after a CPU reset.
 * The user must make sure that the Compare High is greater than Compare Low value
 *
 * @param  base  EWM base pointer
 * @param  value Value to write into Compare Low register
 */
static inline void EWM_SetCompareLow(ewm_type_t *const base, uint8_t value)
{
    base->CMPL = value;
}

/**
 * @brief  Set the Compare High Value.
 *
 * This register can be only written once after a CPU reset.
 * The user must make sure that the Compare High is greater than Compare Low value
 * Note: The maximum Compare High value is 0xFE
 *
 * @param  base    EWM base pointer
 * @param  value   Value to write into Compare High register
 */
static inline void EWM_SetCompareHigh(ewm_type_t *const base, uint8_t value)
{
    base->CMPH = value;
}

/**
 * @brief  Set the Clock Prescaler Value.
 *
 * This register can be only written once after a CPU reset and
 * it must be written before enabling the EWM
 *
 * @param  base   EWM base pointer
 * @param  value  Prescaler Value
 */
static inline void EWM_SetPrescaler(ewm_type_t *const base, uint8_t value)
{
    base->CLKPRESCALER = value;
}

#if defined(__cplusplus)
}
#endif

#endif
