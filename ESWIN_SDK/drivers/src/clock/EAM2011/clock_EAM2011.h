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
 * @file clock_EAM2011.h
 * @brief Clock EAM2011 driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __CLOCK_EAM2011_H__
#define __CLOCK_EAM2011_H__

#include "emps_platform.h"
#include <status.h>
#include <stdbool.h>
#include <stdint.h>

#define ARRAY_SIZE(x)             (sizeof(x) / sizeof(x[0]))
#define toClockMask(shift, width) (((1 << (width)) - 1) << shift)

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief CRG clock category.
 */
typedef enum {
    CRG_FIXED_RATE_CLOCK   = 0x1U,  /**< Fixed rate clock. */
    CRG_FIXED_FACTOR_CLOCK = 0x2U,  /**< Fixed divider clock. */
    CRG_GATE_CLOCK         = 0x4U,  /**< Gate clock. */
    CRG_DIV_CLOCK          = 0x8U,  /**< Divider clock. */
    CRG_MUX_CLOCK          = 0xCU,  /**< Multiplex clock. */
    CRG_EXT_CLOCK          = 0x10U, /**< Ext CLK. */
    CRG_RCOSC_CLOCK        = 0x20U, /**< RCOSC . */
    CRG_SPLL_CLOCK         = 0x40U, /**< SPLL . */
} clock_flag_t;

/**
 * @brief CRG RCOSC clock source.
 */
typedef enum {
    CRG_RCOSC_OSCCLK_S1_MID_8MHZ  = 0U, /**< fS1_MID =  8MHz. */
    CRG_RCOSC_OSCCLK_S2_MID_16MHZ = 1U, /**< fS2_MID = 16MHz. */
    CRG_RCOSC_OSCCLK_S3_MID_32MHZ = 2U, /**< fS3_MID = 32MHz. */
    CRG_RCOSC_OSCCLK_S4_MID_64MHZ = 3U, /**< fS4_MID = 64MHz. */
} crg_rcosc_oscclk_sel_t;

/**
 * @brief CRG RCOSC fmargin.
 */
typedef enum {
    CRG_RCOSC_FMARGIN_S1 = 0U, /**< 0.7 × f S*_MID~0.9 × fS*_MID. */
    CRG_RCOSC_FMARGIN_S2 = 1U, /**< 0.9 × f S*_MID~1.1 × fS*_MID. */
    CRG_RCOSC_FMARGIN_S3 = 2U, /**< 1.1 × f S*_MID~1.3 × fS*_MID. */
    CRG_RCOSC_FMARGIN_S4 = 3U, /**< 1.3 × f S*_MID~1.5 × fS*_MID. */
} crg_rcosc_fmargin_sel_t;

/**
 * @brief CRG SPLL clock source.
 */
typedef enum {
    CRG_SPLL_SRC_FROSC_CLK = 0U, /**< Fast RCOSC CLOCK. */
    CRG_SPLL_SRC_SOSC_CLK  = 1U, /**< System SOC. */
} crg_pll_refclk_sel_t;

/**
 * @brief CRG Fast Clock source.
 */
typedef enum {
    CRG_FASTCLK_SRC_FROSC_CLK = 0U, /**< Fast RCOSC CLOCK. */
    CRG_FASTCLK_SRC_SOSC_CLK  = 1U, /**< System SOC. */
} crg_fastclk_sel_t;

/**
 * @brief CRG LPCK clock source.
 */
typedef enum {
    CRG_LPCK_SRC_SROSC_CLK = 0U, /**< Slow RCOSC CLOCK. */
    CRG_LPCK_SRC_AON_CLK   = 1U, /**< AON CLOCK. */
} crg_lpckclk_sel_t;

/**
 * @brief CRG wdog clock source.
 */
typedef enum {
    CRG_WDOG_SRC_LPSOC_CLK   = 0U, /**< LP OSC clock. */
    CRG_WDOG_SRC_LP1K_CLK    = 1U, /**< LP 1k clock. */
    CRG_WDOG_SRC_PMU_AON_CLK = 2U, /**< AON clock. */
    CRG_WDOG_SRC_RTCOSC_CLK  = 3U, /**< RTC OSC clock. */
} crg_wdog_sel_t;

/**
 * @brief CRG rtc clock source.
 */
typedef enum {
    CRG_RTC_SRC_LPOSC_CLK  = 0U, /**< LP OSC Clock. */
    CRG_RTC_SRC_RTCOSC_CLK = 1U, /**< RTC OSC Clock. */
} crg_rtcclk_sel_t;

/**
 * @brief CRG adc clock source.
 */
typedef enum {
    CRG_ADC_SRC_ADC_PLL_FUNC_CLK = 0U, /**< ADC Pll Function Clock. */
    CRG_ADC_SRC_FROSC_CLK        = 1U, /**< Fast RCOSC Clock. */
} crg_adcclk_sel_t;

/**
 * @brief CRG supertmr clock source.
 */
typedef enum {
    CRG_TCLK0_CLK         = 0U, /**< TCK0 Clock. */
    CRG_TCLK1_CLK         = 1U, /**< TCK0 Clock. */
    CRG_TCLK2_CLK         = 2U, /**< TCK0 Clock. */
    CRG_SUPERTMR_FUNC_CLK = 3U, /**< RTC OSC Clock. */
} crg_supertmr_sel_t;

/**
 * @brief CRG work clock source.
 */
typedef enum {
    CRG_WORK_SRC_FAST_CLK  = 0U, /**< Fast CLOCK. */
    CRG_WORK_SRC_SPLL_CLK  = 1U, /**< System PLL. */
    CRG_WORK_SRC_SOSC_CLK  = 2U, /**< System OSC. */
    CRG_WORK_SRC_SROSC_CLK = 3U, /**< SROSC CLOCK. */
} crg_workclk_sel_t;

/**
 * @brief div clock divider value select
 */
typedef enum {
    DIVIDE_BY_1   = 0x00U, /**< Divide by 1 (pass-through, no clock divide) */
    DIVIDE_BY_2   = 0x01U, /**< Divide by 2   */
    DIVIDE_BY_4   = 0x02U, /**< Divide by 4   */
    DIVIDE_BY_8   = 0x03U, /**< Divide by 8   */
    DIVIDE_BY_16  = 0x04U, /**< Divide by 16  */
    DIVIDE_BY_32  = 0x05U, /**< Divide by 32  */
    DIVIDE_BY_64  = 0x06U, /**< Divide by 64  */
    DIVIDE_BY_128 = 0x07U, /**< Divide by 128 */
} crg_clock_divider_t;

/**
 * @brief Internal RF resistor select
 */
typedef enum {
    XTALRF_2M_OHM_RESISTOR = 0x00U, /**< Select 2Mohm internal RF poly resistor */
    XTALRF_1M_OHM_RESISTOR = 0x01U, /**< Select 1Mohm internal RF poly resistor */
} crg_scosc_xtalrf_t;

/**
 * @brief Operating current of crystal OSC
 */
typedef enum {
    XTALMF_OPS_CURRENT_15uA  = 0x04U, /**< Typical operating current 15uA */
    XTALMF_OPS_CURRENT_80uA  = 0x0DU, /**< Typical operating current 80uA */
    XTALMF_OPS_CURRENT_120uA = 0x0CU, /**< Typical operating current 120uA */
    XTALMF_OPS_CURRENT_240uA = 0x0EU, /**< Typical operating current 240uA */
} crg_scosc_xtalmf_t;

/**
 * @brief SOOSC AGC gain
 */
typedef enum {
    XTAL_AGC_GAIN_0 = 0x00U, /**< SOSC AGC gain 0 */
    XTAL_AGC_GAIN_1 = 0x01U, /**< SOSC AGC gain 1 */
    XTAL_AGC_GAIN_2 = 0x02U, /**< SOSC AGC gain 2 */
    XTAL_AGC_GAIN_3 = 0x03U, /**< SOSC AGC gain 3 */
} crg_scosc_xtal_agc_t;

/**
 * @brief SOOSC AGC gain
 */
typedef enum {
    CRG_SOSC_REF_EXT = 0x0, /**< External reference clock requested.  */
    CRG_SOSC_REF_OSC = 0x1, /**< Internal oscillator of OSC requested. */
} crg_sosc_ext_ref_t;

/**
 * @brief CRG RCOSC configuration.
 */
typedef struct {
    crg_rcosc_oscclk_sel_t oscclkSel;   /**< The registers used to secondly select output frequency range. */
    crg_rcosc_fmargin_sel_t fmarginSel; /**< The registers used to secondly select output frequency range. */
    uint32_t freqCtune;                 /**< Coarse tuning controlling registers of oscillation frequency. */
    uint32_t freqFtune;                 /**< Fine tuning controlling registers of oscillation frequency.   */
    bool autoTrimming;                  /**< Frequency auto trimming enable signal.                        */
    uint32_t stbCnt;                    /**< Clock stable control signal.                                  */
    uint32_t refCnt;                    /**< Target counting values of target frequency.                   */
    bool enable;                        /**< Enable signal of the OSC IP.                                  */
} crg_rcosc_config_t;

/**
 * @brief CRG system PLL configuration.
 */
typedef struct {
    uint32_t freq;                /**< System OSC frequency. \D{CRG_SOSC_REF_OSC}                   */
    crg_sosc_ext_ref_t extRef;    /**< SOSC External Reference Select \D{CRG_SOSC_REF_OSC}          */
    crg_scosc_xtalmf_t current;   /**< Registers to control operating current of crystal OSC \D{XTALMF_OPS_CURRENT_120uA} */
    crg_scosc_xtalrf_t resistor;  /**< Internal RF resistor select \D{XTALRF_1M_OHM_RESISTOR}       */
    crg_scosc_xtal_agc_t xtalAGC; /**< Registers to control AGC gain \D{XTAL_AGC_GAIN_0}            */
    bool xtalAGCBypass;           /**< AGC function bypass \D{true}                                 */
    bool enable;                  /**< Enable signal of the SOSC \D{true}                           */
} crg_sosc_config_t;

/**
 * @brief CRG system SOSC configuration.
 */
typedef struct {
    uint8_t refDiv;            /**< PLL reference clock divider.                    */
    uint8_t posDiv0;           /**< System PLL control post divider 0.              */
    uint16_t divSetInteger;    /**< System PLL integer divider.                     */
    uint16_t divSetFractional; /**< System PLL fractional divider.                  */
} crg_spll_config_t;

/**
 * @brief SCG RTC configuration.
 */
typedef struct {
    uint32_t rtcClkInFreq;      /**< RTC_CLKIN frequency.                            */
    crg_rtcclk_sel_t rtcClkSel; /**< RTC_CLKIN frequency.                            */
} crg_rtc_config_t;

/**
 * @brief SCG TCLK configuration.
 */
typedef struct {
    uint32_t rtcoscFreq;
    uint32_t tclkFreq[NUMBER_OF_TCLK_INPUTS]; /**< TCLKs frequency.                 */
} crg_extclk_config_t;

/**
 * @brief multiplexer clock configuration.
 */
typedef struct {
    clock_names_t clkName; /**< Multiplexer clock Name             */
    uint8_t clkSel;        /**< Multiplexer clock source           */
} multiplexer_clock_config_t;

/**
 * @brief mux configuration
 */
typedef struct {
    uint32_t count;                        /**< Number of Multiplexer clock to be configured.          */
    multiplexer_clock_config_t *muxClocks; /**< Pointer to the multiplexer clock configurations array. */
} crg_mux_config_t;

/**
 * @brief Divider clock configuration.
 */
typedef struct {
    clock_names_t clkName; /**< Divider clock Name           */
    uint8_t clkDiv;        /**< Divider clock division value */
} divider_clock_config_t;

/**
 * @brief div configuration
 */
typedef struct {
    uint32_t count;                    /**< Number of Divider clock to be configured.          */
    divider_clock_config_t *divClocks; /**< Pointer to the divider clock configurations array. */
} crg_div_config_t;

/**
 * @brief Gate clock configuration.
 */
typedef struct {
    clock_names_t clkName; /**< Gate clock Name           */
    bool enable;           /**< Gate clock enable state   */
} gate_clock_config_t;

/**
 * @brief gate configuration
 */
typedef struct {
    uint32_t count;                  /**< Number of Gate clock to be configured.          */
    gate_clock_config_t *gateClocks; /**< Pointer to the gate clock configurations array. */
} crg_gate_config_t;

/**
 * @brief CRG configure structure.
 */
typedef struct {
    crg_sosc_config_t soscConfig;     /**< System Crystal Oscillator configuration.   */
    crg_spll_config_t spllConfig;     /**< System Phase locked loop configuration.    */
    crg_extclk_config_t extClkConfig; /**< Ext Clock option Clock configuration.      */
    crg_mux_config_t muxConfig;       /**< Mux Clock configuration.                   */
    crg_div_config_t divConfig;       /**< Div Clock configuration.                   */
} clock_manager_user_config_t;

typedef clock_manager_user_config_t clock_user_config_t;

/**
 * @brief CRG core clock structure.
 */
typedef struct {
    clock_flag_t flags;   /**< CRG clock category */
    clock_names_t name;   /**< CRG clock name */
    clock_names_t parent; /**< CRG clock parent name */
} clock_core_t;

/**
 * @brief CRG fixed factor clock structure.
 */
typedef struct {
    clock_core_t core; /**< CRG core clock class */
} clock_spll_t;

/**
 * @brief CRG tclk clock structure.
 */
typedef struct {
    clock_core_t core;  /**< CRG core clock class */
    uint32_t clockRate; /**< CRG clock rate */
} clock_extclk_t;

/**
 * @brief CRG fixed rate clock structure.
 */
typedef struct {
    clock_core_t core;  /**< CRG core clock class */
    uint32_t clockRate; /**< CRG clock rate */
} clock_fixed_rate_t;

/**
 * @brief CRG fixed factor clock structure.
 */
typedef struct {
    clock_core_t core; /**< CRG core clock class */
    uint32_t fixedFiv;
    uint32_t fixedMult;
} clock_fixed_factor_t;

/**
 * @brief CRG mux clock structure.
 */
typedef struct {
    clock_core_t core;            /**< CRG core clock class */
    void __IO *reg;               /**< CRG clock register address */
    uint32_t shift;               /**< CRG clock register shift */
    uint32_t width;               /**< CRG clock register width */
    const clock_names_t *parents; /**< CRG clock's parents clock list */
    uint32_t parents_count;       /**< CRG clock's parents clock count */
} clock_mux_t;

/**
 * @brief CRG clock div table structure.
 */
typedef struct {
    uint32_t val;
    uint32_t div;
} clk_div_table_t;

/**
 * @brief CRG div clock structure.
 */
typedef struct {
    clock_core_t core; /**< CRG core clock class */
    void __IO *reg;
    uint8_t shift;
    uint8_t width;
    const clk_div_table_t *table;
} clock_divider_t;

/**
 * @brief CRG gate clock structure.
 */
typedef struct {
    clock_core_t core; /**< CRG core clock class */
    void __IO *reg;
    uint8_t shift;
    uint8_t width;
} clock_gate_t;

#if 0
typedef struct 
{
    clock_core_t core;              /**< CRG core clock class */
    const crg_rcosc_config_t *rcosc;
} clock_rcosc_t;
#endif

#define CRG_SPLL_CLK(_name, _parent)                                              \
    {                                                                             \
        .core.flags = CRG_SPLL_CLOCK, .core.name = _name, .core.parent = _parent, \
    }

#define CRG_EXTCLK_CLK(_name, _rate)                                                               \
    {                                                                                              \
        .core.flags = CRG_EXT_CLOCK, .core.name = _name, .core.parent = _name, .clockRate = _rate, \
    }

#define CRG_RCOSC_CLK(_name)                                                     \
    {                                                                            \
        .core.flags = CRG_RCOSC_CLOCK, .core.name = _name, .core.parent = _name, \
    }
#define CRG_FIXED_RATE_CLK(_name, _rate)                                                                  \
    {                                                                                                     \
        .core.flags = CRG_FIXED_RATE_CLOCK, .core.name = _name, .core.parent = _name, .clockRate = _rate, \
    }

#define CRG_FIXED_FACTOR_CLK(_name, _parent, _div, _mult)                                                   \
    {                                                                                                       \
        .core.flags = CRG_FIXED_FACTOR_CLOCK, .core.name = _name, .core.parent = _parent, .fixedFiv = _div, \
        .fixedMult = _mult,                                                                                 \
    }

#define CRG_MUX_CLK(_name, _reg, _shift, _width, _parents)                                                   \
    {                                                                                                        \
        .core.flags = CRG_MUX_CLOCK, .core.name = _name, .core.parent = _name, .reg = _reg, .shift = _shift, \
        .width = _width, .parents = _parents, .parents_count = ARRAY_SIZE(_parents),                         \
    }

#define CRG_DIV_CLK(_name, _parent, _reg, _shift, _width, _table)                                              \
    {                                                                                                          \
        .core.flags = CRG_DIV_CLOCK, .core.name = _name, .core.parent = _parent, .reg = _reg, .shift = _shift, \
        .width = _width, .table = _table,                                                                      \
    }

#define CRG_GATE_CLK(_name, _parent, _reg, _shift, _width)                                                      \
    {                                                                                                           \
        .core.flags = CRG_GATE_CLOCK, .core.name = _name, .core.parent = _parent, .reg = _reg, .shift = _shift, \
        .width = _width,                                                                                        \
    }

/**
 * @brief Wrapper over CLOCK_DRV_GetFreq function.
 *
 * @param clockName Clock names defined in clock_names_t
 * @param frequency Returned clock frequency value in Hertz
 * @return status_t   Error code defined in status_t
 */
status_t CLOCK_SYS_GetFreq(clock_names_t clockName, uint32_t *frequency);

/**
 * @brief Wrapper over CLOCK_SYS_ClockEnable function.
 *
 * @param clockName Clock names defined in clock_names_t
 * @param enable Enable/Disable clock state
 * @return status_t   Error code defined in status_t
 */
status_t CLOCK_SYS_ClockEnable(clock_names_t clockName, bool enable);

/**
 * @brief Wrapper over CLOCK_SYS_SetParent function.
 *
 * @param clkName Clock names defined in clock_names_t
 * @param parentClkName Clock type name
 * @return status_t   Error code defined in status_t
 */
status_t CLOCK_SYS_SetParent(clock_names_t clkName, clock_names_t parentClkName);

/**
 * @brief Wrapper over CLOCK_SYS_GetParent function.
 *
 * @param clkName Clock names defined in clock_names_t
 * @param parentClkName Clock type name
 * @return status_t   Error code defined in status_t
 */
status_t CLOCK_SYS_GetParent(clock_names_t clkName, clock_names_t *parentClkName);

#if defined(__cplusplus)
}
#endif

#endif
