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
 * @file adma_register.h
 * @brief ADMA driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __ADMA_REGISTER_H__
#define __ADMA_REGISTER_H__

#include "emps_platform.h"
#include <osal.h>
#include <stdint.h>

// clang-format off
/* ----------------------------------------------------------------------------
   -- ADMA Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup ADMA_Peripheral_Access_Layer ADMA Peripheral Access Layer
 * @{
 */

/**
 * @brief ADMA Register Layout Typedef
 */
typedef struct {
    struct {
        __IO uint32_t CMD_REG0;          /**< ADMA Channel's command, first line out of 0x0, array offset: 0x0, array step: 0x100 */
        __IO uint32_t CMD_REG1;          /**< ADMA Channel's command, second line out of 0x4, array offset: 0x0, array step: 0x100 */
        __IO uint32_t CMD_REG2;          /**< ADMA Channel's command, third line out of 0x8, array offset: 0x0, array step: 0x100 */
        __IO uint32_t CMD_REG3;          /**< ADMA Channel's command, last line out of 0xC, array offset: 0x0, array step: 0x100 */
        __IO uint32_t STATIC_REG0;       /**< ADMA Channel's static configuration, array offset: 0x10, array step: 0x100 */
        __IO uint32_t STATIC_REG1;       /**< ADMA Channel's static configuration, array offset: 0x14, array step: 0x100 */
        __IO uint32_t STATIC_REG2;       /**< ADMA Channel's static configuration, array offset: 0x18, array step: 0x100 */
        __IO uint32_t STATIC_REG3;       /**< ADMA Channel's static configuration, array offset: 0x1C, array step: 0x100 */
        __IO uint32_t STATIC_REG4;       /**< ADMA Channel's static configuration, array offset: 0x20, array step: 0x100 */
        __IO uint32_t RESERVED_0[2];
        __IO uint32_t RESTRICT_REG;      /**< ADMA Channel's restrictions status register, array offset: 0x2C, array step: 0x100 */
        __IO uint32_t READ_OFFSET_REG;   /**< ADMA Channel's read offset status register, array offset: 0x30, array step: 0x100 */
        __IO uint32_t WRITE_OFFSET_REG;  /**< ADMA Channel's read offset status register, array offset: 0x34, array step: 0x100 */
        __IO uint32_t FIFO_FULLNESS_REG; /**< ADMA FIFO fullness status register, array offset: 0x38, array step: 0x100 */
        __IO uint32_t RESERVED_1;
        __IO uint32_t CH_ENABLE_REG;     /**< ADMA Channel enable register, array offset: 0x40, array step: 0x100 */
        __IO uint32_t CH_START_REG;      /**< ADMA Channel start register, array offset: 0x44, array step: 0x100 */
        __IO uint32_t CH_ACTIVE_REG;     /**< ADMA Channel active status register, array offset: 0x48, array step: 0x100 */
        __IO uint32_t RESERVED_2;
        __IO uint32_t COUNT_REG;         /**< ADMA Buffer counter status register, array offset: 0x50, array step: 0x100 */
        __IO uint32_t RESERVED_3[19];
        __IO uint32_t INT_RAWSTAT_REG;   /**< ADMA Interrupt raw status register, array offset: 0xA0, array step: 0x100 */
        __IO uint32_t INT_CLEAR_REG;     /**< ADMA Interrupt clear register, array offset: 0xA4, array step: 0x100 */
        __IO uint32_t INT_ENABLE_REG;    /**< ADMA Interrupt enable register, array offset: 0xA8, array step: 0x100 */
        __IO uint32_t INT_STATUS_REG;    /**< ADMA Interrupt status register, array offset: 0xAC, array step: 0x100 */
        __IO uint32_t RESERVED_4[20];
    } CHREG[ADMA_CH_COUNT];

    __IO uint32_t RESERVED_0[512];
    __IO uint32_t INT0_STATUS;          /**< Status register indicating which channels caused an interrupt on INT[0], offset: 0x1000 */
    __IO uint32_t INT1_STATUS;          /**< Status register indicating which channels caused an interrupt on INT[1], offset: 0x1004 */
    __IO uint32_t INT2_STATUS;          /**< Status register indicating which channels caused an interrupt on INT[2], offset: 0x1008 */
    __IO uint32_t INT3_STATUS;          /**< Status register indicating which channels caused an interrupt on INT[3], offset: 0x100C */
    __IO uint32_t INT4_STATUS;          /**< Status register indicating which channels caused an interrupt on INT[4], offset: 0x1010 */
    __IO uint32_t INT5_STATUS;          /**< Status register indicating which channels caused an interrupt on INT[5], offset: 0x1014 */
    __IO uint32_t INT6_STATUS;          /**< Status register indicating which channels caused an interrupt on INT[6], offset: 0x1018 */
    __IO uint32_t INT7_STATUS;          /**< Status register indicating which channels caused an interrupt on INT[7], offset: 0x101C */
    __IO uint32_t RESERVED_1[4];
    union {
        struct {
            __IO uint32_t CORE0_JOINT_MODE;     /**< Core 0 joint mode, offset: 0x1030 */
            __IO uint32_t CORE1_JOINT_MODE;     /**< Core 1 joint mode, offset: 0x1034 */
        };
        __IO uint32_t CORE_JOINT_MODE[2];
    } OPMODE;
    __IO uint32_t CORE0_PRIORITY;               /**< Core 0 priority channels, offset: 0x1038 */
    __IO uint32_t CORE1_PRIORITY;               /**< Core 0 priority channels, offset: 0x103C */
    __IO uint32_t CORE1_CLKDIV;                 /**< Core 1 clock divider, offset: 0x1040 */
    __IO uint32_t RESERVED_2;
    union {
        struct {
            __IO uint32_t CORE0_CH_START;       /**< Core 0 channel start, offset: 0x1048 */
            __IO uint32_t CORE1_CH_START;       /**< Core 1 channel start, offset: 0x104C */
        };
        __IO uint32_t CORE_CH_START[2];
    } CHSTART;
    __IO uint32_t PERIPH_RX_CTRL;               /**< Direct control of peripheral RX request, offset: 0x1050 */
    __IO uint32_t PERIPH_TX_CTRL;               /**< Direct control of peripheral TX request, offset: 0x1054 */
    __IO uint32_t RESERVED_3[30];
    __IO uint32_t IDLE;                         /**< Idle indication register, offset: 0x10D0 */
    __IO uint32_t RESERVED_4[3];
    __IO uint32_t USER_DEF_STATUS;              /**< Status register indicating user defined configurations, offset: 0x10E0 */
    __IO uint32_t RESERVED_5[3];
    __IO uint32_t USER_CORE0_DEF_STATUS0;       /**< Status register indicating user defined configurations, offset: 0x10F0 */
    __IO uint32_t USER_CORE0_DEF_STATUS1;       /**< Status register indicating user defined configurations, offset: 0x10F4 */
    __IO uint32_t USER_CORE1_DEF_STATUS0;       /**< Status register indicating user defined configurations, offset: 0x10F8 */
    __IO uint32_t USER_CORE1_DEF_STATUS1;       /**< Status register indicating user defined configurations, offset: 0x10FC */
} adma_type_t, *adma_mem_map_ptr_t;

/* ----------------------------------------------------------------------------
   -- ADMA Register Masks
   ---------------------------------------------------------------------------- */
/**
 * @addtogroup ADMA_Register_Masks ADMA Register Masks
 * @{
 */

/** CMD_REG0 Bit Fields */
#define ADMA_CMD_REG0_RD_START_ADDR_MASK                  0xFFFFFFFFU
#define ADMA_CMD_REG0_RD_START_ADDR_SHIFT                 0U
#define ADMA_CMD_REG0_RD_START_ADDR_WIDTH                 32U
#define ADMA_CMD_REG0_RD_START_ADDR(x)                    (((uint32_t)(((uint32_t)(x)) << ADMA_CMD_REG0_RD_START_ADDR_SHIFT)) & ADMA_CMD_REG0_RD_START_ADDR_MASK)
/** CMD_REG1 Bit Fields */
#define ADMA_CMD_REG1_WR_START_ADDR_MASK                  0xFFFFFFFFU
#define ADMA_CMD_REG1_WR_START_ADDR_SHIFT                 0U
#define ADMA_CMD_REG1_WR_START_ADDR_WIDTH                 32U
#define ADMA_CMD_REG1_WR_START_ADDR(x)                    (((uint32_t)(((uint32_t)(x)) << ADMA_CMD_REG1_WR_START_ADDR_SHIFT)) & ADMA_CMD_REG1_WR_START_ADDR_MASK)
/** CMD_REG2 Bit Fields */
#define ADMA_CMD_REG2_BUFFER_SIZE_MASK                    0xFFFFU
#define ADMA_CMD_REG2_BUFFER_SIZE_SHIFT                   0U
#define ADMA_CMD_REG2_BUFFER_SIZE_WIDTH                   16U
#define ADMA_CMD_REG2_BUFFER_SIZE(x)                      (((uint32_t)(((uint32_t)(x)) << ADMA_CMD_REG2_BUFFER_SIZE_SHIFT)) & ADMA_CMD_REG2_BUFFER_SIZE_MASK)
/** CMD_REG3 Bit Fields */
#define ADMA_CMD_REG3_CMD_SET_INT_MASK                    0x1U
#define ADMA_CMD_REG3_CMD_SET_INT_SHIFT                   0U
#define ADMA_CMD_REG3_CMD_SET_INT_WIDTH                   1U
#define ADMA_CMD_REG3_CMD_SET_INT(x)                      (((uint32_t)(((uint32_t)(x)) << ADMA_CMD_REG3_CMD_SET_INT_SHIFT)) & ADMA_CMD_REG3_CMD_SET_INT_MASK)
#define ADMA_CMD_REG3_CMD_LAST_MASK                       0x2U
#define ADMA_CMD_REG3_CMD_LAST_SHIFT                      1U
#define ADMA_CMD_REG3_CMD_LAST_WIDTH                      1U
#define ADMA_CMD_REG3_CMD_LAST(x)                         (((uint32_t)(((uint32_t)(x)) << ADMA_CMD_REG3_CMD_LAST_SHIFT)) & ADMA_CMD_REG3_CMD_LAST_MASK)
#define ADMA_CMD_REG3_CMD_NEXT_ADDR_MASK                  0xFFFFFFF0U
#define ADMA_CMD_REG3_CMD_NEXT_ADDR_SHIFT                 4U
#define ADMA_CMD_REG3_CMD_NEXT_ADDR_WIDTH                 28U
#define ADMA_CMD_REG3_CMD_NEXT_ADDR(x)                    (((uint32_t)(((uint32_t)(x)) << ADMA_CMD_REG3_CMD_NEXT_ADDR_SHIFT)) & ADMA_CMD_REG3_CMD_NEXT_ADDR_MASK)
/** STATIC_REG0 Bit Fields */
#define ADMA_STATIC_REG0_RD_BURST_MAX_SIZE_MASK           0x3FFU
#define ADMA_STATIC_REG0_RD_BURST_MAX_SIZE_SHIFT          0U
#define ADMA_STATIC_REG0_RD_BURST_MAX_SIZE_WIDTH          10U
#define ADMA_STATIC_REG0_RD_BURST_MAX_SIZE(x)             (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG0_RD_BURST_MAX_SIZE_SHIFT)) & ADMA_STATIC_REG0_RD_BURST_MAX_SIZE_MASK)
#define ADMA_STATIC_REG0_RD_ALLOW_FULL_BURST_MASK         0x1000U
#define ADMA_STATIC_REG0_RD_ALLOW_FULL_BURST_SHIFT        12U
#define ADMA_STATIC_REG0_RD_ALLOW_FULL_BURST_WIDTH        1U
#define ADMA_STATIC_REG0_RD_ALLOW_FULL_BURST(x)           (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG0_RD_ALLOW_FULL_BURST_SHIFT)) & ADMA_STATIC_REG0_RD_ALLOW_FULL_BURST_MASK)
#define ADMA_STATIC_REG0_RD_ALLOW_FULL_FIFO_MASK          0x2000U
#define ADMA_STATIC_REG0_RD_ALLOW_FULL_FIFO_SHIFT         13U
#define ADMA_STATIC_REG0_RD_ALLOW_FULL_FIFO_WIDTH         1U
#define ADMA_STATIC_REG0_RD_ALLOW_FULL_FIFO(x)            (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG0_RD_ALLOW_FULL_FIFO_SHIFT)) & ADMA_STATIC_REG0_RD_ALLOW_FULL_FIFO_MASK)
#define ADMA_STATIC_REG0_RD_TOKENS_MASK                   0x3F0000U
#define ADMA_STATIC_REG0_RD_TOKENS_SHIFT                  16U
#define ADMA_STATIC_REG0_RD_TOKENS_WIDTH                  6U
#define ADMA_STATIC_REG0_RD_TOKENS(x)                     (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG0_RD_TOKENS_SHIFT)) & ADMA_STATIC_REG0_RD_TOKENS_MASK)
#define ADMA_STATIC_REG0_RD_OUTSTANDING_MASK              0x40000000U
#define ADMA_STATIC_REG0_RD_OUTSTANDING_SHIFT             30U
#define ADMA_STATIC_REG0_RD_OUTSTANDING_WIDTH             1U
#define ADMA_STATIC_REG0_RD_OUTSTANDING(x)                (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG0_RD_OUTSTANDING_SHIFT)) & ADMA_STATIC_REG0_RD_OUTSTANDING_MASK)
#define ADMA_STATIC_REG0_RD_INCR_MASK                     0x80000000U
#define ADMA_STATIC_REG0_RD_INCR_SHIFT                    31U
#define ADMA_STATIC_REG0_RD_INCR_WIDTH                    1U
#define ADMA_STATIC_REG0_RD_INCR(x)                       (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG0_RD_INCR_SHIFT)) & ADMA_STATIC_REG0_RD_INCR_MASK)
/** STATIC_REG1 Bit Fields */
#define ADMA_STATIC_REG1_WR_BURST_MAX_SIZE_MASK           0x3FFU
#define ADMA_STATIC_REG1_WR_BURST_MAX_SIZE_SHIFT          0U
#define ADMA_STATIC_REG1_WR_BURST_MAX_SIZE_WIDTH          10U
#define ADMA_STATIC_REG1_WR_BURST_MAX_SIZE(x)             (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG1_WR_BURST_MAX_SIZE_SHIFT)) & ADMA_STATIC_REG1_WR_BURST_MAX_SIZE_MASK)
#define ADMA_STATIC_REG1_WR_ALLOW_FULL_BURST_MASK         0x1000U
#define ADMA_STATIC_REG1_WR_ALLOW_FULL_BURST_SHIFT        12U
#define ADMA_STATIC_REG1_WR_ALLOW_FULL_BURST_WIDTH        1U
#define ADMA_STATIC_REG1_WR_ALLOW_FULL_BURST(x)           (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG1_WR_ALLOW_FULL_BURST_SHIFT)) & ADMA_STATIC_REG1_WR_ALLOW_FULL_BURST_MASK)
#define ADMA_STATIC_REG1_WR_ALLOW_FULL_FIFO_MASK          0x2000U
#define ADMA_STATIC_REG1_WR_ALLOW_FULL_FIFO_SHIFT         13U
#define ADMA_STATIC_REG1_WR_ALLOW_FULL_FIFO_WIDTH         1U
#define ADMA_STATIC_REG1_WR_ALLOW_FULL_FIFO(x)            (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG1_WR_ALLOW_FULL_FIFO_SHIFT)) & ADMA_STATIC_REG1_WR_ALLOW_FULL_FIFO_MASK)
#define ADMA_STATIC_REG1_WR_TOKENS_MASK                   0x3F0000U
#define ADMA_STATIC_REG1_WR_TOKENS_SHIFT                  16U
#define ADMA_STATIC_REG1_WR_TOKENS_WIDTH                  6U
#define ADMA_STATIC_REG1_WR_TOKENS(x)                     (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG1_WR_TOKENS_SHIFT)) & ADMA_STATIC_REG1_WR_TOKENS_MASK)
#define ADMA_STATIC_REG1_WR_OUTSTANDING_MASK              0x40000000U
#define ADMA_STATIC_REG1_WR_OUTSTANDING_SHIFT             30U
#define ADMA_STATIC_REG1_WR_OUTSTANDING_WIDTH             1U
#define ADMA_STATIC_REG1_WR_OUTSTANDING(x)                (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG1_WR_OUTSTANDING_SHIFT)) & ADMA_STATIC_REG1_WR_OUTSTANDING_MASK)
#define ADMA_STATIC_REG1_WR_INCR_MASK                     0x80000000U
#define ADMA_STATIC_REG1_WR_INCR_SHIFT                    31U
#define ADMA_STATIC_REG1_WR_INCR_WIDTH                    1U
#define ADMA_STATIC_REG1_WR_INCR(x)                       (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG1_WR_INCR_SHIFT)) & ADMA_STATIC_REG1_WR_INCR_MASK)
/** STATIC_REG2 Bit Fields */
#define ADMA_STATIC_REG2_FRAME_WIDTH_MASK                 0xFFFU
#define ADMA_STATIC_REG2_FRAME_WIDTH_SHIFT                0U
#define ADMA_STATIC_REG2_FRAME_WIDTH_WIDTH                12U
#define ADMA_STATIC_REG2_FRAME_WIDTH(x)                   (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG2_FRAME_WIDTH_SHIFT)) & ADMA_STATIC_REG2_FRAME_WIDTH_MASK)
#define ADMA_STATIC_REG2_BLOCK_MASK                       0x8000U
#define ADMA_STATIC_REG2_BLOCK_SHIFT                      15U
#define ADMA_STATIC_REG2_BLOCK_WIDTH                      1U
#define ADMA_STATIC_REG2_BLOCK(x)                         (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG2_BLOCK_SHIFT)) & ADMA_STATIC_REG2_BLOCK_MASK)
#define ADMA_STATIC_REG2_JOINT_MASK                       0x10000U
#define ADMA_STATIC_REG2_JOINT_SHIFT                      16U
#define ADMA_STATIC_REG2_JOINT_WIDTH                      1U
#define ADMA_STATIC_REG2_JOINT(x)                         (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG2_JOINT_SHIFT)) & ADMA_STATIC_REG2_JOINT_MASK)
#define ADMA_STATIC_REG2_AUTO_RETRY_MASK                  0x20000U
#define ADMA_STATIC_REG2_AUTO_RETRY_SHIFT                 17U
#define ADMA_STATIC_REG2_AUTO_RETRY_WIDTH                 1U
#define ADMA_STATIC_REG2_AUTO_RETRY(x)                    (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG2_AUTO_RETRY_SHIFT)) & ADMA_STATIC_REG2_AUTO_RETRY_MASK)
#define ADMA_STATIC_REG2_RD_CMD_PORT_NUM_MASK             0x100000U
#define ADMA_STATIC_REG2_RD_CMD_PORT_NUM_SHIFT            20U
#define ADMA_STATIC_REG2_RD_CMD_PORT_NUM_WIDTH            1U
#define ADMA_STATIC_REG2_RD_CMD_PORT_NUM(x)               (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG2_RD_CMD_PORT_NUM_SHIFT)) & ADMA_STATIC_REG2_RD_CMD_PORT_NUM_MASK)
#define ADMA_STATIC_REG2_RD_PORT_NUM_MASK                 0x200000U
#define ADMA_STATIC_REG2_RD_PORT_NUM_SHIFT                21U
#define ADMA_STATIC_REG2_RD_PORT_NUM_WIDTH                1U
#define ADMA_STATIC_REG2_RD_PORT_NUM(x)                   (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG2_R_PORT_NUM_SHIFT)) & ADMA_STATIC_REG2_R_PORT_NUM_MASK)
#define ADMA_STATIC_REG2_WR_PORT_NUM_MASK                 0x400000U
#define ADMA_STATIC_REG2_WR_PORT_NUM_SHIFT                22U
#define ADMA_STATIC_REG2_WR_PORT_NUM_WIDTH                1U
#define ADMA_STATIC_REG2_WR_PORT_NUM(x)                   (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG2_WR_PORT_NUM_SHIFT)) & ADMA_STATIC_REG2_WR_PORT_NUM_MASK)
#define ADMA_STATIC_REG2_INT_NUM_MASK                     0x7000000U
#define ADMA_STATIC_REG2_INT_NUM_SHIFT                    24U
#define ADMA_STATIC_REG2_INT_NUM_WIDTH                    3U
#define ADMA_STATIC_REG2_INT_NUM(x)                       (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG2_INT_NUM_SHIFT)) & ADMA_STATIC_REG2_INT_NUM_MASK)
#define ADMA_STATIC_REG2_END_SWAP_MASK                    0x30000000U
#define ADMA_STATIC_REG2_END_SWAP_SHIFT                   28U
#define ADMA_STATIC_REG2_END_SWAP_WIDTH                   2U
#define ADMA_STATIC_REG2_END_SWAP(x)                      (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG2_END_SWAP_SHIFT)) & ADMA_STATIC_REG2_END_SWAP_MASK)
/** STATIC_REG3 Bit Fields */
#define ADMA_STATIC_REG3_RD_WAIT_LIMIT_MASK               0xFFFU
#define ADMA_STATIC_REG3_RD_WAIT_LIMIT_SHIFT              0U
#define ADMA_STATIC_REG3_RD_WAIT_LIMIT_WIDTH              12U
#define ADMA_STATIC_REG3_RD_WAIT_LIMIT(x)                 (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG3_RD_WAIT_LIMIT_SHIFT)) & ADMA_STATIC_REG3_RD_WAIT_LIMIT_MASK)
#define ADMA_STATIC_REG3_WR_WAIT_LIMIT_MASK               0xFFF0000U
#define ADMA_STATIC_REG3_WR_WAIT_LIMIT_SHIFT              16U
#define ADMA_STATIC_REG3_WR_WAIT_LIMIT_WIDTH              12U
#define ADMA_STATIC_REG3_WR_WAIT_LIMIT(x)                 (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG3_WR_WAIT_LIMIT_SHIFT)) & ADMA_STATIC_REG3_WR_WAIT_LIMIT_MASK)
/** STATIC_REG4 Bit Fields */
#define ADMA_STATIC_REG4_RD_PERIPH_NUM_MASK               0x1FU
#define ADMA_STATIC_REG4_RD_PERIPH_NUM_SHIFT              0U
#define ADMA_STATIC_REG4_RD_PERIPH_NUM_WIDTH              5U
#define ADMA_STATIC_REG4_RD_PERIPH_NUM(x)                 (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG4_RD_PERIPH_NUM_SHIFT)) & ADMA_STATIC_REG4_RD_PERIPH_NUM_MASK)
#define ADMA_STATIC_REG4_RD_PERIPH_DELAY_MASK             0x700U
#define ADMA_STATIC_REG4_RD_PERIPH_DELAY_SHIFT            8U
#define ADMA_STATIC_REG4_RD_PERIPH_DELAY_WIDTH            3U
#define ADMA_STATIC_REG4_RD_PERIPH_DELAY(x)               (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG4_RD_PERIPH_DELAY_SHIFT)) & ADMA_STATIC_REG4_RD_PERIPH_DELAY_MASK)
#define ADMA_STATIC_REG4_RD_PERIPH_BLOCK_MASK             0x8000U
#define ADMA_STATIC_REG4_RD_PERIPH_BLOCK_SHIFT            15U
#define ADMA_STATIC_REG4_RD_PERIPH_BLOCK_WIDTH            1U
#define ADMA_STATIC_REG4_RD_PERIPH_BLOCK(x)               (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG4_RD_PERIPH_BLOCK_SHIFT)) & ADMA_STATIC_REG4_RD_PERIPH_BLOCK_MASK)
#define ADMA_STATIC_REG4_WR_PERIPH_NUM_MASK               0x1FU
#define ADMA_STATIC_REG4_WR_PERIPH_NUM_SHIFT              16U
#define ADMA_STATIC_REG4_WR_PERIPH_NUM_WIDTH              5U
#define ADMA_STATIC_REG4_WR_PERIPH_NUM(x)                 (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG4_WR_PERIPH_NUM_SHIFT)) & ADMA_STATIC_REG4_WR_PERIPH_NUM_MASK)
#define ADMA_STATIC_REG4_WR_PERIPH_DELAY_MASK             0x700U
#define ADMA_STATIC_REG4_WR_PERIPH_DELAY_SHIFT            24U
#define ADMA_STATIC_REG4_WR_PERIPH_DELAY_WIDTH            3U
#define ADMA_STATIC_REG4_WR_PERIPH_DELAY(x)               (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG4_WR_PERIPH_DELAY_SHIFT)) & ADMA_STATIC_REG4_WR_PERIPH_DELAY_MASK)
#define ADMA_STATIC_REG4_WR_PERIPH_BLOCK_MASK             0x8000U
#define ADMA_STATIC_REG4_WR_PERIPH_BLOCK_SHIFT            31U
#define ADMA_STATIC_REG4_WR_PERIPH_BLOCK_WIDTH            1U
#define ADMA_STATIC_REG4_WR_PERIPH_BLOCK(x)               (((uint32_t)(((uint32_t)(x)) << ADMA_STATIC_REG4_WR_PERIPH_BLOCK_SHIFT)) & ADMA_STATIC_REG4_WR_PERIPH_BLOCK_MASK)
/** RESTRICT_REG Bit Fields */
#define ADMA_RESTRICT_REG_RD_ALLOW_FULL_FIFO_MASK         0x1U
#define ADMA_RESTRICT_REG_RD_ALLOW_FULL_FIFO_SHIFT        0U
#define ADMA_RESTRICT_REG_RD_ALLOW_FULL_FIFO_WIDTH        1U
#define ADMA_RESTRICT_REG_RD_ALLOW_FULL_FIFO(x)           (((uint32_t)(((uint32_t)(x)) << ADMA_RESTRICT_REG_RD_ALLOW_FULL_FIFO_SHIFT)) & ADMA_RESTRICT_REG_RD_ALLOW_FULL_FIFO_MASK)
#define ADMA_RESTRICT_REG_WR_ALLOW_FULL_FIFO_MASK         0x1U
#define ADMA_RESTRICT_REG_WR_ALLOW_FULL_FIFO_SHIFT        1U
#define ADMA_RESTRICT_REG_WR_ALLOW_FULL_FIFO_WIDTH        1U
#define ADMA_RESTRICT_REG_WR_ALLOW_FULL_FIFO(x)           (((uint32_t)(((uint32_t)(x)) << ADMA_RESTRICT_REG_WR_ALLOW_FULL_FIFO_SHIFT)) & ADMA_RESTRICT_REG_WR_ALLOW_FULL_FIFO_MASK)
#define ADMA_RESTRICT_ALLOW_FULL_FIFO_MASK                0x1U
#define ADMA_RESTRICT_ALLOW_FULL_FIFO_SHIFT               2U
#define ADMA_RESTRICT_ALLOW_FULL_FIFO_WIDTH               1U
#define ADMA_RESTRICT_ALLOW_FULL_FIFO(x)                  (((uint32_t)(((uint32_t)(x)) << ADMA_RESTRICT_ALLOW_FULL_FIFO_SHIFT)) & ADMA_RESTRICT_ALLOW_FULL_FIFO_MASK)
#define ADMA_RESTRICT_ALLOW_FULL_BURST_MASK               0x1U
#define ADMA_RESTRICT_ALLOW_FULL_BURST_SHIFT              3U
#define ADMA_RESTRICT_ALLOW_FULL_BURST_WIDTH              1U
#define ADMA_RESTRICT_ALLOW_FULL_BURST(x)                 (((uint32_t)(((uint32_t)(x)) << ADMA_RESTRICT_ALLOW_FULL_BURST_SHIFT)) & ADMA_RESTRICT_ALLOW_FULL_BURST_MASK)
#define ADMA_RESTRICT_ALLOW_JOINT_BURST_MASK              0x1U
#define ADMA_RESTRICT_ALLOW_JOINT_BURST_SHIFT             4U
#define ADMA_RESTRICT_ALLOW_JOINT_BURST_WIDTH             1U
#define ADMA_RESTRICT_ALLOW_JOINT_BURST(x)                (((uint32_t)(((uint32_t)(x)) << ADMA_RESTRICT_ALLOW_JOINT_BURST_SHIFT)) & ADMA_RESTRICT_ALLOW_JOINT_BURST_MASK)
#define ADMA_RESTRICT_RD_OUTSTANDING_STAT_MASK            0x1U
#define ADMA_RESTRICT_RD_OUTSTANDING_STAT_SHIFT           5U
#define ADMA_RESTRICT_RD_OUTSTANDING_STAT_WIDTH           1U
#define ADMA_RESTRICT_RD_OUTSTANDING_STAT(x)              (((uint32_t)(((uint32_t)(x)) << ADMA_RESTRICT_RD_OUTSTANDING_STAT_MASK)) & ADMA_RESTRICT_RD_OUTSTANDING_STAT_MASK)
#define ADMA_RESTRICT_WR_OUTSTANDING_STAT_MASK            0x1U
#define ADMA_RESTRICT_WR_OUTSTANDING_STAT_SHIFT           6U
#define ADMA_RESTRICT_WR_OUTSTANDING_STAT_WIDTH           1U
#define ADMA_RESTRICT_WR_OUTSTANDING_STAT(x)              (((uint32_t)(((uint32_t)(x)) << ADMA_RESTRICT_WR_OUTSTANDING_STAT_SHIFT)) & ADMA_RESTRICT_WR_OUTSTANDING_STAT_MASK)
#define ADMA_RESTRICT_BLOCK_NON_ALIGN_STAT_MASK           0x1U
#define ADMA_RESTRICT_BLOCK_NON_ALIGN_STAT_SHIFT          7U
#define ADMA_RESTRICT_BLOCK_NON_ALIGN_STAT_WIDTH          1U
#define ADMA_RESTRICT_BLOCK_NON_ALIGN_STAT(x)             (((uint32_t)(((uint32_t)(x)) << ADMA_RESTRICT_BLOCK_NON_ALIGN_STAT_SHIFT)) & ADMA_RESTRICT_BLOCK_NON_ALIGN_STAT_MASK)
#define ADMA_RESTRICT_SIMPLE_STAT_MASK                    0x1U
#define ADMA_RESTRICT_SIMPLE_STAT_SHIFT                   8U
#define ADMA_RESTRICT_SIMPLE_STAT_WIDTH                   1U
#define ADMA_RESTRICT_SIMPLE_STAT(x)                      (((uint32_t)(((uint32_t)(x)) << ADMA_RESTRICT_SIMPLE_STAT_SHIFT)) & ADMA_RESTRICT_SIMPLE_STAT_MASK)
/** READ_OFFSET_REG Bit Fields */
#define ADMA_READ_OFFSET_REG_RD_OFFSET_MASK               0xFFFFU
#define ADMA_READ_OFFSET_REG_RD_OFFSET_SHIFT              0U
#define ADMA_READ_OFFSET_REG_RD_OFFSET_WIDTH              16U
#define ADMA_READ_OFFSET_REG_RD_OFFSET(x)                 (((uint32_t)(((uint32_t)(x)) << ADMA_READ_OFFSET_REG_RD_OFFSET_SHIFT)) & ADMA_READ_OFFSET_REG_RD_OFFSET_MASK)
/** WRITE_OFFSET_REG Bit Fields */
#define ADMA_WRITE_OFFSET_REG_WR_OFFSET_MASK              0xFFFFU
#define ADMA_WRITE_OFFSET_REG_WR_OFFSET_SHIFT             0U
#define ADMA_WRITE_OFFSET_REG_WR_OFFSET_WIDTH             16U
#define ADMA_WRITE_OFFSET_REG_WR_OFFSET(x)                (((uint32_t)(((uint32_t)(x)) << ADMA_WRITE_OFFSET_REG_WR_OFFSET_SHIFT)) & ADMA_WRITE_OFFSET_REG_WR_OFFSET_MASK)
/** FIFO_FULLNESS_REG Bit Fields */
#define ADMA_FIFO_FULLNESS_REG_RD_GAP_MASK                0x3FFU
#define ADMA_FIFO_FULLNESS_REG_RD_GAP_SHIFT               0U
#define ADMA_FIFO_FULLNESS_REG_RD_GAP_WIDTH               10U
#define ADMA_FIFO_FULLNESS_REG_RD_GAP(x)                  (((uint32_t)(((uint32_t)(x)) << ADMA_FIFO_FULLNESS_REG_RD_GAP_SHIFT)) & ADMA_FIFO_FULLNESS_REG_RD_GAP_MASK)
#define ADMA_FIFO_FULLNESS_REG_WR_FULLNESS_MASK           0x3FF0000U
#define ADMA_FIFO_FULLNESS_REG_WR_FULLNESS_SHIFT          16U
#define ADMA_FIFO_FULLNESS_REG_WR_FULLNESS_WIDTH          10U
#define ADMA_FIFO_FULLNESS_REG_WR_FULLNESS(x)             (((uint32_t)(((uint32_t)(x)) << ADMA_FIFO_FULLNESS_REG_WR_FULLNESS_SHIFT)) & ADMA_FIFO_FULLNESS_REG_RD_GAP_MASK)
/** CH_ENABLE_REG Bit Fields */
#define ADMA_CH_ENABLE_REG_CH_ENABLE_MASK                 0x1U
#define ADMA_CH_ENABLE_REG_CH_ENABLE_SHIFT                0U
#define ADMA_CH_ENABLE_REG_CH_ENABLE_WIDTH                1U
#define ADMA_CH_ENABLE_REG_CH_ENABLE(x)                   (((uint32_t)(((uint32_t)(x)) << ADMA_CH_ENABLE_REG_CH_ENABLE_SHIFT)) & ADMA_CH_ENABLE_REG_CH_ENABLE_MASK)
/** CH_START_REG Bit Fields */
#define ADMA_CH_START_REG_CH_START_MASK                   0x1U
#define ADMA_CH_START_REG_CH_START_SHIFT                  0U
#define ADMA_CH_START_REG_CH_START_WIDTH                  1U
#define ADMA_CH_START_REG_CH_START(x)                     (((uint32_t)(((uint32_t)(x)) << ADMA_CH_START_REG_CH_START_SHIFT)) & ADMA_CH_START_REG_CH_START_MASK)
/** CH_ENABLE_REG Bit Fields */
#define ADMA_CH_ACTIVE_REG_CH_RD_ACTIVE_MASK              0x1U
#define ADMA_CH_ACTIVE_REG_CH_RD_ACTIVE_SHIFT             0U
#define ADMA_CH_ACTIVE_REG_CH_RD_ACTIVE_WIDTH             1U
#define ADMA_CH_ACTIVE_REG_CH_RD_ACTIVE(x)                (((uint32_t)(((uint32_t)(x)) << ADMA_CH_ACTIVE_REG_CH_RD_ACTIVE_SHIFT)) & ADMA_CH_ACTIVE_REG_CH_RD_ACTIVE_MASK)
#define ADMA_CH_ACTIVE_REG_CH_WR_ACTIVE_MASK              0x1U
#define ADMA_CH_ACTIVE_REG_CH_WR_ACTIVE_SHIFT             0U
#define ADMA_CH_ACTIVE_REG_CH_WR_ACTIVE_WIDTH             1U
#define ADMA_CH_ACTIVE_REG_CH_WR_ACTIVE(x)                (((uint32_t)(((uint32_t)(x)) << ADMA_CH_ACTIVE_REG_CH_WR_ACTIVE_SHIFT)) & ADMA_CH_ACTIVE_REG_CH_WR_ACTIVE_MASK)
/** COUNT_REG Bit Fields */
#define ADMA_COUNT_REG2_BUFF_COUNT_MASK                   0xFFFFU
#define ADMA_COUNT_REG2_BUFF_COUNT_SHIFT                  0U
#define ADMA_COUNT_REG2_BUFF_COUNT_WIDTH                  16U
#define ADMA_COUNT_REG2_BUFF_COUNT(x)                     (((uint32_t)(((uint32_t)(x)) << ADMA_COUNT_REG2_BUFF_COUNT_SHIFT)) & ADMA_COUNT_REG2_BUFF_COUNT_MASK)
#define ADMA_COUNT_REG2_INT_COUNT_MASK                    0x3FU
#define ADMA_COUNT_REG2_INT_COUNT_SHIFT                   0U
#define ADMA_COUNT_REG2_INT_COUNT_WIDTH                   6U
#define ADMA_COUNT_REG2_INT_COUNT(x)                      (((uint32_t)(((uint32_t)(x)) << ADMA_COUNT_REG2_INT_COUNT_SHIFT)) & ADMA_COUNT_REG2_INT_COUNT_MASK)
/** INT_RAWSTAT_REG Bit Fields */
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_MASK             0xFFU
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_SHIFT            0U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_WIDTH            8U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT(x)               (((uint32_t)(((uint32_t)(x)) << ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_SHIFT)) & ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_MASK)
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_ERROR_MASK       0xFEU
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_ERROR_SHIFT      1U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_ERROR_WIDTH      7U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_ERROR(x)         (((uint32_t)(((uint32_t)(x)) << ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_ERROR_SHIFT)) & ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_ERROR_MASK)
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_CH_END_MASK      0x1U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_CH_END_SHIFT     0U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_CH_END_WIDTH     1U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_CH_END(x)        (((uint32_t)(((uint32_t)(x)) << ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_CH_END_SHIFT)) & ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_CH_END_MASK)
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_RD_SLVERR_MASK   0x1U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_RD_SLVERR_SHIFT  1U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_RD_SLVERR_WIDTH  1U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_RD_SLVERR(x)     (((uint32_t)(((uint32_t)(x)) << ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_RD_SLVERR_SHIFT)) & ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_RD_SLVERR_MASK)
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_WR_SLVERR_MASK   0x1U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_WR_SLVERR_SHIFT  2U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_WR_SLVERR_WIDTH  1U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_WR_SLVERR(x)     (((uint32_t)(((uint32_t)(x)) << ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_WR_SLVERR_SHIFT)) & ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_WR_SLVERR_MASK)
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_OVERFLOW_MASK    0x1U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_OVERFLOW_SHIFT   3U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_OVERFLOW_WIDTH   1U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_OVERFLOW(x)      (((uint32_t)(((uint32_t)(x)) << ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_OVERFLOW_SHIFT)) & ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_OVERFLOW_MASK)
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_UNDERFLOW_MASK   0x1U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_UNDERFLOW_SHIFT  4U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_UNDERFLOW_WIDTH  1U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_UNDERFLOW(x)     (((uint32_t)(((uint32_t)(x)) << ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_UNDERFLOW_SHIFT)) & ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_UNDERFLOW_MASK)
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_TIMEOUT_RD_MASK  0x1U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_TIMEOUT_RD_SHIFT 5U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_TIMEOUT_RD_WIDTH 1U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_TIMEOUT_RD(x)    (((uint32_t)(((uint32_t)(x)) << ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_TIMEOUT_RD_SHIFT)) & ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_TIMEOUT_RD_MASK)
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_TIMEOUT_WR_MASK  0x1U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_TIMEOUT_WR_SHIFT 6U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_TIMEOUT_WR_WIDTH 1U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_TIMEOUT_WR(x)    (((uint32_t)(((uint32_t)(x)) << ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_TIMEOUT_WR_SHIFT)) & ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_TIMEOUT_WR_MASK)
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_WDT_MASK         0x1U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_WDT_SHIFT        7U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_WDT_WIDTH        1U
#define ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_WDT(x)           (((uint32_t)(((uint32_t)(x)) << ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_WDT_SHIFT)) & ADMA_INT_RAWSTAT_REG_INT_RAWSTAT_WDT_MASK)
/** INT_CLEAR_REG Bit Fields W Only */
#define ADMA_INT_CLEAR_REG_INT_CLR_MASK                   0xFFU
#define ADMA_INT_CLEAR_REG_INT_CLR_SHIFT                  0U
#define ADMA_INT_CLEAR_REG_INT_CLR_WIDTH                  8U
#define ADMA_INT_CLEAR_REG_INT_CLR(x)                     (((uint32_t)(((uint32_t)(x)) << ADMA_INT_CLEAR_REG_INT_CLR_SHIFT)) & ADMA_INT_CLEAR_REG_INT_CLR_MASK)
#define ADMA_INT_CLEAR_REG_INT_CLR_ERROR_MASK             0xFEU
#define ADMA_INT_CLEAR_REG_INT_CLR_ERROR_SHIFT            1U
#define ADMA_INT_CLEAR_REG_INT_CLR_ERROR_WIDTH            7U
#define ADMA_INT_CLEAR_REG_INT_CLR_ERROR(x)               (((uint32_t)(((uint32_t)(x)) << ADMA_INT_CLEAR_REG_INT_CLR_ERROR_SHIFT)) & ADMA_INT_CLEAR_REG_INT_CLR_ERROR_MASK)
#define ADMA_INT_CLEAR_REG_INT_CLR_CH_END_MASK            0x1U
#define ADMA_INT_CLEAR_REG_INT_CLR_CH_END_SHIFT           0U
#define ADMA_INT_CLEAR_REG_INT_CLR_CH_END_WIDTH           1U
#define ADMA_INT_CLEAR_REG_INT_CLR_CH_END(x)              (((uint32_t)(((uint32_t)(x)) << ADMA_INT_CLEAR_REG_INT_CLR_CH_END_SHIFT)) & ADMA_INT_CLEAR_REG_INT_CLR_CH_END_MASK)
/** INT_ENABLE_REG Bit Fields */
#define ADMA_INT_ENABLE_REG_INT_ENABLE_MASK               0xFFU
#define ADMA_INT_ENABLE_REG_INT_ENABLE_SHIFT              0U
#define ADMA_INT_ENABLE_REG_INT_ENABLE_WIDTH              8U
#define ADMA_INT_ENABLE_REG_INT_ENABLE(x)                 (((uint32_t)(((uint32_t)(x)) << ADMA_INT_ENABLE_REG_INT_ENABLE_SHIFT)) & ADMA_INT_ENABLE_REG_INT_ENABLE_MASK)
#define ADMA_INT_ENABLE_REG_INT_ENABLE_ERROR_MASK         0xFEU
#define ADMA_INT_ENABLE_REG_INT_ENABLE_ERROR_SHIFT        1U
#define ADMA_INT_ENABLE_REG_INT_ENABLE_ERROR_WIDTH        7U
#define ADMA_INT_ENABLE_REG_INT_ENABLE_ERROR(x)           (((uint32_t)(((uint32_t)(x)) << ADMA_INT_ENABLE_REG_INT_ENABLE_ERROR_SHIFT)) & ADMA_INT_ENABLE_REG_INT_ENABLE_ERROR_MASK)
#define ADMA_INT_ENABLE_REG_INT_ENABLE_CH_END_MASK        0x1U
#define ADMA_INT_ENABLE_REG_INT_ENABLE_CH_END_SHIFT       0U
#define ADMA_INT_ENABLE_REG_INT_ENABLE_CH_END_WIDTH       1U
#define ADMA_INT_ENABLE_REG_INT_ENABLE_CH_END(x)          (((uint32_t)(((uint32_t)(x)) << ADMA_INT_ENABLE_REG_INT_ENABLE_CH_END_SHIFT)) & ADMA_INT_ENABLE_REG_INT_ENABLE_CH_END_MASK)
/** INT_STATUS_REG Bit Fields */
#define ADMA_INT_STATUS_REG_INT_STATUS_MASK               0xFFU
#define ADMA_INT_STATUS_REG_INT_STATUS_SHIFT              0U
#define ADMA_INT_STATUS_REG_INT_STATUS_WIDTH              8U
#define ADMA_INT_STATUS_REG_INT_STATUS(x)                 (((uint32_t)(((uint32_t)(x)) << ADMA_INT_STATUS_REG_INT_STATUS_SHIFT)) & ADMA_INT_STATUS_REG_INT_STATUS_MASK)
#define ADMA_INT_STATUS_REG_INT_STATUS_ERROR_MASK         0xFEU
#define ADMA_INT_STATUS_REG_INT_STATUS_ERROR_SHIFT        1U
#define ADMA_INT_STATUS_REG_INT_STATUS_ERROR_WIDTH        7U
#define ADMA_INT_STATUS_REG_INT_STATUS_ERROR(x)           (((uint32_t)(((uint32_t)(x)) << ADMA_INT_STATUS_REG_INT_STATUS_ERROR_SHIFT)) & ADMA_INT_STATUS_REG_INT_STATUS_ERROR_MASK)
#define ADMA_INT_STATUS_REG_INT_STATUS_CH_END_MASK        0x1U
#define ADMA_INT_STATUS_REG_INT_STATUS_CH_END_SHIFT       0U
#define ADMA_INT_STATUS_REG_INT_STATUS_CH_END_WIDTH       1U
#define ADMA_INT_STATUS_REG_INT_STATUS_CH_END(x)          (((uint32_t)(((uint32_t)(x)) << ADMA_INT_STATUS_REG_INT_STATUS_CH_END_SHIFT)) & ADMA_INT_STATUS_REG_INT_STATUS_CH_END_MASK)
#define ADMA_INT_STATUS_REG_INT_STATUS_RD_SLVERR_MASK     0x1U
#define ADMA_INT_STATUS_REG_INT_STATUS_RD_SLVERR_SHIFT    1U
#define ADMA_INT_STATUS_REG_INT_STATUS_RD_SLVERR_WIDTH    1U
#define ADMA_INT_STATUS_REG_INT_STATUS_RD_SLVERR(x)       (((uint32_t)(((uint32_t)(x)) << ADMA_INT_STATUS_REG_INT_STATUS_RD_SLVERR_SHIFT)) & ADMA_INT_STATUS_REG_INT_STATUS_RD_SLVERR_MASK)
#define ADMA_INT_STATUS_REG_INT_STATUS_WR_SLVERR_MASK     0x1U
#define ADMA_INT_STATUS_REG_INT_STATUS_WR_SLVERR_SHIFT    2U
#define ADMA_INT_STATUS_REG_INT_STATUS_WR_SLVERR_WIDTH    1U
#define ADMA_INT_STATUS_REG_INT_STATUS_WR_SLVERR(x)       (((uint32_t)(((uint32_t)(x)) << ADMA_INT_STATUS_REG_INT_STATUS_WR_SLVERR_SHIFT)) & ADMA_INT_STATUS_REG_INT_STATUS_WR_SLVERR_MASK)
#define ADMA_INT_STATUS_REG_INT_STATUS_OVERFLOW_MASK      0x1U
#define ADMA_INT_STATUS_REG_INT_STATUS_OVERFLOW_SHIFT     3U
#define ADMA_INT_STATUS_REG_INT_STATUS_OVERFLOW_WIDTH     1U
#define ADMA_INT_STATUS_REG_INT_STATUS_OVERFLOW(x)        (((uint32_t)(((uint32_t)(x)) << ADMA_INT_STATUS_REG_INT_STATUS_OVERFLOW_SHIFT)) & ADMA_INT_STATUS_REG_INT_STATUS_OVERFLOW_MASK)
#define ADMA_INT_STATUS_REG_INT_STATUS_UNDERFLOW_MASK     0x1U
#define ADMA_INT_STATUS_REG_INT_STATUS_UNDERFLOW_SHIFT    4U
#define ADMA_INT_STATUS_REG_INT_STATUS_UNDERFLOW_WIDTH    1U
#define ADMA_INT_STATUS_REG_INT_STATUS_UNDERFLOW(x)       (((uint32_t)(((uint32_t)(x)) << ADMA_INT_STATUS_REG_INT_STATUS_UNDERFLOW_SHIFT)) & ADMA_INT_STATUS_REG_INT_STATUS_UNDERFLOW_MASK)
#define ADMA_INT_STATUS_REG_INT_STATUS_TIMEOUT_RD_MASK    0x1U
#define ADMA_INT_STATUS_REG_INT_STATUS_TIMEOUT_RD_SHIFT   5U
#define ADMA_INT_STATUS_REG_INT_STATUS_TIMEOUT_RD_WIDTH   1U
#define ADMA_INT_STATUS_REG_INT_STATUS_TIMEOUT_RD(x)      (((uint32_t)(((uint32_t)(x)) << ADMA_INT_STATUS_REG_INT_STATUS_TIMEOUT_RD_SHIFT)) & ADMA_INT_STATUS_REG_INT_STATUS_TIMEOUT_RD_MASK)
#define ADMA_INT_STATUS_REG_INT_STATUS_TIMEOUT_WR_MASK    0x1U
#define ADMA_INT_STATUS_REG_INT_STATUS_TIMEOUT_WR_SHIFT   6U
#define ADMA_INT_STATUS_REG_INT_STATUS_TIMEOUT_WR_WIDTH   1U
#define ADMA_INT_STATUS_REG_INT_STATUS_TIMEOUT_WR(x)      (((uint32_t)(((uint32_t)(x)) << ADMA_INT_STATUS_REG_INT_STATUS_TIMEOUT_WR_SHIFT)) & ADMA_INT_STATUS_REG_INT_STATUS_TIMEOUT_WR_MASK)
#define ADMA_INT_STATUS_REG_INT_STATUS_WDT_MASK           0x1U
#define ADMA_INT_STATUS_REG_INT_STATUS_WDT_SHIFT          7U
#define ADMA_INT_STATUS_REG_INT_STATUS_WDT_WIDTH          1U
#define ADMA_INT_STATUS_REG_INT_STATUS_WDT(x)             (((uint32_t)(((uint32_t)(x)) << ADMA_INT_STATUS_REG_INT_STATUS_WDT_SHIFT)) & ADMA_INT_STATUS_REG_INT_STATUS_WDT_MASK)
/** INT0_STATUS Bit Fields */
#define ADMA_INT0_STATUS_INT_STATUS_MASK                  0xFFFFU
#define ADMA_INT0_STATUS_INT_STATUS_SHIFT                 0U
#define ADMA_INT0_STATUS_INT_STATUS_WIDTH                 16U
#define ADMA_INT0_STATUS_INT_STATUS(x)                    (((uint32_t)(((uint32_t)(x)) << ADMA_INT0_STATUS_INT_STATUS_SHIFT)) & ADMA_INT0_STATUS_INT_STATUS_MASK)
#define ADMA_INT0_STATUS_CORE0_CH_INT_STATUS_MASK         0xFFU
#define ADMA_INT0_STATUS_CORE0_CH_INT_STATUS_SHIFT        0U
#define ADMA_INT0_STATUS_CORE0_CH_INT_STATUS_WIDTH        8U
#define ADMA_INT0_STATUS_CORE0_CH_INT_STATUS(x)           (((uint32_t)(((uint32_t)(x)) << ADMA_INT0_STATUS_CORE0_CH_INT_STATUS_SHIFT)) & ADMA_INT0_STATUS_CORE0_CH_INT_STATUS_MASK)
#define ADMA_INT0_STATUS_CORE1_CH_INT_STATUS_MASK         0xFF00U
#define ADMA_INT0_STATUS_CORE1_CH_INT_STATUS_SHIFT        8U
#define ADMA_INT0_STATUS_CORE1_CH_INT_STATUS_WIDTH        8U
#define ADMA_INT0_STATUS_CORE1_CH_INT_STATUS(x)           (((uint32_t)(((uint32_t)(x)) << ADMA_INT0_STATUS_CORE1_CH_INT_STATUS_SHIFT)) & ADMA_INT0_STATUS_CORE1_CH_INT_STATUS_MASK)
/** COREx_JOINT_MODE Bit Fields */
#define ADMA_CORE_JOINT_MODE_MODE_MASK                    0x1U
#define ADMA_CORE_JOINT_MODE_MODE_SHIFT                   0U
#define ADMA_CORE_JOINT_MODE_MODE_WIDTH                   1U
#define ADMA_CORE_JOINT_MODE_MODE(x)                      (((uint32_t)(((uint32_t)(x)) << ADMA_CORE_JOINT_MODE_MODE_SHIFT)) & ADMA_CORE_JOINT_MODE_MODE_MASK)
/** CORE1_CLKDIV Bit Fields */
#define ADMA_CORE1_CLKDIV_RATIO_MASK                      0xFU
#define ADMA_CORE1_CLKDIV_RATIO_SHIFT                     0U
#define ADMA_CORE1_CLKDIV_RATIO_WIDTH                     4U
#define ADMA_CORE1_CLKDIV_RATIO(x)                        (((uint32_t)(((uint32_t)(x)) << ADMA_CORE1_CLKDIV_RATIO_SHIFT)) & ADMA_CORE1_CLKDIV_RATIO_MASK)
/** COREx_CH_START Bit Fields */
#define ADMA_CORE_CH_START_MASK                           0xFU
#define ADMA_CORE_CH_START_SHIFT                          0U
#define ADMA_CORE_CH_START_WIDTH                          4U
#define ADMA_CORE_CH_START(x)                             (((uint32_t)(((uint32_t)(x)) << ADMA_CORE1_CLKDIV_RATIO_SHIFT)) & ADMA_CORE1_CLKDIV_RATIO_MASK)

/**
 * @}
 */ /* end of group ADMA_Register_Masks */

/**
 * @}
 */ /* end of group ADMA_Peripheral_Access_Layer */
// clang-format on

#endif // __ADMA_REGISTER_H__
