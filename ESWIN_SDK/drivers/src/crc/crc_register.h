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
 * @file crc_register.h
 * @brief CRC driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __CRC_REGISTER_H__
#define __CRC_REGISTER_H__

#include <osal.h>
#include <stdint.h>

// clang-format off

/* ----------------------------------------------------------------------------
   -- CRC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup CRC_Peripheral_Access_Layer CRC Peripheral Access Layer
 * @{
 */

/**
 * @brief CRC Register Layout Typedef
 */
typedef struct {
  union {                                           /**< offset: 0x0 */
    __IO uint32_t DATA;                             /**< CRC Data register, offset: 0x0 */
    struct {                                        /**< offset: 0x0 */
      __IO uint16_t L;                              /**< CRC_DATAL register., offset: 0x0 */
      __IO uint16_t H;                              /**< CRC_DATAH register., offset: 0x2 */
    } DATA_16;
    struct {                                        /**< offset: 0x0 */
      __IO uint8_t LL;                              /**< CRC_DATALL register., offset: 0x0 */
      __IO uint8_t LU;                              /**< CRC_DATALU register., offset: 0x1 */
      __IO uint8_t HL;                              /**< CRC_DATAHL register., offset: 0x2 */
      __IO uint8_t HU;                              /**< CRC_DATAHU register., offset: 0x3 */
    } DATA_8;
  } DATAu;
  __IO uint32_t GPOLY;                              /**< CRC Polynomial register, offset: 0x4 */
  __IO uint32_t CTRL;                               /**< CRC Control register, offset: 0x8 */
} crc_type_t, *crc_mem_map_t_ptr_t;

/* ----------------------------------------------------------------------------
   -- CRC Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup CRC_Register_Masks CRC Register Masks
 * @{
 */

/** DATAu_DATA Bit Fields */
#define CRC_DATAu_DATA_LL_MASK                   0xFFU
#define CRC_DATAu_DATA_LL_SHIFT                  0U
#define CRC_DATAu_DATA_LL_WIDTH                  8U
#define CRC_DATAu_DATA_LL(x)                     (((uint32_t)(((uint32_t)(x))<<CRC_DATAu_DATA_LL_SHIFT))&CRC_DATAu_DATA_LL_MASK)
#define CRC_DATAu_DATA_LU_MASK                   0xFF00U
#define CRC_DATAu_DATA_LU_SHIFT                  8U
#define CRC_DATAu_DATA_LU_WIDTH                  8U
#define CRC_DATAu_DATA_LU(x)                     (((uint32_t)(((uint32_t)(x))<<CRC_DATAu_DATA_LU_SHIFT))&CRC_DATAu_DATA_LU_MASK)
#define CRC_DATAu_DATA_HL_MASK                   0xFF0000U
#define CRC_DATAu_DATA_HL_SHIFT                  16U
#define CRC_DATAu_DATA_HL_WIDTH                  8U
#define CRC_DATAu_DATA_HL(x)                     (((uint32_t)(((uint32_t)(x))<<CRC_DATAu_DATA_HL_SHIFT))&CRC_DATAu_DATA_HL_MASK)
#define CRC_DATAu_DATA_HU_MASK                   0xFF000000U
#define CRC_DATAu_DATA_HU_SHIFT                  24U
#define CRC_DATAu_DATA_HU_WIDTH                  8U
#define CRC_DATAu_DATA_HU(x)                     (((uint32_t)(((uint32_t)(x))<<CRC_DATAu_DATA_HU_SHIFT))&CRC_DATAu_DATA_HU_MASK)
/** DATAu_DATA_16_L Bit Fields */
#define CRC_DATAu_DATA_16_L_DATAL_MASK           0xFFFFU
#define CRC_DATAu_DATA_16_L_DATAL_SHIFT          0U
#define CRC_DATAu_DATA_16_L_DATAL_WIDTH          16U
#define CRC_DATAu_DATA_16_L_DATAL(x)             (((uint16_t)(((uint16_t)(x))<<CRC_DATAu_DATA_16_L_DATAL_SHIFT))&CRC_DATAu_DATA_16_L_DATAL_MASK)
/** DATAu_DATA_16_H Bit Fields */
#define CRC_DATAu_DATA_16_H_DATAH_MASK           0xFFFFU
#define CRC_DATAu_DATA_16_H_DATAH_SHIFT          0U
#define CRC_DATAu_DATA_16_H_DATAH_WIDTH          16U
#define CRC_DATAu_DATA_16_H_DATAH(x)             (((uint16_t)(((uint16_t)(x))<<CRC_DATAu_DATA_16_H_DATAH_SHIFT))&CRC_DATAu_DATA_16_H_DATAH_MASK)
/** DATAu_DATA_8_LL Bit Fields */
#define CRC_DATAu_DATA_8_LL_DATALL_MASK          0xFFU
#define CRC_DATAu_DATA_8_LL_DATALL_SHIFT         0U
#define CRC_DATAu_DATA_8_LL_DATALL_WIDTH         8U
#define CRC_DATAu_DATA_8_LL_DATALL(x)            (((uint8_t)(((uint8_t)(x))<<CRC_DATAu_DATA_8_LL_DATALL_SHIFT))&CRC_DATAu_DATA_8_LL_DATALL_MASK)
/** DATAu_DATA_8_LU Bit Fields */
#define CRC_DATAu_DATA_8_LU_DATALU_MASK          0xFFU
#define CRC_DATAu_DATA_8_LU_DATALU_SHIFT         0U
#define CRC_DATAu_DATA_8_LU_DATALU_WIDTH         8U
#define CRC_DATAu_DATA_8_LU_DATALU(x)            (((uint8_t)(((uint8_t)(x))<<CRC_DATAu_DATA_8_LU_DATALU_SHIFT))&CRC_DATAu_DATA_8_LU_DATALU_MASK)
/** DATAu_DATA_8_HL Bit Fields */
#define CRC_DATAu_DATA_8_HL_DATAHL_MASK          0xFFU
#define CRC_DATAu_DATA_8_HL_DATAHL_SHIFT         0U
#define CRC_DATAu_DATA_8_HL_DATAHL_WIDTH         8U
#define CRC_DATAu_DATA_8_HL_DATAHL(x)            (((uint8_t)(((uint8_t)(x))<<CRC_DATAu_DATA_8_HL_DATAHL_SHIFT))&CRC_DATAu_DATA_8_HL_DATAHL_MASK)
/** DATAu_DATA_8_HU Bit Fields */
#define CRC_DATAu_DATA_8_HU_DATAHU_MASK          0xFFU
#define CRC_DATAu_DATA_8_HU_DATAHU_SHIFT         0U
#define CRC_DATAu_DATA_8_HU_DATAHU_WIDTH         8U
#define CRC_DATAu_DATA_8_HU_DATAHU(x)            (((uint8_t)(((uint8_t)(x))<<CRC_DATAu_DATA_8_HU_DATAHU_SHIFT))&CRC_DATAu_DATA_8_HU_DATAHU_MASK)
/** GPOLY Bit Fields */
#define CRC_GPOLY_LOW_MASK                       0xFFFFU
#define CRC_GPOLY_LOW_SHIFT                      0U
#define CRC_GPOLY_LOW_WIDTH                      16U
#define CRC_GPOLY_LOW(x)                         (((uint32_t)(((uint32_t)(x))<<CRC_GPOLY_LOW_SHIFT))&CRC_GPOLY_LOW_MASK)
#define CRC_GPOLY_HIGH_MASK                      0xFFFF0000U
#define CRC_GPOLY_HIGH_SHIFT                     16U
#define CRC_GPOLY_HIGH_WIDTH                     16U
#define CRC_GPOLY_HIGH(x)                        (((uint32_t)(((uint32_t)(x))<<CRC_GPOLY_HIGH_SHIFT))&CRC_GPOLY_HIGH_MASK)
/** CTRL Bit Fields */
#define CRC_CTRL_TCRC_MASK                       0x1000000U
#define CRC_CTRL_TCRC_SHIFT                      24U
#define CRC_CTRL_TCRC_WIDTH                      1U
#define CRC_CTRL_TCRC(x)                         (((uint32_t)(((uint32_t)(x))<<CRC_CTRL_TCRC_SHIFT))&CRC_CTRL_TCRC_MASK)
#define CRC_CTRL_WAS_MASK                        0x2000000U
#define CRC_CTRL_WAS_SHIFT                       25U
#define CRC_CTRL_WAS_WIDTH                       1U
#define CRC_CTRL_WAS(x)                          (((uint32_t)(((uint32_t)(x))<<CRC_CTRL_WAS_SHIFT))&CRC_CTRL_WAS_MASK)
#define CRC_CTRL_FXOR_MASK                       0x4000000U
#define CRC_CTRL_FXOR_SHIFT                      26U
#define CRC_CTRL_FXOR_WIDTH                      1U
#define CRC_CTRL_FXOR(x)                         (((uint32_t)(((uint32_t)(x))<<CRC_CTRL_FXOR_SHIFT))&CRC_CTRL_FXOR_MASK)
#define CRC_CTRL_TOTR_MASK                       0x30000000U
#define CRC_CTRL_TOTR_SHIFT                      28U
#define CRC_CTRL_TOTR_WIDTH                      2U
#define CRC_CTRL_TOTR(x)                         (((uint32_t)(((uint32_t)(x))<<CRC_CTRL_TOTR_SHIFT))&CRC_CTRL_TOTR_MASK)
#define CRC_CTRL_TOT_MASK                        0xC0000000U
#define CRC_CTRL_TOT_SHIFT                       30U
#define CRC_CTRL_TOT_WIDTH                       2U
#define CRC_CTRL_TOT(x)                          (((uint32_t)(((uint32_t)(x))<<CRC_CTRL_TOT_SHIFT))&CRC_CTRL_TOT_MASK)

/**
 * @}
 */ /* end of group CRC_Register_Masks */

/**
 * @}
 */ /* end of group CRC_Peripheral_Access_Layer */

// clang-format on

#endif /* __CRC_REGISTER_H__ */
