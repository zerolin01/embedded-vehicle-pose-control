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
 * @file superio_pwm_driver.c
 * @brief SUPERIO pwm driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "superio_pwm_driver.h"
#include "clock_driver.h"
#include "superio_common.h"
#include "superio_hw_access.h"

#define SUPERIO_PWM_TIMER_CMP_MAX_VALUE (0x0100U)

#define PWM_TIMER(x) (x)

/*
 * @brief Interrupt enable masks for used timers
 */
static volatile uint8_t superio_pwm_TimerIrqMask;

#ifdef __cplusplus
extern "C" {
#endif

static inline void SUPERIO_PWM_ResetTimerRegisters(superio_type_t *Base, uint8_t timerId)
{
    SUPERIO_PWM_SetTimerPinPolarity(Base, timerId, SUPERIO_PIN_POLARITY_HIGH);
    SUPERIO_PWM_SetTimerPin(Base, timerId, 0U);
    SUPERIO_PWM_SetTimerPinOutput(Base, timerId, SUPERIO_PIN_CONFIG_DISABLED);
    SUPERIO_SetTimerInitialOutput(Base, timerId, SUPERIO_TIMER_INITOUT_ONE);
    SUPERIO_SetTimerDecrement(Base, timerId, SUPERIO_TIMER_DECREMENT_CLK_SHIFT_TMR);
    SUPERIO_PWM_SetUpperValue(Base, timerId, 0U);
    SUPERIO_PWM_SetLowerValue(Base, timerId, 0U);
}

static inline void SUPERIO_PWM_InitTimerPin(superio_type_t *const Base, uint8_t timerId, uint8_t pinId)
{
    /* Enable the pin out for the selected timer */
    SUPERIO_PWM_SetTimerPinOutput(Base, timerId, SUPERIO_PIN_CONFIG_OUTPUT);
    /* Select the pin that the selected timer will ouput the signal */
    SUPERIO_PWM_SetTimerPin(Base, timerId, pinId);
}

/*
 * Function Name : SUPERIO_PWM_UpdateInterruptMode
 * Description   : Update the interrupt mode for a channel.
 * The function will set a new mode for the flag event response on the selected channel.
 *
 */
static void SUPERIO_PWM_UpdateInterruptMode(superio_type_t *const Base, uint8_t timerId, bool irqEnable)
{
    // enter critical, to do

    /* Clear Flags and irq mask for pin and timer */
    SUPERIO_ClearTimerStatus(Base, timerId);

    /* Stop all Irq for this channel for case SuperIO_PWM_IRQ_DISABLED */
    SUPERIO_SetTimerInterrupt(Base, (1U << timerId), false);

    if (true == irqEnable) {
        SUPERIO_SetTimerInterrupt(Base, (1U << timerId), true);
        superio_pwm_TimerIrqMask = superio_pwm_TimerIrqMask | (uint8_t)((uint8_t)1U << timerId);
    } else {
        SUPERIO_SetTimerInterrupt(Base, (1U << timerId), false);
        superio_pwm_TimerIrqMask = superio_pwm_TimerIrqMask & ~((uint8_t)0x01U << timerId);
    }

    // exit critical, to do
}

/*
 * Function Name : SUPERIO_PWM_CheckStatus
 * Description   : Check status of the PWM transmission. This function can be
 *                 called either in an interrupt routine or directly in polling
 *                 mode to advance the transfer.
 *
 */
static void SUPERIO_PWM_CheckStatus(void *stateStruct)
{
    superio_type_t *baseAddr;
    uint8_t resourceIndex; // Index of first used internal resource instance (shifter and timer)
    superio_pwm_state_t *state;

    OS_ASSERT(stateStruct != NULL);

    state         = (superio_pwm_state_t *)stateStruct;
    baseAddr      = g_superioBase[state->superioCommon.instance];
    resourceIndex = state->superioCommon.resourceIndex;

    /* Check for timer status */
    if (SUPERIO_GetTimerStatus(baseAddr, PWM_TIMER(resourceIndex))) {
        /* Clear timer status */
        SUPERIO_ClearTimerStatus(baseAddr, PWM_TIMER(resourceIndex));
        /* Call callback to announce the pwm event to the user */
        if (NULL != state->callback) {
            state->callback(state, EVENT_COMPARE, state->callbackParam);
        }
    } else {
        /* No relevant events - nothing to do */
    }
}

/*
 * Function Name : SUPERIO_PWM_mode_Configure
 * Description   : Initialize a SuperIO channel in pwm mode.
 * The function will initialize one timer and pin of the selected SuperIO channel in pwm
 * mode, with the configuration of the user. The interrupts will be disabled.
 *
 */
static void SUPERIO_PWM_mode_Configure(superio_pwm_state_t *state, const superio_pwm_channel_cfg_t *ChannelCfg,
                                       uint32_t inputClock)
{
    /* Check received parameters */
    OS_ASSERT(NULL != ChannelCfg);
    /* Check ChannelCfg period can be achieved with the selected duty cycle */
    OS_ASSERT(ChannelCfg->Period >= ChannelCfg->DutyCycle);
    OS_ASSERT(SUPERIO_PWM_TIMER_CMP_MAX_VALUE >= (uint16_t)ChannelCfg->DutyCycle);
    OS_ASSERT(SUPERIO_PWM_TIMER_CMP_MAX_VALUE >= (uint16_t)(ChannelCfg->Period - ChannelCfg->DutyCycle));

    superio_type_t *const Base = g_superioBase[state->superioCommon.instance];
    /* Index of first used internal resource instance (timer) */
    uint8_t resourceIndex = state->superioCommon.resourceIndex;

    /* Compute compare reg, period, duty, to do */
    /* Configure the timer comparator with duty and period values */
    SUPERIO_PWM_SetLowerValue(Base, PWM_TIMER(resourceIndex), (uint8_t)(ChannelCfg->DutyCycle - 1U));
    SUPERIO_PWM_SetUpperValue(Base, PWM_TIMER(resourceIndex),
                              (uint8_t)(ChannelCfg->Period - ChannelCfg->DutyCycle - 1U));

    /* Configure the timer initial and clock source and control register */
    if (SUPERIO_PWM_ACTIVE_HIGH == ChannelCfg->Polarity) {
        SUPERIO_SetTimerInitialOutput(Base, PWM_TIMER(resourceIndex), SUPERIO_TIMER_INITOUT_ONE);
    } else {
        SUPERIO_SetTimerInitialOutput(Base, PWM_TIMER(resourceIndex), SUPERIO_TIMER_INITOUT_ZERO);
    }

    superio_timer_decrement_t decrement;
    if (SUPERIO_PWM_DECREMENT_CLK_SHIFT_TMR == ChannelCfg->Prescaler)
        decrement = SUPERIO_TIMER_DECREMENT_CLK_SHIFT_TMR;
    else if (SUPERIO_PWM_DECREMENT_TRG_SHIFT_TMR == ChannelCfg->Prescaler)
        decrement = SUPERIO_TIMER_DECREMENT_TRG_SHIFT_TMR;
    else if (SUPERIO_PWM_DECREMENT_PIN_SHIFT_PIN == ChannelCfg->Prescaler)
        decrement = SUPERIO_TIMER_DECREMENT_PIN_SHIFT_PIN;
    else if (SUPERIO_PWM_DECREMENT_TRG_SHIFT_TRG == ChannelCfg->Prescaler)
        decrement = SUPERIO_TIMER_DECREMENT_TRG_SHIFT_TRG;
    SUPERIO_SetTimerDecrement(Base, PWM_TIMER(resourceIndex), decrement);

    superio_timer_enable_t timerEn;
    if (SUPERIO_PWM_ENABLE_ALWAYS == ChannelCfg->TimerEnable)
        timerEn = SUPERIO_TIMER_ENABLE_ALWAYS;
    else if (SUPERIO_PWM_ENABLE_TIM_ENABLE == ChannelCfg->TimerEnable)
        timerEn = SUPERIO_TIMER_ENABLE_TIM_ENABLE;
    else if (SUPERIO_PWM_ENABLE_TRG_HIGH == ChannelCfg->TimerEnable)
        timerEn = SUPERIO_TIMER_ENABLE_TRG_HIGH;
    else if (SUPERIO_PWM_ENABLE_TRG_PIN_HIGH == ChannelCfg->TimerEnable)
        timerEn = SUPERIO_TIMER_ENABLE_TRG_PIN_HIGH;
    else if (SUPERIO_PWM_ENABLE_PIN_POSEDGE == ChannelCfg->TimerEnable)
        timerEn = SUPERIO_TIMER_ENABLE_PIN_POSEDGE;
    else if (SUPERIO_PWM_ENABLE_PIN_POSEDGE_TRG_HIGH == ChannelCfg->TimerEnable)
        timerEn = SUPERIO_TIMER_ENABLE_PIN_POSEDGE_TRG_HIGH;
    else if (SUPERIO_PWM_ENABLE_TRG_POSEDGE == ChannelCfg->TimerEnable)
        timerEn = SUPERIO_TIMER_ENABLE_TRG_POSEDGE;
    else if (SUPERIO_PWM_ENABLE_TRG_EDGE == ChannelCfg->TimerEnable)
        timerEn = SUPERIO_TIMER_ENABLE_TRG_EDGE;

    SUPERIO_SetTimerEnable(Base, PWM_TIMER(resourceIndex), timerEn);

    superio_trigger_source_t trigger_src;
    if (SUPERIO_PWM_TRIGGER_SOURCE_EXTERNAL == ChannelCfg->triggerSrc)
        trigger_src = SUPERIO_TRIGGER_SOURCE_EXTERNAL;
    else
        trigger_src = SUPERIO_TRIGGER_SOURCE_INTERNAL;

    superio_trigger_polarity_t trigger_polarity;
    if (SUPERIO_PWM_TRIGGER_POLARITY_HIGH == ChannelCfg->triggerPolarity)
        trigger_polarity = SUPERIO_TRIGGER_POLARITY_HIGH;
    else
        trigger_polarity = SUPERIO_TRIGGER_POLARITY_LOW;
    SUPERIO_SetTimerTrigger(Base, PWM_TIMER(resourceIndex), ChannelCfg->triggerSel, trigger_polarity, trigger_src);

    /* Configure timer pin */
    SUPERIO_PWM_InitTimerPin(Base, PWM_TIMER(resourceIndex), ChannelCfg->PinId);

    SUPERIO_PWM_SetTimerPinPolarity(Base, PWM_TIMER(resourceIndex), SUPERIO_PIN_POLARITY_HIGH);
    /* set interrupts as requirements */
    bool irqEnable = (ChannelCfg->driverType == SUPERIO_DRIVER_TYPE_INTERRUPTS);
    SUPERIO_PWM_UpdateInterruptMode(Base, PWM_TIMER(resourceIndex), irqEnable);

    SUPERIO_SetTimerMode(Base, PWM_TIMER(resourceIndex), SUPERIO_TIMER_MODE_8BIT_PWM);
}

/*
 * Function Name : SUPERIO_PWM_DRV_GetClkFreq
 * Description   : Get freqency of the SuperIO_CLK
 *
 */
uint32_t SUPERIO_PWM_DRV_GetClkFreq(uint32_t instance)
{
    uint32_t freqency;
    (void)CLOCK_SYS_GetFreq(g_superioClock[instance], &freqency);
    return freqency;
}

/*
 * Function Name : SUPERIO_PWM_DRV_Init
 * Description   : Initialize the SuperIO_PWM driver
 *
 */
status_t SUPERIO_PWM_DRV_Init(uint32_t instance, const superio_pwm_channel_cfg_t *channelConfigPtr,
                              superio_pwm_state_t *state)
{
    uint32_t inputClock;
    status_t clkErr;
    status_t retCode;
    status_t osifError = STATUS_SUCCESS;

    OS_ASSERT(state != NULL);
    OS_ASSERT(instance < SUPERIO_INSTANCE_COUNT);
    /* Check that device was initialized */
    OS_ASSERT(g_superioDeviceStatePtr[instance] != NULL);

    /* Get the protocol clock frequency */
    clkErr = CLOCK_SYS_GetFreq(g_superioClock[instance], &inputClock);
    OS_ASSERT(clkErr == STATUS_SUCCESS);
    OS_ASSERT(inputClock > 0U);

    /* Instruct the resource allocator that we need one shifter/timer */
    state->superioCommon.resourceCount = 1U;
    /* Common SuperIO driver initialization */
    retCode = SUPERIO_DRV_InitDriver(instance, (superio_common_state_t *)state);
    if (STATUS_SUCCESS != retCode) { /* Initialization failed, not enough resources */
        return retCode;
    }

    /* Initialize driver-specific context structure */
    state->callback      = channelConfigPtr->callback;
    state->callbackParam = channelConfigPtr->callbackParam;
    state->driverType    = channelConfigPtr->driverType;
    state->status        = STATUS_SUCCESS;
    state->period        = channelConfigPtr->Period;
    state->dutyCycle     = channelConfigPtr->DutyCycle;
    state->polarity      = channelConfigPtr->Polarity;
    state->prescaler     = channelConfigPtr->Prescaler;
    state->pinId         = channelConfigPtr->PinId;

    /* Configure device for pwm mode */
    SUPERIO_PWM_mode_Configure(state, channelConfigPtr, inputClock);

    /* Set up pwm mode engine */
    switch (state->driverType) {
    case SUPERIO_DRIVER_TYPE_INTERRUPTS:
        state->superioCommon.isr = SUPERIO_PWM_CheckStatus;
        break;
    case SUPERIO_DRIVER_TYPE_POLLING:
        /* Nothing to do here, SuperIO_PWM_DRV_GetStatus() will handle the transfer */
        break;
    default:
        /* Impossible type - do nothing */
        break;
    }

    (void)clkErr;
    (void)osifError;
    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_PWM_DRV_Deinit
 * Description   : De-initialize the SuperIO_PWM driver
 *
 */
status_t SUPERIO_PWM_DRV_Deinit(superio_pwm_state_t *state)
{
    OS_ASSERT(state != NULL);

    superio_type_t *const Base = g_superioBase[state->superioCommon.instance];
    /* Index of first used internal resource instance (timer) */
    uint8_t resourceIndex = state->superioCommon.resourceIndex;

    /* Disable the timer */
    SUPERIO_SetTimerMode(Base, PWM_TIMER(resourceIndex), SUPERIO_TIMER_MODE_DISABLED);

    /* Clear flags for timer and pin */
    SUPERIO_PWM_UpdateInterruptMode(Base, PWM_TIMER(resourceIndex), false);

    /* Reset the timer registers */
    SUPERIO_PWM_ResetTimerRegisters(Base, PWM_TIMER(resourceIndex));

    return SUPERIO_DRV_DeinitDriver((superio_common_state_t *)state);
}

/*
 * Function Name : SUPERIO_PWM_DRV_UpdateClockPrescaler
 * Description   : Set clock prescaler for a SuperIO channel
 * The function will change the prescaler value for the selected SuperIO channel.
 *
 */
status_t SUPERIO_PWM_DRV_UpdateClockPrescaler(superio_pwm_state_t *state, superio_pwm_decrement_t Prescaler)
{
    OS_ASSERT(state != NULL);
    superio_type_t *Base  = g_superioBase[state->superioCommon.instance];
    uint8_t resourceIndex = state->superioCommon.resourceIndex;

    // enter critical, to do
    /* update timer configuration register */
    superio_timer_decrement_t decrement;
    if (SUPERIO_PWM_DECREMENT_CLK_SHIFT_TMR == Prescaler)
        decrement = SUPERIO_TIMER_DECREMENT_CLK_SHIFT_TMR;
    else if (SUPERIO_PWM_DECREMENT_TRG_SHIFT_TMR == Prescaler)
        decrement = SUPERIO_TIMER_DECREMENT_TRG_SHIFT_TMR;
    else if (SUPERIO_PWM_DECREMENT_PIN_SHIFT_PIN == Prescaler)
        decrement = SUPERIO_TIMER_DECREMENT_PIN_SHIFT_PIN;
    else if (SUPERIO_PWM_DECREMENT_TRG_SHIFT_TRG == Prescaler)
        decrement = SUPERIO_TIMER_DECREMENT_TRG_SHIFT_TRG;
    SUPERIO_SetTimerDecrement(Base, PWM_TIMER(resourceIndex), decrement);
    // exit critical, to do

    return STATUS_SUCCESS;
}

/*
 * Function Name : SuperIO_PWM_UpdatePeriodDuty
 * Description   : Set a new value for duty cycle and period of the channel.
 * The function will update the selected SuperIO channel with the new values for the
 * duty cycle and period.
 *
 */
status_t SUPERIO_PWM_DRV_UpdatePeriodDuty(superio_pwm_state_t *state, uint16_t Period, uint16_t DutyCycle,
                                          superio_pwm_PolarityType_t Polarity)
{
    OS_ASSERT(state != NULL);
    superio_type_t *Base  = g_superioBase[state->superioCommon.instance];
    uint8_t resourceIndex = state->superioCommon.resourceIndex;

    /* Check ChannelCfg period can be achieved with the selected duty cycle */
    OS_ASSERT(Period >= DutyCycle);
    OS_ASSERT(SUPERIO_PWM_TIMER_CMP_MAX_VALUE >= DutyCycle);
    OS_ASSERT(SUPERIO_PWM_TIMER_CMP_MAX_VALUE >= (uint16_t)(Period - DutyCycle));

    // enter critical, to do

    if (SUPERIO_PWM_ACTIVE_HIGH == Polarity) {
        SUPERIO_SetTimerInitialOutput(Base, PWM_TIMER(resourceIndex), SUPERIO_TIMER_INITOUT_ONE);
    } else {
        SUPERIO_SetTimerInitialOutput(Base, PWM_TIMER(resourceIndex), SUPERIO_TIMER_INITOUT_ZERO);
    }
    SUPERIO_SetTimerMode(Base, PWM_TIMER(resourceIndex), SUPERIO_TIMER_MODE_8BIT_PWM);

    /* Configure the timer comparator with duty and period values */
    SUPERIO_PWM_SetLowerValue(Base, PWM_TIMER(resourceIndex), (uint8_t)(DutyCycle - 1U));
    SUPERIO_PWM_SetUpperValue(Base, PWM_TIMER(resourceIndex), (uint8_t)(Period - DutyCycle - 1U));

    // exit critical, to do

    return STATUS_SUCCESS;
}

/*
 * Function Name : SuperIO_PWM_GetOutputState
 * Description   : Get the logic level of the channel ouput.
 * The function will return the logic level that the selected SuperIO channel is driving on
 * on the output pin.
 *
 */
bool SUPERIO_PWM_DRV_GetOutputState(superio_pwm_state_t *state)
{
    OS_ASSERT(state != NULL);
    superio_type_t *Base = g_superioBase[state->superioCommon.instance];
    uint8_t pinId        = state->pinId;

    return SUPERIO_PWM_GetPinState(Base, pinId);
}

/*
 * Function Name : SUPERIO_PWM_DRV_GetPeriod
 * Description   : Getting the period for a channel.
 * The function will get the period on the selected channel.
 *
 */
uint16_t SUPERIO_PWM_DRV_GetPeriod(superio_pwm_state_t *state)
{
    OS_ASSERT(state != NULL);
    superio_type_t *Base  = g_superioBase[state->superioCommon.instance];
    uint8_t resourceIndex = state->superioCommon.resourceIndex;

    /* Getting the period for a channel. */
    uint16_t Period = (uint16_t)SUPERIO_PWM_GetUpperValue(Base, PWM_TIMER(resourceIndex)) +
                      (uint16_t)SUPERIO_PWM_GetLowerValue(Base, PWM_TIMER(resourceIndex)) + (uint16_t)2U;

    return Period;
}

/*
 * Function Name : SUPERIO_PWM_DRV_Init_Dummy
 * Description   : Initialize the SuperIO_PWM driver
 * Description   : just occupy a dummy resourceIndex, not really use.
 *
 */
status_t SUPERIO_PWM_DRV_Init_Dummy(uint32_t instance, const superio_pwm_channel_cfg_t *channelConfigPtr,
                                    superio_pwm_state_t *state)
{
    uint32_t inputClock;
    status_t clkErr;
    status_t retCode;
    status_t osifError = STATUS_SUCCESS;

    OS_ASSERT(state != NULL);
    OS_ASSERT(instance < SUPERIO_INSTANCE_COUNT);
    /* Check that device was initialized */
    OS_ASSERT(g_superioDeviceStatePtr[instance] != NULL);

    /* Get the protocol clock frequency */
    clkErr = CLOCK_SYS_GetFreq(g_superioClock[instance], &inputClock);
    OS_ASSERT(clkErr == STATUS_SUCCESS);
    OS_ASSERT(inputClock > 0U);

    /* Instruct the resource allocator that we need one shifter/timer */
    state->superioCommon.resourceCount = 1U;
    /* Common SuperIO driver initialization */
    retCode = SUPERIO_DRV_InitDriver(instance, (superio_common_state_t *)state);
    if (STATUS_SUCCESS != retCode) { /* Initialization failed, not enough resources */
        return retCode;
    }

    (void)clkErr;
    (void)osifError;
    return STATUS_SUCCESS;
}

/*
 * Function Name : SUPERIO_PWM_DRV_Deinit_Dummy
 * Description   : just deinit the occupied dummy resourceIndex.
 *
 */
status_t SUPERIO_PWM_DRV_Deinit_Dummy(superio_pwm_state_t *state)
{
    OS_ASSERT(state != NULL);

    superio_type_t *const Base = g_superioBase[state->superioCommon.instance];
    /* Index of first used internal resource instance (timer) */
    uint8_t resourceIndex = state->superioCommon.resourceIndex;

    /* Disable the timer */
    SUPERIO_SetTimerMode(Base, PWM_TIMER(resourceIndex), SUPERIO_TIMER_MODE_DISABLED);

    return SUPERIO_DRV_DeinitDriver((superio_common_state_t *)state);
}

/*
 * Function Name : SUPERIO_PWM_DRV_UpdateInterrupt
 * Description   : Update the interrupt mode for a channel.
 *
 */
void SUPERIO_PWM_DRV_UpdateInterrupt(superio_pwm_state_t *state, bool irqEnable)
{
    OS_ASSERT(state != NULL);

    superio_type_t *const Base = g_superioBase[state->superioCommon.instance];
    /* Index of first used internal resource instance (timer) */
    uint8_t resourceIndex = state->superioCommon.resourceIndex;

    SUPERIO_PWM_UpdateInterruptMode(Base, resourceIndex, irqEnable);
}

/*
 * Function Name : SUPERIO_DRV_TcsrStopZoneMode
 * Description   : Enter/exit stop mode according the TCSR
 *
 */
void SUPERIO_DRV_TcsrStopZoneMode(superio_pwm_state_t *state, bool en)
{
    OS_ASSERT(state != NULL);

    uint32_t stop_enable    = 1 << TCSR_STOP_SUPERIO_SHIFT;
    uint32_t stopSel_enable = 1 << TCSR_STOPMODE_SUPERIO_SHIFT;

    tcsr_type_t *tcsr     = (tcsr_type_t *)TCSR_BASEADDR;
    uint32_t val_stop     = tcsr->STOP;
    uint32_t val_stopMode = tcsr->STOPMODE;

    if (en) {
        val_stop     = val_stop | stop_enable;
        val_stopMode = val_stopMode | stopSel_enable;
    } else {
        val_stop     = val_stop & (~stop_enable);
        val_stopMode = val_stopMode & (~stopSel_enable);
    }

    tcsr->STOP     = val_stop;
    tcsr->STOPMODE = val_stopMode;
}

/*
 * Function Name : SUPERIO_PWM_DRV_CheckTcsrStopACK
 * Description   : Check stop ack according the TCSR
 *
 */
bool SUPERIO_PWM_DRV_CheckTcsrStopACK(superio_pwm_state_t *state)
{
    OS_ASSERT(state != NULL);

    uint32_t stop_ack = 0, val = 0;
    stop_ack = 1 << 0;

    tcsr_type_t *tcsr = (tcsr_type_t *)TCSR_BASEADDR;
    val               = tcsr->STOPACK;

    if (val & stop_ack)
        return true;
    else
        return false;
}

bool SUPERIO_PWM_Temp_softRst_test(superio_pwm_state_t *state)
{
    OS_ASSERT(state != NULL);

    superio_type_t *const Base = g_superioBase[state->superioCommon.instance];

    uint16_t period_before = SUPERIO_PWM_DRV_GetPeriod(state);
    SUPERIO_SetSoftwareReset(Base, true);
    OS_DelayMs(5);
    uint16_t period_after = SUPERIO_PWM_DRV_GetPeriod(state);
    if (period_before > 0 && period_after == 2) // refer superio TIMCMP register
        return true;
    else
        return false;
}

#ifdef __cplusplus
}
#endif
