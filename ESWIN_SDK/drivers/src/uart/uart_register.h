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
 * @file uart_register.h
 * @brief UART driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __UART_REGISTER_H__
#define __UART_REGISTER_H__

// clang-format off

/* ----------------------------------------------------------------------------
   -- UART Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup UART_Peripheral_Access_Layer UART Peripheral Access Layer
 * @{
 */

/** UART - Size of Registers Arrays */

/**
 * @brief UART Register Layout Typedef
 */
typedef struct {
  __I  uint32_t VERID;                             /**< Version ID Register, offset: 0x0 */
  __I  uint32_t PARAM;                             /**< Parameter Register, offset: 0x4 */
  __IO uint32_t GLOBAL;                            /**< UART Global Register, offset: 0x8 */
  __IO uint32_t PINCFG;                            /**< UART Pin Configuration Register, offset: 0xC */
  __IO uint32_t BAUD;                              /**< UART Baud Rate Register, offset: 0x10 */
  __IO uint32_t STAT;                              /**< UART Status Register, offset: 0x14 */
  __IO uint32_t CTRL;                              /**< UART Control Register, offset: 0x18 */
  __IO uint32_t DATA;                              /**< UART Data Register, offset: 0x1C */
  __IO uint32_t MATCH;                             /**< UART Match Address Register, offset: 0x20 */
  __IO uint32_t MODIR;                             /**< UART Modem IrDA Register, offset: 0x24 */
  __IO uint32_t FIFO;                              /**< UART FIFO Register, offset: 0x28 */
  __IO uint32_t WATER;                             /**< UART Watermark Register, offset: 0x2C */
} uart_type_t, *uart_mem_map_ptr_t;


/* ----------------------------------------------------------------------------
   -- UART Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup UART_Register_Masks UART Register Masks
 * @{
 */

/** VERID Bit Fields */
#define UART_VERID_FEATURE_MASK                (0xFFFFU)
#define UART_VERID_FEATURE_SHIFT               (0U)
#define UART_VERID_FEATURE_WIDTH               (16U)
#define UART_VERID_FEATURE(x)                  (((uint32_t)(((uint32_t)(x))<<UART_VERID_FEATURE_SHIFT))&UART_VERID_FEATURE_MASK)
#define UART_VERID_MINOR_MASK                  (0xFF0000U)
#define UART_VERID_MINOR_SHIFT                 (16U)
#define UART_VERID_MINOR_WIDTH                 (8U)
#define UART_VERID_MINOR(x)                    (((uint32_t)(((uint32_t)(x))<<UART_VERID_MINOR_SHIFT))&UART_VERID_MINOR_MASK)
#define UART_VERID_MAJOR_MASK                  (0xFF000000U)
#define UART_VERID_MAJOR_SHIFT                 (24U)
#define UART_VERID_MAJOR_WIDTH                 (8U)
#define UART_VERID_MAJOR(x)                    (((uint32_t)(((uint32_t)(x))<<UART_VERID_MAJOR_SHIFT))&UART_VERID_MAJOR_MASK)
/** PARAM Bit Fields */
#define UART_PARAM_TXFIFO_MASK                 (0xFFU)
#define UART_PARAM_TXFIFO_SHIFT                (0U)
#define UART_PARAM_TXFIFO_WIDTH                (8U)
#define UART_PARAM_TXFIFO(x)                   (((uint32_t)(((uint32_t)(x))<<UART_PARAM_TXFIFO_SHIFT))&UART_PARAM_TXFIFO_MASK)
#define UART_PARAM_RXFIFO_MASK                 (0xFF00U)
#define UART_PARAM_RXFIFO_SHIFT                (8U)
#define UART_PARAM_RXFIFO_WIDTH                (8U)
#define UART_PARAM_RXFIFO(x)                   (((uint32_t)(((uint32_t)(x))<<UART_PARAM_RXFIFO_SHIFT))&UART_PARAM_RXFIFO_MASK)
/** GLOBAL Bit Fields */
#define UART_GLOBAL_RST_MASK                   (0x2U)
#define UART_GLOBAL_RST_SHIFT                  (1U)
#define UART_GLOBAL_RST_WIDTH                  (1U)
#define UART_GLOBAL_RST(x)                     (((uint32_t)(((uint32_t)(x))<<UART_GLOBAL_RST_SHIFT))&UART_GLOBAL_RST_MASK)
/** PINCFG Bit Fields */
#define UART_PINCFG_TRGSEL_MASK                (0x3U)
#define UART_PINCFG_TRGSEL_SHIFT               (0U)
#define UART_PINCFG_TRGSEL_WIDTH               (2U)
#define UART_PINCFG_TRGSEL(x)                  (((uint32_t)(((uint32_t)(x))<<UART_PINCFG_TRGSEL_SHIFT))&UART_PINCFG_TRGSEL_MASK)
/** BAUD Bit Fields */
#define UART_BAUD_SBR_MASK                     (0x1FFFU)
#define UART_BAUD_SBR_SHIFT                    (0U)
#define UART_BAUD_SBR_WIDTH                    (13U)
#define UART_BAUD_SBR(x)                       (((uint32_t)(((uint32_t)(x))<<UART_BAUD_SBR_SHIFT))&UART_BAUD_SBR_MASK)
#define UART_BAUD_SBNS_MASK                    (0x2000U)
#define UART_BAUD_SBNS_SHIFT                   (13U)
#define UART_BAUD_SBNS_WIDTH                   (1U)
#define UART_BAUD_SBNS(x)                      (((uint32_t)(((uint32_t)(x))<<UART_BAUD_SBNS_SHIFT))&UART_BAUD_SBNS_MASK)
#define UART_BAUD_RXEDGIE_MASK                 (0x4000U)
#define UART_BAUD_RXEDGIE_SHIFT                (14U)
#define UART_BAUD_RXEDGIE_WIDTH                (1U)
#define UART_BAUD_RXEDGIE(x)                   (((uint32_t)(((uint32_t)(x))<<UART_BAUD_RXEDGIE_SHIFT))&UART_BAUD_RXEDGIE_MASK)
#define UART_BAUD_LBKDIE_MASK                  (0x8000U)
#define UART_BAUD_LBKDIE_SHIFT                 (15U)
#define UART_BAUD_LBKDIE_WIDTH                 (1U)
#define UART_BAUD_LBKDIE(x)                    (((uint32_t)(((uint32_t)(x))<<UART_BAUD_LBKDIE_SHIFT))&UART_BAUD_LBKDIE_MASK)
#define UART_BAUD_RESYNCDIS_MASK               (0x10000U)
#define UART_BAUD_RESYNCDIS_SHIFT              (16U)
#define UART_BAUD_RESYNCDIS_WIDTH              (1U)
#define UART_BAUD_RESYNCDIS(x)                 (((uint32_t)(((uint32_t)(x))<<UART_BAUD_RESYNCDIS_SHIFT))&UART_BAUD_RESYNCDIS_MASK)
#define UART_BAUD_BOTHEDGE_MASK                (0x20000U)
#define UART_BAUD_BOTHEDGE_SHIFT               (17U)
#define UART_BAUD_BOTHEDGE_WIDTH               (1U)
#define UART_BAUD_BOTHEDGE(x)                  (((uint32_t)(((uint32_t)(x))<<UART_BAUD_BOTHEDGE_SHIFT))&UART_BAUD_BOTHEDGE_MASK)
#define UART_BAUD_MATCFG_MASK                  (0xC0000U)
#define UART_BAUD_MATCFG_SHIFT                 (18U)
#define UART_BAUD_MATCFG_WIDTH                 (2U)
#define UART_BAUD_MATCFG(x)                    (((uint32_t)(((uint32_t)(x))<<UART_BAUD_MATCFG_SHIFT))&UART_BAUD_MATCFG_MASK)
#define UART_BAUD_RIDMAE_MASK                  (0x100000U)
#define UART_BAUD_RIDMAE_SHIFT                 (20U)
#define UART_BAUD_RIDMAE_WIDTH                 (1U)
#define UART_BAUD_RIDMAE(x)                    (((uint32_t)(((uint32_t)(x))<<UART_BAUD_RIDMAE_SHIFT))&UART_BAUD_RIDMAE_MASK)
#define UART_BAUD_RDMAE_MASK                   (0x200000U)
#define UART_BAUD_RDMAE_SHIFT                  (21U)
#define UART_BAUD_RDMAE_WIDTH                  (1U)
#define UART_BAUD_RDMAE(x)                     (((uint32_t)(((uint32_t)(x))<<UART_BAUD_RDMAE_SHIFT))&UART_BAUD_RDMAE_MASK)
#define UART_BAUD_TDMAE_MASK                   (0x800000U)
#define UART_BAUD_TDMAE_SHIFT                  (23U)
#define UART_BAUD_TDMAE_WIDTH                  (1U)
#define UART_BAUD_TDMAE(x)                     (((uint32_t)(((uint32_t)(x))<<UART_BAUD_TDMAE_SHIFT))&UART_BAUD_TDMAE_MASK)
#define UART_BAUD_OSR_MASK                     (0x1F000000U)
#define UART_BAUD_OSR_SHIFT                    (24U)
#define UART_BAUD_OSR_WIDTH                    (5U)
#define UART_BAUD_OSR(x)                       (((uint32_t)(((uint32_t)(x))<<UART_BAUD_OSR_SHIFT))&UART_BAUD_OSR_MASK)
#define UART_BAUD_M10_MASK                     (0x20000000U)
#define UART_BAUD_M10_SHIFT                    (29U)
#define UART_BAUD_M10_WIDTH                    (1U)
#define UART_BAUD_M10(x)                       (((uint32_t)(((uint32_t)(x))<<UART_BAUD_M10_SHIFT))&UART_BAUD_M10_MASK)
#define UART_BAUD_MAEN2_MASK                   (0x40000000U)
#define UART_BAUD_MAEN2_SHIFT                  (30U)
#define UART_BAUD_MAEN2_WIDTH                  (1U)
#define UART_BAUD_MAEN2(x)                     (((uint32_t)(((uint32_t)(x))<<UART_BAUD_MAEN2_SHIFT))&UART_BAUD_MAEN2_MASK)
#define UART_BAUD_MAEN1_MASK                   (0x80000000U)
#define UART_BAUD_MAEN1_SHIFT                  (31U)
#define UART_BAUD_MAEN1_WIDTH                  (1U)
#define UART_BAUD_MAEN1(x)                     (((uint32_t)(((uint32_t)(x))<<UART_BAUD_MAEN1_SHIFT))&UART_BAUD_MAEN1_MASK)

/* @brief Default oversampling ratio. */
#define UART_DEFAULT_OSR (0x0FUL)
/* @brief Default baud rate modulo divisor. */
#define UART_DEFAULT_SBR (0x04UL)

/** STAT Bit Fields */
#define UART_STAT_MA2F_MASK                    (0x4000U)
#define UART_STAT_MA2F_SHIFT                   (14U)
#define UART_STAT_MA2F_WIDTH                   (1U)
#define UART_STAT_MA2F(x)                      (((uint32_t)(((uint32_t)(x))<<UART_STAT_MA2F_SHIFT))&UART_STAT_MA2F_MASK)
#define UART_STAT_MA1F_MASK                    (0x8000U)
#define UART_STAT_MA1F_SHIFT                   (15U)
#define UART_STAT_MA1F_WIDTH                   (1U)
#define UART_STAT_MA1F(x)                      (((uint32_t)(((uint32_t)(x))<<UART_STAT_MA1F_SHIFT))&UART_STAT_MA1F_MASK)
#define UART_STAT_PF_MASK                      (0x10000U)
#define UART_STAT_PF_SHIFT                     (16U)
#define UART_STAT_PF_WIDTH                     (1U)
#define UART_STAT_PF(x)                        (((uint32_t)(((uint32_t)(x))<<UART_STAT_PF_SHIFT))&UART_STAT_PF_MASK)
#define UART_STAT_FE_MASK                      (0x20000U)
#define UART_STAT_FE_SHIFT                     (17U)
#define UART_STAT_FE_WIDTH                     (1U)
#define UART_STAT_FE(x)                        (((uint32_t)(((uint32_t)(x))<<UART_STAT_FE_SHIFT))&UART_STAT_FE_MASK)
#define UART_STAT_NF_MASK                      (0x40000U)
#define UART_STAT_NF_SHIFT                     (18U)
#define UART_STAT_NF_WIDTH                     (1U)
#define UART_STAT_NF(x)                        (((uint32_t)(((uint32_t)(x))<<UART_STAT_NF_SHIFT))&UART_STAT_NF_MASK)
#define UART_STAT_OR_MASK                      (0x80000U)
#define UART_STAT_OR_SHIFT                     (19U)
#define UART_STAT_OR_WIDTH                     (1U)
#define UART_STAT_OR(x)                        (((uint32_t)(((uint32_t)(x))<<UART_STAT_OR_SHIFT))&UART_STAT_OR_MASK)
#define UART_STAT_IDLE_MASK                    (0x100000U)
#define UART_STAT_IDLE_SHIFT                   (20U)
#define UART_STAT_IDLE_WIDTH                   (1U)
#define UART_STAT_IDLE(x)                      (((uint32_t)(((uint32_t)(x))<<UART_STAT_IDLE_SHIFT))&UART_STAT_IDLE_MASK)
#define UART_STAT_RDRF_MASK                    (0x200000U)
#define UART_STAT_RDRF_SHIFT                   (21U)
#define UART_STAT_RDRF_WIDTH                   (1U)
#define UART_STAT_RDRF(x)                      (((uint32_t)(((uint32_t)(x))<<UART_STAT_RDRF_SHIFT))&UART_STAT_RDRF_MASK)
#define UART_STAT_TC_MASK                      (0x400000U)
#define UART_STAT_TC_SHIFT                     (22U)
#define UART_STAT_TC_WIDTH                     (1U)
#define UART_STAT_TC(x)                        (((uint32_t)(((uint32_t)(x))<<UART_STAT_TC_SHIFT))&UART_STAT_TC_MASK)
#define UART_STAT_TDRE_MASK                    (0x800000U)
#define UART_STAT_TDRE_SHIFT                   (23U)
#define UART_STAT_TDRE_WIDTH                   (1U)
#define UART_STAT_TDRE(x)                      (((uint32_t)(((uint32_t)(x))<<UART_STAT_TDRE_SHIFT))&UART_STAT_TDRE_MASK)
#define UART_STAT_RAF_MASK                     (0x1000000U)
#define UART_STAT_RAF_SHIFT                    (24U)
#define UART_STAT_RAF_WIDTH                    (1U)
#define UART_STAT_RAF(x)                       (((uint32_t)(((uint32_t)(x))<<UART_STAT_RAF_SHIFT))&UART_STAT_RAF_MASK)
#define UART_STAT_LBKDE_MASK                   (0x2000000U)
#define UART_STAT_LBKDE_SHIFT                  (25U)
#define UART_STAT_LBKDE_WIDTH                  (1U)
#define UART_STAT_LBKDE(x)                     (((uint32_t)(((uint32_t)(x))<<UART_STAT_LBKDE_SHIFT))&UART_STAT_LBKDE_MASK)
#define UART_STAT_BRK13_MASK                   (0x4000000U)
#define UART_STAT_BRK13_SHIFT                  (26U)
#define UART_STAT_BRK13_WIDTH                  (1U)
#define UART_STAT_BRK13(x)                     (((uint32_t)(((uint32_t)(x))<<UART_STAT_BRK13_SHIFT))&UART_STAT_BRK13_MASK)
#define UART_STAT_RWUID_MASK                   (0x8000000U)
#define UART_STAT_RWUID_SHIFT                  (27U)
#define UART_STAT_RWUID_WIDTH                  (1U)
#define UART_STAT_RWUID(x)                     (((uint32_t)(((uint32_t)(x))<<UART_STAT_RWUID_SHIFT))&UART_STAT_RWUID_MASK)
#define UART_STAT_RXINV_MASK                   (0x10000000U)
#define UART_STAT_RXINV_SHIFT                  (28U)
#define UART_STAT_RXINV_WIDTH                  (1U)
#define UART_STAT_RXINV(x)                     (((uint32_t)(((uint32_t)(x))<<UART_STAT_RXINV_SHIFT))&UART_STAT_RXINV_MASK)
#define UART_STAT_MSBF_MASK                    (0x20000000U)
#define UART_STAT_MSBF_SHIFT                   (29U)
#define UART_STAT_MSBF_WIDTH                   (1U)
#define UART_STAT_MSBF(x)                      (((uint32_t)(((uint32_t)(x))<<UART_STAT_MSBF_SHIFT))&UART_STAT_MSBF_MASK)
#define UART_STAT_RXEDGIF_MASK                 (0x40000000U)
#define UART_STAT_RXEDGIF_SHIFT                (30U)
#define UART_STAT_RXEDGIF_WIDTH                (1U)
#define UART_STAT_RXEDGIF(x)                   (((uint32_t)(((uint32_t)(x))<<UART_STAT_RXEDGIF_SHIFT))&UART_STAT_RXEDGIF_MASK)
#define UART_STAT_LBKDIF_MASK                  (0x80000000U)
#define UART_STAT_LBKDIF_SHIFT                 (31U)
#define UART_STAT_LBKDIF_WIDTH                 (1U)
#define UART_STAT_LBKDIF(x)                    (((uint32_t)(((uint32_t)(x))<<UART_STAT_LBKDIF_SHIFT))&UART_STAT_LBKDIF_MASK)

/* @brief Flag clearance mask for STAT register.(0xC01FC000U) */
#define UART_STAT_REG_FLAGS_MASK (UART_STAT_LBKDIF(1) | UART_STAT_RXEDGIF(1) | UART_STAT_IDLE(1) | UART_STAT_OR(1) | \
                                          UART_STAT_NF(1) | UART_STAT_FE(1) | UART_STAT_PF(1) | UART_STAT_MA1F(1) | UART_STAT_MA2F(1))

/** CTRL Bit Fields */
#define UART_CTRL_PT_MASK                      (0x1U)
#define UART_CTRL_PT_SHIFT                     (0U)
#define UART_CTRL_PT_WIDTH                     (1U)
#define UART_CTRL_PT(x)                        (((uint32_t)(((uint32_t)(x))<<UART_CTRL_PT_SHIFT))&UART_CTRL_PT_MASK)
#define UART_CTRL_PE_MASK                      (0x2U)
#define UART_CTRL_PE_SHIFT                     (1U)
#define UART_CTRL_PE_WIDTH                     (1U)
#define UART_CTRL_PE(x)                        (((uint32_t)(((uint32_t)(x))<<UART_CTRL_PE_SHIFT))&UART_CTRL_PE_MASK)
#define UART_CTRL_ILT_MASK                     (0x4U)
#define UART_CTRL_ILT_SHIFT                    (2U)
#define UART_CTRL_ILT_WIDTH                    (1U)
#define UART_CTRL_ILT(x)                       (((uint32_t)(((uint32_t)(x))<<UART_CTRL_ILT_SHIFT))&UART_CTRL_ILT_MASK)
#define UART_CTRL_WAKE_MASK                    (0x8U)
#define UART_CTRL_WAKE_SHIFT                   (3U)
#define UART_CTRL_WAKE_WIDTH                   (1U)
#define UART_CTRL_WAKE(x)                      (((uint32_t)(((uint32_t)(x))<<UART_CTRL_WAKE_SHIFT))&UART_CTRL_WAKE_MASK)
#define UART_CTRL_M_MASK                       (0x10U)
#define UART_CTRL_M_SHIFT                      (4U)
#define UART_CTRL_M_WIDTH                      (1U)
#define UART_CTRL_M(x)                         (((uint32_t)(((uint32_t)(x))<<UART_CTRL_M_SHIFT))&UART_CTRL_M_MASK)
#define UART_CTRL_RSRC_MASK                    (0x20U)
#define UART_CTRL_RSRC_SHIFT                   (5U)
#define UART_CTRL_RSRC_WIDTH                   (1U)
#define UART_CTRL_RSRC(x)                      (((uint32_t)(((uint32_t)(x))<<UART_CTRL_RSRC_SHIFT))&UART_CTRL_RSRC_MASK)
#define UART_CTRL_DOZEEN_MASK                  (0x40U)
#define UART_CTRL_DOZEEN_SHIFT                 (6U)
#define UART_CTRL_DOZEEN_WIDTH                 (1U)
#define UART_CTRL_DOZEEN(x)                    (((uint32_t)(((uint32_t)(x))<<UART_CTRL_DOZEEN_SHIFT))&UART_CTRL_DOZEEN_MASK)
#define UART_CTRL_LOOPS_MASK                   (0x80U)
#define UART_CTRL_LOOPS_SHIFT                  (7U)
#define UART_CTRL_LOOPS_WIDTH                  (1U)
#define UART_CTRL_LOOPS(x)                     (((uint32_t)(((uint32_t)(x))<<UART_CTRL_LOOPS_SHIFT))&UART_CTRL_LOOPS_MASK)
#define UART_CTRL_IDLECFG_MASK                 (0x700U)
#define UART_CTRL_IDLECFG_SHIFT                (8U)
#define UART_CTRL_IDLECFG_WIDTH                (3U)
#define UART_CTRL_IDLECFG(x)                   (((uint32_t)(((uint32_t)(x))<<UART_CTRL_IDLECFG_SHIFT))&UART_CTRL_IDLECFG_MASK)
#define UART_CTRL_M7_MASK                      (0x800U)
#define UART_CTRL_M7_SHIFT                     (11U)
#define UART_CTRL_M7_WIDTH                     (1U)
#define UART_CTRL_M7(x)                        (((uint32_t)(((uint32_t)(x))<<UART_CTRL_M7_SHIFT))&UART_CTRL_M7_MASK)
#define UART_CTRL_MA2IE_MASK                   (0x4000U)
#define UART_CTRL_MA2IE_SHIFT                  (14U)
#define UART_CTRL_MA2IE_WIDTH                  (1U)
#define UART_CTRL_MA2IE(x)                     (((uint32_t)(((uint32_t)(x))<<UART_CTRL_MA2IE_SHIFT))&UART_CTRL_MA2IE_MASK)
#define UART_CTRL_MA1IE_MASK                   (0x8000U)
#define UART_CTRL_MA1IE_SHIFT                  (15U)
#define UART_CTRL_MA1IE_WIDTH                  (1U)
#define UART_CTRL_MA1IE(x)                     (((uint32_t)(((uint32_t)(x))<<UART_CTRL_MA1IE_SHIFT))&UART_CTRL_MA1IE_MASK)
#define UART_CTRL_SBK_MASK                     (0x10000U)
#define UART_CTRL_SBK_SHIFT                    (16U)
#define UART_CTRL_SBK_WIDTH                    (1U)
#define UART_CTRL_SBK(x)                       (((uint32_t)(((uint32_t)(x))<<UART_CTRL_SBK_SHIFT))&UART_CTRL_SBK_MASK)
#define UART_CTRL_RWU_MASK                     (0x20000U)
#define UART_CTRL_RWU_SHIFT                    (17U)
#define UART_CTRL_RWU_WIDTH                    (1U)
#define UART_CTRL_RWU(x)                       (((uint32_t)(((uint32_t)(x))<<UART_CTRL_RWU_SHIFT))&UART_CTRL_RWU_MASK)
#define UART_CTRL_RE_MASK                      (0x40000U)
#define UART_CTRL_RE_SHIFT                     (18U)
#define UART_CTRL_RE_WIDTH                     (1U)
#define UART_CTRL_RE(x)                        (((uint32_t)(((uint32_t)(x))<<UART_CTRL_RE_SHIFT))&UART_CTRL_RE_MASK)
#define UART_CTRL_TE_MASK                      (0x80000U)
#define UART_CTRL_TE_SHIFT                     (19U)
#define UART_CTRL_TE_WIDTH                     (1U)
#define UART_CTRL_TE(x)                        (((uint32_t)(((uint32_t)(x))<<UART_CTRL_TE_SHIFT))&UART_CTRL_TE_MASK)
#define UART_CTRL_ILIE_MASK                    (0x100000U)
#define UART_CTRL_ILIE_SHIFT                   (20U)
#define UART_CTRL_ILIE_WIDTH                   (1U)
#define UART_CTRL_ILIE(x)                      (((uint32_t)(((uint32_t)(x))<<UART_CTRL_ILIE_SHIFT))&UART_CTRL_ILIE_MASK)
#define UART_CTRL_RIE_MASK                     (0x200000U)
#define UART_CTRL_RIE_SHIFT                    (21U)
#define UART_CTRL_RIE_WIDTH                    (1U)
#define UART_CTRL_RIE(x)                       (((uint32_t)(((uint32_t)(x))<<UART_CTRL_RIE_SHIFT))&UART_CTRL_RIE_MASK)
#define UART_CTRL_TCIE_MASK                    (0x400000U)
#define UART_CTRL_TCIE_SHIFT                   (22U)
#define UART_CTRL_TCIE_WIDTH                   (1U)
#define UART_CTRL_TCIE(x)                      (((uint32_t)(((uint32_t)(x))<<UART_CTRL_TCIE_SHIFT))&UART_CTRL_TCIE_MASK)
#define UART_CTRL_TIE_MASK                     (0x800000U)
#define UART_CTRL_TIE_SHIFT                    (23U)
#define UART_CTRL_TIE_WIDTH                    (1U)
#define UART_CTRL_TIE(x)                       (((uint32_t)(((uint32_t)(x))<<UART_CTRL_TIE_SHIFT))&UART_CTRL_TIE_MASK)
#define UART_CTRL_PEIE_MASK                    (0x1000000U)
#define UART_CTRL_PEIE_SHIFT                   (24U)
#define UART_CTRL_PEIE_WIDTH                   (1U)
#define UART_CTRL_PEIE(x)                      (((uint32_t)(((uint32_t)(x))<<UART_CTRL_PEIE_SHIFT))&UART_CTRL_PEIE_MASK)
#define UART_CTRL_FEIE_MASK                    (0x2000000U)
#define UART_CTRL_FEIE_SHIFT                   (25U)
#define UART_CTRL_FEIE_WIDTH                   (1U)
#define UART_CTRL_FEIE(x)                      (((uint32_t)(((uint32_t)(x))<<UART_CTRL_FEIE_SHIFT))&UART_CTRL_FEIE_MASK)
#define UART_CTRL_NEIE_MASK                    (0x4000000U)
#define UART_CTRL_NEIE_SHIFT                   (26U)
#define UART_CTRL_NEIE_WIDTH                   (1U)
#define UART_CTRL_NEIE(x)                      (((uint32_t)(((uint32_t)(x))<<UART_CTRL_NEIE_SHIFT))&UART_CTRL_NEIE_MASK)
#define UART_CTRL_ORIE_MASK                    (0x8000000U)
#define UART_CTRL_ORIE_SHIFT                   (27U)
#define UART_CTRL_ORIE_WIDTH                   (1U)
#define UART_CTRL_ORIE(x)                      (((uint32_t)(((uint32_t)(x))<<UART_CTRL_ORIE_SHIFT))&UART_CTRL_ORIE_MASK)
#define UART_CTRL_TXINV_MASK                   (0x10000000U)
#define UART_CTRL_TXINV_SHIFT                  (28U)
#define UART_CTRL_TXINV_WIDTH                  (1U)
#define UART_CTRL_TXINV(x)                     (((uint32_t)(((uint32_t)(x))<<UART_CTRL_TXINV_SHIFT))&UART_CTRL_TXINV_MASK)
#define UART_CTRL_TXDIR_MASK                   (0x20000000U)
#define UART_CTRL_TXDIR_SHIFT                  (29U)
#define UART_CTRL_TXDIR_WIDTH                  (1U)
#define UART_CTRL_TXDIR(x)                     (((uint32_t)(((uint32_t)(x))<<UART_CTRL_TXDIR_SHIFT))&UART_CTRL_TXDIR_MASK)
#define UART_CTRL_R9T8_MASK                    (0x40000000U)
#define UART_CTRL_R9T8_SHIFT                   (30U)
#define UART_CTRL_R9T8_WIDTH                   (1U)
#define UART_CTRL_R9T8(x)                      (((uint32_t)(((uint32_t)(x))<<UART_CTRL_R9T8_SHIFT))&UART_CTRL_R9T8_MASK)
#define UART_CTRL_R8T9_MASK                    (0x80000000U)
#define UART_CTRL_R8T9_SHIFT                   (31U)
#define UART_CTRL_R8T9_WIDTH                   (1U)
#define UART_CTRL_R8T9(x)                      (((uint32_t)(((uint32_t)(x))<<UART_CTRL_R8T9_SHIFT))&UART_CTRL_R8T9_MASK)
/** DATA Bit Fields */
#define UART_DATA_R0T0_MASK                    (0x1U)
#define UART_DATA_R0T0_SHIFT                   (0U)
#define UART_DATA_R0T0_WIDTH                   (1U)
#define UART_DATA_R0T0(x)                      (((uint32_t)(((uint32_t)(x))<<UART_DATA_R0T0_SHIFT))&UART_DATA_R0T0_MASK)
#define UART_DATA_R1T1_MASK                    (0x2U)
#define UART_DATA_R1T1_SHIFT                   (1U)
#define UART_DATA_R1T1_WIDTH                   (1U)
#define UART_DATA_R1T1(x)                      (((uint32_t)(((uint32_t)(x))<<UART_DATA_R1T1_SHIFT))&UART_DATA_R1T1_MASK)
#define UART_DATA_R2T2_MASK                    (0x4U)
#define UART_DATA_R2T2_SHIFT                   (2U)
#define UART_DATA_R2T2_WIDTH                   (1U)
#define UART_DATA_R2T2(x)                      (((uint32_t)(((uint32_t)(x))<<UART_DATA_R2T2_SHIFT))&UART_DATA_R2T2_MASK)
#define UART_DATA_R3T3_MASK                    (0x8U)
#define UART_DATA_R3T3_SHIFT                   (3U)
#define UART_DATA_R3T3_WIDTH                   (1U)
#define UART_DATA_R3T3(x)                      (((uint32_t)(((uint32_t)(x))<<UART_DATA_R3T3_SHIFT))&UART_DATA_R3T3_MASK)
#define UART_DATA_R4T4_MASK                    (0x10U)
#define UART_DATA_R4T4_SHIFT                   (4U)
#define UART_DATA_R4T4_WIDTH                   (1U)
#define UART_DATA_R4T4(x)                      (((uint32_t)(((uint32_t)(x))<<UART_DATA_R4T4_SHIFT))&UART_DATA_R4T4_MASK)
#define UART_DATA_R5T5_MASK                    (0x20U)
#define UART_DATA_R5T5_SHIFT                   (5U)
#define UART_DATA_R5T5_WIDTH                   (1U)
#define UART_DATA_R5T5(x)                      (((uint32_t)(((uint32_t)(x))<<UART_DATA_R5T5_SHIFT))&UART_DATA_R5T5_MASK)
#define UART_DATA_R6T6_MASK                    (0x40U)
#define UART_DATA_R6T6_SHIFT                   (6U)
#define UART_DATA_R6T6_WIDTH                   (1U)
#define UART_DATA_R6T6(x)                      (((uint32_t)(((uint32_t)(x))<<UART_DATA_R6T6_SHIFT))&UART_DATA_R6T6_MASK)
#define UART_DATA_R7T7_MASK                    (0x80U)
#define UART_DATA_R7T7_SHIFT                   (7U)
#define UART_DATA_R7T7_WIDTH                   (1U)
#define UART_DATA_R7T7(x)                      (((uint32_t)(((uint32_t)(x))<<UART_DATA_R7T7_SHIFT))&UART_DATA_R7T7_MASK)
#define UART_DATA_R8T8_MASK                    (0x100U)
#define UART_DATA_R8T8_SHIFT                   (8U)
#define UART_DATA_R8T8_WIDTH                   (1U)
#define UART_DATA_R8T8(x)                      (((uint32_t)(((uint32_t)(x))<<UART_DATA_R8T8_SHIFT))&UART_DATA_R8T8_MASK)
#define UART_DATA_R9T9_MASK                    (0x200U)
#define UART_DATA_R9T9_SHIFT                   (9U)
#define UART_DATA_R9T9_WIDTH                   (1U)
#define UART_DATA_R9T9(x)                      (((uint32_t)(((uint32_t)(x))<<UART_DATA_R9T9_SHIFT))&UART_DATA_R9T9_MASK)
#define UART_DATA_IDLINE_MASK                  (0x800U)
#define UART_DATA_IDLINE_SHIFT                 (11U)
#define UART_DATA_IDLINE_WIDTH                 (1U)
#define UART_DATA_IDLINE(x)                    (((uint32_t)(((uint32_t)(x))<<UART_DATA_IDLINE_SHIFT))&UART_DATA_IDLINE_MASK)
#define UART_DATA_RXEMPT_MASK                  (0x1000U)
#define UART_DATA_RXEMPT_SHIFT                 (12U)
#define UART_DATA_RXEMPT_WIDTH                 (1U)
#define UART_DATA_RXEMPT(x)                    (((uint32_t)(((uint32_t)(x))<<UART_DATA_RXEMPT_SHIFT))&UART_DATA_RXEMPT_MASK)
#define UART_DATA_FRETSC_MASK                  (0x2000U)
#define UART_DATA_FRETSC_SHIFT                 (13U)
#define UART_DATA_FRETSC_WIDTH                 (1U)
#define UART_DATA_FRETSC(x)                    (((uint32_t)(((uint32_t)(x))<<UART_DATA_FRETSC_SHIFT))&UART_DATA_FRETSC_MASK)
#define UART_DATA_PARITYE_MASK                 (0x4000U)
#define UART_DATA_PARITYE_SHIFT                (14U)
#define UART_DATA_PARITYE_WIDTH                (1U)
#define UART_DATA_PARITYE(x)                   (((uint32_t)(((uint32_t)(x))<<UART_DATA_PARITYE_SHIFT))&UART_DATA_PARITYE_MASK)
#define UART_DATA_NOISY_MASK                   (0x8000U)
#define UART_DATA_NOISY_SHIFT                  (15U)
#define UART_DATA_NOISY_WIDTH                  (1U)
#define UART_DATA_NOISY(x)                     (((uint32_t)(((uint32_t)(x))<<UART_DATA_NOISY_SHIFT))&UART_DATA_NOISY_MASK)
/** MATCH Bit Fields */
#define UART_MATCH_MA1_MASK                    (0x3FFU)
#define UART_MATCH_MA1_SHIFT                   (0U)
#define UART_MATCH_MA1_WIDTH                   (10U)
#define UART_MATCH_MA1(x)                      (((uint32_t)(((uint32_t)(x))<<UART_MATCH_MA1_SHIFT))&UART_MATCH_MA1_MASK)
#define UART_MATCH_MA2_MASK                    (0x3FF0000U)
#define UART_MATCH_MA2_SHIFT                   (16U)
#define UART_MATCH_MA2_WIDTH                   (10U)
#define UART_MATCH_MA2(x)                      (((uint32_t)(((uint32_t)(x))<<UART_MATCH_MA2_SHIFT))&UART_MATCH_MA2_MASK)
/** MODIR Bit Fields */
#define UART_MODIR_TXCTSE_MASK                 (0x1U)
#define UART_MODIR_TXCTSE_SHIFT                (0U)
#define UART_MODIR_TXCTSE_WIDTH                (1U)
#define UART_MODIR_TXCTSE(x)                   (((uint32_t)(((uint32_t)(x))<<UART_MODIR_TXCTSE_SHIFT))&UART_MODIR_TXCTSE_MASK)
#define UART_MODIR_TXRTSE_MASK                 (0x2U)
#define UART_MODIR_TXRTSE_SHIFT                (1U)
#define UART_MODIR_TXRTSE_WIDTH                (1U)
#define UART_MODIR_TXRTSE(x)                   (((uint32_t)(((uint32_t)(x))<<UART_MODIR_TXRTSE_SHIFT))&UART_MODIR_TXRTSE_MASK)
#define UART_MODIR_TXRTSPOL_MASK               (0x4U)
#define UART_MODIR_TXRTSPOL_SHIFT              (2U)
#define UART_MODIR_TXRTSPOL_WIDTH              (1U)
#define UART_MODIR_TXRTSPOL(x)                 (((uint32_t)(((uint32_t)(x))<<UART_MODIR_TXRTSPOL_SHIFT))&UART_MODIR_TXRTSPOL_MASK)
#define UART_MODIR_RXRTSE_MASK                 (0x8U)
#define UART_MODIR_RXRTSE_SHIFT                (3U)
#define UART_MODIR_RXRTSE_WIDTH                (1U)
#define UART_MODIR_RXRTSE(x)                   (((uint32_t)(((uint32_t)(x))<<UART_MODIR_RXRTSE_SHIFT))&UART_MODIR_RXRTSE_MASK)
#define UART_MODIR_TXCTSC_MASK                 (0x10U)
#define UART_MODIR_TXCTSC_SHIFT                (4U)
#define UART_MODIR_TXCTSC_WIDTH                (1U)
#define UART_MODIR_TXCTSC(x)                   (((uint32_t)(((uint32_t)(x))<<UART_MODIR_TXCTSC_SHIFT))&UART_MODIR_TXCTSC_MASK)
#define UART_MODIR_TXCTSSRC_MASK               (0x20U)
#define UART_MODIR_TXCTSSRC_SHIFT              (5U)
#define UART_MODIR_TXCTSSRC_WIDTH              (1U)
#define UART_MODIR_TXCTSSRC(x)                 (((uint32_t)(((uint32_t)(x))<<UART_MODIR_TXCTSSRC_SHIFT))&UART_MODIR_TXCTSSRC_MASK)
#define UART_MODIR_RTSWATER_MASK               (0x300U)
#define UART_MODIR_RTSWATER_SHIFT              (8U)
#define UART_MODIR_RTSWATER_WIDTH              (2U)
#define UART_MODIR_RTSWATER(x)                 (((uint32_t)(((uint32_t)(x))<<UART_MODIR_RTSWATER_SHIFT))&UART_MODIR_RTSWATER_MASK)
#define UART_MODIR_TNP_MASK                    (0x30000U)
#define UART_MODIR_TNP_SHIFT                   (16U)
#define UART_MODIR_TNP_WIDTH                   (2U)
#define UART_MODIR_TNP(x)                      (((uint32_t)(((uint32_t)(x))<<UART_MODIR_TNP_SHIFT))&UART_MODIR_TNP_MASK)
#define UART_MODIR_IREN_MASK                   (0x40000U)
#define UART_MODIR_IREN_SHIFT                  (18U)
#define UART_MODIR_IREN_WIDTH                  (1U)
#define UART_MODIR_IREN(x)                     (((uint32_t)(((uint32_t)(x))<<UART_MODIR_IREN_SHIFT))&UART_MODIR_IREN_MASK)
/** FIFO Bit Fields */
#define UART_FIFO_RXFIFOSIZE_MASK              (0x7U)
#define UART_FIFO_RXFIFOSIZE_SHIFT             (0U)
#define UART_FIFO_RXFIFOSIZE_WIDTH             (3U)
#define UART_FIFO_RXFIFOSIZE(x)                (((uint32_t)(((uint32_t)(x))<<UART_FIFO_RXFIFOSIZE_SHIFT))&UART_FIFO_RXFIFOSIZE_MASK)
#define UART_FIFO_RXFE_MASK                    (0x8U)
#define UART_FIFO_RXFE_SHIFT                   (3U)
#define UART_FIFO_RXFE_WIDTH                   (1U)
#define UART_FIFO_RXFE(x)                      (((uint32_t)(((uint32_t)(x))<<UART_FIFO_RXFE_SHIFT))&UART_FIFO_RXFE_MASK)
#define UART_FIFO_TXFIFOSIZE_MASK              (0x70U)
#define UART_FIFO_TXFIFOSIZE_SHIFT             (4U)
#define UART_FIFO_TXFIFOSIZE_WIDTH             (3U)
#define UART_FIFO_TXFIFOSIZE(x)                (((uint32_t)(((uint32_t)(x))<<UART_FIFO_TXFIFOSIZE_SHIFT))&UART_FIFO_TXFIFOSIZE_MASK)
#define UART_FIFO_TXFE_MASK                    (0x80U)
#define UART_FIFO_TXFE_SHIFT                   (7U)
#define UART_FIFO_TXFE_WIDTH                   (1U)
#define UART_FIFO_TXFE(x)                      (((uint32_t)(((uint32_t)(x))<<UART_FIFO_TXFE_SHIFT))&UART_FIFO_TXFE_MASK)
#define UART_FIFO_RXUFE_MASK                   (0x100U)
#define UART_FIFO_RXUFE_SHIFT                  (8U)
#define UART_FIFO_RXUFE_WIDTH                  (1U)
#define UART_FIFO_RXUFE(x)                     (((uint32_t)(((uint32_t)(x))<<UART_FIFO_RXUFE_SHIFT))&UART_FIFO_RXUFE_MASK)
#define UART_FIFO_TXOFE_MASK                   (0x200U)
#define UART_FIFO_TXOFE_SHIFT                  (9U)
#define UART_FIFO_TXOFE_WIDTH                  (1U)
#define UART_FIFO_TXOFE(x)                     (((uint32_t)(((uint32_t)(x))<<UART_FIFO_TXOFE_SHIFT))&UART_FIFO_TXOFE_MASK)
#define UART_FIFO_RXIDEN_MASK                  (0x1C00U)
#define UART_FIFO_RXIDEN_SHIFT                 (10U)
#define UART_FIFO_RXIDEN_WIDTH                 (3U)
#define UART_FIFO_RXIDEN(x)                    (((uint32_t)(((uint32_t)(x))<<UART_FIFO_RXIDEN_SHIFT))&UART_FIFO_RXIDEN_MASK)
#define UART_FIFO_RXFLUSH_MASK                 (0x4000U)
#define UART_FIFO_RXFLUSH_SHIFT                (14U)
#define UART_FIFO_RXFLUSH_WIDTH                (1U)
#define UART_FIFO_RXFLUSH(x)                   (((uint32_t)(((uint32_t)(x))<<UART_FIFO_RXFLUSH_SHIFT))&UART_FIFO_RXFLUSH_MASK)
#define UART_FIFO_TXFLUSH_MASK                 (0x8000U)
#define UART_FIFO_TXFLUSH_SHIFT                (15U)
#define UART_FIFO_TXFLUSH_WIDTH                (1U)
#define UART_FIFO_TXFLUSH(x)                   (((uint32_t)(((uint32_t)(x))<<UART_FIFO_TXFLUSH_SHIFT))&UART_FIFO_TXFLUSH_MASK)
#define UART_FIFO_RXUF_MASK                    (0x10000U)
#define UART_FIFO_RXUF_SHIFT                   (16U)
#define UART_FIFO_RXUF_WIDTH                   (1U)
#define UART_FIFO_RXUF(x)                      (((uint32_t)(((uint32_t)(x))<<UART_FIFO_RXUF_SHIFT))&UART_FIFO_RXUF_MASK)
#define UART_FIFO_TXOF_MASK                    (0x20000U)
#define UART_FIFO_TXOF_SHIFT                   (17U)
#define UART_FIFO_TXOF_WIDTH                   (1U)
#define UART_FIFO_TXOF(x)                      (((uint32_t)(((uint32_t)(x))<<UART_FIFO_TXOF_SHIFT))&UART_FIFO_TXOF_MASK)
#define UART_FIFO_RXEMPT_MASK                  (0x400000U)
#define UART_FIFO_RXEMPT_SHIFT                 (22U)
#define UART_FIFO_RXEMPT_WIDTH                 (1U)
#define UART_FIFO_RXEMPT(x)                    (((uint32_t)(((uint32_t)(x))<<UART_FIFO_RXEMPT_SHIFT))&UART_FIFO_RXEMPT_MASK)
#define UART_FIFO_TXEMPT_MASK                  (0x800000U)
#define UART_FIFO_TXEMPT_SHIFT                 (23U)
#define UART_FIFO_TXEMPT_WIDTH                 (1U)
#define UART_FIFO_TXEMPT(x)                    (((uint32_t)(((uint32_t)(x))<<UART_FIFO_TXEMPT_SHIFT))&UART_FIFO_TXEMPT_MASK)

/* @brief Flag clearance mask for FIFO register(0x00030000). */
#define UART_FIFO_REG_FLAGS_MASK     (UART_FIFO_TXOF(1) | UART_FIFO_RXUF(1))
/* @brief Reset mask for FIFO register.(0x0003C000U) */
#define UART_FIFO_RESET_MASK         (UART_FIFO_REG_FLAGS_MASK | UART_FIFO_TXFLUSH(1) | UART_FIFO_RXFLUSH(1))
/* @brief Reset mask for FIFO register. (0x0003C088U) */
#define UART_FIFO_EN_RESET_MASK      (UART_FIFO_RESET_MASK | UART_FIFO_TXFE(1) | UART_FIFO_RXFE(1))
#define UART_FIFO_DISEN_RESET_MASK   (UART_FIFO_RESET_MASK | UART_FIFO_TXFE(0) | UART_FIFO_RXFE(0))


/** WATER Bit Fields */
#define UART_WATER_TXWATER_MASK                (0x3U)
#define UART_WATER_TXWATER_SHIFT               (0U)
#define UART_WATER_TXWATER_WIDTH               (2U)
#define UART_WATER_TXWATER(x)                  (((uint32_t)(((uint32_t)(x))<<UART_WATER_TXWATER_SHIFT))&UART_WATER_TXWATER_MASK)
#define UART_WATER_TXCOUNT_MASK                (0x700U)
#define UART_WATER_TXCOUNT_SHIFT               (8U)
#define UART_WATER_TXCOUNT_WIDTH               (3U)
#define UART_WATER_TXCOUNT(x)                  (((uint32_t)(((uint32_t)(x))<<UART_WATER_TXCOUNT_SHIFT))&UART_WATER_TXCOUNT_MASK)
#define UART_WATER_RXWATER_MASK                (0x30000U)
#define UART_WATER_RXWATER_SHIFT               (16U)
#define UART_WATER_RXWATER_WIDTH               (2U)
#define UART_WATER_RXWATER(x)                  (((uint32_t)(((uint32_t)(x))<<UART_WATER_RXWATER_SHIFT))&UART_WATER_RXWATER_MASK)
#define UART_WATER_RXCOUNT_MASK                (0x7000000U)
#define UART_WATER_RXCOUNT_SHIFT               (24U)
#define UART_WATER_RXCOUNT_WIDTH               (3U)
#define UART_WATER_RXCOUNT(x)                  (((uint32_t)(((uint32_t)(x))<<UART_WATER_RXCOUNT_SHIFT))&UART_WATER_RXCOUNT_MASK)

/*@brief UART register default value */
#define UART_REG_DEFAULT_VERID  (0x01CEE026U)
#define UART_REG_DEFAULT_PARAM  (0x00000505U)
#define UART_REG_DEFAULT_GLOBAL (0x00000000U)
#define UART_REG_DEFAULT_PINCFG (0x00000000U)
#define UART_REG_DEFAULT_BAUD   (0x0F000004U)
#define UART_REG_DEFAULT_STAT   (0x00C00000U)
#define UART_REG_DEFAULT_CTRL   (0x00000000U)
#define UART_REG_DEFAULT_DATA   (0x00001000U)
#define UART_REG_DEFAULT_MATCH  (0x00000000U)
#define UART_REG_DEFAULT_MODIR  (0x00000000U)
#define UART_REG_DEFAULT_FIFO   (0x00C00044U)
#define UART_REG_DEFAULT_WATER  (0x00000000U)

/**
 * @}
 */ /* end of group UART_Register_Masks */

/**
 * @}
 */ /* end of group UART_Peripheral_Access_Layer */

// clang-format on

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

#endif // __UART_REGISTER_H__
