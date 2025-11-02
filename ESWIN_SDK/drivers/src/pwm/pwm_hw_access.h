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
 * @file pwm_hw_access.h
 * @brief PWM driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __PWM_HW_ACCESS_H__
#define __PWM_HW_ACCESS_H__

#include "pwm_register.h"
#include "status.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief PWM clk source configuration.
 */
typedef enum {
    CLK_OFF           = 0U, /**< Clock is off. */
    IPG_CLK           = 1U, /**< IP Global clock. */
    IPG_CLK_HIGH_FREQ = 2U, /**< IP Global clock high freq */
    IPG_CLK_32K       = 3U, /**< IP Global clock 32k */
} pwm_clk_src_cfg_t;

/**
 * @brief PWM output configuration.
 */
typedef enum {
    SET_RV_CL_CMP = 0U, /**< Output pin is set at roll over and cleaned at comparision. */
    CL_RV_SET_CMP = 1U, /**< Output pin is cleared at roll over and set at comparision. */
    DISCONT       = 2U, /**< Output is disconnected */
    // DISCONT = 3U,    /**< Output is disconnected */
} pwm_output_cfg_t;

/**
 * @brief PWM sample repeat configuration.
 */
typedef enum {
    REPEAT_ONCE  = 0U, /**< Output pin is set at roll over and cleaned at comparision. */
    REPEAT_TWICE = 1U, /**< Output pin is cleared at roll over and set at comparision. */
    REPEAT_FOUR  = 2U, /**< Output is disconnected */
    REPEAT_EIGHT = 3U, /**< Output is disconnected */
} pwm_sample_repeat_t;

/**
 * @brief Active in different mode.
 */
typedef enum {
    STOP_EN = PWM_PWMCR_STOPEN_SHIFT, /**< Keeps the PWM functional while in stop mode. */
    DOZ_EN  = PWM_PWMCR_DOZEN_SHIFT,  /**< Keeps the PWM functional in doze mode. */
    WAIT_EN = PWM_PWMCR_WAITEN_SHIFT, /**< Keeps the PWM functional in wait mode. */
    DBG_EN  = PWM_PWMCR_DBGEN_SHIFT,  /**< Keeps the PWM functional in debug mode */
} pwm_active_mode_t;

/**
 * @brief PWM status.
 */
typedef enum {
    FE  = PWM_PWMSR_FE_SHIFT,  /**< FIFO empty status */
    ROV = PWM_PWMSR_ROV_SHIFT, /**< Roll-over status */
    CMP = PWM_PWMSR_CMP_SHIFT, /**< Compare Status */
    FWE = PWM_PWMSR_FWE_SHIFT, /**< FIFO write error status */
} pwm_status_flag_t;

/**
 * @brief Interrupt Enable.
 */
typedef enum {
    FIE = PWM_PWMIR_FIE_SHIFT, /**< FIFO empty interrupt enable */
    RIE = PWM_PWMIR_RIE_SHIFT, /**< Roll-over interrupt enable */
    CIE = PWM_PWMIR_CIE_SHIFT, /**< Compare interrupt enable */
} pwm_int_en_t;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Resets the PWM internal logic and registers to their default settings.
 *
 * This function first performs a software reset of the PWM module which resets the
 * internal PWM logic and most registers, then proceeds to manually reset all of the
 * PWM registers to their default setting to ensuring these registers at programmed to
 * their default value which includes disabling the module.
 *
 * @param base base pointer of type pwm_type_t.
 */
void PWM_Init(pwm_type_t *base);

/**
 * @brief Enable/Disable the PWM module.
 *
 * @param base Module base pointer of type pwm_type_t.
 * @param  enable enable/disable
 */
void PWM_Enable(pwm_type_t *base, bool enable);

/**
 * @brief Get PWM module status.
 *
 * @param base Module base pointer of type pwm_type_t.
 * @return PWM module status
 */
uint32_t PWM_GetEnable(pwm_type_t *base);

/**
 * @brief Configure PWM output polarity.
 *
 * @param base Module base pointer of type pwm_type_t.
 * @param pwmOutputCfg Output configure
 * @return if success, it returns STATUS_SUCCESS. if error, it returns STATUS_ERROR
 */
status_t PWM_OutputPolarityCfg(pwm_type_t *base, pwm_output_cfg_t pwmOutputCfg);

/**
 * @brief Configure PWM clock source.
 *
 * @param base Module base pointer of type pwm_type_t.
 * @param pwmClkSrcCfg CLock source
 * @return if success, it returns STATUS_SUCCESS. if error, it returns STATUS_ERROR
 */
status_t PWM_SetClkSrc(pwm_type_t *base, pwm_clk_src_cfg_t pwmClkSrcCfg);

/**
 * @brief Set PWM active in different Mode.
 *
 * @param base Module base pointer of type pwm_type_t.
 * @param pwmActiveMode Active mode
 * @param enable enable/disable
 */
void PWM_SetActive(pwm_type_t *base, pwm_active_mode_t pwmActiveMode, bool enable);

/**
 * @brief Configure PWM prescaler.
 *
 * @param base Module base pointer of type pwm_type_t.
 * @param div prescaler ratio (max 0xfff)
 * @return if success, it returns STATUS_SUCCESS. if error, it returns STATUS_ERROR
 */
status_t PWM_SetPreScaler(pwm_type_t *base, uint32_t div);

/**
 * @brief Configure FIFO repeat.
 *
 * @param base Module base pointer of type pwm_type_t.
 * @param repeatTimes repeat times (max 0xfff)
 * @return if success, it returns STATUS_SUCCESS. if error, it returns STATUS_ERROR
 */
status_t PWM_CfgFifoRepeat(pwm_type_t *base, pwm_sample_repeat_t repeatTimes);

/**
 * @brief Set FIFO Water Mark.
 *
 * @param base Module base pointer of type pwm_type_t.
 * @param waterMark FIFO empty flag is set when there are more than waterMark+1 empty in FIFO
 * @return if success, it returns STATUS_SUCCESS. if error, it returns STATUS_ERROR
 */
status_t PWM_SetFifoWaterMark(pwm_type_t *base, uint8_t waterMark);

/**
 * @brief Gets the PWM status flag state.
 *
 * This function returns the state of one of the PWM status flags as requested by
 * the user.
 *
 * @param base Module base pointer of type pwm_type_t.
 * @param statusFlag The status flag, of type pwm_status_flag_t
 * @return State of the status flag: asserted (true) or not-asserted (false)
 */
bool PWM_GetStatusFlag(const pwm_type_t *base, pwm_status_flag_t statusFlag);

/**
 * @brief Clear the PWM status flag state.
 *
 * This function returns the state of one of the PWM status flags as requested by
 * the user.
 *
 * @param base Module base pointer of type pwm_type_t.
 * @param statusFlag The status flag, of type pwm_status_flag_t
 */
void PWM_ClearStatusFlag(pwm_type_t *base, pwm_status_flag_t statusFlag);

/**
 * @brief Gets the PWM FIFO data number.
 *
 * This function returns the number of data in FIFO
 *
 * @param base Module base pointer of type pwm_type_t.
 * @return number of data in FIFO
 */
uint8_t PWM_GetFifoDataNum(const pwm_type_t *base);

/**
 * @brief Enable or Disable PWM interrupt.
 * 
 * @param  base Module base pointer of type pwm_type_t.
 * @param  pwmIntEn Interrupt bit, of type pwm_int_en_t.
 * @param  enable enable/disable
 * @return status_t if success, it returns STATUS_SUCCESS. if error, it returns STATUS_ERROR
 */
status_t PWM_SetIntMode(pwm_type_t *base, pwm_int_en_t pwmIntEn, bool enable);

/**
 * @brief Gets the PWM interrupt mode.
 *
 * This function returns the state of one of the PWM interrupt flags as requested by
 * the user.
 *
 * @param base Module base pointer of type pwm_type_t.
 * @param pwmIntEn The Interrupt bit, of type pwm_int_en_t
 * @return State of the interrupt flag: asserted (true) or not-asserted (false)
 */
bool PWM_GetIntMode(const pwm_type_t *base, pwm_int_en_t pwmIntEn);

/**
 * @brief Set sample register.
 *
 * @param base Module base pointer of type pwm_type_t.
 * @param sample sample write to the FIFO(0xffff)
 * @return if success, it returns STATUS_SUCCESS. if error, it returns STATUS_ERROR
 */
status_t PWM_SetSample(pwm_type_t *base, uint32_t sample);

/**
 * @brief Get sample register.
 *
 * @param base Module base pointer of type pwm_type_t.
 * @return Sample currently used
 */
uint32_t PWM_GetSample(const pwm_type_t *base);

/**
 * @brief Set Period register.
 *
 * @param base Module base pointer of type pwm_type_t.
 * @param period period of PWM(0xffff)
 * @return if success, it returns STATUS_SUCCESS. if error, it returns STATUS_ERROR
 */
status_t PWM_SetPeriod(pwm_type_t *base, uint32_t period);

/**
 * @brief Get Period of PWM.
 *
 * @param base Module base pointer of type pwm_type_t.
 * @return period of PWM
 */
uint32_t PWM_GetPeriod(const pwm_type_t *base);

/**
 * @brief Get counter register.
 *
 * @param base Module base pointer of type pwm_type_t.
 * @return period of PWM
 */
uint32_t PWM_GetCounter(const pwm_type_t *base);

#if defined(__cplusplus)
}
#endif

#endif /* __PWM_HW_ACCESS_H__ */
