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
 * @file rtc_hw_access.c
 * @brief RTC driver hardware access source file
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
 *
 * Function Name : RTC_Enable.
 * Description   : Enable RTC counter
 *
 */
void RTC_Enable(rtc_type_t *const base)
{
    uint32_t tmp = base->RTCCTL;

    tmp |= RTC_CTL_EN(RTC_ENABLE);
    base->RTCCTL = tmp;
}

/*
 *
 * Function Name : RTC_Disable.
 * Description   : Disable RTC counter
 *
 */
void RTC_Disable(rtc_type_t *const base)
{
    uint32_t tmp = base->RTCCTL;

    tmp &= RTC_CTL_EN(RTC_DISABLE);
    base->RTCCTL = tmp;
}

/*
 *
 * Function Name : RTC_SetTimeSecondsRegister.
 * Description   : Set Time Seconds Register.
 *
 */
void RTC_SetTimeSecondsRegister(rtc_type_t *const base, uint32_t seconds)
{
    uint32_t tmp = base->SECONDS;

    tmp = RTC_SEC_SEC(seconds);

    base->SECONDS = tmp;
}

/*
 *
 * Function Name : RTC_SetTimeMinutesRegister.
 * Description   : Set Time Minutes Register.
 *
 */
void RTC_SetTimeMinutesRegister(rtc_type_t *const base, uint32_t minutes)
{
    uint32_t tmp = base->HOURMIN;

    tmp &= ~RTC_HM_MIN_MASK;
    tmp |= RTC_HM_MIN(minutes);
    base->HOURMIN = tmp;
}

/*
 *
 * Function Name : RTC_SetTimeHoursRegister.
 * Description   : Set Time Hours Register.
 *
 */
void RTC_SetTimeHoursRegister(rtc_type_t *const base, uint32_t hours)
{
    uint32_t tmp = base->HOURMIN;

    tmp &= ~RTC_HM_HOUR_MASK;
    tmp |= RTC_HM_HOUR(hours);
    base->HOURMIN = tmp;
}

/*
 *
 * Function Name : RTC_SetTimeDaysRegister.
 * Description   : This function will help you set the time at a specified value.
 *
 */
void RTC_SetTimeDaysRegister(rtc_type_t *const base, uint32_t days)
{
    uint32_t tmp = base->DAYR;

    tmp &= ~RTC_DAY_DAY_MASK;
    tmp |= RTC_DAY_DAY(days);
    base->DAYR = tmp;
}

/*
 *
 * Function Name : RTC_SetAlarmSecondsRegister.
 * Description   : This function will help you set the time at a specified value.
 *
 */
void RTC_SetAlarmSecondsRegister(rtc_type_t *const base, uint32_t seconds)
{
    uint32_t tmp = base->ALRM_SEC;

    tmp = RTC_ALARM_SEC(seconds);

    base->ALRM_SEC = tmp;
}

/*
 *
 * Function Name : RTC_SetAlarmMinutesRegister.
 * Description   : Set Time Alarm Minutes Register.
 *
 */
void RTC_SetAlarmMinutesRegister(rtc_type_t *const base, uint32_t minutes)
{
    uint32_t tmp = base->ALRM_HM;

    tmp &= ~RTC_ALARM_MIN_MASK;
    tmp |= RTC_ALARM_MIN(minutes);
    base->ALRM_HM = tmp;
}

/*
 *
 * Function Name : RTC_SetAlarmHoursRegister.
 * Description   : Set Time Alarm Hours Register.
 *
 */
void RTC_SetAlarmHoursRegister(rtc_type_t *const base, uint32_t hours)
{
    uint32_t tmp = base->ALRM_HM;

    tmp &= ~RTC_ALARM_HOUR_MASK;
    tmp |= RTC_ALARM_HOUR(hours);
    base->ALRM_HM = tmp;
}

/*
 *
 * Function Name : RTC_SetAlarmDaysRegister.
 * Description   : Set Time Alarm Days Register.
 *
 */
void RTC_SetAlarmDaysRegister(rtc_type_t *const base, uint32_t days)
{
    uint32_t tmp = base->DAYALARM;

    tmp &= ~RTC_ALARM_DAY_MASK;
    tmp |= RTC_ALARM_DAY(days);
    base->DAYALARM = tmp;
}

/*
 *
 * Function Name : RTC_SetTimeStopWatchRegister.
 * Description   : This function will help you set the time at a specified value.
 *
 */
void RTC_SetTimeStopWatchRegister(rtc_type_t *const base, uint32_t val)
{

    uint32_t tmp = base->STPWCH;

    tmp = RTC_SW_CNT(val);

    base->STPWCH = tmp;
}
