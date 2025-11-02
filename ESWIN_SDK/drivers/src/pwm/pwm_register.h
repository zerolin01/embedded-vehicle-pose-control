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
 * @file pwm_register.h
 * @brief PWM driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __PWM_REGISTER_H__
#define __PWM_REGISTER_H__

#include "osal.h"
#include "platform.h"
#include <stddef.h>

// clang-format off
/* ----------------------------------------------------------------------------
   -- PWM Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup PWM_Peripheral_Access_Layer PWM Peripheral Access Layer
 * @{
 */

/** PWM - Size of Registers Arrays */

/**
 * @brief PWM Register Layout Typedef
 */
typedef struct {
  __IO uint32_t PWMCR;                             /**< PWM Control Register, offset: 0x00 */
  __IO uint32_t PWMSR;                             /**< PWM Status Register, offset: 0x04 */
  __IO uint32_t PWMIR;                             /**< PWM Interrupt Register, offset: 0x08 */
  __IO uint32_t PWMSAR;                            /**< PWM Status Register, offset: 0x0c */
  __IO uint32_t PWMPR;                             /**< PWM Period Register, offset: 0x10 */
  __IO uint32_t PWMCNR;                            /**< PWM Counter Register, offset: 0x14 */
} pwm_type_t, *pwm_mem_map_ptr_t;

/* ----------------------------------------------------------------------------
   -- PWM Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup PWM_Register_Masks PWM Register Masks
 * @{
 */

/** PWMCR Bit Fields */
#define PWM_PWMCR_EN_SHIFT                                             0U
#define PWM_PWMCR_EN_MASK                                             (0x1UL << PWM_PWMCR_EN_SHIFT)
#define PWM_PWMCR_REPEAT_SHIFT                                         1U
#define PWM_PWMCR_REPEAT_MASK                                         (0x3UL << PWM_PWMCR_REPEAT_SHIFT)
#define PWM_PWMCR_SWR_SHIFT                                            3U
#define PWM_PWMCR_SWR_MASK                                            (0x1UL << PWM_PWMCR_SWR_SHIFT)
#define PWM_PWMCR_PRESCALER_SHIFT                                      4U
#define PWM_PWMCR_PRESCALER_MASK                                      (0xfffUL << PWM_PWMCR_PRESCALER_SHIFT)
#define PWM_PWMCR_CLKSRC_SHIFT                                         16U
#define PWM_PWMCR_CLKSRC_MASK                                         (0x3UL << PWM_PWMCR_CLKSRC_SHIFT)
#define PWM_PWMCR_POUTC_SHIFT                                          18U
#define PWM_PWMCR_POUTC_MASK                                          (0x3UL << PWM_PWMCR_POUTC_SHIFT)
#define PWM_PWMCR_HCTR_SHIFT                                           20U
#define PWM_PWMCR_HCTR_MASK                                           (0x1UL << PWM_PWMCR_HCTR_SHIFT)
#define PWM_PWMCR_BCTR_SHIFT                                           21U
#define PWM_PWMCR_BCTR_MASK                                           (0x1UL << PWM_PWMCR_BCTR_SHIFT)
#define PWM_PWMCR_DBGEN_SHIFT                                          22U
#define PWM_PWMCR_DBGEN_MASK                                          (0x1UL << PWM_PWMCR_DBGEN_SHIFT)
#define PWM_PWMCR_WAITEN_SHIFT                                         23U
#define PWM_PWMCR_WAITEN_MASK                                         (0x1UL << PWM_PWMCR_WAITEN_SHIFT)
#define PWM_PWMCR_DOZEN_SHIFT                                          24U
#define PWM_PWMCR_DOZEN_MASK                                          (0x1UL << PWM_PWMCR_DOZEN_SHIFT)
#define PWM_PWMCR_STOPEN_SHIFT                                         25U
#define PWM_PWMCR_STOPEN_MASK                                         (0x1UL << PWM_PWMCR_STOPEN_SHIFT)
#define PWM_PWMCR_FIFO_WATER_MARK_SHIFT                                26U
#define PWM_PWMCR_FIFO_WATER_MARK_MASK                                (0x3UL << PWM_PWMCR_FIFO_WATER_MARK_SHIFT)

/** PWMSR Bit Fields */
#define PWM_PWMSR_FIFOAV_SHIFT                                         0U
#define PWM_PWMSR_FIFOAV_MASK                                         (0x7UL << PWM_PWMSR_FIFOAV_SHIFT)
#define PWM_PWMSR_FE_SHIFT                                             3U
#define PWM_PWMSR_FE_MASK                                             (0x1UL << PWM_PWMSR_FE_SHIFT)
#define PWM_PWMSR_ROV_SHIFT                                            4U
#define PWM_PWMSR_ROV_MASK                                            (0x1UL << PWM_PWMSR_ROV_SHIFT)
#define PWM_PWMSR_CMP_SHIFT                                            5U
#define PWM_PWMSR_CMP_MASK                                            (0x1UL << PWM_PWMSR_CMP_SHIFT)
#define PWM_PWMSR_FWE_SHIFT                                            6U
#define PWM_PWMSR_FWE_MASK                                            (0x1UL << PWM_PWMSR_FWE_SHIFT)

/** PWMIR Bit Fields */
#define PWM_PWMIR_FIE_SHIFT                                            0U
#define PWM_PWMIR_FIE_MASK                                            (0x1UL << PWM_PWMIR_FIE_SHIFT)
#define PWM_PWMIR_RIE_SHIFT                                            1U
#define PWM_PWMIR_RIE_MASK                                            (0x1UL << PWM_PWMIR_RIE_SHIFT)
#define PWM_PWMIR_CIE_SHIFT                                            2U
#define PWM_PWMIR_CIE_MASK                                            (0x1UL << PWM_PWMIR_CIE_SHIFT)

/** PWMSAR Bit Fields */
#define PWM_PWMSAR_SAMPLE_SHIFT                                        0U
#define PWM_PWMSAR_SAMPLE_MASK                                        (0xffffUL << PWM_PWMSAR_SAMPLE_SHIFT)

/** PWMPR Bit Fields */
#define PWM_PWMPR_PERIOD_SHIFT                                         0U
#define PWM_PWMPR_PERIOD_MASK                                         (0xffffUL << PWM_PWMPR_PERIOD_SHIFT)

/** PWMCNR Bit Fields */
#define PWM_PWMCNR_COUNT_SHIFT                                         0U
#define PWM_PWMCNR_COUNT_MASK                                         (0xffffUL << PWM_PWMCNR_COUNT_SHIFT)

/**
 * @}
 */ /* end of group PWM_Register_Masks */

/**
 * @}
 */ /* end of group PWM_Peripheral_Access_Layer */

// clang-format on

#endif // __PWM_REGISTER_H__
