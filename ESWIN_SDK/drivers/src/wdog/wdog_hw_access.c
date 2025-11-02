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
 * @file wdog_hw_access.c
 * @brief WDOG driver hardware access source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-10
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "wdog_hw_access.h"

/**
 *
 * Function Name : WDOG_Config
 * Description   : Configures the WDOG.
 *
 * This function configures the WDOG by user configuration
 *
 */
status_t WDOG_Config(wdog_type_t *const base, const wdog_user_config_t *wdogUserConfig)
{
    status_t status = STATUS_SUCCESS;

    bool tmp = WDOG_IsEnabled(base);

    if (true == tmp) {
        WDOG_Disable(base);
    } else {
    }
    /* Config low power mode */
    WDOG_SetLowPowerMode(base, wdogUserConfig);

    /* Set Wdog timeout value */
    WDOG_SetTimeOutValue(base, wdogUserConfig->timeoutValue);

    /*Set WDOG interrupt value if interrupt is enabled*/
    if (wdogUserConfig->intEnable) {
        WDOG_SetInt(base, wdogUserConfig->intValue);
    } else {
    }

    return status;
}

/**
 *
 * Function Name : WDOG_GetConfig
 * Description   : Gets the current WDOG configuration.
 *
 * This function gets the current WDOG configuration
 *
 */
wdog_user_config_t WDOG_GetConfig(const wdog_type_t *base)
{
    wdog_user_config_t config;
    uint32_t regValue = 0;

    uint32_t wcrRegValue  = base->WCR;
    uint32_t wicrRegValue = base->WICR;
#if 0
    /* Get wait status */
    regValue = (wcrRegValue & WDOG_WCR_WDW_MASK) >> WDOG_WCR_WDW_SHIFT;
    if (1 == regValue) {
        config.wait = WDOG_WORK_SUSPEND;
    } else {
        config.wait = WDOG_WORK_CONTINUE;
    }

    /* Get debug status */
    regValue = (wcrRegValue & WDOG_WCR_WDBG_MASK) >> WDOG_WCR_WDBG_SHIFT;
    if (1 == regValue) {
        config.debug = WDOG_WORK_SUSPEND;
    } else {
        config.debug = WDOG_WORK_CONTINUE;
    }
#endif
    /* Get stop/doze status*/
    regValue = (wcrRegValue & WDOG_WCR_WDZST_MASK) >> WDOG_WCR_WDZST_SHIFT;
    if (1 == regValue) {
        config.stop = WDOG_WORK_SUSPEND;
    } else {
        config.stop = WDOG_WORK_CONTINUE;
    }

    /* Get timeout value*/
    regValue            = (wcrRegValue & WDOG_WCR_WT_MASK) >> WDOG_WCR_WT_SHIFT;
    config.timeoutValue = regValue;

    /* Get interrupt enable status */
    regValue = (wicrRegValue & WDOG_WICR_WIE_MASK) >> WDOG_WICR_WIE_SHIFT;
    if (1 == regValue) {
        config.intEnable = true;
    } else {
        config.intEnable = false;
    }

    /* Get interrupt value */
    regValue = (wicrRegValue & WDOG_WICR_WICT_MASK) >> WDOG_WICR_WICT_SHIFT;

    config.intValue = regValue;

    return config;
}
