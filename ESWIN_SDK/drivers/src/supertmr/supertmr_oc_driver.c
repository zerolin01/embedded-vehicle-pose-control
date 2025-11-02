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
 * @file supertmr_oc_driver.c
 * @brief SUPERTMR out compare driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "supertmr_oc_driver.h"
#include "supertmr_hw_access.h"

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_InitOutputCompare
 * Description   : Configures the SUPERTMR to generate timed pulses
 * When the SUPERTMR counter matches the value of compareVal argument (this is
 * written into CnV register), the channel output is changed based on what is specified
 * in the compareMode argument.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_InitOutputCompare(uint32_t instance, const supertmr_output_cmp_param_t *param)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    OS_ASSERT(NULL != param);
    OS_ASSERT(param->maxCountValue > 0U);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    uint8_t index                 = 0U;
    uint8_t hwChannel             = 0U;
    uint8_t chnlPairNum           = 0U;
    supertmr_state_t *state       = supertmrStatePtr[instance];
    status_t retStatus            = STATUS_SUCCESS;

    if ((NULL != state) && (SUPERTMR_MODE_NOT_INITIALIZED == state->supertmrMode)) {
        SUPERTMR_DRV_SetClockSource(supertmrBase, SUPERTMR_CLOCK_SOURCE_NONE);
        SUPERTMR_DRV_SetCpwms(supertmrBase, false);
        /* Clear the overflow flag */
        SUPERTMR_DRV_ClearTimerOverflow(supertmrBase);
        SUPERTMR_DRV_SetCounterInitVal(supertmrBase, 0U);
        SUPERTMR_DRV_SetMod(supertmrBase, param->maxCountValue);
        SUPERTMR_DRV_SetCounter(supertmrBase, 0U);
        SUPERTMR_DRV_SetQuadDecoderCmd(supertmrBase, false);
        /* Use SUPERTMR as counter, disable all the channels */
        for (index = 0U; index < param->nNumOutputChannels; index++) {
            OS_ASSERT(param->maxCountValue >= param->outputChannelConfig[index].comparedValue);
            hwChannel   = param->outputChannelConfig[index].hwChannelId;
            chnlPairNum = (uint8_t)(hwChannel >> 1U);
            SUPERTMR_DRV_SetDualChnMofCombineCmd(supertmrBase, chnlPairNum, false);
            SUPERTMR_DRV_SetDualChnCombineCmd(supertmrBase, chnlPairNum, false);
            SUPERTMR_DRV_SetDualEdgeCaptureCmd(supertmrBase, chnlPairNum, false);
            /* Set Channel Output mode */
            SUPERTMR_DRV_SetChnEdgeLevel(supertmrBase, hwChannel, (uint8_t)(param->outputChannelConfig[index].chMode));
            /* Enter counter mode for all configured channels */
            SUPERTMR_DRV_SetChnMSnBAMode(supertmrBase, hwChannel, 1U);
            /* Write initial count value for all channels */
            SUPERTMR_DRV_SetChnCountVal(supertmrBase, hwChannel, param->outputChannelConfig[index].comparedValue);
            /* Enable channel output */
            SUPERTMR_DRV_EnablePwmChannelOutputs(supertmrBase, hwChannel);
            /* Enable the generation a trigger on chip module */
            SUPERTMR_DRV_SetChnTriggerCmd(supertmrBase, hwChannel,
                                          param->outputChannelConfig[index].enableExternalTrigger);
        }

        /* Set software trigger */
        SUPERTMR_DRV_SetSoftwareTriggerCmd(supertmrBase, true);
        state->supertmrMode = param->mode;
        /* Set clock source to start the counter */
        SUPERTMR_DRV_SetClockSource(supertmrBase, state->supertmrClockSource);
    } else {
        retStatus = STATUS_ERROR;
    }

    return retStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_DeinitOutputCompare
 * Description   : Disables compare match output control and clears SUPERTMR timer configuration
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_DeinitOutputCompare(uint32_t instance, const supertmr_output_cmp_param_t *param)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    OS_ASSERT(NULL != param);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    uint8_t index                 = 0U;
    uint8_t hwChannel             = 0U;
    supertmr_state_t *state       = supertmrStatePtr[instance];
    OS_ASSERT(state != NULL);

    /* Stop the SUPERTMR counter */
    SUPERTMR_DRV_SetClockSource(supertmrBase, SUPERTMR_CLOCK_SOURCE_NONE);
    /* Clear the overflow flag */
    SUPERTMR_DRV_ClearTimerOverflow(supertmrBase);
    SUPERTMR_DRV_SetCounterInitVal(supertmrBase, 0U);
    for (index = 0U; index < param->nNumOutputChannels; index++) {
        hwChannel = param->outputChannelConfig[index].hwChannelId;
        /* Disable Channel Output mode */
        SUPERTMR_DRV_SetChnEdgeLevel(supertmrBase, hwChannel, (uint8_t)0U);
        /* Write initial count value for all channels to 0xFFFF */
        SUPERTMR_DRV_SetChnCountVal(supertmrBase, hwChannel, 0U);
        /* Disable channel output */
        SUPERTMR_DRV_DisablePwmChannelOutputs(supertmrBase, hwChannel);
    }

    /* Clear out the registers */
    SUPERTMR_DRV_SetMod(supertmrBase, 0U);
    SUPERTMR_DRV_SetCounter(supertmrBase, 0U);
    state->supertmrMode = SUPERTMR_MODE_NOT_INITIALIZED;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_UpdateOutputCompareChannel
 * Description   : Sets the next compare match value on the given channel starting
 *                 from the current counter value.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_UpdateOutputCompareChannel(uint32_t instance, uint8_t channel, uint16_t nextComparematchValue,
                                                 supertmr_output_compare_update_t update, bool softwareTrigger)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);
    uint16_t counterValue    = SUPERTMR_DRV_GetCounter(g_pstSupertmrBase[instance]);
    uint16_t compareValue    = 0U;
    uint16_t maxCounterValue = SUPERTMR_DRV_GetMod(g_pstSupertmrBase[instance]);
    OS_ASSERT(nextComparematchValue <= maxCounterValue);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];

    if (SUPERTMR_RELATIVE_VALUE == update) {
        /* Configure channel compare register */
        if (nextComparematchValue > (maxCounterValue - counterValue)) {
            compareValue = (uint16_t)(nextComparematchValue - (maxCounterValue - counterValue));
        } else {
            compareValue = (uint16_t)(counterValue + nextComparematchValue);
        }
    } else {
        compareValue = nextComparematchValue;
    }

    /* Set CnV value and use software trigger for sync */
    SUPERTMR_DRV_SetChnCountVal(g_pstSupertmrBase[instance], channel, compareValue);
    SUPERTMR_DRV_SetSoftwareTriggerCmd(supertmrBase, softwareTrigger);

    return STATUS_SUCCESS;
}
