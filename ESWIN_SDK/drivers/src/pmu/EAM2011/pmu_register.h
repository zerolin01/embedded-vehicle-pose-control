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
 * @file pmu_register.h
 * @brief PMU driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __PMU_REGISTER_H__
#define __PMU_REGISTER_H__

#include "emps_platform.h"
#include <osal.h>
#include <stdint.h>

// clang-format off
/* ----------------------------------------------------------------------------
   -- PMU Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup PMU_Peripheral_Access_Layer PMU Peripheral Access Layer
 * @{
 */

/**
 * @brief PMU Register Layout Typedef
 */
typedef struct {
    __IO uint32_t REG_IOPSEL_LV;         /**< IOPSEL, Low Voltage, offset: 0x00 */
    __IO uint32_t RESERVED_0;
    __IO uint32_t SW_LBIST_CLAMP_EN;     /**< LBIST Clamp Enable, offset: 0x08 */
    __IO uint32_t REG_BOR_EN_LV;         /**< BOR Enable, Low Voltage, offset: 0x0C */
    __IO uint32_t REG_PVD_EN_LV;         /**< PVD Enable, Low Voltage, offset: 0x10 */
    __IO uint32_t REG_BUF_EN_LV;         /**< Buffing Enable, Low Voltage, offset: 0x14 */
    __IO uint32_t REG_LDOF_EN_LV;        /**< LDOF Enable, offset: 0x18 */
    __IO uint32_t REG_LPBGP_LV;          /**< LPBGP Configuration, offset: 0x1C*/
    __IO uint32_t REG_LPLDO_LV;          /**< LPLDO Configuration, offset: 0x20 */
    __IO uint32_t REG_MBGP_LV;           /**< MBGP Enable, Low Voltage, offset: 0x24 */
    __IO uint32_t REG_LDOC_LV;           /**< LDOC Enable, offset: 0x28 */
    __IO uint32_t REG_BOR_LV;            /**< BOR Configuration, offset: 0x2C */
    __IO uint32_t REG_PVD_LV;            /**< PVD Configuration, offset: 0x30 */
    __IO uint32_t REG_VREF10_LV;         /**< VREF10 Configuration, offset: 0x34 */
    __IO uint32_t REG_LDOF_LV;           /**< LDOF Configuration, offset: 0x38 */
    __IO uint32_t REG_TEST_LV;           /**< TEST Configuration, offset: 0x3C */
    __O uint32_t REG_WR_UPD;             /**< UPD Trigger, offset: 0x40 */
    __IO uint32_t FSM_CNT_CTRL;          /**< FSM Control, offset: 0x44 */
    __IO uint32_t FSM_CNT_VAL;           /**< FSM Counter Value, offset: 0x48 */
    __IO uint32_t RTC_CK_GF_SEL;         /**< RTC Clock Selection, offset: 0x4C */
    __IO uint32_t STANDBY_CTRL;          /**< Standby Control, offset: 0x50 */
    __IO uint32_t AON_PAD_CTRL0;         /**< Always On Pad Control0, offset: 0x54 */
    __IO uint32_t AON_PAD_CTRL1;         /**< Always On Pad Control1, offset: 0x54 */
    __IO uint32_t RESERVED_1[5];
    __IO uint32_t REG_INTR_MASK;         /**< PMU Interrupt Mask, offset: 0x70 */
    __IO uint32_t RESERVED_2[2];
    __I uint32_t INTR_STATUS;            /**< Interrupt Status, offset: 0x7C */
    __IO uint32_t RESERVED_3[4];
    __IO uint32_t POWERUP_INT_CLEAN;     /**< Powerup Interrupt Clean, offset: 0x90 */
    __IO uint32_t BOR_INT_CLEAN;         /**< BOR Interrupt Clean, offset: 0x94 */
    __IO uint32_t POR_INT_CLEAN;         /**< POR Interrupt Clean, offset: 0x98 */
    __IO uint32_t SLEEP_MODE;            /**< Sleep Mode Control, offset: 0x9C */
    __IO uint32_t REBOOT_CNT_CTRL;       /**< Reboot Counter Control, offset: 0xA0 */
    __IO uint32_t REBOOT_FAIL_IRQ_MASK;  /**< Reboot Fail Irq Mask, offset: 0xA4 */
    __I uint32_t REBOOT_FAIL_IRQ_STATUS; /**< Reboot Fail Irq Status, offset: 0xA8 */
    __IO uint32_t REBOOT_FAIL_IRQ_CLEAR; /**< Reboot Fail Irq Clear, offset: 0xAC */
    __IO uint32_t WAKEUP_CTRL;           /**< Wakeup Control, offset: 0xB0 */
    __I uint32_t RSTN_PVD_LV;            /**< RSTN_PVD_LV Status, offset: 0xB4 */
    __IO uint32_t REG_EN_OSC_LV_ASYNC;   /**< Oscillator Enable, offset: 0xB8 */
    __I uint32_t STANDBY_MODE_CNT;       /**< Standby Mode Counter, offset: 0xBC */
    __IO uint32_t WAKEUP_DISABLE;        /**< Wakeup Disable, offset: 0xC0 */
    __IO uint32_t RESERVED_4;
    __IO uint32_t BOR_RSTN_MASK;         /**< BOR RSTN Mask, offset: 0xC8 */
    __IO uint32_t SW_SYS_RSTN;           /**< Software SYS_RSTN, offset: 0xCC */
} pmu_type_t, *pmu_mem_map_ptr_t;

/* ----------------------------------------------------------------------------
   -- PMU Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup PMU_Register_Masks PMU Register Masks
 * @{
 */
/** REG_IOPSEL_LV Bit Fields */
#define PMU_IOPSEL_LV_MASK                        0x3FU
#define PMU_IOPSEL_LV_SHIFT                       0U
#define PMU_IOPSEL_LV_WIDTH                       6U
#define PMU_IOPSEL_LV(x)                          (((uint32_t)(((uint32_t)(x))<<PMU_IOPSEL_LV_SHIFT))&PMU_IOPSEL_LV_MASK)
/** SW_LBIST_CLAMP_EN Bit Fields */
#define PMU_SW_LBIST_CLAMP_EN_MASK                0x1U
#define PMU_SW_LBIST_CLAMP_EN_SHIFT               0U
#define PMU_SW_LBIST_CLAMP_EN_WIDTH               1U
#define PMU_SW_LBIST_CLAMP_EN(x)                  (((uint32_t)(((uint32_t)(x))<<PMU_SW_LBIST_CLAMP_EN_SHIFT))&PMU_SW_LBIST_CLAMP_EN_MASK)
/** REG_BOR_EN_LV Bit Fields */
#define PMU_BOR_EN_LV_MASK                        0x1U
#define PMU_BOR_EN_LV_SHIFT                       0U
#define PMU_BOR_EN_LV_WIDTH                       1U
#define PMU_BOR_EN_LV(x)                          (((uint32_t)(((uint32_t)(x))<<PMU_BOR_EN_LV_SHIFT))&PMU_BOR_EN_LV_MASK)
/** REG_PVD_EN_LV Bit Fields */
#define PMU_PVD_EN_LV_MASK                        0x1U
#define PMU_PVD_EN_LV_SHIFT                       0U
#define PMU_PVD_EN_LV_WIDTH                       1U
#define PMU_PVD_EN_LV(x)                          (((uint32_t)(((uint32_t)(x))<<PMU_PVD_EN_LV_SHIFT))&PMU_PVD_EN_LV_MASK)
/** REG_BUF_EN_LV Bit Fields */
#define PMU_BUF_EN_LV_VREF10A_MASK                0x1U
#define PMU_BUF_EN_LV_VREF10A_SHIFT               0U
#define PMU_BUF_EN_LV_VREF10A_WIDTH               1U
#define PMU_BUF_EN_LV_VREF10A(x)                  (((uint32_t)(((uint32_t)(x))<<PMU_BUF_EN_LV_VREF10A_SHIFT))&PMU_BUF_EN_LV_VREF10A_MASK)
#define PMU_BUF_EN_LV_VREF12A_MASK                0x10000U
#define PMU_BUF_EN_LV_VREF12A_SHIFT               16U
#define PMU_BUF_EN_LV_VREF12A_WIDTH               1U
#define PMU_BUF_EN_LV_VREF12A(x)                  (((uint32_t)(((uint32_t)(x))<<PMU_BUF_EN_LV_VREF12A_SHIFT))&PMU_BUF_EN_LV_VREF12A_MASK)
/** REG_LDOF_EN_LV Bit Fields */
#define PMU_LDOF_EN_LV_MASK                       0x1U
#define PMU_LDOF_EN_LV_SHIFT                      0U
#define PMU_LDOF_EN_LV_WIDTH                      1U
#define PMU_LDOF_EN_LV(x)                         (((uint32_t)(((uint32_t)(x))<<PMU_LDOF_EN_LV_SHIFT))&PMU_LDOF_EN_LV_MASK)
/** REG_LPBGP_LV Bit Fields */
#define PMU_LPBGP_LV_MASK                         0x1FU
#define PMU_LPBGP_LV_SHIFT                        0U
#define PMU_LPBGP_LV_WIDTH                        5U
#define PMU_LPBGP_LV(x)                           (((uint32_t)(((uint32_t)(x))<<PMU_LPBGP_LV_SHIFT))&PMU_LPBGP_LV_MASK)
/** REG_LPLDO_LV Bit Fields */
#define PMU_LPLDO_LV_MASK                         0xFU
#define PMU_LPLDO_LV_SHIFT                        0U
#define PMU_LPLDO_LV_WIDTH                        4U
#define PMU_LPLDO_LV(x)                           (((uint32_t)(((uint32_t)(x))<<PMU_LPLDO_LV_SHIFT))&PMU_LPLDO_LV_MASK)
/** REG_MBGP_LV Bit Fields */
#define PMU_MBGP_LV_MASK                          0x1FU
#define PMU_MBGP_LV_SHIFT                         0U
#define PMU_MBGP_LV_WIDTH                         5U
#define PMU_MBGP_LV(x)                            (((uint32_t)(((uint32_t)(x))<<PMU_MBGP_LV_SHIFT))&PMU_MBGP_LV_MASK)
/** REG_LDOC_LV Bit Fields */
#define PMU_LDOC_LV_V11_CORE_MASK                 0xFU
#define PMU_LDOC_LV_V11_CORE_SHIFT                0U
#define PMU_LDOC_LV_V11_CORE_WIDTH                4U
#define PMU_LDOC_LV_V11_CORE(x)                   (((uint32_t)(((uint32_t)(x))<<PMU_LDOC_LV_V11_CORE_SHIFT))&PMU_LDOC_LV_V11_CORE_MASK)
#define PMU_LDOC_LV_CURRENT_LIMIT_MASK            0x10U
#define PMU_LDOC_LV_CURRENT_LIMIT_SHIFT           4U
#define PMU_LDOC_LV_CURRENT_LIMIT_WIDTH           1U
#define PMU_LDOC_LV_CURRENT_LIMIT(x)              (((uint32_t)(((uint32_t)(x))<<PMU_LDOC_LV_CURRENT_LIMIT_SHIFT))&PMU_LDOC_LV_CURRENT_LIMIT_MASK)
/** REG_BOR_LV Bit Fields */
#define PMU_BOR_LV_MASK                           0xFU
#define PMU_BOR_LV_SHIFT                          0U
#define PMU_BOR_LV_WIDTH                          4U
#define PMU_BOR_LV(x)                             (((uint32_t)(((uint32_t)(x))<<PMU_BOR_LV_SHIFT))&PMU_BOR_LV_MASK)
/** REG_PVD_LV Bit Fields */
#define PMU_PVD_LV_MASK                           0xFU
#define PMU_PVD_LV_SHIFT                          0U
#define PMU_PVD_LV_WIDTH                          4U
#define PMU_PVD_LV(x)                             (((uint32_t)(((uint32_t)(x))<<PMU_PVD_LV_SHIFT))&PMU_PVD_LV_MASK)
/** REG_VREF10_LV Bit Fields */
#define PMU_VREF10_LV_MASK                        0xFU
#define PMU_VREF10_LV_SHIFT                       0U
#define PMU_VREF10_LV_WIDTH                       4U
#define PMU_VREF10_LV(x)                          (((uint32_t)(((uint32_t)(x))<<PMU_VREF10_LV_SHIFT))&PMU_VREF10_LV_MASK)
/** REG_LDOF_LV Bit Fields */
#define PMU_LDOF_LV_LDOF_CORE_MASK                0xFU
#define PMU_LDOF_LV_LDOF_CORE_SHIFT               0U
#define PMU_LDOF_LV_LDOF_CORE_WIDTH               4U
#define PMU_LDOF_LV_LDOF_CORE(x)                  (((uint32_t)(((uint32_t)(x))<<PMU_LDOF_LV_LDOF_CORE_SHIFT))&PMU_LDOF_LV_LDOF_CORE_MASK)
#define PMU_LDOF_LV_CURRENT_LIMIT_MASK            0x10U
#define PMU_LDOF_LV_CURRENT_LIMIT_SHIFT           4U
#define PMU_LDOF_LV_CURRENT_LIMIT_WIDTH           1U
#define PMU_LDOF_LV_CURRENT_LIMIT(x)              (((uint32_t)(((uint32_t)(x))<<PMU_LDOF_LV_CURRENT_LIMIT_SHIFT))&PMU_LDOF_LV_CURRENT_LIMIT_MASK)
/** REG_TEST_LV Bit Fields */
#define PMU_REG_TEST_LV_VREF_MASK                 0x7U
#define PMU_REG_TEST_LV_VREF_SHIFT                0U
#define PMU_REG_TEST_LV_VREF_WIDTH                3U
#define PMU_REG_TEST_LV_VREF(x)                   (((uint32_t)(((uint32_t)(x))<<PMU_REG_TEST_LV_VREF_SHIFT))&PMU_REG_TEST_LV_VREF_MASK)
#define PMU_REG_TEST_LV_PVD_MASK                  0x8U
#define PMU_REG_TEST_LV_PVD_SHIFT                 3U
#define PMU_REG_TEST_LV_PVD_WIDTH                 1U
#define PMU_REG_TEST_LV_PVD(x)                    (((uint32_t)(((uint32_t)(x))<<PMU_REG_TEST_LV_PVD_SHIFT))&PMU_REG_TEST_LV_PVD_MASK)
#define PMU_REG_TEST_LV_BOR_MASK                  0x10U
#define PMU_REG_TEST_LV_BOR_SHIFT                 4U
#define PMU_REG_TEST_LV_BOR_WIDTH                 1U
#define PMU_REG_TEST_LV_BOR(x)                    (((uint32_t)(((uint32_t)(x))<<PMU_REG_TEST_LV_BOR_SHIFT))&PMU_REG_TEST_LV_BOR_MASK)
#define PMU_REG_TEST_LV_MODE_MASK                 0x10000U
#define PMU_REG_TEST_LV_MODE_SHIFT                16U
#define PMU_REG_TEST_LV_MODE_WIDTH                1U
#define PMU_REG_TEST_LV_MODE(x)                   (((uint32_t)(((uint32_t)(x))<<PMU_REG_TEST_LV_MODE_SHIFT))&PMU_REG_TEST_LV_MODE_MASK)
/** REG_WR_UPD Bit Fields */
#define PMU_WR_UPD_MASK                           0x1U
#define PMU_WR_UPD_SHIFT                          0U
#define PMU_WR_UPD_WIDTH                          1U
#define PMU_WR_UPD(x)                             (((uint32_t)(((uint32_t)(x))<<PMU_WR_UPD_SHIFT))&PMU_WR_UPD_MASK)
/** FSM_CNT_CTRL Bit Fields */
#define PMU_FSM_CNT_CTRL_TIMEOUT_EN_MASK          0x1U
#define PMU_FSM_CNT_CTRL_TIMEOUT_EN_SHIFT         0U
#define PMU_FSM_CNT_CTRL_TIMEOUT_EN_WIDTH         1U
#define PMU_FSM_CNT_CTRL_TIMEOUT_EN(x)            (((uint32_t)(((uint32_t)(x))<<PMU_FSM_CNT_CTRL_TIMEOUT_EN_SHIFT))&PMU_FSM_CNT_CTRL_TIMEOUT_EN_MASK)
#define PMU_FSM_CNT_CTRL_TIMEOUT_OV_MASK          0x100U
#define PMU_FSM_CNT_CTRL_TIMEOUT_OV_SHIFT         8U
#define PMU_FSM_CNT_CTRL_TIMEOUT_OV_WIDTH         1U
#define PMU_FSM_CNT_CTRL_TIMEOUT_OV(x)            (((uint32_t)(((uint32_t)(x))<<PMU_FSM_CNT_CTRL_TIMEOUT_OV_SHIFT))&PMU_FSM_CNT_CTRL_TIMEOUT_OV_MASK)
/** FSM_CNT_VAL Bit Fields */
#define PMU_FSM_CNT_VAL_MASK                      0xFFFFFFFFU
#define PMU_FSM_CNT_VAL_SHIFT                     0U
#define PMU_FSM_CNT_VAL_WIDTH                     32U
#define PMU_FSM_CNT_VAL(x)                        (((uint32_t)(((uint32_t)(x))<<PMU_FSM_CNT_VAL_SHIFT))&PMU_FSM_CNT_VAL_MASK)
/** RTC_CK_GF_SEL Bit Fields */ 
#define PMU_RTC_CK_GF_SEL_MASK                    0x1U
#define PMU_RTC_CK_GF_SEL_SHIFT                   0U
#define PMU_RTC_CK_GF_SEL_WIDTH                   1U
#define PMU_RTC_CK_GF_SEL(x)                      (((uint32_t)(((uint32_t)(x))<<PMU_RTC_CK_GF_SEL_SHIFT))&PMU_RTC_CK_GF_SEL_MASK)
/** STANDBY_CTRL Bit Fields */
#define PMU_STANDBY_CTRL_MASK                     0x1U
#define PMU_STANDBY_CTRL_SHIFT                    0U
#define PMU_STANDBY_CTRL_WIDTH                    1U
#define PMU_STANDBY_CTRL(x)                       (((uint32_t)(((uint32_t)(x))<<PMU_STANDBY_CTRL_SHIFT))&PMU_STANDBY_CTRL_MASK)
/** REG_INTR_MASK Bit Fields */
#define PMU_INTR_MASK_MASK                        0x7FU
#define PMU_INTR_MASK_SHIFT                       0U
#define PMU_INTR_MASK_WIDTH                       3U
#define PMU_INTR_MASK(x)                          (((uint32_t)(((uint32_t)(x))<<PMU_INTR_MASK_SHIFT))&PMU_INTR_MASK_MASK)
#define PMU_INTR_MASK_PVD_MASK                    0x1U
#define PMU_INTR_MASK_PVD_SHIFT                   0U
#define PMU_INTR_MASK_PVD_WIDTH                   1U
#define PMU_INTR_MASK_PVD(x)                      (((uint32_t)(((uint32_t)(x))<<PMU_INTR_MASK_PVD_SHIFT))&PMU_INTR_MASK_PVD_MASK)
#define PMU_INTR_MASK_BOR_MASK                    0x2U
#define PMU_INTR_MASK_BOR_SHIFT                   1U
#define PMU_INTR_MASK_BOR_WIDTH                   1U
#define PMU_INTR_MASK_BOR(x)                      (((uint32_t)(((uint32_t)(x))<<PMU_INTR_MASK_BOR_SHIFT))&PMU_INTR_MASK_BOR_MASK)
#define PMU_INTR_MASK_WAKEUP_MASK                 0x4U
#define PMU_INTR_MASK_WAKEUP_SHIFT                2U
#define PMU_INTR_MASK_WAKEUP_WIDTH                1U
#define PMU_INTR_MASK_WAKEUP(x)                   (((uint32_t)(((uint32_t)(x))<<PMU_INTR_MASK_WAKEUP_SHIFT))&PMU_INTR_MASK_WAKEUP_MASK)
/** INTR_STATUS Bit Fields */
#define PMU_INTR_STATUS_MASK                      0x7FU
#define PMU_INTR_STATUS_SHIFT                     0U
#define PMU_INTR_STATUS_WIDTH                     7U
#define PMU_INTR_STATUS(x)                        (((uint32_t)(((uint32_t)(x))<<PMU_INTR_STATUS_SHIFT))&PMU_INTR_STATUS_MASK)
#define PMU_INTR_STATUS_PVD_MASK                  0x1U
#define PMU_INTR_STATUS_PVD_SHIFT                 0U
#define PMU_INTR_STATUS_PVD_WIDTH                 1U
#define PMU_INTR_STATUS_PVD(x)                    (((uint32_t)(((uint32_t)(x))<<PMU_INTR_STATUS_PVD_SHIFT))&PMU_INTR_STATUS_PVD_MASK)
#define PMU_INTR_STATUS_BOR_MASK                  0x2U
#define PMU_INTR_STATUS_BOR_SHIFT                 1U
#define PMU_INTR_STATUS_BOR_WIDTH                 1U
#define PMU_INTR_STATUS_BOR(x)                    (((uint32_t)(((uint32_t)(x))<<PMU_INTR_STATUS_BOR_SHIFT))&PMU_INTR_STATUS_BOR_MASK)
#define PMU_INTR_STATUS_WAKEUP_MASK               0x4U
#define PMU_INTR_STATUS_WAKEUP_SHIFT              2U
#define PMU_INTR_STATUS_WAKEUP_WIDTH              1U
#define PMU_INTR_STATUS_WAKEUP(x)                 (((uint32_t)(((uint32_t)(x))<<PMU_INTR_STATUS_WAKEUP_SHIFT))&PMU_INTR_STATUS_WAKEUP_MASK)
/** POWERUP_INT_CLEAN Bit Fields */
#define PMU_POWERUP_INT_CLEAN_MASK                0x1U
#define PMU_POWERUP_INT_CLEAN_SHIFT               0U
#define PMU_POWERUP_INT_CLEAN_WIDTH               1U
#define PMU_POWERUP_INT_CLEAN(x)                  (((uint32_t)(((uint32_t)(x))<<PMU_POWERUP_INT_CLEAN_SHIFT))&PMU_POWERUP_INT_CLEAN_MASK)
/** BOR_INT_CLEAN Bit Fields */
#define PMU_BOR_INT_CLEAN_MASK                    0x1U
#define PMU_BOR_INT_CLEAN_SHIFT                   0U
#define PMU_BOR_INT_CLEAN_WIDTH                   1U
#define PMU_BOR_INT_CLEAN(x)                      (((uint32_t)(((uint32_t)(x))<<PMU_BOR_INT_CLEAN_SHIFT))&PMU_BOR_INT_CLEAN_MASK)
/** POR_INT_CLEAN Bit Fields */
#define PMU_POR_INT_CLEAN_MASK                    0x1U
#define PMU_POR_INT_CLEAN_SHIFT                   0U
#define PMU_POR_INT_CLEAN_WIDTH                   1U
#define PMU_POR_INT_CLEAN(x)                      (((uint32_t)(((uint32_t)(x))<<PMU_POR_INT_CLEAN_SHIFT))&PMU_POR_INT_CLEAN_MASK)
/** SLEEP_MODE Bit Fields */
#define PMU_SLEEP_MODE_SOURCE_MASK                0x1U
#define PMU_SLEEP_MODE_SOURCE_SHIFT               0U
#define PMU_SLEEP_MODE_SOURCE_WIDTH               1U
#define PMU_SLEEP_MODE_SOURCE(x)                  (((uint32_t)(((uint32_t)(x))<<PMU_SLEEP_MODE_SOURCE_SHIFT))&PMU_SLEEP_MODE_SOURCE_MASK)
#define PMU_SLEEP_MODE_TRG_MASK                   0x100U
#define PMU_SLEEP_MODE_TRG_SHIFT                  8U
#define PMU_SLEEP_MODE_TRG_WIDTH                  1U
#define PMU_SLEEP_MODE_TRG(x)                     (((uint32_t)(((uint32_t)(x))<<PMU_SLEEP_MODE_TRG_SHIFT))&PMU_SLEEP_MODE_TRG_MASK)
/** REBOOT_CNT_CTRL Bit Fields */
#define PMU_REBOOT_CNT_CTRL_MASK                  0x1U
#define PMU_REBOOT_CNT_CTRL_SHIFT                 0U
#define PMU_REBOOT_CNT_CTRL_WIDTH                 1U
#define PMU_REBOOT_CNT_CTRL(x)                    (((uint32_t)(((uint32_t)(x))<<PMU_REBOOT_CNT_CTRL_SHIFT))&PMU_REBOOT_CNT_CTRL_MASK)
/** REBOOT_FAIL_IRQ_MASK Bit Fields */
#define PMU_REBOOT_FAIL_IRQ_MASK_MASK             0x1U
#define PMU_REBOOT_FAIL_IRQ_MASK_SHIFT            0U
#define PMU_REBOOT_FAIL_IRQ_MASK_WIDTH            1U
#define PMU_REBOOT_FAIL_IRQ_MASK(x)               (((uint32_t)(((uint32_t)(x))<<PMU_REBOOT_FAIL_IRQ_MASK_SHIFT))&PMU_REBOOT_FAIL_IRQ_MASK_MASK)
/** REBOOT_FAIL_IRQ_STATUS Bit Fields */
#define PMU_REBOOT_FAIL_IRQ_STATUS_MASK           0x1U
#define PMU_REBOOT_FAIL_IRQ_STATUS_SHIFT          0U
#define PMU_REBOOT_FAIL_IRQ_STATUS_WIDTH          1U
#define PMU_REBOOT_FAIL_IRQ_STATUS(x)             (((uint32_t)(((uint32_t)(x))<<PMU_REBOOT_FAIL_IRQ_STATUS_SHIFT))&PMU_REBOOT_FAIL_IRQ_STATUS_MASK)
/** REBOOT_FAIL_IRQ_CLEAR Bit Fields */
#define PMU_REBOOT_FAIL_IRQ_CLEAR_MASK            0x1U
#define PMU_REBOOT_FAIL_IRQ_CLEAR_SHIFT           0U
#define PMU_REBOOT_FAIL_IRQ_CLEAR_WIDTH           1U
#define PMU_REBOOT_FAIL_IRQ_CLEAR(x)              (((uint32_t)(((uint32_t)(x))<<PMU_REBOOT_FAIL_IRQ_CLEAR_SHIFT))&PMU_REBOOT_FAIL_IRQ_CLEAR_MASK)
/** WAKEUP_CTRL Bit Fields */
#define PMU_WAKEUP_CTRL_MASK                      0x3FFU
#define PMU_WAKEUP_CTRL_SHIFT                     0U
#define PMU_WAKEUP_CTRL_WIDTH                     10U
#define PMU_WAKEUP_CTRL(x)                        (((uint32_t)(((uint32_t)(x))<<PMU_WAKEUP_CTRL_SHIFT))&PMU_WAKEUP_CTRL_MASK)
/** RSTN_PVD_LV Bit Fields */
#define PMU_RSTN_PVD_LV_MASK                      0x1U
#define PMU_RSTN_PVD_LV_SHIFT                     0U
#define PMU_RSTN_PVD_LV_WIDTH                     1U
#define PMU_RSTN_PVD_LV(x)                        (((uint32_t)(((uint32_t)(x))<<PMU_RSTN_PVD_LV_SHIFT))&PMU_RSTN_PVD_LV_MASK)
/** REG_EN_OSC_LV_ASYNC Bit Fields */
#define PMU_REG_EN_OSC_LV_ASYNC_MASK              0x1U
#define PMU_REG_EN_OSC_LV_ASYNC_SHIFT             0U
#define PMU_REG_EN_OSC_LV_ASYNC_WIDTH             1U
#define PMU_REG_EN_OSC_LV_ASYNC(x)                (((uint32_t)(((uint32_t)(x))<<PMU_REG_EN_OSC_LV_ASYNC_SHIFT))&PMU_REG_EN_OSC_LV_ASYNC_MASK)
/** STANDBY_MODE_CNT Bit Fields */
#define PMU_STANDBY_MODE_CNT_MASK                 0xFFFFU
#define PMU_STANDBY_MODE_CNT_SHIFT                0U
#define PMU_STANDBY_MODE_CNT_WIDTH                16U
#define PMU_STANDBY_MODE_CNT(x)                   (((uint32_t)(((uint32_t)(x))<<PMU_STANDBY_MODE_CNT_SHIFT))&PMU_STANDBY_MODE_CNT_MASK)
/** WAKEUP_DISABLE Bit Fields */
#define PMU_WAKEUP_DISABLE_MASK                   0x3FFU
#define PMU_WAKEUP_DISABLE_SHIFT                  0U
#define PMU_WAKEUP_DISABLE_WIDTH                  10U
#define PMU_WAKEUP_DISABLE(x)                     (((uint32_t)(((uint32_t)(x))<<PMU_WAKEUP_DISABLE_SHIFT))&PMU_WAKEUP_DISABLE_MASK)
/** BOR_RSTN_MASK Bit Fields */
#define PMU_BOR_RSTN_MASK_MASK                    0x1U
#define PMU_BOR_RSTN_MASK_SHIFT                   0U
#define PMU_BOR_RSTN_MASK_WIDTH                   1U
#define PMU_BOR_RSTN_MASK(x)                      (((uint32_t)(((uint32_t)(x))<<PMU_BOR_RSTN_MASK_SHIFT))&PMU_BOR_RSTN_MASK_MASK)
/** SW_SYS_RSTN Bit Fields */
#define PMU_SW_SYS_RSTN_MASK                      0x1U
#define PMU_SW_SYS_RSTN_SHIFT                     0U
#define PMU_SW_SYS_RSTN_WIDTH                     1U
#define PMU_SW_SYS_RSTN(x)                        (((uint32_t)(((uint32_t)(x))<<PMU_SW_SYS_RSTN_SHIFT))&PMU_SW_SYS_RSTN_MASK)

/**
 * @}
 */ /* end of group PMU_Register_Masks */

/**
 * @}
 */ /* end of group PMU_Peripheral_Access_Layer */
// clang-format on

#endif // __PMU_REGISTER_H__
