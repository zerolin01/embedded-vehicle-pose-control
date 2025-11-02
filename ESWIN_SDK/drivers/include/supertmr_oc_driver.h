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
 * @file supertmr_oc_driver.h
 * @brief SUPERTMR out compare driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __SUPERTMR_OC_DRIVER_H__
#define __SUPERTMR_OC_DRIVER_H__

#include "supertmr_common.h"

/**
 * @brief SuperTimer Mode configuration for output compare mode.
 */
typedef enum {
    SUPERTMR_DISABLE_OUTPUT  = 0x00U, /**< No action on output pin. */
    SUPERTMR_TOGGLE_ON_MATCH = 0x01U, /**< Toggle on match. */
    SUPERTMR_CLEAR_ON_MATCH  = 0x02U, /**< Clear on match. */
    SUPERTMR_SET_ON_MATCH    = 0x03U  /**< Set on match. */
} supertmr_output_compare_mode_t;

/**
 * @brief SuperTimer input capture type of the next output compare value.
 */
typedef enum {
    SUPERTMR_RELATIVE_VALUE = 0x00U, /**< Next compared value is relative to current value. */
    SUPERTMR_ABSOLUTE_VALUE = 0x01U  /**< Next compared value is absolute. */
} supertmr_output_compare_update_t;

/**
 * @brief SuperTimer driver PWM parameters each channel in the output compare mode.
 */
typedef struct {
    uint8_t hwChannelId;                   /**< Physical hardware channel ID. */
    supertmr_output_compare_mode_t chMode; /**< Channel output mode. */
    uint16_t comparedValue;                /**< The compared value. */
    bool enableExternalTrigger;            /**< true: enable the generation of a trigger is used for on-chip modules.
                                            *   false: disable the generation of a trigger. */
} supertmr_output_cmp_ch_param_t;

/**
 * @brief SuperTimer driver PWM parameters which is configured for the list of channels.
 */
typedef struct {
    uint8_t nNumOutputChannels;                          /**< Number of output compare channels. */
    supertmr_config_mode_t mode;                         /**< SuperTimer PWM operation mode. */
    uint16_t maxCountValue;                              /**< Maximum count value in ticks. */
    supertmr_output_cmp_ch_param_t *outputChannelConfig; /**< Output compare channels configuration. */
} supertmr_output_cmp_param_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Configures the SUPERTMR to generate timed pulses (Output compare mode).
 *
 * When the SUPERTMR counter matches the value of CnV, the channel output is changed based on what is
 * specified in the mode argument. The signal period can be modified using
 * param->maxCountValue. After this function when the max counter value and CnV are equal.
 * SUPERTMR_DRV_UpdateOutputCompareChannel function can be used to change CnV value.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  param Configuration of the output compare channels.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t SUPERTMR_DRV_InitOutputCompare(uint32_t instance, const supertmr_output_cmp_param_t *param);

/**
 * @brief Disables compare match output control and clears SUPERTMR timer configuration.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  param Configuration of the output compare channel.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t SUPERTMR_DRV_DeinitOutputCompare(uint32_t instance, const supertmr_output_cmp_param_t *param);

/**
 * @brief Sets the next compare match value based on the current counter value.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  channel Configuration of the output compare channel.
 * @param  nextComparematchValue Timer value in ticks until the next compare match event should appear.
 * @param  update
 * SUPERTMR_RELATIVE_VALUE : nextComparematchValue will be added to current counter value.
 * SUPERTMR_ABSOLUTE_VALUE : nextComparematchValue will be written in counter register as it is.
 * @param  softwareTrigger This parameter will be true if software trigger sync is enabled and
 * the user want to generate a software trigger (the value from buffer will be moved to register immediate or
 * at next loading point depending on the sync configuration). Otherwise this parameter must be false
 * and the next compared value will be stored in buffer until a trigger signal will be received.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t SUPERTMR_DRV_UpdateOutputCompareChannel(uint32_t instance, uint8_t channel, uint16_t nextComparematchValue,
                                                 supertmr_output_compare_update_t update, bool softwareTrigger);

#if defined(__cplusplus)
}
#endif

#endif // __SUPERTMR_OC_DRIVER_H__
