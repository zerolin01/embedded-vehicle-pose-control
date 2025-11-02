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
 * @file spi_register.h
 * @brief SPI driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:        Version:        Author:
 * Changes:
 *
 */

#ifndef __SPI_REGISTER_H__
#define __SPI_REGISTER_H__

#include "osal.h"
#include "platform.h"
#include <stddef.h>

// clang-format off

/* ----------------------------------------------------------------------------
   -- SPI Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup SPI_Peripheral_Access_Layer SPI Peripheral Access Layer
 * @{
 */

/**
 * @brief SPI Register Layout Typedef
 */
typedef struct {
  __I  uint32_t VERID;                             /**< Version ID Register, offset: 0x0 */
  __I  uint32_t PARAM;                             /**< Parameter Register, offset: 0x4 */
       uint8_t RESERVED_0[8];
  __IO uint32_t CR;                                /**< Control Register, offset: 0x10 */
  __IO uint32_t SR;                                /**< Status Register, offset: 0x14 */
  __IO uint32_t IER;                               /**< Interrupt Enable Register, offset: 0x18 */
  __IO uint32_t DER;                               /**< PDMA Enable Register, offset: 0x1C */
  __IO uint32_t CFGR0;                             /**< Configuration Register 0, offset: 0x20 */
  __IO uint32_t CFGR1;                             /**< Configuration Register 1, offset: 0x24 */
       uint8_t RESERVED_1[8];
  __IO uint32_t DMR0;                              /**< Data Match Register 0, offset: 0x30 */
  __IO uint32_t DMR1;                              /**< Data Match Register 1, offset: 0x34 */
       uint8_t RESERVED_2[8];
  __IO uint32_t CCR;                               /**< Clock Configuration Register, offset: 0x40 */
       uint8_t RESERVED_3[20];
  __IO uint32_t FCR;                               /**< FIFO Control Register, offset: 0x58 */
  __I  uint32_t FSR;                               /**< FIFO Status Register, offset: 0x5C */
  __IO uint32_t TCR;                               /**< Transmit Command Register, offset: 0x60 */
  __O  uint32_t TDR;                               /**< Transmit Data Register, offset: 0x64 */
       uint8_t RESERVED_4[8];
  __I  uint32_t RSR;                               /**< Receive Status Register, offset: 0x70 */
  __I  uint32_t RDR;                               /**< Receive Data Register, offset: 0x74 */
} spi_type_t, *spi_mem_map_ptr_t;

/* ----------------------------------------------------------------------------
   -- SPI Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup SPI_Register_Masks SPI Register Masks
 * @{
 */

/** VERID Bit Fields */
#define SPI_VERID_FEATURE_MASK                 (0xFFFFU)
#define SPI_VERID_FEATURE_SHIFT                (0U)
#define SPI_VERID_FEATURE_WIDTH                (16U)
#define SPI_VERID_FEATURE(x)                   (((uint32_t)(((uint32_t)(x))<<SPI_VERID_FEATURE_SHIFT))&SPI_VERID_FEATURE_MASK)
#define SPI_VERID_MINOR_MASK                   (0xFF0000U)
#define SPI_VERID_MINOR_SHIFT                  (16U)
#define SPI_VERID_MINOR_WIDTH                  (8U)
#define SPI_VERID_MINOR(x)                     (((uint32_t)(((uint32_t)(x))<<SPI_VERID_MINOR_SHIFT))&SPI_VERID_MINOR_MASK)
#define SPI_VERID_MAJOR_MASK                   (0xFF000000U)
#define SPI_VERID_MAJOR_SHIFT                  (24U)
#define SPI_VERID_MAJOR_WIDTH                  (8U)
#define SPI_VERID_MAJOR(x)                     (((uint32_t)(((uint32_t)(x))<<SPI_VERID_MAJOR_SHIFT))&SPI_VERID_MAJOR_MASK)
/** PARAM Bit Fields */
#define SPI_PARAM_TXFIFO_MASK                  (0xFFU)
#define SPI_PARAM_TXFIFO_SHIFT                 (0U)
#define SPI_PARAM_TXFIFO_WIDTH                 (8U)
#define SPI_PARAM_TXFIFO(x)                    (((uint32_t)(((uint32_t)(x))<<SPI_PARAM_TXFIFO_SHIFT))&SPI_PARAM_TXFIFO_MASK)
#define SPI_PARAM_RXFIFO_MASK                  (0xFF00U)
#define SPI_PARAM_RXFIFO_SHIFT                 (8U)
#define SPI_PARAM_RXFIFO_WIDTH                 (8U)
#define SPI_PARAM_RXFIFO(x)                    (((uint32_t)(((uint32_t)(x))<<SPI_PARAM_RXFIFO_SHIFT))&SPI_PARAM_RXFIFO_MASK)
/** CR Bit Fields */
#define SPI_CR_MEN_MASK                        (0x1U)
#define SPI_CR_MEN_SHIFT                       (0U)
#define SPI_CR_MEN_WIDTH                       (1U)
#define SPI_CR_MEN(x)                          (((uint32_t)(((uint32_t)(x))<<SPI_CR_MEN_SHIFT))&SPI_CR_MEN_MASK)
#define SPI_CR_RST_MASK                        (0x2U)
#define SPI_CR_RST_SHIFT                       (1U)
#define SPI_CR_RST_WIDTH                       (1U)
#define SPI_CR_RST(x)                          (((uint32_t)(((uint32_t)(x))<<SPI_CR_RST_SHIFT))&SPI_CR_RST_MASK)
#define SPI_CR_DOZEN_MASK                      (0x4U)
#define SPI_CR_DOZEN_SHIFT                     (2U)
#define SPI_CR_DOZEN_WIDTH                     (1U)
#define SPI_CR_DOZEN(x)                        (((uint32_t)(((uint32_t)(x))<<SPI_CR_DOZEN_SHIFT))&SPI_CR_DOZEN_MASK)
#define SPI_CR_DBGEN_MASK                      (0x8U)
#define SPI_CR_DBGEN_SHIFT                     (3U)
#define SPI_CR_DBGEN_WIDTH                     (1U)
#define SPI_CR_DBGEN(x)                        (((uint32_t)(((uint32_t)(x))<<SPI_CR_DBGEN_SHIFT))&SPI_CR_DBGEN_MASK)
#define SPI_CR_RTF_MASK                        (0x100U)
#define SPI_CR_RTF_SHIFT                       (8U)
#define SPI_CR_RTF_WIDTH                       (1U)
#define SPI_CR_RTF(x)                          (((uint32_t)(((uint32_t)(x))<<SPI_CR_RTF_SHIFT))&SPI_CR_RTF_MASK)
#define SPI_CR_RRF_MASK                        (0x200U)
#define SPI_CR_RRF_SHIFT                       (9U)
#define SPI_CR_RRF_WIDTH                       (1U)
#define SPI_CR_RRF(x)                          (((uint32_t)(((uint32_t)(x))<<SPI_CR_RRF_SHIFT))&SPI_CR_RRF_MASK)
/** SR Bit Fields */
#define SPI_SR_TDF_MASK                        (0x1U)
#define SPI_SR_TDF_SHIFT                       (0U)
#define SPI_SR_TDF_WIDTH                       (1U)
#define SPI_SR_TDF(x)                          (((uint32_t)(((uint32_t)(x))<<SPI_SR_TDF_SHIFT))&SPI_SR_TDF_MASK)
#define SPI_SR_RDF_MASK                        (0x2U)
#define SPI_SR_RDF_SHIFT                       (1U)
#define SPI_SR_RDF_WIDTH                       (1U)
#define SPI_SR_RDF(x)                          (((uint32_t)(((uint32_t)(x))<<SPI_SR_RDF_SHIFT))&SPI_SR_RDF_MASK)
#define SPI_SR_WCF_MASK                        (0x100U)
#define SPI_SR_WCF_SHIFT                       (8U)
#define SPI_SR_WCF_WIDTH                       (1U)
#define SPI_SR_WCF(x)                          (((uint32_t)(((uint32_t)(x))<<SPI_SR_WCF_SHIFT))&SPI_SR_WCF_MASK)
#define SPI_SR_FCF_MASK                        (0x200U)
#define SPI_SR_FCF_SHIFT                       (9U)
#define SPI_SR_FCF_WIDTH                       (1U)
#define SPI_SR_FCF(x)                          (((uint32_t)(((uint32_t)(x))<<SPI_SR_FCF_SHIFT))&SPI_SR_FCF_MASK)
#define SPI_SR_TCF_MASK                        (0x400U)
#define SPI_SR_TCF_SHIFT                       (10U)
#define SPI_SR_TCF_WIDTH                       (1U)
#define SPI_SR_TCF(x)                          (((uint32_t)(((uint32_t)(x))<<SPI_SR_TCF_SHIFT))&SPI_SR_TCF_MASK)
#define SPI_SR_TEF_MASK                        (0x800U)
#define SPI_SR_TEF_SHIFT                       (11U)
#define SPI_SR_TEF_WIDTH                       (1U)
#define SPI_SR_TEF(x)                          (((uint32_t)(((uint32_t)(x))<<SPI_SR_TEF_SHIFT))&SPI_SR_TEF_MASK)
#define SPI_SR_REF_MASK                        (0x1000U)
#define SPI_SR_REF_SHIFT                       (12U)
#define SPI_SR_REF_WIDTH                       (1U)
#define SPI_SR_REF(x)                          (((uint32_t)(((uint32_t)(x))<<SPI_SR_REF_SHIFT))&SPI_SR_REF_MASK)
#define SPI_SR_DMF_MASK                        (0x2000U)
#define SPI_SR_DMF_SHIFT                       (13U)
#define SPI_SR_DMF_WIDTH                       (1U)
#define SPI_SR_DMF(x)                          (((uint32_t)(((uint32_t)(x))<<SPI_SR_DMF_SHIFT))&SPI_SR_DMF_MASK)
#define SPI_SR_MBF_MASK                        (0x1000000U)
#define SPI_SR_MBF_SHIFT                       (24U)
#define SPI_SR_MBF_WIDTH                       (1U)
#define SPI_SR_MBF(x)                          (((uint32_t)(((uint32_t)(x))<<SPI_SR_MBF_SHIFT))&SPI_SR_MBF_MASK)
/** IER Bit Fields */
#define SPI_IER_TDIE_MASK                      (0x1U)
#define SPI_IER_TDIE_SHIFT                     (0U)
#define SPI_IER_TDIE_WIDTH                     (1U)
#define SPI_IER_TDIE(x)                        (((uint32_t)(((uint32_t)(x))<<SPI_IER_TDIE_SHIFT))&SPI_IER_TDIE_MASK)
#define SPI_IER_RDIE_MASK                      (0x2U)
#define SPI_IER_RDIE_SHIFT                     (1U)
#define SPI_IER_RDIE_WIDTH                     (1U)
#define SPI_IER_RDIE(x)                        (((uint32_t)(((uint32_t)(x))<<SPI_IER_RDIE_SHIFT))&SPI_IER_RDIE_MASK)
#define SPI_IER_WCIE_MASK                      (0x100U)
#define SPI_IER_WCIE_SHIFT                     (8U)
#define SPI_IER_WCIE_WIDTH                     (1U)
#define SPI_IER_WCIE(x)                        (((uint32_t)(((uint32_t)(x))<<SPI_IER_WCIE_SHIFT))&SPI_IER_WCIE_MASK)
#define SPI_IER_FCIE_MASK                      (0x200U)
#define SPI_IER_FCIE_SHIFT                     (9U)
#define SPI_IER_FCIE_WIDTH                     (1U)
#define SPI_IER_FCIE(x)                        (((uint32_t)(((uint32_t)(x))<<SPI_IER_FCIE_SHIFT))&SPI_IER_FCIE_MASK)
#define SPI_IER_TCIE_MASK                      (0x400U)
#define SPI_IER_TCIE_SHIFT                     (10U)
#define SPI_IER_TCIE_WIDTH                     (1U)
#define SPI_IER_TCIE(x)                        (((uint32_t)(((uint32_t)(x))<<SPI_IER_TCIE_SHIFT))&SPI_IER_TCIE_MASK)
#define SPI_IER_TEIE_MASK                      (0x800U)
#define SPI_IER_TEIE_SHIFT                     (11U)
#define SPI_IER_TEIE_WIDTH                     (1U)
#define SPI_IER_TEIE(x)                        (((uint32_t)(((uint32_t)(x))<<SPI_IER_TEIE_SHIFT))&SPI_IER_TEIE_MASK)
#define SPI_IER_REIE_MASK                      (0x1000U)
#define SPI_IER_REIE_SHIFT                     (12U)
#define SPI_IER_REIE_WIDTH                     (1U)
#define SPI_IER_REIE(x)                        (((uint32_t)(((uint32_t)(x))<<SPI_IER_REIE_SHIFT))&SPI_IER_REIE_MASK)
#define SPI_IER_DMIE_MASK                      (0x2000U)
#define SPI_IER_DMIE_SHIFT                     (13U)
#define SPI_IER_DMIE_WIDTH                     (1U)
#define SPI_IER_DMIE(x)                        (((uint32_t)(((uint32_t)(x))<<SPI_IER_DMIE_SHIFT))&SPI_IER_DMIE_MASK)
/** DER Bit Fields */
#define SPI_DER_TDDE_MASK                      (0x1U)
#define SPI_DER_TDDE_SHIFT                     (0U)
#define SPI_DER_TDDE_WIDTH                     (1U)
#define SPI_DER_TDDE(x)                        (((uint32_t)(((uint32_t)(x))<<SPI_DER_TDDE_SHIFT))&SPI_DER_TDDE_MASK)
#define SPI_DER_RDDE_MASK                      (0x2U)
#define SPI_DER_RDDE_SHIFT                     (1U)
#define SPI_DER_RDDE_WIDTH                     (1U)
#define SPI_DER_RDDE(x)                        (((uint32_t)(((uint32_t)(x))<<SPI_DER_RDDE_SHIFT))&SPI_DER_RDDE_MASK)
/** CFGR0 Bit Fields */
#define SPI_CFGR0_HREN_MASK                    (0x1U)
#define SPI_CFGR0_HREN_SHIFT                   (0U)
#define SPI_CFGR0_HREN_WIDTH                   (1U)
#define SPI_CFGR0_HREN(x)                      (((uint32_t)(((uint32_t)(x))<<SPI_CFGR0_HREN_SHIFT))&SPI_CFGR0_HREN_MASK)
#define SPI_CFGR0_HRPOL_MASK                   (0x2U)
#define SPI_CFGR0_HRPOL_SHIFT                  (1U)
#define SPI_CFGR0_HRPOL_WIDTH                  (1U)
#define SPI_CFGR0_HRPOL(x)                     (((uint32_t)(((uint32_t)(x))<<SPI_CFGR0_HRPOL_SHIFT))&SPI_CFGR0_HRPOL_MASK)
#define SPI_CFGR0_HRSEL_MASK                   (0x4U)
#define SPI_CFGR0_HRSEL_SHIFT                  (2U)
#define SPI_CFGR0_HRSEL_WIDTH                  (1U)
#define SPI_CFGR0_HRSEL(x)                     (((uint32_t)(((uint32_t)(x))<<SPI_CFGR0_HRSEL_SHIFT))&SPI_CFGR0_HRSEL_MASK)
#define SPI_CFGR0_CIRFIFO_MASK                 (0x100U)
#define SPI_CFGR0_CIRFIFO_SHIFT                (8U)
#define SPI_CFGR0_CIRFIFO_WIDTH                (1U)
#define SPI_CFGR0_CIRFIFO(x)                   (((uint32_t)(((uint32_t)(x))<<SPI_CFGR0_CIRFIFO_SHIFT))&SPI_CFGR0_CIRFIFO_MASK)
#define SPI_CFGR0_RDMO_MASK                    (0x200U)
#define SPI_CFGR0_RDMO_SHIFT                   (9U)
#define SPI_CFGR0_RDMO_WIDTH                   (1U)
#define SPI_CFGR0_RDMO(x)                      (((uint32_t)(((uint32_t)(x))<<SPI_CFGR0_RDMO_SHIFT))&SPI_CFGR0_RDMO_MASK)
/** CFGR1 Bit Fields */
#define SPI_CFGR1_MASTER_MASK                  (0x1U)
#define SPI_CFGR1_MASTER_SHIFT                 (0U)
#define SPI_CFGR1_MASTER_WIDTH                 (1U)
#define SPI_CFGR1_MASTER(x)                    (((uint32_t)(((uint32_t)(x))<<SPI_CFGR1_MASTER_SHIFT))&SPI_CFGR1_MASTER_MASK)
#define SPI_CFGR1_SAMPLE_MASK                  (0x2U)
#define SPI_CFGR1_SAMPLE_SHIFT                 (1U)
#define SPI_CFGR1_SAMPLE_WIDTH                 (1U)
#define SPI_CFGR1_SAMPLE(x)                    (((uint32_t)(((uint32_t)(x))<<SPI_CFGR1_SAMPLE_SHIFT))&SPI_CFGR1_SAMPLE_MASK)
#define SPI_CFGR1_AUTOPCS_MASK                 (0x4U)
#define SPI_CFGR1_AUTOPCS_SHIFT                (2U)
#define SPI_CFGR1_AUTOPCS_WIDTH                (1U)
#define SPI_CFGR1_AUTOPCS(x)                   (((uint32_t)(((uint32_t)(x))<<SPI_CFGR1_AUTOPCS_SHIFT))&SPI_CFGR1_AUTOPCS_MASK)
#define SPI_CFGR1_NOSTALL_MASK                 (0x8U)
#define SPI_CFGR1_NOSTALL_SHIFT                (3U)
#define SPI_CFGR1_NOSTALL_WIDTH                (1U)
#define SPI_CFGR1_NOSTALL(x)                   (((uint32_t)(((uint32_t)(x))<<SPI_CFGR1_NOSTALL_SHIFT))&SPI_CFGR1_NOSTALL_MASK)
#define SPI_CFGR1_PCSPOL_MASK                  (0xF00U)
#define SPI_CFGR1_PCSPOL_SHIFT                 (8U)
#define SPI_CFGR1_PCSPOL_WIDTH                 (4U)
#define SPI_CFGR1_PCSPOL(x)                    (((uint32_t)(((uint32_t)(x))<<SPI_CFGR1_PCSPOL_SHIFT))&SPI_CFGR1_PCSPOL_MASK)
#define SPI_CFGR1_MATCFG_MASK                  (0x70000U)
#define SPI_CFGR1_MATCFG_SHIFT                 (16U)
#define SPI_CFGR1_MATCFG_WIDTH                 (3U)
#define SPI_CFGR1_MATCFG(x)                    (((uint32_t)(((uint32_t)(x))<<SPI_CFGR1_MATCFG_SHIFT))&SPI_CFGR1_MATCFG_MASK)
#define SPI_CFGR1_PINCFG_MASK                  (0x3000000U)
#define SPI_CFGR1_PINCFG_SHIFT                 (24U)
#define SPI_CFGR1_PINCFG_WIDTH                 (2U)
#define SPI_CFGR1_PINCFG(x)                    (((uint32_t)(((uint32_t)(x))<<SPI_CFGR1_PINCFG_SHIFT))&SPI_CFGR1_PINCFG_MASK)
#define SPI_CFGR1_OUTCFG_MASK                  (0x4000000U)
#define SPI_CFGR1_OUTCFG_SHIFT                 (26U)
#define SPI_CFGR1_OUTCFG_WIDTH                 (1U)
#define SPI_CFGR1_OUTCFG(x)                    (((uint32_t)(((uint32_t)(x))<<SPI_CFGR1_OUTCFG_SHIFT))&SPI_CFGR1_OUTCFG_MASK)
#define SPI_CFGR1_PCSCFG_MASK                  (0x8000000U)
#define SPI_CFGR1_PCSCFG_SHIFT                 (27U)
#define SPI_CFGR1_PCSCFG_WIDTH                 (1U)
#define SPI_CFGR1_PCSCFG(x)                    (((uint32_t)(((uint32_t)(x))<<SPI_CFGR1_PCSCFG_SHIFT))&SPI_CFGR1_PCSCFG_MASK)
/** DMR0 Bit Fields */
#define SPI_DMR0_MATCH0_MASK                   (0xFFFFFFFFU)
#define SPI_DMR0_MATCH0_SHIFT                  (0U)
#define SPI_DMR0_MATCH0_WIDTH                  (32U)
#define SPI_DMR0_MATCH0(x)                     (((uint32_t)(((uint32_t)(x))<<SPI_DMR0_MATCH0_SHIFT))&SPI_DMR0_MATCH0_MASK)
/** DMR1 Bit Fields */
#define SPI_DMR1_MATCH1_MASK                   (0xFFFFFFFFU)
#define SPI_DMR1_MATCH1_SHIFT                  (0U)
#define SPI_DMR1_MATCH1_WIDTH                  (32U)
#define SPI_DMR1_MATCH1(x)                     (((uint32_t)(((uint32_t)(x))<<SPI_DMR1_MATCH1_SHIFT))&SPI_DMR1_MATCH1_MASK)
/** CCR Bit Fields */
#define SPI_CCR_SCKDIV_MASK                    (0xFFU)
#define SPI_CCR_SCKDIV_SHIFT                   (0U)
#define SPI_CCR_SCKDIV_WIDTH                   (8U)
#define SPI_CCR_SCKDIV(x)                      (((uint32_t)(((uint32_t)(x))<<SPI_CCR_SCKDIV_SHIFT))&SPI_CCR_SCKDIV_MASK)
#define SPI_CCR_DBT_MASK                       (0xFF00U)
#define SPI_CCR_DBT_SHIFT                      (8U)
#define SPI_CCR_DBT_WIDTH                      (8U)
#define SPI_CCR_DBT(x)                         (((uint32_t)(((uint32_t)(x))<<SPI_CCR_DBT_SHIFT))&SPI_CCR_DBT_MASK)
#define SPI_CCR_PCSSCK_MASK                    (0xFF0000U)
#define SPI_CCR_PCSSCK_SHIFT                   (16U)
#define SPI_CCR_PCSSCK_WIDTH                   (8U)
#define SPI_CCR_PCSSCK(x)                      (((uint32_t)(((uint32_t)(x))<<SPI_CCR_PCSSCK_SHIFT))&SPI_CCR_PCSSCK_MASK)
#define SPI_CCR_SCKPCS_MASK                    (0xFF000000U)
#define SPI_CCR_SCKPCS_SHIFT                   (24U)
#define SPI_CCR_SCKPCS_WIDTH                   (8U)
#define SPI_CCR_SCKPCS(x)                      (((uint32_t)(((uint32_t)(x))<<SPI_CCR_SCKPCS_SHIFT))&SPI_CCR_SCKPCS_MASK)
/** FCR Bit Fields */
#define SPI_FCR_TXWATER_MASK                   (0x3U)
#define SPI_FCR_TXWATER_SHIFT                  (0U)
#define SPI_FCR_TXWATER_WIDTH                  (2U)
#define SPI_FCR_TXWATER(x)                     (((uint32_t)(((uint32_t)(x))<<SPI_FCR_TXWATER_SHIFT))&SPI_FCR_TXWATER_MASK)
#define SPI_FCR_RXWATER_MASK                   (0x30000U)
#define SPI_FCR_RXWATER_SHIFT                  (16U)
#define SPI_FCR_RXWATER_WIDTH                  (2U)
#define SPI_FCR_RXWATER(x)                     (((uint32_t)(((uint32_t)(x))<<SPI_FCR_RXWATER_SHIFT))&SPI_FCR_RXWATER_MASK)
/** FSR Bit Fields */
#define SPI_FSR_TXCOUNT_MASK                   (0x7U)
#define SPI_FSR_TXCOUNT_SHIFT                  (0U)
#define SPI_FSR_TXCOUNT_WIDTH                  (3U)
#define SPI_FSR_TXCOUNT(x)                     (((uint32_t)(((uint32_t)(x))<<SPI_FSR_TXCOUNT_SHIFT))&SPI_FSR_TXCOUNT_MASK)
#define SPI_FSR_RXCOUNT_MASK                   (0x70000U)
#define SPI_FSR_RXCOUNT_SHIFT                  (16U)
#define SPI_FSR_RXCOUNT_WIDTH                  (3U)
#define SPI_FSR_RXCOUNT(x)                     (((uint32_t)(((uint32_t)(x))<<SPI_FSR_RXCOUNT_SHIFT))&SPI_FSR_RXCOUNT_MASK)
/** TCR Bit Fields */
#define SPI_TCR_FRAMESZ_MASK                   (0xFFFU)
#define SPI_TCR_FRAMESZ_SHIFT                  (0U)
#define SPI_TCR_FRAMESZ_WIDTH                  (12U)
#define SPI_TCR_FRAMESZ(x)                     (((uint32_t)(((uint32_t)(x))<<SPI_TCR_FRAMESZ_SHIFT))&SPI_TCR_FRAMESZ_MASK)
#define SPI_TCR_WIDTH_MASK                     (0x30000U)
#define SPI_TCR_WIDTH_SHIFT                    (16U)
#define SPI_TCR_WIDTH_WIDTH                    (2U)
#define SPI_TCR_WIDTH(x)                       (((uint32_t)(((uint32_t)(x))<<SPI_TCR_WIDTH_SHIFT))&SPI_TCR_WIDTH_MASK)
#define SPI_TCR_TXMSK_MASK                     (0x40000U)
#define SPI_TCR_TXMSK_SHIFT                    (18U)
#define SPI_TCR_TXMSK_WIDTH                    (1U)
#define SPI_TCR_TXMSK(x)                       (((uint32_t)(((uint32_t)(x))<<SPI_TCR_TXMSK_SHIFT))&SPI_TCR_TXMSK_MASK)
#define SPI_TCR_RXMSK_MASK                     (0x80000U)
#define SPI_TCR_RXMSK_SHIFT                    (19U)
#define SPI_TCR_RXMSK_WIDTH                    (1U)
#define SPI_TCR_RXMSK(x)                       (((uint32_t)(((uint32_t)(x))<<SPI_TCR_RXMSK_SHIFT))&SPI_TCR_RXMSK_MASK)
#define SPI_TCR_CONTC_MASK                     (0x100000U)
#define SPI_TCR_CONTC_SHIFT                    (20U)
#define SPI_TCR_CONTC_WIDTH                    (1U)
#define SPI_TCR_CONTC(x)                       (((uint32_t)(((uint32_t)(x))<<SPI_TCR_CONTC_SHIFT))&SPI_TCR_CONTC_MASK)
#define SPI_TCR_CONT_MASK                      (0x200000U)
#define SPI_TCR_CONT_SHIFT                     (21U)
#define SPI_TCR_CONT_WIDTH                     (1U)
#define SPI_TCR_CONT(x)                        (((uint32_t)(((uint32_t)(x))<<SPI_TCR_CONT_SHIFT))&SPI_TCR_CONT_MASK)
#define SPI_TCR_BYSW_MASK                      (0x400000U)
#define SPI_TCR_BYSW_SHIFT                     (22U)
#define SPI_TCR_BYSW_WIDTH                     (1U)
#define SPI_TCR_BYSW(x)                        (((uint32_t)(((uint32_t)(x))<<SPI_TCR_BYSW_SHIFT))&SPI_TCR_BYSW_MASK)
#define SPI_TCR_LSBF_MASK                      (0x800000U)
#define SPI_TCR_LSBF_SHIFT                     (23U)
#define SPI_TCR_LSBF_WIDTH                     (1U)
#define SPI_TCR_LSBF(x)                        (((uint32_t)(((uint32_t)(x))<<SPI_TCR_LSBF_SHIFT))&SPI_TCR_LSBF_MASK)
#define SPI_TCR_PCS_MASK                       (0x3000000U)
#define SPI_TCR_PCS_SHIFT                      (24U)
#define SPI_TCR_PCS_WIDTH                      (2U)
#define SPI_TCR_PCS(x)                         (((uint32_t)(((uint32_t)(x))<<SPI_TCR_PCS_SHIFT))&SPI_TCR_PCS_MASK)
#define SPI_TCR_PRESCALE_MASK                  (0x38000000U)
#define SPI_TCR_PRESCALE_SHIFT                 (27U)
#define SPI_TCR_PRESCALE_WIDTH                 (3U)
#define SPI_TCR_PRESCALE(x)                    (((uint32_t)(((uint32_t)(x))<<SPI_TCR_PRESCALE_SHIFT))&SPI_TCR_PRESCALE_MASK)
#define SPI_TCR_CPHA_MASK                      (0x40000000U)
#define SPI_TCR_CPHA_SHIFT                     (30U)
#define SPI_TCR_CPHA_WIDTH                     (1U)
#define SPI_TCR_CPHA(x)                        (((uint32_t)(((uint32_t)(x))<<SPI_TCR_CPHA_SHIFT))&SPI_TCR_CPHA_MASK)
#define SPI_TCR_CPOL_MASK                      (0x80000000U)
#define SPI_TCR_CPOL_SHIFT                     (31U)
#define SPI_TCR_CPOL_WIDTH                     (1U)
#define SPI_TCR_CPOL(x)                        (((uint32_t)(((uint32_t)(x))<<SPI_TCR_CPOL_SHIFT))&SPI_TCR_CPOL_MASK)
/** TDR Bit Fields */
#define SPI_TDR_DATA_MASK                      (0xFFFFFFFFU)
#define SPI_TDR_DATA_SHIFT                     (0U)
#define SPI_TDR_DATA_WIDTH                     (32U)
#define SPI_TDR_DATA(x)                        (((uint32_t)(((uint32_t)(x))<<SPI_TDR_DATA_SHIFT))&SPI_TDR_DATA_MASK)
/** RSR Bit Fields */
#define SPI_RSR_SOF_MASK                       (0x1U)
#define SPI_RSR_SOF_SHIFT                      (0U)
#define SPI_RSR_SOF_WIDTH                      (1U)
#define SPI_RSR_SOF(x)                         (((uint32_t)(((uint32_t)(x))<<SPI_RSR_SOF_SHIFT))&SPI_RSR_SOF_MASK)
#define SPI_RSR_RXEMPTY_MASK                   (0x2U)
#define SPI_RSR_RXEMPTY_SHIFT                  (1U)
#define SPI_RSR_RXEMPTY_WIDTH                  (1U)
#define SPI_RSR_RXEMPTY(x)                     (((uint32_t)(((uint32_t)(x))<<SPI_RSR_RXEMPTY_SHIFT))&SPI_RSR_RXEMPTY_MASK)
/** RDR Bit Fields */
#define SPI_RDR_DATA_MASK                      (0xFFFFFFFFU)
#define SPI_RDR_DATA_SHIFT                     (0U)
#define SPI_RDR_DATA_WIDTH                     (32U)
#define SPI_RDR_DATA(x)                        (((uint32_t)(((uint32_t)(x))<<SPI_RDR_DATA_SHIFT))&SPI_RDR_DATA_MASK)

/**
 * @}
 */ /* end of group SPI_Register_Masks */

/**
 * @}
 */ /* end of group SPI_Peripheral_Access_Layer */

// clang-format on

#endif // __SPI_REGISTER_H__
