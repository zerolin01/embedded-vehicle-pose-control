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
 * @file rtc_driver.c
 * @brief RTC driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-13
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "rtc_hw_access.h"

/*
 * @brief Table of RTC base pointers
 */
static rtc_type_t *const g_pstRtcBase[RTC_INSTANCE_COUNT] = RTC_BASE_PTRS;

/*
 * @brief Table used to store the RTC IRQ names
 */
static const IRQn_Type g_stRtcIrqId[RTC_INSTANCE_COUNT] = RTC_IRQS;

/*
 * @brief Table of month length (in days) for the Un-leap-year
 */
static const uint8_t ULY[] = {0U, 31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U};

/*
 * @brief Table of month length (in days) for the Leap-year
 */
static const uint8_t LY[] = {0U, 31U, 29U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U};

/*
 * @brief Number of days from begin of the non Leap-year
 */
static const uint16_t MONTH_DAYS[] = {0U, 0U, 31U, 59U, 90U, 120U, 151U, 181U, 212U, 243U, 273U, 304U, 334U};

/*
 * @brief static RTC runtime structure, it is designed only for internal
 * purposes such as storing interrupt configuration for each instance.
 */
static struct {
    rtc_alarm_config_t *alarmConfig;         /*Alarm configuration                   */
    rtc_samping_config_t *sampingConfig;     /*samping Interrupt configuration        */
    rtc_stopwatch_config_t *stopwatchConfig; /*Time stopwatch interrupt configuration */
} g_rtcRuntimeConfig_t;

/*
 *
 * Function Name : RTC_IRQCallback
 * Description   : This method is the API's interrupt handler for generic, alarm and 
 *                 samping IRQ.it will calls the user callbacks if they are not NULL.
 *
 */
void RTC_IRQCallback(uint32_t instance, uint32_t num)
{
    rtc_samping_config_t *sampingConfig     = g_rtcRuntimeConfig_t.sampingConfig;
    rtc_alarm_config_t *alarmConfig         = g_rtcRuntimeConfig_t.alarmConfig;
    rtc_stopwatch_config_t *stopwatchConfig = g_rtcRuntimeConfig_t.stopwatchConfig;

    switch (num) {
    case RTC_INT_SW:
        if (stopwatchConfig->rtcStopwatchCallback != NULL)
        /* If the user has defined a callback, call it */
        {
            stopwatchConfig->rtcStopwatchCallback(stopwatchConfig->callbackParams);
        }
        break;
    case RTC_INT_ALM:
        /* If the user has defined a callback, call it */
        if (alarmConfig->alarmCallback != NULL) {
            alarmConfig->alarmCallback(alarmConfig->callbackParams);
        }
        break;
    case RTC_INT_MIN:
    case RTC_INT_DAY:
    case RTC_INT_1HZ:
    case RTC_INT_HR:
    case RTC_INT_2HZ:
    case RTC_INT_SAM0:
    case RTC_INT_SAM1:
    case RTC_INT_SAM2:
    case RTC_INT_SAM3:
    case RTC_INT_SAM4:
    case RTC_INT_SAM5:
    case RTC_INT_SAM6:
    case RTC_INT_SAM7:
        /* If the user has defined a callback, call it */
        if (sampingConfig->rtcSampingCallback != NULL) {
            sampingConfig->rtcSampingCallback(sampingConfig->callbackParams);
        }
        break;
    default:
        //log_err("no that interrupt callback function\n");
        break;
    }
}

/*
 *
 * Function Name : RTC_DRV_InstallCallback.
 * Description   : Registers the callback function and the parameter for PDU.
 * 
 */
status_t RTC_DRV_InstallCallback(uint32_t instance, rtc_callback_t callback, void *parameter, rtc_irq_type_t irqType)
{
    status_t reStatus = STATUS_SUCCESS;

    if ((NULL == callback) || (instance >= 1)) {
        return STATUS_ERRORPAR;
    }

    switch (irqType) {
    case RTC_IRQ_ALARM:
        g_rtcRuntimeConfig_t.alarmConfig->alarmCallback  = callback;
        g_rtcRuntimeConfig_t.alarmConfig->callbackParams = parameter;
        break;

    case RTC_IRQ_STOPWATCH:
        g_rtcRuntimeConfig_t.stopwatchConfig->rtcStopwatchCallback = callback;
        g_rtcRuntimeConfig_t.stopwatchConfig->callbackParams       = parameter;
        break;

    case RTC_IRQ_SAMPLEING:
        g_rtcRuntimeConfig_t.sampingConfig->rtcSampingCallback = callback;
        g_rtcRuntimeConfig_t.sampingConfig->callbackParams     = parameter;

        break;
    default:
        break;
    }

    return reStatus;
}

/*
 *
 * Function Name : RTC_DRV_IRQHandler
 * Description   : This method is the API's interrupt handler for generic, alarm and 
 *                 samping IRQ.it will calls the user callbacks if they are not NULL.
 *
 */
void RTC_DRV_IRQHandler(void *data)
{
    uint32_t instance = *(uint32_t *)data;
    OS_ASSERT(instance < RTC_INSTANCE_COUNT);

    uint32_t i        = 0;
    uint32_t int_flag = 0;

    int_flag = RTC_Get_InterruptStatus(g_pstRtcBase[instance]);

    /* Check if an interrupt has occurred */
    if (int_flag > 0) {
        for (i = 0; i < 16; i++) {
            if (((int_flag >> i) & 0x1) > 0) {
                RTC_IRQCallback(instance, i);
                break;
            }
        }
        RTC_DRV_ClearInterrupt(instance);
    } else {
    }
}

/*
 *
 * Function Name : RTC_DRV_Init
 * Description   : This function initializes the RTC instance with the settings
 *                 provided by the user via the rtcUserCfg parameter.
 *
 */
status_t RTC_DRV_Init(uint32_t instance, const rtc_init_config_t *const rtcUserCfg)
{
    OS_ASSERT(instance < RTC_INSTANCE_COUNT);
    OS_ASSERT(rtcUserCfg != NULL);

    status_t statusCode = STATUS_SUCCESS;

    rtc_type_t *basePtr = g_pstRtcBase[instance];

    register_type_t type;

    /* Registration interrupt */
    type.trig_mode = CLIC_LEVEL_TRIGGER;
    type.lvl       = 1;
    type.priority  = 0;
    type.data_ptr  = NULL;
    OS_RequestIrq(g_stRtcIrqId[instance], RTC_DRV_IRQHandler, &type);

    /* Initialize runtime structure */
    g_rtcRuntimeConfig_t.alarmConfig     = NULL;
    g_rtcRuntimeConfig_t.stopwatchConfig = NULL;
    g_rtcRuntimeConfig_t.sampingConfig   = NULL;

    /* Disable the RTC instance IRQ to perform a software reset */
    OS_DisableIrq(g_stRtcIrqId[instance]);

    /* set rtc disable */
    RTC_Disable(basePtr);

    /* Perform a software reset */
    RTC_SoftwareReset(basePtr);

    /* Clear the pending interrupt generated by the software reset */
    CLIC_ClearPendingIRQ(g_stRtcIrqId[instance]);

    RTC_ResetRTCInterrupt(basePtr);

    /*Select RTC clock*/
    RTC_ClkSelect(basePtr, rtcUserCfg->clockSelect);

    /* Enable the RTC instance IRQ */
    OS_EnableIrq(g_stRtcIrqId[instance]);

    /* Return the exit code */
    return statusCode;
}

/*
 *
 * Function Name : RTC_DRV_Deinit
 * Description   :This function deinitialize the RTC instance.
 *
 */
status_t RTC_DRV_Deinit(uint32_t instance)
{
    OS_ASSERT(instance < RTC_INSTANCE_COUNT);

    status_t statusCode = STATUS_SUCCESS;

    rtc_type_t *basePtr = g_pstRtcBase[instance];

    /* Destroy registration function */
    OS_UnRequestIrq(g_stRtcIrqId[instance]);

    /* Disable RTC instance's interrupts */
    OS_DisableIrq(g_stRtcIrqId[instance]);

    /* set rtc disable */
    RTC_Disable(basePtr);

    /* Perform a software reset */
    RTC_SoftwareReset(g_pstRtcBase[instance]);
    /* Clear the pending interrupt generated by the software reset */
    CLIC_ClearPendingIRQ(g_stRtcIrqId[instance]);

    /* Return the exit code */
    return statusCode;
}

/*
 *
 * Function Name : RTC_DRV_GetDefaultConfig
 * Description   : This function will set the default configuration values into the structure passed as a parameter.
 *
 */
void RTC_DRV_GetDefaultConfig(rtc_init_config_t *const config)
{
    OS_ASSERT(config != NULL);

    config->clockSelect = RTC_CLK_SRC_OSC_32768HZ;
}

/*
 *
 * Function Name : RTC_DRV_StartCounter
 * Description   : you manually stop the RTC module, you need to call this function to restart.
 *
 */
void RTC_DRV_StartCounter(uint32_t instance)
{
    OS_ASSERT(instance < RTC_INSTANCE_COUNT);

    /* Enable the counter */
    RTC_Enable(g_pstRtcBase[instance]);
}

/*
 *
 * Function Name : RTC_DRV_StopCounter
 * Description   : Stop RTC.
 *
 */
void RTC_DRV_StopCounter(uint32_t instance)
{
    OS_ASSERT(instance < RTC_INSTANCE_COUNT);

    /* Disable the RTC instance */
    RTC_Disable(g_pstRtcBase[instance]);
}

/*
 *
 * Function Name : RTC_DRV_ConvertDaysToTimeDate
 * Description   : Convert days to rtc_timedate_t structure.
 *
 */
void RTC_DRV_ConvertDaysToTimeDate(rtc_timedate_t *const timeDate, const uint32_t *const days)
{
    OS_ASSERT(days != NULL);
    OS_ASSERT(timeDate != NULL);

    /* Declare the variables needed */
    uint8_t i;
    bool yearLeap         = false;
    uint32_t numberOfDays = 0U;
    // uint32_t tempSeconds;
    uint16_t daysInYear;

    /* Because the starting year(1970) is not leap, set the daysInYear
     * variable with the number of the days in a normal year
     */
    daysInYear = DAYS_IN_A_YEAR;

    /* Set the year to the beginning of the range */
    timeDate->year = YEAR_RANGE_START;

    /* Get the number of days */
    numberOfDays = *days;

    /* Get the current year */
    while (numberOfDays >= daysInYear) {
        /* Increment year if the number of days is greater than the ones in
         * one year
         */
        timeDate->year++;
        /* Subtract the number of the days */
        numberOfDays -= daysInYear;

        /* Check if the year is leap or unleap */
        if (!RTC_DRV_IsYearLeap(timeDate->year)) {
            /* Set the number of non leap year to the current year number
             * of days.
             */
            daysInYear = DAYS_IN_A_YEAR;
        } else {
            /* Set the number of leap year to the current year number
             * of days.
             */
            daysInYear = DAYS_IN_A_LEAP_YEAR;
        }
    }

    /* Add the current day */
    numberOfDays += 1U;

    /* Check if the current year is leap */
    yearLeap = RTC_DRV_IsYearLeap(timeDate->year);

    /* Get the month */
    for (i = 1U; i <= 12U; i++) {
        uint32_t daysInCurrentMonth = ((yearLeap == true) ? (uint32_t)LY[i] : (uint32_t)ULY[i]);
        if (numberOfDays <= daysInCurrentMonth) {
            timeDate->month = (uint16_t)i;
            break;
        } else {
            numberOfDays -= daysInCurrentMonth;
        }
    }

    /* Set the current day */
    timeDate->day = (uint16_t)numberOfDays;
}

/*
 *
 * Function Name : RTC_DRV_ConvertTimeDateToDays
 * Description   : Convert rtc_timedate_t structure to days.
 *
 */
void RTC_DRV_ConvertTimeDateToDays(const rtc_timedate_t *const timeDate, uint32_t *const days)
{
    OS_ASSERT(timeDate != NULL);
    OS_ASSERT(days != NULL);

    /* Declare local variables */
    uint16_t year;

    /* Convert years to seconds */
    (*days) = (uint32_t)(DAYS_IN_A_YEAR);
    (*days) *= ((uint32_t)timeDate->year - YEAR_RANGE_START);

    /* Add the day from the leap years */
    for (year = YEAR_RANGE_START; year < timeDate->year; year++) {
        if (RTC_DRV_IsYearLeap(year)) {
            (*days) += 1;
        }
    }

    /* If the current year is leap and 29th of February has passed, add
     * another day  passed.
     */
    if ((RTC_DRV_IsYearLeap(year)) && (timeDate->month > 2U)) {
        (*days) += 1;
    }

    /* Add the rest of the days from the current month */
    (*days) += MONTH_DAYS[timeDate->month];
    /* Add the rest of the days from the current day */
    (*days) += (uint32_t)((uint32_t)timeDate->day - 1U);
}

/*
 *
 * Function Name : RTC_DRV_IsTimeDateCorrectFormat
 * Description   : Check if the date time struct is configured properly.
 *
 */
bool RTC_DRV_IsTimeDateCorrectFormat(const rtc_timedate_t *const timeDate)
{
    OS_ASSERT(timeDate != NULL);

    /* Declare local variables */
    bool returnCode = true;
    const uint8_t *pDays;

    /* Set the days-in-month table for the corresponding year */
    pDays = RTC_DRV_IsYearLeap(timeDate->year) ? (LY) : (ULY);

    /* Check if the time and date are in the correct ranges */
    if ((timeDate->year < YEAR_RANGE_START) || (timeDate->year > YEAR_RANGE_END) || (timeDate->month < 1U) ||
        (timeDate->month > 12U) || (timeDate->day < 1U) || (timeDate->day > 31U) ||
        (timeDate->hour >= HOURS_IN_A_DAY) || (timeDate->minutes >= MINS_IN_A_HOUR) ||
        (timeDate->seconds >= SECONDS_IN_A_MIN)) {
        returnCode = false;
    }
    /* Check if the day is a valid day from the corresponding month */
    else if (timeDate->day > pDays[timeDate->month]) {
        returnCode = false;
    } else {
        returnCode = true;
    }

    /* Return the exit code */
    return returnCode;
}

/*
 *
 * Function Name : RTC_DRV_ConvertTimeDateToSeconds
 * Description   : Convert year,month,day to seconds.
 *
 */
void RTC_DRV_ConvertTimeDateToSeconds(const rtc_timedate_t *const timeDate, uint32_t *const seconds)
{
    OS_ASSERT(seconds != NULL);
    OS_ASSERT(timeDate != NULL);

    /* Declare local variables */
    uint16_t year;

    /* Convert years to seconds */
    (*seconds) = (uint32_t)(DAYS_IN_A_YEAR * (uint32_t)(SECONDS_IN_A_DAY));
    (*seconds) *= ((uint32_t)timeDate->year - YEAR_RANGE_START);

    /* Add the seconds from the leap years */
    for (year = YEAR_RANGE_START; year < timeDate->year; year++) {
        if (RTC_DRV_IsYearLeap(year)) {
            (*seconds) += SECONDS_IN_A_DAY;
        }
    }

    /* If the current year is leap and 29th of February has passed, add
     * another day to seconds passed.
     */
    if ((RTC_DRV_IsYearLeap(year)) && (timeDate->month > 2U)) {
        (*seconds) += SECONDS_IN_A_DAY;
    }

    /* Add the rest of the seconds from the current month */
    (*seconds) += MONTH_DAYS[timeDate->month] * SECONDS_IN_A_DAY;
    /* Add the rest of the seconds from the current day */
    (*seconds) += (uint32_t)(((uint32_t)timeDate->day - 1U) * (uint32_t)SECONDS_IN_A_DAY);
    /* Add the rest of the seconds from the current time */
    (*seconds) += (uint32_t)(((uint32_t)timeDate->hour * SECONDS_IN_A_HOUR) +
                             ((uint32_t)timeDate->minutes * SECONDS_IN_A_MIN) + (uint32_t)timeDate->seconds);
}

/*
 *
 * Function Name : RTC_DRV_IsYearLeap
 * Description   : Check whether it is leap year.
 *
 */
bool RTC_DRV_IsYearLeap(uint16_t year)
{
    bool isYearLeap = false;

    if ((year % 4U) > 0U) {
        isYearLeap = false;
    } else if ((year % 100U) > 0U) {
        isYearLeap = true;
    } else if ((year % 400U) > 0U) {
        isYearLeap = false;
    } else {
        isYearLeap = true;
    }

    /* Return the exit code */
    return isYearLeap;
}

/*
 *
 * Function Name : RTC_DRV_GetCurrentTimeDate.
 * Description   : Get current time and date from RTC instance.
 *
 */
void RTC_DRV_GetCurrentTimeDate(uint32_t instance, rtc_timedate_t *const currentTime)
{
    OS_ASSERT(instance < RTC_INSTANCE_COUNT);
    OS_ASSERT(currentTime != NULL);

    /* Declare local variables */
    uint32_t seconds;
    uint32_t minutes;
    uint32_t days;
    uint32_t hours;

    seconds = RTC_GetTimeSecondsRegister(g_pstRtcBase[instance]);
    minutes = RTC_GetTimeMinutesRegister(g_pstRtcBase[instance]);
    hours   = RTC_GetTimeHoursRegister(g_pstRtcBase[instance]);
    days    = RTC_GetTimeDaysRegister(g_pstRtcBase[instance]);

    currentTime->seconds = seconds;
    currentTime->minutes = minutes;
    currentTime->hour    = hours;

    /* Convert the current time from days to time date structure */
    RTC_DRV_ConvertDaysToTimeDate(currentTime, &days);
}

/*
 *
 * Function Name : RTC_DRV_SetTimeDate.
 * Description   : Set time and date for RTC instance.
 *
 */
status_t RTC_DRV_SetTimeDate(uint32_t instance, const rtc_timedate_t *const time)
{
    OS_ASSERT(instance < RTC_INSTANCE_COUNT);
    OS_ASSERT(time != NULL);

    /* Declare local variables */
    status_t statusCode = STATUS_SUCCESS;
    uint32_t days       = 0;

    /* Check if the time is in the correct format */
    if (RTC_DRV_IsTimeDateCorrectFormat(time) == false) {
        /* Set the exit code to error */
        statusCode = STATUS_ERROR;
    }

    RTC_DRV_ConvertTimeDateToDays(time, &days);

    RTC_SetTimeSecondsRegister(g_pstRtcBase[instance], time->seconds);
    RTC_SetTimeMinutesRegister(g_pstRtcBase[instance], time->minutes);
    RTC_SetTimeHoursRegister(g_pstRtcBase[instance], time->hour);
    RTC_SetTimeDaysRegister(g_pstRtcBase[instance], days);

    /* Return the exit code */
    return statusCode;
}

/*
 *
 * Function Name : RTC_DRV_ConfigureAlarm.
 * Description   : This method configures the alarm with the configuration from the alarmConfig parameter.
 *
 *
 */
status_t RTC_DRV_ConfigureAlarm(uint32_t instance, rtc_alarm_config_t *const alarmConfig)
{
    OS_ASSERT(instance < RTC_INSTANCE_COUNT);
    OS_ASSERT(alarmConfig != NULL);

    status_t statusCode      = STATUS_SUCCESS;
    rtc_timedate_t alarmTime = alarmConfig->alarmTime;
    rtc_timedate_t currentTime;
    uint32_t alarmSec   = 0;
    uint32_t currentSec = 0;
    uint32_t days       = 0;

    /* Check if the alarm time is in a correct format */
    if (RTC_DRV_IsTimeDateCorrectFormat(&(alarmConfig->alarmTime)) == true) {
        /* Get current time in seconds */
        RTC_DRV_GetCurrentTimeDate(instance, &currentTime);
        // TODO year

        RTC_DRV_ConvertTimeDateToSeconds(&currentTime, &currentSec);
        RTC_DRV_ConvertTimeDateToSeconds(&alarmTime, &alarmSec);

        /* Check if the alarm time is greater than current time */
        if (alarmSec > currentSec) {
            /* Disable the IRQ to avoid accidental interrupt requests */
            g_rtcRuntimeConfig_t.alarmConfig = alarmConfig;

            RTC_DRV_ConvertTimeDateToDays(&alarmTime, &days);

            /* Write alarm time into Time Alarm Register */
            RTC_SetAlarmSecondsRegister(g_pstRtcBase[instance], alarmTime.seconds);
            RTC_SetAlarmMinutesRegister(g_pstRtcBase[instance], alarmTime.minutes);
            RTC_SetAlarmHoursRegister(g_pstRtcBase[instance], alarmTime.hour);
            RTC_SetAlarmDaysRegister(g_pstRtcBase[instance], days);
            /* Enable/disable interrupt source based on the configuration */
            RTC_SetTimeInterruptEnable(g_pstRtcBase[instance], RTC_INT_ALM, alarmConfig->alarmIntEnable);
            /* After the configuration is done, re-enable the interrupt in
             * NVIC.
             */
        } else {
            statusCode = STATUS_ERROR;
        }
    } else {
        statusCode = STATUS_ERROR;
    }

    /* Return the exit code */
    return statusCode;
}

/*
 *
 * Function Name : RTC_DRV_GetAlarmConfig.
 * Description   : Get RTC alarm configuration.
 *
 */
void RTC_DRV_GetAlarmConfig(uint32_t instance, rtc_alarm_config_t *alarmConfig)
{
    OS_ASSERT(instance < RTC_INSTANCE_COUNT);
    OS_ASSERT(alarmConfig != NULL);

    *alarmConfig = *(g_rtcRuntimeConfig_t.alarmConfig);
}

#if 0
/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_GetTestValue
 * Description   : This method retrieves the test register value.
 * Return        : None
 * 
 *END**************************************************************************/
uint32_t RTC_DRV_GetTestValue(uint32_t instance, rtc_alarm_config_t * alarmConfig, uint32_t value)
{
    status_t statusCode = STATUS_SUCCESS;
    uint32_t TestValue = 0;
    if((value > 3) || (value <= 0))
    {
        //statusCode = STATUS_ERROR;
        return 0;
    }
    switch(value)
    {
        case 1:

    }
    uint32_t tmp = base->SECONDS;
    tmp = (tmp & RTC_SEC_SEC_MASK) >> RTC_SEC_SEC_SHIFT;
    return (uint32_t) (tmp);
}
#endif

/*
 *
 * Function Name : RTC_DRV_ConfigureStopwatch.
 * Description   : This method configures the stopwatch interrupt with the
 *                 configuration from the stopwatchConfig parameter.
 *
 */
status_t RTC_DRV_ConfigureStopwatch(uint32_t instance, rtc_stopwatch_config_t *const stopwatchConfig)
{
    OS_ASSERT(instance < RTC_INSTANCE_COUNT);
    OS_ASSERT(stopwatchConfig != NULL);

    status_t statusCode = STATUS_SUCCESS;

    /* Disable the IRQ to avoid accidental interrupt requests */
    // OS_DisableIrq(g_stRtcIrqId[instance]);
    g_rtcRuntimeConfig_t.stopwatchConfig = stopwatchConfig;

    RTC_SetTimeStopWatchRegister(g_pstRtcBase[instance], stopwatchConfig->stopwatchVal);
    /* Enable/disable interrupt source based on the configuration */
    RTC_SetTimeInterruptEnable(g_pstRtcBase[instance], RTC_INT_SW, stopwatchConfig->stopwatchIntEnable);

    /* Return the exit code */
    return statusCode;
}

/*
 *
 * Function Name : RTC_DRV_GetStopwatchConfig.
 * Description   : Get Stopwatch configuration.
 *
 */
void RTC_DRV_GetStopwatchConfig(uint32_t instance, rtc_stopwatch_config_t *stopwatchConfig)
{
    OS_ASSERT(instance < RTC_INSTANCE_COUNT);
    OS_ASSERT(stopwatchConfig != NULL);

    *stopwatchConfig = *(g_rtcRuntimeConfig_t.stopwatchConfig);
}

/*
 *
 * Function Name : RTC_DRV_ConfigureSamping.
 * Description   : This method configures the samping interrupt with the configuration 
 *                  from the sampingConfig parameter.
 *
 */
void RTC_DRV_ConfigureSamping(uint32_t instance, rtc_samping_config_t *const sampingConfig)
{
    OS_ASSERT(instance < RTC_INSTANCE_COUNT);
    OS_ASSERT(sampingConfig != NULL);

    /* Disable the IRQ to avoid accidental interrupt requests */
    g_rtcRuntimeConfig_t.sampingConfig = sampingConfig;

    /* Enable/disable interrupt source based on the configuration */
    RTC_SetTimeInterruptEnable(g_pstRtcBase[instance], RTC_INT_MIN, sampingConfig->minIntEnable);
    RTC_SetTimeInterruptEnable(g_pstRtcBase[instance], RTC_INT_DAY, sampingConfig->dayIntEnable);
    RTC_SetTimeInterruptEnable(g_pstRtcBase[instance], RTC_INT_1HZ, sampingConfig->hz1IntEnable);
    RTC_SetTimeInterruptEnable(g_pstRtcBase[instance], RTC_INT_HR, sampingConfig->hourIntEnable);
    RTC_SetTimeInterruptEnable(g_pstRtcBase[instance], RTC_INT_2HZ, sampingConfig->hz2IntEnable);
    RTC_SetTimeInterruptEnable(g_pstRtcBase[instance], RTC_INT_SAM0, sampingConfig->sam0IntEnable);
    RTC_SetTimeInterruptEnable(g_pstRtcBase[instance], RTC_INT_SAM1, sampingConfig->sam1IntEnable);
    RTC_SetTimeInterruptEnable(g_pstRtcBase[instance], RTC_INT_SAM2, sampingConfig->sam2IntEnable);
    RTC_SetTimeInterruptEnable(g_pstRtcBase[instance], RTC_INT_SAM3, sampingConfig->sam3IntEnable);
    RTC_SetTimeInterruptEnable(g_pstRtcBase[instance], RTC_INT_SAM4, sampingConfig->sam4IntEnable);
    RTC_SetTimeInterruptEnable(g_pstRtcBase[instance], RTC_INT_SAM5, sampingConfig->sam5IntEnable);
    RTC_SetTimeInterruptEnable(g_pstRtcBase[instance], RTC_INT_SAM6, sampingConfig->sam6IntEnable);
    RTC_SetTimeInterruptEnable(g_pstRtcBase[instance], RTC_INT_SAM7, sampingConfig->sam7IntEnable);
}

/*
 *
 * Function Name : RTC_DRV_GetSampingConfig.
 * Description   :  Get RTC samping configuration.
 *
 */
void RTC_DRV_GetSampingConfig(uint32_t instance, rtc_samping_config_t *sampingConfig)
{
    OS_ASSERT(instance < RTC_INSTANCE_COUNT);
    OS_ASSERT(sampingConfig != NULL);

    *sampingConfig = *(g_rtcRuntimeConfig_t.sampingConfig);
}

/*
 *
 * Function Name : RTC_DRV_IsIRQPending.
 * Description   : Check if interrupt has occurred include SAMPLE, ALARM, STOPWATCH IRQ.
 *
 */
bool RTC_DRV_IsIRQPending(uint32_t instance, rtc_int_t IRQType)
{
    OS_ASSERT(instance < RTC_INSTANCE_COUNT);

    /* Return the exit code */
    return RTC_GetTimeInterruptFlag(g_pstRtcBase[instance], IRQType);
}

/*
 *
 * Function Name : RTC_DRV_ClearInterrupt.
 * Description   : Clear RTC interrupt.
 *
 */
void RTC_DRV_ClearInterrupt(uint32_t instance)
{
    OS_ASSERT(instance < RTC_INSTANCE_COUNT);

    rtc_type_t *basePtr = g_pstRtcBase[instance];

    RTC_ClearTimeInterruptFlag(basePtr);
}
