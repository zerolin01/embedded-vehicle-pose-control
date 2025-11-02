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
 * @file pctmr_register.h
 * @brief PCTMR driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-12
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __PCTMR_REGISTER_H__
#define __PCTMR_REGISTER_H__

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
   -- PCTMR Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup PCTMR_Peripheral_Access_Layer PCTMR Peripheral Access Layer
 * @{
 */

/**
 * @brief PCTMR Register Layout Typedef
 */
typedef struct {
  __IO uint32_t CSR;                               /**< Low Power Timer Control Status Register, offset: 0x0 */
  __IO uint32_t PSR;                               /**< Low Power Timer Prescale Register, offset: 0x4 */
  __IO uint32_t CMR;                               /**< Low Power Timer Compare Register, offset: 0x8 */
  __IO uint32_t CNR;                               /**< Low Power Timer Counter Register, offset: 0xC */
} pctmr_type_t, *pctmr_mem_map_ptr_t;

/* ----------------------------------------------------------------------------
   -- PCTMR Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup PCTMR_Register_Masks PCTMR Register Masks
 * @{
 */

/** CSR Bit Fields */
#define PCTMR_CSR_TEN_MASK                       0x1U
#define PCTMR_CSR_TEN_SHIFT                      0U
#define PCTMR_CSR_TEN_WIDTH                      1U
#define PCTMR_CSR_TEN(x)                         (((uint32_t)(((uint32_t)(x))<<PCTMR_CSR_TEN_SHIFT))&PCTMR_CSR_TEN_MASK)
#define PCTMR_CSR_TMS_MASK                       0x2U
#define PCTMR_CSR_TMS_SHIFT                      1U
#define PCTMR_CSR_TMS_WIDTH                      1U
#define PCTMR_CSR_TMS(x)                         (((uint32_t)(((uint32_t)(x))<<PCTMR_CSR_TMS_SHIFT))&PCTMR_CSR_TMS_MASK)
#define PCTMR_CSR_TFC_MASK                       0x4U
#define PCTMR_CSR_TFC_SHIFT                      2U
#define PCTMR_CSR_TFC_WIDTH                      1U
#define PCTMR_CSR_TFC(x)                         (((uint32_t)(((uint32_t)(x))<<PCTMR_CSR_TFC_SHIFT))&PCTMR_CSR_TFC_MASK)
#define PCTMR_CSR_TPP_MASK                       0x8U
#define PCTMR_CSR_TPP_SHIFT                      3U
#define PCTMR_CSR_TPP_WIDTH                      1U
#define PCTMR_CSR_TPP(x)                         (((uint32_t)(((uint32_t)(x))<<PCTMR_CSR_TPP_SHIFT))&PCTMR_CSR_TPP_MASK)
#define PCTMR_CSR_TPS_MASK                       0x30U
#define PCTMR_CSR_TPS_SHIFT                      4U
#define PCTMR_CSR_TPS_WIDTH                      2U
#define PCTMR_CSR_TPS(x)                         (((uint32_t)(((uint32_t)(x))<<PCTMR_CSR_TPS_SHIFT))&PCTMR_CSR_TPS_MASK)
#define PCTMR_CSR_TIE_MASK                       0x40U
#define PCTMR_CSR_TIE_SHIFT                      6U
#define PCTMR_CSR_TIE_WIDTH                      1U
#define PCTMR_CSR_TIE(x)                         (((uint32_t)(((uint32_t)(x))<<PCTMR_CSR_TIE_SHIFT))&PCTMR_CSR_TIE_MASK)
#define PCTMR_CSR_TCF_MASK                       0x80U
#define PCTMR_CSR_TCF_SHIFT                      7U
#define PCTMR_CSR_TCF_WIDTH                      1U
#define PCTMR_CSR_TCF(x)                         (((uint32_t)(((uint32_t)(x))<<PCTMR_CSR_TCF_SHIFT))&PCTMR_CSR_TCF_MASK)
#define PCTMR_CSR_TDRE_MASK                      0x100U
#define PCTMR_CSR_TDRE_SHIFT                     8U
#define PCTMR_CSR_TDRE_WIDTH                     1U
#define PCTMR_CSR_TDRE(x)                        (((uint32_t)(((uint32_t)(x))<<PCTMR_CSR_TDRE_SHIFT))&PCTMR_CSR_TDRE_MASK)
/** PSR Bit Fields */
#define PCTMR_PSR_PCS_MASK                       0x3U
#define PCTMR_PSR_PCS_SHIFT                      0U
#define PCTMR_PSR_PCS_WIDTH                      2U
#define PCTMR_PSR_PCS(x)                         (((uint32_t)(((uint32_t)(x))<<PCTMR_PSR_PCS_SHIFT))&PCTMR_PSR_PCS_MASK)
#define PCTMR_PSR_PBYP_MASK                      0x4U
#define PCTMR_PSR_PBYP_SHIFT                     2U
#define PCTMR_PSR_PBYP_WIDTH                     1U
#define PCTMR_PSR_PBYP(x)                        (((uint32_t)(((uint32_t)(x))<<PCTMR_PSR_PBYP_SHIFT))&PCTMR_PSR_PBYP_MASK)
#define PCTMR_PSR_PRESCALE_MASK                  0x78U
#define PCTMR_PSR_PRESCALE_SHIFT                 3U
#define PCTMR_PSR_PRESCALE_WIDTH                 4U
#define PCTMR_PSR_PRESCALE(x)                    (((uint32_t)(((uint32_t)(x))<<PCTMR_PSR_PRESCALE_SHIFT))&PCTMR_PSR_PRESCALE_MASK)
/** CMR Bit Fields */
#define PCTMR_CMR_COMPARE_MASK                   0xFFFFU
#define PCTMR_CMR_COMPARE_SHIFT                  0U
#define PCTMR_CMR_COMPARE_WIDTH                  16U
#define PCTMR_CMR_COMPARE(x)                     (((uint32_t)(((uint32_t)(x))<<PCTMR_CMR_COMPARE_SHIFT))&PCTMR_CMR_COMPARE_MASK)
/** CNR Bit Fields */
#define PCTMR_CNR_COUNTER_MASK                   0xFFFFU
#define PCTMR_CNR_COUNTER_SHIFT                  0U
#define PCTMR_CNR_COUNTER_WIDTH                  16U
#define PCTMR_CNR_COUNTER(x)                     (((uint32_t)(((uint32_t)(x))<<PCTMR_CNR_COUNTER_SHIFT))&PCTMR_CNR_COUNTER_MASK)

/**
 * @}
 */ /* end of group PCTMR_Register_Masks */

/**
 * @}
 */ /* end of group PCTMR_Peripheral_Access_Layer */


/**
 * @}
 */ /* end of group Peripheral_Access_Layer_EAM2011 */
// clang-format on

#endif // __PCTMR_REGISTER_H__
