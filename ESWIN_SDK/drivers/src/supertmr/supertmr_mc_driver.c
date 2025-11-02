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
 * @file supertmr_mc_driver.c
 * @brief SUPERTMR counter driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "supertmr_mc_driver.h"
#include "supertmr_hw_access.h"

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_InitCounter
 * Description   : Initializes the SUPERTMR counter. This function provides access to the
 * SUPERTMR counter settings. The counter can be run in Up counting or Up-down counting modes.
 * To run the counter in Free running mode, choose Up counting option and provide
 * 0x0 for the countStartVal and 0xFFFF for countFinalVal. Please call this
 * function only when SUPERTMR is used as timer/counter. User must call the SUPERTMR_DRV_Deinit
 * and the SUPERTMR_DRV_Init to Re-Initialize the SUPERTMR before calling SUPERTMR_DRV_InitCounter
 * for the second time and afterwards.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_InitCounter(uint32_t instance, const supertmr_timer_param_t *timer)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    OS_ASSERT(NULL != timer);
    OS_ASSERT((SUPERTMR_MODE_UP_TIMER == timer->mode) || (SUPERTMR_MODE_UP_DOWN_TIMER == timer->mode));
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    supertmr_state_t *state       = supertmrStatePtr[instance];
    status_t retStatus            = STATUS_SUCCESS;
    uint8_t channel               = 0U;

    if ((NULL != state) && (SUPERTMR_MODE_NOT_INITIALIZED == state->supertmrMode)) {
        /* Disable counter clock */
        SUPERTMR_DRV_SetClockSource(supertmrBase, SUPERTMR_CLOCK_SOURCE_NONE);
        /* Clear the overflow flag */
        SUPERTMR_DRV_ClearTimerOverflow(supertmrBase);
        /* Set counter initial and maximum values */
        SUPERTMR_DRV_SetCounterInitVal(supertmrBase, timer->initialValue);
        SUPERTMR_DRV_SetMod(supertmrBase, timer->finalValue);
        /* Disable the quadrature decoder mode */
        SUPERTMR_DRV_SetQuadDecoderCmd(supertmrBase, false);
        /* Use SUPERTMR as counter, disable all the channels */
        for (channel = 0U; channel < SUPERTMR_CHANNEL_COUNT; channel++) {
            SUPERTMR_DRV_SetChnEdgeLevel(supertmrBase, channel, 0U);
        }

        /* Check the SUPERTMR counter modes */
        if (SUPERTMR_MODE_UP_TIMER == timer->mode) {
            SUPERTMR_DRV_SetCpwms(supertmrBase, false);
        } else if (SUPERTMR_MODE_UP_DOWN_TIMER == timer->mode) {
            SUPERTMR_DRV_SetCpwms(supertmrBase, true);
        } else {
            /* Do nothing */
        }

        state->supertmrMode = timer->mode;
    } else {
        retStatus = STATUS_ERROR;
    }

    return retStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_CounterStart
 * Description   : Starts the SUPERTMR counter.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_CounterStart(uint32_t instance)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];

    const supertmr_state_t *state = supertmrStatePtr[instance];
    /* Check the clock source is available for SUPERTMR counter */
    OS_ASSERT(state->supertmrSourceClockFrequency > 0U);
    /* Enable counter clock */
    SUPERTMR_DRV_SetClockSource(supertmrBase, state->supertmrClockSource);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_CounterStop
 * Description   : Stops the SUPERTMR counter.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_CounterStop(uint32_t instance)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];

    /* Stop the SUPERTMR counter */
    SUPERTMR_DRV_SetClockSource(supertmrBase, SUPERTMR_CLOCK_SOURCE_NONE);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_CounterRead
 * Description   : Reads back the current value of the SUPERTMR counter.
 *
 *END**************************************************************************/
uint32_t SUPERTMR_DRV_CounterRead(uint32_t instance)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    supertmr_type_t const *supertmrBase = g_pstSupertmrBase[instance];

    return SUPERTMR_DRV_GetCounter(supertmrBase);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_MC_DRV_GetDefaultConfig
 * Description   : This function will get the default configuration values
 * in the structure which is used as a common use-case.
 * Return        : None
 *
 *END**************************************************************************/
void SUPERTMR_MC_DRV_GetDefaultConfig(supertmr_timer_param_t *const config)
{
    OS_ASSERT(config != NULL);

    config->mode         = SUPERTMR_MODE_UP_TIMER;
    config->initialValue = 0U;
    config->finalValue   = 65535U;
}
