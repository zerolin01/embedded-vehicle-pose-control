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
 * @file supertmr_qd_driver.c
 * @brief SUPERTMR quadrature decoder driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "supertmr_qd_driver.h"
#include "supertmr_hw_access.h"

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_QuadDecodeStart
 * Description   : Configures the parameters needed and activates quadrature
 * decode mode.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_QuadDecodeStart(uint32_t instance, const supertmr_quad_decode_config_t *config)
{
    OS_ASSERT((1U == instance) || (2U == instance));
    OS_ASSERT(NULL != config);
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    supertmr_state_t *state       = supertmrStatePtr[instance];
    status_t retStatus            = STATUS_SUCCESS;

    if ((NULL != state) && (SUPERTMR_MODE_NOT_INITIALIZED == state->supertmrMode)) {
        /* Disable Quadrature Decoder */
        SUPERTMR_DRV_SetQuadDecoderCmd(supertmrBase, false);
        SUPERTMR_DRV_SetClockSource(supertmrBase, SUPERTMR_CLOCK_SOURCE_NONE);
        /* Configure Quadrature Decoder */
        /* Set decoder mode Speed and direction or Phase A and Phase B encoding */
        SUPERTMR_DRV_SetQuadMode(supertmrBase, (uint8_t)config->mode);
        /* Set filter state for Phase A (enable/disable) */
        SUPERTMR_DRV_SetQuadPhaseAFilterCmd(supertmrBase, config->phaseAConfig.phaseInputFilter);
        /* Set Phase A filter value if phase filter is enabled */
        if (config->phaseAConfig.phaseInputFilter) {
            SUPERTMR_DRV_SetChnInputCaptureFilter(supertmrBase, CHAN0_IDX, config->phaseAConfig.phaseFilterVal);
        }

        /* Set filter state for Phase B (enable/disable) */
        SUPERTMR_DRV_SetQuadPhaseBFilterCmd(supertmrBase, config->phaseBConfig.phaseInputFilter);
        /* Set Phase B filter value if phase filter is enabled */
        if (config->phaseBConfig.phaseInputFilter) {
            SUPERTMR_DRV_SetChnInputCaptureFilter(supertmrBase, CHAN1_IDX, config->phaseBConfig.phaseFilterVal);
        }

        /* Set polarity for Phase A and Phase B */
        SUPERTMR_DRV_SetQuadPhaseAPolarity(supertmrBase, (uint8_t)config->phaseAConfig.phasePolarity);
        SUPERTMR_DRV_SetQuadPhaseBPolarity(supertmrBase, (uint8_t)config->phaseBConfig.phasePolarity);
        /* Configure counter (initial value and maximum value) */
        SUPERTMR_DRV_SetCounterInitVal(supertmrBase, config->initialVal);
        SUPERTMR_DRV_SetMod(supertmrBase, config->maxVal);
        SUPERTMR_DRV_SetCounter(supertmrBase, config->initialVal);
        /* Enable Quadrature Decoder */
        SUPERTMR_DRV_SetQuadDecoderCmd(supertmrBase, true);
        state->supertmrMode = SUPERTMR_MODE_QUADRATURE_DECODER;
    } else {
        retStatus = STATUS_ERROR;
    }

    return retStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_QuadDecodeStop
 * Description   : De-activates quadrature decoder mode.
 *
 *END**************************************************************************/
status_t SUPERTMR_DRV_QuadDecodeStop(uint32_t instance)
{
    OS_ASSERT((1U == instance) || (2U == instance));
    supertmr_type_t *supertmrBase = g_pstSupertmrBase[instance];
    supertmr_state_t *state       = supertmrStatePtr[instance];
    OS_ASSERT(NULL != state);

    /* Disable Quadrature decoder */
    SUPERTMR_DRV_SetQuadDecoderCmd(supertmrBase, false);
    state->supertmrMode = SUPERTMR_MODE_NOT_INITIALIZED;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_DRV_QuadGetState
 * Description   : Return the current quadrature decoder state
 * (counter value, overflow flag and overflow direction)
 *
 *END**************************************************************************/
supertmr_quad_decoder_state_t SUPERTMR_DRV_QuadGetState(uint32_t instance)
{
    OS_ASSERT((1U == instance) || (2U == instance));
    supertmr_type_t const *supertmrBase = g_pstSupertmrBase[instance];
    supertmr_quad_decoder_state_t state;

    state.counterDirection  = SUPERTMR_DRV_GetQuadDir(supertmrBase);
    state.overflowDirection = SUPERTMR_DRV_GetQuadTimerOverflowDir(supertmrBase);
    state.overflowFlag      = SUPERTMR_DRV_HasTimerOverflowed(supertmrBase);
    state.counter           = SUPERTMR_DRV_GetCounter(supertmrBase);

    return state;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SUPERTMR_QD_DRV_GetDefaultConfig
 * Description   : This function will get the default configuration values
 * in the structure which is used as a common use-case.
 *
 *END**************************************************************************/
void SUPERTMR_QD_DRV_GetDefaultConfig(supertmr_quad_decode_config_t *const config)
{
    OS_ASSERT(NULL != config);

    config->mode                          = SUPERTMR_QUAD_COUNT_AND_DIR;
    config->initialVal                    = 0U;
    config->maxVal                        = 65535U;
    config->phaseAConfig.phaseInputFilter = false;
    config->phaseAConfig.phaseFilterVal   = 0U;
    config->phaseAConfig.phasePolarity    = SUPERTMR_QUAD_PHASE_NORMAL;
    config->phaseBConfig.phaseInputFilter = false;
    config->phaseBConfig.phaseFilterVal   = 0U;
    config->phaseBConfig.phasePolarity    = SUPERTMR_QUAD_PHASE_NORMAL;
}
