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
 * @file superio_register.h
 * @brief SUPERIO driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-11
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __SUPERIO_REGISTER_H__
#define __SUPERIO_REGISTER_H__

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
   -- SUPERIO Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup SUPERIO_Peripheral_Access_Layer SUPERIO Peripheral Access Layer
 * @{
 */

/** SUPERIO - Size of Registers Arrays */
#define SUPERIO_SHIFTCTL_COUNT                    (4U)
#define SUPERIO_SHIFTCFG_COUNT                    (4U)
#define SUPERIO_SHIFTBUF_COUNT                    (4U)
#define SUPERIO_SHIFTBUFBIS_COUNT                 (4U)
#define SUPERIO_SHIFTBUFBYS_COUNT                 (4U)
#define SUPERIO_SHIFTBUFBBS_COUNT                 (4U)
#define SUPERIO_TIMCTL_COUNT                      (4U)
#define SUPERIO_TIMCFG_COUNT                      (4U)
#define SUPERIO_TIMCMP_COUNT                      (4U)

/**
 * @brief SUPERIO Register Layout Typedef
 */
typedef struct {
  __I  uint32_t VERID;                                      /**< Version ID Register, offset: 0x0 */
  __I  uint32_t PARAM;                                      /**< Parameter Register, offset: 0x4 */
  __IO uint32_t CTRL;                                       /**< SUPERIO Control Register, offset: 0x8 */
  __I  uint32_t PIN;                                        /**< Pin State Register, offset: 0xC */
  __IO uint32_t SHIFTSTAT;                                  /**< Shifter Status Register, offset: 0x10 */
  __IO uint32_t SHIFTERR;                                   /**< Shifter Error Register, offset: 0x14 */
  __IO uint32_t TIMSTAT;                                    /**< Timer Status Register, offset: 0x18 */
       uint8_t RESERVED_0[4];
  __IO uint32_t SHIFTSIEN;                                  /**< Shifter Status Interrupt Enable, offset: 0x20 */
  __IO uint32_t SHIFTEIEN;                                  /**< Shifter Error Interrupt Enable, offset: 0x24 */
  __IO uint32_t TIMIEN;                                     /**< Timer Interrupt Enable Register, offset: 0x28 */
       uint8_t RESERVED_1[4];
  __IO uint32_t SHIFTSDEN;                                  /**< Shifter Status PDMA Enable, offset: 0x30 */
       uint8_t RESERVED_2[76];
  __IO uint32_t SHIFTCTL[SUPERIO_SHIFTCTL_COUNT];           /**< Shifter Control N Register, array offset: 0x80, array step: 0x4 */
       uint8_t RESERVED_3[112];
  __IO uint32_t SHIFTCFG[SUPERIO_SHIFTCFG_COUNT];           /**< Shifter Configuration N Register, array offset: 0x100, array step: 0x4 */
       uint8_t RESERVED_4[240];
  __IO uint32_t SHIFTBUF[SUPERIO_SHIFTBUF_COUNT];           /**< Shifter Buffer N Register, array offset: 0x200, array step: 0x4 */
       uint8_t RESERVED_5[112];
  __IO uint32_t SHIFTBUFBIS[SUPERIO_SHIFTBUFBIS_COUNT];     /**< Shifter Buffer N Bit Swapped Register, array offset: 0x280, array step: 0x4 */
       uint8_t RESERVED_6[112];
  __IO uint32_t SHIFTBUFBYS[SUPERIO_SHIFTBUFBYS_COUNT];     /**< Shifter Buffer N Byte Swapped Register, array offset: 0x300, array step: 0x4 */
       uint8_t RESERVED_7[112];
  __IO uint32_t SHIFTBUFBBS[SUPERIO_SHIFTBUFBBS_COUNT];     /**< Shifter Buffer N Bit Byte Swapped Register, array offset: 0x380, array step: 0x4 */
       uint8_t RESERVED_8[112];
  __IO uint32_t TIMCTL[SUPERIO_TIMCTL_COUNT];               /**< Timer Control N Register, array offset: 0x400, array step: 0x4 */
       uint8_t RESERVED_9[112];
  __IO uint32_t TIMCFG[SUPERIO_TIMCFG_COUNT];               /**< Timer Configuration N Register, array offset: 0x480, array step: 0x4 */
       uint8_t RESERVED_10[112];
  __IO uint32_t TIMCMP[SUPERIO_TIMCMP_COUNT];               /**< Timer Compare N Register, array offset: 0x500, array step: 0x4 */
} superio_type_t, *superio_mem_map_ptr_t;

/** Number of instances of the SUPERIO module. */
#define SUPERIO_INSTANCE_COUNT                    (1U)
#define SUPERIO_PWM_CHANNEL_COUNT                 (4U)

/** SUPERIO - Peripheral instance base addresses */
/** Peripheral SUPERIO base address */
#define SUPERIO_BASE                              (SUPERIO_BASEADDR)
/** Peripheral SUPERIO base pointer */
#define SUPERIO                                   ((superio_type_t *)SUPERIO_BASE)
/** Array initializer of SUPERIO peripheral base addresses */
#define SUPERIO_BASE_ADDRS                        { SUPERIO_BASE }
/** Array initializer of SUPERIO peripheral base pointers */
#define SUPERIO_BASE_PTRS                         { SUPERIO }
/** Number of interrupt vector arrays for the SUPERIO module. */
#define SUPERIO_IRQS_ARR_COUNT                    (1U)
/** Number of interrupt channels for the SUPERIO module. */
#define SUPERIO_IRQS_CH_COUNT                     (1U)
/** Interrupt vectors for the SUPERIO peripheral type */
#define SUPERIO_IRQS                              { SUPERIO_IRQn }

/* ----------------------------------------------------------------------------
   -- SUPERIO Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup SUPERIO_Register_Masks SUPERIO Register Masks
 * @{
 */

/** VERID Bit Fields */
#define SUPERIO_VERID_FEATURE_MASK                0xFFFFU
#define SUPERIO_VERID_FEATURE_SHIFT               0U
#define SUPERIO_VERID_FEATURE_WIDTH               16U
#define SUPERIO_VERID_FEATURE(x)                  (((uint32_t)(((uint32_t)(x))<<SUPERIO_VERID_FEATURE_SHIFT))&SUPERIO_VERID_FEATURE_MASK)
#define SUPERIO_VERID_MINOR_MASK                  0xFF0000U
#define SUPERIO_VERID_MINOR_SHIFT                 16U
#define SUPERIO_VERID_MINOR_WIDTH                 8U
#define SUPERIO_VERID_MINOR(x)                    (((uint32_t)(((uint32_t)(x))<<SUPERIO_VERID_MINOR_SHIFT))&SUPERIO_VERID_MINOR_MASK)
#define SUPERIO_VERID_MAJOR_MASK                  0xFF000000U
#define SUPERIO_VERID_MAJOR_SHIFT                 24U
#define SUPERIO_VERID_MAJOR_WIDTH                 8U
#define SUPERIO_VERID_MAJOR(x)                    (((uint32_t)(((uint32_t)(x))<<SUPERIO_VERID_MAJOR_SHIFT))&SUPERIO_VERID_MAJOR_MASK)
/** PARAM Bit Fields */
#define SUPERIO_PARAM_SHIFTER_MASK                0xFFU
#define SUPERIO_PARAM_SHIFTER_SHIFT               0U
#define SUPERIO_PARAM_SHIFTER_WIDTH               8U
#define SUPERIO_PARAM_SHIFTER(x)                  (((uint32_t)(((uint32_t)(x))<<SUPERIO_PARAM_SHIFTER_SHIFT))&SUPERIO_PARAM_SHIFTER_MASK)
#define SUPERIO_PARAM_TIMER_MASK                  0xFF00U
#define SUPERIO_PARAM_TIMER_SHIFT                 8U
#define SUPERIO_PARAM_TIMER_WIDTH                 8U
#define SUPERIO_PARAM_TIMER(x)                    (((uint32_t)(((uint32_t)(x))<<SUPERIO_PARAM_TIMER_SHIFT))&SUPERIO_PARAM_TIMER_MASK)
#define SUPERIO_PARAM_PIN_MASK                    0xFF0000U
#define SUPERIO_PARAM_PIN_SHIFT                   16U
#define SUPERIO_PARAM_PIN_WIDTH                   8U
#define SUPERIO_PARAM_PIN(x)                      (((uint32_t)(((uint32_t)(x))<<SUPERIO_PARAM_PIN_SHIFT))&SUPERIO_PARAM_PIN_MASK)
#define SUPERIO_PARAM_TRIGGER_MASK                0xFF000000U
#define SUPERIO_PARAM_TRIGGER_SHIFT               24U
#define SUPERIO_PARAM_TRIGGER_WIDTH               8U
#define SUPERIO_PARAM_TRIGGER(x)                  (((uint32_t)(((uint32_t)(x))<<SUPERIO_PARAM_TRIGGER_SHIFT))&SUPERIO_PARAM_TRIGGER_MASK)
/** CTRL Bit Fields */
#define SUPERIO_CTRL_FLEXEN_MASK                  0x1U
#define SUPERIO_CTRL_FLEXEN_SHIFT                 0U
#define SUPERIO_CTRL_FLEXEN_WIDTH                 1U
#define SUPERIO_CTRL_FLEXEN(x)                    (((uint32_t)(((uint32_t)(x))<<SUPERIO_CTRL_FLEXEN_SHIFT))&SUPERIO_CTRL_FLEXEN_MASK)
#define SUPERIO_CTRL_SWRST_MASK                   0x2U
#define SUPERIO_CTRL_SWRST_SHIFT                  1U
#define SUPERIO_CTRL_SWRST_WIDTH                  1U
#define SUPERIO_CTRL_SWRST(x)                     (((uint32_t)(((uint32_t)(x))<<SUPERIO_CTRL_SWRST_SHIFT))&SUPERIO_CTRL_SWRST_MASK)
#define SUPERIO_CTRL_FASTACC_MASK                 0x4U
#define SUPERIO_CTRL_FASTACC_SHIFT                2U
#define SUPERIO_CTRL_FASTACC_WIDTH                1U
#define SUPERIO_CTRL_FASTACC(x)                   (((uint32_t)(((uint32_t)(x))<<SUPERIO_CTRL_FASTACC_SHIFT))&SUPERIO_CTRL_FASTACC_MASK)
#define SUPERIO_CTRL_DBGE_MASK                    0x40000000U
#define SUPERIO_CTRL_DBGE_SHIFT                   30U
#define SUPERIO_CTRL_DBGE_WIDTH                   1U
#define SUPERIO_CTRL_DBGE(x)                      (((uint32_t)(((uint32_t)(x))<<SUPERIO_CTRL_DBGE_SHIFT))&SUPERIO_CTRL_DBGE_MASK)
#define SUPERIO_CTRL_DOZEN_MASK                   0x80000000U
#define SUPERIO_CTRL_DOZEN_SHIFT                  31U
#define SUPERIO_CTRL_DOZEN_WIDTH                  1U
#define SUPERIO_CTRL_DOZEN(x)                     (((uint32_t)(((uint32_t)(x))<<SUPERIO_CTRL_DOZEN_SHIFT))&SUPERIO_CTRL_DOZEN_MASK)
/** PIN Bit Fields */
#define SUPERIO_PIN_PDI_MASK                      0xFFU
#define SUPERIO_PIN_PDI_SHIFT                     0U
#define SUPERIO_PIN_PDI_WIDTH                     8U
#define SUPERIO_PIN_PDI(x)                        (((uint32_t)(((uint32_t)(x))<<SUPERIO_PIN_PDI_SHIFT))&SUPERIO_PIN_PDI_MASK)
/** SHIFTSTAT Bit Fields */
#define SUPERIO_SHIFTSTAT_SSF_MASK                0xFU
#define SUPERIO_SHIFTSTAT_SSF_SHIFT               0U
#define SUPERIO_SHIFTSTAT_SSF_WIDTH               4U
#define SUPERIO_SHIFTSTAT_SSF(x)                  (((uint32_t)(((uint32_t)(x))<<SUPERIO_SHIFTSTAT_SSF_SHIFT))&SUPERIO_SHIFTSTAT_SSF_MASK)
/** SHIFTERR Bit Fields */
#define SUPERIO_SHIFTERR_SEF_MASK                 0xFU
#define SUPERIO_SHIFTERR_SEF_SHIFT                0U
#define SUPERIO_SHIFTERR_SEF_WIDTH                4U
#define SUPERIO_SHIFTERR_SEF(x)                   (((uint32_t)(((uint32_t)(x))<<SUPERIO_SHIFTERR_SEF_SHIFT))&SUPERIO_SHIFTERR_SEF_MASK)
/** TIMSTAT Bit Fields */
#define SUPERIO_TIMSTAT_TSF_MASK                  0xFU
#define SUPERIO_TIMSTAT_TSF_SHIFT                 0U
#define SUPERIO_TIMSTAT_TSF_WIDTH                 4U
#define SUPERIO_TIMSTAT_TSF(x)                    (((uint32_t)(((uint32_t)(x))<<SUPERIO_TIMSTAT_TSF_SHIFT))&SUPERIO_TIMSTAT_TSF_MASK)
/** SHIFTSIEN Bit Fields */
#define SUPERIO_SHIFTSIEN_SSIE_MASK               0xFU
#define SUPERIO_SHIFTSIEN_SSIE_SHIFT              0U
#define SUPERIO_SHIFTSIEN_SSIE_WIDTH              4U
#define SUPERIO_SHIFTSIEN_SSIE(x)                 (((uint32_t)(((uint32_t)(x))<<SUPERIO_SHIFTSIEN_SSIE_SHIFT))&SUPERIO_SHIFTSIEN_SSIE_MASK)
/** SHIFTEIEN Bit Fields */
#define SUPERIO_SHIFTEIEN_SEIE_MASK               0xFU
#define SUPERIO_SHIFTEIEN_SEIE_SHIFT              0U
#define SUPERIO_SHIFTEIEN_SEIE_WIDTH              4U
#define SUPERIO_SHIFTEIEN_SEIE(x)                 (((uint32_t)(((uint32_t)(x))<<SUPERIO_SHIFTEIEN_SEIE_SHIFT))&SUPERIO_SHIFTEIEN_SEIE_MASK)
/** TIMIEN Bit Fields */
#define SUPERIO_TIMIEN_TEIE_MASK                  0xFU
#define SUPERIO_TIMIEN_TEIE_SHIFT                 0U
#define SUPERIO_TIMIEN_TEIE_WIDTH                 4U
#define SUPERIO_TIMIEN_TEIE(x)                    (((uint32_t)(((uint32_t)(x))<<SUPERIO_TIMIEN_TEIE_SHIFT))&SUPERIO_TIMIEN_TEIE_MASK)
/** SHIFTSDEN Bit Fields */
#define SUPERIO_SHIFTSDEN_SSDE_MASK               0xFU
#define SUPERIO_SHIFTSDEN_SSDE_SHIFT              0U
#define SUPERIO_SHIFTSDEN_SSDE_WIDTH              4U
#define SUPERIO_SHIFTSDEN_SSDE(x)                 (((uint32_t)(((uint32_t)(x))<<SUPERIO_SHIFTSDEN_SSDE_SHIFT))&SUPERIO_SHIFTSDEN_SSDE_MASK)
/** SHIFTCTL Bit Fields */
#define SUPERIO_SHIFTCTL_SMOD_MASK                0x7U
#define SUPERIO_SHIFTCTL_SMOD_SHIFT               0U
#define SUPERIO_SHIFTCTL_SMOD_WIDTH               3U
#define SUPERIO_SHIFTCTL_SMOD(x)                  (((uint32_t)(((uint32_t)(x))<<SUPERIO_SHIFTCTL_SMOD_SHIFT))&SUPERIO_SHIFTCTL_SMOD_MASK)
#define SUPERIO_SHIFTCTL_PINPOL_MASK              0x80U
#define SUPERIO_SHIFTCTL_PINPOL_SHIFT             7U
#define SUPERIO_SHIFTCTL_PINPOL_WIDTH             1U
#define SUPERIO_SHIFTCTL_PINPOL(x)                (((uint32_t)(((uint32_t)(x))<<SUPERIO_SHIFTCTL_PINPOL_SHIFT))&SUPERIO_SHIFTCTL_PINPOL_MASK)
#define SUPERIO_SHIFTCTL_PINSEL_MASK              0x700U
#define SUPERIO_SHIFTCTL_PINSEL_SHIFT             8U
#define SUPERIO_SHIFTCTL_PINSEL_WIDTH             3U
#define SUPERIO_SHIFTCTL_PINSEL(x)                (((uint32_t)(((uint32_t)(x))<<SUPERIO_SHIFTCTL_PINSEL_SHIFT))&SUPERIO_SHIFTCTL_PINSEL_MASK)
#define SUPERIO_SHIFTCTL_PINCFG_MASK              0x30000U
#define SUPERIO_SHIFTCTL_PINCFG_SHIFT             16U
#define SUPERIO_SHIFTCTL_PINCFG_WIDTH             2U
#define SUPERIO_SHIFTCTL_PINCFG(x)                (((uint32_t)(((uint32_t)(x))<<SUPERIO_SHIFTCTL_PINCFG_SHIFT))&SUPERIO_SHIFTCTL_PINCFG_MASK)
#define SUPERIO_SHIFTCTL_TIMPOL_MASK              0x800000U
#define SUPERIO_SHIFTCTL_TIMPOL_SHIFT             23U
#define SUPERIO_SHIFTCTL_TIMPOL_WIDTH             1U
#define SUPERIO_SHIFTCTL_TIMPOL(x)                (((uint32_t)(((uint32_t)(x))<<SUPERIO_SHIFTCTL_TIMPOL_SHIFT))&SUPERIO_SHIFTCTL_TIMPOL_MASK)
#define SUPERIO_SHIFTCTL_TIMSEL_MASK              0x3000000U
#define SUPERIO_SHIFTCTL_TIMSEL_SHIFT             24U
#define SUPERIO_SHIFTCTL_TIMSEL_WIDTH             2U
#define SUPERIO_SHIFTCTL_TIMSEL(x)                (((uint32_t)(((uint32_t)(x))<<SUPERIO_SHIFTCTL_TIMSEL_SHIFT))&SUPERIO_SHIFTCTL_TIMSEL_MASK)
/** SHIFTCFG Bit Fields */
#define SUPERIO_SHIFTCFG_SSTART_MASK              0x3U
#define SUPERIO_SHIFTCFG_SSTART_SHIFT             0U
#define SUPERIO_SHIFTCFG_SSTART_WIDTH             2U
#define SUPERIO_SHIFTCFG_SSTART(x)                (((uint32_t)(((uint32_t)(x))<<SUPERIO_SHIFTCFG_SSTART_SHIFT))&SUPERIO_SHIFTCFG_SSTART_MASK)
#define SUPERIO_SHIFTCFG_SSTOP_MASK               0x30U
#define SUPERIO_SHIFTCFG_SSTOP_SHIFT              4U
#define SUPERIO_SHIFTCFG_SSTOP_WIDTH              2U
#define SUPERIO_SHIFTCFG_SSTOP(x)                 (((uint32_t)(((uint32_t)(x))<<SUPERIO_SHIFTCFG_SSTOP_SHIFT))&SUPERIO_SHIFTCFG_SSTOP_MASK)
#define SUPERIO_SHIFTCFG_INSRC_MASK               0x100U
#define SUPERIO_SHIFTCFG_INSRC_SHIFT              8U
#define SUPERIO_SHIFTCFG_INSRC_WIDTH              1U
#define SUPERIO_SHIFTCFG_INSRC(x)                 (((uint32_t)(((uint32_t)(x))<<SUPERIO_SHIFTCFG_INSRC_SHIFT))&SUPERIO_SHIFTCFG_INSRC_MASK)
/** SHIFTBUF Bit Fields */
#define SUPERIO_SHIFTBUF_SHIFTBUF_MASK            0xFFFFFFFFU
#define SUPERIO_SHIFTBUF_SHIFTBUF_SHIFT           0U
#define SUPERIO_SHIFTBUF_SHIFTBUF_WIDTH           32U
#define SUPERIO_SHIFTBUF_SHIFTBUF(x)              (((uint32_t)(((uint32_t)(x))<<SUPERIO_SHIFTBUF_SHIFTBUF_SHIFT))&SUPERIO_SHIFTBUF_SHIFTBUF_MASK)
/** SHIFTBUFBIS Bit Fields */
#define SUPERIO_SHIFTBUFBIS_SHIFTBUFBIS_MASK      0xFFFFFFFFU
#define SUPERIO_SHIFTBUFBIS_SHIFTBUFBIS_SHIFT     0U
#define SUPERIO_SHIFTBUFBIS_SHIFTBUFBIS_WIDTH     32U
#define SUPERIO_SHIFTBUFBIS_SHIFTBUFBIS(x)        (((uint32_t)(((uint32_t)(x))<<SUPERIO_SHIFTBUFBIS_SHIFTBUFBIS_SHIFT))&SUPERIO_SHIFTBUFBIS_SHIFTBUFBIS_MASK)
/** SHIFTBUFBYS Bit Fields */
#define SUPERIO_SHIFTBUFBYS_SHIFTBUFBYS_MASK      0xFFFFFFFFU
#define SUPERIO_SHIFTBUFBYS_SHIFTBUFBYS_SHIFT     0U
#define SUPERIO_SHIFTBUFBYS_SHIFTBUFBYS_WIDTH     32U
#define SUPERIO_SHIFTBUFBYS_SHIFTBUFBYS(x)        (((uint32_t)(((uint32_t)(x))<<SUPERIO_SHIFTBUFBYS_SHIFTBUFBYS_SHIFT))&SUPERIO_SHIFTBUFBYS_SHIFTBUFBYS_MASK)
/** SHIFTBUFBBS Bit Fields */
#define SUPERIO_SHIFTBUFBBS_SHIFTBUFBBS_MASK      0xFFFFFFFFU
#define SUPERIO_SHIFTBUFBBS_SHIFTBUFBBS_SHIFT     0U
#define SUPERIO_SHIFTBUFBBS_SHIFTBUFBBS_WIDTH     32U
#define SUPERIO_SHIFTBUFBBS_SHIFTBUFBBS(x)        (((uint32_t)(((uint32_t)(x))<<SUPERIO_SHIFTBUFBBS_SHIFTBUFBBS_SHIFT))&SUPERIO_SHIFTBUFBBS_SHIFTBUFBBS_MASK)
/** TIMCTL Bit Fields */
#define SUPERIO_TIMCTL_TIMOD_MASK                 0x3U
#define SUPERIO_TIMCTL_TIMOD_SHIFT                0U
#define SUPERIO_TIMCTL_TIMOD_WIDTH                2U
#define SUPERIO_TIMCTL_TIMOD(x)                   (((uint32_t)(((uint32_t)(x))<<SUPERIO_TIMCTL_TIMOD_SHIFT))&SUPERIO_TIMCTL_TIMOD_MASK)
#define SUPERIO_TIMCTL_PINPOL_MASK                0x80U
#define SUPERIO_TIMCTL_PINPOL_SHIFT               7U
#define SUPERIO_TIMCTL_PINPOL_WIDTH               1U
#define SUPERIO_TIMCTL_PINPOL(x)                  (((uint32_t)(((uint32_t)(x))<<SUPERIO_TIMCTL_PINPOL_SHIFT))&SUPERIO_TIMCTL_PINPOL_MASK)
#define SUPERIO_TIMCTL_PINSEL_MASK                0x700U
#define SUPERIO_TIMCTL_PINSEL_SHIFT               8U
#define SUPERIO_TIMCTL_PINSEL_WIDTH               3U
#define SUPERIO_TIMCTL_PINSEL(x)                  (((uint32_t)(((uint32_t)(x))<<SUPERIO_TIMCTL_PINSEL_SHIFT))&SUPERIO_TIMCTL_PINSEL_MASK)
#define SUPERIO_TIMCTL_PINCFG_MASK                0x30000U
#define SUPERIO_TIMCTL_PINCFG_SHIFT               16U
#define SUPERIO_TIMCTL_PINCFG_WIDTH               2U
#define SUPERIO_TIMCTL_PINCFG(x)                  (((uint32_t)(((uint32_t)(x))<<SUPERIO_TIMCTL_PINCFG_SHIFT))&SUPERIO_TIMCTL_PINCFG_MASK)
#define SUPERIO_TIMCTL_TRGSRC_MASK                0x400000U
#define SUPERIO_TIMCTL_TRGSRC_SHIFT               22U
#define SUPERIO_TIMCTL_TRGSRC_WIDTH               1U
#define SUPERIO_TIMCTL_TRGSRC(x)                  (((uint32_t)(((uint32_t)(x))<<SUPERIO_TIMCTL_TRGSRC_SHIFT))&SUPERIO_TIMCTL_TRGSRC_MASK)
#define SUPERIO_TIMCTL_TRGPOL_MASK                0x800000U
#define SUPERIO_TIMCTL_TRGPOL_SHIFT               23U
#define SUPERIO_TIMCTL_TRGPOL_WIDTH               1U
#define SUPERIO_TIMCTL_TRGPOL(x)                  (((uint32_t)(((uint32_t)(x))<<SUPERIO_TIMCTL_TRGPOL_SHIFT))&SUPERIO_TIMCTL_TRGPOL_MASK)
#define SUPERIO_TIMCTL_TRGSEL_MASK                0xF000000U
#define SUPERIO_TIMCTL_TRGSEL_SHIFT               24U
#define SUPERIO_TIMCTL_TRGSEL_WIDTH               4U
#define SUPERIO_TIMCTL_TRGSEL(x)                  (((uint32_t)(((uint32_t)(x))<<SUPERIO_TIMCTL_TRGSEL_SHIFT))&SUPERIO_TIMCTL_TRGSEL_MASK)
/** TIMCFG Bit Fields */
#define SUPERIO_TIMCFG_TSTART_MASK                0x2U
#define SUPERIO_TIMCFG_TSTART_SHIFT               1U
#define SUPERIO_TIMCFG_TSTART_WIDTH               1U
#define SUPERIO_TIMCFG_TSTART(x)                  (((uint32_t)(((uint32_t)(x))<<SUPERIO_TIMCFG_TSTART_SHIFT))&SUPERIO_TIMCFG_TSTART_MASK)
#define SUPERIO_TIMCFG_TSTOP_MASK                 0x30U
#define SUPERIO_TIMCFG_TSTOP_SHIFT                4U
#define SUPERIO_TIMCFG_TSTOP_WIDTH                2U
#define SUPERIO_TIMCFG_TSTOP(x)                   (((uint32_t)(((uint32_t)(x))<<SUPERIO_TIMCFG_TSTOP_SHIFT))&SUPERIO_TIMCFG_TSTOP_MASK)
#define SUPERIO_TIMCFG_TIMENA_MASK                0x700U
#define SUPERIO_TIMCFG_TIMENA_SHIFT               8U
#define SUPERIO_TIMCFG_TIMENA_WIDTH               3U
#define SUPERIO_TIMCFG_TIMENA(x)                  (((uint32_t)(((uint32_t)(x))<<SUPERIO_TIMCFG_TIMENA_SHIFT))&SUPERIO_TIMCFG_TIMENA_MASK)
#define SUPERIO_TIMCFG_TIMDIS_MASK                0x7000U
#define SUPERIO_TIMCFG_TIMDIS_SHIFT               12U
#define SUPERIO_TIMCFG_TIMDIS_WIDTH               3U
#define SUPERIO_TIMCFG_TIMDIS(x)                  (((uint32_t)(((uint32_t)(x))<<SUPERIO_TIMCFG_TIMDIS_SHIFT))&SUPERIO_TIMCFG_TIMDIS_MASK)
#define SUPERIO_TIMCFG_TIMRST_MASK                0x70000U
#define SUPERIO_TIMCFG_TIMRST_SHIFT               16U
#define SUPERIO_TIMCFG_TIMRST_WIDTH               3U
#define SUPERIO_TIMCFG_TIMRST(x)                  (((uint32_t)(((uint32_t)(x))<<SUPERIO_TIMCFG_TIMRST_SHIFT))&SUPERIO_TIMCFG_TIMRST_MASK)
#define SUPERIO_TIMCFG_TIMDEC_MASK                0x300000U
#define SUPERIO_TIMCFG_TIMDEC_SHIFT               20U
#define SUPERIO_TIMCFG_TIMDEC_WIDTH               2U
#define SUPERIO_TIMCFG_TIMDEC(x)                  (((uint32_t)(((uint32_t)(x))<<SUPERIO_TIMCFG_TIMDEC_SHIFT))&SUPERIO_TIMCFG_TIMDEC_MASK)
#define SUPERIO_TIMCFG_TIMOUT_MASK                0x3000000U
#define SUPERIO_TIMCFG_TIMOUT_SHIFT               24U
#define SUPERIO_TIMCFG_TIMOUT_WIDTH               2U
#define SUPERIO_TIMCFG_TIMOUT(x)                  (((uint32_t)(((uint32_t)(x))<<SUPERIO_TIMCFG_TIMOUT_SHIFT))&SUPERIO_TIMCFG_TIMOUT_MASK)
/** TIMCMP Bit Fields */
#define SUPERIO_TIMCMP_CMP_MASK                   0xFFFFU
#define SUPERIO_TIMCMP_CMP_SHIFT                  0U
#define SUPERIO_TIMCMP_CMP_WIDTH                  16U
#define SUPERIO_TIMCMP_CMP(x)                     (((uint32_t)(((uint32_t)(x))<<SUPERIO_TIMCMP_CMP_SHIFT))&SUPERIO_TIMCMP_CMP_MASK)
/** @brief      Timer comparator upper 8 bit mask*/
#define SUPERIO_PWM_TIMCMP_CMP_UPPER_MASK     (0x0000FF00UL)
#define SUPERIO_PWM_TIMCMP_CMP_UPPER_SHIFT    (0x8U)
#define SUPERIO_PWM_TIMCMP_CMP_UPPER(x)       (((uint32_t)(((uint32_t)(x)) << SUPERIO_PWM_TIMCMP_CMP_UPPER_SHIFT)) & SUPERIO_PWM_TIMCMP_CMP_UPPER_MASK)
/** @brief      Timer comparator lower 8 bit mask*/
#define SUPERIO_PWM_TIMCMP_CMP_LOWER_MASK     (0x000000FFUL)                               
#define SUPERIO_PWM_TIMCMP_CMP_LOWER_SHIFT    (0x0U)
#define SUPERIO_PWM_TIMCMP_CMP_LOWER(x)       (((uint32_t)(((uint32_t)(x)) << SUPERIO_PWM_TIMCMP_CMP_LOWER_SHIFT)) & SUPERIO_PWM_TIMCMP_CMP_LOWER_MASK)

/**
 * @}
 */ /* end of group SUPERIO_Register_Masks */

/**
 * @}
 */ /* end of group SUPERIO_Peripheral_Access_Layer */

/**
 * @}
 */ /* end of group Peripheral_Access_Layer_EAM2011 */

// clang-format on

#endif /* __SUPERIO_REGISTER_H__ */
