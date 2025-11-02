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
 * @file supertmr_pwm_driver.h
 * @brief SUPERTMR pwm driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __SUPERTMR_PWM_DRIVER_H__
#define __SUPERTMR_PWM_DRIVER_H__

#include "supertmr_common.h"

/**
 * @brief Maximum value for PWM duty cycle.
 */
#define SUPERTMR_MAX_DUTY_CYCLE (0x8000U)

/**
 * @brief Shift value which converts duty to ticks.
 *
 */
#define SUPERTMR_DUTY_TO_TICKS_SHIFT (15U)

/**
 * @brief SuperTimer Configure type of PWM update in the duty cycle or in ticks.
 */
typedef enum {
    SUPERTMR_PWM_UPDATE_IN_DUTY_CYCLE = 0x00U, /**< The type of PWM update in the duty cycle/pulse or also use in frequency update. */
    SUPERTMR_PWM_UPDATE_IN_TICKS      = 0x01U  /**< The type of PWM update in ticks. */
} supertmr_pwm_update_option_t;

/**
 * @brief The polarity of the channel output is configured in PWM signal.
 */
typedef enum {
    SUPERTMR_POLARITY_LOW  = 0x00U, /**< The channel polarity is active LOW which is defined again. */
    SUPERTMR_POLARITY_HIGH = 0x01U  /**< The channel polarity is active HIGH which is defined again. */
} supertmr_polarity_t;

/**
 * @brief SuperTimer PWM channel (n+1) polarity for combine mode.
 */
typedef enum {
    SUPERTMR_MAIN_INVERTED   = 0x01U, /**< The channel (n+1) output is the inverse of the channel (n) output. */
    SUPERTMR_MAIN_DUPLICATED = 0x00U  /**< The channel (n+1) output is the same as the channel (n) output. */
} supertmr_second_channel_polarity_t;

/**
 * @brief SuperTimer fault control.
 */
typedef enum {
    SUPERTMR_FAULT_CONTROL_DISABLED = 0x00U, /**< Fault control is disabled for all channels. */
    SUPERTMR_FAULT_CONTROL_MAN_EVEN = 0x01U, /**< Fault control is enabled for even channels.
                                              *   only (channels 0, 2, 4, and 6), and the selected
                                              *   mode is the manual fault clearing. */
    SUPERTMR_FAULT_CONTROL_MAN_ALL  = 0x02U, /**< Fault control is enabled for all channels,
                                              *   and the selected mode is the manual fault clearing. */
    SUPERTMR_FAULT_CONTROL_AUTO_ALL = 0x03U  /**< Fault control is enabled for all channels, and
                                              *   the selected mode is the automatic fault clearing. */
} supertmr_fault_mode_t;

/**
 * @brief Select level of the channel (n) output at the beginning.
 */
typedef enum {
    SUPERTMR_LOW_STATE  = 0x00U, /**< When fault is detected PWM channel is low. */
    SUPERTMR_HIGH_STATE = 0x01U  /**< When fault is detected PWM channel is high. */
} supertmr_safe_state_polarity_t;

/**
 * @brief SuperTimer driver PWM Fault channel parameters.
 */
typedef struct {
    bool faultChannelEnabled;                     /**< Fault channel state. \D{false} */
    bool faultFilterEnabled;                      /**< Fault channel filter state. \D{false} */
    supertmr_polarity_t supertmrFaultPinPolarity; /**< Channel output state on fault. \D{SUPERTMR_POLARITY_LOW} */
} supertmr_pwm_ch_fault_param_t;

/**
 * @brief SuperTimer driver PWM Fault parameter.
 */
typedef struct {
    bool pwmOutputStateOnFault;                                                       /**< Output pin state on fault (safe state or tri-state). \D{false} */
    bool pwmFaultInterrupt;                                                           /**< PWM fault interrupt state. \D{false} */
    uint8_t faultFilterValue;                                                         /**< Fault filter value. \D{0U} */
    supertmr_fault_mode_t faultMode;                                                  /**< Fault mode. \D{SUPERTMR_FAULT_CONTROL_DISABLED} */
    supertmr_pwm_ch_fault_param_t supertmrFaultChannelParam[SUPERTMR_FAULT_CHANNELS]; /**< Fault channels configuration. */
} supertmr_pwm_fault_param_t;

/**
 * @brief SuperTimer driver independent PWM parameter.
 */
typedef struct {
    uint8_t hwChannelId;                                      /**< Physical hardware channel ID. \D{0} */
    supertmr_polarity_t polarity;                             /**< Polarity of the PWM signal generated on MCU pin. \D{SUPERTMR_POLARITY_HIGH} */
    uint16_t uDutyCyclePercent;                               /**< PWM pulse width, value should be between
                                                                *   0 (0%) to SUPERTMR_MAX_DUTY_CYCLE (100%). \R{0,0x8000} \D{0x4000} */
    bool enableExternalTrigger;                               /**< true: enable the generation of a trigger is used for on-chip modules.
                                                                *   false: disable the generation of a trigger. \D{true} */
    supertmr_safe_state_polarity_t safeState;                 /**< Logical state of the PWM channel n when an fault is detected
                                                                *   and to set up the polarity of PWM signal on the channel (n+1). \D{SUPERTMR_LOW_STATE} */
    bool enableSecondChannelOutput;                           /**< Enable complementary mode on next channel. \D{false} */
    supertmr_second_channel_polarity_t secondChannelPolarity; /**< Polarity of the channel n+1 relative to channel n in
                                                                 the complementary mode. \D{SUPERTMR_MAIN_INVERTED} */
    bool deadTime;                                            /**< Enable/disable dead time for channel. \D{false} */
} supertmr_independent_ch_param_t;

/**
 * @brief SuperTimer driver combined PWM parameter.
 */
typedef struct {
    uint8_t hwChannelId;                                      /**< Physical hardware channel ID for channel (n). \D{0} */
    uint16_t firstEdge;                                       /**< First edge time. This time is relative to signal period. The value for this parameter is
                                                                *   between 0 and SUPERTMR_MAX_DUTY_CYCLE(0 = 0% from period and SUPERTMR_MAX_DUTY_CYCLE = 100%
                                                                * from period). \D{0} */
    uint16_t secondEdge;                                      /**< Second edge time. This time is relative to signal period. The value for this parameter is
                                                                *   between 0 and SUPERTMR_MAX_DUTY_CYCLE(0 = 0% from period and SUPERTMR_MAX_DUTY_CYCLE =
                                                                * 100% from period). \D{0x3000} */
    bool deadTime;                                            /**< Enable/disable dead time for channel. \D{false} */
    bool enableModifiedCombine;                               /**< Enable/disable the modified combine mode for channels (n) and (n+1) \D{true} */
    supertmr_polarity_t mainChannelPolarity;                  /**< Polarity of the PWM signal generated on MCU pin for channel n. \D{SUPERTMR_POLARITY_HIGH} */
    bool enableSecondChannelOutput;                           /**< Select if channel (n+1)  output is enabled/disabled for the complementary mode. \D{false} */
    supertmr_second_channel_polarity_t secondChannelPolarity; /**< Select channel (n+1) polarity relative to channel (n) in the complementary mode. \D{SUPERTMR_MAIN_INVERTED} */
    bool enableExternalTrigger;                               /**< The generation of the channel (n) trigger
                                                                *   true: enable the generation of a trigger on the channel (n).
                                                                *   false: disable the generation of a trigger on the channel (n). \D{false} */
    bool enableExternalTriggerOnNextChn;                      /**< The generation of the channel (n+1) trigger
                                                                *   true: enable the generation of a trigger on the channel (n+1).
                                                                *   false: disable the generation of a trigger on the channel (n+1). \D{false}
                                                                */
    supertmr_safe_state_polarity_t mainChannelSafeState;      /**< The selection of the channel (n) state when fault is detected. \D{SUPERTMR_LOW_STATE} */
    supertmr_safe_state_polarity_t secondChannelSafeState;    /**< The selection of the channel (n+1) state when fault is detected and set up the polarity of PWM signal on the
                                                                * channel (n+1). \D{SUPERTMR_LOW_STATE} */
} supertmr_combined_ch_param_t;

/**
 * @brief SuperTimer driver PWM parameters.
 */
typedef struct {
    uint8_t nNumIndependentPwmChannels;                           /**< Number of independent PWM channels. \D{0U} */
    uint8_t nNumCombinedPwmChannels;                              /**< Number of combined PWM channels. */
    supertmr_config_mode_t mode;                                  /**< SUPERTMR mode. \D{SUPERTMR_MODE_EDGE_ALIGNED_PWM} */
    uint8_t deadTimeValue;                                        /**< Dead time value in [ticks]. \D{48U} */
    supertmr_deadtime_ps_t deadTimePrescaler;                     /**< Dead time pre-scaler value[ticks]. \D{SUPERTMR_DEADTIME_DIVID_BY_4} */
    uint32_t uFrequencyHZ;                                        /**< PWM period in Hz. \D{10000U} */
    supertmr_independent_ch_param_t *pwmIndependentChannelConfig; /**< Configuration for independent PWM channels. \D{NULL} */
    supertmr_combined_ch_param_t *pwmCombinedChannelConfig;       /**< Configuration for combined PWM channels. */
    supertmr_pwm_fault_param_t *faultConfig;                      /**< Configuration for PWM fault. */
} supertmr_pwm_param_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Stops all PWM channels.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @return status_t Counter the current counter value.
 */
status_t SUPERTMR_DRV_DeinitPwm(uint32_t instance);

/**
 * @brief Configures the duty cycle and frequency and starts the output of the PWM on
 * all channels configured in the param structure.
 * The independent channel configuration need to clarify the polarity and safe state as following:
 * - In the first channel, the POL bit is the value of safeState variable. In the second channel,
 * the POL bit is the same value of safeSate with the inverted channel and the POL bit is inverted
 * safeState with the duplicated channel.
 * - If the polarity and sate state are the value, it will be Low-true pulses. It means the ELSB:ELSA = 0:1.
 * Otherwise, it will be High-true pulses. It means the ELSB:ELSA = 1:0.
 * Regarding the combined channel configuration:
 * - In both channels, the POL bit is the same value with the safeState variable
 * - If the polarity and sate state are the value, it will be Low-true pulses. It means the ELSB:ELSA = 0:1.
 * Otherwise, it will be High-true pulses. It means the ELSB:ELSA = 1:0.
 * - COMP bit will be true when the polarity and safeState are the same value, the second channel is inverted
 *.the first channel or when the polarity and safeState are difference value, the second channel is duplicated the first
 *channel.
 * - COMP bit will be false when the polarity and safeState are the same value, the second channel is duplicated
 *.the first channel or when the polarity and safeState are difference value, the second channel is inverted the first
 *channel.
 *
 * @note: These configuration will impact to the SUPERTMR_DRV_SetSoftwareOutputChannelControl and
 *SUPERTMR_DRV_SetAllChnSoftwareOutputControl function. Because the software output control behavior depends on the
 *polarity and COMP bit.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  param SUPERTMR driver PWM parameter to configure PWM options.
 * @return status_t
 * STATUS_SUCCESS : Completed successfully.
 * STATUS_ERROR : Error occurred.
 */
status_t SUPERTMR_DRV_InitPwm(uint32_t instance, const supertmr_pwm_param_t *param);

/**
 * @brief This function updates the waveform output in PWM mode (duty cycle and phase).
 *
 * @note: Regarding the type of updating PWM in the duty cycle, if the expected duty
 * is 100% then the value that is to be written to hardware will be exceed value of period.
 * It means that the SUPERTMR counter will not match the value of the CnV register in this case.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  channel The channel number. In combined mode, the code finds the channel.
 * @param  typeOfUpdate The type of PWM update in the duty cycle/pulse or in ticks.
 * @param  firstEdge Duty cycle or first edge time for PWM mode. Can take value between
 *                      0 - SUPERTMR_MAX_DUTY_CYCLE(0 = 0% from period  and SUPERTMR_MAX_DUTY_CYCLE = 100% from period)
 *                      Or value in ticks for the first of the PWM mode in which can have value between 0
 *                      and supertmrPeriod is stored in the state structure.
 * @param  secondEdge Second edge time - only for combined mode. Can take value between 0 - SUPERTMR_MAX_DUTY_CYCLE(0 =
 *                      0% from period  and SUPERTMR_MAX_DUTY_CYCLE = 100% from period). Or value in ticks for the
 *                      second of the PWM mode in which can have value between 0 and supertmrPeriod is stored in the
 *                      state structure.
 * @param  softwareTrigger If true a software trigger is generate to update PWM parameters.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t SUPERTMR_DRV_UpdatePwmChannel(uint32_t instance, uint8_t channel, supertmr_pwm_update_option_t typeOfUpdate,
                                       uint16_t firstEdge, uint16_t secondEdge, bool softwareTrigger);

/**
 * @brief This function will update the duty cycle of PWM output for multiple channels.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  numberOfChannels The number of channels which should be updated.
 * @param  channels The list of channels which should be updated.
 * @param  duty The list of duty cycles for selected channels.
 * @param  softwareTrigger If true a software trigger is generate to update PWM parameters.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t SUPERTMR_DRV_FastUpdatePwmChannels(uint32_t instance, uint8_t numberOfChannels, const uint8_t *channels,
                                            const uint16_t *duty, bool softwareTrigger);

/**
 * @brief This function will update the new period in the frequency or
 * in the counter value into mode register which modify the period of PWM signal
 * on the channel output.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  typeOfUpdate The type of PWM update is a period in Hz or in ticks.
 *                          - For SUPERTMR_PWM_UPDATE_IN_DUTY_CYCLE which reuse in SUPERTMR_DRV_UpdatePwmChannel.
 * function will update in Hz.
 *                          - For SUPERTMR_PWM_UPDATE_IN_TICKS will update in ticks.
 * @param  newValue The frequency or the counter value which will select with modified value for PWM signal.
 *                      If the type of update in the duty cycle, the newValue parameter must be value
 *                      between 1U and maximum is the frequency of the SUPERTMR counter.
 *                      If the type of update in ticks, the newValue parameter must be value between 1U and 0xFFFFU.
 * @param  softwareTrigger If true a software trigger is generate to update PWM parameters.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t SUPERTMR_DRV_UpdatePwmPeriod(uint32_t instance, supertmr_pwm_update_option_t typeOfUpdate, uint32_t newValue,
                                      bool softwareTrigger);

/**
 * @brief This function is used to control the final logic of the channel output.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  channel The channel which is used in PWM mode.
 * @param  enableChannelOutput Enable/disable the channel output.
 * @return status_t
 * STATUS_SUCCESS : Completed successfully.
 */
status_t SUPERTMR_DRV_ControlChannelOutput(uint32_t instance, uint8_t channel, bool enableChannelOutput);

/**
 * @brief This function will use in the PWM period dithering. This value
 * is added to an internal accumulator at the end of each PWM period. The value is
 * updated with its write buffer value according to the register synchronization.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  newModFracVal The modulo fractional value.
 * @param  softwareTrigger If true a software trigger is generate to update parameters.
 * @return status_t
 * STATUS_SUCCESS : Completed successfully.
 * STATUS_ERROR : Error occurred.
 */
status_t SUPERTMR_DRV_UpdatePwmPeriodDither(uint32_t instance, uint8_t newModFracVal, bool softwareTrigger);

/**
 * @brief This function will use in the PWM edge dithering. This value
 * is added to the channel (n) internal accumulator at the end of each PWM period.
 * The FRACVAL is updated with its write buffer value according to the register
 * synchronization. The PWM edge dithering is not available when the channel in the
 * input capture modes, and the channel in output compare modes.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  channel The channel number.
 * @param  newMatchFracVal The channel (n) match fractional value.
 * @param  softwareTrigger If true a software trigger is generate to update parameters.
 * @return status_t
 * STATUS_SUCCESS : Completed successfully.
 * STATUS_ERROR : Error occurred.
 */
status_t SUPERTMR_DRV_UpdatePwmEdgeChannelDither(uint32_t instance, uint8_t channel, uint8_t newMatchFracVal,
                                                 bool softwareTrigger);

/**
 * @brief Sets the SUPERTMR dead-time value for the channel pair.
 *
 * @param  instance The SUPERTMR base address pointer.
 * @param  channelPair The SUPERTMR peripheral channel pair (n).
 * @param  count The SUPERTMR peripheral selects the dead-time value
 * 0U : no counts inserted.
 * 1U : 1 count is inserted.
 * 2U : 2 count is inserted.
 * ... up to a possible 63 counts.
 */
void SUPERTMR_DRV_SetPairDeadtimeValue(uint32_t instance, uint8_t channelPair, uint8_t count);

#if defined(__cplusplus)
}
#endif

#endif // __SUPERTMR_PWM_DRIVER_H__
