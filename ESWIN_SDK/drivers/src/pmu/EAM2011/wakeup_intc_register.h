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
 * @file wakeup_intc_register.h
 * @brief Wakeup intc driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __WAKEUP_INTC_REGISTER_H__
#define __WAKEUP_INTC_REGISTER_H__

#include "emps_platform.h"
#include <osal.h>
#include <stdint.h>

// clang-format off
/* ----------------------------------------------------------------------------
   -- WAKEUP_INTC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup WAKEUP_INTC_Peripheral_Access_Layer WAKEUP_INTC Peripheral Access Layer
 * @{
 */

/**
 * @brief WAKEUP_INTC Register Layout Typedef
 */
typedef struct {
  __IO uint32_t INTC_STATUS0;          /**< Wakeup interrupt status, offset: 0x00 */
  __IO uint32_t INTC_STATUS1;          /**< Wakeup interrupt status, offset: 0x04 */
  __IO uint32_t RESERVED_1[2];     
  __IO uint32_t INTC_CLEAR0;           /**< Wakeup interrupt clear, offset: 0x10 */
  __IO uint32_t INTC_CLEAR1;           /**< Wakeup interrupt clear, offset: 0x14 */
  __IO uint32_t RESERVED_2[2];
  __IO uint32_t INTC_MASK0;            /**< Wakeup interrupt mask, offset: 0x20 */
  __IO uint32_t INTC_MASK1;            /**< Wakeup interrupt mask, offset: 0x24*/
  __IO uint32_t RESERVED_3[2];
  __IO uint32_t INTC_ENABLE0;          /**< Wakeup interrupt enable, offset: 0x30 */
  __IO uint32_t INTC_ENABLE1;          /**< Wakeup interrupt enable, offset: 0x34 */
} wakeup_type_t, *wakeup_mem_map_ptr_t;

/* ----------------------------------------------------------------------------
   -- WAKEUP_INTC Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup WAKEUP_INTC_Register_Masks WAKEUP_INTC Register Masks
 * @{
 */
/** INTC_ENABLE0 Bit Fields */
#define WAKEUP_INTC_ENABLE0_MASK                0xFFFFFFFFU
#define WAKEUP_INTC_ENABLE0_SHIFT               0U
#define WAKEUP_INTC_ENABLE0_WIDTH               32U
#define WAKEUP_INTC_ENABLE0(x)                  (((uint32_t)(((uint32_t)(x))<<WAKEUP_INTC_ENABLE0_SHIFT))&WAKEUP_INTC_ENABLE0_MASK)
/** INTC_ENABLE1 Bit Fields */
#define WAKEUP_INTC_ENABLE1_MASK                0xFFFFFFFU
#define WAKEUP_INTC_ENABLE1_SHIFT               0U
#define WAKEUP_INTC_ENABLE1_WIDTH               28U
#define WAKEUP_INTC_ENABLE1(x)                  (((uint32_t)(((uint32_t)(x))<<WAKEUP_INTC_ENABLE1_SHIFT))&WAKEUP_INTC_ENABLE1_MASK)
/** INTC_MASK0 Bit Fields */   
#define WAKEUP_INTC_MASK0_MASK                  0xFFFFFFFFU
#define WAKEUP_INTC_MASK0_SHIFT                 0U
#define WAKEUP_INTC_MASK0_WIDTH                 32U
#define WAKEUP_INTC_MASK0(x)                    (((uint32_t)(((uint32_t)(x))<<WAKEUP_INTC_MASK0_SHIFT))&WAKEUP_INTC_MASK0_MASK)
/** INTC_MASK1 Bit Fields */           
#define WAKEUP_INTC_MASK1_MASK                  0xFFFFFFFU
#define WAKEUP_INTC_MASK1_SHIFT                 0U
#define WAKEUP_INTC_MASK1_WIDTH                 28U
#define WAKEUP_INTC_MASK1(x)                    (((uint32_t)(((uint32_t)(x))<<WAKEUP_INTC_MASK1_SHIFT))&WAKEUP_INTC_MASK1_MASK)
/** INTC_CLEAR0 Bit Fields */   
#define WAKEUP_INTC_CLEAR0_MASK                 0xFFFFFFFFU
#define WAKEUP_INTC_CLEAR0_SHIFT                0U
#define WAKEUP_INTC_CLEAR0_WIDTH                32U
#define WAKEUP_INTC_CLEAR0(x)                   (((uint32_t)(((uint32_t)(x))<<WAKEUP_INTC_CLEAR0_SHIFT))&WAKEUP_INTC_CLEAR0_MASK)
/** INTC_CLEAR1 Bit Fields */
#define WAKEUP_INTC_CLEAR1_MASK                 0xFFFFFFFU
#define WAKEUP_INTC_CLEAR1_SHIFT                0U
#define WAKEUP_INTC_CLEAR1_WIDTH                28U
#define WAKEUP_INTC_CLEAR1(x)                   (((uint32_t)(((uint32_t)(x))<<WAKEUP_INTC_CLEAR1_SHIFT))&WAKEUP_INTC_CLEAR1_MASK)
/** INTC_STATUS0 Bit Fields */   
#define WAKEUP_INTC_STATUS0_MASK                0xFFFFFFFFU
#define WAKEUP_INTC_STATUS0_SHIFT               0U
#define WAKEUP_INTC_STATUS0_WIDTH               32U
#define WAKEUP_INTC_STATUS0(x)                  (((uint32_t)(((uint32_t)(x))<<WAKEUP_INTC_STATUS0_SHIFT))&WAKEUP_INTC_STATUS0_MASK)
/** INTC_STATUS1 Bit Fields */  
#define WAKEUP_INTC_STATUS1_MASK                0xFFFFFFFU
#define WAKEUP_INTC_STATUS1_SHIFT               0U
#define WAKEUP_INTC_STATUS1_WIDTH               28U
#define WAKEUP_INTC_STATUS1(x)                  (((uint32_t)(((uint32_t)(x))<<WAKEUP_INTC_STATUS1_SHIFT))&WAKEUP_INTC_STATUS1_MASK)

/**
 * @}
 */ /* end of group WAKEUP_INTC_Register_Masks */

/**
 * @}
 */ /* end of group WAKEUP_INTC_Peripheral_Access_Layer */
// clang-format on

#endif // __WAKEUP_INTC_REGISTER_H__
