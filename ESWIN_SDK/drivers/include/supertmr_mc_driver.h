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
 * @file supertmr_mc_driver.h
 * @brief SUPERTMR counter driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __SUPERTMR_MC_DRIVER_H__
#define __SUPERTMR_MC_DRIVER_H__

#include "supertmr_common.h"

/**
 * @brief The configuration structure in timer mode.
 */
typedef struct {
    supertmr_config_mode_t mode; /**< SUPERTMR mode. \D{SUPERTMR_MODE_UP_TIMER} */
    uint16_t initialValue;       /**< Initial counter value. \D{0} */
    uint16_t finalValue;         /**< Final counter value. \D{31250} */
} supertmr_timer_param_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Initialize the SUPERTMR counter.
 *
 * Starts the SUPERTMR counter. This function provides access to the
 * SUPERTMR counter settings. The counter can be run in Up counting and Up-down counting modes.
 * To run the counter in Free running mode, choose Up counting option and provide
 * 0x0 value for the initialValue and 0xFFFF for finalValue. Please call this
 * function only when SUPERTMR is used as timer/counter. User must call the SUPERTMR_DRV_Deinit
 * and the SUPERTMR_DRV_Init to Re-Initialize the SUPERTMR before calling SUPERTMR_DRV_InitCounter
 * for the second time and afterwards.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  timer Timer configuration structure.
 * @return status_t Operation status
 * STATUS_SUCCESS : Initialized successfully.
 */
status_t SUPERTMR_DRV_InitCounter(uint32_t instance, const supertmr_timer_param_t *timer);

/**
 * @brief Starts the SUPERTMR counter.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @return status_t Operation status
 * STATUS_SUCCESS : Completed successfully.
 * STATUS_ERROR : Error occurred.
 */
status_t SUPERTMR_DRV_CounterStart(uint32_t instance);

/**
 * @brief Stops the SUPERTMR counter.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @return status_t Operation status
 * STATUS_SUCCESS : Completed successfully.
 */
status_t SUPERTMR_DRV_CounterStop(uint32_t instance);

/**
 * @brief Reads back the current value of the SUPERTMR counter.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @return uint32_t The current counter value.
 */
uint32_t SUPERTMR_DRV_CounterRead(uint32_t instance);

/**
 * @brief This function will get the default configuration values
 *        in the structure which is used as a common use-case.
 *
 * @param  config Pointer to the structure in which the configuration will be saved.
 */
void SUPERTMR_MC_DRV_GetDefaultConfig(supertmr_timer_param_t *const config);

#if defined(__cplusplus)
}
#endif

#endif // __SUPERTMR_MC_DRIVER_H__
