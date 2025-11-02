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
 * @file i2c_register.h
 * @brief I2C driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __I2C_REGISTER_H__
#define __I2C_REGISTER_H__

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
   -- I2C Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup I2C_Peripheral_Access_Layer I2C Peripheral Access Layer
 * @{
 */

/**
 * @brief I2C Register Layout Typedef
 */
typedef struct {
  __I  uint32_t VERID;                             /**< Version ID Register, offset: 0x0 */
  __I  uint32_t PARAM;                             /**< Parameter Register, offset: 0x4 */
       uint8_t RESERVED_0[8];
  __IO uint32_t MCR;                               /**< Master Control Register, offset: 0x10 */
  __IO uint32_t MSR;                               /**< Master Status Register, offset: 0x14 */
  __IO uint32_t MIER;                              /**< Master Interrupt Enable Register, offset: 0x18 */
  __IO uint32_t MDER;                              /**< Master PDMA Enable Register, offset: 0x1C */
  __IO uint32_t MCFGR0;                            /**< Master Configuration Register 0, offset: 0x20 */
  __IO uint32_t MCFGR1;                            /**< Master Configuration Register 1, offset: 0x24 */
  __IO uint32_t MCFGR2;                            /**< Master Configuration Register 2, offset: 0x28 */
  __IO uint32_t MCFGR3;                            /**< Master Configuration Register 3, offset: 0x2C */
       uint8_t RESERVED_1[16];
  __IO uint32_t MDMR;                              /**< Master Data Match Register, offset: 0x40 */
       uint8_t RESERVED_2[4];
  __IO uint32_t MCCR0;                             /**< Master Clock Configuration Register 0, offset: 0x48 */
       uint8_t RESERVED_3[4];
  __IO uint32_t MCCR1;                             /**< Master Clock Configuration Register 1, offset: 0x50 */
       uint8_t RESERVED_4[4];
  __IO uint32_t MFCR;                              /**< Master FIFO Control Register, offset: 0x58 */
  __I  uint32_t MFSR;                              /**< Master FIFO Status Register, offset: 0x5C */
  __O  uint32_t MTDR;                              /**< Master Transmit Data Register, offset: 0x60 */
       uint8_t RESERVED_5[12];
  __I  uint32_t MRDR;                              /**< Master Receive Data Register, offset: 0x70 */
       uint8_t RESERVED_6[156];
  __IO uint32_t SCR;                               /**< Slave Control Register, offset: 0x110 */
  __IO uint32_t SSR;                               /**< Slave Status Register, offset: 0x114 */
  __IO uint32_t SIER;                              /**< Slave Interrupt Enable Register, offset: 0x118 */
  __IO uint32_t SDER;                              /**< Slave PDMA Enable Register, offset: 0x11C */
       uint8_t RESERVED_7[4];
  __IO uint32_t SCFGR1;                            /**< Slave Configuration Register 1, offset: 0x124 */
  __IO uint32_t SCFGR2;                            /**< Slave Configuration Register 2, offset: 0x128 */
       uint8_t RESERVED_8[20];
  __IO uint32_t SAMR;                              /**< Slave Address Match Register, offset: 0x140 */
       uint8_t RESERVED_9[12];
  __I  uint32_t SASR;                              /**< Slave Address Status Register, offset: 0x150 */
  __IO uint32_t STAR;                              /**< Slave Transmit ACK Register, offset: 0x154 */
  __IO uint32_t SFCR;                              /**< Slave FIFO Control Register, offset: 0x158 */
  __IO uint32_t SFSR;                              /**< Slave FIFO Status Register, offset: 0x15C */
  __O  uint32_t STDR;                              /**< Slave Transmit Data Register, offset: 0x160 */
       uint8_t RESERVED_10[12];
  __I  uint32_t SRDR;                              /**< Slave Receive Data Register, offset: 0x170 */
} i2c_type_t, *i2c_mem_map_ptr_t;

/** Number of instances of the I2C module. */
#define I2C_INSTANCE_COUNT                     (2U)


/** I2C - Peripheral instances define. */
#define INST_I2C_I2C_0                         (0)
#define INST_I2C_I2C_1                         (1)
/** Peripheral I2C0 base pointer. */
#define I2C0                                   ((i2c_type_t *)I2C_0_BASEADDR)
#define I2C1                                   ((i2c_type_t *)I2C_1_BASEADDR)
/** Array initializer of I2C peripheral base addresses. */
#define I2C_BASE_ADDRS                         { I2C_0_BASEADDR,I2C_1_BASEADDR }
/** Array initializer of I2C peripheral base pointers. */
#define I2C_BASE_PTRS                          { I2C0,I2C1 }
/** Number of interrupt vector arrays for the I2C module. */
#define I2C_IRQS_ARR_COUNT                     (2U)
/** Number of interrupt channels for the MASTER type of I2C module. */
#define I2C_MASTER_IRQS_CH_COUNT               (1U)
/** Number of interrupt channels for the SLAVE type of I2C module. */
#define I2C_SLAVE_IRQS_CH_COUNT                (1U)
/** Interrupt vectors for the I2C peripheral type. */
#define I2C_MASTER_IRQS                        { I2C0_Master_IRQn ,I2C1_Master_IRQn }
#define I2C_SLAVE_IRQS                         { I2C0_Slave_IRQn, I2C1_Slave_IRQn }

/* ----------------------------------------------------------------------------
   -- I2C Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup I2C_Register_Masks I2C Register Masks
 * @{
 */

/** VERID Bit Fields */
#define I2C_VERID_FEATURE_MASK                 0xFFFFU
#define I2C_VERID_FEATURE_SHIFT                0U
#define I2C_VERID_FEATURE_WIDTH                16U
#define I2C_VERID_FEATURE(x)                   (((uint32_t)(((uint32_t)(x))<<I2C_VERID_FEATURE_SHIFT))&I2C_VERID_FEATURE_MASK)
#define I2C_VERID_MINOR_MASK                   0xFF0000U
#define I2C_VERID_MINOR_SHIFT                  16U
#define I2C_VERID_MINOR_WIDTH                  8U
#define I2C_VERID_MINOR(x)                     (((uint32_t)(((uint32_t)(x))<<I2C_VERID_MINOR_SHIFT))&I2C_VERID_MINOR_MASK)
#define I2C_VERID_MAJOR_MASK                   0xFF000000U
#define I2C_VERID_MAJOR_SHIFT                  24U
#define I2C_VERID_MAJOR_WIDTH                  8U
#define I2C_VERID_MAJOR(x)                     (((uint32_t)(((uint32_t)(x))<<I2C_VERID_MAJOR_SHIFT))&I2C_VERID_MAJOR_MASK)
/** PARAM Bit Fields */
#define I2C_PARAM_MTXFIFO_MASK                 0xFU
#define I2C_PARAM_MTXFIFO_SHIFT                0U
#define I2C_PARAM_MTXFIFO_WIDTH                4U
#define I2C_PARAM_MTXFIFO(x)                   (((uint32_t)(((uint32_t)(x))<<I2C_PARAM_MTXFIFO_SHIFT))&I2C_PARAM_MTXFIFO_MASK)
#define I2C_PARAM_MRXFIFO_MASK                 0xF00U
#define I2C_PARAM_MRXFIFO_SHIFT                8U
#define I2C_PARAM_MRXFIFO_WIDTH                4U
#define I2C_PARAM_MRXFIFO(x)                   (((uint32_t)(((uint32_t)(x))<<I2C_PARAM_MRXFIFO_SHIFT))&I2C_PARAM_MRXFIFO_MASK)
/** MCR Bit Fields */
#define I2C_MCR_MEN_MASK                       0x1U
#define I2C_MCR_MEN_SHIFT                      0U
#define I2C_MCR_MEN_WIDTH                      1U
#define I2C_MCR_MEN(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_MCR_MEN_SHIFT))&I2C_MCR_MEN_MASK)
#define I2C_MCR_RST_MASK                       0x2U
#define I2C_MCR_RST_SHIFT                      1U
#define I2C_MCR_RST_WIDTH                      1U
#define I2C_MCR_RST(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_MCR_RST_SHIFT))&I2C_MCR_RST_MASK)
#define I2C_MCR_DOZEN_MASK                     0x4U
#define I2C_MCR_DOZEN_SHIFT                    2U
#define I2C_MCR_DOZEN_WIDTH                    1U
#define I2C_MCR_DOZEN(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_MCR_DOZEN_SHIFT))&I2C_MCR_DOZEN_MASK)
#define I2C_MCR_DBGEN_MASK                     0x8U
#define I2C_MCR_DBGEN_SHIFT                    3U
#define I2C_MCR_DBGEN_WIDTH                    1U
#define I2C_MCR_DBGEN(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_MCR_DBGEN_SHIFT))&I2C_MCR_DBGEN_MASK)
#define I2C_MCR_RTF_MASK                       0x100U
#define I2C_MCR_RTF_SHIFT                      8U
#define I2C_MCR_RTF_WIDTH                      1U
#define I2C_MCR_RTF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_MCR_RTF_SHIFT))&I2C_MCR_RTF_MASK)
#define I2C_MCR_RRF_MASK                       0x200U
#define I2C_MCR_RRF_SHIFT                      9U
#define I2C_MCR_RRF_WIDTH                      1U
#define I2C_MCR_RRF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_MCR_RRF_SHIFT))&I2C_MCR_RRF_MASK)
/** MSR Bit Fields */
#define I2C_MSR_TDF_MASK                       0x1U
#define I2C_MSR_TDF_SHIFT                      0U
#define I2C_MSR_TDF_WIDTH                      1U
#define I2C_MSR_TDF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_MSR_TDF_SHIFT))&I2C_MSR_TDF_MASK)
#define I2C_MSR_RDF_MASK                       0x2U
#define I2C_MSR_RDF_SHIFT                      1U
#define I2C_MSR_RDF_WIDTH                      1U
#define I2C_MSR_RDF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_MSR_RDF_SHIFT))&I2C_MSR_RDF_MASK)
#define I2C_MSR_EPF_MASK                       0x100U
#define I2C_MSR_EPF_SHIFT                      8U
#define I2C_MSR_EPF_WIDTH                      1U
#define I2C_MSR_EPF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_MSR_EPF_SHIFT))&I2C_MSR_EPF_MASK)
#define I2C_MSR_SDF_MASK                       0x200U
#define I2C_MSR_SDF_SHIFT                      9U
#define I2C_MSR_SDF_WIDTH                      1U
#define I2C_MSR_SDF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_MSR_SDF_SHIFT))&I2C_MSR_SDF_MASK)
#define I2C_MSR_NDF_MASK                       0x400U
#define I2C_MSR_NDF_SHIFT                      10U
#define I2C_MSR_NDF_WIDTH                      1U
#define I2C_MSR_NDF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_MSR_NDF_SHIFT))&I2C_MSR_NDF_MASK)
#define I2C_MSR_ALF_MASK                       0x800U
#define I2C_MSR_ALF_SHIFT                      11U
#define I2C_MSR_ALF_WIDTH                      1U
#define I2C_MSR_ALF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_MSR_ALF_SHIFT))&I2C_MSR_ALF_MASK)
#define I2C_MSR_FEF_MASK                       0x1000U
#define I2C_MSR_FEF_SHIFT                      12U
#define I2C_MSR_FEF_WIDTH                      1U
#define I2C_MSR_FEF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_MSR_FEF_SHIFT))&I2C_MSR_FEF_MASK)
#define I2C_MSR_PLTF_MASK                      0x2000U
#define I2C_MSR_PLTF_SHIFT                     13U
#define I2C_MSR_PLTF_WIDTH                     1U
#define I2C_MSR_PLTF(x)                        (((uint32_t)(((uint32_t)(x))<<I2C_MSR_PLTF_SHIFT))&I2C_MSR_PLTF_MASK)
#define I2C_MSR_DMF_MASK                       0x4000U
#define I2C_MSR_DMF_SHIFT                      14U
#define I2C_MSR_DMF_WIDTH                      1U
#define I2C_MSR_DMF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_MSR_DMF_SHIFT))&I2C_MSR_DMF_MASK)
#define I2C_MSR_MBF_MASK                       0x1000000U
#define I2C_MSR_MBF_SHIFT                      24U
#define I2C_MSR_MBF_WIDTH                      1U
#define I2C_MSR_MBF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_MSR_MBF_SHIFT))&I2C_MSR_MBF_MASK)
#define I2C_MSR_BBF_MASK                       0x2000000U
#define I2C_MSR_BBF_SHIFT                      25U
#define I2C_MSR_BBF_WIDTH                      1U
#define I2C_MSR_BBF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_MSR_BBF_SHIFT))&I2C_MSR_BBF_MASK)
/** MIER Bit Fields */
#define I2C_MIER_TDIE_MASK                     0x1U
#define I2C_MIER_TDIE_SHIFT                    0U
#define I2C_MIER_TDIE_WIDTH                    1U
#define I2C_MIER_TDIE(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_MIER_TDIE_SHIFT))&I2C_MIER_TDIE_MASK)
#define I2C_MIER_RDIE_MASK                     0x2U
#define I2C_MIER_RDIE_SHIFT                    1U
#define I2C_MIER_RDIE_WIDTH                    1U
#define I2C_MIER_RDIE(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_MIER_RDIE_SHIFT))&I2C_MIER_RDIE_MASK)
#define I2C_MIER_EPIE_MASK                     0x100U
#define I2C_MIER_EPIE_SHIFT                    8U
#define I2C_MIER_EPIE_WIDTH                    1U
#define I2C_MIER_EPIE(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_MIER_EPIE_SHIFT))&I2C_MIER_EPIE_MASK)
#define I2C_MIER_SDIE_MASK                     0x200U
#define I2C_MIER_SDIE_SHIFT                    9U
#define I2C_MIER_SDIE_WIDTH                    1U
#define I2C_MIER_SDIE(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_MIER_SDIE_SHIFT))&I2C_MIER_SDIE_MASK)
#define I2C_MIER_NDIE_MASK                     0x400U
#define I2C_MIER_NDIE_SHIFT                    10U
#define I2C_MIER_NDIE_WIDTH                    1U
#define I2C_MIER_NDIE(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_MIER_NDIE_SHIFT))&I2C_MIER_NDIE_MASK)
#define I2C_MIER_ALIE_MASK                     0x800U
#define I2C_MIER_ALIE_SHIFT                    11U
#define I2C_MIER_ALIE_WIDTH                    1U
#define I2C_MIER_ALIE(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_MIER_ALIE_SHIFT))&I2C_MIER_ALIE_MASK)
#define I2C_MIER_FEIE_MASK                     0x1000U
#define I2C_MIER_FEIE_SHIFT                    12U
#define I2C_MIER_FEIE_WIDTH                    1U
#define I2C_MIER_FEIE(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_MIER_FEIE_SHIFT))&I2C_MIER_FEIE_MASK)
#define I2C_MIER_PLTIE_MASK                    0x2000U
#define I2C_MIER_PLTIE_SHIFT                   13U
#define I2C_MIER_PLTIE_WIDTH                   1U
#define I2C_MIER_PLTIE(x)                      (((uint32_t)(((uint32_t)(x))<<I2C_MIER_PLTIE_SHIFT))&I2C_MIER_PLTIE_MASK)
#define I2C_MIER_DMIE_MASK                     0x4000U
#define I2C_MIER_DMIE_SHIFT                    14U
#define I2C_MIER_DMIE_WIDTH                    1U
#define I2C_MIER_DMIE(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_MIER_DMIE_SHIFT))&I2C_MIER_DMIE_MASK)
/** MDER Bit Fields */
#define I2C_MDER_TDDE_MASK                     0x1U
#define I2C_MDER_TDDE_SHIFT                    0U
#define I2C_MDER_TDDE_WIDTH                    1U
#define I2C_MDER_TDDE(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_MDER_TDDE_SHIFT))&I2C_MDER_TDDE_MASK)
#define I2C_MDER_RDDE_MASK                     0x2U
#define I2C_MDER_RDDE_SHIFT                    1U
#define I2C_MDER_RDDE_WIDTH                    1U
#define I2C_MDER_RDDE(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_MDER_RDDE_SHIFT))&I2C_MDER_RDDE_MASK)
/** MCFGR0 Bit Fields */
#define I2C_MCFGR0_HREN_MASK                   0x1U
#define I2C_MCFGR0_HREN_SHIFT                  0U
#define I2C_MCFGR0_HREN_WIDTH                  1U
#define I2C_MCFGR0_HREN(x)                     (((uint32_t)(((uint32_t)(x))<<I2C_MCFGR0_HREN_SHIFT))&I2C_MCFGR0_HREN_MASK)
#define I2C_MCFGR0_HRPOL_MASK                  0x2U
#define I2C_MCFGR0_HRPOL_SHIFT                 1U
#define I2C_MCFGR0_HRPOL_WIDTH                 1U
#define I2C_MCFGR0_HRPOL(x)                    (((uint32_t)(((uint32_t)(x))<<I2C_MCFGR0_HRPOL_SHIFT))&I2C_MCFGR0_HRPOL_MASK)
#define I2C_MCFGR0_HRSEL_MASK                  0x4U
#define I2C_MCFGR0_HRSEL_SHIFT                 2U
#define I2C_MCFGR0_HRSEL_WIDTH                 1U
#define I2C_MCFGR0_HRSEL(x)                    (((uint32_t)(((uint32_t)(x))<<I2C_MCFGR0_HRSEL_SHIFT))&I2C_MCFGR0_HRSEL_MASK)
#define I2C_MCFGR0_CIRFIFO_MASK                0x100U
#define I2C_MCFGR0_CIRFIFO_SHIFT               8U
#define I2C_MCFGR0_CIRFIFO_WIDTH               1U
#define I2C_MCFGR0_CIRFIFO(x)                  (((uint32_t)(((uint32_t)(x))<<I2C_MCFGR0_CIRFIFO_SHIFT))&I2C_MCFGR0_CIRFIFO_MASK)
#define I2C_MCFGR0_RDMO_MASK                   0x200U
#define I2C_MCFGR0_RDMO_SHIFT                  9U
#define I2C_MCFGR0_RDMO_WIDTH                  1U
#define I2C_MCFGR0_RDMO(x)                     (((uint32_t)(((uint32_t)(x))<<I2C_MCFGR0_RDMO_SHIFT))&I2C_MCFGR0_RDMO_MASK)
/** MCFGR1 Bit Fields */
#define I2C_MCFGR1_PRESCALE_MASK               0x7U
#define I2C_MCFGR1_PRESCALE_SHIFT              0U
#define I2C_MCFGR1_PRESCALE_WIDTH              3U
#define I2C_MCFGR1_PRESCALE(x)                 (((uint32_t)(((uint32_t)(x))<<I2C_MCFGR1_PRESCALE_SHIFT))&I2C_MCFGR1_PRESCALE_MASK)
#define I2C_MCFGR1_AUTOSTOP_MASK               0x100U
#define I2C_MCFGR1_AUTOSTOP_SHIFT              8U
#define I2C_MCFGR1_AUTOSTOP_WIDTH              1U
#define I2C_MCFGR1_AUTOSTOP(x)                 (((uint32_t)(((uint32_t)(x))<<I2C_MCFGR1_AUTOSTOP_SHIFT))&I2C_MCFGR1_AUTOSTOP_MASK)
#define I2C_MCFGR1_IGNACK_MASK                 0x200U
#define I2C_MCFGR1_IGNACK_SHIFT                9U
#define I2C_MCFGR1_IGNACK_WIDTH                1U
#define I2C_MCFGR1_IGNACK(x)                   (((uint32_t)(((uint32_t)(x))<<I2C_MCFGR1_IGNACK_SHIFT))&I2C_MCFGR1_IGNACK_MASK)
#define I2C_MCFGR1_TIMECFG_MASK                0x400U
#define I2C_MCFGR1_TIMECFG_SHIFT               10U
#define I2C_MCFGR1_TIMECFG_WIDTH               1U
#define I2C_MCFGR1_TIMECFG(x)                  (((uint32_t)(((uint32_t)(x))<<I2C_MCFGR1_TIMECFG_SHIFT))&I2C_MCFGR1_TIMECFG_MASK)
#define I2C_MCFGR1_MATCFG_MASK                 0x70000U
#define I2C_MCFGR1_MATCFG_SHIFT                16U
#define I2C_MCFGR1_MATCFG_WIDTH                3U
#define I2C_MCFGR1_MATCFG(x)                   (((uint32_t)(((uint32_t)(x))<<I2C_MCFGR1_MATCFG_SHIFT))&I2C_MCFGR1_MATCFG_MASK)
#define I2C_MCFGR1_PINCFG_MASK                 0x7000000U
#define I2C_MCFGR1_PINCFG_SHIFT                24U
#define I2C_MCFGR1_PINCFG_WIDTH                3U
#define I2C_MCFGR1_PINCFG(x)                   (((uint32_t)(((uint32_t)(x))<<I2C_MCFGR1_PINCFG_SHIFT))&I2C_MCFGR1_PINCFG_MASK)
/** MCFGR2 Bit Fields */
#define I2C_MCFGR2_BUSIDLE_MASK                0xFFFU
#define I2C_MCFGR2_BUSIDLE_SHIFT               0U
#define I2C_MCFGR2_BUSIDLE_WIDTH               12U
#define I2C_MCFGR2_BUSIDLE(x)                  (((uint32_t)(((uint32_t)(x))<<I2C_MCFGR2_BUSIDLE_SHIFT))&I2C_MCFGR2_BUSIDLE_MASK)
#define I2C_MCFGR2_FILTSCL_MASK                0xF0000U
#define I2C_MCFGR2_FILTSCL_SHIFT               16U
#define I2C_MCFGR2_FILTSCL_WIDTH               4U
#define I2C_MCFGR2_FILTSCL(x)                  (((uint32_t)(((uint32_t)(x))<<I2C_MCFGR2_FILTSCL_SHIFT))&I2C_MCFGR2_FILTSCL_MASK)
#define I2C_MCFGR2_FILTSDA_MASK                0xF000000U
#define I2C_MCFGR2_FILTSDA_SHIFT               24U
#define I2C_MCFGR2_FILTSDA_WIDTH               4U
#define I2C_MCFGR2_FILTSDA(x)                  (((uint32_t)(((uint32_t)(x))<<I2C_MCFGR2_FILTSDA_SHIFT))&I2C_MCFGR2_FILTSDA_MASK)
/** MCFGR3 Bit Fields */
#define I2C_MCFGR3_PINLOW_MASK                 0xFFF00U
#define I2C_MCFGR3_PINLOW_SHIFT                8U
#define I2C_MCFGR3_PINLOW_WIDTH                12U
#define I2C_MCFGR3_PINLOW(x)                   (((uint32_t)(((uint32_t)(x))<<I2C_MCFGR3_PINLOW_SHIFT))&I2C_MCFGR3_PINLOW_MASK)
/** MDMR Bit Fields */
#define I2C_MDMR_MATCH0_MASK                   0xFFU
#define I2C_MDMR_MATCH0_SHIFT                  0U
#define I2C_MDMR_MATCH0_WIDTH                  8U
#define I2C_MDMR_MATCH0(x)                     (((uint32_t)(((uint32_t)(x))<<I2C_MDMR_MATCH0_SHIFT))&I2C_MDMR_MATCH0_MASK)
#define I2C_MDMR_MATCH1_MASK                   0xFF0000U
#define I2C_MDMR_MATCH1_SHIFT                  16U
#define I2C_MDMR_MATCH1_WIDTH                  8U
#define I2C_MDMR_MATCH1(x)                     (((uint32_t)(((uint32_t)(x))<<I2C_MDMR_MATCH1_SHIFT))&I2C_MDMR_MATCH1_MASK)
/** MCCR0 Bit Fields */
#define I2C_MCCR0_CLKLO_MASK                   0x3FU
#define I2C_MCCR0_CLKLO_SHIFT                  0U
#define I2C_MCCR0_CLKLO_WIDTH                  6U
#define I2C_MCCR0_CLKLO(x)                     (((uint32_t)(((uint32_t)(x))<<I2C_MCCR0_CLKLO_SHIFT))&I2C_MCCR0_CLKLO_MASK)
#define I2C_MCCR0_CLKHI_MASK                   0x3F00U
#define I2C_MCCR0_CLKHI_SHIFT                  8U
#define I2C_MCCR0_CLKHI_WIDTH                  6U
#define I2C_MCCR0_CLKHI(x)                     (((uint32_t)(((uint32_t)(x))<<I2C_MCCR0_CLKHI_SHIFT))&I2C_MCCR0_CLKHI_MASK)
#define I2C_MCCR0_SETHOLD_MASK                 0x3F0000U
#define I2C_MCCR0_SETHOLD_SHIFT                16U
#define I2C_MCCR0_SETHOLD_WIDTH                6U
#define I2C_MCCR0_SETHOLD(x)                   (((uint32_t)(((uint32_t)(x))<<I2C_MCCR0_SETHOLD_SHIFT))&I2C_MCCR0_SETHOLD_MASK)
#define I2C_MCCR0_DATAVD_MASK                  0x3F000000U
#define I2C_MCCR0_DATAVD_SHIFT                 24U
#define I2C_MCCR0_DATAVD_WIDTH                 6U
#define I2C_MCCR0_DATAVD(x)                    (((uint32_t)(((uint32_t)(x))<<I2C_MCCR0_DATAVD_SHIFT))&I2C_MCCR0_DATAVD_MASK)
/** MCCR1 Bit Fields */
#define I2C_MCCR1_CLKLO_MASK                   0x3FU
#define I2C_MCCR1_CLKLO_SHIFT                  0U
#define I2C_MCCR1_CLKLO_WIDTH                  6U
#define I2C_MCCR1_CLKLO(x)                     (((uint32_t)(((uint32_t)(x))<<I2C_MCCR1_CLKLO_SHIFT))&I2C_MCCR1_CLKLO_MASK)
#define I2C_MCCR1_CLKHI_MASK                   0x3F00U
#define I2C_MCCR1_CLKHI_SHIFT                  8U
#define I2C_MCCR1_CLKHI_WIDTH                  6U
#define I2C_MCCR1_CLKHI(x)                     (((uint32_t)(((uint32_t)(x))<<I2C_MCCR1_CLKHI_SHIFT))&I2C_MCCR1_CLKHI_MASK)
#define I2C_MCCR1_SETHOLD_MASK                 0x3F0000U
#define I2C_MCCR1_SETHOLD_SHIFT                16U
#define I2C_MCCR1_SETHOLD_WIDTH                6U
#define I2C_MCCR1_SETHOLD(x)                   (((uint32_t)(((uint32_t)(x))<<I2C_MCCR1_SETHOLD_SHIFT))&I2C_MCCR1_SETHOLD_MASK)
#define I2C_MCCR1_DATAVD_MASK                  0x3F000000U
#define I2C_MCCR1_DATAVD_SHIFT                 24U
#define I2C_MCCR1_DATAVD_WIDTH                 6U
#define I2C_MCCR1_DATAVD(x)                    (((uint32_t)(((uint32_t)(x))<<I2C_MCCR1_DATAVD_SHIFT))&I2C_MCCR1_DATAVD_MASK)
/** MFCR Bit Fields */
#define I2C_MFCR_TXWATER_MASK                  0xFU
#define I2C_MFCR_TXWATER_SHIFT                 0U
#define I2C_MFCR_TXWATER_WIDTH                 4U
#define I2C_MFCR_TXWATER(x)                    (((uint32_t)(((uint32_t)(x))<<I2C_MFCR_TXWATER_SHIFT))&I2C_MFCR_TXWATER_MASK)
#define I2C_MFCR_RXWATER_MASK                  0x1F0000U
#define I2C_MFCR_RXWATER_SHIFT                 16U
#define I2C_MFCR_RXWATER_WIDTH                 5U
#define I2C_MFCR_RXWATER(x)                    (((uint32_t)(((uint32_t)(x))<<I2C_MFCR_RXWATER_SHIFT))&I2C_MFCR_RXWATER_MASK)
/** MFSR Bit Fields */
#define I2C_MFSR_TXCOUNT_MASK                  0xFU
#define I2C_MFSR_TXCOUNT_SHIFT                 0U
#define I2C_MFSR_TXCOUNT_WIDTH                 4U
#define I2C_MFSR_TXCOUNT(x)                    (((uint32_t)(((uint32_t)(x))<<I2C_MFSR_TXCOUNT_SHIFT))&I2C_MFSR_TXCOUNT_MASK)
#define I2C_MFSR_RXCOUNT_MASK                  0x1F0000U
#define I2C_MFSR_RXCOUNT_SHIFT                 16U
#define I2C_MFSR_RXCOUNT_WIDTH                 5U
#define I2C_MFSR_RXCOUNT(x)                    (((uint32_t)(((uint32_t)(x))<<I2C_MFSR_RXCOUNT_SHIFT))&I2C_MFSR_RXCOUNT_MASK)
/** MTDR Bit Fields */
#define I2C_MTDR_DATA_MASK                     0xFFU
#define I2C_MTDR_DATA_SHIFT                    0U
#define I2C_MTDR_DATA_WIDTH                    8U
#define I2C_MTDR_DATA(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_MTDR_DATA_SHIFT))&I2C_MTDR_DATA_MASK)
#define I2C_MTDR_CMD_MASK                      0x700U
#define I2C_MTDR_CMD_SHIFT                     8U
#define I2C_MTDR_CMD_WIDTH                     3U
#define I2C_MTDR_CMD(x)                        (((uint32_t)(((uint32_t)(x))<<I2C_MTDR_CMD_SHIFT))&I2C_MTDR_CMD_MASK)
/** MRDR Bit Fields */
#define I2C_MRDR_DATA_MASK                     0xFFU
#define I2C_MRDR_DATA_SHIFT                    0U
#define I2C_MRDR_DATA_WIDTH                    8U
#define I2C_MRDR_DATA(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_MRDR_DATA_SHIFT))&I2C_MRDR_DATA_MASK)
#define I2C_MRDR_RXEMPTY_MASK                  0x4000U
#define I2C_MRDR_RXEMPTY_SHIFT                 14U
#define I2C_MRDR_RXEMPTY_WIDTH                 1U
#define I2C_MRDR_RXEMPTY(x)                    (((uint32_t)(((uint32_t)(x))<<I2C_MRDR_RXEMPTY_SHIFT))&I2C_MRDR_RXEMPTY_MASK)
/** SCR Bit Fields */
#define I2C_SCR_SEN_MASK                       0x1U
#define I2C_SCR_SEN_SHIFT                      0U
#define I2C_SCR_SEN_WIDTH                      1U
#define I2C_SCR_SEN(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_SCR_SEN_SHIFT))&I2C_SCR_SEN_MASK)
#define I2C_SCR_RST_MASK                       0x2U
#define I2C_SCR_RST_SHIFT                      1U
#define I2C_SCR_RST_WIDTH                      1U
#define I2C_SCR_RST(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_SCR_RST_SHIFT))&I2C_SCR_RST_MASK)
#define I2C_SCR_FILTEN_MASK                    0x10U
#define I2C_SCR_FILTEN_SHIFT                   4U
#define I2C_SCR_FILTEN_WIDTH                   1U
#define I2C_SCR_FILTEN(x)                      (((uint32_t)(((uint32_t)(x))<<I2C_SCR_FILTEN_SHIFT))&I2C_SCR_FILTEN_MASK)
#define I2C_SCR_FILTDZ_MASK                    0x20U
#define I2C_SCR_FILTDZ_SHIFT                   5U
#define I2C_SCR_FILTDZ_WIDTH                   1U
#define I2C_SCR_FILTDZ(x)                      (((uint32_t)(((uint32_t)(x))<<I2C_SCR_FILTDZ_SHIFT))&I2C_SCR_FILTDZ_MASK)
#define I2C_SCR_RTF_MASK                       0x100U
#define I2C_SCR_RTF_SHIFT                      8U
#define I2C_SCR_RTF_WIDTH                      1U
#define I2C_SCR_RTF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_SCR_RTF_SHIFT))&I2C_SCR_RTF_MASK)
#define I2C_SCR_RRF_MASK                       0x200U
#define I2C_SCR_RRF_SHIFT                      9U
#define I2C_SCR_RRF_WIDTH                      1U
#define I2C_SCR_RRF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_SCR_RRF_SHIFT))&I2C_SCR_RRF_MASK)
/** SSR Bit Fields */
#define I2C_SSR_TDF_MASK                       0x1U
#define I2C_SSR_TDF_SHIFT                      0U
#define I2C_SSR_TDF_WIDTH                      1U
#define I2C_SSR_TDF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_SSR_TDF_SHIFT))&I2C_SSR_TDF_MASK)
#define I2C_SSR_RDF_MASK                       0x2U
#define I2C_SSR_RDF_SHIFT                      1U
#define I2C_SSR_RDF_WIDTH                      1U
#define I2C_SSR_RDF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_SSR_RDF_SHIFT))&I2C_SSR_RDF_MASK)
#define I2C_SSR_AVF_MASK                       0x4U
#define I2C_SSR_AVF_SHIFT                      2U
#define I2C_SSR_AVF_WIDTH                      1U
#define I2C_SSR_AVF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_SSR_AVF_SHIFT))&I2C_SSR_AVF_MASK)
#define I2C_SSR_TAF_MASK                       0x8U
#define I2C_SSR_TAF_SHIFT                      3U
#define I2C_SSR_TAF_WIDTH                      1U
#define I2C_SSR_TAF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_SSR_TAF_SHIFT))&I2C_SSR_TAF_MASK)
#define I2C_SSR_RSF_MASK                       0x100U
#define I2C_SSR_RSF_SHIFT                      8U
#define I2C_SSR_RSF_WIDTH                      1U
#define I2C_SSR_RSF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_SSR_RSF_SHIFT))&I2C_SSR_RSF_MASK)
#define I2C_SSR_SDF_MASK                       0x200U
#define I2C_SSR_SDF_SHIFT                      9U
#define I2C_SSR_SDF_WIDTH                      1U
#define I2C_SSR_SDF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_SSR_SDF_SHIFT))&I2C_SSR_SDF_MASK)
#define I2C_SSR_BEF_MASK                       0x400U
#define I2C_SSR_BEF_SHIFT                      10U
#define I2C_SSR_BEF_WIDTH                      1U
#define I2C_SSR_BEF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_SSR_BEF_SHIFT))&I2C_SSR_BEF_MASK)
#define I2C_SSR_FEF_MASK                       0x800U
#define I2C_SSR_FEF_SHIFT                      11U
#define I2C_SSR_FEF_WIDTH                      1U
#define I2C_SSR_FEF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_SSR_FEF_SHIFT))&I2C_SSR_FEF_MASK)
#define I2C_SSR_AM0F_MASK                      0x1000U
#define I2C_SSR_AM0F_SHIFT                     12U
#define I2C_SSR_AM0F_WIDTH                     1U
#define I2C_SSR_AM0F(x)                        (((uint32_t)(((uint32_t)(x))<<I2C_SSR_AM0F_SHIFT))&I2C_SSR_AM0F_MASK)
#define I2C_SSR_AM1F_MASK                      0x2000U
#define I2C_SSR_AM1F_SHIFT                     13U
#define I2C_SSR_AM1F_WIDTH                     1U
#define I2C_SSR_AM1F(x)                        (((uint32_t)(((uint32_t)(x))<<I2C_SSR_AM1F_SHIFT))&I2C_SSR_AM1F_MASK)
#define I2C_SSR_GCF_MASK                       0x4000U
#define I2C_SSR_GCF_SHIFT                      14U
#define I2C_SSR_GCF_WIDTH                      1U
#define I2C_SSR_GCF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_SSR_GCF_SHIFT))&I2C_SSR_GCF_MASK)
#define I2C_SSR_SARF_MASK                      0x8000U
#define I2C_SSR_SARF_SHIFT                     15U
#define I2C_SSR_SARF_WIDTH                     1U
#define I2C_SSR_SARF(x)                        (((uint32_t)(((uint32_t)(x))<<I2C_SSR_SARF_SHIFT))&I2C_SSR_SARF_MASK)
#define I2C_SSR_SBF_MASK                       0x1000000U
#define I2C_SSR_SBF_SHIFT                      24U
#define I2C_SSR_SBF_WIDTH                      1U
#define I2C_SSR_SBF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_SSR_SBF_SHIFT))&I2C_SSR_SBF_MASK)
#define I2C_SSR_BBF_MASK                       0x2000000U
#define I2C_SSR_BBF_SHIFT                      25U
#define I2C_SSR_BBF_WIDTH                      1U
#define I2C_SSR_BBF(x)                         (((uint32_t)(((uint32_t)(x))<<I2C_SSR_BBF_SHIFT))&I2C_SSR_BBF_MASK)
/** SIER Bit Fields */
#define I2C_SIER_TDIE_MASK                     0x1U
#define I2C_SIER_TDIE_SHIFT                    0U
#define I2C_SIER_TDIE_WIDTH                    1U
#define I2C_SIER_TDIE(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_SIER_TDIE_SHIFT))&I2C_SIER_TDIE_MASK)
#define I2C_SIER_RDIE_MASK                     0x2U
#define I2C_SIER_RDIE_SHIFT                    1U
#define I2C_SIER_RDIE_WIDTH                    1U
#define I2C_SIER_RDIE(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_SIER_RDIE_SHIFT))&I2C_SIER_RDIE_MASK)
#define I2C_SIER_AVIE_MASK                     0x4U
#define I2C_SIER_AVIE_SHIFT                    2U
#define I2C_SIER_AVIE_WIDTH                    1U
#define I2C_SIER_AVIE(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_SIER_AVIE_SHIFT))&I2C_SIER_AVIE_MASK)
#define I2C_SIER_TAIE_MASK                     0x8U
#define I2C_SIER_TAIE_SHIFT                    3U
#define I2C_SIER_TAIE_WIDTH                    1U
#define I2C_SIER_TAIE(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_SIER_TAIE_SHIFT))&I2C_SIER_TAIE_MASK)
#define I2C_SIER_RSIE_MASK                     0x100U
#define I2C_SIER_RSIE_SHIFT                    8U
#define I2C_SIER_RSIE_WIDTH                    1U
#define I2C_SIER_RSIE(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_SIER_RSIE_SHIFT))&I2C_SIER_RSIE_MASK)
#define I2C_SIER_SDIE_MASK                     0x200U
#define I2C_SIER_SDIE_SHIFT                    9U
#define I2C_SIER_SDIE_WIDTH                    1U
#define I2C_SIER_SDIE(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_SIER_SDIE_SHIFT))&I2C_SIER_SDIE_MASK)
#define I2C_SIER_BEIE_MASK                     0x400U
#define I2C_SIER_BEIE_SHIFT                    10U
#define I2C_SIER_BEIE_WIDTH                    1U
#define I2C_SIER_BEIE(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_SIER_BEIE_SHIFT))&I2C_SIER_BEIE_MASK)
#define I2C_SIER_FEIE_MASK                     0x800U
#define I2C_SIER_FEIE_SHIFT                    11U
#define I2C_SIER_FEIE_WIDTH                    1U
#define I2C_SIER_FEIE(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_SIER_FEIE_SHIFT))&I2C_SIER_FEIE_MASK)
#define I2C_SIER_AM0IE_MASK                    0x1000U
#define I2C_SIER_AM0IE_SHIFT                   12U
#define I2C_SIER_AM0IE_WIDTH                   1U
#define I2C_SIER_AM0IE(x)                      (((uint32_t)(((uint32_t)(x))<<I2C_SIER_AM0IE_SHIFT))&I2C_SIER_AM0IE_MASK)
#define I2C_SIER_AM1F_MASK                     0x2000U
#define I2C_SIER_AM1F_SHIFT                    13U
#define I2C_SIER_AM1F_WIDTH                    1U
#define I2C_SIER_AM1F(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_SIER_AM1F_SHIFT))&I2C_SIER_AM1F_MASK)
#define I2C_SIER_GCIE_MASK                     0x4000U
#define I2C_SIER_GCIE_SHIFT                    14U
#define I2C_SIER_GCIE_WIDTH                    1U
#define I2C_SIER_GCIE(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_SIER_GCIE_SHIFT))&I2C_SIER_GCIE_MASK)
#define I2C_SIER_SARIE_MASK                    0x8000U
#define I2C_SIER_SARIE_SHIFT                   15U
#define I2C_SIER_SARIE_WIDTH                   1U
#define I2C_SIER_SARIE(x)                      (((uint32_t)(((uint32_t)(x))<<I2C_SIER_SARIE_SHIFT))&I2C_SIER_SARIE_MASK)
/** SDER Bit Fields */
#define I2C_SDER_TDDE_MASK                     0x1U
#define I2C_SDER_TDDE_SHIFT                    0U
#define I2C_SDER_TDDE_WIDTH                    1U
#define I2C_SDER_TDDE(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_SDER_TDDE_SHIFT))&I2C_SDER_TDDE_MASK)
#define I2C_SDER_RDDE_MASK                     0x2U
#define I2C_SDER_RDDE_SHIFT                    1U
#define I2C_SDER_RDDE_WIDTH                    1U
#define I2C_SDER_RDDE(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_SDER_RDDE_SHIFT))&I2C_SDER_RDDE_MASK)
#define I2C_SDER_AVDE_MASK                     0x4U
#define I2C_SDER_AVDE_SHIFT                    2U
#define I2C_SDER_AVDE_WIDTH                    1U
#define I2C_SDER_AVDE(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_SDER_AVDE_SHIFT))&I2C_SDER_AVDE_MASK)
/** SCFGR1 Bit Fields */
#define I2C_SCFGR1_ADRSTALL_MASK               0x1U
#define I2C_SCFGR1_ADRSTALL_SHIFT              0U
#define I2C_SCFGR1_ADRSTALL_WIDTH              1U
#define I2C_SCFGR1_ADRSTALL(x)                 (((uint32_t)(((uint32_t)(x))<<I2C_SCFGR1_ADRSTALL_SHIFT))&I2C_SCFGR1_ADRSTALL_MASK)
#define I2C_SCFGR1_RXSTALL_MASK                0x2U
#define I2C_SCFGR1_RXSTALL_SHIFT               1U
#define I2C_SCFGR1_RXSTALL_WIDTH               1U
#define I2C_SCFGR1_RXSTALL(x)                  (((uint32_t)(((uint32_t)(x))<<I2C_SCFGR1_RXSTALL_SHIFT))&I2C_SCFGR1_RXSTALL_MASK)
#define I2C_SCFGR1_TXDSTALL_MASK               0x4U
#define I2C_SCFGR1_TXDSTALL_SHIFT              2U
#define I2C_SCFGR1_TXDSTALL_WIDTH              1U
#define I2C_SCFGR1_TXDSTALL(x)                 (((uint32_t)(((uint32_t)(x))<<I2C_SCFGR1_TXDSTALL_SHIFT))&I2C_SCFGR1_TXDSTALL_MASK)
#define I2C_SCFGR1_ACKSTALL_MASK               0x8U
#define I2C_SCFGR1_ACKSTALL_SHIFT              3U
#define I2C_SCFGR1_ACKSTALL_WIDTH              1U
#define I2C_SCFGR1_ACKSTALL(x)                 (((uint32_t)(((uint32_t)(x))<<I2C_SCFGR1_ACKSTALL_SHIFT))&I2C_SCFGR1_ACKSTALL_MASK)
#define I2C_SCFGR1_GCEN_MASK                   0x100U
#define I2C_SCFGR1_GCEN_SHIFT                  8U
#define I2C_SCFGR1_GCEN_WIDTH                  1U
#define I2C_SCFGR1_GCEN(x)                     (((uint32_t)(((uint32_t)(x))<<I2C_SCFGR1_GCEN_SHIFT))&I2C_SCFGR1_GCEN_MASK)
#define I2C_SCFGR1_SAEN_MASK                   0x200U
#define I2C_SCFGR1_SAEN_SHIFT                  9U
#define I2C_SCFGR1_SAEN_WIDTH                  1U
#define I2C_SCFGR1_SAEN(x)                     (((uint32_t)(((uint32_t)(x))<<I2C_SCFGR1_SAEN_SHIFT))&I2C_SCFGR1_SAEN_MASK)
#define I2C_SCFGR1_TXCFG_MASK                  0x400U
#define I2C_SCFGR1_TXCFG_SHIFT                 10U
#define I2C_SCFGR1_TXCFG_WIDTH                 1U
#define I2C_SCFGR1_TXCFG(x)                    (((uint32_t)(((uint32_t)(x))<<I2C_SCFGR1_TXCFG_SHIFT))&I2C_SCFGR1_TXCFG_MASK)
#define I2C_SCFGR1_RXCFG_MASK                  0x800U
#define I2C_SCFGR1_RXCFG_SHIFT                 11U
#define I2C_SCFGR1_RXCFG_WIDTH                 1U
#define I2C_SCFGR1_RXCFG(x)                    (((uint32_t)(((uint32_t)(x))<<I2C_SCFGR1_RXCFG_SHIFT))&I2C_SCFGR1_RXCFG_MASK)
#define I2C_SCFGR1_IGNACK_MASK                 0x1000U
#define I2C_SCFGR1_IGNACK_SHIFT                12U
#define I2C_SCFGR1_IGNACK_WIDTH                1U
#define I2C_SCFGR1_IGNACK(x)                   (((uint32_t)(((uint32_t)(x))<<I2C_SCFGR1_IGNACK_SHIFT))&I2C_SCFGR1_IGNACK_MASK)
#define I2C_SCFGR1_HSMEN_MASK                  0x2000U
#define I2C_SCFGR1_HSMEN_SHIFT                 13U
#define I2C_SCFGR1_HSMEN_WIDTH                 1U
#define I2C_SCFGR1_HSMEN(x)                    (((uint32_t)(((uint32_t)(x))<<I2C_SCFGR1_HSMEN_SHIFT))&I2C_SCFGR1_HSMEN_MASK)
#define I2C_SCFGR1_ADDRCFG_MASK                0x70000U
#define I2C_SCFGR1_ADDRCFG_SHIFT               16U
#define I2C_SCFGR1_ADDRCFG_WIDTH               3U
#define I2C_SCFGR1_ADDRCFG(x)                  (((uint32_t)(((uint32_t)(x))<<I2C_SCFGR1_ADDRCFG_SHIFT))&I2C_SCFGR1_ADDRCFG_MASK)
/** SCFGR2 Bit Fields */
#define I2C_SCFGR2_CLKHOLD_MASK                0xFU
#define I2C_SCFGR2_CLKHOLD_SHIFT               0U
#define I2C_SCFGR2_CLKHOLD_WIDTH               4U
#define I2C_SCFGR2_CLKHOLD(x)                  (((uint32_t)(((uint32_t)(x))<<I2C_SCFGR2_CLKHOLD_SHIFT))&I2C_SCFGR2_CLKHOLD_MASK)
#define I2C_SCFGR2_DATAVD_MASK                 0x3F00U
#define I2C_SCFGR2_DATAVD_SHIFT                8U
#define I2C_SCFGR2_DATAVD_WIDTH                6U
#define I2C_SCFGR2_DATAVD(x)                   (((uint32_t)(((uint32_t)(x))<<I2C_SCFGR2_DATAVD_SHIFT))&I2C_SCFGR2_DATAVD_MASK)
#define I2C_SCFGR2_FILTSCL_MASK                0xF0000U
#define I2C_SCFGR2_FILTSCL_SHIFT               16U
#define I2C_SCFGR2_FILTSCL_WIDTH               4U
#define I2C_SCFGR2_FILTSCL(x)                  (((uint32_t)(((uint32_t)(x))<<I2C_SCFGR2_FILTSCL_SHIFT))&I2C_SCFGR2_FILTSCL_MASK)
#define I2C_SCFGR2_FILTSDA_MASK                0xF000000U
#define I2C_SCFGR2_FILTSDA_SHIFT               24U
#define I2C_SCFGR2_FILTSDA_WIDTH               4U
#define I2C_SCFGR2_FILTSDA(x)                  (((uint32_t)(((uint32_t)(x))<<I2C_SCFGR2_FILTSDA_SHIFT))&I2C_SCFGR2_FILTSDA_MASK)
/** SAMR Bit Fields */
#define I2C_SAMR_ADDR0_MASK                    0x7FEU
#define I2C_SAMR_ADDR0_SHIFT                   1U
#define I2C_SAMR_ADDR0_WIDTH                   10U
#define I2C_SAMR_ADDR0(x)                      (((uint32_t)(((uint32_t)(x))<<I2C_SAMR_ADDR0_SHIFT))&I2C_SAMR_ADDR0_MASK)
#define I2C_SAMR_ADDR1_MASK                    0x7FE0000U
#define I2C_SAMR_ADDR1_SHIFT                   17U
#define I2C_SAMR_ADDR1_WIDTH                   10U
#define I2C_SAMR_ADDR1(x)                      (((uint32_t)(((uint32_t)(x))<<I2C_SAMR_ADDR1_SHIFT))&I2C_SAMR_ADDR1_MASK)
/** SASR Bit Fields */
#define I2C_SASR_RADDR_MASK                    0x7FFU
#define I2C_SASR_RADDR_SHIFT                   0U
#define I2C_SASR_RADDR_WIDTH                   11U
#define I2C_SASR_RADDR(x)                      (((uint32_t)(((uint32_t)(x))<<I2C_SASR_RADDR_SHIFT))&I2C_SASR_RADDR_MASK)
#define I2C_SASR_ANV_MASK                      0x4000U
#define I2C_SASR_ANV_SHIFT                     14U
#define I2C_SASR_ANV_WIDTH                     1U
#define I2C_SASR_ANV(x)                        (((uint32_t)(((uint32_t)(x))<<I2C_SASR_ANV_SHIFT))&I2C_SASR_ANV_MASK)
/** STAR Bit Fields */
#define I2C_STAR_TXNACK_MASK                   0x1U
#define I2C_STAR_TXNACK_SHIFT                  0U
#define I2C_STAR_TXNACK_WIDTH                  1U
#define I2C_STAR_TXNACK(x)                     (((uint32_t)(((uint32_t)(x))<<I2C_STAR_TXNACK_SHIFT))&I2C_STAR_TXNACK_MASK)
/** SFCR Bit Fields */
#define I2C_SFCR_RXWATER_MASK                  0x1FU
#define I2C_SFCR_RXWATER_SHIFT                 0U
#define I2C_SFCR_RXWATER_WIDTH                 5U
#define I2C_SFCR_RXWATER(x)                    (((uint32_t)(((uint32_t)(x))<<I2C_MFCR_RXWATER_SHIFT))&I2C_MFCR_RXWATER_MASK)
/** SFSR Bit Fields */
#define I2C_SFSR_RXCOUNT_MASK                  0x1FU
#define I2C_SFSR_RXCOUNT_SHIFT                 0U
#define I2C_SFSR_RXCOUNT_WIDTH                 5U
#define I2C_SFSR_RXCOUNT(x)                    (((uint32_t)(((uint32_t)(x))<<I2C_MFSR_RXCOUNT_SHIFT))&I2C_MFSR_RXCOUNT_MASK)
/** STDR Bit Fields */
#define I2C_STDR_DATA_MASK                     0xFFU
#define I2C_STDR_DATA_SHIFT                    0U
#define I2C_STDR_DATA_WIDTH                    8U
#define I2C_STDR_DATA(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_STDR_DATA_SHIFT))&I2C_STDR_DATA_MASK)
/** SRDR Bit Fields */
#define I2C_SRDR_DATA_MASK                     0xFFU
#define I2C_SRDR_DATA_SHIFT                    0U
#define I2C_SRDR_DATA_WIDTH                    8U
#define I2C_SRDR_DATA(x)                       (((uint32_t)(((uint32_t)(x))<<I2C_SRDR_DATA_SHIFT))&I2C_SRDR_DATA_MASK)
#define I2C_SRDR_RXEMPTY_MASK                  0x4000U
#define I2C_SRDR_RXEMPTY_SHIFT                 14U
#define I2C_SRDR_RXEMPTY_WIDTH                 1U
#define I2C_SRDR_RXEMPTY(x)                    (((uint32_t)(((uint32_t)(x))<<I2C_SRDR_RXEMPTY_SHIFT))&I2C_SRDR_RXEMPTY_MASK)
#define I2C_SRDR_SOF_MASK                      0x8000U
#define I2C_SRDR_SOF_SHIFT                     15U
#define I2C_SRDR_SOF_WIDTH                     1U
#define I2C_SRDR_SOF(x)                        (((uint32_t)(((uint32_t)(x))<<I2C_SRDR_SOF_SHIFT))&I2C_SRDR_SOF_MASK)

/**
 * @}
 */ /* end of group I2C_Register_Masks */

/**
 * @}
 */ /* end of group I2C_Peripheral_Access_Layer */

/**
 * @}
 */ /* end of group Peripheral_Access_Layer_EAM2011 */

// clang-format on

#endif /* __I2C_REGISTER_H__ */
