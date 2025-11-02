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
 * @file wakeup_EAM2011.h
 * @brief Wakeup EAM2011 driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __WAKEUP_INTC_DRIVER_H__
#define __WAKEUP_INTC_DRIVER_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "emps_platform.h"
#include "status.h"

/**
 * @brief Sleep Mode wakeup source
 */
typedef enum {
    WDOG_INT          = 0x00U,
    RTC_INT           = 0x01U,
    DMA_REQ_INT       = 0x02U,
    PDMA_PITMR_INT    = 0x03U,
    CAN0_WAKEUP_MATCH = 0x04U,
    CAN0_WAKEUP_TO    = 0x05U,
    CAN1_WAKEUP_MATCH = 0x06U,
    CAN1_WAKEUP_TO    = 0x07U,
    CAN2_WAKEUP_MATCH = 0x08U,
    CAN2_WAKEUP_TO    = 0x09U,
    Reserved_0        = 0x0AU,
    Reserved_1        = 0x0BU,
    CAN3_WAKEUP_MATCH = 0x0CU,
    CAN3_WAKEUP_TO    = 0x0DU,
    CAN5_WAKEUP_MATCH = 0x0EU,
    CAN6_WAKEUP_TO    = 0x0FU,
    SPI0_INT          = 0x10U,
    SPI1_INT          = 0x11U,
    SPI2_INT          = 0x12U,
    SPI3_INT          = 0x13U,
    UART0_TX_INT      = 0x14U,
    UART0_RX_INT      = 0x15U,
    UART1_TX_INT      = 0x16U,
    UART1_RX_INT      = 0x17U,
    UART2_TX_INT      = 0x18U,
    UART2_RX_INT      = 0x19U,
    UART3_TX_INT      = 0x1AU,
    UART3_RX_INT      = 0x1BU,
    UART4_TX_INT      = 0x1CU,
    UART4_RX_INT      = 0x1DU,
    UART5_TX_INT      = 0x1EU,
    UART5_RX_INT      = 0x1FU,
    I2C0_MASTER_INT   = 0x20U,
    I2C0_SLAVE_INT    = 0x21U,
    I2C1_MASTER_INT   = 0x22U,
    I2C1_SLAVE_INT    = 0x23U,
    SUPERIO_INT       = 0x24U,
    PCTMR0_INT        = 0x25U,
    PCTMR1_INT        = 0x26U,
    PCTMR2_INT        = 0x27U,
    PCTMR3_INT        = 0x28U,
    PCTMR4_INT        = 0x29U,
    PCTMR5_INT        = 0x2AU,
    PCTMR6_INT        = 0x2BU,
    PCTMR7_INT        = 0x2CU,
    PCTMR8_INT        = 0x2DU,
    PITMR_INT         = 0x2EU,
    EWM_INT           = 0x2FU,
    ADC0_INT          = 0x30U,
    ADC1_INT          = 0x31U,
    CMP_INT           = 0x32U,
    CMP_STOP_INT      = 0x33U,
    PWM0_INT          = 0x34U,
    PWM1_INT          = 0x35U,
    PWM2_INT          = 0x36U,
    PWM3_INT          = 0x37U,
    PWM4_INT          = 0x38U,
    GPIO_INT          = 0x39U,
    HARD_RSTN_INT     = 0x3AU,
    NMI_B_INT         = 0x3BU,
} sleep_wakeup_source_t;

/**
 * @brief Definition for the PMU callback function.
 *
 * Prototype for the callback function registered in the wakeup driver.
 */
typedef void (*wakeup_callback_t)(void *parameter, uint64_t intrStatus);

/**
 * @brief WAKEUP user configuration structure
 */
typedef struct {
    uint64_t wakeupSource;      /**< wake sources */
    void *callbackParam;        /**< callback parameters */
    wakeup_callback_t callback; /**< The callback of interrupt */
} wakeup_user_config_t;

/*
 *
 * Function Name : WAKEUP_SYS_Configuration
 * Description   : Initializes the WAKEUP_INTC module.
 *
 */
status_t WAKEUP_SYS_Configuration(wakeup_user_config_t *workupConfig);

/*
 *
 * Function Name : WAKEUP_SYS_UpdateWakeupSource
 * Description   : Enable wakeup sources
 *
 */
status_t WAKEUP_SYS_UpdateWakeupSource(const wakeup_user_config_t *wakeupConfig);

#endif
