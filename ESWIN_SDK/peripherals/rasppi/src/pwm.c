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
 * @file pwm.c
 * @brief Examples for PWM module of RaspPi Port.
 * @author
 * @date 2023-12-13
 *
 * @details
 *
 *
 * @pins
 * | function name | port number | function number | connector |
 * |:------------- |:------------|:----------------|:----------|
 *      PWM1       |   PORTB12   |      ALT2       |   J12-32
 *
 * @connections
 *
 * @attention
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "rasppi.h"

/**
 * @brief The Rasppi PWM Example.
 *
 * @return void
 */
void RaspPi_Pwm_Example(void)
{
    /* Initialize PWM instance */
    PWM_DRV_Init(INST_PWM_1, &g_stPwmState_1, &g_stPwm1Config0);

    for (uint32_t i = 0; i < 0xffff;) {
        g_stPwm1Config0.duty = i;
        i += 0x1111;
        PWM_DRV_Cfg(INST_PWM_1, &g_stPwmState_1, &g_stPwm1Config0);
        OS_DelayMs(100);
    }

    for (uint32_t i = 0xffff; i > 0;) {
        g_stPwm1Config0.duty = i;
        i -= 0x1111;
        PWM_DRV_Cfg(INST_PWM_1, &g_stPwmState_1, &g_stPwm1Config0);
        OS_DelayMs(100);
    }

    g_stPwm1Config0.duty = 0;
    PWM_DRV_Cfg(INST_PWM_1, &g_stPwmState_1, &g_stPwm1Config0);

    OS_DelayMs(1000);

    PWM_DRV_Deinit(INST_PWM_1);
}
