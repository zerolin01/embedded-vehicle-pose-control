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
 * @file pin_config.c
 * @author abu (abu@eswincomputing.com)
 * @date 2025-06-29
 *
 */

#include "pin_config.h"

/**
 * @brief User configuration structure
 */
pin_settings_config_t g_stPinmuxConfigArr[NUM_OF_CONFIGURED_PINS] = {
    {
        //PWM2 function, 100pin package, 52pin
        .base        = PORTB,
        .pinPortIdx  = 30U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR1_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT3,
        .isGpio      = false,
    },
    {
        //PORTB11 function, 100pin package, 41pin
        .base           = PORTB,
        .pinPortIdx     = 11U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //PORTB13 function, 100pin package, 43pin
        .base           = PORTB,
        .pinPortIdx     = 13U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //PORTB14 function, 100pin package, 44pin
        .base           = PORTB,
        .pinPortIdx     = 14U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //PWM0 function, 100pin package, 7pin
        .base        = PORTA,
        .pinPortIdx  = 6U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR1_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT6,
        .isGpio      = false,
    },
    {
        //PORTB0 function, 100pin package, 28pin
        .base           = PORTB,
        .pinPortIdx     = 0U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //PORTB1 function, 100pin package, 29pin
        .base           = PORTB,
        .pinPortIdx     = 1U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //PORTB2 function, 100pin package, 30pin
        .base           = PORTB,
        .pinPortIdx     = 2U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //PWM1 function, 100pin package, 42pin
        .base        = PORTB,
        .pinPortIdx  = 12U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR1_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT2,
        .isGpio      = false,
    },
    {
        //PORTB4 function, 100pin package, 32pin
        .base           = PORTB,
        .pinPortIdx     = 4U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //PORTB5 function, 100pin package, 33pin
        .base           = PORTB,
        .pinPortIdx     = 5U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //PORTB7 function, 100pin package, 35pin
        .base           = PORTB,
        .pinPortIdx     = 7U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //PWM3 function, 100pin package, 82pin
        .base        = PORTD,
        .pinPortIdx  = 8U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR1_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT4,
        .isGpio      = false,
    },
    {
        //PORTB8 function, 100pin package, 36pin
        .base           = PORTB,
        .pinPortIdx     = 8U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //PORTB25 function, 100pin package, 49pin
        .base           = PORTB,
        .pinPortIdx     = 25U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //PORTB26 function, 100pin package, 50pin
        .base           = PORTB,
        .pinPortIdx     = 26U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //SUPERTMR1_QD_PHA function, 100pin package, 80pin
        .base        = PORTD,
        .pinPortIdx  = 6U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT6,
        .isGpio      = false,
    },
    {
        //SUPERTMR1_QD_PHB function, 100pin package, 81pin
        .base        = PORTD,
        .pinPortIdx  = 7U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT6,
        .isGpio      = false,
    },
    {
        //SUPERTMR2_QD_PHA function, 100pin package, 89pin
        .base        = PORTD,
        .pinPortIdx  = 17U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT6,
        .isGpio      = false,
    },
    {
        //UART5_RX function, 100pin package, 63pin
        .base        = PORTC,
        .pinPortIdx  = 14U,
        .pullConfig  = PORT_INTERNAL_PULL_UP_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT6,
        .isGpio      = false,
    },
    {
        //UART5_TX function, 100pin package, 62pin
        .base        = PORTC,
        .pinPortIdx  = 13U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT6,
        .isGpio      = false,
    },
    {
        // UART2_TX配置
        .base        = PORTA,
        .pinPortIdx  = 16U,
        .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT3,
        .isGpio      = false,
    },
    {
        // UART2_RX配置
        .base        = PORTA,
        .pinPortIdx  = 18U,
        .pullConfig  = PORT_INTERNAL_PULL_UP_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT3,
        .isGpio      = false,
    },
    {
        //SERVO (software PWM) on PORTD21, replace PWM4 mux with GPIO out
        .base           = PORTD,
        .pinPortIdx     = 21U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
        {
        //PORTA30 function, 100pin package, 26pin
        .base           = PORTA,
        .pinPortIdx     = 30U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_INPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_LOW_LEVEL,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //PORTA31 function, 100pin package, 27pin
        .base           = PORTA,
        .pinPortIdx     = 31U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_LOW_LEVEL,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
	{
	        //I2C0_SDA function, 100pin package, 73pin
	        .base        = PORTC,
	        .pinPortIdx  = 26U,
	        .pullConfig  = PORT_INTERNAL_PULL_UP_ENABLED,
	        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
	        .mux         = PORT_MUX_ALT3,
	        .isGpio      = false,
	    },
	        {
        //I2C0_SCL function, 100pin package, 72pin
        .base        = PORTC,
        .pinPortIdx  = 25U,
        .pullConfig  = PORT_INTERNAL_PULL_UP_ENABLED,
        .driveSelect = PORT_STR2_DRIVE_STRENGTH,
        .mux         = PORT_MUX_ALT3,
        .isGpio      = false,
    },
    {
        //PORTD2 function, 100pin package, 78pin - H30 AD0
        .base           = PORTD,
        .pinPortIdx     = 2U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //PORTD4 function, 100pin package, 79pin - H30 INT
        .base           = PORTD,
        .pinPortIdx     = 4U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_INPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },
    {
        //PORTC4 function, 100pin package, 56pin - 第二个舵机GPIO控制
        .base           = PORTC,
        .pinPortIdx     = 4U,
        .pullConfig     = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect    = PORT_STR2_DRIVE_STRENGTH,
        .mux            = PORT_MUX_ALT1,
        .isGpio         = true,
        .direction      = GPIO_OUTPUT_DIRECTION,
        .initValue      = 0,
        .intConfig      = PORT_INT_DISABLED,
        .clearIntFlag   = true,
        .debounceEnable = false,
    },    
};
