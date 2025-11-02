/*
 * Copyright (c) 2020 ESWIN Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __CORE_FEATURE_PMU__
#define __CORE_FEATURE_PMU__

/**
 * @file    core_feature_pmu.h
 * @brief   PMU feature API for ESWIN Core
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_RV_PMU_PRESENT

/**
 * \defgroup EMSIS_Core_PMU_Registers    Register Define and Type Definitions of PMU
 * \ingroup  EMSIS_Core_Registers
 * \brief d  Type definition and defines for pmu registers.
 *
 * @{
 */

/**
 * \brief   Union type to access wakeup INT enable register
 */
typedef union {
    struct {
        uint32_t nmi : 1;   /** bit: 0  NMI INT wakeup core enable/disable */
        uint32_t ext : 1;   /** bit: 1  external INT wakeup core enable/disable */
        uint32_t int0 : 1;  /** bit: 2  external fast INT0 wakeup core enable/disable */
        uint32_t int1 : 1;  /** bit: 3  external fast INT1 wakeup core enable/disable */
        uint32_t int2 : 1;  /** bit: 4  external fast INT2 wakeup core enable/disable */
        uint32_t int3 : 1;  /** bit: 5  external fast INT3 wakeup core enable/disable */
        uint32_t int4 : 1;  /** bit: 6  external fast INT4 wakeup core enable/disable */
        uint32_t int5 : 1;  /** bit: 7  external fast INT5 wakeup core enable/disable */
        uint32_t int6 : 1;  /** bit: 8  external fast INT6 wakeup core enable/disable */
        uint32_t int7 : 1;  /** bit: 9  external fast INT7 wakeup core enable/disable */
        uint32_t int8 : 1;  /** bit: 10 external fast INT8 wakeup core enable/disable */
        uint32_t int9 : 1;  /** bit: 11 external fast INT9 wakeup core enable/disable */
        uint32_t int10 : 1; /** bit: 12 external fast INT10 wakeup core enable/disable */
        uint32_t int11 : 1; /** bit: 13 external fast INT11 wakeup core enable/disable */
        uint32_t int12 : 1; /** bit: 14 external fast INT12 wakeup core enable/disable */
        uint32_t int13 : 1; /** bit: 15 external fast INT13 wakeup core enable/disable */
        uint32_t int14 : 1; /** bit: 16 external fast INT14 wakeup core enable/disable */
        uint32_t int15 : 1; /** bit: 17 external fast INT15 wakeup core enable/disable */
        uint32_t int16 : 1; /** bit: 18 external fast INT15 wakeup core enable/disable */
        uint32_t int17 : 1; /** bit: 19 external fast INT15 wakeup core enable/disable */
        uint32_t int18 : 1; /** bit: 20 external fast INT15 wakeup core enable/disable */
        uint32_t int19 : 1; /** bit: 21 external fast INT15 wakeup core enable/disable */
        uint32_t int20 : 1; /** bit: 22 external fast INT15 wakeup core enable/disable */
        uint32_t int21 : 1; /** bit: 23 external fast INT15 wakeup core enable/disable */
        uint32_t int22 : 1; /** bit: 24 external fast INT15 wakeup core enable/disable */
        uint32_t int23 : 1; /** bit: 25 external fast INT15 wakeup core enable/disable */
        uint32_t int24 : 1; /** bit: 26 external fast INT15 wakeup core enable/disable */
        uint32_t int25 : 1; /** bit: 27 external fast INT15 wakeup core enable/disable */
        uint32_t int26 : 1; /** bit: 28 external fast INT15 wakeup core enable/disable */
        uint32_t int27 : 1; /** bit: 29 external fast INT15 wakeup core enable/disable */
        uint32_t int28 : 1; /** bit: 30 external fast INT15 wakeup core enable/disable */
        uint32_t int29 : 1; /** bit: 31 external fast INT15 wakeup core enable/disable */
    } b;                    /** Structure used for bit access */
    uint32_t w;             /** Type      used for word access */
} PMU_WAKEUP_INT_EN;

/**
 * \brief  Union type to access Common Flag Enable register
 */
typedef union {
    struct {
        uint32_t com0 : 1;        /** bit: 0  COM Flag 0 enable bit */
        uint32_t com1 : 1;        /** bit: 1  COM Flag 1 enable bit */
        uint32_t com2 : 1;        /** bit: 2  COM Flag 2 enable bit */
        uint32_t com3 : 1;        /** bit: 3  COM Flag 3 enable bit */
        uint32_t com4 : 1;        /** bit: 4  COM Flag 4 enable bit */
        uint32_t com5 : 1;        /** bit: 5  COM Flag 5 enable bit */
        uint32_t com6 : 1;        /** bit: 6  COM Flag 6 enable bit */
        uint32_t com7 : 1;        /** bit: 7  COM Flag 7 enable bit */
        uint32_t _reserved0 : 24; /** bit: 8..31 Reserved */
    } b;                          /** Structure  used for bit access */
    uint32_t w;                   /** Type       used for word access */
} PMU_COM_FLAG_EN;

/**
 * \brief  Union type to access Common Flag Set register
 */
typedef union {
    struct {
        uint32_t com0 : 1;        /** bit: 0  COM Flag 0 set bit */
        uint32_t com1 : 1;        /** bit: 1  COM Flag 1 set bit */
        uint32_t com2 : 1;        /** bit: 2  COM Flag 2 set bit */
        uint32_t com3 : 1;        /** bit: 3  COM Flag 3 set bit */
        uint32_t com4 : 1;        /** bit: 4  COM Flag 4 set bit */
        uint32_t com5 : 1;        /** bit: 5  COM Flag 5 set bit */
        uint32_t com6 : 1;        /** bit: 6  COM Flag 6 set bit */
        uint32_t com7 : 1;        /** bit: 7  COM Flag 7 set bit */
        uint32_t _reserved0 : 24; /** bit: 8..31 Reserved */
    } b;                          /** Structure  used for bit access */
    uint32_t w;                   /** Type       used for word access */
} PMU_COM_FLAG_SET;

/**
 * \brief  Union type to access Common Flag Clear register
 */
typedef union {
    struct {
        uint32_t com0 : 1;       /** bit: 0  COM Flag 0 clear bit */
        uint32_t com1 : 1;       /** bit: 1  COM Flag 1 clear bit */
        uint32_t com2 : 1;       /** bit: 2  COM Flag 2 clear bit */
        uint32_t com3 : 1;       /** bit: 3  COM Flag 3 clear bit */
        uint32_t com4 : 1;       /** bit: 4  COM Flag 4 clear bit */
        uint32_t com5 : 1;       /** bit: 5  COM Flag 5 clear bit */
        uint32_t com6 : 1;       /** bit: 6  COM Flag 6 clear bit */
        uint32_t com7 : 1;       /** bit: 7  COM Flag 7 clear bit */
        uint32_t _reserved : 24; /** bit: 8..31 Reserved */
    } b;                         /** Structure  used for bit access */
    uint32_t w;                  /** Type       used for word access */
} PMU_COM_FLAG_CLR;

/**
 * \brief  Union type to access Common Flag Status register
 */
typedef union {
    struct {
        uint32_t com0 : 1;       /** bit: 0  COM Flag 0 status bit */
        uint32_t com1 : 1;       /** bit: 1  COM Flag 1 status bit */
        uint32_t com2 : 1;       /** bit: 2  COM Flag 2 status bit */
        uint32_t com3 : 1;       /** bit: 3  COM Flag 3 status bit */
        uint32_t com4 : 1;       /** bit: 4  COM Flag 4 status bit */
        uint32_t com5 : 1;       /** bit: 5  COM Flag 5 status bit */
        uint32_t com6 : 1;       /** bit: 6  COM Flag 6 status bit */
        uint32_t com7 : 1;       /** bit: 7  COM Flag 7 status bit */
        uint32_t _reserved : 24; /** bit: 8..31 Reserved */
    } b;                         /** Structure  used for bit access */
    uint32_t w;                  /** Type       used for word access */
} PMU_COM_FLAG_STS;

/**
 * \brief  Union type to access clock gate enable register
 */
typedef union {
    struct {
        uint32_t enable : 1;      /** bit: 0  clock gate enable bit */
        uint32_t _reserved0 : 30; /** bit: 1..31 Reserved */
    } b;                          /** Structure  used for bit access */
    uint32_t w;                   /** Type       used for word access */
} PMU_CLK_GATE_EN;

/**
 * \brief  Union type to access Clock Enable Status register
 */
typedef union {
    struct {
        uint32_t c0_clk : 1;      /** bit: 0  C0 clock enable status */
        uint32_t c1_clk : 1;      /** bit: 1  C1 clock enable status */
        uint32_t uc_clk : 1;      /** bit: 2  Uncore clock enable status */
        uint32_t c0_sleep : 1;    /** bit: 3  C0 sleep status */
        uint32_t c1_sleep : 1;    /** bit: 4  C1 sleep status */
        uint32_t _reserved0 : 27; /** bit: 5..31 Reserved */
    } b;                          /** Structure  used for bit access */
    uint32_t w;                   /** Type       used for word access */
} PMU_CLK_STS;

#define PMU_WAKEUP_INT_EN_NMI_Pos    0U                                   /** PMU WAKEUP INT ENABLE: NMI Position */
#define PMU_WAKEUP_INT_EN_NMI_Msk    (0x1UL << PMU_WAKEUP_INT_EN_NMI_Pos) /** PMU WAKEUP INT ENABLE: NMI Mask */
#define PMU_WAKEUP_INT_EN_EXT_Pos    1U /** PMU WAKEUP INT ENABLE: External Position */
#define PMU_WAKEUP_INT_EN_EXT_Msk    (0x1UL << PMU_WAKEUP_INT_EN_EXT_Pos) /** PMU WAKEUP INT ENABLE: External Mask */
#define PMU_WAKEUP_INT_EN_INT_Pos(x) (x + 2U) /** PMU WAKEUP INT ENABLE: Fast Interrupt x Position */
#define PMU_WAKEUP_INT_EN_INT_Msk(x)                                                                                   \
    (0x1UL << PMU_WAKEUP_INT_EN_INT_Pos(x)) /** PMU WAKEUP INT ENABLE: Fast Interrupt x Mask */

#define PMU_COM_FLAG_EN_Pos(x) (x)                               /** PMU COM FLAG ENABLE: Com Flag x Enable Position */
#define PMU_COM_FLAG_EN_Msk(x) (0x1UL << PMU_COM_FLAG_EN_Pos(x)) /** PMU COM FLAG ENABLE: Com Flag x Enable Mask */

#define PMU_COM_FLAG_SET_Pos(x) (x)                                /** PMU COM FLAG SET: Com Flag x Set Position */
#define PMU_COM_FLAG_SET_Msk(x) (0x1UL << PMU_COM_FLAG_SET_Pos(x)) /** PMU COM FLAG SET: Com Flag x Set Mask */

#define PMU_COM_FLAG_CLR_Pos(x) (x)                                /** PMU COM FLAG CLEAR: Com Flag x Clear Position */
#define PMU_COM_FLAG_CLR_Msk(x) (0x1UL << PMU_COM_FLAG_CLR_Pos(x)) /** PMU COM FLAG CLEAR: Com Flag x Clear Mask */

#define PMU_COM_FLAG_STS_Pos(x) (x) /** PMU COM FLAG STATUS: Com Flag x Status Position */
#define PMU_COM_FLAG_STS_Msk(x) (0x1UL << PMU_COM_FLAG_STS_Pos(x)) /** PMU COM FLAG STATUS: Com Flag x Status Mask */

#define PMU_CLOCK_GATE_EN_Pos 0U                               /** PMU CLOCK GATE ENABLE: Enable Position */
#define PMU_CLOCK_GATE_EN_Msk (0x1UL << PMU_CLOCK_GATE_EN_Pos) /** PMU CLOCK GATE ENABLE: Enable Mask */

/**
 * \brief  Access to the structure of PMU control for specific core
 */
typedef struct {
    __IOM uint32_t wakeup_enable;   /** Offset: 0x00 - 0x03 (R/W) Core 0 wakeup interrupt enable */
    __IOM uint32_t com_flag_enable; /** Offset: 0x04 - 0x07 (R/W) Core 0 com flag enable */
    __IOM uint32_t com_flag_set;    /** Offset: 0x08 - 0x0B (W)   Core 0 com flag set */
    __IOM uint32_t com_flag_clear;  /** Offset: 0x0C - 0x0F (W)   Core 0 com flag clear */
    __IOM uint32_t com_flag_status; /** Offset: 0x10 - 0x13 (R)   Core 0 com flag status */
    __IOM uint32_t clk_gate_enable; /** Offset: 0x14 - 0x17 (R/W) Core 0 clock gate enable */
    uint8_t _reserved0[8];          /** Offset: 0x18 - 0x1F Reserved */
} CORE_PMU_Type;

/**
 * \brief  Access to the structure of PMU Controller
 */
typedef struct {
    CORE_PMU_Type Core[2];                  /** Offset: 0x00 - 0x3F Core specific control */
    __IOM uint32_t clk_enable_status;       /** Offset: 0x40 - 0x43 (R)   Clock enable status */
    uint8_t _reserved2[12];                 /** Offset: 0x44 - 0x4F Reserved */
    __IOM uint32_t uc_clk_gate_enable;      /** Offset: 0x50 - 0x53 (R/W) Uncore clock gate enable */
    uint8_t _reserved3[12];                 /** Offset: 0x54 - 0x5F Reserved */
    __IOM uint32_t auto_clk_gating_disable; /** Offset: 0x60 - 0x63 (R/W) auto clock gating disabled */
} pmu_type_t;

#define PMU ((pmu_type_t *)CONFIG_RV_PMU_BASEADDR)

/**
 * @{
 * */ /* End of group EMSIS_Core_PMU_Registers */

/* ################################# PMU Functions ############################## */
/**
 * defgroup  EMSIS_Core_PMU       PMU implementation
 * \brief  Function Implementation that control PMU module
 *
 * @{
 */

/**
 * \brief  Enable wakeup interrupt for Core
 */
__STATIC_FORCEINLINE void __PMU_SetWakeupEnableBits(uint32_t wakeup)
{
    PMU->Core[__get_hartid()].wakeup_enable |= wakeup;
}

/**
 * \brief  disable wakeup interrupt for Core
 */
__STATIC_FORCEINLINE void __PMU_ClearWakeupEnableBits(uint32_t wakeup)
{
    PMU->Core[__get_hartid()].wakeup_enable &= ~wakeup;
}

/**
 * \brief  Get wakeup interrupt setting for Core
 */
__STATIC_FORCEINLINE uint32_t __PMU_GetWakeupEnableBits(void)
{
    return (uint32_t)PMU->Core[__get_hartid()].wakeup_enable;
}

/**
 * \brief  Enable Com Flag for Core
 */
__STATIC_FORCEINLINE void __PMU_SetComFlagEnableBits(int index)
{
    PMU->Core[__get_hartid()].com_flag_enable |= (0x1UL << index);
}

/**
 * \brief  Disable Com Flag for Core
 */
__STATIC_FORCEINLINE void __PMU_ClearComFlagEnableBits(int index)
{
    PMU->Core[__get_hartid()].com_flag_enable &= ~(0x1UL << index);
}

/**
 * \brief  Get Com Flag for Core
 */
__STATIC_FORCEINLINE uint32_t __PMU_GetComFlagEnableBits(void)
{
    return (uint32_t)PMU->Core[__get_hartid()].com_flag_enable;
}

/**
 * \brief  Set Com Flag for Core
 */
__STATIC_FORCEINLINE void __PMU_SetComFlagBits(int hartid, int index)
{
    PMU->Core[hartid].com_flag_set |= (0x1UL << index);
}

/**
 * \brief  Clear Com Flag for Core
 */
__STATIC_FORCEINLINE void __PMU_ClearComFlagBits(int index)
{
    PMU->Core[__get_hartid()].com_flag_clear &= ~(0x1UL << index);
}

/**
 * \brief  Get Com Flag Status
 */
__STATIC_FORCEINLINE uint32_t __PMU_GetComFlagStatus(void)
{
    return (uint32_t)PMU->Core[__get_hartid()].com_flag_status;
}

/**
 * \brief  Enable Clock Gate for Core
 */
__STATIC_FORCEINLINE void __PMU_EnableClockGate(void)
{
    PMU->Core[__get_hartid()].clk_gate_enable |= 1;
}

/**
 * \brief  Disable Clock Gate for Core
 */
__STATIC_FORCEINLINE void __PMU_DisableClockGate(void)
{
    PMU->Core[__get_hartid()].clk_gate_enable &= ~(0x1UL);
}

/**
 * \brief  Get Clock Enable Status
 */
__STATIC_FORCEINLINE uint32_t __PMU_GetClockEnableStatus(void)
{
    PMU->clk_enable_status;
}

/**
 * \brief  Enable Clock Gate for Uncore
 */
__STATIC_FORCEINLINE void __PMU_EnableClockGateUncore(void)
{
    PMU->uc_clk_gate_enable |= 1;
}

/**
 * \brief  Disable Clock Gate for Uncore
 */
__STATIC_FORCEINLINE void __PMU_DisableClockGateUncore(void)
{
    PMU->uc_clk_gate_enable &= ~(0x1UL);
}

/**
 * \brief  Enable Auto gating
 */
__STATIC_FORCEINLINE void __PMU_EnableAutoGating(void)
{
    PMU->auto_clk_gating_disable &= ~(0x1UL);
}

/**
 * \brief  Disable Auto gating
 */
__STATIC_FORCEINLINE void __PMU_DisableAutoGating(void)
{
    PMU->auto_clk_gating_disable |= 1;
}

/**
 * \brief  Get Auto gating setting
 */
__STATIC_FORCEINLINE uint32_t __PMU_GetAutoGating(void)
{
    return (uint32_t)PMU->auto_clk_gating_disable;
}

#endif /* CONFIG_RV_PMU_PRESENT */

#ifdef __cplusplus
}
#endif
#endif /* __CORE_FEATURE_PMU__ */
