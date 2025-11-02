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
 * @file cmu_register.h
 * @brief 
 * @author abu (abu@eswincomputing.com)
 * @date 2023-05-23
 * 
 * Modification History :
 * Date:               Version:                    Author:     
 * Changes: 
 * 
 */

#ifndef __CMU_REGISTER_H__
#define __CMU_REGISTER_H__

#include "osal.h"
#include "platform.h"
#include <stddef.h>

// clang-format off
/* ----------------------------------------------------------------------------
   -- CMU Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup CMU_Peripheral_Access_Layer CMU Peripheral Access Layer
 * @{
 */

/** CMU - Size of Registers Arrays */

/**
 * @brief CMU Register Layout Typedef
 */
typedef struct {
    __IO uint32_t CMU_TH_HVAL;                   /**< CMU Threshold high val Register, offset: 0x00 */
    __IO uint32_t CMU_TH_LVAL;                   /**< CMU Threshold low val Register, offset: 0x04 */
    __IO uint32_t CMU_MON_CNT_VAL;               /**< CMU monitor cnt val Register, offset: 0x08 */
    __IO uint32_t CMU_MON_MEAS_DONE;             /**< CMU measure done Register, offset: 0x0c */
} cmu_type_t;

typedef struct {
    __IO uint32_t CMU_IT_MASK;                   /**< CMU interrupt mask Register, offset: 0x00 */
    __IO uint32_t CMU_IT_CLEAR;                  /**< CMU interrupt clear Register, offset: 0x04 */
    __IO uint32_t CMU_IT_STATUS;                 /**< CMU status Register, offset: 0x08 */
    __IO uint32_t CMU_EN;                        /**< CMU enable Register, offset: 0x0c */
} cmu_common_type_t;

/* ----------------------------------------------------------------------------
   -- CMU Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup CMU_Register_Masks CMU Register Masks
 * @{
 */

/** CMU_TH_HVAL Bit Fields */
#define CMU_TH_HVAL_SHIFT                                       0U
#define CMU_TH_HVAL_MASK                                        (0xffffffffUL << CMU_TH_HVAL_SHIFT)

/** CMU_TH_LVAL Bit Fields */
#define CMU_TH_LVAL_SHIFT                                       0U
#define CMU_TH_LVAL_MASK                                        (0xffffffffUL << CMU_TH_LVAL_SHIFT)

/** CMU_MON_MEAS_DONE Bit Fields */
#define CMU_MON_MEAS_DONE_SHIFT                                 0U
#define CMU_MON_MEAS_DONE_MASK                                  (0x1UL << CMU_MON_MEAS_DONE_SHIFT)

/** CMU_IT_MASK Bit Fields */
#define CMU_IT_ABNORMAL_MASK_SHIFT(x)                           (0U + x)
#define CMU_IT_ABNORMAL_MASK_MASK(x)                            (0x1UL << CMU_IT_ABNORMAL_MASK_SHIFT(x))
#define CMU_IT_NORMAL_MASK_SHIFT(x)                             (8U + x)
#define CMU_IT_NORMAL_MASK_MASK(x)                              (0x1UL << CMU_IT_NORMAL_MASK_SHIFT(x))

/** CMU_IT_CLEAR Bit Fields */
#define CMU_IT_ABNORMAL_CLEAR_SHIFT(x)                           (0U + x)
#define CMU_IT_ABNORMAL_CLEAR_MASK(x)                            (0x1UL << CMU_IT_ABNORMAL_CLEAR_SHIFT(x))
#define CMU_IT_NORMAL_CLEAR_SHIFT(x)                             (8U + x)
#define CMU_IT_NORMAL_CLEAR_MASK(x)                              (0x1UL << CMU_IT_NORMAL_CLEAR_SHIFT(x))

/** CMU_IT_STATUS Bit Fields */
#define CMU_IT_ABNORMAL_STATUS_SHIFT(x)                           (0U + x)
#define CMU_IT_ABNORMAL_STATUS_MASK(x)                            (0x1UL << CMU_IT_ABNORMAL_STATUS_SHIFT(x))
#define CMU_IT_NORMAL_STATUS_SHIFT(x)                             (8U + x)
#define CMU_IT_NORMAL_STATUS_MASK(x)                              (0x1UL << CMU_IT_NORMAL_STATUS_SHIFT(x))

/** CMU_EN Bit Fields */
#define CMU_EN_SHIFT(x)                                           (0U + x)
#define CMU_EN_MASK(x)                                            (0x1UL << CMU_EN_SHIFT(x))
#define CMU_EN(v, x)                                              (((uint32_t)(((uint32_t)(v))<<CMU_EN_SHIFT(x)))&CMU_EN_MASK(x))

/**
 * @}
 */ /* end of group CMU_Register_Masks */

/**
 * @}
 */ /* end of group CMU_Peripheral_Access_Layer */

// clang-format on

#endif /* __CMU_REGISTER_H__ */