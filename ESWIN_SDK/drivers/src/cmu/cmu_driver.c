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
 * @file cmu_driver.c
 * @brief 
 * @author abu (abu@eswincomputing.com)
 * @date 2023-05-23
 * 
 * Modification History :
 * Date:               Version:                    Author:     
 * Changes: 
 * 
 */

#include "cmu_driver.h"
#include "callbacks.h"
#include "cmu_hw_access.h"
#include <assert.h>

/*
 * @brief Table of base pointers of CMU instances.
 */
static cmu_type_t *g_cmuBase[CMU_INSTANCE_COUNT] = CMU_BASE_PTRS;
static cmu_common_type_t *g_cmuCommonBase        = CMU_COMMON;
static uint32_t g_last_instance                  = 0xffffffff;

/*
 * @brief Table to save CMU IRQ enumeration numbers defined in the hard related header file.
 */
static IRQn_Type g_cmuIrqId[CMU_INSTANCE_COUNT] = CMU_IRQS;

/* Pointer to runtime state structure */
static cmu_state_t *g_cmuStatePtr[CMU_INSTANCE_COUNT] = {NULL};

static void CMU_IRQHandler(void *data);

/*
 * Function Name : CMU_DRV_Init
 * Description   : Init CMU with user config.
 *
 */
status_t CMU_DRV_Init(uint32_t instance, cmu_state_t *cmuState, const cmu_config_t *cmuConfig)
{
    OS_ASSERT(instance < CMU_INSTANCE_COUNT);
    OS_ASSERT(cmuState != NULL);
    OS_ASSERT(cmuConfig != NULL);
    cmu_type_t *base              = g_cmuBase[instance];
    cmu_common_type_t *commonBase = g_cmuCommonBase;
    OS_ASSERT(base != NULL);
    OS_ASSERT(commonBase != NULL);
    OS_RegisterType_t uType = {0};

    /* runtime state */
    g_cmuStatePtr[instance] = cmuState;

    /* store config to runtime */
    cmuState->callback      = cmuConfig->callback;
    cmuState->callbackParam = cmuConfig->callbackParam;

    /* Set Th Hval */
    CMU_Set_ThHval(base, cmuConfig->th_hval);
    /* Set Th Lval */
    CMU_Set_ThLval(base, cmuConfig->th_lval);

    /* Set irq mask register according int enable */
    CMU_Set_ItNormal_Mask(commonBase, instance, !cmuConfig->int_normal_en);
    CMU_Set_ItAbnormal_Mask(commonBase, instance, !cmuConfig->int_abnormal_en);

    /* Register irq handler */
    uType.trig_mode = CLIC_LEVEL_TRIGGER;
    uType.lvl       = 250;
    uType.priority  = 1;
    uType.data_ptr  = (void *)instance;
    OS_RequestIrq(g_cmuIrqId[instance], CMU_IRQHandler, &uType);
    /* Enable the interrupt */
    OS_EnableIrq(g_cmuIrqId[instance]);

    /* Set CMU Enable */
    CMU_Enable(commonBase, instance, true);

    return STATUS_SUCCESS;
}

/*
 * Function Name : CMU_DRV_Deinit
 * Description   : Deinit CMU.
 *
 */
status_t CMU_DRV_Deinit(uint32_t instance)
{
    OS_ASSERT(instance < CMU_INSTANCE_COUNT);
    cmu_type_t *base              = g_cmuBase[instance];
    cmu_common_type_t *commonBase = g_cmuCommonBase;
    OS_ASSERT(base != NULL);
    OS_ASSERT(commonBase != NULL);

    /* Set CMU Disable*/
    CMU_Enable(commonBase, instance, false);

    /* Disable the interrupt */
    OS_DisableIrq(g_cmuIrqId[instance]);

    g_cmuStatePtr[instance] = NULL;

    /* Delay more than 5 period of clk_mon according CMU spec*/
    if (instance == g_last_instance) {
        g_last_instance = instance;
        OS_DelayMs(1);
    }

    return STATUS_SUCCESS;
}

/*
 * Function Name : CMU_DRV_Get_MeasDone
 * Description   : Get the specified CMU's monitor if it is done.
 *
 */
bool CMU_DRV_Get_MeasDone(uint32_t instance)
{
    OS_ASSERT(instance < CMU_INSTANCE_COUNT);
    cmu_type_t *base = g_cmuBase[instance];
    OS_ASSERT(base != NULL);
    bool done = false;

    uint32_t tmp = CMU_Get_MeasDone(base);
    if (tmp)
        done = true;

    return done;
}

/*
 * Function Name : CMU_DRV_Get_MeaStatus
 * Description   : Get the result of monitoring after its done.
 * The result is normal or abnormal. This function must be called
 * when its monitoring action is done
 *
 */
uint8_t CMU_DRV_Get_MeaStatus(uint32_t instance)
{
    OS_ASSERT(instance < CMU_INSTANCE_COUNT);

    cmu_common_type_t *commonBase = g_cmuCommonBase;
    OS_ASSERT(commonBase != NULL);
    uint8_t status = 0;

    uint32_t tmp = CMU_Get_ItStatus(commonBase);

    if (tmp & CMU_IT_ABNORMAL_STATUS_MASK(instance)) {
        CMU_Clear_ItAbnormal(commonBase, instance);

        while (tmp & CMU_IT_ABNORMAL_STATUS_MASK(instance)) {
            tmp = CMU_Get_ItStatus(commonBase);
        }

        status |= CMU_IT_STATUS_ABNORMAL;
    }

    if (tmp & CMU_IT_NORMAL_STATUS_MASK(instance)) {
        CMU_Clear_ItNormal(commonBase, instance);

        while (tmp & CMU_IT_NORMAL_STATUS_MASK(instance)) {
            tmp = CMU_Get_ItStatus(commonBase);
        }

        status |= CMU_IT_STATUS_NORMAL;
    }

    return status;
}

/*
 * Function Name : CMU_IRQHandler
 * Description   : The function CMU_IRQHandler passes IRQ control driver.
 *
 */
static void CMU_IRQHandler(void *data)
{
    uint32_t instance = (uint32_t)data;

    OS_ASSERT(instance < CMU_INSTANCE_COUNT);

    cmu_common_type_t *commonBase = g_cmuCommonBase;
    cmu_state_t *state            = g_cmuStatePtr[instance];
    OS_ASSERT(commonBase != NULL);
    OS_ASSERT(state != NULL);

    uint32_t tmp = CMU_Get_ItStatus(commonBase);

    if (tmp & CMU_IT_ABNORMAL_STATUS_MASK(instance)) {
        CMU_Clear_ItAbnormal(commonBase, instance);

        uint32_t status = CMU_Get_ItStatus(commonBase);
        while (status & CMU_IT_ABNORMAL_STATUS_MASK(instance)) {
            status = CMU_Get_ItStatus(commonBase);
        }

        if (state->callback) {
            state->callback(CMU_EVENT_ABNORMAL, state->callbackParam);
        }
    }

    if (tmp & CMU_IT_NORMAL_STATUS_MASK(instance)) {
        CMU_Clear_ItNormal(commonBase, instance);

        uint32_t status = CMU_Get_ItStatus(commonBase);
        while (status & CMU_IT_NORMAL_STATUS_MASK(instance)) {
            status = CMU_Get_ItStatus(commonBase);
        }

        if (state->callback) {
            state->callback(CMU_EVENT_NORMAL, state->callbackParam);
        }
    }

    return;
}