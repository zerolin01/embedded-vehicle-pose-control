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
 * @file superio_pwm_driver.h
 * @brief SUPERIO pwm driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __SUPERIO_PWM_DRIVER_H__
#define __SUPERIO_PWM_DRIVER_H__

#include "callbacks.h"
#include "superio_driver.h"
#include <stdbool.h>
#include <stddef.h>

/**
 * @addtogroup Peripheral_Driver_layer Peripheral Driver Layer
 * @{
 */

/**
 * @addtogroup SUPERIO_PWM_Driver SUPERIO_PWM Driver
 * @{
 */

/**
 * @brief PWM output polarity.
 */
typedef enum {
    SUPERIO_PWM_ACTIVE_HIGH = 0x0U,
    SUPERIO_PWM_ACTIVE_LOW  = 0x1U,
} superio_pwm_PolarityType_t;

/**
 * @brief Timer decrement options.
 */
typedef enum {
    SUPERIO_PWM_DECREMENT_CLK_SHIFT_TMR =
        0x00U, /**< Decrement counter on SuperIO clock, Shift clock equals Timer output. */
    SUPERIO_PWM_DECREMENT_TRG_SHIFT_TMR =
        0x01U, /**< Decrement counter on Trigger input (both edges), Shift clock equals Timer output. */
    SUPERIO_PWM_DECREMENT_PIN_SHIFT_PIN =
        0x02U, /**< Decrement counter on Pin input (both edges), Shift clock equals Pin input. */
    SUPERIO_PWM_DECREMENT_TRG_SHIFT_TRG =
        0x03U, /**< Decrement counter on Trigger input (both edges), Shift clock equals Trigger input. */
} superio_pwm_decrement_t;

/**
 * @brief Timer enable options.
 */
typedef enum {
    SUPERIO_PWM_ENABLE_ALWAYS               = 0x00U, /**< Timer always enabled. */
    SUPERIO_PWM_ENABLE_TIM_ENABLE           = 0x01U, /**< Timer enabled on Timer N-1 enable. */
    SUPERIO_PWM_ENABLE_TRG_HIGH             = 0x02U, /**< Timer enabled on Trigger high. */
    SUPERIO_PWM_ENABLE_TRG_PIN_HIGH         = 0x03U, /**< Timer enabled on Trigger high and Pin high. */
    SUPERIO_PWM_ENABLE_PIN_POSEDGE          = 0x04U, /**< Timer enabled on Pin rising edge. */
    SUPERIO_PWM_ENABLE_PIN_POSEDGE_TRG_HIGH = 0x05U, /**< Timer enabled on Pin rising edge and Trigger high. */
    SUPERIO_PWM_ENABLE_TRG_POSEDGE          = 0x06U, /**< Timer enabled on Trigger rising edge. */
    SUPERIO_PWM_ENABLE_TRG_EDGE             = 0x07U, /**< Timer enabled on Trigger rising or falling edge. */
} superio_pwm_enable_t;

/**
 * @brief Trigger polarity.
 */
typedef enum {
    SUPERIO_PWM_TRIGGER_POLARITY_HIGH = 0x00U, /**< Trigger is active high. */
    SUPERIO_PWM_TRIGGER_POLARITY_LOW  = 0x01U, /**< Trigger is active low. */
} superio_pwm_trigger_polarity_t;

/**
 * @brief Trigger sources.
 */
typedef enum {
    SUPERIO_PWM_TRIGGER_SOURCE_EXTERNAL = 0x00U, /**< External trigger selected. */
    SUPERIO_PWM_TRIGGER_SOURCE_INTERNAL = 0x01U, /**< Internal trigger selected. */
} superio_pwm_trigger_source_t;

/**
 * @brief PWM configuration parameters structure.
 *
 * Superio Pwm specific channel configuration structure type.
 */
typedef struct {
    superio_driver_type_t driverType; /**< Driver type: interrupts/polling/PDMA. \D{SUPERIO_DRIVER_TYPE_POLLING} */
    // uint8_t TimerId;                                 /**< Superio used timer index. */
    uint8_t PinId;     /**< Superio used pin index. \R{0,7} \D{0} */
    uint16_t Period;   /**< Pwm period in ticks. \R{1,0xff} \D{300} */
    uint8_t DutyCycle; /**< Pwm duty cycle in ticks. \R{1,0xff} \D{150} */
    superio_pwm_decrement_t
        Prescaler;                                  /**< Counter decrement clock prescaler. \D{SUPERIO_PWM_DECREMENT_CLK_SHIFT_TMR} */
    superio_pwm_PolarityType_t Polarity;            /**< Pwm output polarity. \D{SUPERIO_PWM_ACTIVE_HIGH} */
    superio_pwm_enable_t TimerEnable;               /**< \D{SUPERIO_PWM_ENABLE_ALWAYS} */
    superio_pwm_trigger_source_t triggerSrc;        /**< \D{SUPERIO_PWM_TRIGGER_SOURCE_EXTERNAL} */
    superio_pwm_trigger_polarity_t triggerPolarity; /**< \D{SUPERIO_PWM_TRIGGER_POLARITY_HIGH} */
    uint8_t triggerSel;                             /**< \R{0,0xf} \D{0} */
    pwm_callback_t callback;                        /**< User notification callback. \D{NULL} */
    void *callbackParam;                            /**< \D{NULL} */
} superio_pwm_channel_cfg_t;

/**
 * @brief Driver internal context structure.
 *
 * This structure is used by the superio_pwm driver for its internal logic. It must
 * be provided by the application through the SUPERIO_UART_DRV_Init() function, then
 * it cannot be freed until the driver is de-initialized using SUPERIO_UART_DRV_DeInit().
 * The application should make no assumptions about the content of this structure.
 */
typedef struct {
    superio_common_state_t superioCommon; /**< Common superio drivers structure. */
    pwm_callback_t callback;              /**< User callback function. */
    void *callbackParam;                  /**< Parameter for the callback function. */
    superio_driver_type_t driverType;     /**< Driver type: interrupts/polling/PDMA. */
    status_t status;                      /**< Current status of the driver. */
    uint16_t period;
    uint8_t dutyCycle;
    superio_pwm_PolarityType_t polarity;
    superio_pwm_decrement_t prescaler;
    uint8_t pinId;
} superio_pwm_state_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize a superio channel in pwm mode.
 * @details The function will initialize one timer and pin of the selected superio channel in pwm
 * mode, with the configuration of the user. The interrupts will be disabled.
 *
 * @param instance   The Superio instance.
 * @param channelConfigPtr      The channel configuration for the selected Superio instance.
 * @param state      The pwm state pointer.
 * @return SUPERIO_PWM_STATUS_SUCCESS - if the initialization was succeed.
 */
status_t SUPERIO_PWM_DRV_Init(uint32_t instance, const superio_pwm_channel_cfg_t *channelConfigPtr,
                              superio_pwm_state_t *state);
/**
 * @brief Deinitialize a superio channel.
 * @details The function will reset the timer and pin of the selected superio channel.
 *
 * @param state    The pwm state pointer.
 * @return SUPERIO_PWM_STATUS_SUCCESS - if the deinitialization was succeed.
 */
status_t SUPERIO_PWM_DRV_Deinit(superio_pwm_state_t *state);

/**
 * @brief Get freqency of the SuperIO_CLK.
 * @param instance    The superIO instance.
 * @return frequency - if the CLOCK_SYS_GetFreq was return succeed.
 */
uint32_t SUPERIO_PWM_DRV_GetClkFreq(uint32_t instance);

/**
 * @brief Set clock prescaler for a superio channel.
 * @details The function will change the prescaler value for the selected superio channel.
 *
 * @param state         The Superio state.
 * @param Prescaler     The new prescaler value.
 * @return SUPERIO_PWM_STATUS_SUCCESS - if the initialization was succeed.
 */
status_t SUPERIO_PWM_DRV_UpdateClockPrescaler(superio_pwm_state_t *state, superio_pwm_decrement_t Prescaler);

/**
 * @brief Set a new value for duty cycle and period of the channel.
 * @details The function will update the selected superio channel with the new values for the duty cycle and period.
 *
 * @param state    The pwm state pointer.
 * @param Period        The new value for the period.
 * @param DutyCycle     The new value for the duty cycle.
 * @param Polarity     The new value for the Polarity.
 * @return SUPERIO_PWM_STATUS_SUCCESS - if the initialization was succeed.
 */
status_t SUPERIO_PWM_DRV_UpdatePeriodDuty(superio_pwm_state_t *state, uint16_t Period, uint16_t DutyCycle,
                                          superio_pwm_PolarityType_t Polarity);

/**
 * @brief Get the logic level of the channel ouput.
 * @details The function will return the logic level that the selected superio channel is driving on the output pin.
 *
 * @param state    The pwm state pointer.
 * @return SUPERIO_PWM_STATUS_SUCCESS - if the initialization was succeed.
 */
bool SUPERIO_PWM_DRV_GetOutputState(superio_pwm_state_t *state);

/**
 * @brief Getting the period for a channel.
 * @details The function will get the period on the selected channel.
 *
 * @param   state The pwm state pointer.

 * @return uint16_t the period value.
 */
uint16_t SUPERIO_PWM_DRV_GetPeriod(superio_pwm_state_t *state);

/**
 * @brief  Initialize a dummy superio channel, just to occupied a resourceId.
 *
 * @param  instance The Superio instance id.
 * @param  channelConfigPtr The channel configuration for the selected Superio instance.
 * @param  state The pwm state pointer.
 * @return status_t Status return code.
 */
status_t SUPERIO_PWM_DRV_Init_Dummy(uint32_t instance, const superio_pwm_channel_cfg_t *channelConfigPtr,
                                    superio_pwm_state_t *state);

/**
 * @brief Deinitialize a dummy superio channel of occupied resourceId.
 *
 * @param  state The pwm state pointer.
 * @return status_t Status return code.
 */
status_t SUPERIO_PWM_DRV_Deinit_Dummy(superio_pwm_state_t *state);

/**
 * @brief Update the pwm interrupt.
 *
 * @param  state  The pwm state pointer.
 * @param  irqEnable Enable or not.
 */
void SUPERIO_PWM_DRV_UpdateInterrupt(superio_pwm_state_t *state, bool irqEnable);

#ifdef __cplusplus
}
#endif

/**
 * @brief Enter/exit stop mode according the TCSR
 * 
 * @param  state The pwm state pointer.
 * @param  en enable or not
 */
void SUPERIO_DRV_TcsrStopZoneMode(superio_pwm_state_t *state, bool en);

/**
 * @brief Check stop ack according the TCSR
 * 
 * @param  state The pwm state pointer.
 * @return true 
 * @return false 
 */
bool SUPERIO_PWM_DRV_CheckTcsrStopACK(superio_pwm_state_t *state);

/**
 * @}
 */
/* end of group SUPERIO_PWM_Driver */

/**
 * @}
 */
/* end of group Peripheral_Driver_layer */

#endif /* __SUPERIO_PWM_DRIVER_H__ */
