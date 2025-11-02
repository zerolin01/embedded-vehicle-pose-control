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
 * @file cmp_register.h
 * @brief CMP driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __CMP_REGISTER_H__
#define __CMP_REGISTER_H__

#include "emps_platform.h"
#include <osal.h>
#include <stdint.h>

// clang-format off
/* ----------------------------------------------------------------------------
   -- CMP Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup CMP_Peripheral_Access_Layer CMP Peripheral Access Layer
 * @{
 */

/**
 * @brief CMP Register Layout Typedef
 */
typedef struct {
    __IO uint32_t C0; /**< CMP Control Register 0, offset: 0x0 */
    __IO uint32_t C1; /**< CMP Control Register 1, offset: 0x4 */
    __IO uint32_t C2; /**< CMP Control Register 2, offset: 0x8 */
    __IO uint32_t C3; /**< CMP Control Register 3, offset: 0xc */
} cmp_type_t, *cmp_mem_map_ptr_t;

/* ----------------------------------------------------------------------------
   -- CMP Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup CMP_Register_Masks CMP Register Masks
 * @{
 */

/** C0 Bit Fields */
#define CMP_C0_HYSTCTR_MASK     0x2U
#define CMP_C0_HYSTCTR_SHIFT    1U
#define CMP_C0_HYSTCTR_WIDTH    1U
#define CMP_C0_HYSTCTR(x)       (((uint32_t)(((uint32_t)(x)) << CMP_C0_HYSTCTR_SHIFT)) & CMP_C0_HYSTCTR_MASK)
#define CMP_C0_FILTER_CNT_MASK  0x1CU
#define CMP_C0_FILTER_CNT_SHIFT 2U
#define CMP_C0_FILTER_CNT_WIDTH 3U
#define CMP_C0_FILTER_CNT(x)    (((uint32_t)(((uint32_t)(x)) << CMP_C0_FILTER_CNT_SHIFT)) & CMP_C0_FILTER_CNT_MASK)
#define CMP_C0_COPE_MASK        0x200U
#define CMP_C0_COPE_SHIFT       9U
#define CMP_C0_COPE_WIDTH       1U
#define CMP_C0_COPE(x)          (((uint32_t)(((uint32_t)(x)) << CMP_C0_COPE_SHIFT)) & CMP_C0_COPE_MASK)
#define CMP_C0_COS_MASK         0x400U
#define CMP_C0_COS_SHIFT        10U
#define CMP_C0_COS_WIDTH        1U
#define CMP_C0_COS(x)           (((uint32_t)(((uint32_t)(x)) << CMP_C0_COS_SHIFT)) & CMP_C0_COS_MASK)
#define CMP_C0_INVT_MASK        0x800U
#define CMP_C0_INVT_SHIFT       11U
#define CMP_C0_INVT_WIDTH       1U
#define CMP_C0_INVT(x)          (((uint32_t)(((uint32_t)(x)) << CMP_C0_INVT_SHIFT)) & CMP_C0_INVT_MASK)
#define CMP_C0_PMODE_MASK       0x3000U
#define CMP_C0_PMODE_SHIFT      12U
#define CMP_C0_PMODE_WIDTH      2U
#define CMP_C0_PMODE(x)         (((uint32_t)(((uint32_t)(x)) << CMP_C0_PMODE_SHIFT)) & CMP_C0_PMODE_MASK)
#define CMP_C0_WE_MASK          0x4000U
#define CMP_C0_WE_SHIFT         14U
#define CMP_C0_WE_WIDTH         1U
#define CMP_C0_WE(x)            (((uint32_t)(((uint32_t)(x)) << CMP_C0_WE_SHIFT)) & CMP_C0_WE_MASK)
#define CMP_C0_SE_MASK          0x8000U
#define CMP_C0_SE_SHIFT         15U
#define CMP_C0_SE_WIDTH         1U
#define CMP_C0_SE(x)            (((uint32_t)(((uint32_t)(x)) << CMP_C0_SE_SHIFT)) & CMP_C0_SE_MASK)
#define CMP_C0_FSP_MASK         0xFF0000U
#define CMP_C0_FSP_SHIFT        16U
#define CMP_C0_FSP_WIDTH        8U
#define CMP_C0_FSP(x)           (((uint32_t)(((uint32_t)(x)) << CMP_C0_FSP_SHIFT)) & CMP_C0_FSP_MASK)
#define CMP_C0_COUT_MASK        0x1000000U
#define CMP_C0_COUT_SHIFT       24U
#define CMP_C0_COUT_WIDTH       1U
#define CMP_C0_COUT(x)          (((uint32_t)(((uint32_t)(x)) << CMP_C0_COUT_SHIFT)) & CMP_C0_COUT_MASK)
#define CMP_C0_IFE_MASK         0x2000000U
#define CMP_C0_IFE_SHIFT        25U
#define CMP_C0_IFE_WIDTH        1U
#define CMP_C0_IFE(x)           (((uint32_t)(((uint32_t)(x)) << CMP_C0_IFE_SHIFT)) & CMP_C0_IFE_MASK)
#define CMP_C0_IRE_MASK         0x4000000U
#define CMP_C0_IRE_SHIFT        26U
#define CMP_C0_IRE_WIDTH        1U
#define CMP_C0_IRE(x)           (((uint32_t)(((uint32_t)(x)) << CMP_C0_IRE_SHIFT)) & CMP_C0_IRE_MASK)
#define CMP_C0_CFF_MASK         0x8000000U
#define CMP_C0_CFF_SHIFT        27U
#define CMP_C0_CFF_WIDTH        1U
#define CMP_C0_CFF(x)           (((uint32_t)(((uint32_t)(x)) << CMP_C0_CFF_SHIFT)) & CMP_C0_CFF_MASK)
#define CMP_C0_CRF_MASK         0x10000000U
#define CMP_C0_CRF_SHIFT        28U
#define CMP_C0_CRF_WIDTH        1U
#define CMP_C0_CRF(x)           (((uint32_t)(((uint32_t)(x)) << CMP_C0_CRF_SHIFT)) & CMP_C0_CRF_MASK)
#define CMP_C0_EN_MASK          0x40000000U
#define CMP_C0_EN_SHIFT         30U
#define CMP_C0_EN_WIDTH         1U
#define CMP_C0_EN(x)            (((uint32_t)(((uint32_t)(x)) << CMP_C0_EN_SHIFT)) & CMP_C0_EN_MASK)
#define CMP_C0_DMAEN_MASK       0x80000000U
#define CMP_C0_DMAEN_SHIFT      31U
#define CMP_C0_DMAEN_WIDTH      1U
#define CMP_C0_DMAEN(x)         (((uint32_t)(((uint32_t)(x)) << CMP_C0_DMAEN_SHIFT)) & CMP_C0_DMAEN_MASK)
/** C1 Bit Fields */
#define CMP_C1_NS_MASK          0x7U
#define CMP_C1_NS_SHIFT         0U
#define CMP_C1_NS_WIDTH         3U
#define CMP_C1_NS(x)            (((uint32_t)(((uint32_t)(x)) << CMP_C1_NS_SHIFT)) & CMP_C1_NS_MASK)
#define CMP_C1_PS_MASK          0x38U
#define CMP_C1_PS_SHIFT         3U
#define CMP_C1_PS_WIDTH         3U
#define CMP_C1_PS(x)            (((uint32_t)(((uint32_t)(x)) << CMP_C1_PS_SHIFT)) & CMP_C1_PS_MASK)
#define CMP_C1_CHN_MASK         0xFF00U
#define CMP_C1_CHN_SHIFT        8U
#define CMP_C1_CHN_WIDTH        8U
#define CMP_C1_CHN(x)           (((uint32_t)(((uint32_t)(x)) << CMP_C1_CHN_SHIFT)) & CMP_C1_CHN_MASK)
#define CMP_C1_CNS_MASK         0x30000000U
#define CMP_C1_CNS_SHIFT        28U
#define CMP_C1_CNS_WIDTH        2U
#define CMP_C1_CNS(x)           (((uint32_t)(((uint32_t)(x)) << CMP_C1_CNS_SHIFT)) & CMP_C1_CNS_MASK)
#define CMP_C1_CPS_MASK         0xc0000000U
#define CMP_C1_CPS_SHIFT        30U
#define CMP_C1_CPS_WIDTH        2U
#define CMP_C1_CPS(x)           (((uint32_t)(((uint32_t)(x)) << CMP_C1_CPS_SHIFT)) & CMP_C1_CPS_MASK)
/** C2 Bit Fields */
#define CMP_C2_RESn_MASK        0xFFU
#define CMP_C2_RESn_SHIFT       0U
#define CMP_C2_RESn_WIDTH       8U
#define CMP_C2_RESn(x)          (((uint32_t)(((uint32_t)(x)) << CMP_C2_RESn_SHIFT)) & CMP_C2_RESn_MASK)
#define CMP_C2_NSCN_MASK        0x300U
#define CMP_C2_NSCN_SHIFT       8U
#define CMP_C2_NSCN_WIDTH       2U
#define CMP_C2_NSCN(x)          (((uint32_t)(((uint32_t)(x)) << CMP_C2_NSCN_SHIFT)) & CMP_C2_NSCN_MASK)
#define CMP_C2_CHF_MASK         0xFF0000U
#define CMP_C2_CHF_SHIFT        16U
#define CMP_C2_CHF_WIDTH        8U
#define CMP_C2_CHF(x)           (((uint32_t)(((uint32_t)(x)) << CMP_C2_CHF_SHIFT)) & CMP_C2_CHF_MASK)
#define CMP_C2_FCS_MASK         0x7000000U
#define CMP_C2_FCS_SHIFT        24U
#define CMP_C2_FCS_WIDTH        3U
#define CMP_C2_FCS(x)           (((uint32_t)(((uint32_t)(x)) << CMP_C2_FCS_SHIFT)) & CMP_C2_FCS_MASK)
#define CMP_C2_FMP_MASK         0x20000000U
#define CMP_C2_FMP_SHIFT        29U
#define CMP_C2_FMP_WIDTH        1U
#define CMP_C2_FMP(x)           (((uint32_t)(((uint32_t)(x)) << CMP_C2_FMP_SHIFT)) & CMP_C2_FMP_MASK)
#define CMP_C2_RRIE_MASK        0x40000000U
#define CMP_C2_RRIE_SHIFT       30U
#define CMP_C2_RRIE_WIDTH       1U
#define CMP_C2_RRIE(x)          (((uint32_t)(((uint32_t)(x)) << CMP_C2_RRIE_SHIFT)) & CMP_C2_RRIE_MASK)
#define CMP_C2_RRE_MASK         0x80000000U
#define CMP_C2_RRE_SHIFT        31U
#define CMP_C2_RRE_WIDTH        1U
#define CMP_C2_RRE(x)           (((uint32_t)(((uint32_t)(x)) << CMP_C2_RRE_SHIFT)) & CMP_C2_RRE_MASK)
/** C3 Bit Fields */
#define CMP_C3_INITM_MASK       0xFFFU
#define CMP_C3_INITM_SHIFT      0U
#define CMP_C3_INITM_WIDTH      12U
#define CMP_C3_INITM(x)         (((uint32_t)(((uint32_t)(x)) << CMP_C3_INITM_SHIFT)) & CMP_C3_INITM_MASK)

/**
 * @}
 */ /* end of group CMP_Register_Masks */

/**
 * @}
 */ /* end of group CMP_Peripheral_Access_Layer */

// clang-format on

#endif // __CMP_REGISTER_H__
