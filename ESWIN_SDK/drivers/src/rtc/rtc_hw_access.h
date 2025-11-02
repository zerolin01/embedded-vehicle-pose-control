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
 * @file rtc_hw_access.h
 * @brief RTC driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __RTC_HW_ACCESS_H__
#define __RTC_HW_ACCESS_H__

#include "rtc_driver.h"
#include "rtc_register.h"
#include "status.h"
#include <stdbool.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Enable RTC instance counter
 *
 * @param  base RTC base pointer
 */
void RTC_Enable(rtc_type_t *const base);

/**
 * @brief Disable RTC instance counter
 *
 * @param  base RTC base pointer
 */
void RTC_Disable(rtc_type_t *const base);

/**
 * @brief  Trigger a software reset
 *
 * @param  base RTC base pointer
 */
static inline void RTC_SoftwareReset(rtc_type_t *const base)
{
    uint32_t count = 0;

    base->RTCCTL |= RTC_CTL_SWR(1);
    for (count = 0; count < 2000; count++) {
    }
    base->RTCCTL &= ~RTC_CTL_SWR(1);

    base->TEST3 |= 0x40;
    for (count = 0; count < 2000; count++) {
    }
    base->TEST3 &= 0xBF;
}

/**
 * @brief RTC clk select
 *
 * @param  base RTC base pointer
 * @param  xtl  clk value
 */
static inline void RTC_ClkSelect(rtc_type_t *const base, uint8_t xtl)
{

    uint32_t tmp = base->RTCCTL;

    tmp &= ~RTC_CTL_SWR_MASK;
    tmp |= RTC_CTL_XTL(xtl);
    base->RTCCTL = tmp;
}

/**
 * @brief  Get Time Seconds Register Value
 *
 * @param  base RTC base pointer
 * @return uint32_t Number of seconds passed
 */
static inline uint32_t RTC_GetTimeSecondsRegister(const rtc_type_t *const base)
{
    uint32_t tmp = base->SECONDS;
    tmp          = (tmp & RTC_SEC_SEC_MASK) >> RTC_SEC_SEC_SHIFT;
    return tmp;
}

/**
 * @brief  Set Time Seconds Register
 *
 * @param  base    RTC base pointer
 * @param  seconds number of seconds passed
 */
void RTC_SetTimeSecondsRegister(rtc_type_t *const base, uint32_t seconds);

/**
 * @brief  Get Time Minutes Register Value
 *
 * @param  base  RTC base pointer
 * @return uint32_t  Number of minutes passed
 */
static inline uint32_t RTC_GetTimeMinutesRegister(const rtc_type_t *const base)
{
    uint32_t tmp = base->HOURMIN;
    tmp          = (tmp & RTC_HM_MIN_MASK) >> RTC_HM_MIN_SHIFT;
    return tmp;
}

/**
 * @brief  Set Time Minutes Register
 *
 * @param  base RTC base pointer
 * @param  minutes number of minutes passed
 */
void RTC_SetTimeMinutesRegister(rtc_type_t *const base, uint32_t minutes);

/**
 * @brief Get Time Hours Register Value
 *
 * @param  base base RTC base pointer
 * @return uint32_t Number of hours passed
 */
static inline uint32_t RTC_GetTimeHoursRegister(const rtc_type_t *const base)
{
    uint32_t tmp = base->HOURMIN;
    tmp          = (tmp & RTC_HM_HOUR_MASK) >> RTC_HM_HOUR_SHIFT;
    return tmp;
}

/**
 * @brief  Set Time Hours Register
 *
 * @param  base  RTC base pointer
 * @param  hours  number of hours passed
 */
void RTC_SetTimeHoursRegister(rtc_type_t *const base, uint32_t hours);

/**
 * @brief  Get Time Days Register Value
 *
 * @param  base RTC base pointer
 * @return uint32_t Number of Days passed
 */
static inline uint32_t RTC_GetTimeDaysRegister(const rtc_type_t *const base)
{
    uint32_t tmp = base->DAYR;
    tmp          = (tmp & RTC_DAY_DAY_MASK) >> RTC_DAY_DAY_SHIFT;
    return tmp;
}

/**
 * @brief Set Time Days Register
 *
 * This function will help you set the time at a specified value.
 *
 * @param  base RTC base pointer
 * @param  days Setting the day value
 */
void RTC_SetTimeDaysRegister(rtc_type_t *const base, uint32_t days);

/**
 * @brief Get Time Alarm Seconds Register Value
 *
 * @param  base  RTC base pointer
 * @return uint32_t the alarm second value
 */
static inline uint32_t RTC_GetAlarmSecondsRegister(const rtc_type_t *const base)
{
    uint32_t tmp = base->ALRM_SEC;
    tmp          = (tmp & RTC_ALARM_SEC_MASK) >> RTC_ALARM_SEC_SHIFT;
    return tmp;
}

/**
 * @brief  This function will help you set the time at a specified value.
 *
 * @param  base RTC base pointer
 * @param  seconds Setting Alarm seconds
 */
void RTC_SetAlarmSecondsRegister(rtc_type_t *const base, uint32_t seconds);

/**
 * @brief  Get Time Alarm Minutes Register Value
 *
 * @param  base RTC base pointer
 * @return uint32_t Setting Alarm Minute Value
 */
static inline uint32_t RTC_GetAlarmMinutesRegister(const rtc_type_t *const base)
{
    uint32_t tmp = base->ALRM_HM;
    tmp          = (tmp & RTC_ALARM_MIN_MASK) >> RTC_ALARM_MIN_SHIFT;
    return tmp;
}

/**
 * @brief  Set Time Alarm Minutes Register
 *
 * @param  base     RTC base pointer
 * @param  minutes  Setting  minutes value
 */
void RTC_SetAlarmMinutesRegister(rtc_type_t *const base, uint32_t minutes);

/**
 * @brief  Get Time Alarm Hours Register Value
 *
 * @param  base RTC base pointer
 * @return uint32_t Number of hours passed
 */
static inline uint32_t RTC_GetAlarmHoursRegister(const rtc_type_t *const base)
{
    uint32_t tmp = base->ALRM_HM;
    tmp          = (tmp & RTC_ALARM_HOUR_MASK) >> RTC_ALARM_HOUR_SHIFT;
    return (uint32_t)(tmp);
}

/**
 * @brief  Set Time Alarm Hours Register
 *
 * This function will help you set the time at a specified value.
 *
 * @param  base  RTC base pointer
 * @param  hours Setting the alarm value
 */
void RTC_SetAlarmHoursRegister(rtc_type_t *const base, uint32_t hours);

/**
 * @brief  Get Time Alarm Days Register Value
 *
 * @param  base RTC base pointer
 * @return uint32_t Number of Days passed
 */
static inline uint32_t RTC_GetAlarmDaysRegister(const rtc_type_t *const base)
{
    uint32_t tmp = base->DAYALARM;
    tmp          = (tmp & RTC_ALARM_DAY_MASK) >> RTC_ALARM_DAY_SHIFT;
    return tmp;
}

/**
 * @brief Set Time Alarm Days Register
 *
 * @param  base  RTC base pointer
 * @param  days  Setting Alarm days
 */
void RTC_SetAlarmDaysRegister(rtc_type_t *const base, uint32_t days);

/**
 * @brief  Get the Time interrupt flag
 *
 * @param  base RTC base pointer
 * @param  offset interrupt type select
 * @return true if interrupt flag is 1
 * @return false if interrupt flag is 0
 */
static inline bool RTC_GetTimeInterruptFlag(const rtc_type_t *const base, uint16_t offset)
{
    uint32_t tmp = base->RTCISR;

    tmp = (tmp & RTC_INT_BIT_MASK(offset)) >> RTC_INT_BIT_SHIFT(offset);

    return ((tmp == 1U) ? true : false);
}

/**
 * @brief  Get RTC interrupt status.
 *
 * @param  base   RTC base pointer
 */
static inline uint32_t RTC_Get_InterruptStatus(rtc_type_t *const base)
{
    uint32_t regValue = base->RTCISR;
    
    return regValue;
}


/**
 * @brief Enable RTC interrupt
 *
 * @param  base RTC base pointer
 * @param  offset Interrupt Type
 */
static inline void RTC_ClearTimeInterruptFlag(rtc_type_t *const base)
{
    uint32_t regValue = 0;

    regValue = base->RTCISR;

    base->RTCISR = regValue;
}

/**
 * @brief Reset RTC interrupt
 *
 * @param  base RTC base pointer
 */
static inline void RTC_ResetRTCInterrupt(rtc_type_t *const base)
{
    uint32_t tmp = base->RTCISR;
    tmp |= 0xFF;
    base->RTCISR = tmp;
}

/**
 * @brief  Enable RTC_SetTimeInterruptEnable interrupt
 *
 * @param  base   RTC base pointer
 * @param  offset  interrupt type
 * @param  enable  enable or disable
 */
static inline void RTC_SetTimeInterruptEnable(rtc_type_t *const base, uint16_t offset, bool enable)
{
    uint32_t tmp = base->RTCIENR;
    tmp &= ~RTC_INT_BIT_MASK(offset);
    tmp |= RTC_INT_BIT(offset, enable);
    base->RTCIENR = tmp;
}





/**
 * @brief  This function will help you set the time at a specified value
 *
 * @param  base RTC base pointer
 * @param  val  Setting stopWatch value
 */
void RTC_SetTimeStopWatchRegister(rtc_type_t *const base, uint32_t val);




#if defined(__cplusplus)
}
#endif

#endif
