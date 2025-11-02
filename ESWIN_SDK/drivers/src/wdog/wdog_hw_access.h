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
 * @file wdog_hw_access.h
 * @brief WDOG driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-13
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __WDOG_HW_ACCESS_H__
#define __WDOG_HW_ACCESS_H__

#include "wdog_driver.h"
#include "wdog_register.h"
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief  Configures the WDOG.
 *
 * This function configures the WDOG by user configuration
 *
 * @param  base  WDOG base pointer.
 * @param  wdogUserConfig  pointer to user configuration structure
 * @return status_t
 *         STATUS_SUCCESS: if allowed reconfigures WDOG timeout.
 *         STATUS_ERROR if Operation failed.
 */
status_t WDOG_Config(wdog_type_t *const base, const wdog_user_config_t *wdogUserConfig);

/**
 * @brief  Gets the current WDOG configuration.
 *
 * This function gets the current WDOG configuration
 *
 * @param  base WDOG base pointer.
 * @return wdog_user_config_t
 *          the current WDOG configuration
 */
wdog_user_config_t WDOG_GetConfig(const wdog_type_t *base);

/**
 * @brief   Refreshes the WDOG counter
 *
 * @param  base WDOG base pointer.
 */
static inline void WDOG_Refresh(wdog_type_t *const base)
{
    /* Default timeout value */
    base->WSR = 0x5555;
    base->WSR = 0xAAAA;
}
/**
 * @brief  Disable the WDOG.
 *
 * @param  base  WDOG base pointer.
 */
static inline void WDOG_Disable(wdog_type_t *const base)
{
    base->WCR = (WDOG_CR_RESET_VALUE | WDOG_WCR_WT(WDOG_TO_RESET_VALUE));
}

/**
 * @brief  Disable the WDOG.
 *
 * @param  base  WDOG base pointer.
 */
static inline void WDOG_Enable(wdog_type_t *const base)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->WCR;
    tmp &= ~WDOG_WCR_WDE_MASK;
    tmp |= WDOG_WCR_WDE(1U);
    base->WCR = tmp;
}

/**
 * @brief checkout whether the WDOG is enabled.
 *
 * @param  base  WDOG base pointer.
 * @return true if WDOG enabled;
 * @return false if WDOG disabled;
 */
static inline bool WDOG_IsEnabled(const wdog_type_t *base)
{
    bool result = false;

    uint32_t tmp = base->WCR;

    tmp &= WDOG_WCR_WDE_MASK;
    if (1 == tmp) {
        result = true;
    } else {
        result = false;
    }

    return result;
}

/**
 * @brief Clears the Interrupt Flag.
 *
 * This function clears the Interrupt flag
 *
 * @param  base  WDOG base pointer.
 */
static inline void WDOG_ClearIntFlag(wdog_type_t *const base)
{
    base->WICR = base->WICR | WDOG_WICR_WTIS(1U);
}

/**
 * @brief   Enables/Disables WDOG in wait mode.
 *
 * This function enables/disables the WDOG in wait mode.
 *
 * @param  enable  enable/disable WDOG in wait mode.
 */
static inline void WDOG_SetLowPowerMode(wdog_type_t *const base, const wdog_user_config_t *wdogUserConfig)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = 0;

    /* Config wait mode */
    tmp &= ~WDOG_WCR_WDW_MASK;
    tmp |= WDOG_WCR_WDW(1);

    /*Config debug mode*/
    tmp &= ~WDOG_WCR_WDBG_MASK;
    tmp |= WDOG_WCR_WDBG(1);

    /*Config stop/doze mode*/
    tmp &= ~WDOG_WCR_WDZST_MASK;
    tmp |= WDOG_WCR_WDZST(wdogUserConfig->stop);

    base->WCR |= tmp;
}

/**
 * @brief  Enables/Disables WDOG in stop mode.
 *
 * This function enables/disables the WDOG in stop mode.
 *
 * @param  enable   enable/disable WDOG in stop mode
 */
static inline void WDOG_SetStopDoze(wdog_type_t *const base, wdog_work_mode_t workMode)
{
    OS_ASSERT(base != NULL);

    uint32_t tmp = base->WCR;
    tmp &= ~WDOG_WCR_WDZST_MASK;
    tmp |= WDOG_WCR_WDZST(workMode);
    base->WCR = tmp;
}

/**
 * @brief  Set time how long before the counter time-out must the interrupt occur and enable the interrupt.
 *
 * This field is write once only.Once the software does a write access to this field, is will get locked and cannot be
 * reprogrammed until the next system reset assertion.
 *
 * @param  instance WDOG instance number.
 * @param  intval   The value of the interrupt before timeout.
 */
static inline void WDOG_SetInt(wdog_type_t *const base, uint16_t intval)
{
    OS_ASSERT(base != NULL);

    uint32_t WICR = 0;

    /* Set interrupt value */
    WICR &= ~WDOG_WICR_WICT_MASK;
    WICR |= WDOG_WICR_WICT(intval);

    /* Enable interrupt */
    WICR |= WDOG_WICR_WIE(1);

    base->WICR = WICR;
}

/**
 * @brief Set WDOG timeout interrupt value.
 *
 * @param  base WDOG base pointer.
 */
static inline void WDOG_SetTimeOutValue(wdog_type_t *const base, const uint16_t TimeoutValue)
{
    OS_ASSERT(base != NULL);

    base->WCR &= ~WDOG_WCR_WT_MASK;
    base->WCR |= WDOG_WCR_WT(TimeoutValue);
}

#if defined(__cplusplus)
}
#endif
#endif
