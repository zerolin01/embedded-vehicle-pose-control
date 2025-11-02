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
 * @file crg_register.h
 * @brief CRG driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __CRG_REGISTER_H__
#define __CRG_REGISTER_H__

#include "emps_platform.h"
#include <osal.h>
#include <stdint.h>

// clang-format off
/* ----------------------------------------------------------------------------
   -- CRG Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup CRG_Peripheral_Access_Layer CRG Peripheral Access Layer
 * @{
 */

/**
 * @brief CRG Register Layout Typedef
 */
typedef struct {
    __IO uint32_t DEBUG_CLKEN;           /**< offset: 0x0 */
    __IO uint32_t MEMORY_CLKEN;          /**< offset: 0x4 */
    __IO uint32_t MOTOR_CLKEN;           /**< offset: 0x8 */
    __IO uint32_t OCC_CLKEN;             /**< offset: 0xC */
    __IO uint32_t COM_CLKEN;             /**< offset: 0x10 */
    __IO uint32_t SPI_CLKEN;             /**< offset: 0x14 */
    __IO uint32_t CAN_CLKEN;             /**< offset: 0x18 */
    __IO uint32_t SPLL_REFDIV;           /**< offset: 0x1C */
    __IO uint32_t SPLL_DIVSET;           /**< offset: 0x20 */
    __IO uint32_t SPLL_PDPLL;            /**< offset: 0x24 */
    __IO uint32_t SPLL_POSDIV0;          /**< offset: 0x28 */
    __IO uint32_t SPLL_POSDIV1;          /**< offset: 0x2C */
    __IO uint32_t SPLL_POSDIV2;          /**< offset: 0x30 */
    __IO uint32_t TEST_REG;              /**< offset: 0x34 */
    __IO uint32_t RESERVED_0[2];         /**< offset: 0x38 */
    __IO uint32_t TEST_REGHI;            /**< offset: 0x40 */
    __IO uint32_t SPLL_LK;               /**< offset: 0x44 */
    __IO uint32_t TEST_CLKSEL;           /**< offset: 0x48 */
    __IO uint32_t TEST_CLKDIV;           /**< offset: 0x4C */
    __IO uint32_t SRCOSC_OSCEN;          /**< offset: 0x50 */
    __IO uint32_t SRCOSC_OSCCLKSEL;      /**< offset: 0x54 */
    __IO uint32_t SRCOSC_FMARGINSEL;     /**< offset: 0x58 */
    __IO uint32_t SRCOSC_FREQCTUNE;      /**< offset: 0x5C */
    __IO uint32_t SRCOSC_FREQFTUNE;      /**< offset: 0x60 */
    __IO uint32_t SRCOSC_CLKSTABLE;      /**< offset: 0x64 */
    __IO uint32_t SRCOSC_TRIMEN;         /**< offset: 0x68 */
    __IO uint32_t SRCOSC_STBCNT;         /**< offset: 0x6C */
    __IO uint32_t SRCOSC_REFCNT;         /**< offset: 0x70 */
    __IO uint32_t SRCOSC_TRIMFREQCTUNE;  /**< offset: 0x74 */
    __IO uint32_t SRCOSC_TRIMFREQFTUNE;  /**< offset: 0x78 */
    __IO uint32_t SRCOSC_TRIMDONE;       /**< offset: 0x7C */
    __IO uint32_t FRCOSC_OSCEN;          /**< offset: 0x80 */
    __IO uint32_t FRCOSC_OSCCLKSEL;      /**< offset: 0x84 */
    __IO uint32_t FRCOSC_FMARGINSEL;     /**< offset: 0x88 */
    __IO uint32_t FRCOSC_FREQCTUNE;      /**< offset: 0x8C */
    __IO uint32_t FRCOSC_FREQFTUNE;      /**< offset: 0x90 */
    __IO uint32_t FRCOSC_CLKSTABLE;      /**< offset: 0x94 */
    __IO uint32_t FRCOSC_STBCNT;         /**< offset: 0x98 */
    __IO uint32_t FRCOSC_TRIMEN;         /**< offset: 0x9C */
    __IO uint32_t FRCOSC_REFCNT;         /**< offset: 0xA0 */
    __IO uint32_t FRCOSC_TRIMFREQCTUNE;  /**< offset: 0xA4 */
    __IO uint32_t FRCOSC_TRIMFREQFTUNE;  /**< offset: 0xA8 */
    __IO uint32_t FRCOSC_TRIMDONE;       /**< offset: 0xAC */
    __IO uint32_t SRST_N;                /**< offset: 0xB0 */
    __IO uint32_t SRST1_N;               /**< offset: 0xB4 */
    __IO uint32_t IPG0_CLKEN;            /**< offset: 0xB8 */
    __IO uint32_t IPG1_CLKEN;            /**< offset: 0xBC */
    __IO uint32_t IPG2_CLKEN;            /**< offset: 0xC0 */
    __IO uint32_t IPG3_CLKEN;            /**< offset: 0xC4 */
    __IO uint32_t PLL_SRCSEL;            /**< offset: 0xC8 */
    __IO uint32_t LPCK_SRCSEL;           /**< offset: 0xCC */
    __IO uint32_t WORK_CLKGFSEL;         /**< offset: 0xD0 */
    __IO uint32_t WORK_CKGFACTIVE;       /**< offset: 0xD4 */
    __IO uint32_t MAINBUS_CLKDIV;        /**< offset: 0xD8 */
    __IO uint32_t MAINBUS_MOTOR_CLKDIV;  /**< offset: 0xDC */
    __IO uint32_t MAINBUS_AUX_CLKDIV;    /**< offset: 0xE0 */
    __IO uint32_t PERIPHBUS_CLKDIV;      /**< offset: 0xE4 */
    __IO uint32_t EIP130_CLKDIV;         /**< offset: 0xE8 */
    __IO uint32_t EIP130_CNT_CLKDIV;     /**< offset: 0xEC */
    __IO uint32_t SUPERTMR_CLKDIV;       /**< offset: 0xF0 */
    __IO uint32_t PLLFUNC_CLKDIV;        /**< offset: 0xF4 */
    __IO uint32_t PLLDFUNC_CLKDIV;       /**< offset: 0xF8 */
    __IO uint32_t SUPERIO_CLKDIV;        /**< offset: 0xFC */
    __IO uint32_t PERIPH_PLL_CLKDIV;     /**< offset: 0x100 */
    __IO uint32_t PERIPH_PLL_AUX_CLKDIV; /**< offset: 0x104 */
    __IO uint32_t FASTAUX_CLKDIV;        /**< offset: 0x108 */
    __IO uint32_t ADCFUNC_CLKDIV;        /**< offset: 0x10C */
    __IO uint32_t ADC_CLKSEL;            /**< offset: 0x110 */
    __IO uint32_t DEBOUNCE_CLKDIV;       /**< offset: 0x114 */
    __IO uint32_t LP32K_CLKDIV;          /**< offset: 0x118 */
    __IO uint32_t LPOCK_CLKSEL;          /**< offset: 0x11C */
    __IO uint32_t RESERVED_1[20];
    __IO uint32_t SUPERTMR0_TCLKSEL;     /**< offset: 0x170 */
    __IO uint32_t SUPERTMR1_TCLKSEL;     /**< offset: 0x174 */
    __IO uint32_t SUPERTMR2_TCLKSEL;     /**< offset: 0x178 */
    __IO uint32_t SUPERTMR3_TCLKSEL;     /**< offset: 0x17C */
    __IO uint32_t SUPERTMR4_TCLKSEL;     /**< offset: 0x180 */
    __IO uint32_t SUPERTMR5_TCLKSEL;     /**< offset: 0x184 */
    __IO uint32_t CPURTC_CLKDIV;         /**< offset: 0x188 */
    __IO uint32_t GTMR_CLKEN;            /**< offset: 0x18C */
    __IO uint32_t PITMR1_CLKEN;          /**< offset: 0x190 */
    __IO uint32_t RESERVED_2;            /**< offset: 0x194 */
    __IO uint32_t SRST2_N;               /**< offset: 0x198 */
} crg_type_t, *crg_mem_map_ptr;

/**
 * @brief XTAL Register Layout Typedef
 */
typedef struct {
    __IO uint32_t xtal;                  /**< offset: 0x0 */
} xtal_type_t, *xtal_mem_map_ptr;
/* ----------------------------------------------------------------------------
   -- CRG Register Masks
   ---------------------------------------------------------------------------- */
/**
 * @addtogroup CRG_Register_Masks CRG Register Masks
 * @{
 */

/** IPG0_CLKEN Bit Fields */
#define CRG_OCC_CLKEN_ON                        0xA12U
#define CRG_OCC_CLKEN_MASK                      0xB77U
#define CRG_OCC_CLKEN_SHIFT                     0U
#define CRG_OCC_CLKEN_WIDTH                     12U
#define CRG_OCC_CLKEN(x)                        (((uint32_t)(((uint32_t)(x))<<CRG_IPG3_CLKEN_SHIFT))&CRG_IPG3_CLKEN_MASK)
/** SPLL_PDPLL Bit Fields */
#define CRG_SPLL_PDPLL_MASK                     0x3FU
#define CRG_SPLL_PDPLL_SHIFT                    0U
#define CRG_SPLL_PDPLL_WIDTH                    6U
#define CRG_SPLL_PDPLL(x)                       (((uint32_t)(((uint32_t)(x))<<CRG_SPLL_PDPLL_SHIFT))&CRG_SPLL_PDPLL_MASK)
#define CRG_SPLL_PDPLL_CORE_MASK                0x1U
#define CRG_SPLL_PDPLL_CORE_SHIFT               0U
#define CRG_SPLL_PDPLL_CORE_WIDTH               1U
#define CRG_SPLL_PDPLL_CORE(x)                  (((uint32_t)(((uint32_t)(x))<<CRG_SPLL_PDPLL_CORE_SHIFT))&CRG_SPLL_PDPLL_CORE_MASK)
#define CRG_SPLL_PDPLL_POSDIV_MASK              0x1CU
#define CRG_SPLL_PDPLL_POSDIV_SHIFT             2U
#define CRG_SPLL_PDPLL_POSDIV_WIDTH             3U
#define CRG_SPLL_PDPLL_POSDIV(x)                (((uint32_t)(((uint32_t)(x))<<CRG_SPLL_PDPLL_POSDIV_SHIFT))&CRG_SPLL_PDPLL_POSDIV_MASK)
#define CRG_SPLL_PDPLL_BIAS_MASK                0x20U
#define CRG_SPLL_PDPLL_BIAS_SHIFT               5U
#define CRG_SPLL_PDPLL_BIAS_WIDTH               1U
#define CRG_SPLL_PDPLL_BIAS(x)                  (((uint32_t)(((uint32_t)(x))<<CRG_SPLL_PDPLL_BIAS_SHIFT))&CRG_SPLL_PDPLL_BIAS_MASK)
/** SPPL_SRCSEL Bit Fields */
#define CRG_SPLL_SRCSEL_MASK                    0x1U
#define CRG_SPLL_SRCSEL_SHIFT                   0U
#define CRG_SPLL_SRCSEL_WIDTH                   1U
#define CRG_SPLL_SRCSEL(x)                      (((uint32_t)(((uint32_t)(x))<<CRG_SPLL_SRCSEL_SHIFT))&CRG_SPLL_SRCSEL_MASK)
/** SPLL_REFDIV Bit Fields */
#define CRG_SPLL_REFDIV_MASK                    0xFU
#define CRG_SPLL_REFDIV_SHIFT                   0U
#define CRG_SPLL_REFDIV_WIDTH                   4U
#define CRG_SPLL_REFDIV(x)                      (((uint32_t)(((uint32_t)(x))<<CRG_SPLL_REFDIV_SHIFT))&CRG_SPLL_REFDIV_MASK)
/** SPLL_DIVSET Bit Fields */
#define CRG_SPLL_DEVSET_INTEGER_MASK            0x3E000U
#define CRG_SPLL_DEVSET_INTEGER_SHIFT           13U
#define CRG_SPLL_DEVSET_INTEGER_WIDTH           5U
#define CRG_SPLL_DEVSET_INTEGER(x)              (((uint32_t)(((uint32_t)(x))<<CRG_SPLL_DEVSET_INTEGER_SHIFT))&CRG_SPLL_DEVSET_INTEGER_MASK)
#define CRG_SPLL_DEVSET_FRACT_MASK              0x1FFFU
#define CRG_SPLL_DEVSET_FRACT_SHIFT             0U
#define CRG_SPLL_DEVSET_FRACT_WIDTH             13U
#define CRG_SPLL_DEVSET_FRACT(x)                (((uint32_t)(((uint32_t)(x))<<CRG_SPLL_DEVSET_FRACT_SHIFT))&CRG_SPLL_DEVSET_FRACT_MASK)
/** SPLL_POSDIV Bit Fields */
#define CRG_SPLL_POSDIV_MASK                    0xFU
#define CRG_SPLL_POSDIV_SHIFT                   0U
#define CRG_SPLL_POSDIV_WIDTH                   4U
#define CRG_SPLL_POSDIV(x)                      (((uint32_t)(((uint32_t)(x))<<CRG_SPLL_POSDIV_SHIFT))&CRG_SPLL_POSDIV_MASK)
/** SPLL_LK Bit Fields */
#define CRG_SPLL_PLLLK_MASK                     0x1U
#define CRG_SPLL_PLLLK_SHIFT                    0U
#define CRG_SPLL_PLLLK_WIDTH                    1U
#define CRG_SPLL_PLLLK(x)                       (((uint32_t)(((uint32_t)(x))<<CRG_SPLL_PLLLK_SHIFT))&CRG_SPLL_PLLLK_MASK)
/** LPCK_CLKGFSEL Bit Fields */
#define CRG_LPCK_CLKSEL_MASK                    0x1U
#define CRG_LPCK_CLKSEL_SHIFT                   0U
#define CRG_LPCK_CLKSEL_WIDTH                   1U
#define CRG_LPCK_CLKSEL(x)                      (((uint32_t)(((uint32_t)(x))<<CRG_LPCK_CLKSEL_SHIFT))&CRG_LPCK_CLKSEL_MASK)
/** WORK_CLKGFSEL Bit Fields */
#define CRG_WORK_CLKSEL_MASK                    0x3U
#define CRG_WORK_CLKSEL_SHIFT                   0U
#define CRG_WORK_CLKSEL_WIDTH                   2U
#define CRG_WORK_CLKSEL(x)                      (((uint32_t)(((uint32_t)(x))<<CRG_WORK_CLKSEL_SHIFT))&CRG_WORK_CLKSEL_MASK)
/** SRCOSC_OSC_EN Bit Fields */
#define CRG_SRCOSC_OSCEN_MASK                   0x3U
#define CRG_SRCOSC_OSCEN_SHIFT                  0U
#define CRG_SRCOSC_OSCEN_WIDTH                  2U
#define CRG_SRCOSC_OSCEN(x)                     (((uint32_t)(((uint32_t)(x))<<CRG_SRCOSC_OSCEN_SHIFT))&CRG_SRCOSC_OSCEN_MASK)
/** SRCOSC_OSCCLKSEL Bit Fields */
#define CRG_SRCOSC_OSCCLKSEL_MASK               0x3U
#define CRG_SRCOSC_OSCCLKSEL_SHIFT              0U
#define CRG_SRCOSC_OSCCLKSEL_WIDTH              2U
#define CRG_SRCOSC_OSCCLKSEL(x)                 (((uint32_t)(((uint32_t)(x))<<CRG_SRCOSC_OSCCLKSEL_SHIFT))&CRG_SRCOSC_OSCCLKSEL_MASK)
/** SRCOSC_FMARGINSEL Bit Fields */
#define CRG_SRCOSC_FMARGINSEL_MASK              0x3U
#define CRG_SRCOSC_FMARGINSEL_SHIFT             0U
#define CRG_SRCOSC_FMARGINSEL_WIDTH             2U
#define CRG_SRCOSC_FMARGINSEL(x)                (((uint32_t)(((uint32_t)(x))<<CRG_SRCOSC_FMARGINSEL_SHIFT))&CRG_SRCOSC_FMARGINSEL_MASK)
/** SRCOSC_FREQ_CTUNE Bit Fields */
#define CRG_SRCOSC_FREQ_CTUNE_MASK              0xFFU
#define CRG_SRCOSC_FREQ_CTUNE_SHIFT             0U
#define CRG_SRCOSC_FREQ_CTUNE_WIDTH             8U
#define CRG_SRCOSC_FREQ_CTUNE(x)                (((uint32_t)(((uint32_t)(x))<<CRG_SRCOSC_FREQ_CTUNE_SHIFT))&CRG_SRCOSC_FREQ_CTUNE_MASK)
/** SRCOSC_FREQ_FTUNE Bit Fields */
#define CRG_SRCOSC_FREQ_FTUNE_MASK              0x3FU
#define CRG_SRCOSC_FREQ_FTUNE_SHIFT             0U
#define CRG_SRCOSC_FREQ_FTUNE_WIDTH             6U
#define CRG_SRCOSC_FREQ_FTUNE(x)                (((uint32_t)(((uint32_t)(x))<<CRG_SRCOSC_FREQ_CTUNE_SHIFT))&CRG_SRCOSC_FREQ_CTUNE_MASK)
/** SRCOSC_CLK_STABLE Bit Fields */
#define CRG_SRCOSC_CLK_STABLE_MASK              0x1U
#define CRG_SRCOSC_CLK_STABLE_SHIFT             0U
#define CRG_SRCOSC_CLK_STABLE_WIDTH             1U
#define CRG_SRCOSC_CLK_STABLE(x)                (((uint32_t)(((uint32_t)(x))<<CRG_SRCOSC_CLK_STABLE_SHIFT))&CRG_SRCOSC_CLK_STABLE_MASK)
/** SRCOSC_TRIM_EN Bit Fields */
#define CRG_SRCOSC_TRIMEN_MASK                  0x1U
#define CRG_SRCOSC_TRIMEN_SHIFT                 0U
#define CRG_SRCOSC_TRIMEN_WIDTH                 1U
#define CRG_SRCOSC_TRIMEN(x)                    (((uint32_t)(((uint32_t)(x))<<CRG_SRCOSC_TRIMEN_SHIFT))&CRG_SRCOSC_TRIMEN_MASK)
/** SRCOSC_STB_CNT Bit Fields */
#define CRG_SRCOSC_STBCNT_MASK                  0x3FFU
#define CRG_SRCOSC_STBCNT_SHIFT                 0U
#define CRG_SRCOSC_STBCNT_WIDTH                 10U
#define CRG_SRCOSC_STBCNT(x)                    (((uint32_t)(((uint32_t)(x))<<CRG_SRCOSC_STBCNT_SHIFT))&CRG_SRCOSC_STBCNT_MASK)
/** SRCOSC_REF_CNT Bit Fields */
#define CRG_SRCOSC_REFCNT_MASK                  0x3FFFU
#define CRG_SRCOSC_REFCNT_SHIFT                 0U
#define CRG_SRCOSC_REFCNT_WIDTH                 18U
#define CRG_SRCOSC_REFCNT(x)                    (((uint32_t)(((uint32_t)(x))<<CRG_SRCOSC_REFCNT_SHIFT))&CRG_SRCOSC_REFCNT_MASK)
/** SRCOSC_TRIM_FREQ_CTUNE Bit Fields */
#define CRG_SRCOSC_TRIM_FREQCTUNE_MASK          0xFFU
#define CRG_SRCOSC_TRIM_FREQCTUNE_SHIFT         0U
#define CRG_SRCOSC_TRIM_FREQCTUNE_WIDTH         8U
#define CRG_SRCOSC_TRIM_FREQCTUNE(x)            (((uint32_t)(((uint32_t)(x))<<CRG_SRCOSC_TRIM_FREQCTUNE_SHIFT))&CRG_SRCOSC_TRIM_FREQCTUNE_MASK)
/** SRCOSC_TRIM_FREQ_FTUNE Bit Fields */
#define CRG_SRCOSC_TRIM_FREQFTUNE_MASK          0x3FU
#define CRG_SRCOSC_TRIM_FREQFTUNE_SHIFT         0U
#define CRG_SRCOSC_TRIM_FREQFTUNE_WIDTH         6U
#define CRG_SRCOSC_TRIM_FREQFTUNE(x)            (((uint32_t)(((uint32_t)(x))<<CRG_SRCOSC_TRIM_FREQFTUNE_SHIFT))&CRG_SRCOSC_TRIM_FREQFTUNE_MASK)
/** SRCOSC_TRIM_DONE Bit Fields */
#define CRG_SRCOSC_TRIMDONE_MASK                0x1U
#define CRG_SRCOSC_TRIMDONE_SHIFT               0U
#define CRG_SRCOSC_TRIMDONE_WIDTH               1U
#define CRG_SRCOSC_TRIMDONE(x)                  (((uint32_t)(((uint32_t)(x))<<CRG_SRCOSC_TRIMDONE_SHIFT))&CRG_SRCOSC_TRIMDONE_MASK)
/** FRCOSC_OSC_EN Bit Fields */
#define CRG_FRCOSC_OSCEN_MASK                   0x3U
#define CRG_FRCOSC_OSCEN_SHIFT                  0U
#define CRG_FRCOSC_OSCEN_WIDTH                  2U
#define CRG_FRCOSC_OSCEN(x)                     (((uint32_t)(((uint32_t)(x))<<CRG_FRCOSC_OSCEN_SHIFT))&CRG_FRCOSC_OSCEN_MASK)
/** FRCOSC_OSCCLKSEL Bit Fields */
#define CRG_FRCOSC_OSCCLKSEL_MASK               0x3U
#define CRG_FRCOSC_OSCCLKSEL_SHIFT              0U
#define CRG_FRCOSC_OSCCLKSEL_WIDTH              2U
#define CRG_FRCOSC_OSCCLKSEL(x)                 (((uint32_t)(((uint32_t)(x))<<CRG_FRCOSC_OSCCLKSEL_SHIFT))&CRG_FRCOSC_OSCCLKSEL_MASK)
/** FRCOSC_FMARGINSEL Bit Fields */
#define CRG_FRCOSC_FMARGINSEL_MASK              0x3U
#define CRG_FRCOSC_FMARGINSEL_SHIFT             0U
#define CRG_FRCOSC_FMARGINSEL_WIDTH             2U
#define CRG_FRCOSC_FMARGINSEL(x)                (((uint32_t)(((uint32_t)(x))<<CRG_FRCOSC_FMARGINSEL_SHIFT))&CRG_FRCOSC_FMARGINSEL_MASK)
/** FRCOSC_FREQ_CTUNE Bit Fields */
#define CRG_FRCOSC_FREQ_CTUNE_MASK              0xFFU
#define CRG_FRCOSC_FREQ_CTUNE_SHIFT             0U
#define CRG_FRCOSC_FREQ_CTUNE_WIDTH             8U
#define CRG_FRCOSC_FREQ_CTUNE(x)                (((uint32_t)(((uint32_t)(x))<<CRG_FRCOSC_FREQ_CTUNE_SHIFT))&CRG_FRCOSC_FREQ_CTUNE_MASK)
/** FRCOSC_FREQ_FTUNE Bit Fields */
#define CRG_FRCOSC_FREQ_FTUNE_MASK              0x3FU
#define CRG_FRCOSC_FREQ_FTUNE_SHIFT             0U
#define CRG_FRCOSC_FREQ_FTUNE_WIDTH             6U
#define CRG_FRCOSC_FREQ_FTUNE(x)                (((uint32_t)(((uint32_t)(x))<<CRG_FRCOSC_FREQ_CTUNE_SHIFT))&CRG_FRCOSC_FREQ_CTUNE_MASK)
/** FRCOSC_CLK_STABLE Bit Fields */
#define CRG_FRCOSC_CLK_STABLE_MASK              0x1U
#define CRG_FRCOSC_CLK_STABLE_SHIFT             0U
#define CRG_FRCOSC_CLK_STABLE_WIDTH             1U
#define CRG_FRCOSC_CLK_STABLE(x)                (((uint32_t)(((uint32_t)(x))<<CRG_FRCOSC_CLK_STABLE_SHIFT))&CRG_FRCOSC_CLK_STABLE_MASK)
/** FRCOSC_TRIM_EN Bit Fields */
#define CRG_FRCOSC_TRIMEN_MASK                  0x1U
#define CRG_FRCOSC_TRIMEN_SHIFT                 0U
#define CRG_FRCOSC_TRIMEN_WIDTH                 1U
#define CRG_FRCOSC_TRIMEN(x)                    (((uint32_t)(((uint32_t)(x))<<CRG_FRCOSC_TRIMEN_SHIFT))&CRG_FRCOSC_TRIMEN_MASK)
/** FRCOSC_STB_CNT Bit Fields */
#define CRG_FRCOSC_STBCNT_MASK                  0x3FFU
#define CRG_FRCOSC_STBCNT_SHIFT                 0U
#define CRG_FRCOSC_STBCNT_WIDTH                 10U
#define CRG_FRCOSC_STBCNT(x)                    (((uint32_t)(((uint32_t)(x))<<CRG_FRCOSC_STBCNT_SHIFT))&CRG_FRCOSC_STBCNT_MASK)
/** FRCOSC_REF_CNT Bit Fields */
#define CRG_FRCOSC_REFCNT_MASK                  0x3FFFU
#define CRG_FRCOSC_REFCNT_SHIFT                 0U
#define CRG_FRCOSC_REFCNT_WIDTH                 18U
#define CRG_FRCOSC_REFCNT(x)                    (((uint32_t)(((uint32_t)(x))<<CRG_FRCOSC_REFCNT_SHIFT))&CRG_FRCOSC_REFCNT_MASK)
/** FRCOSC_TRIM_FREQ_CTUNE Bit Fields */
#define CRG_FRCOSC_TRIM_FREQCTUNE_MASK          0xFFU
#define CRG_FRCOSC_TRIM_FREQCTUNE_SHIFT         0U
#define CRG_FRCOSC_TRIM_FREQCTUNE_WIDTH         8U
#define CRG_FRCOSC_TRIM_FREQCTUNE(x)            (((uint32_t)(((uint32_t)(x))<<CRG_FRCOSC_TRIM_FREQCTUNE_SHIFT))&CRG_FRCOSC_TRIM_FREQCTUNE_MASK)
/** FRCOSC_TRIM_FREQ_FTUNE Bit Fields */
#define CRG_FRCOSC_TRIM_FREQFTUNE_MASK          0x3FU
#define CRG_FRCOSC_TRIM_FREQFTUNE_SHIFT         0U
#define CRG_FRCOSC_TRIM_FREQFTUNE_WIDTH         6U
#define CRG_FRCOSC_TRIM_FREQFTUNE(x)            (((uint32_t)(((uint32_t)(x))<<CRG_FRCOSC_TRIM_FREQFTUNE_SHIFT))&CRG_FRCOSC_TRIM_FREQFTUNE_MASK)
/** FRCOSC_TRIM_DONE Bit Fields */
#define CRG_FRCOSC_TRIMDONE_MASK                0x1U
#define CRG_FRCOSC_TRIMDONE_SHIFT               0U
#define CRG_FRCOSC_TRIMDONE_WIDTH               1U
#define CRG_FRCOSC_TRIMDONE(x)                  (((uint32_t)(((uint32_t)(x))<<CRG_FRCOSC_TRIMDONE_SHIFT))&CRG_FRCOSC_TRIMDONE_MASK)
/** IPG0_CLKEN Bit Fields */
#define CRG_IPG0_CLKEN_MASK                     0xFFFFFFFFU
#define CRG_IPG0_CLKEN_SHIFT                    0U
#define CRG_IPG0_CLKEN_WIDTH                    32U
#define CRG_IPG0_CLKEN(x)                       (((uint32_t)(((uint32_t)(x))<<CRG_IPG0_CLKEN_SHIFT))&CRG_IPG0_CLKEN_MASK)
/** IPG0_CLKEN Bit Fields */
#define CRG_IPG1_CLKEN_MASK                     0xFFFFFFFFU
#define CRG_IPG1_CLKEN_SHIFT                    0U
#define CRG_IPG1_CLKEN_WIDTH                    32U
#define CRG_IPG1_CLKEN(x)                       (((uint32_t)(((uint32_t)(x))<<CRG_IPG1_CLKEN_SHIFT))&CRG_IPG1_CLKEN_MASK)
/** IPG0_CLKEN Bit Fields */
#define CRG_IPG2_CLKEN_MASK                     0xFFFFFFFFU
#define CRG_IPG2_CLKEN_SHIFT                    0U
#define CRG_IPG2_CLKEN_WIDTH                    32U
#define CRG_IPG2_CLKEN(x)                       (((uint32_t)(((uint32_t)(x))<<CRG_IPG2_CLKEN_SHIFT))&CRG_IPG2_CLKEN_MASK)
/** IPG0_CLKEN Bit Fields */
#define CRG_IPG3_CLKEN_MASK                     0x7FFFU
#define CRG_IPG3_CLKEN_SHIFT                    0U
#define CRG_IPG3_CLKEN_WIDTH                    12U
#define CRG_IPG3_CLKEN(x)                       (((uint32_t)(((uint32_t)(x))<<CRG_IPG3_CLKEN_SHIFT))&CRG_IPG3_CLKEN_MASK)
/** SRST_N Bit Fields */
#define CRG_SRST_N_DEF                          0x80000001U
#define CRG_SRST_N_MASK                         0xFFFFFFFFU
#define CRG_SRST_N_SHIFT                        0U
#define CRG_SRST_N_WIDTH                        32U
#define CRG_SRST_N(x)                           (((uint32_t)(((uint32_t)(x))<<CRG_SRST_N_SHIFT))&CRG_SRST_N_MASK)
/** SRSRT1_N Bit Fields */
#define CRG_SRST1_N_DEF                         0x200U
#define CRG_SRST1_N_MASK                        0xFFFFFFFFU
#define CRG_SRST1_N_SHIFT                       0U
#define CRG_SRST1_N_WIDTH                       32U
#define CRG_SRST1_N(x)                          (((uint32_t)(((uint32_t)(x))<<CRG_SRST1_N_SHIFT))&CRG_SRST1_N_MASK)
/** SRSRT2_N Bit Fields */
#define CRG_SRST2_N_DEF                         0x0U
#define CRG_SRST2_N_MASK                        0xFU
#define CRG_SRST2_N_SHIFT                       0U
#define CRG_SRST2_N_WIDTH                       4U
#define CRG_SRST2_N(x)                          (((uint32_t)(((uint32_t)(x))<<CRG_SRST2_N_SHIFT))&CRG_SRST2_N_MASK)

/** XTAL RF Bit Fields */
#define PAD_XRAL_RF_MASK                        0x200U
#define PAD_XRAL_RF_SHIFT                       9U
#define PAD_XRAL_RF_WIDTH                       1U
#define PAD_XRAL_RF(x)                          (((uint32_t)(((uint32_t)(x))<<PAD_XRAL_RF_SHIFT))&PAD_XRAL_RF_MASK)
/** XTAL AGC Bit Fields */
#define PAD_XRAL_AGC_MASK                       0x180U
#define PAD_XRAL_AGC_SHIFT                      7U
#define PAD_XRAL_AGC_WIDTH                      2U
#define PAD_XRAL_ACG(x)                         (((uint32_t)(((uint32_t)(x))<<PAD_XRAL_AGC_SHIFT))&PAD_XRAL_AGC_MASK)
/** XTAL AGC_BYPSS Bit Fields */
#define PAD_XRAL_AGC_BYPASS_MASK                0x40U
#define PAD_XRAL_AGC_BYPASS_SHIFT               6U
#define PAD_XRAL_AGC_BYPASS_WIDTH               1U
#define PAD_XRAL_AGC_BYPASS(x)                  (((uint32_t)(((uint32_t)(x))<<PAD_XRAL_AGC_BYPASS_SHIFT))&PAD_XRAL_AGC_BYPASS_MASK)
/** XTAL XTALMF Bit Fields */
#define PAD_XRAL_XTALMF_MASK                    0x3CU
#define PAD_XRAL_XTALMF_SHIFT                   2U
#define PAD_XRAL_XTALMF_WIDTH                   4U
#define PAD_XRAL_XTALMF(x)                      (((uint32_t)(((uint32_t)(x))<<PAD_XRAL_XTALMF_SHIFT))&PAD_XRAL_XTALMF_MASK)
/** XTAL BYP Bit Fields */
#define PAD_XRAL_BYP_MASK                       0x02U
#define PAD_XRAL_BYP_SHIFT                      1U
#define PAD_XRAL_BYP_WIDTH                      1U
#define PAD_XRAL_BYP(x)                         (((uint32_t)(((uint32_t)(x))<<PAD_XRAL_BYP_SHIFT))&PAD_XRAL_BYP_MASK)
/** XTAL EN Bit Fields */
#define PAD_XRAL_EN_MASK                        0x1U
#define PAD_XRAL_EN_SHIFT                       0U
#define PAD_XRAL_EN_WIDTH                       1U
#define PAD_XRAL_EN(x)                          (((uint32_t)(((uint32_t)(x))<<PAD_XRAL_EN_SHIFT))&PAD_XRAL_EN_MASK)

/**
 * @}
 */ /* end of group CRG_Register_Masks */

/**
 * @}
 */ /* end of group CRG_Peripheral_Access_Layer */
// clang-format on

#endif // __CRG_REGISTER_H__
