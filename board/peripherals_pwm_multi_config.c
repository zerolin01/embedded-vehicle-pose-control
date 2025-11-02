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
 * @file peripherals_pwm_multi_config.c
 * @author abu (abu@eswincomputing.com)
 * @date 2025-06-21
 *
 */

#include "peripherals_pwm_multi_config.h"

// PWM状态变量定义
pwm_state_t g_stPwmState_0;
pwm_state_t g_stPwmState_1;
pwm_state_t g_stPwmState_2;
pwm_state_t g_stPwmState_3;

// PWM0配置 - 用于电机2
pwm_config_t g_stPwm0Config0 = {
    .period          = 0xffff,
    .duty            = 0x8000,
    .pwm_clk_src_cfg = G_CLK,
    .pwm_polarity    = S_RV_C_CMP,
    .pwm_active_cfg  = 0x1,
    .prescaler       = 0x4,
    .repeat          = ONCE,
    .water_mark      = 4,
    .int_type        = 0x0,
    .callback        = NULL,
    .callbackParam   = NULL,
};

// PWM1配置 - 用于电机3
pwm_config_t g_stPwm1Config0 = {
    .period          = 0xffff,
    .duty            = 0x8000,
    .pwm_clk_src_cfg = G_CLK,
    .pwm_polarity    = S_RV_C_CMP,
    .pwm_active_cfg  = 0x1,
    .prescaler       = 0x4,
    .repeat          = ONCE,
    .water_mark      = 4,
    .int_type        = 0x0,
    .callback        = NULL,
    .callbackParam   = NULL,
};

// PWM2配置 - 用于电机1
pwm_config_t g_stPwm2Config0 = {
    .period          = 0xffff,
    .duty            = 0x8000,
    .pwm_clk_src_cfg = G_CLK,
    .pwm_polarity    = S_RV_C_CMP,
    .pwm_active_cfg  = 0x1,
    .prescaler       = 0x4,
    .repeat          = ONCE,
    .water_mark      = 4,
    .int_type        = 0x0,
    .callback        = NULL,
    .callbackParam   = NULL,
};

// PWM3配置 - 用于电机4
pwm_config_t g_stPwm3Config0 = {
    .period          = 0xffff,
    .duty            = 0x8000,
    .pwm_clk_src_cfg = G_CLK,
    .pwm_polarity    = S_RV_C_CMP,
    .pwm_active_cfg  = 0x1,
    .prescaler       = 0x4,
    .repeat          = ONCE,
    .water_mark      = 4,
    .int_type        = 0x0,
    .callback        = NULL,
    .callbackParam   = NULL,
}; 