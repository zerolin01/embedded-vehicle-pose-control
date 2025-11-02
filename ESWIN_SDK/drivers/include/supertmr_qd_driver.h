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
 * @file supertmr_qd_driver.h
 * @brief  SUPERTMR quadrature decode driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __SUPERTMR_QD_DRIVER_H__
#define __SUPERTMR_QD_DRIVER_H__

#include "supertmr_common.h"

/**
 * @brief SuperTimer quadrature decode modes, phase encode or count and direction mode.
 */
typedef enum {
    SUPERTMR_QUAD_PHASE_ENCODE  = 0x00U, /**< Phase encoding mode.                 */
    SUPERTMR_QUAD_COUNT_AND_DIR = 0x01U  /**< Counter and direction encoding mode. */
} supertmr_quad_decode_mode_t;

/**
 * @brief SuperTimer quadrature phase polarities, normal or inverted polarity.
 */
typedef enum {
    SUPERTMR_QUAD_PHASE_NORMAL = 0x00U, /**< Phase input signal is not inverted before identifying
                                         *   the rising and falling edges of this signal. */
    SUPERTMR_QUAD_PHASE_INVERT = 0x01U  /**< Phase input signal is inverted before identifying
                                         *   the rising and falling edges of this signal. */
} supertmr_quad_phase_polarity_t;

/**
 * @brief SuperTimer quadrature decoder channel parameters.
 */
typedef struct {
    bool phaseInputFilter;                        /**< false: disable phase filter,
                                                   *   true: enable phase filter. */
    uint8_t phaseFilterVal;                       /**< Filter value (if input filter is enabled). */
    supertmr_quad_phase_polarity_t phasePolarity; /**< Phase polarity. */
} supertmr_phase_params_t;

/**
 * @brief SUPERTMR quadrature configure structure.
 */
typedef struct {
    supertmr_quad_decode_mode_t mode;     /**< SUPERTMR_QUAD_PHASE_ENCODE or SUPERTMR_QUAD_COUNT_AND_DIR. */
    uint16_t initialVal;                  /**< Initial counter value. */
    uint16_t maxVal;                      /**< Maximum counter value. */
    supertmr_phase_params_t phaseAConfig; /**< Configuration for the input phase a. */
    supertmr_phase_params_t phaseBConfig; /**< Configuration for the input phase b. */
} supertmr_quad_decode_config_t;

/**
 * @brief SUPERTMR quadrature state(counter value and flags).
 */
typedef struct {
    uint16_t counter;       /**< Counter value. */
    bool overflowFlag;      /**< True if overflow occurred,
                             *   False if overflow doesn't occurred. */
    bool overflowDirection; /**< False if overflow occurred at minimum value,
                             *   True if overflow occurred at maximum value. */
    bool counterDirection;  /**< False SUPERTMR counter is decreasing,
                             *   True SUPERTMR counter is increasing. */
} supertmr_quad_decoder_state_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Configures the quadrature mode and starts measurement.
 *
 * @param  instance Instance number of the SUPERTMR module.
 * @param  config Configuration structure(quadrature decode mode, polarity for both phases,
 *                      initial and maximum value for the counter, filter configuration).
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t SUPERTMR_DRV_QuadDecodeStart(uint32_t instance, const supertmr_quad_decode_config_t *config);

/**
 * @brief De-activates the quadrature decode mode.
 *
 * @param  instance Instance number of the SUPERTMR module.
 * @return status_t
 * STATUS_SUCCESS: Completed successfully.
 * STATUS_ERROR: Error occurred.
 */
status_t SUPERTMR_DRV_QuadDecodeStop(uint32_t instance);

/**
 * @brief Return the current quadrature decoder state (counter value, overflow flag and
 * overflow direction).
 *
 * @param  instance Instance number of the SUPERTMR module.
 * @return supertmr_quad_decoder_state_t The current state of quadrature decoder.
 */
supertmr_quad_decoder_state_t SUPERTMR_DRV_QuadGetState(uint32_t instance);

/**
 * @brief This function will get the default configuration values
 *        in the structure which is used as a common use-case.
 *
 * @param  config Pointer to the structure in which the
 *                    configuration will be saved.
 */
void SUPERTMR_QD_DRV_GetDefaultConfig(supertmr_quad_decode_config_t *const config);

#if defined(__cplusplus)
}
#endif

#endif // __SUPERTMR_QD_DRIVER_H__
