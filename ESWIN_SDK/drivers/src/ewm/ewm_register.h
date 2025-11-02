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
 * @file ewm_register.h
 * @brief EWM driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-12
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __EWM_REGISTER_H__
#define __EWM_REGISTER_H__

#include <osal.h>
#include <stdint.h>

// clang-format off

/* ----------------------------------------------------------------------------
  -- Peripheral Access Layer for EAM2011
  ---------------------------------------------------------------------------- */

/**
 * @addtogroup Peripheral_Access_Layer_EAM2011 Peripheral Access Layer for EAM2011
 * @{
 */

/* ----------------------------------------------------------------------------
   -- EWM Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup EWM_Peripheral_Access_Layer EWM Peripheral Access Layer
 * @{
 */


/**
 * @brief EWM Register Layout Typedef
 */
typedef struct {
  __IO uint32_t CTRL;                               /**< Control Register, offset: 0x0 */
  __O  uint32_t SERV;                               /**< Service Register, offset: 0x4 */
  __IO uint32_t CMPL;                               /**< Compare Low Register, offset: 0x8 */
  __IO uint32_t CMPH;                               /**< Compare High Register, offset: 0xC */
  __IO uint32_t CLKPRESCALER;                       /**< Clock Prescaler Register, offset: 0x10 */
} ewm_type_t, *ewm_mem_map_ptr_t;

/* ----------------------------------------------------------------------------
   -- EWM Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup EWM_Register_Masks EWM Register Masks
 * @{
 */

/** CTRL Bit Fields */
#define EWM_CTRL_EWMEN_MASK                      0x1U
#define EWM_CTRL_EWMEN_SHIFT                     0U
#define EWM_CTRL_EWMEN_WIDTH                     1U
#define EWM_CTRL_EWMEN(x)                        (((uint8_t)(((uint8_t)(x))<<EWM_CTRL_EWMEN_SHIFT))&EWM_CTRL_EWMEN_MASK)
#define EWM_CTRL_ASSIN_MASK                      0x2U
#define EWM_CTRL_ASSIN_SHIFT                     1U
#define EWM_CTRL_ASSIN_WIDTH                     1U
#define EWM_CTRL_ASSIN(x)                        (((uint8_t)(((uint8_t)(x))<<EWM_CTRL_ASSIN_SHIFT))&EWM_CTRL_ASSIN_MASK)
#define EWM_CTRL_INEN_MASK                       0x4U
#define EWM_CTRL_INEN_SHIFT                      2U
#define EWM_CTRL_INEN_WIDTH                      1U
#define EWM_CTRL_INEN(x)                         (((uint8_t)(((uint8_t)(x))<<EWM_CTRL_INEN_SHIFT))&EWM_CTRL_INEN_MASK)
#define EWM_CTRL_INTEN_MASK                      0x8U
#define EWM_CTRL_INTEN_SHIFT                     3U
#define EWM_CTRL_INTEN_WIDTH                     1U
#define EWM_CTRL_INTEN(x)                        (((uint8_t)(((uint8_t)(x))<<EWM_CTRL_INTEN_SHIFT))&EWM_CTRL_INTEN_MASK)
/** SERV Bit Fields */
#define EWM_SERV_SERVICE_MASK                    0xFFU
#define EWM_SERV_SERVICE_SHIFT                   0U
#define EWM_SERV_SERVICE_WIDTH                   8U
#define EWM_SERV_SERVICE(x)                      (((uint8_t)(((uint8_t)(x))<<EWM_SERV_SERVICE_SHIFT))&EWM_SERV_SERVICE_MASK)
/** CMPL Bit Fields */
#define EWM_CMPL_COMPAREL_MASK                   0xFFU
#define EWM_CMPL_COMPAREL_SHIFT                  0U
#define EWM_CMPL_COMPAREL_WIDTH                  8U
#define EWM_CMPL_COMPAREL(x)                     (((uint8_t)(((uint8_t)(x))<<EWM_CMPL_COMPAREL_SHIFT))&EWM_CMPL_COMPAREL_MASK)
/** CMPH Bit Fields */
#define EWM_CMPH_COMPAREH_MASK                   0xFFU
#define EWM_CMPH_COMPAREH_SHIFT                  0U
#define EWM_CMPH_COMPAREH_WIDTH                  8U
#define EWM_CMPH_COMPAREH(x)                     (((uint8_t)(((uint8_t)(x))<<EWM_CMPH_COMPAREH_SHIFT))&EWM_CMPH_COMPAREH_MASK)
/** CLKPRESCALER Bit Fields */
#define EWM_CLKPRESCALER_CLK_DIV_MASK            0xFFU
#define EWM_CLKPRESCALER_CLK_DIV_SHIFT           0U
#define EWM_CLKPRESCALER_CLK_DIV_WIDTH           8U
#define EWM_CLKPRESCALER_CLK_DIV(x)              (((uint8_t)(((uint8_t)(x))<<EWM_CLKPRESCALER_CLK_DIV_SHIFT))&EWM_CLKPRESCALER_CLK_DIV_MASK)

/**
 * @}
 */ /* end of group EWM_Register_Masks */

/**
 * @}
 */ /* end of group EWM_Peripheral_Access_Layer */

/**
 * @}
 */ /* end of group Peripheral_Access_Layer_EAM2011 */

// clang-format on

#endif // __EWM_REGISTER_H__
