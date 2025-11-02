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
 * @file flexcan_register.h
 * @brief FLEXCAN driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __FLEXCAN_REGISTER_H__
#define __FLEXCAN_REGISTER_H__

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
   -- FLEXCAN Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup FLEXCAN_Peripheral_Access_Layer FLEXCAN Peripheral Access Layer
 * @{
 */

/** CAN - Size of Registers Arrays */
#define CAN_RAMn_COUNT                           (128*4U)
#define CAN_RXIMR_COUNT                          (32*4U)
#define CAN_WMB_COUNT                            (4U)
/** RXFIR RXMASK SMBS */
#define CAN_0_RES_COUNT                          (24U)
#define CAN_0_HR_TIME_STAMPn_COUNT               (128U)
#define CAN_0_ERFFELn_COUNT                      (32U)

/**
 * @brief CAN Register Layout Typedef
 */
typedef struct {
  __IO uint32_t MCR;                               /**< Module Configuration Register, offset: 0x0 */
  __IO uint32_t CTRL1;                             /**< Control 1 register, offset: 0x4 */
  __IO uint32_t TIMER;                             /**< Free Running Timer, offset: 0x8 */
       uint8_t RESERVED_0[4];
  __IO uint32_t RXMGMASK;                          /**< Rx Mailboxes Global Mask Register, offset: 0x10 */
  __IO uint32_t RX14MASK;                          /**< Rx 14 Mask register, offset: 0x14 */
  __IO uint32_t RX15MASK;                          /**< Rx 15 Mask register, offset: 0x18 */
  __IO uint32_t ECR;                               /**< Error Counter, offset: 0x1C */
  __IO uint32_t ESR1;                              /**< Error and Status 1 register, offset: 0x20 */
  __IO uint32_t IMASK2;                            /**< Interrupt Masks 2 register, offset: 0x24 */
  __IO uint32_t IMASK1;                            /**< Interrupt Masks 1 register, offset: 0x28 */
  __IO uint32_t IFLAG2;                            /**< Interrupt Flags 2 register, offset: 0x2c */
  __IO uint32_t IFLAG1;                            /**< Interrupt Flags 1 register, offset: 0x30 */
  __IO uint32_t CTRL2;                             /**< Control 2 register, offset: 0x34 */
  __I  uint32_t ESR2;                              /**< Error and Status 2 register, offset: 0x38 */
       uint8_t RESERVED_1[8];
  __I  uint32_t CRCR;                              /**< CRC Register, offset: 0x44 */
  __IO uint32_t RXFGMASK;                          /**< Rx FIFO Global Mask register, offset: 0x48 */
  __I  uint32_t RXFIR;                             /**< Rx FIFO Information Register, offset: 0x4C */
  __IO uint32_t CBT;                               /**< CAN Bit Timing Register, offset: 0x50 */
       uint8_t RESERVED_2[20];
  __IO uint32_t IMASK4;                            /**< CAN Bit Timing Register, offset: 0x68 */
  __IO uint32_t IMASK3;                            /**< CAN Bit Timing Register, offset: 0x6c */
  __IO uint32_t IFLAG4;                            /**< CAN Bit Timing Register, offset: 0x70 */
  __IO uint32_t IFLAG3;                            /**< CAN Bit Timing Register, offset: 74 */
       uint8_t RESERVED_3[8];

  __IO uint32_t RAMn[CAN_RAMn_COUNT];              /**< Embedded RAM, array offset: 0x80, array step: 0x4 */
  __IO uint32_t RXIMR[CAN_RXIMR_COUNT];            /**< Rx Individual Mask Registers, array offset: 0x880, array step: 0x4 */

  __IO uint32_t RXFIR_RAM[6];                      /**< Legacy Rx Fifo information, array offset: 0xA80-0xA94 */
  __IO uint32_t RESERVED_RAM[2];                   /**< Reserved, array offset: 0xA98-0xA9C */
  __IO uint32_t RXMASK_RAM[4];                     /**< Rx Mailbox global mask, array offset: 0xAA0-0xAAC */
  __IO uint32_t SMB_RAM[12];                       /**< SMB(serial message buffers) ram, array offset: 0xAB0-0xADC */

  __IO uint32_t MECR;                              /**< Memory Error Control Register, offset: 0xAE0 */
  __IO uint32_t ERRIAR;                            /**< Error Injection Address Register, offset: 0xAE4 */
  __IO uint32_t ERRIDPR;                           /**< Error Injection Data Pattern Register, offset: 0xAE8 */
  __IO uint32_t ERRIPPR;                           /**< Error Injection Parity Pattern Register, offset: 0xAEC */
  __IO uint32_t RERRAR;                            /**< Error Report Address Register, offset: 0xAF0 */
  __IO uint32_t RERRDR;                            /**< Error Report Data Register, offset: 0xAF4 */
  __IO uint32_t RERRSYNR;                          /**< Error Report Syndrome Register, offset: 0xAF8 */
  __IO uint32_t ERRSR;                             /**< Error Status Register, offset: 0xAFC */

  __IO uint32_t CTRL1_PN;                          /**< Pretended Networking Control 1 Register, offset: 0xB00 */
  __IO uint32_t CTRL2_PN;                          /**< Pretended Networking Control 2 Register, offset: 0xB04 */
  __IO uint32_t WU_MTC;                            /**< Pretended Networking Wake Up Match Register, offset: 0xB08 */
  __IO uint32_t FLT_ID1;                           /**< Pretended Networking ID Filter 1 Register, offset: 0xB0C */
  __IO uint32_t FLT_DLC;                           /**< Pretended Networking DLC Filter Register, offset: 0xB10 */
  __IO uint32_t PL1_LO;                            /**< Pretended Networking Payload Low Filter 1 Register, offset: 0xB14 */
  __IO uint32_t PL1_HI;                            /**< Pretended Networking Payload High Filter 1 Register, offset: 0xB18 */
  __IO uint32_t FLT_ID2_IDMASK;                    /**< Pretended Networking ID Filter 2 Register / ID Mask Register, offset: 0xB1C */
  __IO uint32_t PL2_PLMASK_LO;                     /**< Pretended Networking Payload Low Filter 2 Register / Payload Low Mask Register, offset: 0xB20 */
  __IO uint32_t PL2_PLMASK_HI;                     /**< Pretended Networking Payload High Filter 2 low order bits / Payload High Mask Register, offset: 0xB24 */
       uint8_t RESERVED_4[24];
  struct {                                         /**< offset: 0xB40, array step: 0x10 */
    __I  uint32_t WMBn_CS;                         /**< Wake Up Message Buffer Register for C/S, array offset: 0xB40, array step: 0x10 */
    __I  uint32_t WMBn_ID;                         /**< Wake Up Message Buffer Register for ID, array offset: 0xB44, array step: 0x10 */
    __I  uint32_t WMBn_D03;                        /**< Wake Up Message Buffer Register for Data 0-3, array offset: 0xB48, array step: 0x10 */
    __I  uint32_t WMBn_D47;                        /**< Wake Up Message Buffer Register Data 4-7, array offset: 0xB4C, array step: 0x10 */
  } WMB[CAN_WMB_COUNT];
         uint8_t RESERVED_5[112];

  __IO uint32_t EPRS;                              /**< Pretended Networking Payload High Filter 2 low order bits / Payload High Mask Register, offset: 0xB24 */
  __IO uint32_t ENCBT;                             /**< Pretended Networking Payload High Filter 2 low order bits / Payload High Mask Register, offset: 0xB24 */
  __IO uint32_t EDCBT;                             /**< Pretended Networking Payload High Filter 2 low order bits / Payload High Mask Register, offset: 0xB24 */
  __IO uint32_t ETDC;                              /**< Pretended Networking Payload High Filter 2 low order bits / Payload High Mask Register, offset: 0xB24 */

  __IO uint32_t FDCTRL;                            /**< CAN FD Control Register, offset: 0xC00 */
  __IO uint32_t FDCBT;                             /**< CAN FD Bit Timing Register, offset: 0xC04 */
  __I  uint32_t FDCRC;                             /**< CAN FD CRC Register, offset: 0xC08 */

  __IO uint32_t ERFCR;                             /**< CAN FD Enhanced Rx Fifo Control Register, offset: 0xC0C */
  __IO uint32_t ERFIER;                            /**< CAN FD Enhanced Rx Fifo Interrupt Enable Register, offset: 0xC10 */
  __IO uint32_t ERFSR;                             /**< CAN FD Enhanced Rx Fifo Status Register, offset: 0xC14 */
  __IO uint32_t RESERVED_6_0;                      /**< Reserved Register, offset: 0xC18 */
  __IO uint32_t RESERVED_6_1;                      /**< Reserved Register, offset: 0xC1C */
  __IO uint32_t RSMB_TIME_STAMP[2];                /**< Reserved Register, offset: 0xC20 */
  __IO uint32_t RESERVED_6_2;                      /**< Reserved Register, offset: 0xC28 */
  __IO uint32_t RESERVED_6_3;                      /**< Reserved Register, offset: 0xC2C */
  __IO uint32_t HR_TIME_STAMPn[CAN_0_HR_TIME_STAMPn_COUNT];  /**< HR timer stamp, offset: 0xC30-0xE2C */
  __IO uint32_t RESERVED_7[62];                              /**< Reserved, offset: 0xE30-0xFFC */
  __IO uint32_t FDRMB[54];                                   /**< CAN FD SMB, offset: 0xF28-0xFFC */
  __IO uint32_t EMBn[CAN_RAMn_COUNT];                        /**< Enhanced Message Buffer, offset: 0x1000-0x17fc */
  __IO uint32_t RESERVED_8[512];                             /**< offset: 0x1800-0x1fffc */
  __IO uint32_t ERX_FIFOn[CAN_0_ERFFELn_COUNT*20];           /**< Enhance RX FIFO , offset: 0x2000-0x2A00 */
  __IO uint32_t RESERVED_9[384];
  __IO uint32_t ERFFELn[CAN_0_ERFFELn_COUNT];                /**< Enhance RX FIFO FILTER , offset: 0x3000 */

} can_type_t;

/**
 * @brief CAN instance Layout Typedef
 */
typedef struct
{
  __IO uint32_t instance;                                    /**< CAN instance index */
  can_type_t* CANBasePtr;                                    /**< CAN base address pointer */
} can_instance_t;

/** Number of instances of the CAN module. */
#if (defined(PLATFORM_EAM2011))
#define CAN_INSTANCE_COUNT                       (4U)
#endif

/** CAN - Peripheral instance base addresses */
/** Peripheral CAN0 base address */
#define CAN0_BASE                                (FLEXCAN_0_BASEADDR)
/** Peripheral CAN0 base pointer */
#define CAN0                                     ((can_type_t *)CAN0_BASE)
/** Peripheral CAN1 base address */
#define CAN1_BASE                                (FLEXCAN_1_BASEADDR)
/** Peripheral CAN1 base pointer */
#define CAN1                                     ((can_type_t *)CAN1_BASE)
/** Peripheral CAN2 base address */
#define CAN2_BASE                                (FLEXCAN_2_BASEADDR)
/** Peripheral CAN2 base pointer */
#define CAN2                                     ((can_type_t *)CAN2_BASE)

#if (defined(PLATFORM_EAM2011))
#define CAN3_BASE                                (FLEXCAN_3_BASEADDR)
/** Peripheral CAN2 base pointer */
#define CAN3                                     ((can_type_t *)CAN3_BASE)

#endif

/** Array initializer of CAN peripheral base addresses */
#if (defined(PLATFORM_EAM2011))
#define CAN_BASE_ADDRS                           { CAN0_BASE, CAN1_BASE, CAN2_BASE, CAN3_BASE }
#endif


/** Array initializer of CAN peripheral base pointers */
#if (defined(PLATFORM_EAM2011))
#define CAN_BASE_PTRS                            { CAN0, CAN1, CAN2, CAN3 }
#endif

/** Number of interrupt vector arrays for the CAN module. */
#define CAN_IRQS_ARR_COUNT                       (7U)
/** Number of interrupt channels for the Rx_Warning type of CAN module. */
#define CAN_Rx_Warning_IRQS_CH_COUNT             (1U)
/** Number of interrupt channels for the Tx_Warning type of CAN module. */
#define CAN_Tx_Warning_IRQS_CH_COUNT             (1U)
/** Number of interrupt channels for the Wake_Up type of CAN module. */
#define CAN_Wake_Up_IRQS_CH_COUNT                (1U)
/** Number of interrupt channels for the Error type of CAN module. */
#define CAN_Error_IRQS_CH_COUNT                  (1U)
/** Number of interrupt channels for the Bus_Off type of CAN module. */
#define CAN_Bus_Off_IRQS_CH_COUNT                (1U)
/** Number of interrupt channels for the ORed_0_15_MB type of CAN module. */
#define CAN_ORed_0_15_MB_IRQS_CH_COUNT           (1U)
/** Number of interrupt channels for the ORed_16_31_MB type of CAN module. */
#define CAN_ORed_16_31_MB_IRQS_CH_COUNT          (1u)

/** Interrupt vectors for the CAN peripheral type */
#if (defined(PLATFORM_EAM2011))
#define CAN_Rx_Warning_IRQS                      { CAN0_Rx_Warning_IRQn, CAN1_Rx_Warning_IRQn, CAN2_Rx_Warning_IRQn, CAN3_Rx_Warning_IRQn }
#endif

#if (defined(PLATFORM_EAM2011))
#define CAN_Tx_Warning_IRQS                      { CAN0_Tx_Warning_IRQn, CAN1_Tx_Warning_IRQn, CAN2_Tx_Warning_IRQn, CAN3_Tx_Warning_IRQn }

#endif

#if (defined(PLATFORM_EAM2011))
#define CAN_WakeUp_Match_IRQS                         { CAN0_Wake_Up_Match_IRQn, CAN1_Wake_Up_Match_IRQn, CAN2_Wake_Up_Match_IRQn, CAN3_Wake_Up_Match_IRQn }
#define CAN_WakeUp_TimeOut_IRQS                         { CAN0_Wake_Up_To_IRQn, CAN1_Wake_Up_To_IRQn, CAN2_Wake_Up_To_IRQn, CAN3_Wake_Up_To_IRQn }
#endif

#if (defined(PLATFORM_EAM2011))
#define CAN_Error_IRQS                           { CAN0_Error_IRQn, CAN1_Error_IRQn, CAN2_Error_IRQn, CAN3_Error_IRQn }
#endif

#if (defined(PLATFORM_EAM2011))
#define CAN_Bus_Off_IRQS                         { CAN0_Busoff_IRQn, CAN1_Busoff_IRQn, CAN2_Busoff_IRQn, CAN3_Busoff_IRQn }
#endif

#if (defined(PLATFORM_EAM2011))
#define CAN_ORed_0_127_MB_IRQS                   { CAN0_ORed_0_127_MB_IRQn, CAN1_ORed_0_16_MB_IRQn, CAN2_ORed_0_128_MB_IRQn, CAN3_ORed_0_128_MB_IRQn }
#endif

#if (defined(PLATFORM_EAM2011))
#define CAN_ENHANCED_RX_FIFO_IRQS                {CAN0_Erfwm_IRQn , CAN1_Erfwm_IRQn , CAN2_Erfwm_IRQn, CAN3_Erfwm_IRQn }

#endif

#if (defined(PLATFORM_EAM2011))
#define FLEXCAN_BASE_PTRS_HAS_ENHANCED_RX_FIFO  { CAN0, CAN2, CAN3 }
#endif

#if (defined(PLATFORM_EAM2011))
#define FLEXCAN_BASE_PTRS_HAS_EXPANDABLE_MEMORY { CAN0, CAN2, CAN3 }
#endif

/* ----------------------------------------------------------------------------
   -- FLEXCAN Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup FLEXCAN_Register_Masks FLEXCAN Register Masks
 * @{
 */

/** MCR Bit Fields */
#define CAN_MCR_MAXMB_MASK                       0x7FU
#define CAN_MCR_MAXMB_SHIFT                      0U
#define CAN_MCR_MAXMB_WIDTH                      7U
#define CAN_MCR_MAXMB(x)                         (((uint32_t)(((uint32_t)(x))<<CAN_MCR_MAXMB_SHIFT))&CAN_MCR_MAXMB_MASK)
#define CAN_MCR_IDAM_MASK                        0x300U
#define CAN_MCR_IDAM_SHIFT                       8U
#define CAN_MCR_IDAM_WIDTH                       2U
#define CAN_MCR_IDAM(x)                          (((uint32_t)(((uint32_t)(x))<<CAN_MCR_IDAM_SHIFT))&CAN_MCR_IDAM_MASK)
#define CAN_MCR_FDEN_MASK                        0x800U
#define CAN_MCR_FDEN_SHIFT                       11U
#define CAN_MCR_FDEN_WIDTH                       1U
#define CAN_MCR_FDEN(x)                          (((uint32_t)(((uint32_t)(x))<<CAN_MCR_FDEN_SHIFT))&CAN_MCR_FDEN_MASK)
#define CAN_MCR_AEN_MASK                         0x1000U
#define CAN_MCR_AEN_SHIFT                        12U
#define CAN_MCR_AEN_WIDTH                        1U
#define CAN_MCR_AEN(x)                           (((uint32_t)(((uint32_t)(x))<<CAN_MCR_AEN_SHIFT))&CAN_MCR_AEN_MASK)
#define CAN_MCR_LPRIOEN_MASK                     0x2000U
#define CAN_MCR_LPRIOEN_SHIFT                    13U
#define CAN_MCR_LPRIOEN_WIDTH                    1U
#define CAN_MCR_LPRIOEN(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_MCR_LPRIOEN_SHIFT))&CAN_MCR_LPRIOEN_MASK)
#define CAN_MCR_PNET_EN_MASK                     0x4000U
#define CAN_MCR_PNET_EN_SHIFT                    14U
#define CAN_MCR_PNET_EN_WIDTH                    1U
#define CAN_MCR_PNET_EN(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_MCR_PNET_EN_SHIFT))&CAN_MCR_PNET_EN_MASK)
#define CAN_MCR_DMA_MASK                         0x8000U
#define CAN_MCR_DMA_SHIFT                        15U
#define CAN_MCR_DMA_WIDTH                        1U
#define CAN_MCR_DMA(x)                           (((uint32_t)(((uint32_t)(x))<<CAN_MCR_DMA_SHIFT))&CAN_MCR_DMA_MASK)
#define CAN_MCR_IRMQ_MASK                        0x10000U
#define CAN_MCR_IRMQ_SHIFT                       16U
#define CAN_MCR_IRMQ_WIDTH                       1U
#define CAN_MCR_IRMQ(x)                          (((uint32_t)(((uint32_t)(x))<<CAN_MCR_IRMQ_SHIFT))&CAN_MCR_IRMQ_MASK)
#define CAN_MCR_SRXDIS_MASK                      0x20000U
#define CAN_MCR_SRXDIS_SHIFT                     17U
#define CAN_MCR_SRXDIS_WIDTH                     1U
#define CAN_MCR_SRXDIS(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_MCR_SRXDIS_SHIFT))&CAN_MCR_SRXDIS_MASK)
#define CAN_MCR_LPMACK_MASK                      0x100000U
#define CAN_MCR_LPMACK_SHIFT                     20U
#define CAN_MCR_LPMACK_WIDTH                     1U
#define CAN_MCR_LPMACK(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_MCR_LPMACK_SHIFT))&CAN_MCR_LPMACK_MASK)
#define CAN_MCR_WRNEN_MASK                       0x200000U
#define CAN_MCR_WRNEN_SHIFT                      21U
#define CAN_MCR_WRNEN_WIDTH                      1U
#define CAN_MCR_WRNEN(x)                         (((uint32_t)(((uint32_t)(x))<<CAN_MCR_WRNEN_SHIFT))&CAN_MCR_WRNEN_MASK)
#define CAN_MCR_SUPV_MASK                        0x800000U
#define CAN_MCR_SUPV_SHIFT                       23U
#define CAN_MCR_SUPV_WIDTH                       1U
#define CAN_MCR_SUPV(x)                          (((uint32_t)(((uint32_t)(x))<<CAN_MCR_SUPV_SHIFT))&CAN_MCR_SUPV_MASK)
#define CAN_MCR_FRZACK_MASK                      0x1000000U
#define CAN_MCR_FRZACK_SHIFT                     24U
#define CAN_MCR_FRZACK_WIDTH                     1U
#define CAN_MCR_FRZACK(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_MCR_FRZACK_SHIFT))&CAN_MCR_FRZACK_MASK)
#define CAN_MCR_SOFTRST_MASK                     0x2000000U
#define CAN_MCR_SOFTRST_SHIFT                    25U
#define CAN_MCR_SOFTRST_WIDTH                    1U
#define CAN_MCR_SOFTRST(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_MCR_SOFTRST_SHIFT))&CAN_MCR_SOFTRST_MASK)
#define CAN_MCR_NOTRDY_MASK                      0x8000000U
#define CAN_MCR_NOTRDY_SHIFT                     27U
#define CAN_MCR_NOTRDY_WIDTH                     1U
#define CAN_MCR_NOTRDY(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_MCR_NOTRDY_SHIFT))&CAN_MCR_NOTRDY_MASK)
#define CAN_MCR_HALT_MASK                        0x10000000U
#define CAN_MCR_HALT_SHIFT                       28U
#define CAN_MCR_HALT_WIDTH                       1U
#define CAN_MCR_HALT(x)                          (((uint32_t)(((uint32_t)(x))<<CAN_MCR_HALT_SHIFT))&CAN_MCR_HALT_MASK)
#define CAN_MCR_RFEN_MASK                        0x20000000U
#define CAN_MCR_RFEN_SHIFT                       29U
#define CAN_MCR_RFEN_WIDTH                       1U
#define CAN_MCR_RFEN(x)                          (((uint32_t)(((uint32_t)(x))<<CAN_MCR_RFEN_SHIFT))&CAN_MCR_RFEN_MASK)
#define CAN_MCR_FRZ_MASK                         0x40000000U
#define CAN_MCR_FRZ_SHIFT                        30U
#define CAN_MCR_FRZ_WIDTH                        1U
#define CAN_MCR_FRZ(x)                           (((uint32_t)(((uint32_t)(x))<<CAN_MCR_FRZ_SHIFT))&CAN_MCR_FRZ_MASK)
#define CAN_MCR_MDIS_MASK                        0x80000000U
#define CAN_MCR_MDIS_SHIFT                       31U
#define CAN_MCR_MDIS_WIDTH                       1U
#define CAN_MCR_MDIS(x)                          (((uint32_t)(((uint32_t)(x))<<CAN_MCR_MDIS_SHIFT))&CAN_MCR_MDIS_MASK)
/** CTRL1 Bit Fields */
#define CAN_CTRL1_PROPSEG_MASK                   0x7U
#define CAN_CTRL1_PROPSEG_SHIFT                  0U
#define CAN_CTRL1_PROPSEG_WIDTH                  3U
#define CAN_CTRL1_PROPSEG(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_PROPSEG_SHIFT))&CAN_CTRL1_PROPSEG_MASK)
#define CAN_CTRL1_LOM_MASK                       0x8U
#define CAN_CTRL1_LOM_SHIFT                      3U
#define CAN_CTRL1_LOM_WIDTH                      1U
#define CAN_CTRL1_LOM(x)                         (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_LOM_SHIFT))&CAN_CTRL1_LOM_MASK)
#define CAN_CTRL1_LBUF_MASK                      0x10U
#define CAN_CTRL1_LBUF_SHIFT                     4U
#define CAN_CTRL1_LBUF_WIDTH                     1U
#define CAN_CTRL1_LBUF(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_LBUF_SHIFT))&CAN_CTRL1_LBUF_MASK)
#define CAN_CTRL1_TSYN_MASK                      0x20U
#define CAN_CTRL1_TSYN_SHIFT                     5U
#define CAN_CTRL1_TSYN_WIDTH                     1U
#define CAN_CTRL1_TSYN(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_TSYN_SHIFT))&CAN_CTRL1_TSYN_MASK)
#define CAN_CTRL1_BOFFREC_MASK                   0x40U
#define CAN_CTRL1_BOFFREC_SHIFT                  6U
#define CAN_CTRL1_BOFFREC_WIDTH                  1U
#define CAN_CTRL1_BOFFREC(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_BOFFREC_SHIFT))&CAN_CTRL1_BOFFREC_MASK)
#define CAN_CTRL1_SMP_MASK                       0x80U
#define CAN_CTRL1_SMP_SHIFT                      7U
#define CAN_CTRL1_SMP_WIDTH                      1U
#define CAN_CTRL1_SMP(x)                         (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_SMP_SHIFT))&CAN_CTRL1_SMP_MASK)
#define CAN_CTRL1_RWRNMSK_MASK                   0x400U
#define CAN_CTRL1_RWRNMSK_SHIFT                  10U
#define CAN_CTRL1_RWRNMSK_WIDTH                  1U
#define CAN_CTRL1_RWRNMSK(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_RWRNMSK_SHIFT))&CAN_CTRL1_RWRNMSK_MASK)
#define CAN_CTRL1_TWRNMSK_MASK                   0x800U
#define CAN_CTRL1_TWRNMSK_SHIFT                  11U
#define CAN_CTRL1_TWRNMSK_WIDTH                  1U
#define CAN_CTRL1_TWRNMSK(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_TWRNMSK_SHIFT))&CAN_CTRL1_TWRNMSK_MASK)
#define CAN_CTRL1_LPB_MASK                       0x1000U
#define CAN_CTRL1_LPB_SHIFT                      12U
#define CAN_CTRL1_LPB_WIDTH                      1U
#define CAN_CTRL1_LPB(x)                         (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_LPB_SHIFT))&CAN_CTRL1_LPB_MASK)
#define CAN_CTRL1_CLKSRC_MASK                    0x2000U
#define CAN_CTRL1_CLKSRC_SHIFT                   13U
#define CAN_CTRL1_CLKSRC_WIDTH                   1U
#define CAN_CTRL1_CLKSRC(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_CLKSRC_SHIFT))&CAN_CTRL1_CLKSRC_MASK)
#define CAN_CTRL1_ERRMSK_MASK                    0x4000U
#define CAN_CTRL1_ERRMSK_SHIFT                   14U
#define CAN_CTRL1_ERRMSK_WIDTH                   1U
#define CAN_CTRL1_ERRMSK(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_ERRMSK_SHIFT))&CAN_CTRL1_ERRMSK_MASK)
#define CAN_CTRL1_BOFFMSK_MASK                   0x8000U
#define CAN_CTRL1_BOFFMSK_SHIFT                  15U
#define CAN_CTRL1_BOFFMSK_WIDTH                  1U
#define CAN_CTRL1_BOFFMSK(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_BOFFMSK_SHIFT))&CAN_CTRL1_BOFFMSK_MASK)
#define CAN_CTRL1_PSEG2_MASK                     0x70000U
#define CAN_CTRL1_PSEG2_SHIFT                    16U
#define CAN_CTRL1_PSEG2_WIDTH                    3U
#define CAN_CTRL1_PSEG2(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_PSEG2_SHIFT))&CAN_CTRL1_PSEG2_MASK)
#define CAN_CTRL1_PSEG1_MASK                     0x380000U
#define CAN_CTRL1_PSEG1_SHIFT                    19U
#define CAN_CTRL1_PSEG1_WIDTH                    3U
#define CAN_CTRL1_PSEG1(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_PSEG1_SHIFT))&CAN_CTRL1_PSEG1_MASK)
#define CAN_CTRL1_RJW_MASK                       0xC00000U
#define CAN_CTRL1_RJW_SHIFT                      22U
#define CAN_CTRL1_RJW_WIDTH                      2U
#define CAN_CTRL1_RJW(x)                         (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_RJW_SHIFT))&CAN_CTRL1_RJW_MASK)
#define CAN_CTRL1_PRESDIV_MASK                   0xFF000000U
#define CAN_CTRL1_PRESDIV_SHIFT                  24U
#define CAN_CTRL1_PRESDIV_WIDTH                  8U
#define CAN_CTRL1_PRESDIV(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_PRESDIV_SHIFT))&CAN_CTRL1_PRESDIV_MASK)
/** TIMER Bit Fields */
#define CAN_TIMER_TIMER_MASK                     0xFFFFU
#define CAN_TIMER_TIMER_SHIFT                    0U
#define CAN_TIMER_TIMER_WIDTH                    16U
#define CAN_TIMER_TIMER(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_TIMER_TIMER_SHIFT))&CAN_TIMER_TIMER_MASK)
/** RXMGMASK Bit Fields */
#define CAN_RXMGMASK_MG_MASK                     0xFFFFFFFFU
#define CAN_RXMGMASK_MG_SHIFT                    0U
#define CAN_RXMGMASK_MG_WIDTH                    32U
#define CAN_RXMGMASK_MG(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_RXMGMASK_MG_SHIFT))&CAN_RXMGMASK_MG_MASK)
/** RX14MASK Bit Fields */
#define CAN_RX14MASK_RX14M_MASK                  0xFFFFFFFFU
#define CAN_RX14MASK_RX14M_SHIFT                 0U
#define CAN_RX14MASK_RX14M_WIDTH                 32U
#define CAN_RX14MASK_RX14M(x)                    (((uint32_t)(((uint32_t)(x))<<CAN_RX14MASK_RX14M_SHIFT))&CAN_RX14MASK_RX14M_MASK)
/** RX15MASK Bit Fields */
#define CAN_RX15MASK_RX15M_MASK                  0xFFFFFFFFU
#define CAN_RX15MASK_RX15M_SHIFT                 0U
#define CAN_RX15MASK_RX15M_WIDTH                 32U
#define CAN_RX15MASK_RX15M(x)                    (((uint32_t)(((uint32_t)(x))<<CAN_RX15MASK_RX15M_SHIFT))&CAN_RX15MASK_RX15M_MASK)
/** ECR Bit Fields */
#define CAN_ECR_TXERRCNT_MASK                    0xFFU
#define CAN_ECR_TXERRCNT_SHIFT                   0U
#define CAN_ECR_TXERRCNT_WIDTH                   8U
#define CAN_ECR_TXERRCNT(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_ECR_TXERRCNT_SHIFT))&CAN_ECR_TXERRCNT_MASK)
#define CAN_ECR_RXERRCNT_MASK                    0xFF00U
#define CAN_ECR_RXERRCNT_SHIFT                   8U
#define CAN_ECR_RXERRCNT_WIDTH                   8U
#define CAN_ECR_RXERRCNT(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_ECR_RXERRCNT_SHIFT))&CAN_ECR_RXERRCNT_MASK)
#define CAN_ECR_TXERRCNT_FAST_MASK               0xFF0000U
#define CAN_ECR_TXERRCNT_FAST_SHIFT              16U
#define CAN_ECR_TXERRCNT_FAST_WIDTH              8U
#define CAN_ECR_TXERRCNT_FAST(x)                 (((uint32_t)(((uint32_t)(x))<<CAN_ECR_TXERRCNT_FAST_SHIFT))&CAN_ECR_TXERRCNT_FAST_MASK)
#define CAN_ECR_RXERRCNT_FAST_MASK               0xFF000000U
#define CAN_ECR_RXERRCNT_FAST_SHIFT              24U
#define CAN_ECR_RXERRCNT_FAST_WIDTH              8U
#define CAN_ECR_RXERRCNT_FAST(x)                 (((uint32_t)(((uint32_t)(x))<<CAN_ECR_RXERRCNT_FAST_SHIFT))&CAN_ECR_RXERRCNT_FAST_MASK)
/** ESR1 Bit Fields */
#define CAN_ESR1_ERRINT_MASK                     0x2U
#define CAN_ESR1_ERRINT_SHIFT                    1U
#define CAN_ESR1_ERRINT_WIDTH                    1U
#define CAN_ESR1_ERRINT(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_ERRINT_SHIFT))&CAN_ESR1_ERRINT_MASK)
#define CAN_ESR1_BOFFINT_MASK                    0x4U
#define CAN_ESR1_BOFFINT_SHIFT                   2U
#define CAN_ESR1_BOFFINT_WIDTH                   1U
#define CAN_ESR1_BOFFINT(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_BOFFINT_SHIFT))&CAN_ESR1_BOFFINT_MASK)
#define CAN_ESR1_RX_MASK                         0x8U
#define CAN_ESR1_RX_SHIFT                        3U
#define CAN_ESR1_RX_WIDTH                        1U
#define CAN_ESR1_RX(x)                           (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_RX_SHIFT))&CAN_ESR1_RX_MASK)
#define CAN_ESR1_FLTCONF_MASK                    0x30U
#define CAN_ESR1_FLTCONF_SHIFT                   4U
#define CAN_ESR1_FLTCONF_WIDTH                   2U
#define CAN_ESR1_FLTCONF(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_FLTCONF_SHIFT))&CAN_ESR1_FLTCONF_MASK)
#define CAN_ESR1_TX_MASK                         0x40U
#define CAN_ESR1_TX_SHIFT                        6U
#define CAN_ESR1_TX_WIDTH                        1U
#define CAN_ESR1_TX(x)                           (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_TX_SHIFT))&CAN_ESR1_TX_MASK)
#define CAN_ESR1_IDLE_MASK                       0x80U
#define CAN_ESR1_IDLE_SHIFT                      7U
#define CAN_ESR1_IDLE_WIDTH                      1U
#define CAN_ESR1_IDLE(x)                         (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_IDLE_SHIFT))&CAN_ESR1_IDLE_MASK)
#define CAN_ESR1_RXWRN_MASK                      0x100U
#define CAN_ESR1_RXWRN_SHIFT                     8U
#define CAN_ESR1_RXWRN_WIDTH                     1U
#define CAN_ESR1_RXWRN(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_RXWRN_SHIFT))&CAN_ESR1_RXWRN_MASK)
#define CAN_ESR1_TXWRN_MASK                      0x200U
#define CAN_ESR1_TXWRN_SHIFT                     9U
#define CAN_ESR1_TXWRN_WIDTH                     1U
#define CAN_ESR1_TXWRN(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_TXWRN_SHIFT))&CAN_ESR1_TXWRN_MASK)
#define CAN_ESR1_STFERR_MASK                     0x400U
#define CAN_ESR1_STFERR_SHIFT                    10U
#define CAN_ESR1_STFERR_WIDTH                    1U
#define CAN_ESR1_STFERR(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_STFERR_SHIFT))&CAN_ESR1_STFERR_MASK)
#define CAN_ESR1_FRMERR_MASK                     0x800U
#define CAN_ESR1_FRMERR_SHIFT                    11U
#define CAN_ESR1_FRMERR_WIDTH                    1U
#define CAN_ESR1_FRMERR(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_FRMERR_SHIFT))&CAN_ESR1_FRMERR_MASK)
#define CAN_ESR1_CRCERR_MASK                     0x1000U
#define CAN_ESR1_CRCERR_SHIFT                    12U
#define CAN_ESR1_CRCERR_WIDTH                    1U
#define CAN_ESR1_CRCERR(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_CRCERR_SHIFT))&CAN_ESR1_CRCERR_MASK)
#define CAN_ESR1_ACKERR_MASK                     0x2000U
#define CAN_ESR1_ACKERR_SHIFT                    13U
#define CAN_ESR1_ACKERR_WIDTH                    1U
#define CAN_ESR1_ACKERR(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_ACKERR_SHIFT))&CAN_ESR1_ACKERR_MASK)
#define CAN_ESR1_BIT0ERR_MASK                    0x4000U
#define CAN_ESR1_BIT0ERR_SHIFT                   14U
#define CAN_ESR1_BIT0ERR_WIDTH                   1U
#define CAN_ESR1_BIT0ERR(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_BIT0ERR_SHIFT))&CAN_ESR1_BIT0ERR_MASK)
#define CAN_ESR1_BIT1ERR_MASK                    0x8000U
#define CAN_ESR1_BIT1ERR_SHIFT                   15U
#define CAN_ESR1_BIT1ERR_WIDTH                   1U
#define CAN_ESR1_BIT1ERR(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_BIT1ERR_SHIFT))&CAN_ESR1_BIT1ERR_MASK)
#define CAN_ESR1_RWRNINT_MASK                    0x10000U
#define CAN_ESR1_RWRNINT_SHIFT                   16U
#define CAN_ESR1_RWRNINT_WIDTH                   1U
#define CAN_ESR1_RWRNINT(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_RWRNINT_SHIFT))&CAN_ESR1_RWRNINT_MASK)
#define CAN_ESR1_TWRNINT_MASK                    0x20000U
#define CAN_ESR1_TWRNINT_SHIFT                   17U
#define CAN_ESR1_TWRNINT_WIDTH                   1U
#define CAN_ESR1_TWRNINT(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_TWRNINT_SHIFT))&CAN_ESR1_TWRNINT_MASK)
#define CAN_ESR1_SYNCH_MASK                      0x40000U
#define CAN_ESR1_SYNCH_SHIFT                     18U
#define CAN_ESR1_SYNCH_WIDTH                     1U
#define CAN_ESR1_SYNCH(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_SYNCH_SHIFT))&CAN_ESR1_SYNCH_MASK)
#define CAN_ESR1_BOFFDONEINT_MASK                0x80000U
#define CAN_ESR1_BOFFDONEINT_SHIFT               19U
#define CAN_ESR1_BOFFDONEINT_WIDTH               1U
#define CAN_ESR1_BOFFDONEINT(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_BOFFDONEINT_SHIFT))&CAN_ESR1_BOFFDONEINT_MASK)
#define CAN_ESR1_ERRINT_FAST_MASK                0x100000U
#define CAN_ESR1_ERRINT_FAST_SHIFT               20U
#define CAN_ESR1_ERRINT_FAST_WIDTH               1U
#define CAN_ESR1_ERRINT_FAST(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_ERRINT_FAST_SHIFT))&CAN_ESR1_ERRINT_FAST_MASK)
#define CAN_ESR1_ERROVR_MASK                     0x200000U
#define CAN_ESR1_ERROVR_SHIFT                    21U
#define CAN_ESR1_ERROVR_WIDTH                    1U
#define CAN_ESR1_ERROVR(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_ERROVR_SHIFT))&CAN_ESR1_ERROVR_MASK)
#define CAN_ESR1_STFERR_FAST_MASK                0x4000000U
#define CAN_ESR1_STFERR_FAST_SHIFT               26U
#define CAN_ESR1_STFERR_FAST_WIDTH               1U
#define CAN_ESR1_STFERR_FAST(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_STFERR_FAST_SHIFT))&CAN_ESR1_STFERR_FAST_MASK)
#define CAN_ESR1_FRMERR_FAST_MASK                0x8000000U
#define CAN_ESR1_FRMERR_FAST_SHIFT               27U
#define CAN_ESR1_FRMERR_FAST_WIDTH               1U
#define CAN_ESR1_FRMERR_FAST(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_FRMERR_FAST_SHIFT))&CAN_ESR1_FRMERR_FAST_MASK)
#define CAN_ESR1_CRCERR_FAST_MASK                0x10000000U
#define CAN_ESR1_CRCERR_FAST_SHIFT               28U
#define CAN_ESR1_CRCERR_FAST_WIDTH               1U
#define CAN_ESR1_CRCERR_FAST(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_CRCERR_FAST_SHIFT))&CAN_ESR1_CRCERR_FAST_MASK)
#define CAN_ESR1_BIT0ERR_FAST_MASK               0x40000000U
#define CAN_ESR1_BIT0ERR_FAST_SHIFT              30U
#define CAN_ESR1_BIT0ERR_FAST_WIDTH              1U
#define CAN_ESR1_BIT0ERR_FAST(x)                 (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_BIT0ERR_FAST_SHIFT))&CAN_ESR1_BIT0ERR_FAST_MASK)
#define CAN_ESR1_BIT1ERR_FAST_MASK               0x80000000U
#define CAN_ESR1_BIT1ERR_FAST_SHIFT              31U
#define CAN_ESR1_BIT1ERR_FAST_WIDTH              1U
#define CAN_ESR1_BIT1ERR_FAST(x)                 (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_BIT1ERR_FAST_SHIFT))&CAN_ESR1_BIT1ERR_FAST_MASK)

/**IMASK4 - Interrupt Masks 4 register */
#define CAN_IMASK4_BUF127TO96M_MASK               (0xFFFFFFFFU)
#define CAN_IMASK4_BUF127TO96M_SHIFT              (0U)
#define CAN_IMASK4_BUF127TO96M_WIDTH              (32U)
#define CAN_IMASK4_BUF127TO96M(x)                 (((uint32_t)(((uint32_t)(x)) << CAN_IMASK4_BUF127TO96M_SHIFT )) & CAN_IMASK4_BUF127TO96M_MASK )

/**IMASK3 - Interrupt Masks 3 register */
#define CAN_IMASK3_BUF95TO64M_MASK               (0xFFFFFFFFU)
#define CAN_IMASK3_BUF95TO64M_SHIFT              (0U)
#define CAN_IMASK3_BUF95TO64M_WIDTH              (32U)
#define CAN_IMASK3_BUF95TO64M(x)                 (((uint32_t)(((uint32_t)(x)) << CAN_IMASK3_BUF95TO64M_SHIFT)) & CAN_IMASK3_BUF63TO32M_SHIFT)

/**IMASK2 - Interrupt Masks 2 register */
#define CAN_IMASK2_BUF63TO32M_MASK               (0xFFFFFFFFU)
#define CAN_IMASK2_BUF63TO32M_SHIFT              (0U)
#define CAN_IMASK2_BUF63TO32M_WIDTH              (32U)
#define CAN_IMASK2_BUF63TO32M(x)                 (((uint32_t)(((uint32_t)(x)) << CAN_IMASK2_BUF63TO32M_SHIFT)) & CAN_IMASK2_BUF63TO32M_MASK)

/** IMASK1 Bit Fields */
#define CAN_IMASK1_BUF31TO0M_MASK                0xFFFFFFFFU
#define CAN_IMASK1_BUF31TO0M_SHIFT               0U
#define CAN_IMASK1_BUF31TO0M_WIDTH               32U
#define CAN_IMASK1_BUF31TO0M(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_IMASK1_BUF31TO0M_SHIFT))&CAN_IMASK1_BUF31TO0M_MASK)
/** IFLAG1 Bit Fields */
#define CAN_IFLAG1_BUF0I_MASK                    0x1U
#define CAN_IFLAG1_BUF0I_SHIFT                   0U
#define CAN_IFLAG1_BUF0I_WIDTH                   1U
#define CAN_IFLAG1_BUF0I(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_IFLAG1_BUF0I_SHIFT))&CAN_IFLAG1_BUF0I_MASK)
#define CAN_IFLAG1_BUF4TO1I_MASK                 0x1Eu
#define CAN_IFLAG1_BUF4TO1I_SHIFT                1U
#define CAN_IFLAG1_BUF4TO1I_WIDTH                4U
#define CAN_IFLAG1_BUF4TO1I(x)                   (((uint32_t)(((uint32_t)(x))<<CAN_IFLAG1_BUF4TO1I_SHIFT))&CAN_IFLAG1_BUF4TO1I_MASK)
#define CAN_IFLAG1_BUF5I_MASK                    0x20U
#define CAN_IFLAG1_BUF5I_SHIFT                   5U
#define CAN_IFLAG1_BUF5I_WIDTH                   1U
#define CAN_IFLAG1_BUF5I(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_IFLAG1_BUF5I_SHIFT))&CAN_IFLAG1_BUF5I_MASK)
#define CAN_IFLAG1_BUF6I_MASK                    0x40U
#define CAN_IFLAG1_BUF6I_SHIFT                   6U
#define CAN_IFLAG1_BUF6I_WIDTH                   1U
#define CAN_IFLAG1_BUF6I(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_IFLAG1_BUF6I_SHIFT))&CAN_IFLAG1_BUF6I_MASK)
#define CAN_IFLAG1_BUF7I_MASK                    0x80U
#define CAN_IFLAG1_BUF7I_SHIFT                   7U
#define CAN_IFLAG1_BUF7I_WIDTH                   1U
#define CAN_IFLAG1_BUF7I(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_IFLAG1_BUF7I_SHIFT))&CAN_IFLAG1_BUF7I_MASK)
#define CAN_IFLAG1_BUF31TO8I_MASK                0xFFFFFF00U
#define CAN_IFLAG1_BUF31TO8I_SHIFT               8U
#define CAN_IFLAG1_BUF31TO8I_WIDTH               24U
#define CAN_IFLAG1_BUF31TO8I(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_IFLAG1_BUF31TO8I_SHIFT))&CAN_IFLAG1_BUF31TO8I_MASK)
/** CTRL2 Bit Fields */
#define CAN_CTRL2_TSTAMPCAP_MASK                 (0xC0U)
#define CAN_CTRL2_TSTAMPCAP_SHIFT                (6U)
#define CAN_CTRL2_TSTAMPCAP_WIDTH                (2U)
#define CAN_CTRL2_TSTAMPCAP(x)                   (((uint32_t)(((uint32_t)(x)) << CAN_CTRL2_TSTAMPCAP_SHIFT)) & CAN_CTRL2_TSTAMPCAP_MASK)

#define CAN_CTRL2_MBTSBASE_MASK                  (0x300U)
#define CAN_CTRL2_MBTSBASE_SHIFT                 (8U)
#define CAN_CTRL2_MBTSBASE_WIDTH                 (2U)
#define CAN_CTRL2_MBTSBASE(x)                    (((uint32_t)(((uint32_t)(x)) << CAN_CTRL2_MBTSBASE_SHIFT)) & CAN_CTRL2_MBTSBASE_MASK)

#define CAN_CTRL2_EDFLTDIS_MASK                  0x800U
#define CAN_CTRL2_EDFLTDIS_SHIFT                 11U
#define CAN_CTRL2_EDFLTDIS_WIDTH                 1U
#define CAN_CTRL2_EDFLTDIS(x)                    (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_EDFLTDIS_SHIFT))&CAN_CTRL2_EDFLTDIS_MASK)
#define CAN_CTRL2_ISOCANFDEN_MASK                0x1000U
#define CAN_CTRL2_ISOCANFDEN_SHIFT               12U
#define CAN_CTRL2_ISOCANFDEN_WIDTH               1U
#define CAN_CTRL2_ISOCANFDEN(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_ISOCANFDEN_SHIFT))&CAN_CTRL2_ISOCANFDEN_MASK)
#define CAN_CTRL2_PREXCEN_MASK                   0x4000U
#define CAN_CTRL2_PREXCEN_SHIFT                  14U
#define CAN_CTRL2_PREXCEN_WIDTH                  1U
#define CAN_CTRL2_PREXCEN(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_PREXCEN_SHIFT))&CAN_CTRL2_PREXCEN_MASK)
#define CAN_CTRL2_TIMER_SRC_MASK                 0x8000U
#define CAN_CTRL2_TIMER_SRC_SHIFT                15U
#define CAN_CTRL2_TIMER_SRC_WIDTH                1U
#define CAN_CTRL2_TIMER_SRC(x)                   (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_TIMER_SRC_SHIFT))&CAN_CTRL2_TIMER_SRC_MASK)
#define CAN_CTRL2_EACEN_MASK                     0x10000U
#define CAN_CTRL2_EACEN_SHIFT                    16U
#define CAN_CTRL2_EACEN_WIDTH                    1U
#define CAN_CTRL2_EACEN(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_EACEN_SHIFT))&CAN_CTRL2_EACEN_MASK)
#define CAN_CTRL2_RRS_MASK                       0x20000U
#define CAN_CTRL2_RRS_SHIFT                      17U
#define CAN_CTRL2_RRS_WIDTH                      1U
#define CAN_CTRL2_RRS(x)                         (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_RRS_SHIFT))&CAN_CTRL2_RRS_MASK)
#define CAN_CTRL2_MRP_MASK                       0x40000U
#define CAN_CTRL2_MRP_SHIFT                      18U
#define CAN_CTRL2_MRP_WIDTH                      1U
#define CAN_CTRL2_MRP(x)                         (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_MRP_SHIFT))&CAN_CTRL2_MRP_MASK)
#define CAN_CTRL2_TASD_MASK                      0xF80000U
#define CAN_CTRL2_TASD_SHIFT                     19U
#define CAN_CTRL2_TASD_WIDTH                     5U
#define CAN_CTRL2_TASD(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_TASD_SHIFT))&CAN_CTRL2_TASD_MASK)
#define CAN_CTRL2_RFFN_MASK                      0xF000000U
#define CAN_CTRL2_RFFN_SHIFT                     24U
#define CAN_CTRL2_RFFN_WIDTH                     4U
#define CAN_CTRL2_RFFN(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_RFFN_SHIFT))&CAN_CTRL2_RFFN_MASK)
#define CAN_CTRL2_WRMFRZ_MASK                    0x10000000U
#define CAN_CTRL2_WRMFRZ_SHIFT                   28U
#define CAN_CTRL2_WRMFRZ_WIDTH                   1U
#define CAN_CTRL2_WRMFRZ(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_WRMFRZ_SHIFT))&CAN_CTRL2_WRMFRZ_MASK)
#define CAN_CTRL2_ECRWRE_MASK                    0x20000000U
#define CAN_CTRL2_ECRWRE_SHIFT                   29U
#define CAN_CTRL2_ECRWRE_WIDTH                   1U
#define CAN_CTRL2_ECRWRE(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_ECRWRE_SHIFT))&CAN_CTRL2_ECRWRE_MASK)
#define CAN_CTRL2_BOFFDONEMSK_MASK               0x40000000U
#define CAN_CTRL2_BOFFDONEMSK_SHIFT              30U
#define CAN_CTRL2_BOFFDONEMSK_WIDTH              1U
#define CAN_CTRL2_BOFFDONEMSK(x)                 (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_BOFFDONEMSK_SHIFT))&CAN_CTRL2_BOFFDONEMSK_MASK)
#define CAN_CTRL2_ERRMSK_FAST_MASK               0x80000000U
#define CAN_CTRL2_ERRMSK_FAST_SHIFT              31U
#define CAN_CTRL2_ERRMSK_FAST_WIDTH              1U
#define CAN_CTRL2_ERRMSK_FAST(x)                 (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_ERRMSK_FAST_SHIFT))&CAN_CTRL2_ERRMSK_FAST_MASK)
/** MECR Bit Fields */
#define CAN_MECR_ECRWRDIS_MASK                   0x80000000U
#define CAN_MECR_ECRWRDIS_SHIFT                  31U
#define CAN_MECR_ECRWRDIS_WIDTH                  1U
#define CAN_MECR_ECRWRDIS(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_MECR_ECRWRDIS_SHIFT))&CAN_MECR_ECRWRDIS_MASK)
#define CAN_MECR_NCEFAFRZ_MASK                   0x80U
#define CAN_MECR_NCEFAFRZ_SHIFT                  7U
#define CAN_MECR_NCEFAFRZ_WIDTH                  1U
#define CAN_MECR_NCEFAFRZ(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_MECR_ECRWRDIS_SHIFT))&CAN_MECR_ECRWRDIS_MASK)
/** ESR2 Bit Fields */
#define CAN_ESR2_IMB_MASK                        0x2000U
#define CAN_ESR2_IMB_SHIFT                       13U
#define CAN_ESR2_IMB_WIDTH                       1U
#define CAN_ESR2_IMB(x)                          (((uint32_t)(((uint32_t)(x))<<CAN_ESR2_IMB_SHIFT))&CAN_ESR2_IMB_MASK)
#define CAN_ESR2_VPS_MASK                        0x4000U
#define CAN_ESR2_VPS_SHIFT                       14U
#define CAN_ESR2_VPS_WIDTH                       1U
#define CAN_ESR2_VPS(x)                          (((uint32_t)(((uint32_t)(x))<<CAN_ESR2_VPS_SHIFT))&CAN_ESR2_VPS_MASK)
#define CAN_ESR2_LPTM_MASK                       0x7F0000U
#define CAN_ESR2_LPTM_SHIFT                      16U
#define CAN_ESR2_LPTM_WIDTH                      7U
#define CAN_ESR2_LPTM(x)                         (((uint32_t)(((uint32_t)(x))<<CAN_ESR2_LPTM_SHIFT))&CAN_ESR2_LPTM_MASK)
/** CRCR Bit Fields */
#define CAN_CRCR_TXCRC_MASK                      0x7FFFU
#define CAN_CRCR_TXCRC_SHIFT                     0U
#define CAN_CRCR_TXCRC_WIDTH                     15U
#define CAN_CRCR_TXCRC(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_CRCR_TXCRC_SHIFT))&CAN_CRCR_TXCRC_MASK)
#define CAN_CRCR_MBCRC_MASK                      0x7F0000U
#define CAN_CRCR_MBCRC_SHIFT                     16U
#define CAN_CRCR_MBCRC_WIDTH                     7U
#define CAN_CRCR_MBCRC(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_CRCR_MBCRC_SHIFT))&CAN_CRCR_MBCRC_MASK)
/** RXFGMASK Bit Fields */
#define CAN_RXFGMASK_FGM_MASK                    0xFFFFFFFFU
#define CAN_RXFGMASK_FGM_SHIFT                   0U
#define CAN_RXFGMASK_FGM_WIDTH                   32U
#define CAN_RXFGMASK_FGM(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_RXFGMASK_FGM_SHIFT))&CAN_RXFGMASK_FGM_MASK)
/** RXFIR Bit Fields */
#define CAN_RXFIR_IDHIT_MASK                     0x1FFU
#define CAN_RXFIR_IDHIT_SHIFT                    0U
#define CAN_RXFIR_IDHIT_WIDTH                    9U
#define CAN_RXFIR_IDHIT(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_RXFIR_IDHIT_SHIFT))&CAN_RXFIR_IDHIT_MASK)
/** CBT Bit Fields */
#define CAN_CBT_EPSEG2_MASK                      0x1FU
#define CAN_CBT_EPSEG2_SHIFT                     0U
#define CAN_CBT_EPSEG2_WIDTH                     5U
#define CAN_CBT_EPSEG2(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_CBT_EPSEG2_SHIFT))&CAN_CBT_EPSEG2_MASK)
#define CAN_CBT_EPSEG1_MASK                      0x3E0U
#define CAN_CBT_EPSEG1_SHIFT                     5U
#define CAN_CBT_EPSEG1_WIDTH                     5U
#define CAN_CBT_EPSEG1(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_CBT_EPSEG1_SHIFT))&CAN_CBT_EPSEG1_MASK)
#define CAN_CBT_EPROPSEG_MASK                    0xFC00U
#define CAN_CBT_EPROPSEG_SHIFT                   10U
#define CAN_CBT_EPROPSEG_WIDTH                   6U
#define CAN_CBT_EPROPSEG(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_CBT_EPROPSEG_SHIFT))&CAN_CBT_EPROPSEG_MASK)
#define CAN_CBT_ERJW_MASK                        0x1F0000U
#define CAN_CBT_ERJW_SHIFT                       16U
#define CAN_CBT_ERJW_WIDTH                       5U
#define CAN_CBT_ERJW(x)                          (((uint32_t)(((uint32_t)(x))<<CAN_CBT_ERJW_SHIFT))&CAN_CBT_ERJW_MASK)
#define CAN_CBT_EPRESDIV_MASK                    0x7FE00000U
#define CAN_CBT_EPRESDIV_SHIFT                   21U
#define CAN_CBT_EPRESDIV_WIDTH                   10U
#define CAN_CBT_EPRESDIV(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_CBT_EPRESDIV_SHIFT))&CAN_CBT_EPRESDIV_MASK)
#define CAN_CBT_BTF_MASK                         0x80000000U
#define CAN_CBT_BTF_SHIFT                        31U
#define CAN_CBT_BTF_WIDTH                        1U
#define CAN_CBT_BTF(x)                           (((uint32_t)(((uint32_t)(x))<<CAN_CBT_BTF_SHIFT))&CAN_CBT_BTF_MASK)
/** RAMn Bit Fields */
#define CAN_RAMn_DATA_BYTE_3_MASK                0xFFU
#define CAN_RAMn_DATA_BYTE_3_SHIFT               0U
#define CAN_RAMn_DATA_BYTE_3_WIDTH               8U
#define CAN_RAMn_DATA_BYTE_3(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_RAMn_DATA_BYTE_3_SHIFT))&CAN_RAMn_DATA_BYTE_3_MASK)
#define CAN_RAMn_DATA_BYTE_2_MASK                0xFF00U
#define CAN_RAMn_DATA_BYTE_2_SHIFT               8U
#define CAN_RAMn_DATA_BYTE_2_WIDTH               8U
#define CAN_RAMn_DATA_BYTE_2(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_RAMn_DATA_BYTE_2_SHIFT))&CAN_RAMn_DATA_BYTE_2_MASK)
#define CAN_RAMn_DATA_BYTE_1_MASK                0xFF0000U
#define CAN_RAMn_DATA_BYTE_1_SHIFT               16U
#define CAN_RAMn_DATA_BYTE_1_WIDTH               8U
#define CAN_RAMn_DATA_BYTE_1(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_RAMn_DATA_BYTE_1_SHIFT))&CAN_RAMn_DATA_BYTE_1_MASK)
#define CAN_RAMn_DATA_BYTE_0_MASK                0xFF000000U
#define CAN_RAMn_DATA_BYTE_0_SHIFT               24U
#define CAN_RAMn_DATA_BYTE_0_WIDTH               8U
#define CAN_RAMn_DATA_BYTE_0(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_RAMn_DATA_BYTE_0_SHIFT))&CAN_RAMn_DATA_BYTE_0_MASK)
/** RXIMR Bit Fields */
#define CAN_RXIMR_MI_MASK                        0xFFFFFFFFU
#define CAN_RXIMR_MI_SHIFT                       0U
#define CAN_RXIMR_MI_WIDTH                       32U
#define CAN_RXIMR_MI(x)                          (((uint32_t)(((uint32_t)(x))<<CAN_RXIMR_MI_SHIFT))&CAN_RXIMR_MI_MASK)
/** CTRL1_PN Bit Fields */
#define CAN_CTRL1_PN_FCS_MASK                    0x3U
#define CAN_CTRL1_PN_FCS_SHIFT                   0U
#define CAN_CTRL1_PN_FCS_WIDTH                   2U
#define CAN_CTRL1_PN_FCS(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_PN_FCS_SHIFT))&CAN_CTRL1_PN_FCS_MASK)
#define CAN_CTRL1_PN_IDFS_MASK                   0xCu
#define CAN_CTRL1_PN_IDFS_SHIFT                  2U
#define CAN_CTRL1_PN_IDFS_WIDTH                  2U
#define CAN_CTRL1_PN_IDFS(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_PN_IDFS_SHIFT))&CAN_CTRL1_PN_IDFS_MASK)
#define CAN_CTRL1_PN_PLFS_MASK                   0x30U
#define CAN_CTRL1_PN_PLFS_SHIFT                  4U
#define CAN_CTRL1_PN_PLFS_WIDTH                  2U
#define CAN_CTRL1_PN_PLFS(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_PN_PLFS_SHIFT))&CAN_CTRL1_PN_PLFS_MASK)
#define CAN_CTRL1_PN_NMATCH_MASK                 0xFF00U
#define CAN_CTRL1_PN_NMATCH_SHIFT                8U
#define CAN_CTRL1_PN_NMATCH_WIDTH                8U
#define CAN_CTRL1_PN_NMATCH(x)                   (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_PN_NMATCH_SHIFT))&CAN_CTRL1_PN_NMATCH_MASK)
#define CAN_CTRL1_PN_WUMF_MSK_MASK               0x10000U
#define CAN_CTRL1_PN_WUMF_MSK_SHIFT              16U
#define CAN_CTRL1_PN_WUMF_MSK_WIDTH              1U
#define CAN_CTRL1_PN_WUMF_MSK(x)                 (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_PN_WUMF_MSK_SHIFT))&CAN_CTRL1_PN_WUMF_MSK_MASK)
#define CAN_CTRL1_PN_WTOF_MSK_MASK               0x20000U
#define CAN_CTRL1_PN_WTOF_MSK_SHIFT              17U
#define CAN_CTRL1_PN_WTOF_MSK_WIDTH              1U
#define CAN_CTRL1_PN_WTOF_MSK(x)                 (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_PN_WTOF_MSK_SHIFT))&CAN_CTRL1_PN_WTOF_MSK_MASK)
/** CTRL2_PN Bit Fields */
#define CAN_CTRL2_PN_MATCHTO_MASK                0xFFFFU
#define CAN_CTRL2_PN_MATCHTO_SHIFT               0U
#define CAN_CTRL2_PN_MATCHTO_WIDTH               16U
#define CAN_CTRL2_PN_MATCHTO(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_PN_MATCHTO_SHIFT))&CAN_CTRL2_PN_MATCHTO_MASK)
/** WU_MTC Bit Fields */
#define CAN_WU_MTC_MCOUNTER_MASK                 0xFF00U
#define CAN_WU_MTC_MCOUNTER_SHIFT                8U
#define CAN_WU_MTC_MCOUNTER_WIDTH                8U
#define CAN_WU_MTC_MCOUNTER(x)                   (((uint32_t)(((uint32_t)(x))<<CAN_WU_MTC_MCOUNTER_SHIFT))&CAN_WU_MTC_MCOUNTER_MASK)
#define CAN_WU_MTC_WUMF_MASK                     0x10000U
#define CAN_WU_MTC_WUMF_SHIFT                    16U
#define CAN_WU_MTC_WUMF_WIDTH                    1U
#define CAN_WU_MTC_WUMF(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_WU_MTC_WUMF_SHIFT))&CAN_WU_MTC_WUMF_MASK)
#define CAN_WU_MTC_WTOF_MASK                     0x20000U
#define CAN_WU_MTC_WTOF_SHIFT                    17U
#define CAN_WU_MTC_WTOF_WIDTH                    1U
#define CAN_WU_MTC_WTOF(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_WU_MTC_WTOF_SHIFT))&CAN_WU_MTC_WTOF_MASK)
/** FLT_ID1 Bit Fields */
#define CAN_FLT_ID1_FLT_ID1_MASK                 0x1FFFFFFFU
#define CAN_FLT_ID1_FLT_ID1_SHIFT                0U
#define CAN_FLT_ID1_FLT_ID1_WIDTH                29U
#define CAN_FLT_ID1_FLT_ID1(x)                   (((uint32_t)(((uint32_t)(x))<<CAN_FLT_ID1_FLT_ID1_SHIFT))&CAN_FLT_ID1_FLT_ID1_MASK)
#define CAN_FLT_ID1_FLT_RTR_MASK                 0x20000000U
#define CAN_FLT_ID1_FLT_RTR_SHIFT                29U
#define CAN_FLT_ID1_FLT_RTR_WIDTH                1U
#define CAN_FLT_ID1_FLT_RTR(x)                   (((uint32_t)(((uint32_t)(x))<<CAN_FLT_ID1_FLT_RTR_SHIFT))&CAN_FLT_ID1_FLT_RTR_MASK)
#define CAN_FLT_ID1_FLT_IDE_MASK                 0x40000000U
#define CAN_FLT_ID1_FLT_IDE_SHIFT                30U
#define CAN_FLT_ID1_FLT_IDE_WIDTH                1U
#define CAN_FLT_ID1_FLT_IDE(x)                   (((uint32_t)(((uint32_t)(x))<<CAN_FLT_ID1_FLT_IDE_SHIFT))&CAN_FLT_ID1_FLT_IDE_MASK)
/** FLT_DLC Bit Fields */
#define CAN_FLT_DLC_FLT_DLC_HI_MASK              0xFU
#define CAN_FLT_DLC_FLT_DLC_HI_SHIFT             0U
#define CAN_FLT_DLC_FLT_DLC_HI_WIDTH             4U
#define CAN_FLT_DLC_FLT_DLC_HI(x)                (((uint32_t)(((uint32_t)(x))<<CAN_FLT_DLC_FLT_DLC_HI_SHIFT))&CAN_FLT_DLC_FLT_DLC_HI_MASK)
#define CAN_FLT_DLC_FLT_DLC_LO_MASK              0xF0000U
#define CAN_FLT_DLC_FLT_DLC_LO_SHIFT             16U
#define CAN_FLT_DLC_FLT_DLC_LO_WIDTH             4U
#define CAN_FLT_DLC_FLT_DLC_LO(x)                (((uint32_t)(((uint32_t)(x))<<CAN_FLT_DLC_FLT_DLC_LO_SHIFT))&CAN_FLT_DLC_FLT_DLC_LO_MASK)
/** PL1_LO Bit Fields */
#define CAN_PL1_LO_Data_byte_3_MASK              0xFFU
#define CAN_PL1_LO_Data_byte_3_SHIFT             0U
#define CAN_PL1_LO_Data_byte_3_WIDTH             8U
#define CAN_PL1_LO_Data_byte_3(x)                (((uint32_t)(((uint32_t)(x))<<CAN_PL1_LO_Data_byte_3_SHIFT))&CAN_PL1_LO_Data_byte_3_MASK)
#define CAN_PL1_LO_Data_byte_2_MASK              0xFF00U
#define CAN_PL1_LO_Data_byte_2_SHIFT             8U
#define CAN_PL1_LO_Data_byte_2_WIDTH             8U
#define CAN_PL1_LO_Data_byte_2(x)                (((uint32_t)(((uint32_t)(x))<<CAN_PL1_LO_Data_byte_2_SHIFT))&CAN_PL1_LO_Data_byte_2_MASK)
#define CAN_PL1_LO_Data_byte_1_MASK              0xFF0000U
#define CAN_PL1_LO_Data_byte_1_SHIFT             16U
#define CAN_PL1_LO_Data_byte_1_WIDTH             8U
#define CAN_PL1_LO_Data_byte_1(x)                (((uint32_t)(((uint32_t)(x))<<CAN_PL1_LO_Data_byte_1_SHIFT))&CAN_PL1_LO_Data_byte_1_MASK)
#define CAN_PL1_LO_Data_byte_0_MASK              0xFF000000U
#define CAN_PL1_LO_Data_byte_0_SHIFT             24U
#define CAN_PL1_LO_Data_byte_0_WIDTH             8U
#define CAN_PL1_LO_Data_byte_0(x)                (((uint32_t)(((uint32_t)(x))<<CAN_PL1_LO_Data_byte_0_SHIFT))&CAN_PL1_LO_Data_byte_0_MASK)
/** PL1_HI Bit Fields */
#define CAN_PL1_HI_Data_byte_7_MASK              0xFFU
#define CAN_PL1_HI_Data_byte_7_SHIFT             0U
#define CAN_PL1_HI_Data_byte_7_WIDTH             8U
#define CAN_PL1_HI_Data_byte_7(x)                (((uint32_t)(((uint32_t)(x))<<CAN_PL1_HI_Data_byte_7_SHIFT))&CAN_PL1_HI_Data_byte_7_MASK)
#define CAN_PL1_HI_Data_byte_6_MASK              0xFF00U
#define CAN_PL1_HI_Data_byte_6_SHIFT             8U
#define CAN_PL1_HI_Data_byte_6_WIDTH             8U
#define CAN_PL1_HI_Data_byte_6(x)                (((uint32_t)(((uint32_t)(x))<<CAN_PL1_HI_Data_byte_6_SHIFT))&CAN_PL1_HI_Data_byte_6_MASK)
#define CAN_PL1_HI_Data_byte_5_MASK              0xFF0000U
#define CAN_PL1_HI_Data_byte_5_SHIFT             16U
#define CAN_PL1_HI_Data_byte_5_WIDTH             8U
#define CAN_PL1_HI_Data_byte_5(x)                (((uint32_t)(((uint32_t)(x))<<CAN_PL1_HI_Data_byte_5_SHIFT))&CAN_PL1_HI_Data_byte_5_MASK)
#define CAN_PL1_HI_Data_byte_4_MASK              0xFF000000U
#define CAN_PL1_HI_Data_byte_4_SHIFT             24U
#define CAN_PL1_HI_Data_byte_4_WIDTH             8U
#define CAN_PL1_HI_Data_byte_4(x)                (((uint32_t)(((uint32_t)(x))<<CAN_PL1_HI_Data_byte_4_SHIFT))&CAN_PL1_HI_Data_byte_4_MASK)
/** FLT_ID2_IDMASK Bit Fields */
#define CAN_FLT_ID2_IDMASK_FLT_ID2_IDMASK_MASK   0x1FFFFFFFU
#define CAN_FLT_ID2_IDMASK_FLT_ID2_IDMASK_SHIFT  0U
#define CAN_FLT_ID2_IDMASK_FLT_ID2_IDMASK_WIDTH  29U
#define CAN_FLT_ID2_IDMASK_FLT_ID2_IDMASK(x)     (((uint32_t)(((uint32_t)(x))<<CAN_FLT_ID2_IDMASK_FLT_ID2_IDMASK_SHIFT))&CAN_FLT_ID2_IDMASK_FLT_ID2_IDMASK_MASK)
#define CAN_FLT_ID2_IDMASK_RTR_MSK_MASK          0x20000000U
#define CAN_FLT_ID2_IDMASK_RTR_MSK_SHIFT         29U
#define CAN_FLT_ID2_IDMASK_RTR_MSK_WIDTH         1U
#define CAN_FLT_ID2_IDMASK_RTR_MSK(x)            (((uint32_t)(((uint32_t)(x))<<CAN_FLT_ID2_IDMASK_RTR_MSK_SHIFT))&CAN_FLT_ID2_IDMASK_RTR_MSK_MASK)
#define CAN_FLT_ID2_IDMASK_IDE_MSK_MASK          0x40000000U
#define CAN_FLT_ID2_IDMASK_IDE_MSK_SHIFT         30U
#define CAN_FLT_ID2_IDMASK_IDE_MSK_WIDTH         1U
#define CAN_FLT_ID2_IDMASK_IDE_MSK(x)            (((uint32_t)(((uint32_t)(x))<<CAN_FLT_ID2_IDMASK_IDE_MSK_SHIFT))&CAN_FLT_ID2_IDMASK_IDE_MSK_MASK)
/** PL2_PLMASK_LO Bit Fields */
#define CAN_PL2_PLMASK_LO_Data_byte_3_MASK       0xFFU
#define CAN_PL2_PLMASK_LO_Data_byte_3_SHIFT      0U
#define CAN_PL2_PLMASK_LO_Data_byte_3_WIDTH      8U
#define CAN_PL2_PLMASK_LO_Data_byte_3(x)         (((uint32_t)(((uint32_t)(x))<<CAN_PL2_PLMASK_LO_Data_byte_3_SHIFT))&CAN_PL2_PLMASK_LO_Data_byte_3_MASK)
#define CAN_PL2_PLMASK_LO_Data_byte_2_MASK       0xFF00U
#define CAN_PL2_PLMASK_LO_Data_byte_2_SHIFT      8U
#define CAN_PL2_PLMASK_LO_Data_byte_2_WIDTH      8U
#define CAN_PL2_PLMASK_LO_Data_byte_2(x)         (((uint32_t)(((uint32_t)(x))<<CAN_PL2_PLMASK_LO_Data_byte_2_SHIFT))&CAN_PL2_PLMASK_LO_Data_byte_2_MASK)
#define CAN_PL2_PLMASK_LO_Data_byte_1_MASK       0xFF0000U
#define CAN_PL2_PLMASK_LO_Data_byte_1_SHIFT      16U
#define CAN_PL2_PLMASK_LO_Data_byte_1_WIDTH      8U
#define CAN_PL2_PLMASK_LO_Data_byte_1(x)         (((uint32_t)(((uint32_t)(x))<<CAN_PL2_PLMASK_LO_Data_byte_1_SHIFT))&CAN_PL2_PLMASK_LO_Data_byte_1_MASK)
#define CAN_PL2_PLMASK_LO_Data_byte_0_MASK       0xFF000000U
#define CAN_PL2_PLMASK_LO_Data_byte_0_SHIFT      24U
#define CAN_PL2_PLMASK_LO_Data_byte_0_WIDTH      8U
#define CAN_PL2_PLMASK_LO_Data_byte_0(x)         (((uint32_t)(((uint32_t)(x))<<CAN_PL2_PLMASK_LO_Data_byte_0_SHIFT))&CAN_PL2_PLMASK_LO_Data_byte_0_MASK)
/** PL2_PLMASK_HI Bit Fields */
#define CAN_PL2_PLMASK_HI_Data_byte_7_MASK       0xFFU
#define CAN_PL2_PLMASK_HI_Data_byte_7_SHIFT      0U
#define CAN_PL2_PLMASK_HI_Data_byte_7_WIDTH      8U
#define CAN_PL2_PLMASK_HI_Data_byte_7(x)         (((uint32_t)(((uint32_t)(x))<<CAN_PL2_PLMASK_HI_Data_byte_7_SHIFT))&CAN_PL2_PLMASK_HI_Data_byte_7_MASK)
#define CAN_PL2_PLMASK_HI_Data_byte_6_MASK       0xFF00U
#define CAN_PL2_PLMASK_HI_Data_byte_6_SHIFT      8U
#define CAN_PL2_PLMASK_HI_Data_byte_6_WIDTH      8U
#define CAN_PL2_PLMASK_HI_Data_byte_6(x)         (((uint32_t)(((uint32_t)(x))<<CAN_PL2_PLMASK_HI_Data_byte_6_SHIFT))&CAN_PL2_PLMASK_HI_Data_byte_6_MASK)
#define CAN_PL2_PLMASK_HI_Data_byte_5_MASK       0xFF0000U
#define CAN_PL2_PLMASK_HI_Data_byte_5_SHIFT      16U
#define CAN_PL2_PLMASK_HI_Data_byte_5_WIDTH      8U
#define CAN_PL2_PLMASK_HI_Data_byte_5(x)         (((uint32_t)(((uint32_t)(x))<<CAN_PL2_PLMASK_HI_Data_byte_5_SHIFT))&CAN_PL2_PLMASK_HI_Data_byte_5_MASK)
#define CAN_PL2_PLMASK_HI_Data_byte_4_MASK       0xFF000000U
#define CAN_PL2_PLMASK_HI_Data_byte_4_SHIFT      24U
#define CAN_PL2_PLMASK_HI_Data_byte_4_WIDTH      8U
#define CAN_PL2_PLMASK_HI_Data_byte_4(x)         (((uint32_t)(((uint32_t)(x))<<CAN_PL2_PLMASK_HI_Data_byte_4_SHIFT))&CAN_PL2_PLMASK_HI_Data_byte_4_MASK)
/** WMBn_CS Bit Fields */
#define CAN_WMBn_CS_DLC_MASK                     0xF0000U
#define CAN_WMBn_CS_DLC_SHIFT                    16U
#define CAN_WMBn_CS_DLC_WIDTH                    4U
#define CAN_WMBn_CS_DLC(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_CS_DLC_SHIFT))&CAN_WMBn_CS_DLC_MASK)
#define CAN_WMBn_CS_RTR_MASK                     0x100000U
#define CAN_WMBn_CS_RTR_SHIFT                    20U
#define CAN_WMBn_CS_RTR_WIDTH                    1U
#define CAN_WMBn_CS_RTR(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_CS_RTR_SHIFT))&CAN_WMBn_CS_RTR_MASK)
#define CAN_WMBn_CS_IDE_MASK                     0x200000U
#define CAN_WMBn_CS_IDE_SHIFT                    21U
#define CAN_WMBn_CS_IDE_WIDTH                    1U
#define CAN_WMBn_CS_IDE(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_CS_IDE_SHIFT))&CAN_WMBn_CS_IDE_MASK)
#define CAN_WMBn_CS_SRR_MASK                     0x400000U
#define CAN_WMBn_CS_SRR_SHIFT                    22U
#define CAN_WMBn_CS_SRR_WIDTH                    1U
#define CAN_WMBn_CS_SRR(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_CS_SRR_SHIFT))&CAN_WMBn_CS_SRR_MASK)
/** WMBn_ID Bit Fields */
#define CAN_WMBn_ID_ID_MASK                      0x1FFFFFFFU
#define CAN_WMBn_ID_ID_SHIFT                     0U
#define CAN_WMBn_ID_ID_WIDTH                     29U
#define CAN_WMBn_ID_ID(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_ID_ID_SHIFT))&CAN_WMBn_ID_ID_MASK)
/** WMBn_D03 Bit Fields */
#define CAN_WMBn_D03_Data_byte_3_MASK            0xFFU
#define CAN_WMBn_D03_Data_byte_3_SHIFT           0U
#define CAN_WMBn_D03_Data_byte_3_WIDTH           8U
#define CAN_WMBn_D03_Data_byte_3(x)              (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_D03_Data_byte_3_SHIFT))&CAN_WMBn_D03_Data_byte_3_MASK)
#define CAN_WMBn_D03_Data_byte_2_MASK            0xFF00U
#define CAN_WMBn_D03_Data_byte_2_SHIFT           8U
#define CAN_WMBn_D03_Data_byte_2_WIDTH           8U
#define CAN_WMBn_D03_Data_byte_2(x)              (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_D03_Data_byte_2_SHIFT))&CAN_WMBn_D03_Data_byte_2_MASK)
#define CAN_WMBn_D03_Data_byte_1_MASK            0xFF0000U
#define CAN_WMBn_D03_Data_byte_1_SHIFT           16U
#define CAN_WMBn_D03_Data_byte_1_WIDTH           8U
#define CAN_WMBn_D03_Data_byte_1(x)              (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_D03_Data_byte_1_SHIFT))&CAN_WMBn_D03_Data_byte_1_MASK)
#define CAN_WMBn_D03_Data_byte_0_MASK            0xFF000000U
#define CAN_WMBn_D03_Data_byte_0_SHIFT           24U
#define CAN_WMBn_D03_Data_byte_0_WIDTH           8U
#define CAN_WMBn_D03_Data_byte_0(x)              (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_D03_Data_byte_0_SHIFT))&CAN_WMBn_D03_Data_byte_0_MASK)
/** WMBn_D47 Bit Fields */
#define CAN_WMBn_D47_Data_byte_7_MASK            0xFFU
#define CAN_WMBn_D47_Data_byte_7_SHIFT           0U
#define CAN_WMBn_D47_Data_byte_7_WIDTH           8U
#define CAN_WMBn_D47_Data_byte_7(x)              (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_D47_Data_byte_7_SHIFT))&CAN_WMBn_D47_Data_byte_7_MASK)
#define CAN_WMBn_D47_Data_byte_6_MASK            0xFF00U
#define CAN_WMBn_D47_Data_byte_6_SHIFT           8U
#define CAN_WMBn_D47_Data_byte_6_WIDTH           8U
#define CAN_WMBn_D47_Data_byte_6(x)              (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_D47_Data_byte_6_SHIFT))&CAN_WMBn_D47_Data_byte_6_MASK)
#define CAN_WMBn_D47_Data_byte_5_MASK            0xFF0000U
#define CAN_WMBn_D47_Data_byte_5_SHIFT           16U
#define CAN_WMBn_D47_Data_byte_5_WIDTH           8U
#define CAN_WMBn_D47_Data_byte_5(x)              (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_D47_Data_byte_5_SHIFT))&CAN_WMBn_D47_Data_byte_5_MASK)
#define CAN_WMBn_D47_Data_byte_4_MASK            0xFF000000U
#define CAN_WMBn_D47_Data_byte_4_SHIFT           24U
#define CAN_WMBn_D47_Data_byte_4_WIDTH           8U
#define CAN_WMBn_D47_Data_byte_4(x)              (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_D47_Data_byte_4_SHIFT))&CAN_WMBn_D47_Data_byte_4_MASK)
/** FDCTRL Bit Fields */
#define CAN_FDCTRL_TDCVAL_MASK                   0x3FU
#define CAN_FDCTRL_TDCVAL_SHIFT                  0U
#define CAN_FDCTRL_TDCVAL_WIDTH                  6U
#define CAN_FDCTRL_TDCVAL(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_FDCTRL_TDCVAL_SHIFT))&CAN_FDCTRL_TDCVAL_MASK)
#define CAN_FDCTRL_TDCOFF_MASK                   0x1F00U
#define CAN_FDCTRL_TDCOFF_SHIFT                  8U
#define CAN_FDCTRL_TDCOFF_WIDTH                  5U
#define CAN_FDCTRL_TDCOFF(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_FDCTRL_TDCOFF_SHIFT))&CAN_FDCTRL_TDCOFF_MASK)
#define CAN_FDCTRL_TDCFAIL_MASK                  0x4000U
#define CAN_FDCTRL_TDCFAIL_SHIFT                 14U
#define CAN_FDCTRL_TDCFAIL_WIDTH                 1U
#define CAN_FDCTRL_TDCFAIL(x)                    (((uint32_t)(((uint32_t)(x))<<CAN_FDCTRL_TDCFAIL_SHIFT))&CAN_FDCTRL_TDCFAIL_MASK)
#define CAN_FDCTRL_TDCEN_MASK                    0x8000U
#define CAN_FDCTRL_TDCEN_SHIFT                   15U
#define CAN_FDCTRL_TDCEN_WIDTH                   1U
#define CAN_FDCTRL_TDCEN(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_FDCTRL_TDCEN_SHIFT))&CAN_FDCTRL_TDCEN_MASK)
#define CAN_FDCTRL_MBDSR0_MASK                   0x30000U
#define CAN_FDCTRL_MBDSR0_SHIFT                  16U
#define CAN_FDCTRL_MBDSR0_WIDTH                  2U
#define CAN_FDCTRL_MBDSR0(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_FDCTRL_MBDSR0_SHIFT))&CAN_FDCTRL_MBDSR0_MASK)
#define CAN_FDCTRL_MBDSR1_MASK                   0x180000U
#define CAN_FDCTRL_MBDSR1_SHIFT                  19U
#define CAN_FDCTRL_MBDSR1_WIDTH                  2U
#define CAN_FDCTRL_MBDSR1(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_FDCTRL_MBDSR1_SHIFT))&CAN_FDCTRL_MBDSR1_MASK)
#define CAN_FDCTRL_MBDSR2_MASK                   0xC00000U
#define CAN_FDCTRL_MBDSR2_SHIFT                  22U
#define CAN_FDCTRL_MBDSR2_WIDTH                  2U
#define CAN_FDCTRL_MBDSR2(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_FDCTRL_MBDSR2_SHIFT))&CAN_FDCTRL_MBDSR2_MASK)
#define CAN_FDCTRL_MBDSR3_MASK                   0x6000000U
#define CAN_FDCTRL_MBDSR3_SHIFT                  25U
#define CAN_FDCTRL_MBDSR3_WIDTH                  2U
#define CAN_FDCTRL_MBDSR3(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_FDCTRL_MBDSR3_SHIFT))&CAN_FDCTRL_MBDSR3_MASK)
#define CAN_FDCTRL_FDRATE_MASK                   0x80000000U
#define CAN_FDCTRL_FDRATE_SHIFT                  31U
#define CAN_FDCTRL_FDRATE_WIDTH                  1U
#define CAN_FDCTRL_FDRATE(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_FDCTRL_FDRATE_SHIFT))&CAN_FDCTRL_FDRATE_MASK)
/** FDCBT Bit Fields */
#define CAN_FDCBT_FPSEG2_MASK                    0x7U
#define CAN_FDCBT_FPSEG2_SHIFT                   0U
#define CAN_FDCBT_FPSEG2_WIDTH                   3U
#define CAN_FDCBT_FPSEG2(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_FDCBT_FPSEG2_SHIFT))&CAN_FDCBT_FPSEG2_MASK)
#define CAN_FDCBT_FPSEG1_MASK                    0xE0U
#define CAN_FDCBT_FPSEG1_SHIFT                   5U
#define CAN_FDCBT_FPSEG1_WIDTH                   3U
#define CAN_FDCBT_FPSEG1(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_FDCBT_FPSEG1_SHIFT))&CAN_FDCBT_FPSEG1_MASK)
#define CAN_FDCBT_FPROPSEG_MASK                  0x7C00U
#define CAN_FDCBT_FPROPSEG_SHIFT                 10U
#define CAN_FDCBT_FPROPSEG_WIDTH                 5U
#define CAN_FDCBT_FPROPSEG(x)                    (((uint32_t)(((uint32_t)(x))<<CAN_FDCBT_FPROPSEG_SHIFT))&CAN_FDCBT_FPROPSEG_MASK)
#define CAN_FDCBT_FRJW_MASK                      0x70000U
#define CAN_FDCBT_FRJW_SHIFT                     16U
#define CAN_FDCBT_FRJW_WIDTH                     3U
#define CAN_FDCBT_FRJW(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_FDCBT_FRJW_SHIFT))&CAN_FDCBT_FRJW_MASK)
#define CAN_FDCBT_FPRESDIV_MASK                  0x3FF00000U
#define CAN_FDCBT_FPRESDIV_SHIFT                 20U
#define CAN_FDCBT_FPRESDIV_WIDTH                 10U
#define CAN_FDCBT_FPRESDIV(x)                    (((uint32_t)(((uint32_t)(x))<<CAN_FDCBT_FPRESDIV_SHIFT))&CAN_FDCBT_FPRESDIV_MASK)
/** FDCRC Bit Fields */
#define CAN_FDCRC_FD_TXCRC_MASK                  0x1FFFFFU
#define CAN_FDCRC_FD_TXCRC_SHIFT                 0U
#define CAN_FDCRC_FD_TXCRC_WIDTH                 21U
#define CAN_FDCRC_FD_TXCRC(x)                    (((uint32_t)(((uint32_t)(x))<<CAN_FDCRC_FD_TXCRC_SHIFT))&CAN_FDCRC_FD_TXCRC_MASK)
#define CAN_FDCRC_FD_MBCRC_MASK                  0x7F000000U
#define CAN_FDCRC_FD_MBCRC_SHIFT                 24U
#define CAN_FDCRC_FD_MBCRC_WIDTH                 7U
#define CAN_FDCRC_FD_MBCRC(x)                    (((uint32_t)(((uint32_t)(x))<<CAN_FDCRC_FD_MBCRC_SHIFT))&CAN_FDCRC_FD_MBCRC_MASK)

/** ERFCR - Enhanced Rx FIFO Control Register */
#define CAN_ERFCR_ERFWM_MASK                 (0x1FU)
#define CAN_ERFCR_ERFWM_SHIFT                (0U)
#define CAN_ERFCR_ERFWM_WIDTH                (5U)
#define CAN_ERFCR_ERFWM(x)                   (((uint32_t)(((uint32_t)(x)) << CAN_ERFCR_ERFWM_SHIFT)) & CAN_ERFCR_ERFWM_MASK)

#define CAN_ERFCR_NFE_MASK                   (0x3F00U)
#define CAN_ERFCR_NFE_SHIFT                  (8U)
#define CAN_ERFCR_NFE_WIDTH                  (6U)
#define CAN_ERFCR_NFE(x)                     (((uint32_t)(((uint32_t)(x)) << CAN_ERFCR_NFE_SHIFT)) & CAN_ERFCR_NFE_MASK)

#define CAN_ERFCR_NEXIF_MASK                 (0x7F0000U)
#define CAN_ERFCR_NEXIF_SHIFT                (16U)
#define CAN_ERFCR_NEXIF_WIDTH                (7U)
#define CAN_ERFCR_NEXIF(x)                   (((uint32_t)(((uint32_t)(x)) << CAN_ERFCR_NEXIF_SHIFT)) & CAN_ERFCR_NEXIF_MASK)

#define CAN_ERFCR_DMALW_MASK                 (0x7C000000U)
#define CAN_ERFCR_DMALW_SHIFT                (26U)
#define CAN_ERFCR_DMALW_WIDTH                (5U)
#define CAN_ERFCR_DMALW(x)                   (((uint32_t)(((uint32_t)(x)) << CAN_ERFCR_DMALW_SHIFT)) & CAN_ERFCR_DMALW_MASK)

#define CAN_ERFCR_ERFEN_MASK                 (0x80000000U)
#define CAN_ERFCR_ERFEN_SHIFT                (31U)
#define CAN_ERFCR_ERFEN_WIDTH                (1U)
#define CAN_ERFCR_ERFEN(x)                   (((uint32_t)(((uint32_t)(x)) << CAN_ERFCR_ERFEN_SHIFT)) & CAN_ERFCR_ERFEN_MASK)
/**
 * @}
 */

/** @name ERFIER - Enhanced Rx FIFO Interrupt Enable Register */
/** @{ */

#define CAN_ERFIER_ERFDAIE_MASK              (0x10000000U)
#define CAN_ERFIER_ERFDAIE_SHIFT             (28U)
#define CAN_ERFIER_ERFDAIE_WIDTH             (1U)
#define CAN_ERFIER_ERFDAIE(x)                (((uint32_t)(((uint32_t)(x)) << CAN_ERFIER_ERFDAIE_SHIFT)) & CAN_ERFIER_ERFDAIE_MASK)

#define CAN_ERFIER_ERFWMIIE_MASK             (0x20000000U)
#define CAN_ERFIER_ERFWMIIE_SHIFT            (29U)
#define CAN_ERFIER_ERFWMIIE_WIDTH            (1U)
#define CAN_ERFIER_ERFWMIIE(x)               (((uint32_t)(((uint32_t)(x)) << CAN_ERFIER_ERFWMIIE_SHIFT)) & CAN_ERFIER_ERFWMIIE_MASK)

#define CAN_ERFIER_ERFOVFIE_MASK             (0x40000000U)
#define CAN_ERFIER_ERFOVFIE_SHIFT            (30U)
#define CAN_ERFIER_ERFOVFIE_WIDTH            (1U)
#define CAN_ERFIER_ERFOVFIE(x)               (((uint32_t)(((uint32_t)(x)) << CAN_ERFIER_ERFOVFIE_SHIFT)) & CAN_ERFIER_ERFOVFIE_MASK)

#define CAN_ERFIER_ERFUFWIE_MASK             (0x80000000U)
#define CAN_ERFIER_ERFUFWIE_SHIFT            (31U)
#define CAN_ERFIER_ERFUFWIE_WIDTH            (1U)
#define CAN_ERFIER_ERFUFWIE(x)               (((uint32_t)(((uint32_t)(x)) << CAN_ERFIER_ERFUFWIE_SHIFT)) & CAN_ERFIER_ERFUFWIE_MASK)
/**
 * @}
 */

/** @name ERFSR - Enhanced Rx FIFO Status Register */
/** @{ */

#define CAN_ERFSR_ERFEL_MASK                 (0x3FU)
#define CAN_ERFSR_ERFEL_SHIFT                (0U)
#define CAN_ERFSR_ERFEL_WIDTH                (6U)
#define CAN_ERFSR_ERFEL(x)                   (((uint32_t)(((uint32_t)(x)) << CAN_ERFSR_ERFEL_SHIFT)) & CAN_ERFSR_ERFEL_MASK)

#define CAN_ERFSR_ERFF_MASK                  (0x10000U)
#define CAN_ERFSR_ERFF_SHIFT                 (16U)
#define CAN_ERFSR_ERFF_WIDTH                 (1U)
#define CAN_ERFSR_ERFF(x)                    (((uint32_t)(((uint32_t)(x)) << CAN_ERFSR_ERFF_SHIFT)) & CAN_ERFSR_ERFF_MASK)

#define CAN_ERFSR_ERFE_MASK                  (0x20000U)
#define CAN_ERFSR_ERFE_SHIFT                 (17U)
#define CAN_ERFSR_ERFE_WIDTH                 (1U)
#define CAN_ERFSR_ERFE(x)                    (((uint32_t)(((uint32_t)(x)) << CAN_ERFSR_ERFE_SHIFT)) & CAN_ERFSR_ERFE_MASK)

#define CAN_ERFSR_ERFCLR_MASK                (0x8000000U)
#define CAN_ERFSR_ERFCLR_SHIFT               (27U)
#define CAN_ERFSR_ERFCLR_WIDTH               (1U)
#define CAN_ERFSR_ERFCLR(x)                  (((uint32_t)(((uint32_t)(x)) << CAN_ERFSR_ERFCLR_SHIFT)) & CAN_ERFSR_ERFCLR_MASK)

#define CAN_ERFSR_ERFDA_MASK                 (0x10000000U)
#define CAN_ERFSR_ERFDA_SHIFT                (28U)
#define CAN_ERFSR_ERFDA_WIDTH                (1U)
#define CAN_ERFSR_ERFDA(x)                   (((uint32_t)(((uint32_t)(x)) << CAN_ERFSR_ERFDA_SHIFT)) & CAN_ERFSR_ERFDA_MASK)

#define CAN_ERFSR_ERFWMI_MASK                (0x20000000U)
#define CAN_ERFSR_ERFWMI_SHIFT               (29U)
#define CAN_ERFSR_ERFWMI_WIDTH               (1U)
#define CAN_ERFSR_ERFWMI(x)                  (((uint32_t)(((uint32_t)(x)) << CAN_ERFSR_ERFWMI_SHIFT)) & CAN_ERFSR_ERFWMI_MASK)

#define CAN_ERFSR_ERFOVF_MASK                (0x40000000U)
#define CAN_ERFSR_ERFOVF_SHIFT               (30U)
#define CAN_ERFSR_ERFOVF_WIDTH               (1U)
#define CAN_ERFSR_ERFOVF(x)                  (((uint32_t)(((uint32_t)(x)) << CAN_ERFSR_ERFOVF_SHIFT)) & CAN_ERFSR_ERFOVF_MASK)

#define CAN_ERFSR_ERFUFW_MASK                (0x80000000U)
#define CAN_ERFSR_ERFUFW_SHIFT               (31U)
#define CAN_ERFSR_ERFUFW_WIDTH               (1U)
#define CAN_ERFSR_ERFUFW(x)                  (((uint32_t)(((uint32_t)(x)) << CAN_ERFSR_ERFUFW_SHIFT)) & CAN_ERFSR_ERFUFW_MASK)

/** @name HR_TIME_STAMP - High Resolution Time Stamp */
/** @{ */

#define CAN_HR_TIME_STAMP_TS_MASK            (0xFFFFFFFFU)
#define CAN_HR_TIME_STAMP_TS_SHIFT           (0U)
#define CAN_HR_TIME_STAMP_TS_WIDTH           (32U)
#define CAN_HR_TIME_STAMP_TS(x)              (((uint32_t)(((uint32_t)(x)) << CAN_HR_TIME_STAMP_TS_SHIFT)) & CAN_HR_TIME_STAMP_TS_MASK)
/**
 * @}
 */

/** @name ERFFEL - Enhanced Rx FIFO Filter Element */
/** @{ */

#define CAN_ERFFEL_FEL_MASK                  (0xFFFFFFFFU)
#define CAN_ERFFEL_FEL_SHIFT                 (0U)
#define CAN_ERFFEL_FEL_WIDTH                 (32U)
#define CAN_ERFFEL_FEL(x)                    (((uint32_t)(((uint32_t)(x)) << CAN_ERFFEL_FEL_SHIFT)) & CAN_ERFFEL_FEL_MASK)
/**
 * @}
 */

/**
 * @}
 */ /* end of group FLEXCAN_Register_Masks */

/**
 * @}
 */ /* end of group FLEXCAN_Peripheral_Access_Layer */

/**
 * @}
 */ /* end of group Peripheral_Access_Layer_EAM2011 */

// clang-format on

#endif /* __FLEXCAN_REGISTER_H__ */
