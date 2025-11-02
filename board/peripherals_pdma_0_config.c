
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
 * @file peripherals_pdma_0_config.c
 * @author abu (abu@eswincomputing.com)
 * @date 2025-07-10
 *
 */

#include "peripherals_pdma_0_config.h"

pdma_state_t g_stPdmaState0;

pdma_chn_state_t g_stPdma0ChnState0;

pdma_channel_config_t g_stPdma0ChannelConfig0 = {
    .groupPriority   = PDMA_GRP0_PRIO_LOW_GRP1_PRIO_HIGH,
    .channelPriority = PDMA_CHN_DEFAULT_PRIORITY,
    .virtChnConfig   = 0,
    .source          = PDMA_REQ_I2C0,
    .enableTrigger   = false,
    .callback        = NULL,
    .callbackParam   = NULL,
};

const pdma_channel_config_t *g_stPdma0ChannelConfigArray[PDMA_CHANNEL_CONFIG_COUNT] = {
    &g_stPdma0ChannelConfig0,
};

pdma_chn_state_t *g_stPdma0ChnStateArray[PDMA_CHN_STATE_COUNT] = {
    &g_stPdma0ChnState0,
};

pdma_user_config_t g_stPdma0UserConfig0 = {
    .chnArbitration   = PDMA_ARBITRATION_FIXED_PRIORITY,
    .groupArbitration = PDMA_ARBITRATION_FIXED_PRIORITY,
    .groupPriority    = PDMA_GRP0_PRIO_LOW_GRP1_PRIO_HIGH,
    .haltOnError      = false,
};
