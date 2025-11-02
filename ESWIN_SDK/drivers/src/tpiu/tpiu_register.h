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
 * @file tpiu_register.h
 * @brief TPIU driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __TPIU_REGISTER_H__
#define __TPIU_REGISTER_H__

#include <osal.h>
#include <stdint.h>

// clang-format off
/**
 * @brief TPIU control type
 */
typedef union {
    struct {
        volatile uint32_t teActive:1;              /**< bit 0    access: Rw       default: 0x0       */
        volatile uint32_t teEnable:1;              /**< bit 1    access: Rw       default: 0x0       */
        volatile uint32_t teTracing:1;             /**< bit 2    access: Rw       default: 0x0       */
        volatile uint32_t teEmpty:1;               /**< bit 3    access: Rw       default: 0x0       */
        volatile uint32_t teInstMode:3;            /**< bit 4..6  access: RO       default: 0x0       */
        volatile uint32_t reserved:25;             /**< bit 7..31    access:RO      default: 0x0       */
    } b;
    volatile uint32_t w;
} te_control_t;

/**
 * @brief TPIU implement type
 */
typedef union {
    struct {
        volatile uint32_t teVersion:4;             /**< bit 0-3    access: Rw       default: 0x0       */
        volatile uint32_t reserved:28;             /**< bit 4..31    access:RO      default: 0x0       */
    } b;
    volatile uint32_t w;
} te_impl_t;

/**
 * @brief TPIU trace type structure
 */
typedef struct {
    volatile uint32_t teControl;             /**< 0x00 */  
    volatile uint32_t teImpl;                /**< 0x04 */
} trace_type_t;

#define TRACE_BASE    (TRACE_CFG_BASEADDR)            
/** Peripheral CAN0 base pointer */
#define TRACE          ((trace_type_t *)TRACE_BASE)

/**
 * @brief TPIU support size type
 */
typedef union {
    struct {
        volatile uint32_t sup_port_size:32;              /**< bit 0..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} tpiu_support_size_type_t;

/**
 * @brief TPIU currort size type
 */
typedef union {
    struct {
        volatile uint32_t cur_port_size:32;              /**< bit 0..31    access: RW       default: 0x8000    */
    } b;
    volatile uint32_t w;
} tpiu_currort_size_type_t;

/**
 * @brief TPIU base address type
 */
typedef union {
    struct {
        volatile uint32_t base_address:32;               /**< bit 0..31    access: RW       default: 0x12030000  */
    } b;
    volatile uint32_t w;
} tpiu_base_address_type_t;

/**
 * @brief TPIU max time type
 */
typedef union {
    struct {
        volatile uint32_t max_time:16;                   /**< bit 0..15    access: RW       default: 0x1       */
        volatile uint32_t reserved:16;                   /**< bit 16..31   access: RW       default: 0x0       */
    } b;
    volatile uint32_t w;
} tpiu_max_time_type_t;

/**
 * @brief TPIU ram write pointer type
 */
typedef union {
    struct {
        volatile uint32_t ram_write_pointer:32;          /**< bit 0..31    access: RW       default: 0x0       */
    } b;
    volatile uint32_t w;
} tpiu_ram_write_pointer_type_t;

/**
 * @brief TPIU control reg type
 */
typedef union {
    struct {
        volatile uint32_t trace_path_sel:1;              /**< bit 0        access: RW       default: 0x0       */
        volatile uint32_t reserved:31;                   /**< bit 1..31    access: RW       default: 0x0       */
    } b;
    volatile uint32_t w;
} tpiu_control_reg_type_t;

/**
 * @brief TPIU format status type
 */
typedef union {
    struct {
        volatile uint32_t flush_status:1;                /**< bit 0        access: RO       default: 0x0       */
        volatile uint32_t reserved:31;                   /**< bit 1..31    access: RW       default: 0x0       */
    } b;
    volatile uint32_t w;
} tpiu_format_status_type_t;

/**
 * @brief TPIU format ctl type
 */
typedef union {
    struct {
        volatile uint32_t enable_formatter:1;            /**< bit 0        access: RW       default: 0x0       */
        volatile uint32_t continuous_mode:1;             /**< bit 1        access: RW       default: 0x0       */
        volatile uint32_t reserved0:4;                   /**< bit 2..5     access: RW       default: 0x0       */
        volatile uint32_t flush_on_man:1;                /**< bit 6        access: RW       default: 0x0       */
        volatile uint32_t reserved1:1;                   /**< bit 7        access: RW       default: 0x0       */
        volatile uint32_t stop_on_man:1;                 /**< bit 8        access: RW       default: 0x0       */
        volatile uint32_t reserved2:23;                  /**< bit 9..31    access: RW       default: 0x0       */
    } b;
    volatile uint32_t w;
} tpiu_format_ctl_type_t;

/**
 * @brief TPIU sync counter type
 */
typedef union {
    struct {
        volatile uint32_t sync_counter:12;               /**< bit 0..11    access: RW       default: 0x40      */
        volatile uint32_t reserved:20;                   /**< bit 12..31   access: RW       default: 0x0       */
    } b;
    volatile uint32_t w;
} tpiu_sync_counter_type_t;

/**
 * @brief TPIU type structure
 */
typedef struct {
    volatile uint32_t tpiu_support_size;             /**< 0x00 */  
    volatile uint32_t tpiu_curport_size;             /**< 0x04 */
    volatile uint32_t tpiu_base_address;             /**< 0x08 */
    volatile uint32_t tpiu_max_time;                 /**< 0x0c */
    volatile uint32_t tpiu_reserved_0;               /**<  */
    volatile uint32_t tpiu_reserved_1;               /**<  */
    volatile uint32_t tpiu_ram_write_pointer;        /**< 0x18 */
    volatile uint32_t tpiu_reserved_2;               /**<  */
    volatile uint32_t tpiu_control_reg;              /**< 0x20 */
    volatile uint32_t tpiu_reserved_3;               /**<  */
    volatile uint32_t tpiu_reserved_4;               /**<  */
    volatile uint32_t tpiu_reserved_5;               /**<  */
    volatile uint32_t tpiu_reserved_6;               /**<  */
    volatile uint32_t tpiu_reserved_7;               /**<  */
    volatile uint32_t tpiu_reserved_8;               /**<  */
    volatile uint32_t tpiu_reserved_9;               /**<  */
    volatile uint32_t tpiu_reserved_10;              /**<  */
    volatile uint32_t tpiu_reserved_11;              /**<  */
    volatile uint32_t tpiu_reserved_12;              /**<  */
    volatile uint32_t tpiu_reserved_13;              /**<  */
    volatile uint32_t tpiu_reserved_14;              /**<  */
    volatile uint32_t tpiu_reserved_15;              /**<  */
    volatile uint32_t tpiu_reserved_16;              /**<  */
    volatile uint32_t tpiu_reserved_17;              /**<  */
    volatile uint32_t tpiu_reserved_18;              /**<  */
    volatile uint32_t tpiu_reserved_19;              /**<  */
    volatile uint32_t tpiu_reserved_20;              /**<  */
    volatile uint32_t tpiu_reserved_21;              /**<  */
    volatile uint32_t tpiu_reserved_22;              /**<  */
    volatile uint32_t tpiu_reserved_23;              /**<  */
    volatile uint32_t tpiu_reserved_24;              /**<  */
    volatile uint32_t tpiu_reserved_25;              /**<  */
    volatile uint32_t tpiu_reserved_26;              /**<  */
    volatile uint32_t tpiu_reserved_27;              /**<  */
    volatile uint32_t tpiu_reserved_28;              /**<  */
    volatile uint32_t tpiu_reserved_29;              /**<  */
    volatile uint32_t tpiu_reserved_30;              /**<  */
    volatile uint32_t tpiu_reserved_31;              /**<  */
    volatile uint32_t tpiu_reserved_32;              /**<  */
    volatile uint32_t tpiu_reserved_33;              /**<  */
    volatile uint32_t tpiu_reserved_34;              /**<  */
    volatile uint32_t tpiu_reserved_35;              /**<  */
    volatile uint32_t tpiu_reserved_36;              /**<  */
    volatile uint32_t tpiu_reserved_37;              /**<  */
    volatile uint32_t tpiu_reserved_38;              /**<  */
    volatile uint32_t tpiu_reserved_39;              /**<  */
    volatile uint32_t tpiu_reserved_40;              /**<  */
    volatile uint32_t tpiu_reserved_41;              /**<  */
    volatile uint32_t tpiu_reserved_42;              /**<  */
    volatile uint32_t tpiu_reserved_43;              /**<  */
    volatile uint32_t tpiu_reserved_44;              /**<  */
    volatile uint32_t tpiu_reserved_45;              /**<  */
    volatile uint32_t tpiu_reserved_46;              /**<  */
    volatile uint32_t tpiu_reserved_47;              /**<  */
    volatile uint32_t tpiu_reserved_48;              /**<  */
    volatile uint32_t tpiu_reserved_49;              /**<  */
    volatile uint32_t tpiu_reserved_50;              /**<  */
    volatile uint32_t tpiu_reserved_51;              /**<  */
    volatile uint32_t tpiu_reserved_52;              /**<  */
    volatile uint32_t tpiu_reserved_53;              /**<  */
    volatile uint32_t tpiu_reserved_54;              /**<  */
    volatile uint32_t tpiu_reserved_55;              /**<  */
    volatile uint32_t tpiu_reserved_56;              /**<  */
    volatile uint32_t tpiu_reserved_57;              /**<  */
    volatile uint32_t tpiu_reserved_58;              /**<  */
    volatile uint32_t tpiu_reserved_59;              /**<  */
    volatile uint32_t tpiu_reserved_60;              /**<  */
    volatile uint32_t tpiu_reserved_61;              /**<  */
    volatile uint32_t tpiu_reserved_62;              /**<  */
    volatile uint32_t tpiu_reserved_63;              /**<  */
    volatile uint32_t tpiu_reserved_64;              /**<  */
    volatile uint32_t tpiu_reserved_65;              /**<  */
    volatile uint32_t tpiu_reserved_66;              /**<  */
    volatile uint32_t tpiu_reserved_67;              /**<  */
    volatile uint32_t tpiu_reserved_68;              /**<  */
    volatile uint32_t tpiu_reserved_69;              /**<  */
    volatile uint32_t tpiu_reserved_70;              /**<  */
    volatile uint32_t tpiu_reserved_71;              /**<  */
    volatile uint32_t tpiu_reserved_72;              /**<  */
    volatile uint32_t tpiu_reserved_73;              /**<  */
    volatile uint32_t tpiu_reserved_74;              /**<  */
    volatile uint32_t tpiu_reserved_75;              /**<  */
    volatile uint32_t tpiu_reserved_76;              /**<  */
    volatile uint32_t tpiu_reserved_77;              /**<  */
    volatile uint32_t tpiu_reserved_78;              /**<  */
    volatile uint32_t tpiu_reserved_79;              /**<  */
    volatile uint32_t tpiu_reserved_80;              /**<  */
    volatile uint32_t tpiu_reserved_81;              /**<  */
    volatile uint32_t tpiu_reserved_82;              /**<  */
    volatile uint32_t tpiu_reserved_83;              /**<  */
    volatile uint32_t tpiu_reserved_84;              /**<  */
    volatile uint32_t tpiu_reserved_85;              /**<  */
    volatile uint32_t tpiu_reserved_86;              /**<  */
    volatile uint32_t tpiu_reserved_87;              /**<  */
    volatile uint32_t tpiu_reserved_88;              /**<  */
    volatile uint32_t tpiu_reserved_89;              /**<  */
    volatile uint32_t tpiu_reserved_90;              /**<  */
    volatile uint32_t tpiu_reserved_91;              /**<  */
    volatile uint32_t tpiu_reserved_92;              /**<  */
    volatile uint32_t tpiu_reserved_93;              /**<  */
    volatile uint32_t tpiu_reserved_94;              /**<  */
    volatile uint32_t tpiu_reserved_95;              /**<  */
    volatile uint32_t tpiu_reserved_96;              /**<  */
    volatile uint32_t tpiu_reserved_97;              /**<  */
    volatile uint32_t tpiu_reserved_98;              /**<  */
    volatile uint32_t tpiu_reserved_99;              /**<  */
    volatile uint32_t tpiu_reserved_100;             /**<  */
    volatile uint32_t tpiu_reserved_101;             /**<  */
    volatile uint32_t tpiu_reserved_102;             /**<  */
    volatile uint32_t tpiu_reserved_103;             /**<  */
    volatile uint32_t tpiu_reserved_104;             /**<  */
    volatile uint32_t tpiu_reserved_105;             /**<  */
    volatile uint32_t tpiu_reserved_106;             /**<  */
    volatile uint32_t tpiu_reserved_107;             /**<  */
    volatile uint32_t tpiu_reserved_108;             /**<  */
    volatile uint32_t tpiu_reserved_109;             /**<  */
    volatile uint32_t tpiu_reserved_110;             /**<  */
    volatile uint32_t tpiu_reserved_111;             /**<  */
    volatile uint32_t tpiu_reserved_112;             /**<  */
    volatile uint32_t tpiu_reserved_113;             /**<  */
    volatile uint32_t tpiu_reserved_114;             /**<  */
    volatile uint32_t tpiu_reserved_115;             /**<  */
    volatile uint32_t tpiu_reserved_116;             /**<  */
    volatile uint32_t tpiu_reserved_117;             /**<  */
    volatile uint32_t tpiu_reserved_118;             /**<  */
    volatile uint32_t tpiu_reserved_119;             /**<  */
    volatile uint32_t tpiu_reserved_120;             /**<  */
    volatile uint32_t tpiu_reserved_121;             /**<  */
    volatile uint32_t tpiu_reserved_122;             /**<  */
    volatile uint32_t tpiu_reserved_123;             /**<  */
    volatile uint32_t tpiu_reserved_124;             /**<  */
    volatile uint32_t tpiu_reserved_125;             /**<  */
    volatile uint32_t tpiu_reserved_126;             /**<  */
    volatile uint32_t tpiu_reserved_127;             /**<  */
    volatile uint32_t tpiu_reserved_128;             /**<  */
    volatile uint32_t tpiu_reserved_129;             /**<  */
    volatile uint32_t tpiu_reserved_130;             /**<  */
    volatile uint32_t tpiu_reserved_131;             /**<  */
    volatile uint32_t tpiu_reserved_132;             /**<  */
    volatile uint32_t tpiu_reserved_133;             /**<  */
    volatile uint32_t tpiu_reserved_134;             /**<  */
    volatile uint32_t tpiu_reserved_135;             /**<  */
    volatile uint32_t tpiu_reserved_136;             /**<  */
    volatile uint32_t tpiu_reserved_137;             /**<  */
    volatile uint32_t tpiu_reserved_138;             /**<  */
    volatile uint32_t tpiu_reserved_139;             /**<  */
    volatile uint32_t tpiu_reserved_140;             /**<  */
    volatile uint32_t tpiu_reserved_141;             /**<  */
    volatile uint32_t tpiu_reserved_142;             /**<  */
    volatile uint32_t tpiu_reserved_143;             /**<  */
    volatile uint32_t tpiu_reserved_144;             /**<  */
    volatile uint32_t tpiu_reserved_145;             /**<  */
    volatile uint32_t tpiu_reserved_146;             /**<  */
    volatile uint32_t tpiu_reserved_147;             /**<  */
    volatile uint32_t tpiu_reserved_148;             /**<  */
    volatile uint32_t tpiu_reserved_149;             /**<  */
    volatile uint32_t tpiu_reserved_150;             /**<  */
    volatile uint32_t tpiu_reserved_151;             /**<  */
    volatile uint32_t tpiu_reserved_152;             /**<  */
    volatile uint32_t tpiu_reserved_153;             /**<  */
    volatile uint32_t tpiu_reserved_154;             /**<  */
    volatile uint32_t tpiu_reserved_155;             /**<  */
    volatile uint32_t tpiu_reserved_156;             /**<  */
    volatile uint32_t tpiu_reserved_157;             /**<  */
    volatile uint32_t tpiu_reserved_158;             /**<  */
    volatile uint32_t tpiu_reserved_159;             /**<  */
    volatile uint32_t tpiu_reserved_160;             /**<  */
    volatile uint32_t tpiu_reserved_161;             /**<  */
    volatile uint32_t tpiu_reserved_162;             /**<  */
    volatile uint32_t tpiu_reserved_163;             /**<  */
    volatile uint32_t tpiu_reserved_164;             /**<  */
    volatile uint32_t tpiu_reserved_165;             /**<  */
    volatile uint32_t tpiu_reserved_166;             /**<  */
    volatile uint32_t tpiu_reserved_167;             /**<  */
    volatile uint32_t tpiu_reserved_168;             /**<  */
    volatile uint32_t tpiu_reserved_169;             /**<  */
    volatile uint32_t tpiu_reserved_170;             /**<  */
    volatile uint32_t tpiu_reserved_171;             /**<  */
    volatile uint32_t tpiu_reserved_172;             /**<  */
    volatile uint32_t tpiu_reserved_173;             /**<  */
    volatile uint32_t tpiu_reserved_174;             /**<  */
    volatile uint32_t tpiu_reserved_175;             /**<  */
    volatile uint32_t tpiu_reserved_176;             /**<  */
    volatile uint32_t tpiu_reserved_177;             /**<  */
    volatile uint32_t tpiu_reserved_178;             /**<  */
    volatile uint32_t tpiu_reserved_179;             /**<  */
    volatile uint32_t tpiu_reserved_180;             /**<  */
    volatile uint32_t tpiu_reserved_181;             /**<  */
    volatile uint32_t tpiu_reserved_182;             /**<  */
    volatile uint32_t tpiu_reserved_183;             /**<  */
    volatile uint32_t tpiu_reserved_184;             /**<  */
    volatile uint32_t tpiu_reserved_185;             /**<  */
    volatile uint32_t tpiu_format_status;            /**< 0x300 */
    volatile uint32_t tpiu_format_ctl;               /**< 0x304 */
    volatile uint32_t tpiu_sync_counter;             /**< 0x308 */
} tpiu_type_t;

#define TPIU_INSTANCE_COUNT                     (1U)

/** TPIU - Peripheral instance base addresses */

/** Peripheral TPIU base address */
#define TPIU_BASE                              (TPIU_BASEADDR)
/** Peripheral TPIU base pointer */
#define TPIU                                  ((tpiu_type_t *)TPIU_BASE)
#define TPIU_SUPPORT_SIZE_SUP_PORT_SIZE_Pos                          0U
#define TPIU_SUPPORT_SIZE_SUP_PORT_SIZE_Msk                          (0xffffffffUL << TPIU_SUPPORT_SIZE_SUP_PORT_SIZE_Pos)

#define TPIU_CURPORT_SIZE_CUR_PORT_SIZE_Pos                          0U
#define TPIU_CURPORT_SIZE_CUR_PORT_SIZE_Msk                          (0xffffffffUL << TPIU_CURPORT_SIZE_CUR_PORT_SIZE_Pos)

#define TPIU_BASE_ADDRESS_BASE_ADDRESS_Pos                           0U
#define TPIU_BASE_ADDRESS_BASE_ADDRESS_Msk                           (0xffffffffUL << TPIU_BASE_ADDRESS_BASE_ADDRESS_Pos)

#define TPIU_MAX_TIME_MAX_TIME_Pos                                   0U
#define TPIU_MAX_TIME_MAX_TIME_Msk                                   (0xffffUL << TPIU_MAX_TIME_MAX_TIME_Pos)
#define TPIU_MAX_TIME_RESERVED_Pos                                   16U
#define TPIU_MAX_TIME_RESERVED_Msk                                   (0xffffUL << TPIU_MAX_TIME_RESERVED_Pos)

#define TPIU_RAM_WRITE_POINTER_RAM_WRITE_POINTER_Pos                 0U
#define TPIU_RAM_WRITE_POINTER_RAM_WRITE_POINTER_Msk                 (0xffffffffUL << TPIU_RAM_WRITE_POINTER_RAM_WRITE_POINTER_Pos)

#define TPIU_CONTROL_REG_TRACE_PATH_SEL_Pos                          0U
#define TPIU_CONTROL_REG_TRACE_PATH_SEL_Msk                          (0x1UL << TPIU_CONTROL_REG_TRACE_PATH_SEL_Pos)
#define TPIU_CONTROL_REG_RESERVED_Pos                                1U
#define TPIU_CONTROL_REG_RESERVED_Msk                                (0x7fffffffUL << TPIU_CONTROL_REG_RESERVED_Pos)

#define TPIU_FORMAT_STATUS_FLUSH_STATUS_Pos                          0U
#define TPIU_FORMAT_STATUS_FLUSH_STATUS_Msk                          (0x1UL << TPIU_FORMAT_STATUS_FLUSH_STATUS_Pos)
#define TPIU_FORMAT_STATUS_RESERVED_Pos                              1U
#define TPIU_FORMAT_STATUS_RESERVED_Msk                              (0x7fffffffUL << TPIU_FORMAT_STATUS_RESERVED_Pos)

#define TPIU_FORMAT_CTL_ENABLE_FORMATTER_Pos                         0U
#define TPIU_FORMAT_CTL_ENABLE_FORMATTER_Msk                         (0x1UL << TPIU_FORMAT_CTL_ENABLE_FORMATTER_Pos)
#define TPIU_FORMAT_CTL_CONTINUOUS_MODE_Pos                          1U
#define TPIU_FORMAT_CTL_CONTINUOUS_MODE_Msk                          (0x1UL << TPIU_FORMAT_CTL_CONTINUOUS_MODE_Pos)
#define TPIU_FORMAT_CTL_RESERVED0_Pos                                2U
#define TPIU_FORMAT_CTL_RESERVED0_Msk                                (0xfUL << TPIU_FORMAT_CTL_RESERVED0_Pos)

#define TPIU_FORMAT_CTL_FLUSH_ON_MAN_Pos                             6U
#define TPIU_FORMAT_CTL_FLUSH_ON_MAN_Msk                             (0x1UL << TPIU_FORMAT_CTL_FLUSH_ON_MAN_Pos)

#define TPIU_FORMAT_CTL_RESERVED1_Pos                                7U
#define TPIU_FORMAT_CTL_RESERVED1_Msk                                (0x1UL << TPIU_FORMAT_CTL_FLUSH_RESERVED1_Pos)

#define TPIU_FORMAT_CTL_STOP_ON_MAN_Pos                              8U
#define TPIU_FORMAT_CTL_STOP_ON_MAN_Msk                              (0x1UL << TPIU_FORMAT_CTL_STOP_ON_MAN_Pos)
#define TPIU_FORMAT_CTL_RESERVED2_Pos                                9U
#define TPIU_FORMAT_CTL_RESERVED2_Msk                                (0x7fffffUL << TPIU_FORMAT_CTL_RESERVED2_Pos)

#define TPIU_SYNC_COUNTER_SYNC_COUNTER_Pos                           0U
#define TPIU_SYNC_COUNTER_SYNC_COUNTER_Msk                           (0xfffUL << TPIU_SYNC_COUNTER_SYNC_COUNTER_Pos)

#define TPIU_SYNC_COUNTER_RESERVED_Pos                               12U
#define TPIU_SYNC_COUNTER_RESERVED_Msk                               (0xfffffUL << TPIU_SYNC_COUNTER_RESERVED_Pos)
// clang-format on

#endif // __TPIU_REGISTER_H__
