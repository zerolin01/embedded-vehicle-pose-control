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
 * @file adc_register.h
 * @brief ADC driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __ADC_REGISTER_H__
#define __ADC_REGISTER_H__

#include "emps_platform.h"
#include <osal.h>
#include <stdint.h>

// clang-format off
/* ----------------------------------------------------------------------------
   -- ADC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup ADC_Peripheral_Access_Layer ADC Peripheral Access Layer
 * @{
 */

/**
 * @brief ADC Register Layout Typedef
 */
typedef struct {
  __IO uint32_t SC1[ADC_SC1_COUNT];                /**< ADC Status and Control Register 1, array offset: 0x0, array step: 0x4 */
  __I  uint32_t INTS;                              /**< ADC Interrupt Status Register, offset: 0x60 */
       uint32_t RESERVED_0[5];
  __IO uint32_t SC2;                               /**< Status and Control Register 2, offset: 0x78 */
  __IO uint32_t SC3;                               /**< Status and Control Register 3, offset: 0x7C */
  __IO uint32_t FW;                                /**< ADC Frequency word register 1, offset: 0x80 */
  __IO uint32_t FPH0;                              /**< ADC Configuration Register 1, offset: 0x84 */
  __IO uint32_t FPH1;                              /**< ADC Configuration Register 1, offset: 0x88*/
  __IO uint32_t COR;                               /**< ADC Correct offset register 1, offset: 0x8C */
  __IO uint32_t CV[ADC_CV_COUNT];                  /**< Compare Value Registers, array offset: 0x90, array step: 0x4 */
  __IO uint32_t CR;                                /**< ADC Configuration Register 1, offset: 0x98 */
       uint32_t RESERVED_1;
  __I  uint32_t R[ADC_R_COUNT];                    /**< ADC Data Result Registers, array offset: 0xA0, array step: 0x4 */
} adc_type_t, *adc_mem_map_ptr_t;

/* ----------------------------------------------------------------------------
   -- ADC Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup ADC_Register_Masks ADC Register Masks
 * @{
 */

/** SC1 Bit Fields */
#define ADC_SC1_ADCH_MASK                        0x3FU
#define ADC_SC1_ADCH_SHIFT                       0U
#define ADC_SC1_ADCH_WIDTH                       6U
#define ADC_SC1_ADCH(x)                          (((uint32_t)(((uint32_t)(x))<<ADC_SC1_ADCH_SHIFT))&ADC_SC1_ADCH_MASK)
#define ADC_SC1_CIEN_MASK                        0x40U
#define ADC_SC1_CIEN_SHIFT                       6U
#define ADC_SC1_CIEN_WIDTH                       1U
#define ADC_SC1_CIEN(x)                          (((uint32_t)(((uint32_t)(x))<<ADC_SC1_CIEN_SHIFT))&ADC_SC1_CIEN_MASK)
#define ADC_SC1_CCF_MASK                         0x80U
#define ADC_SC1_CCF_SHIFT                        7U
#define ADC_SC1_CCF_WIDTH                        1U
#define ADC_SC1_CCF(x)                           (((uint32_t)(((uint32_t)(x))<<ADC_SC1_CCF_SHIFT))&ADC_SC1_CCF_MASK)
/** INTS Bit Fields */
#define ADC_INTS_INTS_MASK                       ((1 << ADC_SC1_COUNT) - 1)
#define ADC_INTS_INTS_SHIFT                      0U
#define ADC_INTS_INTS_WIDTH                      ADC_SC1_COUNT
#define ADC_INTS_INTS(x)                         (((uint32_t)(((uint32_t)(x))<<ADC_SC1_ADCH_SHIFT))&ADC_SC1_ADCH_MASK)
/** SC2 Bit Fields */
#define ADC_SC2_DMAEN_MASK                       0x1U
#define ADC_SC2_DMAEN_SHIFT                      0U
#define ADC_SC2_DMAEN_WIDTH                      1U
#define ADC_SC2_DMAEN(x)                         (((uint32_t)(((uint32_t)(x))<<ADC_SC2_DMAEN_SHIFT))&ADC_SC2_DMAEN_MASK)
#define ADC_SC2_CREN_MASK                        0x2U
#define ADC_SC2_CREN_SHIFT                       1U
#define ADC_SC2_CREN_WIDTH                       1U
#define ADC_SC2_CREN(x)                          (((uint32_t)(((uint32_t)(x))<<ADC_SC2_CREN_SHIFT))&ADC_SC2_CREN_MASK)
#define ADC_SC2_CFGT_MASK                        0x4U
#define ADC_SC2_CFGT_SHIFT                       2U
#define ADC_SC2_CFGT_WIDTH                       1U
#define ADC_SC2_CFGT(x)                          (((uint32_t)(((uint32_t)(x))<<ADC_SC2_CFGT_SHIFT))&ADC_SC2_CFGT_MASK)
#define ADC_SC2_CFE_MASK                         0x8U
#define ADC_SC2_CFE_SHIFT                        3U
#define ADC_SC2_CFE_WIDTH                        1U
#define ADC_SC2_CFE(x)                           (((uint32_t)(((uint32_t)(x))<<ADC_SC2_CFE_SHIFT))&ADC_SC2_CFE_MASK)
#define ADC_SC2_TRGS_MASK                        0x10U
#define ADC_SC2_TRGS_SHIFT                       4U
#define ADC_SC2_TRGS_WIDTH                       1U
#define ADC_SC2_TRGS(x)                          (((uint32_t)(((uint32_t)(x))<<ADC_SC2_TRGS_SHIFT))&ADC_SC2_TRGS_MASK)
#define ADC_SC2_ACT_MASK                         0x20U
#define ADC_SC2_ACT_SHIFT                        5U
#define ADC_SC2_ACT_WIDTH                        1U
#define ADC_SC2_ACT(x)                           (((uint32_t)(((uint32_t)(x))<<ADC_SC2_ACT_SHIFT))&ADC_SC2_ACT_MASK)
#define ADC_SC2_CSE_MASK                         0x40U
#define ADC_SC2_CSE_SHIFT                        6U
#define ADC_SC2_CSE_WIDTH                        1U
#define ADC_SC2_CSE(x)                           (((uint32_t)(((uint32_t)(x))<<ADC_SC2_CSE_SHIFT))&ADC_SC2_CSE_MASK)
#define ADC_SC2_ADEN_MASK                        0x80U
#define ADC_SC2_ADEN_SHIFT                       7U
#define ADC_SC2_ADEN_WIDTH                       1U
#define ADC_SC2_ADEN(x)                          (((uint32_t)(((uint32_t)(x))<<ADC_SC2_ADEN_SHIFT))&ADC_SC2_ADEN_MASK)
#define ADC_SC2_ESCN_MASK                        (((1U << ADC_SC1_COUNT) - 1U) << 8U)
#define ADC_SC2_ESCN_SHIFT                       8U
#define ADC_SC2_ESCN_WIDTH                       ADC_SC1_COUNT
#define ADC_SC2_ESCN(x)                          (((uint32_t)(((uint32_t)(x))<<ADC_SC2_ESCN_SHIFT))&ADC_SC2_ESCN_MASK)
/** SC3 Bit Fields */
#define ADC_SC3_AVGS_MASK                        0x3U
#define ADC_SC3_AVGS_SHIFT                       0U
#define ADC_SC3_AVGS_WIDTH                       2U
#define ADC_SC3_AVGS(x)                          (((uint32_t)(((uint32_t)(x))<<ADC_SC3_AVGS_SHIFT))&ADC_SC3_AVGS_MASK)
#define ADC_SC3_AVGE_MASK                        0x4U
#define ADC_SC3_AVGE_SHIFT                       2U
#define ADC_SC3_AVGE_WIDTH                       1U
#define ADC_SC3_AVGE(x)                          (((uint32_t)(((uint32_t)(x))<<ADC_SC3_AVGE_SHIFT))&ADC_SC3_AVGE_MASK)
#define ADC_SC3_CCE_MASK                         0x8U
#define ADC_SC3_CCE_SHIFT                        3U
#define ADC_SC3_CCE_WIDTH                        1U
#define ADC_SC3_CCE(x)                           (((uint32_t)(((uint32_t)(x))<<ADC_SC3_CCE_SHIFT))&ADC_SC3_CCE_MASK)
#define ADC_SC3_GCMP_MASK                        0x10U
#define ADC_SC3_GCMP_SHIFT                       4U
#define ADC_SC3_GCMP_WIDTH                       1U
#define ADC_SC3_GCMP(x)                          (((uint32_t)(((uint32_t)(x))<<ADC_SC3_GCMP_SHIFT))&ADC_SC3_GCMP_MASK)
#define ADC_SC3_DIFF_MASK                        0x20U
#define ADC_SC3_DIFF_SHIFT                       5U
#define ADC_SC3_DIFF_WIDTH                       1U
#define ADC_SC3_DIFF(x)                          (((uint32_t)(((uint32_t)(x))<<ADC_SC3_DIFF_SHIFT))&ADC_SC3_DIFF_MASK)
#define ADC_SC3_ASR_MASK                         0x100U
#define ADC_SC3_ASR_SHIFT                        8U
#define ADC_SC3_ASR_WIDTH                        1U
#define ADC_SC3_ASR(x)                           (((uint32_t)(((uint32_t)(x))<<ADC_SC3_ASR_SHIFT))&ADC_SC3_ASR_MASK)
#define ADC_SC3_SDMAEN_MASK                      0x10000U
#define ADC_SC3_SDMAEN_SHIFT                     16U
#define ADC_SC3_SDMAEN_WIDTH                     1U
#define ADC_SC3_SDMAEN(x)                        (((uint32_t)(((uint32_t)(x))<<ADC_SC3_SDMAEN_SHIFT))&ADC_SC3_SDMAEN_MASK)
#define ADC_SC3_SFWL_MASK                        0x60000U
#define ADC_SC3_SFWL_SHIFT                       17U
#define ADC_SC3_SFWL_WIDTH                       2U
#define ADC_SC3_SFWL(x)                          (((uint32_t)(((uint32_t)(x))<<ADC_SC3_SFWL_SHIFT))&ADC_SC3_SFWL_MASK)
#define ADC_SC3_DMADF_MASK                       0x180000U
#define ADC_SC3_DMADF_SHIFT                      19U
#define ADC_SC3_DMADF_WIDTH                      2U
#define ADC_SC3_DMADF(x)                         (((uint32_t)(((uint32_t)(x))<<ADC_SC3_DMADF_SHIFT))&ADC_SC3_DMADF_MASK)
/** FW Bit Fields */
#define ADC_FW_FW_MASK                           0xFFFFFFFFU
#define ADC_FW_FW_SHIFT                          0U
#define ADC_FW_FW_WIDTH                          32U
#define ADC_FW_FW(x)                             (((uint32_t)(((uint32_t)(x))<<ADC_FW_FW_SHIFT))&ADC_FW_FW_MASK)
/** FPH0 Bit Fields */
#define ADC_FPH0_FPH0_MASK                       0xFFFFFFFFU
#define ADC_FPH0_FPH0_SHIFT                      0U
#define ADC_FPH0_FPH0_WIDTH                      32U
#define ADC_FPH0_FPH0(x)                         (((uint32_t)(((uint32_t)(x))<<ADC_FPH0_FPH0_SHIFT))&ADC_FPH0_FPH0_MASK)
/** FPH1 Bit Fields */
#define ADC_FPH1_FPH1_MASK                       0x1U
#define ADC_FPH1_FPH1_SHIFT                      0U
#define ADC_FPH1_FPH1_WIDTH                      1U
#define ADC_FPH1_FPH1(x)                         (((uint32_t)(((uint32_t)(x))<<ADC_FPH1_FPH1_SHIFT))&ADC_FPH1_FPH1_MASK)
/** COR Bit Fields */
#define ADC_COR_OFS_MASK                         0x1FU
#define ADC_COR_OFS_SHIFT                        1U
#define ADC_COR_OFS_WIDTH                        5U
#define ADC_COR_OFS(x)                           (((uint32_t)(((uint32_t)(x))<<ADC_COR_OFS_SHIFT))&ADC_COR_OFS_MASK)
#define ADC_COR_OFS_SIGN_MASK                    0x20U
#define ADC_COR_OFS_SIGN_SHIFT                   5U
#define ADC_COR_OFS_SIGN_WIDTH                   1U
#define ADC_COR_OFS_SIGN(x)                      (((uint32_t)(((uint32_t)(x))<<ADC_COR_OFS_SIGN_SHIFT))&ADC_COR_OFS_SIGN_MASK)
#define ADC_COR_GAIN_MASK                        0x7FF0000U
#define ADC_COR_GAIN_SHIFT                       16U
#define ADC_COR_GAIN_WIDTH                       11U
#define ADC_COR_GAIN(x)                          (((uint32_t)(((uint32_t)(x))<<ADC_COR_GAIN_SHIFT))&ADC_COR_GAIN_MASK)
/** CR Bit Fields */
#define ADC_CR_DMAR_MASK                         0x1U
#define ADC_CR_DMAR_SHIFT                        0U
#define ADC_CR_DMAR_WIDTH                        1U
#define ADC_CR_DMAR(x)                           (((uint32_t)(((uint32_t)(x))<<ADC_CR_DMAR_SHIFT))&ADC_CR_DMAR_MASK)
#define ADC_CR_CAL_MASK                          0x100U
#define ADC_CR_CAL_SHIFT                         8U
#define ADC_CR_CAL_WIDTH                         1U
#define ADC_CR_CAL(x)                            (((uint32_t)(((uint32_t)(x))<<ADC_CR_CAL_SHIFT))&ADC_CR_CAL_MASK)
#define ADC_CR_STR_MASK                          0x10000U
#define ADC_CR_STR_SHIFT                         16U
#define ADC_CR_STR_WIDTH                         1U
#define ADC_CR_STR(x)                            (((uint32_t)(((uint32_t)(x))<<ADC_CR_STR_SHIFT))&ADC_CR_STR_MASK)
#define ADC_CR_OPEN_MASK                         0x80000000U
#define ADC_CR_OPEN_SHIFT                        31U
#define ADC_CR_OPEN_WIDTH                        1U
#define ADC_CR_OPEN(x)                           (((uint32_t)(((uint32_t)(x))<<ADC_CR_OPEN_SHIFT))&ADC_CR_OPEN_MASK)
/** R Bit Fields */
#define ADC_R_D_MASK                             0xFFFU
#define ADC_R_D_SHIFT                            0U
#define ADC_R_D_WIDTH                            12U
#define ADC_R_D(x)                               (((uint32_t)(((uint32_t)(x))<<ADC_R_D_SHIFT))&ADC_R_D_MASK)
/** CV Bit Fields */
#define ADC_CV_CV_MASK                           0xFFFU
#define ADC_CV_CV_SHIFT                          0U
#define ADC_CV_CV_WIDTH                          12U
#define ADC_CV_CV(x)                             (((uint32_t)(((uint32_t)(x))<<ADC_CV_CV_SHIFT))&ADC_CV_CV_MASK)

/**
 * @}
 */ /* end of group ADC_Register_Masks */

/**
 * @}
 */ /* end of group ADC_Peripheral_Access_Layer */

// clang-format on

#endif // __ADC_REGISTER_H__
