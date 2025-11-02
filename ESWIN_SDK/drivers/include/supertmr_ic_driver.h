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
 * @file supertmr_ic_driver.h
 * @brief SUPERTMR input capture driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __SUPERTMR_IC_DRIVER_H__
#define __SUPERTMR_IC_DRIVER_H__

#include "supertmr_common.h"

/**
 * @brief Selects mode operation in the input capture.
 */
typedef enum {
    SUPERTMR_EDGE_DETECT        = 0U, /**< SUPERTMR edge detect. */
    SUPERTMR_SIGNAL_MEASUREMENT = 1U, /**< SUPERTMR signal measurement. */
    SUPERTMR_NO_OPERATION       = 2U  /**< SUPERTMR no operation. */
} supertmr_input_op_mode_t;

/**
 * @brief SuperTimer input capture measurement type for dual edge input capture.
 */
typedef enum {
    SUPERTMR_NO_MEASUREMENT                  = 0x00U, /**< No measurement. */
    SUPERTMR_RISING_EDGE_PERIOD_MEASUREMENT  = 0x01U, /**< Period measurement between two consecutive rising edges. */
    SUPERTMR_FALLING_EDGE_PERIOD_MEASUREMENT = 0x02U, /**< Period measurement between two consecutive falling edges. */
    SUPERTMR_PERIOD_ON_MEASUREMENT           = 0x03U, /**< The time measurement taken for the pulse to remain ON or HIGH state. */
    SUPERTMR_PERIOD_OFF_MEASUREMENT          = 0x04U  /**< The time measurement taken for the pulse to remain OFF or LOW state. */
} supertmr_signal_measurement_mode_t;

/**
 * @brief SuperTimer input capture edge mode as rising edge or falling edge.
 */
typedef enum {
    SUPERTMR_NO_PIN_CONTROL = 0x00U, /**< No trigger. */
    SUPERTMR_RISING_EDGE    = 0x01U, /**< Rising edge trigger. */
    SUPERTMR_FALLING_EDGE   = 0x02U, /**< Falling edge trigger. */
    SUPERTMR_BOTH_EDGES     = 0x03U  /**< Rising and falling edge trigger. */
} supertmr_edge_alignment_mode_t;

/**
 * @brief The measurement type for input capture mode.
 */
typedef enum {
    SUPERTMR_DISABLE_OPERATION           = 0x00U, /**< Have no operation. */
    SUPERTMR_TIMESTAMP_RISING_EDGE       = 0x01U, /**< Rising edge trigger. */
    SUPERTMR_TIMESTAMP_FALLING_EDGE      = 0x02U, /**< Falling edge trigger. */
    SUPERTMR_TIMESTAMP_BOTH_EDGES        = 0x03U, /**< Rising and falling edge trigger. */
    SUPERTMR_MEASURE_RISING_EDGE_PERIOD  = 0x04U, /**< Period measurement between two consecutive rising edges. */
    SUPERTMR_MEASURE_FALLING_EDGE_PERIOD = 0x05U, /**< Period measurement between two consecutive falling edges. */
    SUPERTMR_MEASURE_PULSE_HIGH          = 0x06U, /**< The time measurement taken for the pulse to remain ON or HIGH state. */
    SUPERTMR_MEASURE_PULSE_LOW           = 0x07U  /**< The time measurement taken for the pulse to remain OFF or LOW state. */
} supertmr_ic_op_mode_t;

/**
 * @brief SuperTimer driver Input capture parameters for each channel.
 */
typedef struct {
    uint8_t hwChannelId;                                /**< Physical hardware channel ID. \D{0U} */
    supertmr_input_op_mode_t inputMode;                 /**< SuperTimer module mode of operation. \D{SUPERTMR_EDGE_DETECT} */
    supertmr_edge_alignment_mode_t edgeAlignement;      /**< Edge alignment Mode for signal measurement. \D{SUPERTMR_RISING_EDGE} */
    supertmr_signal_measurement_mode_t measurementType; /**< Measurement Mode for signal measurement. \D{SUPERTMR_FALLING_EDGE_PERIOD_MEASUREMENT} */
    uint16_t filterValue;                               /**< Filter Value. \D{0U} */
    bool filterEn;                                      /**< Input capture filter state. \D{false} */
    bool continuousModeEn;                              /**< Continuous measurement state. \D{true} */
    void *channelsCallbacksParams;                      /**< The parameters of callback functions for channels events. *\D{NULL} */
    ic_callback_t channelsCallbacks;                    /**< The callback function for channels events. \D{NULL} */
} supertmr_input_ch_param_t;

/**
 * @brief SuperTimer driver input capture parameters.
 */
typedef struct {
    uint8_t nNumChannels;                     /**< Number of input capture channel used. \D{1U} */
    uint16_t nMaxCountValue;                  /**< Maximum counter value. Minimum value is 0 for this mode. \D{65535U} */
    supertmr_input_ch_param_t *inputChConfig; /**< Input capture channels configuration. */
} supertmr_input_param_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief This function configures the channel in the Input Capture mode for either getting
 * time-stamps on edge detection or on signal measurement. When the edge specified in the captureMode
 * argument occurs on the channel and then the SUPERTMR counter is captured into the CnV register.
 * The user have to read the CnV register separately to get this value. The filter
 * function is disabled if the filterVal argument passed as 0. The filter feature.
 * is available only on channels 0,1,2,3.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  param Configuration of the input capture channel.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t SUPERTMR_DRV_InitInputCapture(uint32_t instance, const supertmr_input_param_t *param);

/**
 * @brief Disables input capture mode and clears SUPERTMR timer configuration.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  param Configuration of the output compare channel.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t SUPERTMR_DRV_DeinitInputCapture(uint32_t instance, const supertmr_input_param_t *param);

/**
 * @brief This function is used to calculate the measurement and/or time stamps values
 * which are read from the C(n, n+1)V registers and stored to the static buffers.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  channel For getting the time stamp of the last edge (in normal input capture) this
 *                    parameter represents the channel number.
 *                    For getting the last measured value (in dual edge input capture) this parameter
 *                    is the lowest channel number of the pair (EX: 0, 2, 4, 6).
 * @return uint16_t The measured value.
 */
uint16_t SUPERTMR_DRV_GetInputCaptureMeasurement(uint32_t instance, uint8_t channel);

/**
 * @brief Starts new single-shot signal measurement of the given channel.
 *
 * @param  instance instance The SUPERTMR peripheral instance number.
 * @param  channel Configuration of the output compare channel.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t SUPERTMR_DRV_StartNewSignalMeasurement(uint32_t instance, uint8_t channel);

/**
 * @brief Set mode operation for channel in the input capture mode
 *
 * This function will change the channel mode at run time or
 * when stopping channel. The channel mode is selected in the supertmr_ic_op_mode_t
 * enumeration type.
 *
 * @param  instance The input capture instance number.
 * @param  channel The channel number.
 * @param  inputMode The channel operation mode.
 * @param  enableContinuousCapture Enable/disable the continuous capture mode.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 */
status_t SUPERTMR_IC_DRV_SetChannelMode(uint32_t instance, uint8_t channel, supertmr_ic_op_mode_t inputMode,
                                        bool enableContinuousCapture);
/**
 * @brief Set hall mode
 *
 * This function will set supertmr support hall mode
 * Set SUPERTMR2CH1SEL bit to XOR SUPERTMR2_CH0, SUPERTMR2_CH1 and SUPERTMR1_CH1 to one single SUPERTMR2_CH1 input
 *
 * @param  enable true-hall mode, false-normal mode
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 */
status_t SUPERTMR_IC_DRV_SetHallMode(bool enable);

#if defined(__cplusplus)
}
#endif

#endif // __SUPERTMR_IC_DRIVER_H__
