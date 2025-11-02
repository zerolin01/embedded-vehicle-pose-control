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
 * @file pdu_register.h
 * @brief PDU driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-12
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __PDU_REGISTER_H__
#define __PDU_REGISTER_H__

// clang-format off
/* ----------------------------------------------------------------------------
   -- PDU Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup PDU_Peripheral_Access_Layer PDU Peripheral Access Layer
 * @{
 */


/**
 * @brief PDU Register Layout Typedef.
 */
typedef struct {
  __IO uint32_t SC;                                /**< Status and Control register, offset: 0x0 */
  __IO uint32_t MOD;                               /**< Modulus register, offset: 0x4 */
  __I  uint32_t CNT;                               /**< Counter register, offset: 0x8 */
  __IO uint32_t IDLY;                              /**< Interrupt Delay register, offset: 0xC */
  struct {                                         /**< offset: 0x10, array step: 0x28 */
    __IO uint32_t C1;                              /**< Channel n Control register 1, array offset: 0x10, array step: 0x28 */
    __IO uint32_t S;                               /**< Channel n Status register, array offset: 0x14, array step: 0x28 */
    __IO uint32_t DLY[PDU_DLY_COUNT];              /**< Channel n Delay 0 register..Channel n Delay 7 register, array offset: 0x18, array step: index*0x28, index2*0x4 */
  } CH[PDU_CH_COUNT];
  __IO uint32_t POEN;                              /**< Pulse-Out n Enable register, offset: 0xB0 */
  struct {                                         /**< offset: 0x194, array step: 0x4 */
    __IO uint32_t PODLY;                           /**< Pulse-Out n Delay register, array offset: 0xB4, array step: 0x4 */
  } POnDLY[PDU_POnDLY_COUNT];
} pdu_type_t, *pdu_mem_map_ptr_t;

/* ----------------------------------------------------------------------------
   -- PDU Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup PDU_Register_Masks PDU Register Masks
 * @{
 */

/** SC Bit Fields */
#define PDU_SC_LDOK_MASK                         0x1U
#define PDU_SC_LDOK_SHIFT                        0U
#define PDU_SC_LDOK_WIDTH                        1U
#define PDU_SC_LDOK(x)                           (((uint32_t)(((uint32_t)(x))<<PDU_SC_LDOK_SHIFT))&PDU_SC_LDOK_MASK)
#define PDU_SC_CONT_MASK                         0x2U
#define PDU_SC_CONT_SHIFT                        1U
#define PDU_SC_CONT_WIDTH                        1U
#define PDU_SC_CONT(x)                           (((uint32_t)(((uint32_t)(x))<<PDU_SC_CONT_SHIFT))&PDU_SC_CONT_MASK)
#define PDU_SC_MULT_MASK                         0xCU
#define PDU_SC_MULT_SHIFT                        2U
#define PDU_SC_MULT_WIDTH                        2U
#define PDU_SC_MULT(x)                           (((uint32_t)(((uint32_t)(x))<<PDU_SC_MULT_SHIFT))&PDU_SC_MULT_MASK)
#define PDU_SC_PDUIE_MASK                        0x20U
#define PDU_SC_PDUIE_SHIFT                       5U
#define PDU_SC_PDUIE_WIDTH                       1U
#define PDU_SC_PDUIE(x)                          (((uint32_t)(((uint32_t)(x))<<PDU_SC_PDUIE_SHIFT))&PDU_SC_PDUIE_MASK)
#define PDU_SC_PDUIF_MASK                        0x40U
#define PDU_SC_PDUIF_SHIFT                       6U
#define PDU_SC_PDUIF_WIDTH                       1U
#define PDU_SC_PDUIF(x)                          (((uint32_t)(((uint32_t)(x))<<PDU_SC_PDUIF_SHIFT))&PDU_SC_PDUIF_MASK)
#define PDU_SC_PDUEN_MASK                        0x80U
#define PDU_SC_PDUEN_SHIFT                       7U
#define PDU_SC_PDUEN_WIDTH                       1U
#define PDU_SC_PDUEN(x)                          (((uint32_t)(((uint32_t)(x))<<PDU_SC_PDUEN_SHIFT))&PDU_SC_PDUEN_MASK)
#define PDU_SC_TRGSEL_MASK                       0xF00U
#define PDU_SC_TRGSEL_SHIFT                      8U
#define PDU_SC_TRGSEL_WIDTH                      4U
#define PDU_SC_TRGSEL(x)                         (((uint32_t)(((uint32_t)(x))<<PDU_SC_TRGSEL_SHIFT))&PDU_SC_TRGSEL_MASK)
#define PDU_SC_PRESCALER_MASK                    0x7000U
#define PDU_SC_PRESCALER_SHIFT                   12U
#define PDU_SC_PRESCALER_WIDTH                   3U
#define PDU_SC_PRESCALER(x)                      (((uint32_t)(((uint32_t)(x))<<PDU_SC_PRESCALER_SHIFT))&PDU_SC_PRESCALER_MASK)
#define PDU_SC_DMAEN_MASK                        0x8000U
#define PDU_SC_DMAEN_SHIFT                       15U
#define PDU_SC_DMAEN_WIDTH                       1U
#define PDU_SC_DMAEN(x)                          (((uint32_t)(((uint32_t)(x))<<PDU_SC_DMAEN_SHIFT))&PDU_SC_DMAEN_MASK)
#define PDU_SC_SWTRIG_MASK                       0x10000U
#define PDU_SC_SWTRIG_SHIFT                      16U
#define PDU_SC_SWTRIG_WIDTH                      1U
#define PDU_SC_SWTRIG(x)                         (((uint32_t)(((uint32_t)(x))<<PDU_SC_SWTRIG_SHIFT))&PDU_SC_SWTRIG_MASK)
#define PDU_SC_PDUEIE_MASK                       0x20000U
#define PDU_SC_PDUEIE_SHIFT                      17U
#define PDU_SC_PDUEIE_WIDTH                      1U
#define PDU_SC_PDUEIE(x)                         (((uint32_t)(((uint32_t)(x))<<PDU_SC_PDUEIE_SHIFT))&PDU_SC_PDUEIE_MASK)
#define PDU_SC_LDMOD_MASK                        0xC0000U
#define PDU_SC_LDMOD_SHIFT                       18U
#define PDU_SC_LDMOD_WIDTH                       2U
#define PDU_SC_LDMOD(x)                          (((uint32_t)(((uint32_t)(x))<<PDU_SC_LDMOD_SHIFT))&PDU_SC_LDMOD_MASK)
/** MOD Bit Fields */
#define PDU_MOD_MOD_MASK                         0xFFFFU
#define PDU_MOD_MOD_SHIFT                        0U
#define PDU_MOD_MOD_WIDTH                        16U
#define PDU_MOD_MOD(x)                           (((uint32_t)(((uint32_t)(x))<<PDU_MOD_MOD_SHIFT))&PDU_MOD_MOD_MASK)
/** CNT Bit Fields */
#define PDU_CNT_CNT_MASK                         0xFFFFU
#define PDU_CNT_CNT_SHIFT                        0U
#define PDU_CNT_CNT_WIDTH                        16U
#define PDU_CNT_CNT(x)                           (((uint32_t)(((uint32_t)(x))<<PDU_CNT_CNT_SHIFT))&PDU_CNT_CNT_MASK)
/** IDLY Bit Fields */
#define PDU_IDLY_IDLY_MASK                       0xFFFFU
#define PDU_IDLY_IDLY_SHIFT                      0U
#define PDU_IDLY_IDLY_WIDTH                      16U
#define PDU_IDLY_IDLY(x)                         (((uint32_t)(((uint32_t)(x))<<PDU_IDLY_IDLY_SHIFT))&PDU_IDLY_IDLY_MASK)
/** C1 Bit Fields */
#define PDU_C1_EN_MASK                           0xFFU
#define PDU_C1_EN_SHIFT                          0U
#define PDU_C1_EN_WIDTH                          8U
#define PDU_C1_EN(x)                             (((uint32_t)(((uint32_t)(x))<<PDU_C1_EN_SHIFT))&PDU_C1_EN_MASK)
#define PDU_C1_TOS_MASK                          0xFF00U
#define PDU_C1_TOS_SHIFT                         8U
#define PDU_C1_TOS_WIDTH                         8U
#define PDU_C1_TOS(x)                            (((uint32_t)(((uint32_t)(x))<<PDU_C1_TOS_SHIFT))&PDU_C1_TOS_MASK)
#define PDU_C1_BB_MASK                           0xFF0000U
#define PDU_C1_BB_SHIFT                          16U
#define PDU_C1_BB_WIDTH                          8U
#define PDU_C1_BB(x)                             (((uint32_t)(((uint32_t)(x))<<PDU_C1_BB_SHIFT))&PDU_C1_BB_MASK)
/** S Bit Fields */
#define PDU_S_ERR_MASK                           0xFFU
#define PDU_S_ERR_SHIFT                          0U
#define PDU_S_ERR_WIDTH                          8U
#define PDU_S_ERR(x)                             (((uint32_t)(((uint32_t)(x))<<PDU_S_ERR_SHIFT))&PDU_S_ERR_MASK)
#define PDU_S_CF_MASK                            0xFF0000U
#define PDU_S_CF_SHIFT                           16U
#define PDU_S_CF_WIDTH                           8U
#define PDU_S_CF(x)                              (((uint32_t)(((uint32_t)(x))<<PDU_S_CF_SHIFT))&PDU_S_CF_MASK)
/** DLY Bit Fields */
#define PDU_DLY_DLY_MASK                         0xFFFFU
#define PDU_DLY_DLY_SHIFT                        0U
#define PDU_DLY_DLY_WIDTH                        16U
#define PDU_DLY_DLY(x)                           (((uint32_t)(((uint32_t)(x))<<PDU_DLY_DLY_SHIFT))&PDU_DLY_DLY_MASK)
/** POEN Bit Fields */
#define PDU_POEN_POEN_MASK                       0xFFU
#define PDU_POEN_POEN_SHIFT                      0U
#define PDU_POEN_POEN_WIDTH                      8U
#define PDU_POEN_POEN(x)                         (((uint32_t)(((uint32_t)(x))<<PDU_POEN_POEN_SHIFT))&PDU_POEN_POEN_MASK)
/** POnDLY_PODLY Bit Fields */
#define PDU_POnDLY_PODLY_DLY2_MASK               0xFFFFU
#define PDU_POnDLY_PODLY_DLY2_SHIFT              0U
#define PDU_POnDLY_PODLY_DLY2_WIDTH              16U
#define PDU_POnDLY_PODLY_DLY2(x)                 (((uint32_t)(((uint32_t)(x))<<PDU_POnDLY_PODLY_DLY2_SHIFT))&PDU_POnDLY_PODLY_DLY2_MASK)
#define PDU_POnDLY_PODLY_DLY1_MASK               0xFFFF0000U
#define PDU_POnDLY_PODLY_DLY1_SHIFT              16U
#define PDU_POnDLY_PODLY_DLY1_WIDTH              16U
#define PDU_POnDLY_PODLY_DLY1(x)                 (((uint32_t)(((uint32_t)(x))<<PDU_POnDLY_PODLY_DLY1_SHIFT))&PDU_POnDLY_PODLY_DLY1_MASK)
/** POnDLY_ACCESS16BIT_DLY2 Bit Fields */
#define PDU_POnDLY_ACCESS16BIT_DLY2_DLY2_MASK    0xFFFFU
#define PDU_POnDLY_ACCESS16BIT_DLY2_DLY2_SHIFT   0U
#define PDU_POnDLY_ACCESS16BIT_DLY2_DLY2_WIDTH   16U
#define PDU_POnDLY_ACCESS16BIT_DLY2_DLY2(x)      (((uint16_t)(((uint16_t)(x))<<PDU_POnDLY_ACCESS16BIT_DLY2_DLY2_SHIFT))&PDU_POnDLY_ACCESS16BIT_DLY2_DLY2_MASK)
/** POnDLY_ACCESS16BIT_DLY1 Bit Fields */
#define PDU_POnDLY_ACCESS16BIT_DLY1_DLY1_MASK    0xFFFFU
#define PDU_POnDLY_ACCESS16BIT_DLY1_DLY1_SHIFT   0U
#define PDU_POnDLY_ACCESS16BIT_DLY1_DLY1_WIDTH   16U
#define PDU_POnDLY_ACCESS16BIT_DLY1_DLY1(x)      (((uint16_t)(((uint16_t)(x))<<PDU_POnDLY_ACCESS16BIT_DLY1_DLY1_SHIFT))&PDU_POnDLY_ACCESS16BIT_DLY1_DLY1_MASK)

/**
 * @}
 */ /* end of group PDU_Register_Masks */

/**
 * @}
 */ /* end of group PDU_Peripheral_Access_Layer */
// clang-format on

#endif // __PDU_REGISTER_H__
