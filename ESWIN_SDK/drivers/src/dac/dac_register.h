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
 * @file dac_register.h
 * @brief DAC driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __DAC_REGISTER_H__
#define __DAC_REGISTER_H__

#include "emps_platform.h"
#include <osal.h>
#include <stdint.h>

// clang-format off
/* ----------------------------------------------------------------------------
   -- DAC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup DAC_Peripheral_Access_Layer DAC Peripheral Access Layer
 * @{
 */

/**
 * @brief DAC Register Layout Typedef
 */
typedef struct {
    __IO uint32_t C0; /**< DAC Control Register 0, offset: 0x0 */
    __IO uint32_t C1; /**< DAC Control Register 1, offset: 0x4 */
    __IO uint32_t C2; /**< DAC Control Register 2, offset: 0x8 */
    __IO uint32_t C3; /**< DAC Control Register 3, offset: 0xC */
} dac_type_t, *dac_mem_map_ptr_t;

/* ----------------------------------------------------------------------------
   -- DAC Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup DAC_Register_Masks DAC Register Masks
 * @{
 */

/** C0 Bit Fields */
#define DAC_C0_LPRN_MASK    0xFU
#define DAC_C0_LPRN_SHIFT   0U
#define DAC_C0_LPRN_WIDTH   4U
#define DAC_C0_LPRN(x)      (((uint32_t)(((uint32_t)(x)) << DAC_C0_LPRN_SHIFT)) & DAC_C0_LPRN_MASK)
#define DAC_C0_IEN_MASK     0x100U
#define DAC_C0_IEN_SHIFT    8U
#define DAC_C0_IEN_WIDTH    1U
#define DAC_C0_IEN(x)       (((uint32_t)(((uint32_t)(x)) << DAC_C0_IEN_SHIFT)) & DAC_C0_IEN_MASK)
#define DAC_C0_WL_MASK      0xC000000U
#define DAC_C0_WL_SHIFT     26U
#define DAC_C0_WL_WIDTH     2U
#define DAC_C0_WL(x)        (((uint32_t)(((uint32_t)(x)) << DAC_C0_WL_SHIFT)) & DAC_C0_WL_MASK)
#define DAC_C0_NEN_MASK     0x10000000U
#define DAC_C0_NEN_SHIFT    28U
#define DAC_C0_NEN_WIDTH    1U
#define DAC_C0_NEN(x)       (((uint32_t)(((uint32_t)(x)) << DAC_C0_NEN_SHIFT)) & DAC_C0_NEN_MASK)
#define DAC_C0_BUFFEN_MASK  0x20000000U
#define DAC_C0_BUFFEN_SHIFT 29U
#define DAC_C0_BUFFEN_WIDTH 1U
#define DAC_C0_BUFFEN(x)    (((uint32_t)(((uint32_t)(x)) << DAC_C0_BUFFEN_SHIFT)) & DAC_C0_BUFFEN_MASK)
#define DAC_C0_LP_MASK      0x40000000U
#define DAC_C0_LP_SHIFT     30U
#define DAC_C0_LP_WIDTH     1U
#define DAC_C0_LP(x)        (((uint32_t)(((uint32_t)(x)) << DAC_C0_LP_SHIFT)) & DAC_C0_LP_MASK)
#define DAC_C0_EN_MASK      0x80000000U
#define DAC_C0_EN_SHIFT     31U
#define DAC_C0_EN_WIDTH     1U
#define DAC_C0_EN(x)        (((uint32_t)(((uint32_t)(x)) << DAC_C0_EN_SHIFT)) & DAC_C0_EN_MASK)
/** C1 Bit Fields */
#define DAC_C1_LPREF_MASK   0x1U
#define DAC_C1_LPREF_SHIFT  0U
#define DAC_C1_LPREF_WIDTH  1U
#define DAC_C1_LPREF(x)     (((uint32_t)(((uint32_t)(x)) << DAC_C1_LPREF_SHIFT)) & DAC_C1_LPREF_MASK)
#define DAC_C1_EMPTY_MASK   0x40000000U
#define DAC_C1_EMPTY_SHIFT  30U
#define DAC_C1_EMPTY_WIDTH  1U
#define DAC_C1_EMPTY(x)     (((uint32_t)(((uint32_t)(x)) << DAC_C1_EMPTY_SHIFT)) & DAC_C1_EMPTY_MASK)
#define DAC_C1_FULL_MASK    0x80000000U
#define DAC_C1_FULL_SHIFT   31U
#define DAC_C1_FULL_WIDTH   1U
#define DAC_C1_FULL(x)      (((uint32_t)(((uint32_t)(x)) << DAC_C1_FULL_SHIFT)) & DAC_C1_FULL_MASK)
/** C2 Bit Fields */
#define DAC_C2_FW_MASK      0xFFFFFFFFU
#define DAC_C2_FW_SHIFT     0U
#define DAC_C2_FW_WIDTH     32U
#define DAC_C2_FW(x)        (((uint32_t)(((uint32_t)(x)) << DAC_C2_FW_SHIFT)) & DAC_C2_FW_MASK)
/** C3 Bit Fields */
#define DAC_C3_DATA_MASK    ((1 << FEATURE_DAC_RESOLUTION) - 1)
#define DAC_C3_DATA_SHIFT   0U
#define DAC_C3_DATA_WIDTH   FEATURE_DAC_RESOLUTION
#define DAC_C3_DATA(x)      (((uint32_t)(((uint32_t)(x)) << DAC_C3_DATA_SHIFT)) & DAC_C3_DATA_MASK)

/**
 * @}
 */ /* end of group DAC_Register_Masks */

/**
 * @}
 */ /* end of group DAC_Peripheral_Access_Layer */
// clang-format on

#endif // __DAC_REGISTER_H__
