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
 * @file tsensor_register.h
 * @brief TSENSOR driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-09
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __TSENSOR_REGISTER_H__
#define __TSENSOR_REGISTER_H__

#include <osal.h>
#include <stdint.h>

// clang-format off
/* ----------------------------------------------------------------------------
   -- TSENSOR Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup TSENSOR_Peripheral_Access_Layer TSENSOR Peripheral Access Layer
 * @{
 */

/**
 * @brief TSENSOR enable type 
 */
typedef union {
    struct {
        volatile uint32_t en:1;                          /**< bit 0        access: RW       default: 0x0       */
        volatile uint32_t reserved0:31;                  /**< bit 1..31    access: RW       default: 0x0       */
    } b;
    volatile uint32_t w;
} tsensor_tsensor_en_type_t;

/**
 * @brief TSENSOR ptat power consumption type
 */
typedef union {
    struct {
        volatile uint32_t regPtatLv:6;                 /**< bit 0..5     access: RW       default: 0x0       */
        volatile uint32_t reserved0:26;                /**< bit 6..31    access: RW       default: 0x0       */
    } b;
    volatile uint32_t w;
} tsensor_reg_ptat_type_t;

/**
 * @brief TSENSOR Register Layout Typedef
 */
typedef struct {
  __IO uint32_t tsensorTsensorEn;                       /**< bit 0        access: RW       default: 0x0       */
  __IO uint32_t tsensorRegPtatLv;                       /**< bit 0..5     access: RW       default: 0x0       */
} tsensor_type_t;

/* ----------------------------------------------------------------------------
   -- TSENSOR Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup TSENSOR_Register_Masks TSENSOR Register Masks
 * @{
 */

/** TSENSOR Bit Fields */
#define TSENSOR_TSEN_EN_EN_MASK             0x1U
#define TSENSOR_TSEN_EN_EN_SHIFT            0U
#define TSENSOR_TSEN_EN_EN_WIDTH            1U
#define TSENSOR_TSEN_EN_EN(x)               (((uint32_t)(((uint32_t)(x))<<TSENSOR_TSEN_EN_EN_SHIFT))&TSENSOR_TSEN_EN_EN_MASK)
#define TSENSOR_REG_PTAT_LV_MASK            0x1FU
#define TSENSOR_REG_PTAT_LV_SHIFT           0U
#define TSENSOR_REG_PTAT_LV_WIDTH           5U
#define TSENSOR_REG_PTAT_LV(x)              (((uint32_t)(((uint32_t)(x))<<TSENSOR_REG_PTAT_LV_SHIFT))&TSENSOR_REG_PTAT_LV_MASK)
#define TSENSOR_REG_PTAT_MODE_MASK          0x20U
#define TSENSOR_REG_PTAT_MODE_SHIFT         5U
#define TSENSOR_REG_PTAT_MODE_WIDTH         1U
#define TSENSOR_REG_PTAT_MODE(x)            (((uint32_t)(((uint32_t)(x))<<TSENSOR_REG_PTAT_MODE_SHIFT))&TSENSOR_REG_PTAT_MODE_MASK)

/**
 * @}
 */ /* end of group TSENSOR_Register_Masks */

/**
 * @}
 */ /* end of group TSENSOR_Peripheral_Access_Layer */

// clang-format on

#endif // __TSENSOR_REGISTER_H__
