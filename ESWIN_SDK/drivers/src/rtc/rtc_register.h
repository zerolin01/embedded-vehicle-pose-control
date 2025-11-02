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
 * @file rtc_register.h
 * @brief RTC driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-13
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __RTC_REGISTER_H__
#define __RTC_REGISTER_H__

#include <osal.h>

// clang-format off

/* ----------------------------------------------------------------------------
  -- Peripheral Access Layer for EAM2011
  ---------------------------------------------------------------------------- */

/**
 * @addtogroup Peripheral_Access_Layer_EAM2011 Peripheral Access Layer for EAM2011
 * @{
 */

/* ----------------------------------------------------------------------------
   -- RTC Peripheral Access Layer
   ---------------------------------------------------------------------------- */


/**
 * @addtogroup RTC_Peripheral_Access_Layer RTC Peripheral Access Layer
 * @{
 */

/**
 * @brief  RTC Register Layout Typedef. 
 */
typedef struct {
  __IO uint32_t HOURMIN;                               /**< RTC Hours and Minutes Counter Register, offset: 0x0 */
  __IO uint32_t SECONDS;                               /**< RTC Seconds Counter Register, offset: 0x4 */
  __IO uint32_t ALRM_HM;                               /**< RTC Hours and Minutes Alarm Register, offset: 0x8 */
  __IO uint32_t ALRM_SEC;                              /**< RTC Seconds Alarm Register, offset: 0xC */
  __IO uint32_t RTCCTL;                                /**< RTC Control Register, offset: 0x10 */
  __IO uint32_t RTCISR;                                /**< RTC Interrupt Status Register, offset: 0x14 */
  __IO uint32_t RTCIENR;                               /**< RTC Interrupt Enable Register, offset: 0x18 */
  __IO uint32_t STPWCH;                                /**< Stopwatch Minutes Register, offset: 0x1C */
  __IO uint32_t DAYR;                                  /**< RTC Days Counter Register, offset: 0x20 */
  __IO uint32_t DAYALARM;                              /**< RTC Days Alarm Register, offset: 0x24 */
  __IO uint32_t TEST1;                                 /**< RTC Test Register, offset: 0x28 */
  __IO uint32_t TEST2;                                 /**< RTC Test Register, offset: 0x2C */
  __IO uint32_t TEST3;                                 /**< RTC Test Register, offset: 0x30 */
} rtc_type_t;

/* ----------------------------------------------------------------------------
   -- RTC Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup RTC_Register_Masks RTC Register Masks
 * @{
 */

/** HOURMIN Bit Fields */
#define RTC_HM_MIN_SHIFT                        0U
#define RTC_HM_MIN_WIDTH                        6U
#define RTC_HM_MIN_MASK                         (0x3F << RTC_HM_MIN_SHIFT)
#define RTC_HM_MIN(x)                           (((uint32_t)(((uint32_t)(x)) << RTC_HM_MIN_SHIFT)) & RTC_HM_MIN_MASK)
#define RTC_HM_HOUR_SHIFT                       8U
#define RTC_HM_HOUR_WIDTH                       5U
#define RTC_HM_HOUR_MASK                        (0x1F << RTC_HM_HOUR_SHIFT)
#define RTC_HM_HOUR(x)                          (((uint32_t)(((uint32_t)(x)) << RTC_HM_HOUR_SHIFT)) & RTC_HM_HOUR_MASK)

/** SECOND Bit Fields */
#define RTC_SEC_SEC_SHIFT                        0U
#define RTC_SEC_SEC_WIDTH                        6U
#define RTC_SEC_SEC_MASK                         (0x3F << RTC_SEC_SEC_SHIFT)
#define RTC_SEC_SEC(x)                           (((uint32_t)(((uint32_t)(x)) << RTC_SEC_SEC_SHIFT)) & RTC_SEC_SEC_MASK)

/** ALARM bit Fields */
#define RTC_ALARM_MIN_SHIFT                       0u
#define RTC_ALARM_MIN_WIDTH                       6u
#define RTC_ALARM_MIN_MASK                        (0x3F << RTC_ALARM_MIN_SHIFT)
#define RTC_ALARM_MIN(x)                          (((uint32_t)(((uint32_t)(x)) << RTC_ALARM_MIN_SHIFT)) & RTC_ALARM_MIN_MASK)
#define RTC_ALARM_HOUR_SHIFT                      8u
#define RTC_ALARM_HOUR_WIDTH                      5u
#define RTC_ALARM_HOUR_MASK                       (0x1F << RTC_ALARM_HOUR_SHIFT)
#define RTC_ALARM_HOUR(x)                         (((uint32_t)(((uint32_t)(x)) << RTC_ALARM_HOUR_SHIFT)) & RTC_ALARM_HOUR_MASK)


/** ALARM SECOND Bit Fields */
#define RTC_ALARM_SEC_SHIFT                        0u
#define RTC_ALARM_SEC_WIDTH                        6u
#define RTC_ALARM_SEC_MASK                         (0x3F << RTC_ALARM_SEC_SHIFT)
#define RTC_ALARM_SEC(x)                           (((uint32_t)(((uint32_t)(x)) << RTC_ALARM_SEC_SHIFT)) & RTC_ALARM_SEC_MASK)

/** CTL Bit Fields */
#define RTC_CTL_EN_SHIFT                         7U
#define RTC_CTL_EN_WIDTH                         1U
#define RTC_CTL_EN_MASK                          (0x1 << RTC_CTL_EN_SHIFT)
#define RTC_CTL_EN(x)                            (((uint32_t)(((uint32_t)(x)) << RTC_CTL_EN_SHIFT)) & RTC_CTL_EN_MASK)
#define RTC_CTL_XTL_SHIFT                        5U
#define RTC_CTL_XTL_WIDTH                        2U
#define RTC_CTL_XTL_MASK                         (0x3 << RTC_CTL_XTL_SHIFT)
#define RTC_CTL_XTL(x)                           (((uint32_t)(((uint32_t)(x)) << RTC_CTL_XTL_SHIFT)) & RTC_CTL_XTL_MASK)
#define RTC_CTL_SWR_SHIFT                        0u
#define RTC_CTL_SWR_WIDTH                        1u
#define RTC_CTL_SWR_MASK                         (0x1 << RTC_CTL_SWR_SHIFT)
#define RTC_CTL_SWR(x)                           (((uint32_t)(((uint32_t)(x)) << RTC_CTL_SWR_SHIFT)) & RTC_CTL_SWR_MASK)

/** ISR Bit Fields */
#define RTC_ISR_SW_SHIFT                         0U
#define RTC_ISR_SW_WIDTH                         1U
#define RTC_ISR_SW_MASK                          (0x1 << RTC_ISR_SW_SHIFT)
#define RTC_ISR_SW(x)                            (((uint32_t)(((uint32_t)(x)) << RTC_ISR_SW_SHIFT)) & RTC_ISR_SW_MASK)
#define RTC_ISR_MIN_SHIFT                        1U
#define RTC_ISR_MIN_WIDTH                        1U
#define RTC_ISR_MIN_MASK                         (0x1 << RTC_ISR_MIN_SHIFT)
#define RTC_ISR_MIN(x)                           (((uint32_t)(((uint32_t)(x)) << RTC_ISR_MIN_SHIFT)) & RTC_ISR_MIN_MASK)
#define RTC_ISR_ALM_SHIFT                        2U
#define RTC_ISR_ALM_WIDTH                        1U
#define RTC_ISR_ALM_MASK                         (0x1 << RTC_ISR_ALM_SHIFT)
#define RTC_ISR_ALM(x)                           (((uint32_t)(((uint32_t)(x)) << RTC_ISR_ALM_SHIFT)) & RTC_ISR_ALM_MASK)
#define RTC_ISR_DAY_SHIFT                        3U
#define RTC_ISR_DAY_WIDTH                        1U
#define RTC_ISR_DAY_MASK                         (0x1 << RTC_ISR_DAY_SHIFT)
#define RTC_ISR_DAY(x)                           (((uint32_t)(((uint32_t)(x)) << RTC_ISR_DAY_SHIFT)) & RTC_ISR_DAY_MASK)
#define RTC_ISR_1HZ_SHIFT                        4U
#define RTC_ISR_1HZ_WIDTH                        1U
#define RTC_ISR_1HZ_MASK                         (0x1 << RTC_ISR_1HZ_SHIFT)
#define RTC_ISR_1HZ(x)                           (((uint32_t)(((uint32_t)(x)) << RTC_ISR_1HZ_SHIFT)) & RTC_ISR_1HZ_MASK)
#define RTC_ISR_HR_SHIFT                         5U
#define RTC_ISR_HR_WIDTH                         1U
#define RTC_ISR_HR_MASK                          (0x1 << RTC_ISR_HR_SHIFT)
#define RTC_ISR_HR(x)                            (((uint32_t)(((uint32_t)(x)) << RTC_ISR_HR_SHIFT)) & RTC_ISR_HR_MASK)
#define RTC_ISR_2HZ_SHIFT                        7U
#define RTC_ISR_2HZ_WIDTH                        1U
#define RTC_ISR_2HZ_MASK                         (0x1 << RTC_ISR_2HZ_SHIFT)
#define RTC_ISR_2HZ(x)                           (((uint32_t)(((uint32_t)(x)) << RTC_ISR_2HZ_SHIFT)) & RTC_ISR_2HZ_MASK)
#define RTC_ISR_SAM0_SHIFT                       8U
#define RTC_ISR_SAM0_WIDTH                       1U
#define RTC_ISR_SAM0_MASK                        (0x1 << RTC_ISR_SAM0_SHIFT)
#define RTC_ISR_SAM0(x)                          (((uint32_t)(((uint32_t)(x)) << RTC_ISR_SAM0_SHIFT)) & RTC_ISR_SAM0_MASK)
#define RTC_ISR_SAM1_SHIFT                       9U
#define RTC_ISR_SAM1_WIDTH                       1U
#define RTC_ISR_SAM1_MASK                        (0x1 << RTC_ISR_SAM1_SHIFT)
#define RTC_ISR_SAM1(x)                          (((uint32_t)(((uint32_t)(x)) << RTC_ISR_SAM1_SHIFT)) & RTC_ISR_SAM1_MASK)
#define RTC_ISR_SAM2_SHIFT                       10U
#define RTC_ISR_SAM2_WIDTH                       1U
#define RTC_ISR_SAM2_MASK                        (0x1 << RTC_ISR_SAM2_SHIFT)
#define RTC_ISR_SAM2(x)                          (((uint32_t)(((uint32_t)(x)) << RTC_ISR_SAM2_SHIFT)) & RTC_ISR_SAM2_MASK)
#define RTC_ISR_SAM3_SHIFT                       11U
#define RTC_ISR_SAM3_WIDTH                       1U
#define RTC_ISR_SAM3_MASK                        (0x1 << RTC_ISR_SAM3_SHIFT)
#define RTC_ISR_SAM3(x)                          (((uint32_t)(((uint32_t)(x)) << RTC_ISR_SAM3_SHIFT)) & RTC_ISR_SAM3_MASK)
#define RTC_ISR_SAM4_SHIFT                       12U
#define RTC_ISR_SAM4_WIDTH                       1U
#define RTC_ISR_SAM4_MASK                        (0x1 << RTC_ISR_SAM4_SHIFT)
#define RTC_ISR_SAM4(x)                          (((uint32_t)(((uint32_t)(x)) << RTC_ISR_SAM4_SHIFT)) & RTC_ISR_SAM4_MASK)
#define RTC_ISR_SAM5_SHIFT                       13U
#define RTC_ISR_SAM5_WIDTH                       1U
#define RTC_ISR_SAM5_MASK                        (0x1 << RTC_ISR_SAM5_SHIFT)
#define RTC_ISR_SAM5(x)                          (((uint32_t)(((uint32_t)(x)) << RTC_ISR_SAM5_SHIFT)) & RTC_ISR_SAM5_MASK)
#define RTC_ISR_SAM6_SHIFT                       14U
#define RTC_ISR_SAM6_WIDTH                       1U
#define RTC_ISR_SAM6_MASK                        (0x1 << RTC_ISR_SAM6_SHIFT)
#define RTC_ISR_SAM6(x)                          (((uint32_t)(((uint32_t)(x)) << RTC_ISR_SAM6_SHIFT)) & RTC_ISR_SAM6_MASK)
#define RTC_ISR_SAM7_SHIFT                       15U
#define RTC_ISR_SAM7_WIDTH                       1U
#define RTC_ISR_SAM7_MASK                        (0x1 << RTC_ISR_SAM7_SHIFT)
#define RTC_ISR_SAM7(x)                          (((uint32_t)(((uint32_t)(x)) << RTC_ISR_SAM7_SHIFT)) & RTC_ISR_SAM7_MASK)

/*RTCIENR Bit field*/
#define RTC_INT_BIT_SHIFT(oft)                   (oft)
#define RTC_INT_1BIT_WIDTH                        1U
#define RTC_INT_BIT_MASK(oft)                     (0x1 << RTC_INT_BIT_SHIFT(oft))
#define RTC_INT_BIT(oft, x)                      (((uint32_t)(((uint32_t)(x)) << RTC_INT_BIT_SHIFT(oft))) & RTC_INT_BIT_MASK(oft))

/** STOPWATCH Bit Fields */
#define RTC_SW_CNT_SHIFT                         0U
#define RTC_SW_CNT_WIDTH                         6U
#define RTC_SW_CNT_MASK                          (0x3F << RTC_SW_CNT_SHIFT)
#define RTC_SW_CNT(x)                            (((uint32_t)(((uint32_t)(x)) << RTC_SW_CNT_SHIFT)) & RTC_SW_CNT_MASK)

/** DAY Bit Fields */
#define RTC_DAY_DAY_SHIFT                        0U
#define RTC_DAY_DAY_WIDTH                        16U
#define RTC_DAY_DAY_MASK                         (0xFFFF << RTC_DAY_DAY_SHIFT)
#define RTC_DAY_DAY(x)                           (((uint32_t)(((uint32_t)(x)) << RTC_DAY_DAY_SHIFT)) & RTC_DAY_DAY_MASK)

/** DAYALARM Bit Fields*/
#define RTC_ALARM_DAY_SHIFT                       0U
#define RTC_ALARM_DAY_WIDTH                       16U
#define RTC_ALARM_DAY_MASK                        (0xFFFF << RTC_ALARM_DAY_SHIFT)
#define RTC_ALARM_DAY(x)                          (((uint32_t)(((uint32_t)(x)) << RTC_ALARM_DAY_SHIFT)) & RTC_ALARM_DAY_MASK)
#define RTC_ALARM_FDRST_SHIFT                     6U
#define RTC_ALARM_FDRST_WIDTH                     1U
#define RTC_ALARM_FDRST_MASK                      (0x1 << RTC_ALARM_FDRST_SHIFT)
#define RTC_ALARM_FDRST(x)                        (((uint32_t)(((uint32_t)(x)) << RTC_ALARM_FDRST_SHIFT)) & RTC_ALARM_FDRST_MASK)

/**
 * @}
 */ /* end of group RTC_Register_Masks */

/**
 * @}
 */ /* end of group RTC_Peripheral_Access_Layer */

/**
 * @}
 */ /* end of group Peripheral_Access_Layer_EAM2011 */

// clang-format on

#endif // __RTC_REGISTER_H__
