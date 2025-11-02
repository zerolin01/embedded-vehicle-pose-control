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
 * @file wakeup_EAM2011.c
 * @brief Wakeup EAM2011 driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include <stddef.h>

#include "emps_platform.h"
#include "wakeup_EAM2011.h"
#include "wakeup_intc_hw_access.h"
#include "wakeup_intc_register.h"

static const IRQn_Type s_wakeupIrqId = WAKEUP_IRQS;

/*
 * Function Name : PMU_DRV_IRQHandler
 * Description   : PMU IRQ handler.
 */
static void WAKEUP_SYS_IRQHandler(void *data)
{
    uint32_t intrStatus0 = 0, intrStatus1 = 0;
    wakeup_user_config_t *wakeConfig = (wakeup_user_config_t *)data;
    if (wakeConfig == NULL) {
        return;
    }

    intrStatus0 = WAKEUP->INTC_STATUS0;
    intrStatus1 = WAKEUP->INTC_STATUS1;
    if (intrStatus0) {
        WAKEUP->INTC_CLEAR0 = intrStatus0;
    }

    if (intrStatus1) {
        WAKEUP->INTC_CLEAR1 = intrStatus1;
    }

    if (wakeConfig->callback != NULL) {
        wakeConfig->callback(wakeConfig->callbackParam, intrStatus0 | (intrStatus1 << WAKEUP_INTC_STATUS1_SHIFT));
    }
}

/*
 * Function Name : WAKEUP_SYS_Configuration
 * Description   : Configuration the WAKEUP_INTC module.
 */
status_t WAKEUP_SYS_Configuration(wakeup_user_config_t *wakeupConfig)
{
    IRQn_Type irqNumber = NotAvail_IRQn;
    register_type_t type;

    /* Disable all wakeup source */
    WAKEUP->INTC_MASK0   = 0;
    WAKEUP->INTC_MASK1   = 0;
    WAKEUP->INTC_ENABLE0 = 0;
    WAKEUP->INTC_ENABLE1 = 0;

    type.lvl       = 1;
    type.priority  = 0;
    type.data_ptr  = wakeupConfig;
    type.trig_mode = CLIC_LEVEL_TRIGGER;
    irqNumber      = s_wakeupIrqId;
    OS_RequestIrq(irqNumber, WAKEUP_SYS_IRQHandler, &type);
    OS_EnableIrq(irqNumber);

    WAKEUP_SetWakeupIntsEnable(WAKEUP, wakeupConfig->wakeupSource);

    return STATUS_SUCCESS;
}

/*
 * Function Name : WAKEUP_SYS_UpdateWakeupSource
 * Description   : update wakeup sources
 */
status_t WAKEUP_SYS_UpdateWakeupSource(const wakeup_user_config_t *wakeupConfig)
{
    WAKEUP_SetWakeupIntsDisable(WAKEUP, 0xFFFFFFFFFFFFFFFF);
    WAKEUP_SetWakeupIntsEnable(WAKEUP, wakeupConfig->wakeupSource);

    return STATUS_SUCCESS;
}
