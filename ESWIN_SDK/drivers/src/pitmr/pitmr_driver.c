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
 * @file pitmr_driver.c
 * @brief PITMR driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-13
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "pitmr_driver.h"
#include "clock_driver.h"
#include "pitmr_hw_access.h"
#include <stddef.h>

/**
 * @brief  Table of base addresses for PITMR instances
 */
static pitmr_type_t *const g_pstPitmrBase[] = PITMR_BASE_PTRS;

/**
 * @brief Table to save PITMR indexes in PCC register map for clock configuration
 */
static const clock_names_t g_stPitmrClkNames[PITMR_INSTANCE_COUNT] = PITMR_CLOCK_NAMES;

/**
 * @brief PITMR functional clock variable which will be updated in some driver functions
 */
static uint32_t g_stPitmrSourceClockFrequency[PITMR_INSTANCE_COUNT] = {0};

/** Table to save  PITMR IRQ enum numbers. */
static const IRQn_Type g_stPitmrIrqId[] = PITMR_IRQS;

/** Interrupt of the configuration structure **/
typedef struct {
    uint16_t instance;
    uint16_t channel;
    pitmr_callback_t callBack;
    void *parameter;
} pitmr_irq_t;

static pitmr_irq_t g_stPitmrIRQ[14];

/*
 *
 * Function Name : PITMR_IRQHandler
 * Description   : interrupt handling functions
 *
 */
static void PITMR_IRQHandler(void *para)
{
    uint32_t irqIndex = (uint32_t)para;

    uint16_t channel  = g_stPitmrIRQ[irqIndex].channel;
    uint16_t instance = g_stPitmrIRQ[irqIndex].instance;

    irqIndex = instance * PITMR0_TMR_COUNT + channel;
    if (NULL != g_stPitmrIRQ[irqIndex].callBack) {
        g_stPitmrIRQ[irqIndex].callBack(g_stPitmrIRQ[irqIndex].parameter);
    } else {
    }
    PITMR_DRV_ClearInterruptFlagTimerChannels(instance, (1 << channel));
}

/*
 *
 * Function Name : PITMR_DRV_InstallCallback.
 * Description   : Registers the callback function and the parameter for PITMR.
 * 
 */
status_t PITMR_DRV_InstallCallback(uint32_t instance, uint32_t channel, pitmr_callback_t callback, void *parameter)
{
    status_t reStatus = STATUS_SUCCESS;
    uint32_t irqIndex;

    /* checkout PITMR channel num */
    reStatus = PITMR_CheckChannel(instance, channel);
    if (STATUS_ERRORPAR == reStatus) {
        // log_err("instance or channel out of range");
        return reStatus;
    }

    if (NULL == callback) {
        return STATUS_ERRORPAR;
    }
    irqIndex = instance * PITMR0_TMR_COUNT + channel;

    g_stPitmrIRQ[irqIndex].instance  = instance;
    g_stPitmrIRQ[irqIndex].channel   = channel;
    g_stPitmrIRQ[irqIndex].callBack  = callback;
    g_stPitmrIRQ[irqIndex].parameter = parameter;

    return STATUS_SUCCESS;
}

/*
 *
 * Function Name : PITMR_DRV_GetDefaultConfig.
 * Description   : Get the default PITMR configuration.
 *
 */
status_t PITMR_DRV_GetDefaultConfig(pitmr_user_config_t *const config)
{
    status_t reStatus = STATUS_SUCCESS;

    if (NULL == config) {
        // log_err("config is NULL");
        reStatus = STATUS_ERRORPAR;
        return reStatus;
    }

    config->enableRunInDebug = false;
    config->enableRunInDoze  = false;

    return reStatus;
}

/*
 *
 * Function Name : PITMR_DRV_GetDefaultChanConfig.
 * Description   : Get the default PITMR channel configuration.
 *
 */
status_t PITMR_DRV_GetDefaultChanConfig(pitmr_user_channel_config_t *const config)
{

    status_t reStatus = STATUS_SUCCESS;

    if (NULL == config) {
        // log_err("config is NULL");
        reStatus = STATUS_ERRORPAR;
        return reStatus;
    }

    config->timerMode             = PITMR_PERIODIC_COUNTER;
    config->periodUnits           = PITMR_PERIOD_UNITS_MICROSECONDS;
    config->period                = 10000U;
    config->triggerSource         = PITMR_TRIGGER_SOURCE_EXTERNAL;
    config->triggerSelect         = 0U;
    config->enableReloadOnTrigger = false;
    config->enableStopOnInterrupt = false;
    config->enableStartOnTrigger  = false;
    config->chainChannel          = false;
    config->isInterruptEnabled    = true;

    return reStatus;
}

/*
 *
 * Function Name : PITMR_DRV_GetDefaultChanConfig.
 * Description   : Initializes the PITMR module.
 *
 */
status_t PITMR_DRV_Init(uint32_t instance, const pitmr_user_config_t *userConfig)
{
    pitmr_type_t *base;
    status_t reStatus  = STATUS_SUCCESS;
    uint32_t core_freq = 0u;

    if ((NULL == userConfig) || (instance >= PITMR_INSTANCE_COUNT)) {
        reStatus = STATUS_ERRORPAR;
        // log_err("Fetch clock error");
        return reStatus;
    }

    base = g_pstPitmrBase[instance];

    /* Enable PITMR clock */
    CLOCK_SYS_ClockEnable(g_stPitmrClkNames[instance], true);

    /* Gets current functional clock frequency of PITMR instance and check the functional clock of PITMR module*/
    reStatus = CLOCK_SYS_GetFreq(g_stPitmrClkNames[instance], &g_stPitmrSourceClockFrequency[instance]);
    if ((STATUS_SUCCESS != reStatus) || (0U == g_stPitmrSourceClockFrequency[instance])) {
        reStatus = STATUS_ERRORCLK;
        // log_err("Fetch clock error");
        return reStatus;
    }

    /* When resetting the PITMR module, a delay of 4 peripheral clock cycles
        must be ensured. This peripheral clock and the core clock running the
        code could be very different, two distinct cases are identified:
         - core_clk > peripheral_clk. This requires a delay loop to be implemented,
            and the delay value based on the ratio between the two frequencies.
         - core_clk <= peripheral_clk. This requires a short delay, which is usually
            below the delay caused naturally by the read-modify-write operation.
     */

    reStatus = CLOCK_SYS_GetFreq(CPU_CORE_CLK, &core_freq);

    uint32_t pitmr_freq = g_stPitmrSourceClockFrequency[instance];

    uint32_t core_to_per_clock_ratio = (core_freq + (pitmr_freq >> 1u)) / pitmr_freq;

    /* Resets PITMR module */
    PITMR_Reset(base, core_to_per_clock_ratio);

    /* Enables functional clock of PITMR module*/
    PITMR_Enable(base, core_to_per_clock_ratio);

    /* Sets PITMR operation in Debug and DOZE mode*/
    PITMR_SetTimerRunInDebugCmd(base, userConfig->enableRunInDebug);

    PITMR_SetTimerRunInDozeCmd(base, userConfig->enableRunInDoze);

    return reStatus;
}

/*
 *
 * Function Name : PITMR_DRV_Deinit.
 * Description   : De-Initializes the PITMR module.
 *
 */
status_t PITMR_DRV_Deinit(uint32_t instance)
{
    pitmr_type_t *base;
    status_t reStatus = STATUS_SUCCESS;
    uint16_t irqCount = 0, i = 0;

    if (instance >= PITMR_INSTANCE_COUNT) {
        reStatus = STATUS_ERRORPAR;
        // log_err("instance out of range");
        return reStatus;
    }

    irqCount = instance * 4;

    if (0 == irqCount) {
        for (i = 0; i < 4; i++) {
        }
    } else {
        for (i = 4; i < 14; i++) {
            /* Disable PCTMR IRQ in NVIC level */
            OS_DisableIrq(g_stPitmrIrqId[i]);

            /* Destroy registration function */
            OS_UnRequestIrq(g_stPitmrIrqId[i]);
        }
    }

    base = g_pstPitmrBase[instance];

    /* Disables PITMR module functional clock*/
    PITMR_Disable(base);

    return reStatus;
}

/*
 *
 * Function Name : PITMR_DRV_InitChannel.
 * Description   : Initializes PITMR channel.
 *
 */
status_t PITMR_DRV_InitChannel(uint32_t instance, uint32_t channel,
                               const pitmr_user_channel_config_t *userChannelConfig)
{
    pitmr_type_t *base;
    register_type_t type;
    status_t reStatus = STATUS_SUCCESS;
    uint32_t irqIndex = 0;

    reStatus = PITMR_CheckPara(instance, channel, userChannelConfig);
    if (STATUS_SUCCESS != reStatus) {
        // log_err("instance or mask out of range, or userChannelConfig error");
        return reStatus;
    }

    base = g_pstPitmrBase[instance];

    irqIndex = channel + instance * PITMR0_IRQS_CH_COUNT;

    /* Save the relevant parameters needed for the interrupt */
    g_stPitmrIRQ[irqIndex].instance  = instance;
    g_stPitmrIRQ[irqIndex].channel   = channel;
    g_stPitmrIRQ[irqIndex].callBack  = userChannelConfig->callBack;
    g_stPitmrIRQ[irqIndex].parameter = userChannelConfig->parameter;

    /* Registration interrupt */
    type.trig_mode = CLIC_LEVEL_TRIGGER;
    type.lvl       = 1;
    type.priority  = 0;
    type.data_ptr  = (void *)irqIndex;
    OS_RequestIrq(g_stPitmrIrqId[irqIndex], PITMR_IRQHandler, &type);

    /* Enable PCTMR instance's interrupts */
    OS_EnableIrq(g_stPitmrIrqId[irqIndex]);

    /* Setups the timer channel chaining  */
    PITMR_SetTimerChannelChainCmd(base, channel, userChannelConfig->chainChannel);

    /*  Setups the timer channel operation mode */
    PITMR_SetTimerChannelModeCmd(base, channel, userChannelConfig->timerMode);

    if (userChannelConfig->periodUnits == PITMR_PERIOD_UNITS_MICROSECONDS) {
        /* Setups timer channel period in microsecond unit */
        reStatus = PITMR_DRV_SetTimerPeriodByUs(instance, channel, userChannelConfig->period);
    } else {
        /* Setups timer channel period in count unit */
        reStatus = PITMR_DRV_SetTimerPeriodByCount(instance, channel, userChannelConfig->period);
    }

    if (reStatus == STATUS_SUCCESS) {
        /* Setups the timer channel trigger source, trigger select, reload on trigger,
            stop on timeout, start on trigger and channel chaining */
        PITMR_SetTriggerSourceCmd(base, channel, userChannelConfig->triggerSource);
        PITMR_SetTriggerSelectCmd(base, channel, userChannelConfig->triggerSelect);
        PITMR_SetReloadOnTriggerCmd(base, channel, userChannelConfig->enableReloadOnTrigger);
        PITMR_SetStopOnInterruptCmd(base, channel, userChannelConfig->enableStopOnInterrupt);
        PITMR_SetStartOnTriggerCmd(base, channel, userChannelConfig->enableStartOnTrigger);
        /* Setups interrupt generation for timer channel */
        if (userChannelConfig->isInterruptEnabled) {
            /* Enables interrupt generation */
            PITMR_EnableInterruptTimerChannels(base, (uint32_t)1U << channel);
        } else {
            /* Disables interrupt generation */
            PITMR_DisableInterruptTimerChannels(base, (uint32_t)1U << channel);
        }
    }

    return reStatus;
}

/*
 *
 * Function Name : PITMR_DRV_StartTimerChannels.
 * Description   : Starts the timer channel counting.
 *
 */
status_t PITMR_DRV_StartTimerChannels(uint32_t instance, uint32_t mask)
{
    pitmr_type_t *base;
    status_t reStatus = STATUS_SUCCESS;

    /* checkout PITMR  instance and channel mask */
    reStatus = PITMR_CheckMask(instance, mask);
    if (STATUS_ERRORPAR == reStatus) {
        // log_err("instance or mask out of range");
        return reStatus;
    }

    base = g_pstPitmrBase[instance];

    /* Starts timer channel counting */
    PITMR_StartTimerChannels(base, mask);

    return reStatus;
}

/*
 *
 * Function Name : PITMR_DRV_StopTimerChannels.
 * Description   : Stop PITMR channel counting.
 *
 */
status_t PITMR_DRV_StopTimerChannels(uint32_t instance, uint32_t mask)
{
    pitmr_type_t *base;
    status_t reStatus = STATUS_SUCCESS;

    /* checkout PITMR  instance and channel mask */
    reStatus = PITMR_CheckMask(instance, mask);
    if (STATUS_ERRORPAR == reStatus) {
        // log_err("instance or mask out of range");
        return reStatus;
    }

    base = g_pstPitmrBase[instance];

    /* Stops timer channel from counting */
    PITMR_StopTimerChannels(base, mask);

    return reStatus;
}

/*
 *
 * Function Name : PITMR_DRV_SetTimerPeriodByUs.
 * Description   : Set PITMR channel period in microseconds..
 *
 */
status_t PITMR_DRV_SetTimerPeriodByUs(uint32_t instance, uint32_t channel, uint32_t periodUs)
{
    pitmr_type_t *base;
    pitmr_timer_modes_t timerMode;
    status_t reStatus = STATUS_SUCCESS;
    uint64_t count;

    /* checkout PITMR channel num */
    reStatus = PITMR_CheckChannel(instance, channel);
    if (STATUS_ERRORPAR == reStatus) {
        // log_err("instance or channel out of range");
        return reStatus;
    }

    /* Gets current functional clock frequency of PITMR instance and checks the functional clock of PITMR module */
    reStatus = CLOCK_SYS_GetFreq(g_stPitmrClkNames[instance], &g_stPitmrSourceClockFrequency[instance]);
    if ((STATUS_SUCCESS != reStatus) || (0U == g_stPitmrSourceClockFrequency[instance])) {
        // log_err("Fetch clock error");
        return reStatus;
    }

    base = g_pstPitmrBase[instance];

    /* Calculates the count value, assign it to timer channel counter register.*/
    count = ((uint64_t)periodUs) * g_stPitmrSourceClockFrequency[instance];
    count = (count / 1000000U) - 1U;
    /* Gets current timer channel operation mode */
    timerMode = PITMR_GetTimerChannelModeCmd(base, channel);
    /* Checks whether the count is valid with timer channel operation mode */
    if (count <= MAX_PERIOD_COUNT) {
        if (timerMode == PITMR_DUAL_PERIODIC_COUNTER) {
            if (count > MAX_PERIOD_COUNT_IN_DUAL_16BIT_MODE) {
                reStatus = STATUS_ERROR;
            } else {
                if (count > MAX_PERIOD_COUNT_16_BIT) {
                    /* Calculates the count value for dual 16 bit periodic counter mode */
                    count = ((count - (MAX_PERIOD_COUNT_16_BIT + 1U)) << 16U) | (MAX_PERIOD_COUNT_16_BIT);
                }
            }
        }
    } else {
        reStatus = STATUS_ERROR;
    }
    if (STATUS_SUCCESS == reStatus) {
        /* Sets the timer channel period in count unit */
        PITMR_SetTimerPeriodByCount(base, channel, (uint32_t)count);
    }
    return reStatus;
}

/*
 *
 * Function Name : PITMR_DRV_SetTimerPeriodInDual16ModeByUs.
 * Description   : Set the timer channel period in microseconds.
 *
 */
status_t PITMR_DRV_SetTimerPeriodInDual16ModeByUs(uint32_t instance, uint32_t channel, uint16_t periodHigh,
                                                  uint16_t periodLow)
{
    pitmr_type_t *base;
    status_t reStatus = STATUS_SUCCESS;
    uint64_t periodHighCount;
    uint64_t periodLowCount;
    uint64_t periodCount;

    /* checkout PITMR channel num */
    reStatus = PITMR_CheckChannel(instance, channel);
    if (STATUS_ERRORPAR == reStatus) {
        // log_err("instance or channel out of range");
        return reStatus;
    }

    /* Gets current functional clock frequency of PITMR instance */
    reStatus = CLOCK_SYS_GetFreq(g_stPitmrClkNames[instance], &g_stPitmrSourceClockFrequency[instance]);
    if ((STATUS_SUCCESS != reStatus) || (0U == g_stPitmrSourceClockFrequency[instance])) {
        // log_err("Fetch clock error");
        return reStatus;
    }

    base = g_pstPitmrBase[instance];

    /* Calculates the count value of 16 bit higher period.*/
    periodHighCount = ((uint64_t)periodHigh) * g_stPitmrSourceClockFrequency[instance];
    periodHighCount = (periodHighCount / 1000000U) - 1U;

    /* Calculates the count value of 16 bit lower period.*/
    periodLowCount = ((uint64_t)periodLow) * g_stPitmrSourceClockFrequency[instance];
    periodLowCount = (periodLowCount / 1000000U) - 1U;
    /* Checks whether the count is valid */
    if ((periodHighCount > MAX_PERIOD_COUNT_16_BIT) || (periodLowCount > MAX_PERIOD_COUNT_16_BIT)) {
        reStatus = STATUS_ERROR;
        // log_err("periodHigh or periodLowCount error");

    } else {
        periodCount = (periodHighCount << 16U) | periodLowCount;
        PITMR_SetTimerPeriodByCount(base, channel, (uint32_t)periodCount);
    }

    return reStatus;
}

/*
 *
 * Function Name : PITMR_DRV_GetTimerPeriodByUs.
 * Description   : Gets the timer channel period in microseconds.
 *
 */
uint64_t PITMR_DRV_GetTimerPeriodByUs(uint32_t instance, uint32_t channel)
{
    const pitmr_type_t *base;
    pitmr_timer_modes_t timerMode;
    uint64_t currentPeriod;
    status_t reStatus = STATUS_SUCCESS;

    /* checkout PITMR  instance and channel */
    reStatus = PITMR_CheckChannel(instance, channel);
    if (STATUS_ERRORPAR == reStatus) {
        // log_err("instance or channel out of range");
    }

    /* Gets current functional clock frequency of PITMR instance */
    reStatus = CLOCK_SYS_GetFreq(g_stPitmrClkNames[instance], &g_stPitmrSourceClockFrequency[instance]);
    if ((STATUS_SUCCESS != reStatus) || (0U == g_stPitmrSourceClockFrequency[instance])) {
        // log_err("Fetch clock error");
    }

    base = g_pstPitmrBase[instance];

    /* Gets current timer channel period in count.*/
    currentPeriod = PITMR_GetTimerPeriodByCount(base, channel);

    /* Gets current timer channel operation mode */
    timerMode = PITMR_GetTimerChannelModeCmd(base, channel);

    if (timerMode == PITMR_DUAL_PERIODIC_COUNTER) {
        if (currentPeriod > MAX_PERIOD_COUNT_16_BIT) {
            currentPeriod = (((currentPeriod >> 16U) + (currentPeriod & MAX_PERIOD_COUNT_16_BIT) + 2U) * 1000000U) /
                            g_stPitmrSourceClockFrequency[instance];
        } else {
            /* Converts period from count unit to microseconds unit for other modes */
            currentPeriod = ((currentPeriod + 1U) * 1000000U) / g_stPitmrSourceClockFrequency[instance];
        }
    } else {
        /* Converts period from count unit to microseconds unit for other modes */
        currentPeriod = ((currentPeriod + 1U) * 1000000U) / g_stPitmrSourceClockFrequency[instance];
    }

    return currentPeriod;
}

/*
 *
 * Function Name : PITMR_DRV_GetCurrentTimerUs.
 * Description   :  Gets the current timer channel counting value in microseconds.
 *
 */
uint64_t PITMR_DRV_GetCurrentTimerUs(uint32_t instance, uint32_t channel)
{
    const pitmr_type_t *base;
    pitmr_timer_modes_t timerMode;
    uint64_t currentTime = 0U;
    status_t reStatus    = STATUS_SUCCESS;

    /* checkout PITMR  instance and channel */
    reStatus = PITMR_CheckChannel(instance, channel);
    if (STATUS_ERRORPAR == reStatus) {
        // log_err("instance or channel out of range");
    }

    /* Gets current functional clock frequency of PITMR instance */
    reStatus = CLOCK_SYS_GetFreq(g_stPitmrClkNames[instance], &g_stPitmrSourceClockFrequency[instance]);
    if ((STATUS_SUCCESS != reStatus) || (0U == g_stPitmrSourceClockFrequency[instance])) {
        // log_err("Fetch clock error");
    }

    base = g_pstPitmrBase[instance];

    /* Gets current timer channel counting value */
    currentTime = PITMR_GetCurrentTimerCount(base, channel);

    /* Gets current timer channel operation mode */
    timerMode = PITMR_GetTimerChannelModeCmd(base, channel);

    if (timerMode == PITMR_DUAL_PERIODIC_COUNTER) {
        currentTime = (((currentTime >> 16U) + (currentTime & MAX_PERIOD_COUNT_16_BIT)) * 1000000U) /
                      g_stPitmrSourceClockFrequency[instance];
    } else {
        /* Converts counting value to microseconds unit for other modes */
        currentTime = (currentTime * 1000000U) / g_stPitmrSourceClockFrequency[instance];
    }

    return currentTime;
}

/*
 *
 * Function Name : PITMR_DRV_SetTimerPeriodByCount.
 * Description   : Set PITMR channel period in count unit.
 *
 */
status_t PITMR_DRV_SetTimerPeriodByCount(uint32_t instance, uint32_t channel, uint32_t count)
{
    pitmr_type_t *base;

    status_t reStatus = STATUS_SUCCESS;

    /* checkout PITMR  instance and channel */
    reStatus = PITMR_CheckChannel(instance, channel);
    if (STATUS_ERRORPAR == reStatus) {
        // log_err("instance or channel out of range");
        return reStatus;
    }

    base = g_pstPitmrBase[instance];

    /* Sets the timer channel period in count unit */
    PITMR_SetTimerPeriodByCount(base, channel, count);

    return reStatus;
}

/*
 *
 * Function Name : PITMR_DRV_SetTimerPeriodInDual16ModeByCount.
 * Description   : Sets the timer channel period in count unit.
 *
 */
status_t PITMR_DRV_SetTimerPeriodInDual16ModeByCount(uint32_t instance, uint32_t channel, uint16_t periodHigh,
                                                     uint16_t periodLow)
{
    pitmr_type_t *base;
    uint32_t period;
    status_t reStatus = STATUS_SUCCESS;

    /* checkout PITMR  instance and channel */
    reStatus = PITMR_CheckChannel(instance, channel);
    if (STATUS_ERRORPAR == reStatus) {
        // log_err("instance or channel out of range");
        return reStatus;
    }

    base = g_pstPitmrBase[instance];

    period = ((uint32_t)periodHigh << 16U) | periodLow;

    /* Sets the timer channel period in count unit */
    PITMR_SetTimerPeriodByCount(base, channel, period);

    return reStatus;
}

/*
 *
 * Function Name : PITMR_DRV_GetTimerPeriodByCount.
 * Description   : Gets the user-configured register value in count unit.
 *
 */
uint32_t PITMR_DRV_GetTimerPeriodByCount(uint32_t instance, uint32_t channel)
{
    const pitmr_type_t *base;
    pitmr_timer_modes_t timerMode;
    uint32_t currentPeriod;
    status_t reStatus = STATUS_SUCCESS;

    /* checkout PITMR  instance and channel */
    reStatus = PITMR_CheckChannel(instance, channel);
    if (STATUS_ERRORPAR == reStatus) {
        // log_err("instance or channel out of range");
    }

    base = g_pstPitmrBase[instance];

    /* Gets current timer channel period by count.*/
    currentPeriod = PITMR_GetTimerPeriodByCount(base, channel);

    /* Gets current timer channel operation mode */
    timerMode = PITMR_GetTimerChannelModeCmd(base, channel);

    if (timerMode == PITMR_DUAL_PERIODIC_COUNTER) {
        /* Calculates the period for dual 16 bit periodic counter mode */
        currentPeriod = (currentPeriod >> 16U) + (currentPeriod & MAX_PERIOD_COUNT_16_BIT);
    }
    return currentPeriod;
}

/*
 *
 * Function Name : PITMR_DRV_GetCurrentTimerCount.
 * Description   : Gets the current timer channel counting value in count.
 *
 */
uint32_t PITMR_DRV_GetCurrentTimerCount(uint32_t instance, uint32_t channel)
{
    const pitmr_type_t *base;
    pitmr_timer_modes_t timerMode;
    uint32_t currentTime = 0;
    status_t reStatus    = STATUS_SUCCESS;

    /* checkout PITMR  instance and channel */
    reStatus = PITMR_CheckChannel(instance, channel);
    if (STATUS_ERRORPAR == reStatus) {
        // log_err("instance or channel out of range");
    }

    base = g_pstPitmrBase[instance];

    /* Gets current timer channel counting value */
    currentTime = PITMR_GetCurrentTimerCount(base, channel);

    /* Gets current timer channel operation mode */
    timerMode = PITMR_GetTimerChannelModeCmd(base, channel);

    if (PITMR_DUAL_PERIODIC_COUNTER == timerMode) {
        /* Calculates the current counting value for dual 16 bit periodic counter mode */
        currentTime = (currentTime >> 16U) + (currentTime & MAX_PERIOD_COUNT_16_BIT);
    }

    return currentTime;
}

/*
 *
 * Function Name : PITMR_DRV_EnableTimerChannelInterrupt.
 * Description   : Enable PITMR channel interrupt.
 *
 */
status_t PITMR_DRV_EnableTimerChannelInterrupt(uint32_t instance, uint32_t mask)
{
    pitmr_type_t *base;
    status_t reStatus = STATUS_SUCCESS;

    /* checkout PITMR  instance and channel mask */
    reStatus = PITMR_CheckMask(instance, mask);
    if (STATUS_ERRORPAR == reStatus) {
        // log_err("instance or mask out of range");
        return reStatus;
    }

    base = g_pstPitmrBase[instance];

    /* Enable interrupt of timer channels */
    PITMR_EnableInterruptTimerChannels(base, mask);

    return reStatus;
}

/*
 *
 * Function Name : PITMR_DRV_DisableTimerChannelInterrupt.
 * Description   : Disable PITMR interrupt.
 *
 */
status_t PITMR_DRV_DisableTimerChannelInterrupt(uint32_t instance, uint32_t mask)
{
    pitmr_type_t *base;
    status_t reStatus = STATUS_SUCCESS;

    /* checkout PITMR  instance and channel mask */
    reStatus = PITMR_CheckMask(instance, mask);
    if (STATUS_ERRORPAR == reStatus) {
        // log_err("instance or mask out of range");
        return reStatus;
    }

    base = g_pstPitmrBase[instance];

    /* Disable interrupt of timer channels */
    PITMR_DisableInterruptTimerChannels(base, mask);

    return reStatus;
}

/*
 *
 * Function Name : PITMR_DRV_GetInterruptFlagTimerChannels.
 * Description   : Gets the current interrupt flag of timer channels.
 *
 */
uint32_t PITMR_DRV_GetInterruptFlagTimerChannels(uint32_t instance, uint32_t mask)
{
    const pitmr_type_t *base;
    uint32_t reValue  = 0;
    status_t reStatus = STATUS_SUCCESS;

    reStatus = PITMR_CheckMask(instance, mask);
    if (STATUS_ERRORPAR == reStatus) {
        // log_err("instance or mask out of range");
    }

    base = g_pstPitmrBase[instance];

    /* Gets the interrupt flag for timer channels */
    reValue = PITMR_GetInterruptFlagTimerChannels(base, mask);

    return reValue;
}

/*
 *
 * Function Name : PITMR_DRV_ClearInterruptFlagTimerChannels
 * Description   : Clear interrupt flag of PITMR channel.
 *
 */
status_t PITMR_DRV_ClearInterruptFlagTimerChannels(uint32_t instance, uint32_t mask)
{
    pitmr_type_t *base;

    status_t reStatus = STATUS_SUCCESS;

    /* checkout PITMR instance and channel mask */
    reStatus = PITMR_CheckMask(instance, mask);
    if (STATUS_ERRORPAR == reStatus) {
        // log_err("instance or mask out of range");
        return reStatus;
    }

    base = g_pstPitmrBase[instance];

    /* Clears the interrupt flag for timer channels */
    PITMR_ClearInterruptFlagTimerChannels(base, mask);

    return reStatus;
}
