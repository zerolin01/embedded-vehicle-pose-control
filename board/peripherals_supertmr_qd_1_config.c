
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
 * @file peripherals_supertmr_qd_1_config.c
 * @author abu (abu@eswincomputing.com)
 * @date 2025-06-29
 *
 */

#include "peripherals_supertmr_qd_1_config.h"

supertmr_quad_decoder_state_t superTmrQuadDecoderState_2;

supertmr_user_config_t g_stSupertmr2UserConfigQd = {
    .syncMethod = {
        .softwareSync     = true,
        .hardwareSync0    = false,
        .hardwareSync1    = false,
        .hardwareSync2    = false,
        .maxLoadingPoint  = false,
        .minLoadingPoint  = false,
        .inverterSync     = SUPERTMR_PWM_SYNC,
        .outRegSync       = SUPERTMR_PWM_SYNC,
        .maskRegSync      = SUPERTMR_PWM_SYNC,
        .initCounterSync  = SUPERTMR_PWM_SYNC,
        .autoClearTrigger = false,
        .syncPoint        = SUPERTMR_UPDATE_NOW,
    },
    .supertmrMode                = SUPERTMR_MODE_QUADRATURE_DECODER,
    .supertmrPrescaler           = SUPERTMR_CLOCK_DIVID_BY_1,
    .supertmrClockSource         = SUPERTMR_CLOCK_SOURCE_SYSTEMCLK,
    .BDMMode                     = SUPERTMR_BDM_MODE_00,
    .isTofIsrEnabled             = false,
    .enableInitializationTrigger = false,
    .callback                    = NULL,
    .cbParams                    = NULL,
};

supertmr_quad_decode_config_t g_stSupertmr2QuadDecodeConfig = {
    .mode         = SUPERTMR_QUAD_COUNT_AND_DIR,
    .initialVal   = 0,
    .maxVal       = 0xFFFF,
    .phaseAConfig = {
        .phaseInputFilter = true,
        .phaseFilterVal   = 0x2,
        .phasePolarity    = SUPERTMR_QUAD_PHASE_NORMAL,
    },
    .phaseBConfig = {
        .phaseInputFilter = true,
        .phaseFilterVal   = 0x2,
        .phasePolarity    = SUPERTMR_QUAD_PHASE_NORMAL,
    },
};
