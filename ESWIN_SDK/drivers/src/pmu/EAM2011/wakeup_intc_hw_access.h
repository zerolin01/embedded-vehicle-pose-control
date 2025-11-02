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
 * @file wakeup_intc_hw_access.h
 * @brief Wakeup intc driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __WAKEUP_INTC_HW_ACCESS_H__
#define __WAKEUP_INTC_HW_ACCESS_H__

#include <stdbool.h>
#include <stdint.h>

#include "wakeup_intc_register.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 *
 * Function Name : WAKEUP_SetWakeupIntsEnable
 * Description   : This function configures the wakeup sources interrupt, with the
 * given value('1' enables interrupt for wakeup source).
 *
 */
static inline void WAKEUP_SetWakeupIntsEnable(wakeup_type_t *const baseAddr, const uint64_t sources)
{
    // uint32_t tmp0 = (uint32_t)baseAddr->INTC_ENABLE0;
    // uint32_t tmp1 = (uint32_t)baseAddr->INTC_ENABLE1;

    uint32_t source_l = sources & WAKEUP_INTC_MASK0_MASK;
    uint32_t source_h = (sources >> 32) & WAKEUP_INTC_MASK1_MASK;

    if (source_l != 0) {
        uint32_t tmp = (uint32_t)baseAddr->INTC_ENABLE0;
        tmp &= ~(WAKEUP_INTC_ENABLE0_MASK);
        tmp |= WAKEUP_INTC_ENABLE0(source_l);
        baseAddr->INTC_ENABLE0 = (uint32_t)tmp;
    }

    if (source_h != 0) {
        uint32_t tmp = (uint32_t)baseAddr->INTC_ENABLE1;
        tmp &= ~(WAKEUP_INTC_ENABLE0_MASK);
        tmp |= WAKEUP_INTC_ENABLE0(source_h);
        baseAddr->INTC_ENABLE1 = (uint32_t)tmp;
    }
}

/**
 *
 * Function Name : WAKEUP_SetWakeupIntsDisable
 * Description   : This function configures the wakeup sources interrupt, with the
 * given value('1' disables interrupt for wakeup source).
 *
 */
static inline void WAKEUP_SetWakeupIntsDisable(wakeup_type_t *const baseAddr, const uint64_t sources)
{
    // uint32_t tmp0 = (uint32_t)baseAddr->INTC_ENABLE0;
    // uint32_t tmp1 = (uint32_t)baseAddr->INTC_ENABLE1;

    uint32_t source_l = sources & WAKEUP_INTC_MASK0_MASK;
    uint32_t source_h = (sources >> 32) & WAKEUP_INTC_MASK1_MASK;

    if (source_l != 0) {
        uint32_t tmp = (uint32_t)baseAddr->INTC_ENABLE0;
        tmp &= (uint32_t)(~WAKEUP_INTC_ENABLE0(source_l));
        baseAddr->INTC_ENABLE0 = (uint32_t)tmp;
    }

    if (source_h != 0) {
        uint32_t tmp = (uint32_t)baseAddr->INTC_ENABLE1;
        tmp &= (uint32_t)(~WAKEUP_INTC_ENABLE1(source_h));
        baseAddr->INTC_ENABLE1 = (uint32_t)tmp;
    }
}

#if defined(__cplusplus)
}
#endif

#endif
