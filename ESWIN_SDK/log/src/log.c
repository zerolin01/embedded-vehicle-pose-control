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
 * NEITHER DOES ESWIN PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE ESWIN SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR
 * ANY WARRANTY CLAIM RELATING THERETO. WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ESWIN BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @file log.c
 * @brief log api
 * @author abu (abu@eswincomputing.com)
 * @date 2023-05-05
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "../include/log.h"
#include "pins_driver.h"
#include "uart_driver.h"

/**
 * @brief debug uart port
 *
 */
extern int32_t log_uart_nr;

/**
 * @brief log init flag
 *
 */
bool logInitFlag = false;

#define DEFALT_LOG_UART_INSTANCE 1U

/**
 * @brief Log init
 *
 * @return status_t
 */
__WEAK status_t logInit(void)
{
    status_t status = STATUS_SUCCESS;
    static uart_state_t g_stUartState_1;

    /**
     * User configuration structure
     */
    pin_settings_config_t g_stPinmuxConfigArr[2] = {
        {
            .base        = PORTA,
            .pinPortIdx  = 16U,
            .pullConfig  = PORT_INTERNAL_PULL_NOT_ENABLED,
            .driveSelect = PORT_STR2_DRIVE_STRENGTH,
            .mux         = PORT_MUX_ALT3,
            .isGpio      = false,
        },
        {
            .base        = PORTA,
            .pinPortIdx  = 18U,
            .pullConfig  = PORT_INTERNAL_PULL_UP_ENABLED,
            .driveSelect = PORT_STR2_DRIVE_STRENGTH,
            .mux         = PORT_MUX_ALT3,
            .isGpio      = false,
        },
    };

    uart_user_config_t g_stUart1UserConfig0 = {
        .baudRate        = 115200UL,
        .parityMode      = UART_PARITY_DISABLED,
        .stopBitCount    = UART_ONE_STOP_BIT,
        .bitCountPerChar = UART_8_BITS_PER_CHAR,
        .transferType    = UART_USING_INTERRUPTS,
        .fifoType        = UART_FIFO_DEPTH_1,
        .rxDMAChannel    = 0,
        .txDMAChannel    = 1,
    };

    PINS_DRV_Init(sizeof(g_stPinmuxConfigArr) / sizeof(pin_settings_config_t), g_stPinmuxConfigArr);
    status = UART_DRV_Init(DEFALT_LOG_UART_INSTANCE, &g_stUartState_1, &g_stUart1UserConfig0);

    log_uart_nr = DEFALT_LOG_UART_INSTANCE;

    return status;
}

__WEAK void logDeinit(void)
{
    UART_DRV_Deinit(DEFALT_LOG_UART_INSTANCE);
    log_uart_nr = -1;
    logInitFlag = false;
}

void setLogPort(uint32_t instance)
{
    OS_ASSERT(instance < UART_INSTANCE_COUNT);
    if (instance == log_uart_nr) {
        return;
    }
    if (logInitFlag) {
        logInitFlag = false;
        logDeinit();
    }
    log_uart_nr = instance;
    logInitFlag = true;
}