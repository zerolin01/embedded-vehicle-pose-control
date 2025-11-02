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
 * @file pwm_driver.h
 * @brief PWM driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __PWM_DRIVER_H__
#define __PWM_DRIVER_H__

#include "callbacks.h"
#include "osal.h"
#include "status.h"
#include <stdint.h>

/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup PWM_Driver PWM Driver
 * @{
 */

/**
 * @brief PWM active mode configuration enum.
 */
typedef enum {
    STOP_MODE_EN = 8, /**< Keeps the PWM functional while in stop mode. */
    DOZE_MODE_EN = 4, /**< Keeps the PWM functional in doze mode. */
    WAIT_MODE_EN = 2, /**< Keeps the PWM functional in wait mode. */
    DBG_MODE_EN  = 1, /**< Keeps the PWM functional in debug mode. */
} pwm_active_cfg_t;

/**
 * @brief PWM interrupt type.
 */
typedef enum {
    INT_CIE = 4, /**< Compare interrupt enable. */
    INT_RIE = 2, /**< Roll-over interrupt enable. */
    INT_FIE = 1, /**< FIFO empty interrupt enable. */
} pwm_interrupt_type;

/**
 * @brief PWM clk source configuration.
 */
typedef enum {
    G_CLK_OFF       = 0U, /**< Clock is off. */
    G_CLK           = 1U, /**< IP Global clock. */
    G_CLK_HIGH_FREQ = 2U, /**< IP Global clock high freq */
    G_CLK_32K       = 3U, /**< IP Global clock 32k */
} pwm_clk_cfg_t;

/**
 * @brief PWM sample repeat configuration.
 */
typedef enum {
    ONCE        = 0U, /**< Output pin is set at roll over and cleaned at comparision. */
    TWICE       = 1U, /**< Output pin is cleared at roll over and set at comparision. */
    FOUR_TIMES  = 2U, /**< Output is disconnected. */
    EIGHT_TIMES = 3U, /**< Output is disconnected. */
} pwm_fifo_repeat_t;

/**
 * @brief PWM output configuration.
 */
typedef enum {
    S_RV_C_CMP = 0U, /**< Output pin is set at roll over and cleaned at comparision. */
    C_RV_S_CMP = 1U, /**< Output pin is cleared at roll over and set at comparision. */
    OUT_DIS    = 2U, /**< Output is disconnected. */
    // DISCONT    = 3U, /**< Output is disconnected. */
} pwm_polarity_t;

/**
 * @brief Data structure containing information about PWM.
 *
 * The user must populate these members to set up the PWM.
 */
typedef struct {
    uint32_t period;               /**< Period of the PWM signal in ticks. \R{0x1,0xffff} \D{0xffff} */
    uint32_t duty;                 /**< Duty cycle in ticks. \R{0x1,0xffff} \D{0x1111} */
    pwm_clk_cfg_t pwm_clk_src_cfg; /**< Source clk. \D{G_CLK} */
    pwm_polarity_t pwm_polarity;   /**< Output polarity. \D{S_RV_C_CMP} */
    uint8_t pwm_active_cfg;        /**< Active mode. \R{0x1,0x8} \D{0x1} */
    uint32_t prescaler;            /**< Prescale ratio. \R{0x1,0xfff} \D{0x4} */
    pwm_fifo_repeat_t repeat;      /**< Repeat times. \D{ONCE} */
    uint8_t water_mark;            /**< Water mark(fifo flag is set when more than or equal to water_mark empty slots in fifo).
                           \R{0x1,0x4} \D{4} */
    uint32_t int_type;             /**< Interrupt type. \R{0x0,0x7} \D{INT_CIE | INT_RIE | INT_FIE} */
    pwm_callback_t callback;       /**< The callback of interrupt. \D{NULL} */
    void *callbackParam;           /**< Callback parameters. \D{NULL} */
} pwm_config_t;

/**
 * @brief PWM callback state type.
 */
typedef struct {
    void *callbackParam;     /**< Callback parameters. */
    pwm_callback_t callback; /**< The callback of interrupt. */
} pwm_state_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Init PWM with user config.
 *
 * @param instance The instance number of the PWM peripheral.
 * @param pwmState Sample write to the FIFO(0xffff).
 * @param pwmConfig The data structure containing information about pwm
 * @return status_t If success, it returns STATUS_SUCCESS. if error, it returns STATUS_ERROR.
 */
status_t PWM_DRV_Init(uint32_t instance, pwm_state_t *pwmState, const pwm_config_t *pwmConfig);

/**
 * @brief Get the frequency of PWM.
 * @param pwmConfig The data structure containing information about pwm.
 * @return The frequency of the PWM instance.
 */
uint32_t PWM_DRV_GetFrequency(const pwm_config_t *pwmConfig);

/**
 * @brief Deinit PWM.
 *
 * @param instance The instance number of the PWM peripheral.
 * @return status_t If success, it returns STATUS_SUCCESS. if error, it returns STATUS_ERROR.
 */
status_t PWM_DRV_Deinit(uint32_t instance);

/**
 * @brief Start generate PWM pulse.
 *
 * @param instance The instance number of the PWM peripheral.
 * @return status_t If success, it returns STATUS_SUCCESS. if error, it returns STATUS_ERROR.
 */
status_t PWM_DRV_Start(uint32_t instance);

/**
 * @brief Stop generate PWM pulse.
 *
 * @param instance The instance number of the PWM peripheral.
 * @return status_t If success, it returns STATUS_SUCCESS. if error, it returns STATUS_ERROR.
 */
status_t PWM_DRV_Stop(uint32_t instance);

/**
 * @brief Configure PWM Parameters.
 *
 * @param instance The instance number of the PWM peripheral.
 * @param pwmState Sample write to the FIFO(0xffff).
 * @param pwmConfig The data structure containing information about pwm.
 * @return status_t If success, it returns STATUS_SUCCESS. if error, it returns STATUS_ERROR.
 */
status_t PWM_DRV_Cfg(uint32_t instance, pwm_state_t *pwmState, const pwm_config_t *pwmConfig);

/**
 * @brief Enter/exit stop mode according the TCSR
 * 
 * @param  instance The instance number of the PWM peripheral.
 * @param  en Enable/disable.
 */
void PWM_DRV_TcsrStopZoneMode(uint8_t instance, bool en);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
/* end of group PWM_Driver */

/**
 * @}
 */
/* end of group Peripheral_Driver_layer */

#endif /* __PWM_DRIVER_H__ */