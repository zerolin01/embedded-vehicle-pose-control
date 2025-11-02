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
 * @file pwm_driver.c
 * @brief PWM driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "pwm_driver.h"
#include "callbacks.h"
#include "pwm_hw_access.h"
#include <assert.h>
#include "clock_driver.h"

#define PWM_CLK_SRC_COUNT   (4U)

/*
 * @brief Table of base pointers of PWM instances.
 */
pwm_type_t *g_pwmBase[PWM_INSTANCE_COUNT] = PWM_BASE_PTRS;

/*
 * @brief Table to save PWM IRQ enumeration numbers defined in the hard related header file.
 */
IRQn_Type g_pwmIrqId[PWM_INSTANCE_COUNT] = PWM_IRQS;

/* Pointer to runtime state structure */
pwm_state_t *g_pwmStatePtr[PWM_INSTANCE_COUNT] = {NULL};

static void PWM_IRQHandler(void *data);

/*
 * Function Name : PWM_DRV_Init
 * Description   : Init PWM with user config.
 *
 */
status_t PWM_DRV_Init(uint32_t instance, pwm_state_t *pwmState, const pwm_config_t *pwmConfig)
{
    OS_ASSERT(instance < PWM_INSTANCE_COUNT);
    OS_ASSERT(pwmState != NULL);
    OS_ASSERT(pwmConfig != NULL);
    pwm_type_t *base = g_pwmBase[instance];
    OS_ASSERT(base != NULL);
    OS_RegisterType_t uType = {0};

    /* runtime state */
    g_pwmStatePtr[instance] = pwmState;

    /* store config to runtime */
    pwmState->callback      = pwmConfig->callback;
    pwmState->callbackParam = pwmConfig->callbackParam;

    const clock_names_t pwmClocks[PWM_INSTANCE_COUNT] = PWM_CLOCKS;
    CLOCK_SYS_ClockEnable(pwmClocks[instance], true);

    uint32_t frequency = 0;
    const clock_names_t pwmClksForGetFreq[PWM_CLK_SRC_COUNT] = PWM_CLOCKS_FOR_GET_FREQ;
    CLOCK_SYS_GetFreq(pwmClksForGetFreq[pwmConfig->pwm_clk_src_cfg], &frequency);

    /* Software reset */
    PWM_Init(base);

    /* Select source clk */
    OS_ASSERT(STATUS_SUCCESS == PWM_SetClkSrc(base, (pwm_clk_src_cfg_t)pwmConfig->pwm_clk_src_cfg));

    /* Set prescaler */
    OS_ASSERT(STATUS_SUCCESS == PWM_SetPreScaler(base, pwmConfig->prescaler));

    /* Set water mark */
    OS_ASSERT(STATUS_SUCCESS == PWM_SetFifoWaterMark(base, pwmConfig->water_mark - 1));

    /* Set sample register */
    OS_ASSERT(STATUS_SUCCESS == PWM_SetSample(base, pwmConfig->duty));

    /* Set period */
    OS_ASSERT(STATUS_SUCCESS == PWM_SetPeriod(base, pwmConfig->period));

    /* Set repeat */
    OS_ASSERT(STATUS_SUCCESS == PWM_CfgFifoRepeat(base, (pwm_sample_repeat_t)pwmConfig->repeat));

    /* Set irq type */
    if (pwmConfig->int_type & PWM_PWMIR_FIE_MASK) {
        PWM_SetIntMode(base, FIE, true);
    }
    if (pwmConfig->int_type & PWM_PWMIR_RIE_MASK) {
        PWM_SetIntMode(base, RIE, true);
    }
    if (pwmConfig->int_type & PWM_PWMIR_CIE_MASK) {
        PWM_SetIntMode(base, CIE, true);
    }

    /* Register irq handler */
    uType.trig_mode = CLIC_LEVEL_TRIGGER;
    uType.lvl       = 250;
    uType.priority  = 1;
    uType.data_ptr  = (void *)instance;
    OS_RequestIrq(g_pwmIrqId[instance], PWM_IRQHandler, &uType);

    /* Enable the interrupt */
    OS_EnableIrq(g_pwmIrqId[instance]);

    /* Set PWM Enable */
    PWM_Enable(base, true);

    return STATUS_SUCCESS;
}

/*
 * Function Name : PWM_DRV_GetFrequency
 * Description   : Get the frequency of PWM.
 *
 */
uint32_t PWM_DRV_GetFrequency(const pwm_config_t *pwmConfig)
{
    uint32_t frequency = 0;
    const clock_names_t pwmClksForGetFreq[PWM_CLK_SRC_COUNT] = PWM_CLOCKS_FOR_GET_FREQ;
    CLOCK_SYS_GetFreq(pwmClksForGetFreq[pwmConfig->pwm_clk_src_cfg], &frequency);
    return frequency;
}

/*
 * Function Name : PWM_DRV_Deinit
 * Description   : Deinit PWM.
 *
 */
status_t PWM_DRV_Deinit(uint32_t instance)
{
    OS_ASSERT(instance < PWM_INSTANCE_COUNT);
    pwm_type_t *base = g_pwmBase[instance];
    OS_ASSERT(base != NULL);

    /* Set PWM Disable*/
    PWM_Enable(base, false);

    const clock_names_t pwmClocks[PWM_INSTANCE_COUNT] = PWM_CLOCKS;
    CLOCK_SYS_ClockEnable(pwmClocks[instance], false);

    /* Enable the interrupt */
    OS_DisableIrq(g_pwmIrqId[instance]);

    g_pwmStatePtr[instance] = NULL;

    /* Software reset */
    PWM_Init(base);

    return STATUS_SUCCESS;
}

/*
 * Function Name : PWM_DRV_Start
 * Description   : Start generate PWM pulse.
 *
 */
status_t PWM_DRV_Start(uint32_t instance)
{
    OS_ASSERT(instance < PWM_INSTANCE_COUNT);
    pwm_type_t *base = g_pwmBase[instance];
    OS_ASSERT(base != NULL);

    PWM_Enable(base, true);

    return STATUS_SUCCESS;
}

/*
 * Function Name : PWM_DRV_Stop
 * Description   : Stop generate PWM pulse.
 *
 */
status_t PWM_DRV_Stop(uint32_t instance)
{
    OS_ASSERT(instance < PWM_INSTANCE_COUNT);
    pwm_type_t *base = g_pwmBase[instance];
    OS_ASSERT(base != NULL);

    PWM_Enable(base, false);

    return STATUS_SUCCESS;
}

/*
 * Function Name : PWM_DRV_Cfg
 * Description   : Configure PWM Parameters.
 *
 */
status_t PWM_DRV_Cfg(uint32_t instance, pwm_state_t *pwmState, const pwm_config_t *pwmConfig)
{
    OS_ASSERT(instance < PWM_INSTANCE_COUNT);
    pwm_type_t *base = g_pwmBase[instance];
    OS_ASSERT(base != NULL);

    PWM_Enable(base, false);

    /* Select source clk */
    OS_ASSERT(STATUS_SUCCESS == PWM_SetClkSrc(base, (pwm_clk_src_cfg_t)pwmConfig->pwm_clk_src_cfg));

    /* Set prescaler */
    OS_ASSERT(STATUS_SUCCESS == PWM_SetPreScaler(base, pwmConfig->prescaler));

    /* Set water mark */
    OS_ASSERT(STATUS_SUCCESS == PWM_SetFifoWaterMark(base, pwmConfig->water_mark - 1));

    /* Set Polarity*/
    OS_ASSERT(STATUS_SUCCESS == PWM_OutputPolarityCfg(base, (pwm_output_cfg_t)(pwmConfig->pwm_polarity)));

    /* Set sample register */
    OS_ASSERT(STATUS_SUCCESS == PWM_SetSample(base, pwmConfig->duty));

    /* Set period */
    OS_ASSERT(STATUS_SUCCESS == PWM_SetPeriod(base, pwmConfig->period));

    PWM_Enable(base, true);

    return STATUS_SUCCESS;
}

/*
 * Function Name : PWM_IRQHandler
 * Description   : The function PWM_IRQHandler passes IRQ control driver.
 *
 */
static void PWM_IRQHandler(void *data)
{
    uint32_t instance = (uint32_t)data;

    OS_ASSERT(instance < PWM_INSTANCE_COUNT);

    pwm_type_t *base   = g_pwmBase[instance];
    pwm_state_t *state = g_pwmStatePtr[instance];
    OS_ASSERT(base != NULL);
    OS_ASSERT(state != NULL);

    uint32_t tmpsr = base->PWMSR;
    uint32_t tmpir = base->PWMIR;
    if ((tmpsr & PWM_PWMSR_FE_MASK) && (tmpir & PWM_PWMIR_FIE_MASK)) {
        if (state->callback) {
            state->callback(state, EVENT_FIFO_EMPTY, state->callbackParam);
        }
        PWM_ClearStatusFlag(base, FE);
    }

    if ((tmpsr & PWM_PWMSR_ROV_MASK) && (tmpir & PWM_PWMIR_RIE_MASK)) {
        if (state->callback) {
            state->callback(state, EVENT_ROLL_OVER, state->callbackParam);
        }
        PWM_ClearStatusFlag(base, ROV);
    }

    if ((tmpsr & PWM_PWMSR_CMP_MASK) && (tmpir & PWM_PWMIR_CIE_MASK)) {
        if (state->callback) {
            state->callback(state, EVENT_COMPARE, state->callbackParam);
        }
        PWM_ClearStatusFlag(base, CMP);
    }

    if (tmpsr & PWM_PWMSR_FWE_SHIFT) {
        if (state->callback) {
            state->callback(state, EVENT_FIFO_ERROR, state->callbackParam);
        }
        PWM_ClearStatusFlag(base, FWE);
    }

    return;
}

/*
 * Function Name : PWM_DRV_TcsrStopZoneMode
 * Description   : Enter/exit stop mode according the TCSR
 *
 */
void PWM_DRV_TcsrStopZoneMode(uint8_t instance, bool en)
{
    OS_ASSERT(instance < PWM_INSTANCE_COUNT);

    pwm_type_t *base = g_pwmBase[instance];
    OS_ASSERT(base != NULL);

    uint32_t stop_enable = 1 << (instance + TCSR_STOP_PWM0_SHIFT);

    tcsr_type_t *tcsr = (tcsr_type_t *)TCSR_BASEADDR;
    uint32_t val      = tcsr->STOP;

    if (en) {
        val = val | stop_enable;
        //PWM_SetActive(base, STOP_EN, true);
    } else {
        val = val & (~stop_enable);
        //PWM_SetActive(base, STOP_EN, false);
    }

    tcsr->STOP = val;
}

bool PWM_DRV_temp_softRstTest(uint8_t instance)
{
    OS_ASSERT(instance < PWM_INSTANCE_COUNT);

    pwm_type_t *base = g_pwmBase[instance];
    OS_ASSERT(base != NULL);

    uint32_t period_before = base->PWMPR;
    base->PWMCR |= PWM_PWMCR_SWR_MASK;
    OS_DelayMs(5);
    uint32_t period_after = base->PWMPR;

    if (period_before > 0 && period_after == 0xFFFE)
        return true;
    else
        return false;
}