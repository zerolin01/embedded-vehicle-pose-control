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
 * @file iopmp_register.h
 * @brief IOPMP driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __IOPMP_REGISTER_H__
#define __IOPMP_REGISTER_H__

#include "emps_platform.h"
#include <osal.h>
#include <stdint.h>

// clang-format off
/* ----------------------------------------------------------------------------
   -- IOPMP Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup IOPMP_Peripheral_Access_Layer IOPMP Peripheral Access Layer
 * @{
 */

/**
 * @brief IOPMP Register Layout Typedef
 */
typedef struct {
    __IO uint32_t RESERVED_0;           
    __IO uint32_t MPU_CTRL;             /**< IOPMP Control Register, offset: 0x4 */
    __IO uint32_t MPU_FAULT_STATUS;     /**< IOPMP Fault Status Register, offset: 0x8 */
    __IO uint32_t MPU_FAULT_ADDR;       /**< IOPMP Fault Address Register, offset: 0xC */
    __IO uint32_t MPU_FAULT_INT_STATUS; /**< IOPMP Fault Interrupt Status Register, offset: 0x10 */
    __IO uint32_t MPU_FAULT_INT_ENABLE; /**< IOPMP Fault Interrupt Enable Register, offset: 0x14 */
    __IO uint32_t RESERVED_1[2];
    struct {
        __IO uint32_t ADDR_LOCK;                        /**< IOPMP Address Lock Register, array offset: 0x20, array step: 0x32 */
        __IO uint32_t ATTR_LOCK[IOPMP_MASTER_COUNT];    /**< IOPMP Attribute Lock Register, array offset: 0x24, array step: 0x32 */
        __IO uint32_t RESERVED_2[3];
    } REGION_CFG[IOPMP_REGION_COUNT];
} iopmp_type_t, *iopmp_mem_map_ptr_t;

/* ----------------------------------------------------------------------------
   -- IOPMP Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup IOPMP_Register_Masks IOPMP Register Masks
 * @{
 */

/** MPU_CTRL Bit Fields */
#define IOPMP_MPU_CTRL_ATTR_REG_ENABLE_MASK        0x1U
#define IOPMP_MPU_CTRL_ATTR_REG_ENABLE_SHIFT       0U
#define IOPMP_MPU_CTRL_ATTR_REG_ENABLE_WIDTH       1U
#define IOPMP_MPU_CTRL_ATTR_REG_ENABLE(x)          (((uint32_t)(((uint32_t)(x))<<IOPMP_MPU_CTRL_ATTR_REG_ENABLE_SHIFT))&IOPMP_MPU_CTRL_ATTR_REG_ENABLE_MASK)
#define IOPMP_MPU_CTRL_LUT_LOCK_MASK               0x1U
#define IOPMP_MPU_CTRL_LUT_LOCK_SHIFT              0U
#define IOPMP_MPU_CTRL_LUT_LOCK_WIDTH              1U
#define IOPMP_MPU_CTRL_LUT_LOCK(x)                 (((uint32_t)(((uint32_t)(x))<<IOPMP_MPU_CTRL_LUT_LOCK_SHIFT))&IOPMP_MPU_CTRL_LUT_LOCK_MASK)
/** MPU_FAULT_STATUS Bit Fields */ 
#define IOPMP_MPU_FALUT_STATUS_MASTER_ID_MASK      0x3FU
#define IOPMP_MPU_FALUT_STATUS_MASTER_ID_SHIFT     0U
#define IOPMP_MPU_FALUT_STATUS_MASTER_ID_WIDTH     6U
#define IOPMP_MPU_FALUT_STATUS_MASTER_ID(x)        (((uint32_t)(((uint32_t)(x))<<IOPMP_MPU_FALUT_STATUS_MASTER_ID_SHIFT))&IOPMP_MPU_FALUT_STATUS_MASTER_ID_MASK)
#define IOPMP_MPU_FALUT_STATUS_FAULT_000_MASK      0x40U
#define IOPMP_MPU_FALUT_STATUS_FAULT_000_SHIFT     6U
#define IOPMP_MPU_FALUT_STATUS_FAULT_000_WIDTH     1U
#define IOPMP_MPU_FALUT_STATUS_FAULT_000(x)        (((uint32_t)(((uint32_t)(x))<<IOPMP_MPU_FALUT_STATUS_FAULT_000_SHIFT))&IOPMP_MPU_FALUT_STATUS_FAULT_000_MASK)
#define IOPMP_MPU_FALUT_STATUS_FAULT_001_MASK      0x80U
#define IOPMP_MPU_FALUT_STATUS_FAULT_001_SHIFT     7U
#define IOPMP_MPU_FALUT_STATUS_FAULT_001_WIDTH     1U
#define IOPMP_MPU_FALUT_STATUS_FAULT_001(x)        (((uint32_t)(((uint32_t)(x))<<IOPMP_MPU_FALUT_STATUS_FAULT_001_SHIFT))&IOPMP_MPU_FALUT_STATUS_FAULT_001_MASK)
#define IOPMP_MPU_FALUT_STATUS_FAULT_010_MASK      0x100U
#define IOPMP_MPU_FALUT_STATUS_FAULT_010_SHIFT     8U
#define IOPMP_MPU_FALUT_STATUS_FAULT_010_WIDTH     1U
#define IOPMP_MPU_FALUT_STATUS_FAULT_010(x)        (((uint32_t)(((uint32_t)(x))<<IOPMP_MPU_FALUT_STATUS_FAULT_010_SHIFT))&IOPMP_MPU_FALUT_STATUS_FAULT_010_MASK)
#define IOPMP_MPU_FALUT_STATUS_FAULT_101_MASK      0x200U
#define IOPMP_MPU_FALUT_STATUS_FAULT_101_SHIFT     9U
#define IOPMP_MPU_FALUT_STATUS_FAULT_101_WIDTH     1U
#define IOPMP_MPU_FALUT_STATUS_FAULT_101(x)        (((uint32_t)(((uint32_t)(x))<<IOPMP_MPU_FALUT_STATUS_FAULT_101_SHIFT))&IOPMP_MPU_FALUT_STATUS_FAULT_101_MASK)
#define IOPMP_MPU_FALUT_STATUS_FAULT_11x_MASK      0x400U
#define IOPMP_MPU_FALUT_STATUS_FAULT_11x_SHIFT     10U
#define IOPMP_MPU_FALUT_STATUS_FAULT_11x_WIDTH     1U
#define IOPMP_MPU_FALUT_STATUS_FAULT_11x(x)        (((uint32_t)(((uint32_t)(x))<<IOPMP_MPU_FALUT_STATUS_FAULT_11x_SHIFT))&IOPMP_MPU_FALUT_STATUS_FAULT_11x_MASK)
#define IOPMP_MPU_FALUT_STATUS_ADDR_HIT_MASK       0x800U
#define IOPMP_MPU_FALUT_STATUS_ADDR_HIT_SHIFT      11U
#define IOPMP_MPU_FALUT_STATUS_ADDR_HIT_WIDTH      1U
#define IOPMP_MPU_FALUT_STATUS_ADDR_HIT(x)         (((uint32_t)(((uint32_t)(x))<<IOPMP_MPU_FALUT_STATUS_ADDR_HIT_SHIFT))&IOPMP_MPU_FALUT_STATUS_ADDR_HIT_MASK)

#define IOPMP_MPU_FALUT_STATUS_MASK                0xFFFU
#define IOPMP_MPU_FALUT_STATUS_SHIFT               0U
#define IOPMP_MPU_FALUT_STATUS_WIDTH               12U
#define IOPMP_MPU_FALUT_STATUS(x)                  (((uint32_t)(((uint32_t)(x))<<IOPMP_MPU_FALUT_STATUS_SHIFT))&IOPMP_MPU_FALUT_STATUS_MASK)

#define IOPMP_MPU_FALUT_STATUS_ILLEGAL_MASK        0x80000000U
#define IOPMP_MPU_FALUT_STATUS_ILLEGAL_SHIFT       31U
#define IOPMP_MPU_FALUT_STATUS_ILLEGAL_WIDTH       1U
#define IOPMP_MPU_FALUT_STATUS_ILLEGAL(x)          (((uint32_t)(((uint32_t)(x))<<IOPMP_MPU_FALUT_STATUS_ILLEGAL_SHIFT))&IOPMP_MPU_FALUT_STATUS_ILLEGAL_MASK)
/** MPU_FAULT_ADDR Bit Fields */
#define IOPMP_MPU_FAULT_ADDR_MASK                  0xFFFFFFFFU
#define IOPMP_MPU_FAULT_ADDR_SHIFT                 0U
#define IOPMP_MPU_FAULT_ADDR_WIDTH                 32U
#define IOPMP_MPU_FAULT_ADDR(x)                    (((uint32_t)(((uint32_t)(x))<<IOPMP_MPU_FAULT_ADDR_SHIFT))&IOPMP_MPU_FAULT_ADDR_MASK)
/** MPU_FAULT_INT_STATUS Bit Fields */
#define IOPMP_MPU_FAULT_INT_STATUS_MASK            0x1U
#define IOPMP_MPU_FAULT_INT_STATUS_SHIFT           0U
#define IOPMP_MPU_FAULT_INT_STATUS_WIDTH           1U
#define IOPMP_MPU_FAULT_INT_STATUS(x)              (((uint32_t)(((uint32_t)(x))<<IOPMP_MPU_FAULT_INT_STATUS_SHIFT))&IOPMP_MPU_FAULT_INT_STATUS_MASK)
/** MPU_FAULT_INT_ENABLE Bit Fields */
#define IOPMP_MPU_FAULT_INT_ENABLE_MASK            0x1U
#define IOPMP_MPU_FAULT_INT_ENABLE_SHIFT           0U
#define IOPMP_MPU_FAULT_INT_ENABLE_WIDTH           1U
#define IOPMP_MPU_FAULT_INT_ENABLE(x)              (((uint32_t)(((uint32_t)(x))<<IOPMP_MPU_FAULT_INT_ENABLE_SHIFT))&IOPMP_MPU_FAULT_INT_ENABLE_MASK)
/** ADDR_LOCK Bit Fields */
#define IOPMP_ADDR_LOCK_REG_LOCK_MASK              0x1U
#define IOPMP_ADDR_LOCK_REG_LOCK_SHIFT             0U
#define IOPMP_ADDR_LOCK_REG_LOCK_WIDTH             1U
#define IOPMP_ADDR_LOCK_REG_LOCK(x)                (((uint32_t)(((uint32_t)(x))<<IOPMP_ADDR_LOCK_REG_LOCK_SHIFT))&IOPMP_ADDR_LOCK_REG_LOCK_MASK)
#define IOPMP_ADDR_LOCK_REG_BASE_ADDR_MASK         0xFFFFFFE0U
#define IOPMP_ADDR_LOCK_REG_BASE_ADDR_SHIFT        0U
#define IOPMP_ADDR_LOCK_REG_BASE_ADDR_WIDTH        27U
#define IOPMP_ADDR_LOCK_REG_BASE_ADDR(x)           (((uint32_t)(((uint32_t)(x))<<IOPMP_ADDR_LOCK_REG_BASE_ADDR_SHIFT))&IOPMP_ADDR_LOCK_REG_BASE_ADDR_MASK)
/** ATTR_LOCK Bit Fields */
#define IOPMP_ATTR_LOCK_REGION_LOCK_MASK           0x1U
#define IOPMP_ATTR_LOCK_REGION_LOCK_SHIFT          0U
#define IOPMP_ATTR_LOCK_REGION_LOCK_WIDTH          1U
#define IOPMP_ATTR_LOCK_REGION_LOCK(x)             (((uint32_t)(((uint32_t)(x))<<IOPMP_ATTR_LOCK_REGION_LOCK_SHIFT))&IOPMP_ATTR_LOCK_REGION_LOCK_MASK)
#define IOPMP_ATTR_LOCK_REGION_EN_MASK             0x2U
#define IOPMP_ATTR_LOCK_REGION_EN_SHIFT            1U
#define IOPMP_ATTR_LOCK_REGION_EN_WIDTH            1U
#define IOPMP_ATTR_LOCK_REGION_EN(x)               (((uint32_t)(((uint32_t)(x))<<IOPMP_ATTR_LOCK_REGION_EN_SHIFT))&IOPMP_ATTR_LOCK_REGION_EN_MASK)
#define IOPMP_ATTR_LOCK_REGION_SIZE_MASK           0x7CU
#define IOPMP_ATTR_LOCK_REGION_SIZE_SHIFT          2U
#define IOPMP_ATTR_LOCK_REGION_SIZE_WIDTH          5U
#define IOPMP_ATTR_LOCK_REGION_SIZE(x)             (((uint32_t)(((uint32_t)(x))<<IOPMP_ATTR_LOCK_REGION_SIZE_SHIFT))&IOPMP_ATTR_LOCK_REGION_SIZE_MASK)
#define IOPMP_ATTR_LOCK_REGION_AP_MASK             0x70000U
#define IOPMP_ATTR_LOCK_REGION_AP_SHIFT            16U
#define IOPMP_ATTR_LOCK_REGION_AP_WIDTH            3U
#define IOPMP_ATTR_LOCK_REGION_AP(x)               (((uint32_t)(((uint32_t)(x))<<IOPMP_ATTR_LOCK_REGION_AP_SHIFT))&IOPMP_ATTR_LOCK_REGION_AP_MASK)

/**
 * @}
 */ /* end of group IOPMP_Register_Masks */

/**
 * @}
 */ /* end of group IOPMP_Peripheral_Access_Layer */
// clang-format on

#endif // __IOPMP_REGISTER_H__
