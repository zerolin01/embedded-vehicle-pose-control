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
 * @file pctmr_driver.c
 * @brief PCTMR driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-12
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "pctmr_driver.h"
#include "clock_driver.h"
#include "pctmr_hw_access.h"

/** Table of base addresses for PCTMR instances */
static pctmr_type_t *const g_pstPctmrBase[PCTMR_INSTANCE_COUNT] = PCTMR_BASE_PTRS;

/** Table to save  PCTMR IRQ enum numbers. */
static const IRQn_Type g_stPctmrIrqId[PCTMR_INSTANCE_COUNT] = PCTMR_IRQS;

static const clock_names_t g_stPctmrClkNames[PCTMR_INSTANCE_COUNT] = FEATURE_PCTMR_CLOCKS_NAMES;

/** Interrupt of the configuration structure **/
typedef struct {
    pctmr_callback_t callBack;
    void *parameter;
} pctmr_irq_t;

static pctmr_irq_t g_stPctmrIRQ[PCTMR_INSTANCE_COUNT];

/*
 *
 * Function Name : PCTMR_IRQHandler
 * Description   : interrupt handling functions
 *
 */
static void PCTMR_IRQHandler(void *para)
{
    uint32_t instance = (uint32_t)para;

    if (NULL != g_stPctmrIRQ[instance].callBack) {
        g_stPctmrIRQ[instance].callBack(g_stPctmrIRQ[instance].parameter);
    } else {
    }

    PCTMR_DRV_ClearIntFlag(instance);
}

/*
 *
 * Function Name : PCTMR_DRV_InstallCallback.
 * Description   : Registers the callback function and the parameter for PCTMR.
 * 
 */
status_t PCTMR_DRV_InstallCallback(uint32_t instance, pctmr_callback_t callback, void *parameter)
{

    if ((NULL == callback) || (instance > 9)) {
        return STATUS_ERRORPAR;
    }

    g_stPctmrIRQ[instance].callBack  = callback;
    g_stPctmrIRQ[instance].parameter = parameter;

    return STATUS_SUCCESS;
}

/*
 *
 * Function Name : PCTMR_DRV_InitConfigStruct
 * Description   : Initialize a configuration structure with default values.
 *
 */
void PCTMR_DRV_InitConfigStruct(pctmr_config_t *const config)
{
    OS_ASSERT(config != NULL);

    /* General parameters */
    config->dmaRequest      = false;
    config->interruptEnable = false;
    config->freeRun         = false;
    config->workMode        = PCTMR_WORKMODE_TIMER;

    /* Counter parameters */
    config->clockSelect     = PCTMR_CLOCKSOURCE_SROSC;
    config->prescaler       = PCTMR_PRESCALE_2;
    config->bypassPrescaler = false;
    config->compareValue    = 2000U;
    config->counterUnits    = PCTMR_COUNTER_UNITS_TICKS;

    /* Pulse Counter specific parameters */
    config->pinSelect   = PCTMR_PINSELECT_TRGMUX;
    config->pinPolarity = PCTMR_PINPOLARITY_RISING;
    config->parameter   = NULL;
    config->callBack    = NULL;
}

/*
 *
 * Function Name : PCTMR_DRV_Init
 * Description   : Initialize a PCTMR instance with values from an input configuration structure.
 * When (counterUnits == PCTMR_COUNTER_UNITS_MICROSECONDS) the function will
 * automatically configure the timer for the input compareValue in microseconds.
 * The input parameters for 'prescaler' and 'bypassPrescaler' will be ignored
 * - their values will be adapted by the function, to best fit the input compareValue
 * (in microseconds) for the operating clock frequency.
 *
 * PCTMR_COUNTER_UNITS_MICROSECONDS may only be used for PCTMR_WORKMODE_TIMER mode.
 * Otherwise the function shall not convert 'compareValue' in ticks
 * and this is likely to cause erroneous behavior.
 *
 * When (counterUnits == PCTMR_COUNTER_UNITS_TICKS) the function will use the
 * 'prescaler' and 'bypassPrescaler' provided in the input configuration structure.
 *
 * When (counterUnits == PCTMR_COUNTER_UNITS_TICKS), 'compareValue' must be lower
 * than 0xFFFFU. Only the least significant 16bits of 'compareValue' will be used.
 * When (counterUnits == PCTMR_COUNTER_UNITS_MICROSECONDS), 'compareValue'
 * may take any 32bits unsigned value.
 *
 */
void PCTMR_DRV_Init(const uint32_t instance, const pctmr_config_t *const config)
{

    OS_ASSERT(instance < PCTMR_INSTANCE_COUNT);
    OS_ASSERT(config != NULL);

    /* Save the relevant parameters needed for the interrupt */
    register_type_t type;

    /* Enable PCTMR clock */
    CLOCK_SYS_ClockEnable(g_stPctmrClkNames[instance], true);

    g_stPctmrIRQ[instance].callBack  = config->callBack;
    g_stPctmrIRQ[instance].parameter = config->parameter;

    /* Registration interrupt */
    type.trig_mode = CLIC_LEVEL_TRIGGER;
    type.lvl       = 1;
    type.priority  = 0;
    type.data_ptr  = (void *)instance;
    OS_RequestIrq(g_stPctmrIrqId[instance], PCTMR_IRQHandler, &type);

    /* Enable PCTMR instance's interrupts */
    OS_EnableIrq(g_stPctmrIrqId[instance]);

    PCTMR_DRV_SetConfig(instance, config);
}

/*
 *
 * Function Name : PCTMR_DRV_SetConfig
 * Description   : When (counterUnits == PCTMR_COUNTER_UNITS_MICROSECONDS) the function will
 * automatically configure the timer for the input compareValue in microseconds.
 * The input parameters for 'prescaler' and 'bypassPrescaler' will be ignored
 * - their values will be adapted by the function, to best fit the input compareValue
 * (in microseconds) for the operating clock frequency.
 *
 * PCTMR_COUNTER_UNITS_MICROSECONDS may only be used for PCTMR_WORKMODE_TIMER mode.
 * Otherwise the function shall not convert 'compareValue' in ticks
 * and this is likely to cause erroneous behavior.
 *
 * When (counterUnits == PCTMR_COUNTER_UNITS_TICKS) the function will use the
 * 'prescaler' and 'bypassPrescaler' provided in the input configuration structure.
 *
 * When (counterUnits == PCTMR_COUNTER_UNITS_TICKS), 'compareValue' must be lower
 * than 0xFFFFU. Only the least significant 16bits of 'compareValue' will be used.
 * When (counterUnits == PCTMR_COUNTER_UNITS_MICROSECONDS), 'compareValue'
 * may take any 32bits unsigned value.
 *
 */

status_t PCTMR_DRV_SetConfig(const uint32_t instance, const pctmr_config_t *const config)
{
    OS_ASSERT(instance < PCTMR_INSTANCE_COUNT);
    OS_ASSERT(config != NULL);

    pctmr_type_t *const base                 = g_pstPctmrBase[instance];
    uint32_t configCmpValue                  = config->compareValue;
    pctmr_workmode_t configWorkMode          = config->workMode;
    uint16_t cmpValueTicks                   = 0U;
    pctmr_prescaler_t prescVal               = config->prescaler;
    bool prescBypass                         = config->bypassPrescaler;
    pctmr_counter_units_t configCounterUnits = config->counterUnits;
    status_t statusCode                      = STATUS_SUCCESS;

    if (configWorkMode == PCTMR_WORKMODE_TIMER) {
        /* A valid clock must be selected when used in Timer Mode. */
        uint32_t clkFreq;
        clkFreq = PCTMR_GetClkFreq(config->clockSelect, instance);
        /* Clock frequency equal to '0', signals invalid value.*/
        if (0U == clkFreq) {
            statusCode = STATUS_ERRORCLK;
            return statusCode;
        }
        if (configCounterUnits == PCTMR_COUNTER_UNITS_MICROSECONDS) {
            bool chooseClkConfigStatus;

            /* When workmode is set to Timer Mode and compare value is provided in microseconds,
             * then the input parameters for prescale value and prescaleBypass are ignored.
             * The prescaleValue, prescaleBypass and cmpValue in ticks, are calculated to best fit
             * the input configCmpValue (in us) for the current operating clk frequency.  */
            chooseClkConfigStatus =
                PCTMR_ChooseClkConfig(clkFreq, configCmpValue, &prescVal, &prescBypass, &cmpValueTicks);
            if (!chooseClkConfigStatus) {
                statusCode = STATUS_ERROR;
                return statusCode;
            }
        } else {
            if (PCTMR_COUNTER_UNITS_TICKS != configCounterUnits) {
                statusCode = STATUS_ERRORPAR;
                return statusCode;
            }
            /* Compare Value in Tick Units must fit in CMR.*/
            if (configCmpValue > PCTMR_CMR_COMPARE_MASK) {
                statusCode = STATUS_ERRORPAR;
                return statusCode;
            }

            cmpValueTicks = (uint16_t)(configCmpValue & PCTMR_CMR_COMPARE_MASK);
        }
    } else {
        /* If configWorkMode is not PCTMR_WORKMODE_TIMER, then it must be PCTMR_WORKMODE_PULSECOUNTER. Only
         * PCTMR_COUNTER_UNITS_TICKS can be used when PCTMR is configured as Pulse Counter. enable glitch filter */
        if ((PCTMR_WORKMODE_PULSECOUNTER != configWorkMode) || (PCTMR_COUNTER_UNITS_TICKS != config->counterUnits) ||
            (!prescBypass)) {
            statusCode = STATUS_ERRORPAR;
            return statusCode;
        }
        /* A valid clock must be selected when glitch filter is enabled */
        if (0u == (PCTMR_GetClkFreq(config->clockSelect, instance))) {
            statusCode = STATUS_ERRORCLK;
            return statusCode;
        }
        /* Glitch filter does not support PCTMR_PRESCALE_2. */
        if ((PCTMR_PRESCALE_2 == prescVal) && (!prescBypass)) {
            statusCode = STATUS_ERRORPAR;
            return statusCode;
        }

        /*Compare Value in Tick Units must fit in CMR.*/
        if (configCmpValue > PCTMR_CMR_COMPARE_MASK) {
            statusCode = STATUS_ERRORPAR;
            return statusCode;
        }

        cmpValueTicks = (uint16_t)(configCmpValue & PCTMR_CMR_COMPARE_MASK);
    }

    /* Clear the PCTMR register */
    PCTMR_Reset(base);

    PCTMR_SetDmaRequest(base, config->dmaRequest);
    PCTMR_SetInterrupt(base, config->interruptEnable);
    PCTMR_SetFreeRunning(base, config->freeRun);
    PCTMR_SetWorkMode(base, configWorkMode);
    PCTMR_SetPrescaler(base, prescVal);
    PCTMR_SetBypass(base, prescBypass);
    PCTMR_SetClockSelect(base, config->clockSelect);
    PCTMR_SetCompareValue(base, cmpValueTicks);
    PCTMR_SetPinSelect(base, config->pinSelect);
    PCTMR_SetPinPolarity(base, config->pinPolarity);

    return statusCode;
}

/*
 *
 * Function Name : PCTMR_DRV_GetConfig
 * Description   : Get the current configuration of a PCTMR instance.
 *
 */
void PCTMR_DRV_GetConfig(const uint32_t instance, pctmr_config_t *const config)
{
    OS_ASSERT(instance < PCTMR_INSTANCE_COUNT);
    OS_ASSERT(config != NULL);

    const pctmr_type_t *const base = g_pstPctmrBase[instance];

    /* Read current configuration */
    config->dmaRequest      = PCTMR_GetDmaRequest(base);
    config->interruptEnable = PCTMR_GetInterruptEnable(base);
    config->freeRun         = PCTMR_GetFreeRunning(base);
    config->workMode        = PCTMR_GetWorkMode(base);
    config->prescaler       = PCTMR_GetPrescaler(base);
    config->bypassPrescaler = PCTMR_GetBypass(base);
    config->clockSelect     = PCTMR_GetClockSelect(base);
    config->compareValue    = PCTMR_GetCompareValue(base);
    config->counterUnits    = PCTMR_COUNTER_UNITS_TICKS;
    config->pinSelect       = PCTMR_GetPinSelect(base);
    config->pinPolarity     = PCTMR_GetPinPolarity(base);
    config->callBack        = NULL;
    config->parameter       = NULL;
}

/*
 *
 * Function Name : PCTMR_DRV_Deinit
 * Description   :  De-initialize a PCTMR instance.
 *
 */
void PCTMR_DRV_Deinit(const uint32_t instance)
{
    OS_ASSERT(instance < PCTMR_INSTANCE_COUNT);

    pctmr_type_t *const base = g_pstPctmrBase[instance];

    /* Disable PCTMR IRQ in NVIC level */
    OS_DisableIrq(g_stPctmrIrqId[instance]);

    /* Destroy registration function */
    OS_UnRequestIrq(g_stPctmrIrqId[instance]);

    PCTMR_Disable(base);

    PCTMR_Reset(base);
}

/*
 *
 * Function Name : PCTMR_DRV_SetCompareValueByCount
 * Description   : Set the compare value in counter tick units, for a PCTMR instance.
 *
 */
status_t PCTMR_DRV_SetCompareValueByCount(const uint32_t instance, const uint16_t compareValueByCount)
{
    OS_ASSERT(instance < PCTMR_INSTANCE_COUNT);

    pctmr_type_t *const base = g_pstPctmrBase[instance];
    status_t statusCode      = STATUS_SUCCESS;

    bool timerEnabled = PCTMR_GetEnable(base);
    bool compareFlag  = PCTMR_GetIntFlag(base);

    uint16_t counterVal;

    /* Check if a valid clock is selected for the timer/glitch filter */

    bool bypass               = PCTMR_GetBypass(base);
    pctmr_workmode_t workMode = PCTMR_GetWorkMode(base);

    if (0u == PCTMR_GetClkFreq(PCTMR_GetClockSelect(base), instance)) {
        statusCode = STATUS_ERRORCLK;
        return statusCode;
    }
    /*bypass must be false when use PCTMR pulse mode */
    if (bypass && (workMode == PCTMR_WORKMODE_PULSECOUNTER)) {
        statusCode = STATUS_ERRORPAR;
        return statusCode;
    }

    /* The compare value can only be written if counter is disabled or the compare flag is set. */
    if (timerEnabled && !compareFlag) {
        statusCode = STATUS_ERROR;
    } else {
        /* Check if new value is below the current counter value */
        PCTMR_SetCompareValue(base, compareValueByCount);
        counterVal = PCTMR_GetCounterValue(base);
        if (counterVal >= compareValueByCount) {
            statusCode = STATUS_TIMEOUT;
        }
    }

    return statusCode;
}

/*
 *
 * Function Name : PCTMR_DRV_GetCompareValueByCount
 * Description   : Get the compare value in counter tick units, of a PCTMR instance.
 *
 */
void PCTMR_DRV_GetCompareValueByCount(const uint32_t instance, uint16_t *const compareValueByCount)
{
    OS_ASSERT(instance < PCTMR_INSTANCE_COUNT);

    const pctmr_type_t *const base = g_pstPctmrBase[instance];

    *compareValueByCount = PCTMR_GetCompareValue(base);
}

/*
 *
 * Function Name : PCTMR_DRV_SetCompareValueByUs
 * Description   : Set the compare value for Timer Mode in microseconds, for a PCTMR instance.
 *
 */
status_t PCTMR_DRV_SetCompareValueByUs(const uint32_t instance, const uint32_t compareValueUs)
{
    OS_ASSERT(instance < PCTMR_INSTANCE_COUNT);

    status_t returnCode      = STATUS_SUCCESS;
    pctmr_type_t *const base = g_pstPctmrBase[instance];
    bool timerEnabled, compareFlag;

    pctmr_clocksource_t clkSrc;
    uint32_t clkFreq;
    uint16_t cmpValTicks, currentCounterVal;
    pctmr_prescaler_t prescVal;
    bool prescBypass, conversionStatus;
    pctmr_workmode_t pctmrMode;

    /* This function can only be used if PCTMR is configured in Timer Mode. */
    pctmrMode = PCTMR_GetWorkMode(base);
    if (PCTMR_WORKMODE_TIMER != pctmrMode) {
        returnCode = STATUS_ERROR;
        return returnCode;
    }

    timerEnabled = PCTMR_GetEnable(base);
    compareFlag  = PCTMR_GetIntFlag(base);
    /* The compare value can only be written if counter is disabled or the compare flag is set. */
    if (timerEnabled && !compareFlag) {
        returnCode = STATUS_ERROR;
    } else {
        clkSrc  = PCTMR_GetClockSelect(base);
        clkFreq = PCTMR_GetClkFreq(clkSrc, instance);
        if (0 == clkFreq) {
            returnCode = STATUS_ERRORCLK;
            return returnCode;
        }

        /* Get prescaler value and prescaler bypass state.*/
        prescVal    = PCTMR_GetPrescaler(base);
        prescBypass = PCTMR_GetBypass(base);
        /* Convert new compare value from microseconds to ticks. */
        conversionStatus = PCTMR_Us2Ticks(clkFreq, prescVal, prescBypass, compareValueUs, &cmpValTicks);
        /* Check the conversion status: compareValueUs doesn't fit for current prescaler. */
        if (!conversionStatus) {
            returnCode = STATUS_ERROR;
            return returnCode;
        }
        /* Write value and check if written successfully */
        PCTMR_SetCompareValue(base, cmpValTicks);
        currentCounterVal = PCTMR_GetCounterValue(base);

        if (currentCounterVal >= cmpValTicks) {
            returnCode = STATUS_TIMEOUT;
        }
    }

    return returnCode;
}

/*
 *
 * Function Name : PCTMR_DRV_GetCompareValueByUs
 * Description   : Get the compare value in microseconds, of a PCTMR instance.
 *
 */
status_t PCTMR_DRV_GetCompareValueByUs(const uint32_t instance, uint32_t *const compareValueUs)
{
    OS_ASSERT(instance < PCTMR_INSTANCE_COUNT);
    OS_ASSERT(compareValueUs != NULL);

    status_t statusCode = STATUS_SUCCESS;

    const pctmr_type_t *const base = g_pstPctmrBase[instance];
    pctmr_clocksource_t clkSrc;
    pctmr_workmode_t pctmrMode;
    uint32_t clkFreq;
    uint16_t cmpValTicks;
    pctmr_prescaler_t prescVal;
    bool prescBypass, conversionStatus;

    /* This function can only be used if PCTMR is configured in Timer Mode. */
    pctmrMode = PCTMR_GetWorkMode(base);
    if (PCTMR_WORKMODE_TIMER != pctmrMode) {
        statusCode = STATUS_ERROR;
        return statusCode;
    }

    clkSrc  = PCTMR_GetClockSelect(base);
    clkFreq = PCTMR_GetClkFreq(clkSrc, instance);
    /* The clock frequency must be valid. */
    if (0U == clkFreq) {
        statusCode = STATUS_ERRORCLK;
        return statusCode;
    }
    /* Get prescaler value and prescaler bypass state.*/
    prescVal    = PCTMR_GetPrescaler(base);
    prescBypass = PCTMR_GetBypass(base);
    cmpValTicks = PCTMR_GetCompareValue(base);

    /* Convert current compare value from ticks to microseconds. */
    conversionStatus = PCTMR_Ticks2Us(clkFreq, prescVal, prescBypass, cmpValTicks, compareValueUs);
    if (false == conversionStatus) {
        statusCode = STATUS_ERROR;
    }
    return statusCode;
}

/*
 *
 * Function Name : PCTMR_DRV_GetIntFlag
 * Description   :Get the current state of the Compare Flag of a PCTMR instance.
 *
 */
bool PCTMR_DRV_GetIntFlag(const uint32_t instance)
{
    OS_ASSERT(instance < PCTMR_INSTANCE_COUNT);

    const pctmr_type_t *const base = g_pstPctmrBase[instance];

    bool compareFlag = PCTMR_GetIntFlag(base);

    return compareFlag;
}

/*
 *
 * Function Name : PCTMR_DRV_ClearIntFlag
 * Description   : Clear the Compare Flag of a PCTMR instance.
 *
 */
void PCTMR_DRV_ClearIntFlag(const uint32_t instance)
{
    OS_ASSERT(instance < PCTMR_INSTANCE_COUNT);

    pctmr_type_t *const base = g_pstPctmrBase[instance];

    PCTMR_ClearIntFlag(base);
}

/*
 *
 * Function Name : PCTMR_DRV_IsRunning
 * Description   : Get the run state of a PCTMR instance.
 *
 */
bool PCTMR_DRV_IsRunning(const uint32_t instance)
{
    OS_ASSERT(instance < PCTMR_INSTANCE_COUNT);

    const pctmr_type_t *const base = g_pstPctmrBase[instance];

    bool runningState = PCTMR_GetEnable(base);

    return runningState;
}

/*
 *
 * Function Name : PCTMR_DRV_SetInterrupt
 * Description   : Enable/disable the PCTMR interrupt.
 *
 */
void PCTMR_DRV_SetInterrupt(const uint32_t instance, const bool enableInterrupt)
{
    OS_ASSERT(instance < PCTMR_INSTANCE_COUNT);

    pctmr_type_t *const base = g_pstPctmrBase[instance];

    PCTMR_SetInterrupt(base, enableInterrupt);
}

/*
 *
 * Function Name : PCTMR_DRV_GetCounterValueByCount
 * Description   : Get the current counter value in counter tick units.
 *
 */
uint16_t PCTMR_DRV_GetCounterValueByCount(const uint32_t instance)
{
    OS_ASSERT(instance < PCTMR_INSTANCE_COUNT);

    pctmr_type_t *const base = g_pstPctmrBase[instance];

    uint16_t counterVal = PCTMR_GetCounterValue(base);

    return counterVal;
}

/*
 *
 * Function Name : PCTMR_DRV_StartCounter
 * Description   : Enable PCTMR.
 *
 */
status_t PCTMR_DRV_StartCounter(const uint32_t instance)
{
    OS_ASSERT(instance < PCTMR_INSTANCE_COUNT);

    status_t statusCode = STATUS_SUCCESS;

    pctmr_type_t *const base = g_pstPctmrBase[instance];

    /* Check if a valid clock is selected for the timer/glitch filter */
    bool bypass               = PCTMR_GetBypass(base);
    pctmr_workmode_t workMode = PCTMR_GetWorkMode(base);

    if (0u == PCTMR_GetClkFreq(PCTMR_GetClockSelect(base), instance)) {
        statusCode = STATUS_ERRORCLK;
        return statusCode;
    }

    /*bypass must be false when use PCTMR pulse mode */
    if (!bypass && (workMode == PCTMR_WORKMODE_PULSECOUNTER)) {
        statusCode = STATUS_ERRORPAR;
        return statusCode;
    }

    /*Enable PCTMR*/
    PCTMR_Enable(base);

    return statusCode;
}

/*
 *
 * Function Name : PCTMR_DRV_StopCounter.
 * Description   : Disable the PCTMR / Stop the counter.
 *
 */
void PCTMR_DRV_StopCounter(const uint32_t instance)
{
    OS_ASSERT(instance < PCTMR_INSTANCE_COUNT);

    pctmr_type_t *const base = g_pstPctmrBase[instance];

    PCTMR_Disable(base);
}

/*
 *
 * Function Name : PCTMR_DRV_SetPinConfiguration.
 * Description   : Set the Input Pin configuration for Pulse Counter mode.
 *
 */
void PCTMR_DRV_SetPinConfiguration(const uint32_t instance, const pctmr_pinselect_t pinSelect,
                                   const pctmr_pinpolarity_t pinPolarity)
{
    OS_ASSERT(instance < PCTMR_INSTANCE_COUNT);

    pctmr_type_t *const base = g_pstPctmrBase[instance];

    PCTMR_SetPinSelect(base, pinSelect);
    PCTMR_SetPinPolarity(base, pinPolarity);
}
