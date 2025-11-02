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
 * @file pwm_hw_access.c
 * @brief PWM driver hardware access source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "pwm_hw_access.h"

/*
 * Function Name : PWM_Init
 * Description   : This function first performs a software reset of the PWM module which resets the
 * internal PWM logic and most registers, then proceeds to manually reset all of the
 * PWM registers to their default setting to ensuring these registers at programmed to
 * their default value which includes disabling the module.
 *
 */
void PWM_Init(pwm_type_t *base)
{
    base->PWMCR = PWM_PWMCR_SWR_MASK;
    base->PWMCR = (uint32_t)0x0;
    return;
}

/*
 * Function Name : PWM_Enable
 * Description   : Enable/Disable the PWM module.
 *
 */
void PWM_Enable(pwm_type_t *base, bool enable)
{
    uint32_t tmp = (base->PWMCR) & (~PWM_PWMCR_EN_MASK);
    tmp |= (uint32_t)enable << PWM_PWMCR_EN_SHIFT;
    (base->PWMCR) = tmp;
    return;
}

/*
 * Function Name : PWM_GetEnable
 * Description   : Get PWM module status.
 *
 */
uint32_t PWM_GetEnable(pwm_type_t *base)
{
    uint32_t tmp = (base->PWMCR) & PWM_PWMCR_EN_MASK;
    return (tmp >> PWM_PWMCR_EN_SHIFT);
}

/*
 * Function Name : PWM_OutputPolarityCfg
 * Description   : Configure PWM output polarity.
 *
 */
status_t PWM_OutputPolarityCfg(pwm_type_t *base, pwm_output_cfg_t pwmOutputCfg)
{
    if ((uint32_t)pwmOutputCfg > DISCONT) {
        return STATUS_ERROR;
    }
    uint32_t tmp = (base->PWMCR) & (~PWM_PWMCR_POUTC_MASK);
    tmp |= (uint32_t)pwmOutputCfg << PWM_PWMCR_POUTC_SHIFT;
    (base->PWMCR) = tmp;
    return STATUS_SUCCESS;
}

/*
 * Function Name : PWM_SetClkSrc
 * Description   : Configure PWM clock source.
 *
 */
status_t PWM_SetClkSrc(pwm_type_t *base, pwm_clk_src_cfg_t pwmClkSrcCfg)
{
    if ((uint32_t)pwmClkSrcCfg > IPG_CLK_32K) {
        return STATUS_ERROR;
    }
    uint32_t tmp = (base->PWMCR) & (~PWM_PWMCR_CLKSRC_MASK);
    tmp |= (uint32_t)pwmClkSrcCfg << PWM_PWMCR_CLKSRC_SHIFT;
    (base->PWMCR) = tmp;
    return STATUS_SUCCESS;
}

/*
 * Function Name : PWM_SetActive
 * Description   : Set PWM active in different Mode.
 *
 */
void PWM_SetActive(pwm_type_t *base, pwm_active_mode_t pwmActiveMode, bool enable)
{
    uint32_t tmp  = (base->PWMCR) & (~(1UL << (uint32_t)pwmActiveMode));
    (base->PWMCR) = tmp | ((uint32_t)enable << (uint32_t)pwmActiveMode);
    return;
}

/*
 * Function Name : PWM_SetPreScaler
 * Description   : Configure PWM prescaler.
 *
 */
status_t PWM_SetPreScaler(pwm_type_t *base, uint32_t div)
{
    if ((div > 0xfff) || (div == 0)) {
        return STATUS_ERROR;
    }
    uint32_t tmp = (base->PWMCR) & (~PWM_PWMCR_PRESCALER_MASK);
    tmp |= (uint32_t)(div - 1) << PWM_PWMCR_PRESCALER_SHIFT;
    (base->PWMCR) = tmp;
    return STATUS_SUCCESS;
}

/*
 * Function Name : PWM_CfgFifoRepeat
 * Description   : Configure FIFO repeat.
 *
 */
status_t PWM_CfgFifoRepeat(pwm_type_t *base, pwm_sample_repeat_t repeatTimes)
{
    uint32_t tmp = (base->PWMCR) & (~PWM_PWMCR_REPEAT_MASK);
    tmp |= (uint32_t)repeatTimes << PWM_PWMCR_REPEAT_SHIFT;
    (base->PWMCR) = tmp;
    return STATUS_SUCCESS;
}

/*
 * Function Name : PWM_SetFifoWaterMark
 * Description   : Set FIFO Water Mark.
 *
 */
status_t PWM_SetFifoWaterMark(pwm_type_t *base, uint8_t waterMark)
{
    if (waterMark > 3) {
        return STATUS_ERROR;
    }
    uint32_t tmp = (base->PWMCR) & (~PWM_PWMCR_FIFO_WATER_MARK_MASK);
    tmp |= (uint32_t)waterMark << PWM_PWMCR_FIFO_WATER_MARK_SHIFT;
    (base->PWMCR) = tmp;
    return STATUS_SUCCESS;
}

/*
 * Function Name : PWM_GetStatusFlag
 * Description   : This function returns the state of one of the PWM status flags as requested by
 * the user.
 *
 */
bool PWM_GetStatusFlag(const pwm_type_t *base, pwm_status_flag_t statusFlag)
{
    uint32_t tmp = (base->PWMSR) & (1UL << (uint32_t)statusFlag);
    return (tmp >> (uint32_t)statusFlag);
}

/*
 * Function Name : PWM_ClearStatusFlag
 * Description   : Clear the PWM status flag state.
 *
 */
void PWM_ClearStatusFlag(pwm_type_t *base, pwm_status_flag_t statusFlag)
{
    (base->PWMSR) = (1UL << (uint32_t)statusFlag);
    return;
}

/*
 * Function Name : PWM_GetFifoDataNum
 * Description   : Gets the PWM FIFO data number.
 *
 */
uint8_t PWM_GetFifoDataNum(const pwm_type_t *base)
{
    return ((base->PWMSR) & PWM_PWMSR_FIFOAV_MASK) >> PWM_PWMSR_FIFOAV_SHIFT;
}

/*
 * Function Name : PWM_SetIntMode
 * Description   : Enable or Disable PWM interrupt.
 *
 */
status_t PWM_SetIntMode(pwm_type_t *base, pwm_int_en_t pwmIntEn, bool enable)
{
    uint32_t tmp  = (base->PWMIR) & (~(1UL << (uint32_t)pwmIntEn));
    (base->PWMIR) = tmp | ((uint32_t)enable << (uint32_t)pwmIntEn);
    return STATUS_SUCCESS;
}

/*
 * Function Name : PWM_GetIntMode
 * Description   : This function returns the state of one of the PWM interrupt flags as requested by
 * the user.
 *
 */
bool PWM_GetIntMode(const pwm_type_t *base, pwm_int_en_t pwmIntEn)
{
    return ((base->PWMIR) & ((1UL << (uint32_t)pwmIntEn))) >> (uint32_t)pwmIntEn;
}

/*
 * Function Name : PWM_SetSample
 * Description   : Set sample register.
 *
 */
status_t PWM_SetSample(pwm_type_t *base, uint32_t sample)
{
    if (sample > 0xffff) {
        return STATUS_ERROR;
    }
    uint32_t tmp   = (base->PWMSAR) & (~PWM_PWMSAR_SAMPLE_MASK);
    (base->PWMSAR) = tmp | (sample << PWM_PWMSAR_SAMPLE_SHIFT);
    return STATUS_SUCCESS;
}

/*
 * Function Name : PWM_GetSample
 * Description   : Get sample register.
 *
 */
uint32_t PWM_GetSample(const pwm_type_t *base)
{
    return ((base->PWMSAR) & PWM_PWMSAR_SAMPLE_MASK) >> PWM_PWMSAR_SAMPLE_SHIFT;
}

/*
 * Function Name : PWM_SetPeriod
 * Description   : Set Period register.
 *
 */
status_t PWM_SetPeriod(pwm_type_t *base, uint32_t period)
{
    if (period > 0xffff) {
        return STATUS_ERROR;
    }
    uint32_t tmp  = (base->PWMPR) & (~PWM_PWMPR_PERIOD_MASK);
    (base->PWMPR) = tmp | (period << PWM_PWMPR_PERIOD_SHIFT);
    return STATUS_SUCCESS;
}

/*
 * Function Name : PWM_GetPeriod
 * Description   : Get Period register.
 *
 */
uint32_t PWM_GetPeriod(const pwm_type_t *base)
{
    return ((base->PWMPR) & PWM_PWMPR_PERIOD_MASK) >> PWM_PWMPR_PERIOD_SHIFT;
}

/*
 * Function Name : PWM_GetCounter
 * Description   : Get counter register.
 *
 */
uint32_t PWM_GetCounter(const pwm_type_t *base)
{
    return ((base->PWMCNR) & PWM_PWMCNR_COUNT_MASK) >> PWM_PWMCNR_COUNT_SHIFT;
}
