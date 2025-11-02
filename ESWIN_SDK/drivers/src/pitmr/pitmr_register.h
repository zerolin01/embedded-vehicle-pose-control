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
 * @file pitmr_register.h
 * @brief PITMR driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __PITMR_REGISTER_H__
#define __PITMR_REGISTER_H__

#include <osal.h>

// clang-format off

/* ----------------------------------------------------------------------------
   -- PITMR Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup PITMR_Peripheral_Access_Layer PITMR Peripheral Access Layer
 * @{
 */

/**
 * @brief  PITMR Register Layout Typedef
 */
typedef struct {
  __I  uint32_t VERID;                               /**< Version ID Register, offset: 0x0 */
  __I  uint32_t PARAM;                               /**< Parameter Register, offset: 0x4 */
  __IO uint32_t MCR;                                 /**< Module Control Register, offset: 0x8 */
  __IO uint32_t MSR;                                 /**< Module Status Register, offset: 0xC */
  __IO uint32_t MIER;                                /**< Module Interrupt Enable Register, offset: 0x10 */
  __IO uint32_t SETTEN;                              /**< Set Timer Enable Register, offset: 0x14 */
  __O  uint32_t CLRTEN;                              /**< Clear Timer Enable Register, offset: 0x18 */
       uint8_t RESERVED_0[4];
  struct {                                           /**< offset: 0x20, array step: 0x10 */
    __IO uint32_t TVAL;                              /**< Timer Value Register, array offset: 0x20, array step: 0x10 */
    __I  uint32_t CVAL;                              /**< Current Timer Value, array offset: 0x24, array step: 0x10 */
    __IO uint32_t TCTRL;                             /**< Timer Control Register, array offset: 0x28, array step: 0x10 */
         uint8_t RESERVED_0[4];
  } TMR[PITMR1_TMR_COUNT];
} pitmr_type_t, *pitmr_mem_map_ptr_t;

/* ----------------------------------------------------------------------------
   -- PITMR Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup PITMR_Register_Masks PITMR Register Masks
 * @{
 */

/** VERID Bit Fields */
#define PITMR_VERID_FEATURE_MASK                  0xFFFFU
#define PITMR_VERID_FEATURE_SHIFT                 0U
#define PITMR_VERID_FEATURE_WIDTH                 16U
#define PITMR_VERID_FEATURE(x)                    (((uint32_t)(((uint32_t)(x))<<PITMR_VERID_FEATURE_SHIFT))&PITMR_VERID_FEATURE_MASK)
#define PITMR_VERID_MINOR_MASK                    0xFF0000U
#define PITMR_VERID_MINOR_SHIFT                   16U
#define PITMR_VERID_MINOR_WIDTH                   8U
#define PITMR_VERID_MINOR(x)                      (((uint32_t)(((uint32_t)(x))<<PITMR_VERID_MINOR_SHIFT))&PITMR_VERID_MINOR_MASK)
#define PITMR_VERID_MAJOR_MASK                    0xFF000000U
#define PITMR_VERID_MAJOR_SHIFT                   24U
#define PITMR_VERID_MAJOR_WIDTH                   8U
#define PITMR_VERID_MAJOR(x)                      (((uint32_t)(((uint32_t)(x))<<PITMR_VERID_MAJOR_SHIFT))&PITMR_VERID_MAJOR_MASK)
/** PARAM Bit Fields */
#define PITMR_PARAM_CHANNEL_MASK                  0xFFU
#define PITMR_PARAM_CHANNEL_SHIFT                 0U
#define PITMR_PARAM_CHANNEL_WIDTH                 8U
#define PITMR_PARAM_CHANNEL(x)                    (((uint32_t)(((uint32_t)(x))<<PITMR_PARAM_CHANNEL_SHIFT))&PITMR_PARAM_CHANNEL_MASK)
#define PITMR_PARAM_EXT_TRIG_MASK                 0xFF00U
#define PITMR_PARAM_EXT_TRIG_SHIFT                8U
#define PITMR_PARAM_EXT_TRIG_WIDTH                8U
#define PITMR_PARAM_EXT_TRIG(x)                   (((uint32_t)(((uint32_t)(x))<<PITMR_PARAM_EXT_TRIG_SHIFT))&PITMR_PARAM_EXT_TRIG_MASK)
/** MCR Bit Fields */
#define PITMR_MCR_M_CEN_MASK                      0x1U
#define PITMR_MCR_M_CEN_SHIFT                     0U
#define PITMR_MCR_M_CEN_WIDTH                     1U
#define PITMR_MCR_M_CEN(x)                        (((uint32_t)(((uint32_t)(x))<<PITMR_MCR_M_CEN_SHIFT))&PITMR_MCR_M_CEN_MASK)
#define PITMR_MCR_SW_RST_MASK                     0x2U
#define PITMR_MCR_SW_RST_SHIFT                    1U
#define PITMR_MCR_SW_RST_WIDTH                    1U
#define PITMR_MCR_SW_RST(x)                       (((uint32_t)(((uint32_t)(x))<<PITMR_MCR_SW_RST_SHIFT))&PITMR_MCR_SW_RST_MASK)
#define PITMR_MCR_DOZE_EN_MASK                    0x4U
#define PITMR_MCR_DOZE_EN_SHIFT                   2U
#define PITMR_MCR_DOZE_EN_WIDTH                   1U
#define PITMR_MCR_DOZE_EN(x)                      (((uint32_t)(((uint32_t)(x))<<PITMR_MCR_DOZE_EN_SHIFT))&PITMR_MCR_DOZE_EN_MASK)
#define PITMR_MCR_DBG_EN_MASK                     0x8U
#define PITMR_MCR_DBG_EN_SHIFT                    3U
#define PITMR_MCR_DBG_EN_WIDTH                    1U
#define PITMR_MCR_DBG_EN(x)                       (((uint32_t)(((uint32_t)(x))<<PITMR_MCR_DBG_EN_SHIFT))&PITMR_MCR_DBG_EN_MASK)
/** MSR Bit Fields */
#define PITMR_MSR_TIF0_MASK                       0x1U
#define PITMR_MSR_TIF0_SHIFT                      0U
#define PITMR_MSR_TIF0_WIDTH                      1U
#define PITMR_MSR_TIF0(x)                         (((uint32_t)(((uint32_t)(x))<<PITMR_MSR_TIF0_SHIFT))&PITMR_MSR_TIF0_MASK)
#define PITMR_MSR_TIF1_MASK                       0x2U
#define PITMR_MSR_TIF1_SHIFT                      1U
#define PITMR_MSR_TIF1_WIDTH                      1U
#define PITMR_MSR_TIF1(x)                         (((uint32_t)(((uint32_t)(x))<<PITMR_MSR_TIF1_SHIFT))&PITMR_MSR_TIF1_MASK)
#define PITMR_MSR_TIF2_MASK                       0x4U
#define PITMR_MSR_TIF2_SHIFT                      2U
#define PITMR_MSR_TIF2_WIDTH                      1U
#define PITMR_MSR_TIF2(x)                         (((uint32_t)(((uint32_t)(x))<<PITMR_MSR_TIF2_SHIFT))&PITMR_MSR_TIF2_MASK)
#define PITMR_MSR_TIF3_MASK                       0x8U
#define PITMR_MSR_TIF3_SHIFT                      3U
#define PITMR_MSR_TIF3_WIDTH                      1U
#define PITMR_MSR_TIF3(x)                         (((uint32_t)(((uint32_t)(x))<<PITMR_MSR_TIF3_SHIFT))&PITMR_MSR_TIF3_MASK)
/** MIER Bit Fields */
#define PITMR_MIER_TIE0_MASK                      0x1U
#define PITMR_MIER_TIE0_SHIFT                     0U
#define PITMR_MIER_TIE0_WIDTH                     1U
#define PITMR_MIER_TIE0(x)                        (((uint32_t)(((uint32_t)(x))<<PITMR_MIER_TIE0_SHIFT))&PITMR_MIER_TIE0_MASK)
#define PITMR_MIER_TIE1_MASK                      0x2U
#define PITMR_MIER_TIE1_SHIFT                     1U
#define PITMR_MIER_TIE1_WIDTH                     1U
#define PITMR_MIER_TIE1(x)                        (((uint32_t)(((uint32_t)(x))<<PITMR_MIER_TIE1_SHIFT))&PITMR_MIER_TIE1_MASK)
#define PITMR_MIER_TIE2_MASK                      0x4U
#define PITMR_MIER_TIE2_SHIFT                     2U
#define PITMR_MIER_TIE2_WIDTH                     1U
#define PITMR_MIER_TIE2(x)                        (((uint32_t)(((uint32_t)(x))<<PITMR_MIER_TIE2_SHIFT))&PITMR_MIER_TIE2_MASK)
#define PITMR_MIER_TIE3_MASK                      0x8U
#define PITMR_MIER_TIE3_SHIFT                     3U
#define PITMR_MIER_TIE3_WIDTH                     1U
#define PITMR_MIER_TIE3(x)                        (((uint32_t)(((uint32_t)(x))<<PITMR_MIER_TIE3_SHIFT))&PITMR_MIER_TIE3_MASK)
/** SETTEN Bit Fields */
#define PITMR_SETTEN_SET_T_EN_0_MASK              0x1U
#define PITMR_SETTEN_SET_T_EN_0_SHIFT             0U
#define PITMR_SETTEN_SET_T_EN_0_WIDTH             1U
#define PITMR_SETTEN_SET_T_EN_0(x)                (((uint32_t)(((uint32_t)(x))<<PITMR_SETTEN_SET_T_EN_0_SHIFT))&PITMR_SETTEN_SET_T_EN_0_MASK)
#define PITMR_SETTEN_SET_T_EN_1_MASK              0x2U
#define PITMR_SETTEN_SET_T_EN_1_SHIFT             1U
#define PITMR_SETTEN_SET_T_EN_1_WIDTH             1U
#define PITMR_SETTEN_SET_T_EN_1(x)                (((uint32_t)(((uint32_t)(x))<<PITMR_SETTEN_SET_T_EN_1_SHIFT))&PITMR_SETTEN_SET_T_EN_1_MASK)
#define PITMR_SETTEN_SET_T_EN_2_MASK              0x4U
#define PITMR_SETTEN_SET_T_EN_2_SHIFT             2U
#define PITMR_SETTEN_SET_T_EN_2_WIDTH             1U
#define PITMR_SETTEN_SET_T_EN_2(x)                (((uint32_t)(((uint32_t)(x))<<PITMR_SETTEN_SET_T_EN_2_SHIFT))&PITMR_SETTEN_SET_T_EN_2_MASK)
#define PITMR_SETTEN_SET_T_EN_3_MASK              0x8U
#define PITMR_SETTEN_SET_T_EN_3_SHIFT             3U
#define PITMR_SETTEN_SET_T_EN_3_WIDTH             1U
#define PITMR_SETTEN_SET_T_EN_3(x)                (((uint32_t)(((uint32_t)(x))<<PITMR_SETTEN_SET_T_EN_3_SHIFT))&PITMR_SETTEN_SET_T_EN_3_MASK)
/** CLRTEN Bit Fields */
#define PITMR_CLRTEN_CLR_T_EN_0_MASK              0x1U
#define PITMR_CLRTEN_CLR_T_EN_0_SHIFT             0U
#define PITMR_CLRTEN_CLR_T_EN_0_WIDTH             1U
#define PITMR_CLRTEN_CLR_T_EN_0(x)                (((uint32_t)(((uint32_t)(x))<<PITMR_CLRTEN_CLR_T_EN_0_SHIFT))&PITMR_CLRTEN_CLR_T_EN_0_MASK)
#define PITMR_CLRTEN_CLR_T_EN_1_MASK              0x2U
#define PITMR_CLRTEN_CLR_T_EN_1_SHIFT             1U
#define PITMR_CLRTEN_CLR_T_EN_1_WIDTH             1U
#define PITMR_CLRTEN_CLR_T_EN_1(x)                (((uint32_t)(((uint32_t)(x))<<PITMR_CLRTEN_CLR_T_EN_1_SHIFT))&PITMR_CLRTEN_CLR_T_EN_1_MASK)
#define PITMR_CLRTEN_CLR_T_EN_2_MASK              0x4U
#define PITMR_CLRTEN_CLR_T_EN_2_SHIFT             2U
#define PITMR_CLRTEN_CLR_T_EN_2_WIDTH             1U
#define PITMR_CLRTEN_CLR_T_EN_2(x)                (((uint32_t)(((uint32_t)(x))<<PITMR_CLRTEN_CLR_T_EN_2_SHIFT))&PITMR_CLRTEN_CLR_T_EN_2_MASK)
#define PITMR_CLRTEN_CLR_T_EN_3_MASK              0x8U
#define PITMR_CLRTEN_CLR_T_EN_3_SHIFT             3U
#define PITMR_CLRTEN_CLR_T_EN_3_WIDTH             1U
#define PITMR_CLRTEN_CLR_T_EN_3(x)                (((uint32_t)(((uint32_t)(x))<<PITMR_CLRTEN_CLR_T_EN_3_SHIFT))&PITMR_CLRTEN_CLR_T_EN_3_MASK)
/** TMR_TVAL Bit Fields */
#define PITMR_TMR_TVAL_TMR_VAL_MASK               0xFFFFFFFFU
#define PITMR_TMR_TVAL_TMR_VAL_SHIFT              0U
#define PITMR_TMR_TVAL_TMR_VAL_WIDTH              32U
#define PITMR_TMR_TVAL_TMR_VAL(x)                 (((uint32_t)(((uint32_t)(x))<<PITMR_TMR_TVAL_TMR_VAL_SHIFT))&PITMR_TMR_TVAL_TMR_VAL_MASK)
/** TMR_CVAL Bit Fields */
#define PITMR_TMR_CVAL_TMR_CUR_VAL_MASK           0xFFFFFFFFU
#define PITMR_TMR_CVAL_TMR_CUR_VAL_SHIFT          0U
#define PITMR_TMR_CVAL_TMR_CUR_VAL_WIDTH          32U
#define PITMR_TMR_CVAL_TMR_CUR_VAL(x)             (((uint32_t)(((uint32_t)(x))<<PITMR_TMR_CVAL_TMR_CUR_VAL_SHIFT))&PITMR_TMR_CVAL_TMR_CUR_VAL_MASK)
/** TMR_TCTRL Bit Fields */
#define PITMR_TMR_TCTRL_T_EN_MASK                 0x1U
#define PITMR_TMR_TCTRL_T_EN_SHIFT                0U
#define PITMR_TMR_TCTRL_T_EN_WIDTH                1U
#define PITMR_TMR_TCTRL_T_EN(x)                   (((uint32_t)(((uint32_t)(x))<<PITMR_TMR_TCTRL_T_EN_SHIFT))&PITMR_TMR_TCTRL_T_EN_MASK)
#define PITMR_TMR_TCTRL_CHAIN_MASK                0x2U
#define PITMR_TMR_TCTRL_CHAIN_SHIFT               1U
#define PITMR_TMR_TCTRL_CHAIN_WIDTH               1U
#define PITMR_TMR_TCTRL_CHAIN(x)                  (((uint32_t)(((uint32_t)(x))<<PITMR_TMR_TCTRL_CHAIN_SHIFT))&PITMR_TMR_TCTRL_CHAIN_MASK)
#define PITMR_TMR_TCTRL_MODE_MASK                 0xCU
#define PITMR_TMR_TCTRL_MODE_SHIFT                2U
#define PITMR_TMR_TCTRL_MODE_WIDTH                2U
#define PITMR_TMR_TCTRL_MODE(x)                   (((uint32_t)(((uint32_t)(x))<<PITMR_TMR_TCTRL_MODE_SHIFT))&PITMR_TMR_TCTRL_MODE_MASK)
#define PITMR_TMR_TCTRL_TSOT_MASK                 0x10000U
#define PITMR_TMR_TCTRL_TSOT_SHIFT                16U
#define PITMR_TMR_TCTRL_TSOT_WIDTH                1U
#define PITMR_TMR_TCTRL_TSOT(x)                   (((uint32_t)(((uint32_t)(x))<<PITMR_TMR_TCTRL_TSOT_SHIFT))&PITMR_TMR_TCTRL_TSOT_MASK)
#define PITMR_TMR_TCTRL_TSOI_MASK                 0x20000U
#define PITMR_TMR_TCTRL_TSOI_SHIFT                17U
#define PITMR_TMR_TCTRL_TSOI_WIDTH                1U
#define PITMR_TMR_TCTRL_TSOI(x)                   (((uint32_t)(((uint32_t)(x))<<PITMR_TMR_TCTRL_TSOI_SHIFT))&PITMR_TMR_TCTRL_TSOI_MASK)
#define PITMR_TMR_TCTRL_TROT_MASK                 0x40000U
#define PITMR_TMR_TCTRL_TROT_SHIFT                18U
#define PITMR_TMR_TCTRL_TROT_WIDTH                1U
#define PITMR_TMR_TCTRL_TROT(x)                   (((uint32_t)(((uint32_t)(x))<<PITMR_TMR_TCTRL_TROT_SHIFT))&PITMR_TMR_TCTRL_TROT_MASK)
#define PITMR_TMR_TCTRL_TRG_SRC_MASK              0x800000U
#define PITMR_TMR_TCTRL_TRG_SRC_SHIFT             23U
#define PITMR_TMR_TCTRL_TRG_SRC_WIDTH             1U
#define PITMR_TMR_TCTRL_TRG_SRC(x)                (((uint32_t)(((uint32_t)(x))<<PITMR_TMR_TCTRL_TRG_SRC_SHIFT))&PITMR_TMR_TCTRL_TRG_SRC_MASK)
#define PITMR_TMR_TCTRL_TRG_SEL_MASK              0xF000000U
#define PITMR_TMR_TCTRL_TRG_SEL_SHIFT             24U
#define PITMR_TMR_TCTRL_TRG_SEL_WIDTH             4U
#define PITMR_TMR_TCTRL_TRG_SEL(x)                (((uint32_t)(((uint32_t)(x))<<PITMR_TMR_TCTRL_TRG_SEL_SHIFT))&PITMR_TMR_TCTRL_TRG_SEL_MASK)

/**
 * @}
 */ /* end of group PITMR_Register_Masks */

/**
 * @}
 */ /* end of group PITMR_Peripheral_Access_Layer */

// clang-format on

#endif // __PITMR_REGISTER_H__
