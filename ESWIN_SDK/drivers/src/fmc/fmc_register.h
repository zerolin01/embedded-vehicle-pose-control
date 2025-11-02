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
 * @file fmc_register.h
 * @brief FMC driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __FMC_REGISTER_H__
#define __FMC_REGISTER_H__

#include "emps_platform.h"
#include "status.h"

#define FPGA_PLT (0)

// clang-format off
/**
 * @brief FMC ctrl configuration structure
 */
typedef union {
    struct {
        volatile uint32_t pflash_wr_protect_en:1;        /**< bit 0        access: RW       default: 0x1       */
        volatile uint32_t dflash_wr_protect_en:1;        /**< bit 1        access: RW       default: 0x1       */
        volatile uint32_t pflash_ecc_en:1;               /**< bit 2        access: RW       default: 0x1       */
        volatile uint32_t dflash_ecc_en:1;               /**< bit 3        access: RW       default: 0x1       */
        volatile uint32_t sram_ecc_en:1;                 /**< bit 4        access: RW       default: 0x1       */
        volatile uint32_t rsv1:1;                        /**< bit 5        access: RW       default: 0x0       */
        volatile uint32_t dflash_prefetch_en:1;          /**< bit 6        access: RW       default: 0x1       */
        volatile uint32_t pflash_prefetch_en:1;          /**< bit 7        access: RW       default: 0x1       */
        volatile uint32_t ahb_bus_error_msk:1;           /**< bit 8        access: RW       default: 0x1       */
        volatile uint32_t apb_bus_error_msk:1;           /**< bit 9        access: RW       default: 0x1       */
        volatile uint32_t fmc_clock_mode:2;              /**< bit 10..11   access: RW       default: 0x1       */
        volatile uint32_t rsv0:4;                        /**< bit 12..15   access: RO       default: 0x0       */
        volatile uint32_t ecc_1bit_err_thod:16;          /**< bit 16..31   access: RW       default: 0xff      */
    } b;
    volatile uint32_t w;
} fmc_ctrl_cfg_type_t;

/**
 * @brief FMC cg en configuration structure
 */
typedef union {
    struct {
        volatile uint32_t auto_cg_en:1;                  /**< bit 0        access: RW       default: 0x0       */
        volatile uint32_t rsv:31;                        /**< bit 1..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_cg_en_cfg_type_t;

/**
 * @brief FMC status structure
 */
typedef union {
    struct {
        volatile uint32_t fmc_status:1;                  /**< bit 0        access: RO       default: 0x0       */
        volatile uint32_t rsv:31;                        /**< bit 1..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_status_type_t;

/** reserved FMC_RESERVED_0_Type */

/** reserved FMC_RESERVED_1_Type */

/** reserved FMC_RESERVED_2_Type */

/** reserved FMC_RESERVED_3_Type */

/** reserved FMC_RESERVED_4_Type */

/**
 * @brief FMC pflash read cmd configuration structure
 */
typedef union {
    struct {
        volatile uint32_t read_mode:2;                   /**< bit 0..1     access: RW       default: 0x0       */
        volatile uint32_t sector_sel:2;                  /**< bit 2..3     access: RW       default: 0x0       */
        volatile uint32_t flash_addr:20;                 /**< bit 4..23    access: RW       default: 0x0       */
        volatile uint32_t redund_sel:1;                  /**< bit 24       access: RW       default: 0x0       */
        volatile uint32_t rsv:7;                         /**< bit 25..31   access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_pflash_read_cmd_cfg_type_t;

/**
 * @brief FMC pflash read cmd status structure
 */
typedef union {
    struct {
        volatile uint32_t cmd_stat:1;                    /**< bit 0        access: RO       default: 0x1       */
        volatile uint32_t rsv1:3;                        /**< bit 1..3     access: RO       default: 0x0       */
        volatile uint32_t flash_data:8;                  /**< bit 4..11    access: RO       default: 0x0       */
        volatile uint32_t rsv0:20;                       /**< bit 12..31   access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_pflash_read_cmd_status_type_t;

/**
 * @brief FMC pflash read cmd data0 structure
 */
typedef union {
    struct {
        volatile uint32_t flash_data:32;                 /**< bit 0..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_pflash_read_cmd_data0_type_t;

/**
 * @brief FMC pflash read cmd data1 structure
 */
typedef union {
    struct {
        volatile uint32_t flash_data:32;                 /**< bit 0..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_pflash_read_cmd_data1_type_t;

/** reserved FMC_RESERVED_5_Type */

/** reserved FMC_RESERVED_6_Type */

/** reserved FMC_RESERVED_7_Type */

/** reserved FMC_RESERVED_8_Type */

/**
 * @brief FMC pflash program cmd configuration structure
 */
typedef union {
    struct {
        volatile uint32_t sector_sel:2;                  /**< bit 0..1     access: RW       default: 0x0       */
        volatile uint32_t rsv1:2;                        /**< bit 2..3     access: RO       default: 0x0       */
        volatile uint32_t flash_addr:20;                 /**< bit 4..23    access: RW       default: 0x0       */
        volatile uint32_t redund_sel:1;                  /**< bit 24       access: RW       default: 0x0       */
        volatile uint32_t rsv0:7;                        /**< bit 25..31   access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_pflash_program_cmd_cfg_type_t;

/**
 * @brief FMC pflash program cmd status structure
 */
typedef union {
    struct {
        volatile uint32_t cmd_stat:1;                    /**< bit 0        access: RO       default: 0x1       */
        volatile uint32_t err_stat:1;                    /**< bit 1        access: RO       default: 0x0       */
        volatile uint32_t rsv:30;                        /**< bit 2..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_pflash_program_cmd_status_type_t;

/**
 * @brief FMC pflash program cmd data0 structure
 */
typedef union {
    struct {
        volatile uint32_t flash_data:32;                 /**< bit 0..31    access: RW       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_pflash_program_cmd_data0_type_t;

/**
 * @brief FMC pflash program cmd data1 structure
 */
typedef union {
    struct {
        volatile uint32_t flash_data:32;                 /**< bit 0..31    access: RW       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_pflash_program_cmd_data1_type_t;

/** reserved FMC_RESERVED_9_Type */

/** reserved FMC_RESERVED_10_Type */

/** reserved FMC_RESERVED_11_Type */

/** reserved FMC_RESERVED_12_Type */

/**
 * @brief FMC pflash erase cmd configuration structure
 */
typedef union {
    struct {
        volatile uint32_t erase_mode:2;                  /**< bit 0..1     access: RW       default: 0x0       */
        volatile uint32_t sector_sel:2;                  /**< bit 2..3     access: RW       default: 0x0       */
        volatile uint32_t flash_addr:20;                 /**< bit 4..23    access: RW       default: 0x0       */
        volatile uint32_t redund_sel:1;                  /**< bit 24       access: RW       default: 0x0       */
        volatile uint32_t retry:2;                       /**< bit 25..26   access: RW       default: 0x0       */
        volatile uint32_t rsv:5;                         /**< bit 27..31   access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_pflash_erase_cmd_cfg_type_t;

/**
 * @brief FMC pflash erase cmd status structure
 */
typedef union {
    struct {
        volatile uint32_t cmd_stat:1;                    /**< bit 0        access: RO       default: 0x1       */
        volatile uint32_t err_stat:1;                    /**< bit 1        access: RO       default: 0x0       */
        volatile uint32_t rsv:30;                        /**< bit 2..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_pflash_erase_cmd_status_type_t;

/** reserved FMC_RESERVED_13_Type */

/** reserved FMC_RESERVED_14_Type */

/**
 * @brief FMC pflash power mode configuration structure
 */
typedef union {
    struct {
        volatile uint32_t power_mode:1;                  /**< bit 0        access: RW       default: 0x0       */
        volatile uint32_t rsv:31;                        /**< bit 1..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_pflash_power_mode_cfg_type_t;

/** reserved FMC_RESERVED_15_Type */

/** reserved FMC_RESERVED_16_Type */

/** reserved FMC_RESERVED_17_Type */

/**
 * @brief FMC dflash read cmd configuration structure
 */
typedef union {
    struct {
        volatile uint32_t read_mode:2;                   /**< bit 0..1     access: RW       default: 0x0       */
        volatile uint32_t sector_sel:2;                  /**< bit 2..3     access: RW       default: 0x0       */
        volatile uint32_t flash_addr:20;                 /**< bit 4..23    access: RW       default: 0x0       */
        volatile uint32_t redund_sel:1;                  /**< bit 24       access: RW       default: 0x0       */
        volatile uint32_t rsv:7;                         /**< bit 25..31   access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_dflash_read_cmd_cfg_type_t;

/**
 * @brief FMC dflash read cmd status structure
 */
typedef union {
    struct {
        volatile uint32_t cmd_stat:1;                    /**< bit 0        access: RO       default: 0x1       */
        volatile uint32_t rsv1:3;                        /**< bit 1..3     access: RO       default: 0x0       */
        volatile uint32_t flash_data:8;                  /**< bit 4..11    access: RO       default: 0x0       */
        volatile uint32_t rsv0:20;                       /**< bit 12..31   access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_dflash_read_cmd_status_type_t;

/**
 * @brief FMC dflash read cmd data0 structure
 */
typedef union {
    struct {
        volatile uint32_t flash_data:32;                 /**< bit 0..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_dflash_read_cmd_data0_type_t;

/**
 * @brief FMC dflash read cmd data1 structure
 */
typedef union {
    struct {
        volatile uint32_t flash_data:32;                 /**< bit 0..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_dflash_read_cmd_data1_type_t;

/** reserved FMC_RESERVED_18_Type */

/** reserved FMC_RESERVED_19_Type */

/** reserved FMC_RESERVED_20_Type */

/** reserved FMC_RESERVED_21_Type */

/**
 * @brief FMC dflash program cmd configuration structure
 */
typedef union {
    struct {
        volatile uint32_t sector_sel:2;                  /**< bit 0..1     access: RW       default: 0x0       */
        volatile uint32_t rsv1:2;                        /**< bit 2..3     access: RO       default: 0x0       */
        volatile uint32_t flash_addr:20;                 /**< bit 4..23    access: RW       default: 0x0       */
        volatile uint32_t redund_sel:1;                  /**< bit 24       access: RW       default: 0x0       */
        volatile uint32_t rsv0:7;                        /**< bit 25..31   access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_dflash_program_cmd_cfg_type_t;

/**
 * @brief FMC dflash program cmd status structure
 */
typedef union {
    struct {
        volatile uint32_t cmd_stat:1;                    /**< bit 0        access: RO       default: 0x1       */
        volatile uint32_t err_stat:1;                    /**< bit 1        access: RO       default: 0x0       */
        volatile uint32_t rsv:30;                        /**< bit 2..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_dflash_program_cmd_status_type_t;

/**
 * @brief FMC dflash program cmd data0 structure
 */
typedef union {
    struct {
        volatile uint32_t flash_data:32;                 /**< bit 0..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_dflash_program_cmd_data0_type_t;

/**
 * @brief FMC dflash program cmd data1 structure
 */
typedef union {
    struct {
        volatile uint32_t flash_data:32;                 /**< bit 0..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_dflash_program_cmd_data1_type_t;

/** reserved FMC_RESERVED_22_Type */

/** reserved FMC_RESERVED_23_Type */

/** reserved FMC_RESERVED_24_Type */

/** reserved FMC_RESERVED_25_Type */

/**
 * @brief FMC dflash erase cmd configuration structure
 */
typedef union {
    struct {
        volatile uint32_t erase_mode:2;                  /**< bit 0..1     access: RW       default: 0x0       */
        volatile uint32_t sector_sel:2;                  /**< bit 2..3     access: RW       default: 0x0       */
        volatile uint32_t flash_addr:20;                 /**< bit 4..23    access: RW       default: 0x0       */
        volatile uint32_t redund_sel:1;                  /**< bit 24       access: RW       default: 0x0       */
        volatile uint32_t retry:2;                       /**< bit 25..26   access: RW       default: 0x0       */
        volatile uint32_t rsv:5;                         /**< bit 27..31   access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_dflash_erase_cmd_cfg_type_t;

/**
 * @brief FMC dflash erase cmd status structure
 */
typedef union {
    struct {
        volatile uint32_t cmd_stat:1;                    /**< bit 0        access: RO       default: 0x1       */
        volatile uint32_t err_stat:1;                    /**< bit 1        access: RO       default: 0x0       */
        volatile uint32_t rsv:30;                        /**< bit 2..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_dflash_erase_cmd_status_type_t;

/** reserved FMC_RESERVED_26_Type */

/** reserved FMC_RESERVED_27_Type */

/**
 * @brief FMC dflash power mode configuration structure
 */
typedef union {
    struct {
        volatile uint32_t power_mode:1;                  /**< bit 0        access: RW       default: 0x0       */
        volatile uint32_t rsv:31;                        /**< bit 1..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_dflash_power_mode_cfg_type_t;

/** reserved FMC_RESERVED_28_Type */

/** reserved FMC_RESERVED_29_Type */

/** reserved FMC_RESERVED_30_Type */

/** reserved FMC_RESERVED_31_Type */

/** reserved FMC_RESERVED_32_Type */

/** reserved FMC_RESERVED_33_Type */

/** reserved FMC_RESERVED_34_Type */

/** reserved FMC_RESERVED_35_Type */

/** reserved FMC_RESERVED_36_Type */

/** reserved FMC_RESERVED_37_Type */

/** reserved FMC_RESERVED_38_Type */

/**
 * @brief FMC pflash wr protect bitmap structure
 */
typedef union {
    struct {
        volatile uint32_t wr_protect_bitmap:32;          /**< bit 0..31    access: RW       default: 0xffffffff  */
    } b;
    volatile uint32_t w;
} fmc_pflash_wr_protect_bitmap_type_t;

/**
 * @brief FMC dflash wr protect bitmap structure
 */
typedef union {
    struct {
        volatile uint32_t wr_protect_bitmap:8;           /**< bit 0..7     access: RW       default: 0xff      */
        volatile uint32_t rsv:24;                        /**< bit 8..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_dflash_wr_protect_bitmap_type_t;

/** reserved FMC_RESERVED_39_Type */

/** reserved FMC_RESERVED_40_Type */

/** reserved FMC_RESERVED_41_Type */

/** reserved FMC_RESERVED_42_Type */

/** reserved FMC_RESERVED_43_Type */

/** reserved FMC_RESERVED_44_Type */

/**
 * @brief FMC normal int raw structure
 */
typedef union {
    struct {
        volatile uint32_t pflash_erase_done:1;           /**< bit 0        access: WC       default: 0x0       */
        volatile uint32_t dflash_erase_done:1;           /**< bit 1        access: WC       default: 0x0       */
        volatile uint32_t pflash_program_done:1;         /**< bit 2        access: WC       default: 0x0       */
        volatile uint32_t dflash_program_done:1;         /**< bit 3        access: WC       default: 0x0       */
        volatile uint32_t eeprom_copy_done:1;            /**< bit 4        access: WC       default: 0x0       */
        volatile uint32_t rsv:27;                        /**< bit 5..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_normal_int_raw_type_t;

/**
 * @brief FMC normal int msk structure
 */
typedef union {
    struct {
        volatile uint32_t pflash_erase_done:1;           /**< bit 0        access: RW       default: 0x1       */
        volatile uint32_t dflash_erase_done:1;           /**< bit 1        access: RW       default: 0x1       */
        volatile uint32_t pflash_program_done:1;         /**< bit 2        access: RW       default: 0x1       */
        volatile uint32_t dflash_program_done:1;         /**< bit 3        access: RW       default: 0x1       */
        volatile uint32_t eeprom_copy_done:1;            /**< bit 4        access: RW       default: 0x1       */
        volatile uint32_t rsv:27;                        /**< bit 5..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_normal_int_msk_type_t;

/**
 * @brief FMC normal int status structure
 */
typedef union {
    struct {
        volatile uint32_t pflash_erase_done:1;           /**< bit 0        access: RO       default: 0x0       */
        volatile uint32_t dflash_erase_done:1;           /**< bit 1        access: RO       default: 0x0       */
        volatile uint32_t pflash_program_done:1;         /**< bit 2        access: RO       default: 0x0       */
        volatile uint32_t dflash_program_done:1;         /**< bit 3        access: RO       default: 0x0       */
        volatile uint32_t eeprom_copy_done:1;            /**< bit 4        access: RO       default: 0x0       */
        volatile uint32_t rsv:27;                        /**< bit 5..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_normal_int_stat_type_t;

/** reserved FMC_RESERVED_45_Type */

/** reserved FMC_RESERVED_46_Type */

/** reserved FMC_RESERVED_47_Type */

/** reserved FMC_RESERVED_48_Type */

/** reserved FMC_RESERVED_49_Type */

/**
 * @brief FMC warning int raw structure
 */
typedef union {
    struct {
        volatile uint32_t pflash_1bit_ecc:1;             /**< bit 0        access: WC       default: 0x0       */
        volatile uint32_t pflash_multi_1bit_ecc:1;       /**< bit 1        access: WC       default: 0x0       */
        volatile uint32_t dflash_1bit_ecc:1;             /**< bit 2        access: WC       default: 0x0       */
        volatile uint32_t dflash_multi_1bit_ecc:1;       /**< bit 3        access: WC       default: 0x0       */
        volatile uint32_t pflash_wr_protect_err:1;       /**< bit 4        access: WC       default: 0x0       */
        volatile uint32_t dflash_wr_protect_err:1;       /**< bit 5        access: WC       default: 0x0       */
        volatile uint32_t sram_1bit_ecc:1;               /**< bit 6        access: WC       default: 0x0       */
        volatile uint32_t rsv:25;                        /**< bit 7..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_warning_int_raw_type_t;

/**
 * @brief FMC warning int msk structure
 */
typedef union {
    struct {
        volatile uint32_t pflash_1bit_ecc:1;             /**< bit 0        access: RW       default: 0x0       */
        volatile uint32_t pflash_multi_1bit_ecc:1;       /**< bit 1        access: RW       default: 0x0       */
        volatile uint32_t dflash_1bit_ecc:1;             /**< bit 2        access: RW       default: 0x0       */
        volatile uint32_t dflash_multi_1bit_ecc:1;       /**< bit 3        access: RW       default: 0x0       */
        volatile uint32_t pflash_wr_protect_err:1;       /**< bit 4        access: RW       default: 0x0       */
        volatile uint32_t dflash_wr_protect_err:1;       /**< bit 5        access: RW       default: 0x0       */
        volatile uint32_t sram_1bit_ecc:1;               /**< bit 6        access: WC       default: 0x0       */
        volatile uint32_t rsv:25;                        /**< bit 7..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_warning_int_msk_type_t;

/**
 * @brief FMC warning int status structure
 */
typedef union {
    struct {
        volatile uint32_t pflash_1bit_ecc:1;             /**< bit 0        access: RO       default: 0x0       */
        volatile uint32_t pflash_multi_1bit_ecc:1;       /**< bit 1        access: RO       default: 0x0       */
        volatile uint32_t dflash_1bit_ecc:1;             /**< bit 2        access: RO       default: 0x0       */
        volatile uint32_t dflash_multi_1bit_ecc:1;       /**< bit 3        access: RO       default: 0x0       */
        volatile uint32_t pflash_wr_protect_err:1;       /**< bit 4        access: RO       default: 0x0       */
        volatile uint32_t dflash_wr_protect_err:1;       /**< bit 5        access: RO       default: 0x0       */
        volatile uint32_t sram_1bit_ecc:1;               /**< bit 6        access: WC       default: 0x0       */
        volatile uint32_t rsv:25;                        /**< bit 7..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_warning_int_stat_type_t;

/** reserved FMC_RESERVED_50_Type */

/** reserved FMC_RESERVED_51_Type */

/** reserved FMC_RESERVED_52_Type */

/** reserved FMC_RESERVED_53_Type */

/** reserved FMC_RESERVED_54_Type */

/**
 * @brief FMC warning pflash 1bit ecc addr structure
 */
typedef union {
    struct {
        volatile uint32_t sector_sel:2;                  /**< bit 0..1     access: RO       default: 0x0       */
        volatile uint32_t redund_sel:1;                  /**< bit 2        access: RO       default: 0x0       */
        volatile uint32_t rsv1:1;                        /**< bit 3        access: RO       default: 0x0       */
        volatile uint32_t flash_addr:20;                 /**< bit 4..23    access: RO       default: 0x0       */
        volatile uint32_t rsv0:8;                        /**< bit 24..31   access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_warning_pflash_1bit_ecc_addr_type_t;

/**
 * @brief FMC warning pflash 1bit ecc cnt structure
 */
typedef union {
    struct {
        volatile uint32_t _1bit_ecc_cnt:16;              /**< bit 0..15    access: RW1C     default: 0x0       */
        volatile uint32_t rsv:16;                        /**< bit 16..31   access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_warning_pflash_1bit_ecc_cnt_type_t;

/** reserved FMC_RESERVED_55_Type */

/** reserved FMC_RESERVED_56_Type */

/**
 * @brief FMC warning dflash 1bit ecc addr structure
 */
typedef union {
    struct {
        volatile uint32_t sector_sel:2;                  /**< bit 0..1     access: RO       default: 0x0       */
        volatile uint32_t redund_sel:1;                  /**< bit 2        access: RO       default: 0x0       */
        volatile uint32_t rsv1:1;                        /**< bit 3        access: RO       default: 0x0       */
        volatile uint32_t flash_addr:20;                 /**< bit 4..23    access: RO       default: 0x0       */
        volatile uint32_t rsv0:8;                        /**< bit 24..31   access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_warning_dflash_1bit_ecc_addr_type_t;

/**
 * @brief FMC warning dflash 1bit ecc cnt structure
 */
typedef union {
    struct {
        volatile uint32_t _1bit_ecc_cnt:16;              /**< bit 0..15    access: RW1C     default: 0x0       */
        volatile uint32_t rsv:16;                        /**< bit 16..31   access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_warning_dflash_1bit_ecc_cnt_type_t;

/** reserved FMC_RESERVED_57_Type */

/** reserved FMC_RESERVED_58_Type */

/**
 * @brief FMC warning sram 1bit ecc addr structure
 */
typedef union {
    struct {
        volatile uint32_t flash_addr:17;                 /**< bit 0..16    access: RO       default: 0x0       */
        volatile uint32_t rsv:15;                        /**< bit 17..31   access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_warning_sram_1bit_ecc_addr_type_t;

/**
 * @brief FMC warning sram 1bit ecc cnt structure
 */
typedef union {
    struct {
        volatile uint32_t _1bit_ecc_cnt:16;              /**< bit 0..15    access: RW1C     default: 0x0       */
        volatile uint32_t rsv:16;                        /**< bit 16..31   access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_warning_sram_1bit_ecc_cnt_type_t;

/** reserved FMC_RESERVED_59_Type */

/** reserved FMC_RESERVED_60_Type */

/**
 * @brief FMC warning pflash wr protect status structure
 */
typedef union {
    struct {
        volatile uint32_t sector_sel:2;                  /**< bit 0..1     access: RO       default: 0x0       */
        volatile uint32_t redund_sel:1;                  /**< bit 2        access: RO       default: 0x0       */
        volatile uint32_t ahb_sel:1;                     /**< bit 3        access: RO       default: 0x0       */
        volatile uint32_t flash_addr:20;                 /**< bit 4..23    access: RO       default: 0x0       */
        volatile uint32_t erase_sel:1;                   /**< bit 24       access: RO       default: 0x0       */
        volatile uint32_t rsv:7;                         /**< bit 25..31   access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_warning_pflash_wr_protect_stat_type_t;

/**
 * @brief FMC warning dflash wr protect status structure
 */
typedef union {
    struct {
        volatile uint32_t sector_sel:2;                  /**< bit 0..1     access: RO       default: 0x0       */
        volatile uint32_t redund_sel:1;                  /**< bit 2        access: RO       default: 0x0       */
        volatile uint32_t ahb_sel:1;                     /**< bit 3        access: RO       default: 0x0       */
        volatile uint32_t flash_addr:20;                 /**< bit 4..23    access: RO       default: 0x0       */
        volatile uint32_t erase_sel:1;                   /**< bit 24       access: RO       default: 0x0       */
        volatile uint32_t rsv:7;                         /**< bit 25..31   access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_warning_dflash_wr_protect_stat_type_t;

/** reserved FMC_RESERVED_61_Type */

/** reserved FMC_RESERVED_62_Type */

/**
 * @brief FMC error int raw structure
 */
typedef union {
    struct {
        volatile uint32_t pflash_2bit_ecc:1;             /**< bit 0        access: WC       default: 0x0       */
        volatile uint32_t dflash_2bit_ecc:1;             /**< bit 1        access: WC       default: 0x0       */
        volatile uint32_t sram_2bit_ecc:1;               /**< bit 2        access: WC       default: 0x0       */
        volatile uint32_t otp_cmp_err:1;                 /**< bit 3        access: WC       default: 0x0       */
        volatile uint32_t otp_ecc_err:1;                 /**< bit 4        access: WC       default: 0x0       */
        volatile uint32_t rsv:27;                        /**< bit 5..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_error_int_raw_type_t;

/** reserved FMC_RESERVED_63_Type */

/** reserved FMC_RESERVED_64_Type */

/** reserved FMC_RESERVED_65_Type */

/**
 * @brief FMC error pflash 2bit ecc status structure
 */
typedef union {
    struct {
        volatile uint32_t sector_sel:2;                  /**< bit 0..1     access: RO       default: 0x0       */
        volatile uint32_t redund_sel:1;                  /**< bit 2        access: RO       default: 0x0       */
        volatile uint32_t rsv1:1;                        /**< bit 3        access: RO       default: 0x0       */
        volatile uint32_t flash_addr:20;                 /**< bit 4..23    access: RO       default: 0x0       */
        volatile uint32_t reserved0:4;                   /**< bit 24..27   access: RW       default: 0x0       */
        volatile uint32_t rsv0:4;                        /**< bit 28..31   access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_error_pflash_2bit_ecc_stat_type_t;

/**
 * @brief FMC error dflash 2bit ecc status structure
 */
typedef union {
    struct {
        volatile uint32_t sector_sel:2;                  /**< bit 0..1     access: RO       default: 0x0       */
        volatile uint32_t redund_sel:1;                  /**< bit 2        access: RO       default: 0x0       */
        volatile uint32_t rsv1:1;                        /**< bit 3        access: RO       default: 0x0       */
        volatile uint32_t flash_addr:20;                 /**< bit 4..23    access: RO       default: 0x0       */
        volatile uint32_t reserved0:4;                   /**< bit 24..27   access: RW       default: 0x0       */
        volatile uint32_t rsv0:4;                        /**< bit 28..31   access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_error_dflash_2bit_ecc_stat_type_t;

/**
 * @brief FMC error sram 2bit ecc status structure
 */
typedef union {
    struct {
        volatile uint32_t flash_addr:17;                 /**< bit 0..16    access: RO       default: 0x0       */
        volatile uint32_t rsv:15;                        /**< bit 17..31   access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_error_sram_2bit_ecc_stat_type_t;

/** reserved FMC_RESERVED_66_Type */

/** reserved FMC_RESERVED_67_Type */

/** reserved FMC_RESERVED_68_Type */

/** reserved FMC_RESERVED_69_Type */

/** reserved FMC_RESERVED_70_Type */

/** reserved FMC_RESERVED_71_Type */

/** reserved FMC_RESERVED_72_Type */

/** reserved FMC_RESERVED_73_Type */

/** reserved FMC_RESERVED_74_Type */

/** reserved FMC_RESERVED_75_Type */

/** reserved FMC_RESERVED_76_Type */

/** reserved FMC_RESERVED_77_Type */

/** reserved FMC_RESERVED_78_Type */

/** reserved FMC_RESERVED_79_Type */

/** reserved FMC_RESERVED_80_Type */

/** reserved FMC_RESERVED_81_Type */

/** reserved FMC_RESERVED_82_Type */

/**
 * @brief FMC eeprom sector configuration structure
 */
typedef union {
    struct {
        volatile uint32_t sector_begin0:8;               /**< bit 0..7     access: RW       default: 0x0       */
        volatile uint32_t sector_num0:8;                 /**< bit 8..15    access: RW       default: 0x0       */
        volatile uint32_t sector_begin1:8;               /**< bit 16..23   access: RW       default: 0x0       */
        volatile uint32_t sector_num1:8;                 /**< bit 24..31   access: RW       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_eeprom_sector_cfg_type_t;

/** reserved FMC_RESERVED_83_Type */

/** reserved FMC_RESERVED_84_Type */

/** reserved FMC_RESERVED_85_Type */

/**
 * @brief FMC eeprom copy cmd configuration structure
 */
typedef union {
    struct {
        volatile uint32_t sector_begin0:8;               /**< bit 0..7     access: RW       default: 0x0       */
        volatile uint32_t sector_num0:8;                 /**< bit 8..15    access: RW       default: 0x0       */
        volatile uint32_t sector_begin1:8;               /**< bit 16..23   access: RW       default: 0x0       */
        volatile uint32_t sector_num1:8;                 /**< bit 24..31   access: RW       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_eeprom_copy_cmd_cfg_type_t;

/**
 * @brief FMC eeprom copy cmd ctrl structure
 */
typedef union {
    struct {
        volatile uint32_t sector0_en:1;                  /**< bit 0        access: RW       default: 0x0       */
        volatile uint32_t sector1_en:1;                  /**< bit 1        access: RW       default: 0x0       */
        volatile uint32_t rsv:30;                        /**< bit 2..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_eeprom_copy_cmd_ctrl_type_t;

/**
 * @brief FMC eeprom copy cmd status structure
 */
typedef union {
    struct {
        volatile uint32_t sector0_copy_done:1;           /**< bit 0        access: RW1C     default: 0x0       */
        volatile uint32_t sector1_copy_done:1;           /**< bit 1        access: RW1C     default: 0x0       */
        volatile uint32_t rsv:30;                        /**< bit 2..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_eeprom_copy_cmd_status_type_t;

/** reserved FMC_RESERVED_86_Type */

/**
 * @brief FMC eeprom write cmd configuration structure
 */
typedef union {
    struct {
        volatile uint32_t sram_addr:12;                  /**< bit 0..11    access: RW       default: 0x0       */
        volatile uint32_t db_length:4;                   /**< bit 12..15   access: RW       default: 0x0       */
        volatile uint32_t flash_addr:16;                 /**< bit 16..31   access: RW       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_eeprom_write_cmd_cfg_type_t;

/**
 * @brief FMC eeprom write cmd status structure
 */
typedef union {
    struct {
        volatile uint32_t write_done:1;                  /**< bit 0        access: RW1C     default: 0x0       */
        volatile uint32_t status:1;                      /**< bit 1        access: RW1C     default: 0x0       */
        volatile uint32_t rsv:30;                        /**< bit 2..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_eeprom_write_cmd_status_type_t;

/** reserved FMC_RESERVED_87_Type */

/** reserved FMC_RESERVED_88_Type */

/**
 * @brief FMC pflash ahb access cnt structure
 */
typedef union {
    struct {
        volatile uint32_t cnt:32;                        /**< bit 0..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_pflash_ahb_aces_cnt_type_t;

/**
 * @brief FMC pflash ahb pref cnt structure
 */
typedef union {
    struct {
        volatile uint32_t cnt:32;                        /**< bit 0..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_pflash_ahb_pref_cnt_type_t;

/**
 * @brief FMC pflash ahb conflict cnt structure
 */
typedef union {
    struct {
        volatile uint32_t cnt:32;                        /**< bit 0..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_pflash_ahb_conflict_cnt_type_t;

/** reserved FMC_RESERVED_89_Type */

/**
 * @brief FMC dflash ahb access cnt structure
 */
typedef union {
    struct {
        volatile uint32_t cnt:32;                        /**< bit 0..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_dflash_ahb_aces_cnt_type_t;

/**
 * @brief FMC dflash ahb pref cnt structure
 */
typedef union {
    struct {
        volatile uint32_t cnt:32;                        /**< bit 0..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_dflash_ahb_pref_cnt_type_t;

/**
 * @brief FMC dflash ahb conflict cnt structure
 */
typedef union {
    struct {
        volatile uint32_t cnt:32;                        /**< bit 0..31    access: RO       default: 0x0       */
    } b;
    volatile uint32_t w;
} fmc_dflash_ahb_conflict_cnt_type_t;

/**
 * @brief FMC Register Layout Typedef
 */
typedef struct {
    volatile uint32_t FMC_CTRL_CFG;              /**< FMC Control Register, offset: 0x00 */
    volatile uint32_t FMC_CG_EN_CFG;             /**< FMC CG Enable Configuration Register, 0xffset: 0x04 */
    volatile uint32_t FMC_STATUS;                /**< FMC Status Register , 0xffset: 0x08 */
    volatile uint32_t RESERVED_0;                /**< RESERVED, offset: 0x0C */
    volatile uint32_t RESERVED_1;                /**< RESERVED, offset: 0x10 */
    volatile uint32_t RESERVED_2;                /**< RESERVED, offset: 0x14*/
    volatile uint32_t RESERVED_3;                /**< RESERVED, offset: 0x18*/
    volatile uint32_t RESERVED_4;                /**< RESERVED, offset: 0x1C */
    volatile uint32_t FMC_PFLASH_READ_CMD_CFG;   /**< PFlash Read Command Configuration Register, offset: 0x20 */
    volatile uint32_t FMC_PFLASH_READ_CMD_STATUS; /**< PFlash Read Command Status Register, offset: 0x24 */
    volatile uint32_t FMC_PFLASH_READ_CMD_DATA0; /**< PFlash Read Command Data1 Register, offset: 0x28 */
    volatile uint32_t FMC_PFLASH_READ_CMD_DATA1; /**< PFlash Read Command Data0 Register, offset: 0x2C */
    volatile uint32_t RESERVED_5;                /**< RESERVED, offset: 0x30 */
    volatile uint32_t RESERVED_6;                /**< RESERVED, offset: 0x34 */
    volatile uint32_t RESERVED_7;                /**< RESERVED, offset: 0x38 */
    volatile uint32_t RESERVED_8;                /**< RESERVED, offset: 0x3C */
    volatile uint32_t FMC_PFLASH_PROGRAM_CMD_CFG; /**< PFlash Program Command Register, offset: 0x40  */
    volatile uint32_t FMC_PFLASH_PROGRAM_CMD_STATUS; /**< FMC PFlash Program Command Status Register, offset:0x44 */
    volatile uint32_t FMC_PFLASH_PROGRAM_CMD_DATA0; /**< PFlash Program Command Data1 Register, offset:0x48 */
    volatile uint32_t FMC_PFLASH_PROGRAM_CMD_DATA1; /**< PFlash Program Command Data1 Register, offset: 0x4C */
    volatile uint32_t RESERVED_9;                /**< RESERVED, offset: 0x50 */
    volatile uint32_t RESERVED_10;               /**< RESERVED, offset: 0x54 */
    volatile uint32_t RESERVED_11;               /**< RESERVED, offset: 0x58 */
    volatile uint32_t RESERVED_12;               /**< RESERVED, offset: 0x5C  */
    volatile uint32_t FMC_PFLASH_ERASE_CMD_CFG;  /**< PFlash Erase Command Register, offset: 0x60 */
    volatile uint32_t FMC_PFLASH_ERASE_CMD_STATUS; /**< PFlash Erase Command Status Register, offset: 0x64  */
    volatile uint32_t RESERVED_13;               /**< RESERVED, offset: 0x68 */
    volatile uint32_t RESERVED_14;               /**< RESERVED, offset: 0x6C*/
    volatile uint32_t FMC_PFLASH_POWER_MODE_CFG; /**< PFlash Power Mode Configuration Register, offset: 0x70 */
    volatile uint32_t RESERVED_15;               /**< RESERVED, offset: 0x74 */
    volatile uint32_t RESERVED_16;               /**< RESERVED, offset: 0x78 */
    volatile uint32_t RESERVED_17;               /**< RESERVED, offset: 0x7C */
    volatile uint32_t FMC_DFLASH_READ_CMD_CFG;   /**< DFlash Read Command Register, offset: 0x80 */
    volatile uint32_t FMC_DFLASH_READ_CMD_STATUS; /**< DFlash Read Command Status Register, offset: 0x84 */
    volatile uint32_t FMC_DFLASH_READ_CMD_DATA0; /**< DFlash Read Command Data0 Register, offset: 0x88 */
    volatile uint32_t FMC_DFLASH_READ_CMD_DATA1; /**< DFlash Read Command Data1 Register, offset: 0x8C */
    volatile uint32_t RESERVED_18;               /**< RESERVED, offset: 0x90 */
    volatile uint32_t RESERVED_19;               /**< RESERVED, offset: 0x94 */
    volatile uint32_t RESERVED_20;               /**< RESERVED, offset: 0x98 */
    volatile uint32_t RESERVED_21;               /**< RESERVED, offset: 0x9C*/
    volatile uint32_t FMC_DFLASH_PROGRAM_CMD_CFG; /**< DFlash Program Command Register, offset: 0xA0 */
    volatile uint32_t FMC_DFLASH_PROGRAM_CMD_STATUS; /**< DFlash Program Command Status Register, offset: 0xA4 */
    volatile uint32_t FMC_DFLASH_PROGRAM_CMD_DATA0; /**< DFlash Program Command Data0 Register, offset: 0xA8 */
    volatile uint32_t FMC_DFLASH_PROGRAM_CMD_DATA1; /**< DFlash Program Command Data1 Register, offset: 0xAC */
    volatile uint32_t RESERVED_22;               /**< RESERVED, offset: 0xB0 */
    volatile uint32_t RESERVED_23;               /**< RESERVED, offset: 0xB4 */
    volatile uint32_t RESERVED_24;               /**< RESERVED, offset: 0xB8 */
    volatile uint32_t RESERVED_25;               /**< RESERVED, offset: 0xBC */
    volatile uint32_t FMC_DFLASH_ERASE_CMD_CFG;  /**< DFlash Erase Command Register, offset: 0xC0 */
    volatile uint32_t FMC_DFLASH_ERASE_CMD_STATUS; /**< DFlash Erase Command Status Register, offset: 0xC4  */
    volatile uint32_t RESERVED_26;               /**< RESERVED, offset: 0xC8 */
    volatile uint32_t RESERVED_27;               /**< RESERVED, offset: 0xCC */
    volatile uint32_t FMC_DFLASH_POWER_MODE_CFG; /**< DFlash Power Mode Configuration Register, offset: 0xD0 */
    volatile uint32_t RESERVED_28;               /**< RESERVED, offset: 0xD4 */
    volatile uint32_t RESERVED_29;               /**< RESERVED, offset: 0xD8 */
    volatile uint32_t RESERVED_30;               /**< RESERVED, offset: 0xDC */
    volatile uint32_t RESERVED_31;               /**< RESERVED, offset: 0xE0 */
    volatile uint32_t RESERVED_32;               /**< RESERVED, offset: 0xE4 */
    volatile uint32_t RESERVED_33;               /**< RESERVED, offset: 0xE8 */
    volatile uint32_t RESERVED_34;               /**< RESERVED, offset: 0xEC */
    volatile uint32_t RESERVED_35;               /**< RESERVED, offset: 0xF0 */
    volatile uint32_t RESERVED_36;               /**< RESERVED, offset: 0xF4 */
    volatile uint32_t RESERVED_37;               /**< RESERVED, offset: 0xF8 */
    volatile uint32_t RESERVED_38;               /**< RESERVED, offset: 0xFC */
    volatile uint32_t FMC_PFLASH_WR_PROTECT_BITMAP; /**< PFlash Write Protection Bitmap Registerm, offset: 0x100 */
    volatile uint32_t FMC_DFLASH_WR_PROTECT_BITMAP; /**< DFlash Write Protection Bitmap Register, offset: 0x104 */
    volatile uint32_t RESERVED_39;               /**< RESERVED, offset: 0x108 */
    volatile uint32_t RESERVED_40;               /**< RESERVED, offset: 0x10C */
    volatile uint32_t RESERVED_41;               /**< RESERVED, offset: 0x110 */
    volatile uint32_t RESERVED_42;               /**< RESERVED, offset: 0x114 */
    volatile uint32_t RESERVED_43;               /**< RESERVED, offset: 0x118 */
    volatile uint32_t RESERVED_44;               /**< RESERVED, offset: 0x11C */
    volatile uint32_t FMC_NORMAL_INT_RAW;        /**< Normal Interrupt Raw Register, offset: 0x120 */
    volatile uint32_t FMC_NORMAL_INT_MSK;        /**< Normal Interrupt Mask Register, offset: 0x124 */
    volatile uint32_t FMC_NORMAL_INT_STAT;       /**< Normal Interrupt Status Register, offset: 0x128 */
    volatile uint32_t RESERVED_45;               /**< RESERVED, offset: 0x12C */
    volatile uint32_t RESERVED_46;               /**< RESERVED, offset: 0x130 */
    volatile uint32_t RESERVED_47;               /**< RESERVED, offset: 0x134 */
    volatile uint32_t RESERVED_48;               /**< RESERVED, offset: 0x138 */
    volatile uint32_t RESERVED_49;               /**< RESERVED, offset: 0x13C */
    volatile uint32_t FMC_WARNING_INT_RAW;       /**< Warning Interrupt Register, offset: 0x140  */
    volatile uint32_t FMC_WARNING_INT_MSK;       /**< Warning Interrupt Mask Register, offset: 0x144 */
    volatile uint32_t FMC_WARNING_INT_STAT;      /**< Warning Interrupt Status Register, offset: 0x148 */
    volatile uint32_t RESERVED_50;               /**< RESERVED, offset: 0x14C */
    volatile uint32_t RESERVED_51;               /**< RESERVED, offset: 0x150 */
    volatile uint32_t RESERVED_52;               /**< RESERVED, offset: 0x154 */
    volatile uint32_t RESERVED_53;               /**< RESERVED, offset: 0x158 */
    volatile uint32_t RESERVED_54;               /**< RESERVED, offset: 0x15C */
    volatile uint32_t FMC_WARNING_PFLASH_1BIT_ECC_ADDR; /**< PFlash 1-bit ECC Error Register, offset: 0x160 */
    volatile uint32_t FMC_WARNING_PFLASH_1BIT_ECC_CNT; /**< PFlash 1-bit ECC Error Counter Register, offset: 0x164 */
    volatile uint32_t RESERVED_55;               /**< RESERVED, offset: 0x168 */
    volatile uint32_t RESERVED_56;               /**< RESERVED, offset: 0x16C */
    volatile uint32_t FMC_WARNING_DFLASH_1BIT_ECC_ADDR; /**< DFlash 1-bit ECC Address Register, offset: 0x170 */
    volatile uint32_t FMC_WARNING_DFLASH_1BIT_ECC_CNT; /**< DFlash 1-bit ECC Address Register, offset: 0x174 */
    volatile uint32_t RESERVED_57;               /**< RESERVED, offset: 0x178 */
    volatile uint32_t RESERVED_58;               /**< RESERVED, offset: 0x17C */
    volatile uint32_t FMC_WARNING_SRAM_1BIT_ECC_ADDR; /**< SRAM 1-bit ECC Address Register, offset: 0x180 */
    volatile uint32_t FMC_WARNING_SRAM_1BIT_ECC_CNT; /**< SRAM 1-bit ECC Error Counter, offet: 0x184 */
    volatile uint32_t RESERVED_59;               /**< RESERVED, offset: 0x188 */
    volatile uint32_t RESERVED_60;               /**< RESERVED, offset: 0x18C */
    volatile uint32_t FMC_WARNING_PFLASH_WR_PROTECT_STAT; /**< PFlash Write Protection Status Register, offset: 0x190  */
    volatile uint32_t FMC_WARNING_DFLASH_WR_PROTECT_STAT; /**< DFlash Write Protection Status Register, offset: 0x194   */
    volatile uint32_t RESERVED_61;               /**< RESERVED, offset: 0x198 */
    volatile uint32_t RESERVED_62;               /**< RESERVED, offset: 0x19C */
    volatile uint32_t FMC_ERROR_INT_RAW;         /**< FMC Error Interrupt Raw Register, offset: 0x1A0 */
    volatile uint32_t RESERVED_63;               /**< RESERVED, offset: 0x1A4 */
    volatile uint32_t RESERVED_64;               /**< RESERVED, offset: 0x1A8 */
    volatile uint32_t RESERVED_65;               /**< RESERVED, offset: 0x1AC */
    volatile uint32_t FMC_ERROR_PFLASH_2BIT_ECC_STAT; /**< PFlash 2-bit ECC Error Status Register, 0x1B0  */
    volatile uint32_t FMC_ERROR_DFLASH_2BIT_ECC_STAT; /**< DFlash 2-bit ECC Error Status Register, 0x1B4 */
    volatile uint32_t FMC_ERROR_SRAM_2BIT_ECC_STAT; /**<  SRAM 2-bit ECC Error Status Register, 0x1B8 */
    volatile uint32_t RESERVED_66;               /**< RESERVED, offset: 0x1BC */
    volatile uint32_t RESERVED_67;               /**< RESERVED, offset: 0x1C0 */
    volatile uint32_t RESERVED_68;               /**< RESERVED, offset: 0x1C4 */
    volatile uint32_t RESERVED_69;               /**< RESERVED, offset: 0x1C8 */
    volatile uint32_t RESERVED_70;               /**< RESERVED, offset: 0x1CC */
    volatile uint32_t RESERVED_71;               /**< RESERVED, offset: 0x1D0 */
    volatile uint32_t RESERVED_72;               /**< RESERVED, offset: 0x1D4 */
    volatile uint32_t RESERVED_73;               /**< RESERVED, offset: 0x1D8 */
    volatile uint32_t RESERVED_74;               /**< RESERVED, offset: 0x1DC */
    volatile uint32_t RESERVED_75;               /**< RESERVED, offset: 0x1E0 */
    volatile uint32_t RESERVED_76;               /**< RESERVED, offset: 0x1E4 */
    volatile uint32_t RESERVED_77;               /**< RESERVED, offset: 0x1E8 */
    volatile uint32_t RESERVED_78;               /**< RESERVED, offset: 0x1EC */
    volatile uint32_t RESERVED_79;               /**< RESERVED, offset: 0x1F0 */
    volatile uint32_t RESERVED_80;               /**< RESERVED, offset: 0x1F4 */
    volatile uint32_t RESERVED_81;               /**< RESERVED, offset: 0x1F8  */
    volatile uint32_t RESERVED_82;               /**< RESERVED, offset: 0x1FC */
    volatile uint32_t FMC_EEPROM_SECTOR_CFG;     /**< EEPROM Sector Configuration Register, offset: 0x200 */
    volatile uint32_t RESERVED_83;               /**< RESERVED, offset: 0x204 */
    volatile uint32_t RESERVED_84;               /**< RESERVED, offset: 0x208 */
    volatile uint32_t RESERVED_85;               /**< RESERVED, offset: 0x20C */
    volatile uint32_t FMC_EEPROM_COPY_CMD_CFG;   /**< EEPROM Copy Command Register , offset: 0x210 */
    volatile uint32_t FMC_EEPROM_COPY_CMD_CTRL;  /**< EEPROM Copy Command Control Register, offset: 0x214 */
    volatile uint32_t FMC_EEPROM_COPY_CMD_STATUS; /**< EEPROM Copy Command Status Register, offset: 0x218 */
    volatile uint32_t RESERVED_86;               /**< RESERVED, offset: 0x21C */
    volatile uint32_t FMC_EEPROM_WRITE_CMD_CFG;  /**< EEPROM Write Command Register, offset: 0x220  */
    volatile uint32_t FMC_EEPROM_WRITE_CMD_STATUS; /**< EEPROM Write Command Status Register, 0x224 */
    volatile uint32_t RESERVED_87;               /**< RESERVED, offset: 0x228 */
    volatile uint32_t RESERVED_88;               /**< RESERVED, offset: 0x22C */
    volatile uint32_t FMC_PFLASH_AHB_ACES_CNT;   /**< PFlash Access through AHB Counter, offset: 0x230 */
    volatile uint32_t FMC_PFLASH_AHB_PREF_CNT;   /**< PFlash Access Through AHB for Prefetch Counter, offset: 0x234 */
    volatile uint32_t FMC_PFLASH_AHB_CONFLICT_CNT; /**< PFlash Access Through AHB Conflict Counter, offset: 0x238 */
    volatile uint32_t RESERVED_89;               /**< RESERVED, offset: 0x23C */
    volatile uint32_t FMC_DFLASH_AHB_ACES_CNT;   /**< DFlash Access Through AHB Counter, offset: 0x240 */
    volatile uint32_t FMC_DFLASH_AHB_PREF_CNT;   /**< DFlash Access Through AHB Counter, offset: 0x244 */
    volatile uint32_t FMC_DFLASH_AHB_CONFLICT_CNT; /**< DFlash Access Through AHB Conflict Counter, 0x248 */
} fmc_type_t;

#define FMC_INSTANCE_COUNT                     (1U)

/** FMC - Peripheral instance base addresses */

/** Peripheral FMC base address */
#define FMC_BASE                              (FMC_BASEADDR)
/** Peripheral FMC base pointer */
#define FMC                                  ((fmc_type_t *)FMC_BASE)
#define FMC_FMC_CTRL_CFG_PFLASH_WR_PROTECT_EN_Pos                    (0U)
#define FMC_FMC_CTRL_CFG_PFLASH_WR_PROTECT_EN_Msk                    (0x1UL << FMC_FMC_CTRL_CFG_PFLASH_WR_PROTECT_EN_Pos)
#define FMC_FMC_CTRL_CFG_DFLASH_WR_PROTECT_EN_Pos                    (1U)
#define FMC_FMC_CTRL_CFG_DFLASH_WR_PROTECT_EN_Msk                    (0x1UL << FMC_FMC_CTRL_CFG_DFLASH_WR_PROTECT_EN_Pos)
#define FMC_FMC_CTRL_CFG_PFLASH_ECC_EN_Pos                           (2U)
#define FMC_FMC_CTRL_CFG_PFLASH_ECC_EN_Msk                           (0x1UL << FMC_FMC_CTRL_CFG_PFLASH_ECC_EN_Pos)
#define FMC_FMC_CTRL_CFG_DFLASH_ECC_EN_Pos                           (3U)
#define FMC_FMC_CTRL_CFG_DFLASH_ECC_EN_Msk                           (0x1UL << FMC_FMC_CTRL_CFG_DFLASH_ECC_EN_Pos)
#define FMC_FMC_CTRL_CFG_SRAM_ECC_EN_Pos                             (4U)
#define FMC_FMC_CTRL_CFG_SRAM_ECC_EN_Msk                             (0x1UL << FMC_FMC_CTRL_CFG_SRAM_ECC_EN_Pos)
#define FMC_FMC_CTRL_CFG_RSV1_Pos                                    (5U)
#define FMC_FMC_CTRL_CFG_RSV1_Msk                                    (0x1UL << FMC_FMC_CTRL_CFG_RSV1_Pos)
#define FMC_FMC_CTRL_CFG_DFLASH_PREFETCH_EN_Pos                      (6U)
#define FMC_FMC_CTRL_CFG_DFLASH_PREFETCH_EN_Msk                      (0x1UL << FMC_FMC_CTRL_CFG_DFLASH_PREFETCH_EN_Pos)
#define FMC_FMC_CTRL_CFG_PFLASH_PREFETCH_EN_Pos                      (7U)
#define FMC_FMC_CTRL_CFG_PFLASH_PREFETCH_EN_Msk                      (0x1UL << FMC_FMC_CTRL_CFG_PFLASH_PREFETCH_EN_Pos)
#define FMC_FMC_CTRL_CFG_AHB_BUS_ERROR_MSK_Pos                       (8U)
#define FMC_FMC_CTRL_CFG_AHB_BUS_ERROR_MSK_Msk                       (0x1UL << FMC_FMC_CTRL_CFG_AHB_BUS_ERROR_MSK_Pos)
#define FMC_FMC_CTRL_CFG_APB_BUS_ERROR_MSK_Pos                       (9U)
#define FMC_FMC_CTRL_CFG_APB_BUS_ERROR_MSK_Msk                       (0x1UL << FMC_FMC_CTRL_CFG_APB_BUS_ERROR_MSK_Pos)
#define FMC_FMC_CTRL_CFG_FMC_CLOCK_MODE_Pos                          (10U)
#define FMC_FMC_CTRL_CFG_FMC_CLOCK_MODE_Msk                          (0x3UL << FMC_FMC_CTRL_CFG_FMC_CLOCK_MODE_Pos)
#define FMC_FMC_CTRL_CFG_RSV0_Pos                                    (12U)
#define FMC_FMC_CTRL_CFG_RSV0_Msk                                    (0xfUL << FMC_FMC_CTRL_CFG_RSV0_Pos)
#define FMC_FMC_CTRL_CFG_ECC_1BIT_ERR_THOD_Pos                       (16U)
#define FMC_FMC_CTRL_CFG_ECC_1BIT_ERR_THOD_Msk                       (0xffffUL << FMC_FMC_CTRL_CFG_ECC_1BIT_ERR_THOD_Pos)

#define FMC_FMC_CG_EN_CFG_AUTO_CG_EN_Pos                             (0U)
#define FMC_FMC_CG_EN_CFG_AUTO_CG_EN_Msk                             (0x1UL << FMC_FMC_CG_EN_CFG_AUTO_CG_EN_Pos)
#define FMC_FMC_CG_EN_CFG_RSV_Pos                                    (1U)
#define FMC_FMC_CG_EN_CFG_RSV_Msk                                    (0x7fffffffUL << FMC_FMC_CG_EN_CFG_RSV_Pos)

#define FMC_FMC_STATUS_FMC_STATUS_Pos                                (0U)
#define FMC_FMC_STATUS_FMC_STATUS_Msk                                (0x1UL << FMC_FMC_STATUS_FMC_STATUS_Pos)
#define FMC_FMC_STATUS_RSV_Pos                                       (1U)
#define FMC_FMC_STATUS_RSV_Msk                                       (0x7fffffffUL << FMC_FMC_STATUS_RSV_Pos)






#define FMC_FMC_PFLASH_READ_CMD_CFG_READ_MODE_Pos                    (0U)
#define FMC_FMC_PFLASH_READ_CMD_CFG_READ_MODE_Msk                    (0x3UL << FMC_FMC_PFLASH_READ_CMD_CFG_READ_MODE_Pos)
#define FMC_FMC_PFLASH_READ_CMD_CFG_SECTOR_SEL_Pos                   (2U)
#define FMC_FMC_PFLASH_READ_CMD_CFG_SECTOR_SEL_Msk                   (0x3UL << FMC_FMC_PFLASH_READ_CMD_CFG_SECTOR_SEL_Pos)
#define FMC_FMC_PFLASH_READ_CMD_CFG_FLASH_ADDR_Pos                   (4U)
#define FMC_FMC_PFLASH_READ_CMD_CFG_FLASH_ADDR_Msk                   (0xfffffUL << FMC_FMC_PFLASH_READ_CMD_CFG_FLASH_ADDR_Pos)
#define FMC_FMC_PFLASH_READ_CMD_CFG_REDUND_SEL_Pos                   (24U)
#define FMC_FMC_PFLASH_READ_CMD_CFG_REDUND_SEL_Msk                   (0x1UL << FMC_FMC_PFLASH_READ_CMD_CFG_REDUND_SEL_Pos)
#define FMC_FMC_PFLASH_READ_CMD_CFG_RSV_Pos                          (25U)
#define FMC_FMC_PFLASH_READ_CMD_CFG_RSV_Msk                          (0x7fUL << FMC_FMC_PFLASH_READ_CMD_CFG_RSV_Pos)

#define FMC_FMC_PFLASH_READ_CMD_STATUS_CMD_STAT_Pos                  (0U)
#define FMC_FMC_PFLASH_READ_CMD_STATUS_CMD_STAT_Msk                  (0x1UL << FMC_FMC_PFLASH_READ_CMD_STATUS_CMD_STAT_Pos)
#define FMC_FMC_PFLASH_READ_CMD_STATUS_RSV1_Pos                      (1U)
#define FMC_FMC_PFLASH_READ_CMD_STATUS_RSV1_Msk                      (0x7UL << FMC_FMC_PFLASH_READ_CMD_STATUS_RSV1_Pos)
#define FMC_FMC_PFLASH_READ_CMD_STATUS_FLASH_DATA_Pos                (4U)
#define FMC_FMC_PFLASH_READ_CMD_STATUS_FLASH_DATA_Msk                (0xffUL << FMC_FMC_PFLASH_READ_CMD_STATUS_FLASH_DATA_Pos)
#define FMC_FMC_PFLASH_READ_CMD_STATUS_RSV0_Pos                      (12U)
#define FMC_FMC_PFLASH_READ_CMD_STATUS_RSV0_Msk                      (0xfffffUL << FMC_FMC_PFLASH_READ_CMD_STATUS_RSV0_Pos)

#define FMC_FMC_PFLASH_READ_CMD_DATA0_FLASH_DATA_Pos                 (0U)
#define FMC_FMC_PFLASH_READ_CMD_DATA0_FLASH_DATA_Msk                 (0xffffffffUL << FMC_FMC_PFLASH_READ_CMD_DATA0_FLASH_DATA_Pos)

#define FMC_FMC_PFLASH_READ_CMD_DATA1_FLASH_DATA_Pos                 (0U)
#define FMC_FMC_PFLASH_READ_CMD_DATA1_FLASH_DATA_Msk                 (0xffffffffUL << FMC_FMC_PFLASH_READ_CMD_DATA1_FLASH_DATA_Pos)





#define FMC_FMC_PFLASH_PROGRAM_CMD_CFG_SECTOR_SEL_Pos                (0U)
#define FMC_FMC_PFLASH_PROGRAM_CMD_CFG_SECTOR_SEL_Msk                (0x3UL << FMC_FMC_PFLASH_PROGRAM_CMD_CFG_SECTOR_SEL_Pos)
#define FMC_FMC_PFLASH_PROGRAM_CMD_CFG_RSV1_Pos                      (2U)
#define FMC_FMC_PFLASH_PROGRAM_CMD_CFG_RSV1_Msk                      (0x3UL << FMC_FMC_PFLASH_PROGRAM_CMD_CFG_RSV1_Pos)
#define FMC_FMC_PFLASH_PROGRAM_CMD_CFG_FLASH_ADDR_Pos                (4U)
#define FMC_FMC_PFLASH_PROGRAM_CMD_CFG_FLASH_ADDR_Msk                (0xfffffUL << FMC_FMC_PFLASH_PROGRAM_CMD_CFG_FLASH_ADDR_Pos)
#define FMC_FMC_PFLASH_PROGRAM_CMD_CFG_REDUND_SEL_Pos                (24U)
#define FMC_FMC_PFLASH_PROGRAM_CMD_CFG_REDUND_SEL_Msk                (0x1UL << FMC_FMC_PFLASH_PROGRAM_CMD_CFG_REDUND_SEL_Pos)
#define FMC_FMC_PFLASH_PROGRAM_CMD_CFG_RSV0_Pos                      (25U)
#define FMC_FMC_PFLASH_PROGRAM_CMD_CFG_RSV0_Msk                      (0x7fUL << FMC_FMC_PFLASH_PROGRAM_CMD_CFG_RSV0_Pos)

#define FMC_FMC_PFLASH_PROGRAM_CMD_STATUS_CMD_STAT_Pos               (0U)
#define FMC_FMC_PFLASH_PROGRAM_CMD_STATUS_CMD_STAT_Msk               (0x1UL << FMC_FMC_PFLASH_PROGRAM_CMD_STATUS_CMD_STAT_Pos)
#define FMC_FMC_PFLASH_PROGRAM_CMD_STATUS_ERR_STAT_Pos               (1U)
#define FMC_FMC_PFLASH_PROGRAM_CMD_STATUS_ERR_STAT_Msk               (0x1UL << FMC_FMC_PFLASH_PROGRAM_CMD_STATUS_ERR_STAT_Pos)
#define FMC_FMC_PFLASH_PROGRAM_CMD_STATUS_RSV_Pos                    (2U)
#define FMC_FMC_PFLASH_PROGRAM_CMD_STATUS_RSV_Msk                    (0x3fffffffUL << FMC_FMC_PFLASH_PROGRAM_CMD_STATUS_RSV_Pos)

#define FMC_FMC_PFLASH_PROGRAM_CMD_DATA0_FLASH_DATA_Pos              (0U)
#define FMC_FMC_PFLASH_PROGRAM_CMD_DATA0_FLASH_DATA_Msk              (0xffffffffUL << FMC_FMC_PFLASH_PROGRAM_CMD_DATA0_FLASH_DATA_Pos)

#define FMC_FMC_PFLASH_PROGRAM_CMD_DATA1_FLASH_DATA_Pos              (0U)
#define FMC_FMC_PFLASH_PROGRAM_CMD_DATA1_FLASH_DATA_Msk              (0xffffffffUL << FMC_FMC_PFLASH_PROGRAM_CMD_DATA1_FLASH_DATA_Pos)





#define FMC_FMC_PFLASH_ERASE_CMD_CFG_ERASE_MODE_Pos                  (0U)
#define FMC_FMC_PFLASH_ERASE_CMD_CFG_ERASE_MODE_Msk                  (0x3UL << FMC_FMC_PFLASH_ERASE_CMD_CFG_ERASE_MODE_Pos)
#define FMC_FMC_PFLASH_ERASE_CMD_CFG_SECTOR_SEL_Pos                  (2U)
#define FMC_FMC_PFLASH_ERASE_CMD_CFG_SECTOR_SEL_Msk                  (0x3UL << FMC_FMC_PFLASH_ERASE_CMD_CFG_SECTOR_SEL_Pos)
#define FMC_FMC_PFLASH_ERASE_CMD_CFG_FLASH_ADDR_Pos                  (4U)
#define FMC_FMC_PFLASH_ERASE_CMD_CFG_FLASH_ADDR_Msk                  (0xfffffUL << FMC_FMC_PFLASH_ERASE_CMD_CFG_FLASH_ADDR_Pos)
#define FMC_FMC_PFLASH_ERASE_CMD_CFG_REDUND_SEL_Pos                  (24U)
#define FMC_FMC_PFLASH_ERASE_CMD_CFG_REDUND_SEL_Msk                  (0x1UL << FMC_FMC_PFLASH_ERASE_CMD_CFG_REDUND_SEL_Pos)
#define FMC_FMC_PFLASH_ERASE_CMD_CFG_RETRY_Pos                       (25U)
#define FMC_FMC_PFLASH_ERASE_CMD_CFG_RETRY_Msk                       (0x3UL << FMC_FMC_PFLASH_ERASE_CMD_CFG_RETRY_Pos)
#define FMC_FMC_PFLASH_ERASE_CMD_CFG_RSV_Pos                         (27U)
#define FMC_FMC_PFLASH_ERASE_CMD_CFG_RSV_Msk                         (0x1fUL << FMC_FMC_PFLASH_ERASE_CMD_CFG_RSV_Pos)

#define FMC_FMC_PFLASH_ERASE_CMD_STATUS_CMD_STAT_Pos                 (0U)
#define FMC_FMC_PFLASH_ERASE_CMD_STATUS_CMD_STAT_Msk                 (0x1UL << FMC_FMC_PFLASH_ERASE_CMD_STATUS_CMD_STAT_Pos)
#define FMC_FMC_PFLASH_ERASE_CMD_STATUS_ERR_STAT_Pos                 (1U)
#define FMC_FMC_PFLASH_ERASE_CMD_STATUS_ERR_STAT_Msk                 (0x1UL << FMC_FMC_PFLASH_ERASE_CMD_STATUS_ERR_STAT_Pos)
#define FMC_FMC_PFLASH_ERASE_CMD_STATUS_RSV_Pos                      (2U)
#define FMC_FMC_PFLASH_ERASE_CMD_STATUS_RSV_Msk                      (0x3fffffffUL << FMC_FMC_PFLASH_ERASE_CMD_STATUS_RSV_Pos)



#define FMC_FMC_PFLASH_POWER_MODE_CFG_POWER_MODE_Pos                 (0U)
#define FMC_FMC_PFLASH_POWER_MODE_CFG_POWER_MODE_Msk                 (0x1UL << FMC_FMC_PFLASH_POWER_MODE_CFG_POWER_MODE_Pos)
#define FMC_FMC_PFLASH_POWER_MODE_CFG_RSV_Pos                        (1U)
#define FMC_FMC_PFLASH_POWER_MODE_CFG_RSV_Msk                        (0x7fffffffUL << FMC_FMC_PFLASH_POWER_MODE_CFG_RSV_Pos)




#define FMC_FMC_DFLASH_READ_CMD_CFG_READ_MODE_Pos                    (0U)
#define FMC_FMC_DFLASH_READ_CMD_CFG_READ_MODE_Msk                    (0x3UL << FMC_FMC_DFLASH_READ_CMD_CFG_READ_MODE_Pos)
#define FMC_FMC_DFLASH_READ_CMD_CFG_SECTOR_SEL_Pos                   (2U)
#define FMC_FMC_DFLASH_READ_CMD_CFG_SECTOR_SEL_Msk                   (0x3UL << FMC_FMC_DFLASH_READ_CMD_CFG_SECTOR_SEL_Pos)
#define FMC_FMC_DFLASH_READ_CMD_CFG_FLASH_ADDR_Pos                   (4U)
#define FMC_FMC_DFLASH_READ_CMD_CFG_FLASH_ADDR_Msk                   (0xfffffUL << FMC_FMC_DFLASH_READ_CMD_CFG_FLASH_ADDR_Pos)
#define FMC_FMC_DFLASH_READ_CMD_CFG_REDUND_SEL_Pos                   (24U)
#define FMC_FMC_DFLASH_READ_CMD_CFG_REDUND_SEL_Msk                   (0x1UL << FMC_FMC_DFLASH_READ_CMD_CFG_REDUND_SEL_Pos)
#define FMC_FMC_DFLASH_READ_CMD_CFG_RSV_Pos                          (25U)
#define FMC_FMC_DFLASH_READ_CMD_CFG_RSV_Msk                          (0x7fUL << FMC_FMC_DFLASH_READ_CMD_CFG_RSV_Pos)

#define FMC_FMC_DFLASH_READ_CMD_STATUS_CMD_STAT_Pos                  (0U)
#define FMC_FMC_DFLASH_READ_CMD_STATUS_CMD_STAT_Msk                  (0x1UL << FMC_FMC_DFLASH_READ_CMD_STATUS_CMD_STAT_Pos)
#define FMC_FMC_DFLASH_READ_CMD_STATUS_RSV1_Pos                      (1U)
#define FMC_FMC_DFLASH_READ_CMD_STATUS_RSV1_Msk                      (0x7UL << FMC_FMC_DFLASH_READ_CMD_STATUS_RSV1_Pos)
#define FMC_FMC_DFLASH_READ_CMD_STATUS_FLASH_DATA_Pos                (4U)
#define FMC_FMC_DFLASH_READ_CMD_STATUS_FLASH_DATA_Msk                (0xffUL << FMC_FMC_DFLASH_READ_CMD_STATUS_FLASH_DATA_Pos)
#define FMC_FMC_DFLASH_READ_CMD_STATUS_RSV0_Pos                      (12U)
#define FMC_FMC_DFLASH_READ_CMD_STATUS_RSV0_Msk                      (0xfffffUL << FMC_FMC_DFLASH_READ_CMD_STATUS_RSV0_Pos)

#define FMC_FMC_DFLASH_READ_CMD_DATA0_FLASH_DATA_Pos                 (0U)
#define FMC_FMC_DFLASH_READ_CMD_DATA0_FLASH_DATA_Msk                 (0xffffffffUL << FMC_FMC_DFLASH_READ_CMD_DATA0_FLASH_DATA_Pos)

#define FMC_FMC_DFLASH_READ_CMD_DATA1_FLASH_DATA_Pos                 (0U)
#define FMC_FMC_DFLASH_READ_CMD_DATA1_FLASH_DATA_Msk                 (0xffffffffUL << FMC_FMC_DFLASH_READ_CMD_DATA1_FLASH_DATA_Pos)





#define FMC_FMC_DFLASH_PROGRAM_CMD_CFG_SECTOR_SEL_Pos                (0U)
#define FMC_FMC_DFLASH_PROGRAM_CMD_CFG_SECTOR_SEL_Msk                (0x3UL << FMC_FMC_DFLASH_PROGRAM_CMD_CFG_SECTOR_SEL_Pos)
#define FMC_FMC_DFLASH_PROGRAM_CMD_CFG_RSV1_Pos                      (2U)
#define FMC_FMC_DFLASH_PROGRAM_CMD_CFG_RSV1_Msk                      (0x3UL << FMC_FMC_DFLASH_PROGRAM_CMD_CFG_RSV1_Pos)
#define FMC_FMC_DFLASH_PROGRAM_CMD_CFG_FLASH_ADDR_Pos                (4U)
#define FMC_FMC_DFLASH_PROGRAM_CMD_CFG_FLASH_ADDR_Msk                (0xfffffUL << FMC_FMC_DFLASH_PROGRAM_CMD_CFG_FLASH_ADDR_Pos)
#define FMC_FMC_DFLASH_PROGRAM_CMD_CFG_REDUND_SEL_Pos                (24U)
#define FMC_FMC_DFLASH_PROGRAM_CMD_CFG_REDUND_SEL_Msk                (0x1UL << FMC_FMC_DFLASH_PROGRAM_CMD_CFG_REDUND_SEL_Pos)
#define FMC_FMC_DFLASH_PROGRAM_CMD_CFG_RSV0_Pos                      (25U)
#define FMC_FMC_DFLASH_PROGRAM_CMD_CFG_RSV0_Msk                      (0x7fUL << FMC_FMC_DFLASH_PROGRAM_CMD_CFG_RSV0_Pos)

#define FMC_FMC_DFLASH_PROGRAM_CMD_STATUS_CMD_STAT_Pos               (0U)
#define FMC_FMC_DFLASH_PROGRAM_CMD_STATUS_CMD_STAT_Msk               (0x1UL << FMC_FMC_DFLASH_PROGRAM_CMD_STATUS_CMD_STAT_Pos)
#define FMC_FMC_DFLASH_PROGRAM_CMD_STATUS_ERR_STAT_Pos               (1U)
#define FMC_FMC_DFLASH_PROGRAM_CMD_STATUS_ERR_STAT_Msk               (0x1UL << FMC_FMC_DFLASH_PROGRAM_CMD_STATUS_ERR_STAT_Pos)
#define FMC_FMC_DFLASH_PROGRAM_CMD_STATUS_RSV_Pos                    (2U)
#define FMC_FMC_DFLASH_PROGRAM_CMD_STATUS_RSV_Msk                    (0x3fffffffUL << FMC_FMC_DFLASH_PROGRAM_CMD_STATUS_RSV_Pos)

#define FMC_FMC_DFLASH_PROGRAM_CMD_DATA0_FLASH_DATA_Pos              (0U)
#define FMC_FMC_DFLASH_PROGRAM_CMD_DATA0_FLASH_DATA_Msk              (0xffffffffUL << FMC_FMC_DFLASH_PROGRAM_CMD_DATA0_FLASH_DATA_Pos)

#define FMC_FMC_DFLASH_PROGRAM_CMD_DATA1_FLASH_DATA_Pos              (0U)
#define FMC_FMC_DFLASH_PROGRAM_CMD_DATA1_FLASH_DATA_Msk              (0xffffffffUL << FMC_FMC_DFLASH_PROGRAM_CMD_DATA1_FLASH_DATA_Pos)





#define FMC_FMC_DFLASH_ERASE_CMD_CFG_ERASE_MODE_Pos                  (0U)
#define FMC_FMC_DFLASH_ERASE_CMD_CFG_ERASE_MODE_Msk                  (0x3UL << FMC_FMC_DFLASH_ERASE_CMD_CFG_ERASE_MODE_Pos)
#define FMC_FMC_DFLASH_ERASE_CMD_CFG_SECTOR_SEL_Pos                  (2U)
#define FMC_FMC_DFLASH_ERASE_CMD_CFG_SECTOR_SEL_Msk                  (0x3UL << FMC_FMC_DFLASH_ERASE_CMD_CFG_SECTOR_SEL_Pos)
#define FMC_FMC_DFLASH_ERASE_CMD_CFG_FLASH_ADDR_Pos                  (4U)
#define FMC_FMC_DFLASH_ERASE_CMD_CFG_FLASH_ADDR_Msk                  (0xfffffUL << FMC_FMC_DFLASH_ERASE_CMD_CFG_FLASH_ADDR_Pos)
#define FMC_FMC_DFLASH_ERASE_CMD_CFG_REDUND_SEL_Pos                  (24U)
#define FMC_FMC_DFLASH_ERASE_CMD_CFG_REDUND_SEL_Msk                  (0x1UL << FMC_FMC_DFLASH_ERASE_CMD_CFG_REDUND_SEL_Pos)
#define FMC_FMC_DFLASH_ERASE_CMD_CFG_RETRY_Pos                       (25U)
#define FMC_FMC_DFLASH_ERASE_CMD_CFG_RETRY_Msk                       (0x3UL << FMC_FMC_DFLASH_ERASE_CMD_CFG_RETRY_Pos)
#define FMC_FMC_DFLASH_ERASE_CMD_CFG_RSV_Pos                         (27U)
#define FMC_FMC_DFLASH_ERASE_CMD_CFG_RSV_Msk                         (0x1fUL << FMC_FMC_DFLASH_ERASE_CMD_CFG_RSV_Pos)

#define FMC_FMC_DFLASH_ERASE_CMD_STATUS_CMD_STAT_Pos                 (0U)
#define FMC_FMC_DFLASH_ERASE_CMD_STATUS_CMD_STAT_Msk                 (0x1UL << FMC_FMC_DFLASH_ERASE_CMD_STATUS_CMD_STAT_Pos)
#define FMC_FMC_DFLASH_ERASE_CMD_STATUS_ERR_STAT_Pos                 (1U)
#define FMC_FMC_DFLASH_ERASE_CMD_STATUS_ERR_STAT_Msk                 (0x1UL << FMC_FMC_DFLASH_ERASE_CMD_STATUS_ERR_STAT_Pos)
#define FMC_FMC_DFLASH_ERASE_CMD_STATUS_RSV_Pos                      (2U)
#define FMC_FMC_DFLASH_ERASE_CMD_STATUS_RSV_Msk                      (0x3fffffffUL << FMC_FMC_DFLASH_ERASE_CMD_STATUS_RSV_Pos)



#define FMC_FMC_DFLASH_POWER_MODE_CFG_POWER_MODE_Pos                 (0U)
#define FMC_FMC_DFLASH_POWER_MODE_CFG_POWER_MODE_Msk                 (0x1UL << FMC_FMC_DFLASH_POWER_MODE_CFG_POWER_MODE_Pos)
#define FMC_FMC_DFLASH_POWER_MODE_CFG_RSV_Pos                        (1U)
#define FMC_FMC_DFLASH_POWER_MODE_CFG_RSV_Msk                        (0x7fffffffUL << FMC_FMC_DFLASH_POWER_MODE_CFG_RSV_Pos)












#define FMC_FMC_PFLASH_WR_PROTECT_BITMAP_WR_PROTECT_BITMAP_Pos       (0U)
#define FMC_FMC_PFLASH_WR_PROTECT_BITMAP_WR_PROTECT_BITMAP_Msk       (0xffffffffUL << FMC_FMC_PFLASH_WR_PROTECT_BITMAP_WR_PROTECT_BITMAP_Pos)

#define FMC_FMC_DFLASH_WR_PROTECT_BITMAP_WR_PROTECT_BITMAP_Pos       (0U)
#define FMC_FMC_DFLASH_WR_PROTECT_BITMAP_WR_PROTECT_BITMAP_Msk       (0xffUL << FMC_FMC_DFLASH_WR_PROTECT_BITMAP_WR_PROTECT_BITMAP_Pos)
#define FMC_FMC_DFLASH_WR_PROTECT_BITMAP_RSV_Pos                     (8U)
#define FMC_FMC_DFLASH_WR_PROTECT_BITMAP_RSV_Msk                     (0xffffffUL << FMC_FMC_DFLASH_WR_PROTECT_BITMAP_RSV_Pos)







#define FMC_FMC_NORMAL_INT_RAW_PFLASH_ERASE_DONE_Pos                 (0U)
#define FMC_FMC_NORMAL_INT_RAW_PFLASH_ERASE_DONE_Msk                 (0x1UL << FMC_FMC_NORMAL_INT_RAW_PFLASH_ERASE_DONE_Pos)
#define FMC_FMC_NORMAL_INT_RAW_DFLASH_ERASE_DONE_Pos                 (1U)
#define FMC_FMC_NORMAL_INT_RAW_DFLASH_ERASE_DONE_Msk                 (0x1UL << FMC_FMC_NORMAL_INT_RAW_DFLASH_ERASE_DONE_Pos)
#define FMC_FMC_NORMAL_INT_RAW_PFLASH_PROGRAM_DONE_Pos               (2U)
#define FMC_FMC_NORMAL_INT_RAW_PFLASH_PROGRAM_DONE_Msk               (0x1UL << FMC_FMC_NORMAL_INT_RAW_PFLASH_PROGRAM_DONE_Pos)
#define FMC_FMC_NORMAL_INT_RAW_DFLASH_PROGRAM_DONE_Pos               (3U)
#define FMC_FMC_NORMAL_INT_RAW_DFLASH_PROGRAM_DONE_Msk               (0x1UL << FMC_FMC_NORMAL_INT_RAW_DFLASH_PROGRAM_DONE_Pos)
#define FMC_FMC_NORMAL_INT_RAW_EEPROM_COPY_DONE_Pos                  (4U)
#define FMC_FMC_NORMAL_INT_RAW_EEPROM_COPY_DONE_Msk                  (0x1UL << FMC_FMC_NORMAL_INT_RAW_EEPROM_COPY_DONE_Pos)
#define FMC_FMC_NORMAL_INT_RAW_RSV_Pos                               (5U)
#define FMC_FMC_NORMAL_INT_RAW_RSV_Msk                               (0x7ffffffUL << FMC_FMC_NORMAL_INT_RAW_RSV_Pos)

#define FMC_FMC_NORMAL_INT_MSK_PFLASH_ERASE_DONE_Pos                 (0U)
#define FMC_FMC_NORMAL_INT_MSK_PFLASH_ERASE_DONE_Msk                 (0x1UL << FMC_FMC_NORMAL_INT_MSK_PFLASH_ERASE_DONE_Pos)
#define FMC_FMC_NORMAL_INT_MSK_DFLASH_ERASE_DONE_Pos                 (1U)
#define FMC_FMC_NORMAL_INT_MSK_DFLASH_ERASE_DONE_Msk                 (0x1UL << FMC_FMC_NORMAL_INT_MSK_DFLASH_ERASE_DONE_Pos)
#define FMC_FMC_NORMAL_INT_MSK_PFLASH_PROGRAM_DONE_Pos               (2U)
#define FMC_FMC_NORMAL_INT_MSK_PFLASH_PROGRAM_DONE_Msk               (0x1UL << FMC_FMC_NORMAL_INT_MSK_PFLASH_PROGRAM_DONE_Pos)
#define FMC_FMC_NORMAL_INT_MSK_DFLASH_PROGRAM_DONE_Pos               (3U)
#define FMC_FMC_NORMAL_INT_MSK_DFLASH_PROGRAM_DONE_Msk               (0x1UL << FMC_FMC_NORMAL_INT_MSK_DFLASH_PROGRAM_DONE_Pos)
#define FMC_FMC_NORMAL_INT_MSK_EEPROM_COPY_DONE_Pos                  (4U)
#define FMC_FMC_NORMAL_INT_MSK_EEPROM_COPY_DONE_Msk                  (0x1UL << FMC_FMC_NORMAL_INT_MSK_EEPROM_COPY_DONE_Pos)
#define FMC_FMC_NORMAL_INT_MSK_RSV_Pos                               (5U)
#define FMC_FMC_NORMAL_INT_MSK_RSV_Msk                               (0x7ffffffUL << FMC_FMC_NORMAL_INT_MSK_RSV_Pos)

#define FMC_FMC_NORMAL_INT_STAT_PFLASH_ERASE_DONE_Pos                (0U)
#define FMC_FMC_NORMAL_INT_STAT_PFLASH_ERASE_DONE_Msk                (0x1UL << FMC_FMC_NORMAL_INT_STAT_PFLASH_ERASE_DONE_Pos)
#define FMC_FMC_NORMAL_INT_STAT_DFLASH_ERASE_DONE_Pos                (1U)
#define FMC_FMC_NORMAL_INT_STAT_DFLASH_ERASE_DONE_Msk                (0x1UL << FMC_FMC_NORMAL_INT_STAT_DFLASH_ERASE_DONE_Pos)
#define FMC_FMC_NORMAL_INT_STAT_PFLASH_PROGRAM_DONE_Pos              (2U)
#define FMC_FMC_NORMAL_INT_STAT_PFLASH_PROGRAM_DONE_Msk              (0x1UL << FMC_FMC_NORMAL_INT_STAT_PFLASH_PROGRAM_DONE_Pos)
#define FMC_FMC_NORMAL_INT_STAT_DFLASH_PROGRAM_DONE_Pos              (3U)
#define FMC_FMC_NORMAL_INT_STAT_DFLASH_PROGRAM_DONE_Msk              (0x1UL << FMC_FMC_NORMAL_INT_STAT_DFLASH_PROGRAM_DONE_Pos)
#define FMC_FMC_NORMAL_INT_STAT_EEPROM_COPY_DONE_Pos                 (4U)
#define FMC_FMC_NORMAL_INT_STAT_EEPROM_COPY_DONE_Msk                 (0x1UL << FMC_FMC_NORMAL_INT_STAT_EEPROM_COPY_DONE_Pos)
#define FMC_FMC_NORMAL_INT_STAT_RSV_Pos                              (5U)
#define FMC_FMC_NORMAL_INT_STAT_RSV_Msk                              (0x7ffffffUL << FMC_FMC_NORMAL_INT_STAT_RSV_Pos)






#define FMC_FMC_WARNING_INT_RAW_PFLASH_1BIT_ECC_Pos                  (0U)
#define FMC_FMC_WARNING_INT_RAW_PFLASH_1BIT_ECC_Msk                  (0x1UL << FMC_FMC_WARNING_INT_RAW_PFLASH_1BIT_ECC_Pos)
#define FMC_FMC_WARNING_INT_RAW_PFLASH_MULTI_1BIT_ECC_Pos            (1U)
#define FMC_FMC_WARNING_INT_RAW_PFLASH_MULTI_1BIT_ECC_Msk            (0x1UL << FMC_FMC_WARNING_INT_RAW_PFLASH_MULTI_1BIT_ECC_Pos)
#define FMC_FMC_WARNING_INT_RAW_DFLASH_1BIT_ECC_Pos                  (2U)
#define FMC_FMC_WARNING_INT_RAW_DFLASH_1BIT_ECC_Msk                  (0x1UL << FMC_FMC_WARNING_INT_RAW_DFLASH_1BIT_ECC_Pos)
#define FMC_FMC_WARNING_INT_RAW_DFLASH_MULTI_1BIT_ECC_Pos            (3U)
#define FMC_FMC_WARNING_INT_RAW_DFLASH_MULTI_1BIT_ECC_Msk            (0x1UL << FMC_FMC_WARNING_INT_RAW_DFLASH_MULTI_1BIT_ECC_Pos)
#define FMC_FMC_WARNING_INT_RAW_PFLASH_WR_PROTECT_ERR_Pos            (4U)
#define FMC_FMC_WARNING_INT_RAW_PFLASH_WR_PROTECT_ERR_Msk            (0x1UL << FMC_FMC_WARNING_INT_RAW_PFLASH_WR_PROTECT_ERR_Pos)
#define FMC_FMC_WARNING_INT_RAW_DFLASH_WR_PROTECT_ERR_Pos            (5U)
#define FMC_FMC_WARNING_INT_RAW_DFLASH_WR_PROTECT_ERR_Msk            (0x1UL << FMC_FMC_WARNING_INT_RAW_DFLASH_WR_PROTECT_ERR_Pos)
#define FMC_FMC_WARNING_INT_RAW_SRAM_1BIT_ECC_Pos                    (6U)
#define FMC_FMC_WARNING_INT_RAW_SRAM_1BIT_ECC_Msk                    (0x1UL << FMC_FMC_WARNING_INT_RAW_SRAM_1BIT_ECC_Pos)
#define FMC_FMC_WARNING_INT_RAW_RSV_Pos                              (7U)
#define FMC_FMC_WARNING_INT_RAW_RSV_Msk                              (0x1ffffffUL << FMC_FMC_WARNING_INT_RAW_RSV_Pos)

#define FMC_FMC_WARNING_INT_MSK_PFLASH_1BIT_ECC_Pos                  (0U)
#define FMC_FMC_WARNING_INT_MSK_PFLASH_1BIT_ECC_Msk                  (0x1UL << FMC_FMC_WARNING_INT_MSK_PFLASH_1BIT_ECC_Pos)
#define FMC_FMC_WARNING_INT_MSK_PFLASH_MULTI_1BIT_ECC_Pos            (1U)
#define FMC_FMC_WARNING_INT_MSK_PFLASH_MULTI_1BIT_ECC_Msk            (0x1UL << FMC_FMC_WARNING_INT_MSK_PFLASH_MULTI_1BIT_ECC_Pos)
#define FMC_FMC_WARNING_INT_MSK_DFLASH_1BIT_ECC_Pos                  (2U)
#define FMC_FMC_WARNING_INT_MSK_DFLASH_1BIT_ECC_Msk                  (0x1UL << FMC_FMC_WARNING_INT_MSK_DFLASH_1BIT_ECC_Pos)
#define FMC_FMC_WARNING_INT_MSK_DFLASH_MULTI_1BIT_ECC_Pos            (3U)
#define FMC_FMC_WARNING_INT_MSK_DFLASH_MULTI_1BIT_ECC_Msk            (0x1UL << FMC_FMC_WARNING_INT_MSK_DFLASH_MULTI_1BIT_ECC_Pos)
#define FMC_FMC_WARNING_INT_MSK_PFLASH_WR_PROTECT_ERR_Pos            (4U)
#define FMC_FMC_WARNING_INT_MSK_PFLASH_WR_PROTECT_ERR_Msk            (0x1UL << FMC_FMC_WARNING_INT_MSK_PFLASH_WR_PROTECT_ERR_Pos)
#define FMC_FMC_WARNING_INT_MSK_DFLASH_WR_PROTECT_ERR_Pos            (5U)
#define FMC_FMC_WARNING_INT_MSK_DFLASH_WR_PROTECT_ERR_Msk            (0x1UL << FMC_FMC_WARNING_INT_MSK_DFLASH_WR_PROTECT_ERR_Pos)
#define FMC_FMC_WARNING_INT_MSK_SRAM_1BIT_ECC_Pos                    (6U)
#define FMC_FMC_WARNING_INT_MSK_SRAM_1BIT_ECC_Msk                    (0x1UL << FMC_FMC_WARNING_INT_MSK_SRAM_1BIT_ECC_Pos)
#define FMC_FMC_WARNING_INT_MSK_RSV_Pos                              (7U)
#define FMC_FMC_WARNING_INT_MSK_RSV_Msk                              (0x1ffffffUL << FMC_FMC_WARNING_INT_MSK_RSV_Pos)

#define FMC_FMC_WARNING_INT_STAT_PFLASH_1BIT_ECC_Pos                 (0U)
#define FMC_FMC_WARNING_INT_STAT_PFLASH_1BIT_ECC_Msk                 (0x1UL << FMC_FMC_WARNING_INT_STAT_PFLASH_1BIT_ECC_Pos)
#define FMC_FMC_WARNING_INT_STAT_PFLASH_MULTI_1BIT_ECC_Pos           (1U)
#define FMC_FMC_WARNING_INT_STAT_PFLASH_MULTI_1BIT_ECC_Msk           (0x1UL << FMC_FMC_WARNING_INT_STAT_PFLASH_MULTI_1BIT_ECC_Pos)
#define FMC_FMC_WARNING_INT_STAT_DFLASH_1BIT_ECC_Pos                 (2U)
#define FMC_FMC_WARNING_INT_STAT_DFLASH_1BIT_ECC_Msk                 (0x1UL << FMC_FMC_WARNING_INT_STAT_DFLASH_1BIT_ECC_Pos)
#define FMC_FMC_WARNING_INT_STAT_DFLASH_MULTI_1BIT_ECC_Pos           (3U)
#define FMC_FMC_WARNING_INT_STAT_DFLASH_MULTI_1BIT_ECC_Msk           (0x1UL << FMC_FMC_WARNING_INT_STAT_DFLASH_MULTI_1BIT_ECC_Pos)
#define FMC_FMC_WARNING_INT_STAT_PFLASH_WR_PROTECT_ERR_Pos           (4U)
#define FMC_FMC_WARNING_INT_STAT_PFLASH_WR_PROTECT_ERR_Msk           (0x1UL << FMC_FMC_WARNING_INT_STAT_PFLASH_WR_PROTECT_ERR_Pos)
#define FMC_FMC_WARNING_INT_STAT_DFLASH_WR_PROTECT_ERR_Pos           (5U)
#define FMC_FMC_WARNING_INT_STAT_DFLASH_WR_PROTECT_ERR_Msk           (0x1UL << FMC_FMC_WARNING_INT_STAT_DFLASH_WR_PROTECT_ERR_Pos)
#define FMC_FMC_WARNING_INT_STAT_SRAM_1BIT_ECC_Pos                   (6U)
#define FMC_FMC_WARNING_INT_STAT_SRAM_1BIT_ECC_Msk                   (0x1UL << FMC_FMC_WARNING_INT_STAT_SRAM_1BIT_ECC_Pos)
#define FMC_FMC_WARNING_INT_STAT_RSV_Pos                             (7U)
#define FMC_FMC_WARNING_INT_STAT_RSV_Msk                             (0x1ffffffUL << FMC_FMC_WARNING_INT_STAT_RSV_Pos)






#define FMC_FMC_WARNING_PFLASH_1BIT_ECC_ADDR_SECTOR_SEL_Pos          (0U)
#define FMC_FMC_WARNING_PFLASH_1BIT_ECC_ADDR_SECTOR_SEL_Msk          (0x3UL << FMC_FMC_WARNING_PFLASH_1BIT_ECC_ADDR_SECTOR_SEL_Pos)
#define FMC_FMC_WARNING_PFLASH_1BIT_ECC_ADDR_REDUND_SEL_Pos          (2U)
#define FMC_FMC_WARNING_PFLASH_1BIT_ECC_ADDR_REDUND_SEL_Msk          (0x1UL << FMC_FMC_WARNING_PFLASH_1BIT_ECC_ADDR_REDUND_SEL_Pos)
#define FMC_FMC_WARNING_PFLASH_1BIT_ECC_ADDR_RSV1_Pos                (3U)
#define FMC_FMC_WARNING_PFLASH_1BIT_ECC_ADDR_RSV1_Msk                (0x1UL << FMC_FMC_WARNING_PFLASH_1BIT_ECC_ADDR_RSV1_Pos)
#define FMC_FMC_WARNING_PFLASH_1BIT_ECC_ADDR_FLASH_ADDR_Pos          (4U)
#define FMC_FMC_WARNING_PFLASH_1BIT_ECC_ADDR_FLASH_ADDR_Msk          (0xfffffUL << FMC_FMC_WARNING_PFLASH_1BIT_ECC_ADDR_FLASH_ADDR_Pos)
#define FMC_FMC_WARNING_PFLASH_1BIT_ECC_ADDR_RSV0_Pos                (24U)
#define FMC_FMC_WARNING_PFLASH_1BIT_ECC_ADDR_RSV0_Msk                (0xffUL << FMC_FMC_WARNING_PFLASH_1BIT_ECC_ADDR_RSV0_Pos)

#define FMC_FMC_WARNING_PFLASH_1BIT_ECC_CNT__1BIT_ECC_CNT_Pos        (0U)
#define FMC_FMC_WARNING_PFLASH_1BIT_ECC_CNT__1BIT_ECC_CNT_Msk        (0xffffUL << FMC_FMC_WARNING_PFLASH_1BIT_ECC_CNT__1BIT_ECC_CNT_Pos)
#define FMC_FMC_WARNING_PFLASH_1BIT_ECC_CNT_RSV_Pos                  (16U)
#define FMC_FMC_WARNING_PFLASH_1BIT_ECC_CNT_RSV_Msk                  (0xffffUL << FMC_FMC_WARNING_PFLASH_1BIT_ECC_CNT_RSV_Pos)



#define FMC_FMC_WARNING_DFLASH_1BIT_ECC_ADDR_SECTOR_SEL_Pos          (0U)
#define FMC_FMC_WARNING_DFLASH_1BIT_ECC_ADDR_SECTOR_SEL_Msk          (0x3UL << FMC_FMC_WARNING_DFLASH_1BIT_ECC_ADDR_SECTOR_SEL_Pos)
#define FMC_FMC_WARNING_DFLASH_1BIT_ECC_ADDR_REDUND_SEL_Pos          (2U)
#define FMC_FMC_WARNING_DFLASH_1BIT_ECC_ADDR_REDUND_SEL_Msk          (0x1UL << FMC_FMC_WARNING_DFLASH_1BIT_ECC_ADDR_REDUND_SEL_Pos)
#define FMC_FMC_WARNING_DFLASH_1BIT_ECC_ADDR_RSV1_Pos                (3U)
#define FMC_FMC_WARNING_DFLASH_1BIT_ECC_ADDR_RSV1_Msk                (0x1UL << FMC_FMC_WARNING_DFLASH_1BIT_ECC_ADDR_RSV1_Pos)
#define FMC_FMC_WARNING_DFLASH_1BIT_ECC_ADDR_FLASH_ADDR_Pos          (4U)
#define FMC_FMC_WARNING_DFLASH_1BIT_ECC_ADDR_FLASH_ADDR_Msk          (0xfffffUL << FMC_FMC_WARNING_DFLASH_1BIT_ECC_ADDR_FLASH_ADDR_Pos)
#define FMC_FMC_WARNING_DFLASH_1BIT_ECC_ADDR_RSV0_Pos                (24U)
#define FMC_FMC_WARNING_DFLASH_1BIT_ECC_ADDR_RSV0_Msk                (0xffUL << FMC_FMC_WARNING_DFLASH_1BIT_ECC_ADDR_RSV0_Pos)

#define FMC_FMC_WARNING_DFLASH_1BIT_ECC_CNT__1BIT_ECC_CNT_Pos        (0U)
#define FMC_FMC_WARNING_DFLASH_1BIT_ECC_CNT__1BIT_ECC_CNT_Msk        (0xffffUL << FMC_FMC_WARNING_DFLASH_1BIT_ECC_CNT__1BIT_ECC_CNT_Pos)
#define FMC_FMC_WARNING_DFLASH_1BIT_ECC_CNT_RSV_Pos                  (16U)
#define FMC_FMC_WARNING_DFLASH_1BIT_ECC_CNT_RSV_Msk                  (0xffffUL << FMC_FMC_WARNING_DFLASH_1BIT_ECC_CNT_RSV_Pos)



#define FMC_FMC_WARNING_SRAM_1BIT_ECC_ADDR_FLASH_ADDR_Pos            (0U)
#define FMC_FMC_WARNING_SRAM_1BIT_ECC_ADDR_FLASH_ADDR_Msk            (0x1ffffUL << FMC_FMC_WARNING_SRAM_1BIT_ECC_ADDR_FLASH_ADDR_Pos)
#define FMC_FMC_WARNING_SRAM_1BIT_ECC_ADDR_RSV_Pos                   (17U)
#define FMC_FMC_WARNING_SRAM_1BIT_ECC_ADDR_RSV_Msk                   (0x7fffUL << FMC_FMC_WARNING_SRAM_1BIT_ECC_ADDR_RSV_Pos)

#define FMC_FMC_WARNING_SRAM_1BIT_ECC_CNT__1BIT_ECC_CNT_Pos          (0U)
#define FMC_FMC_WARNING_SRAM_1BIT_ECC_CNT__1BIT_ECC_CNT_Msk          (0xffffUL << FMC_FMC_WARNING_SRAM_1BIT_ECC_CNT__1BIT_ECC_CNT_Pos)
#define FMC_FMC_WARNING_SRAM_1BIT_ECC_CNT_RSV_Pos                    (16U)
#define FMC_FMC_WARNING_SRAM_1BIT_ECC_CNT_RSV_Msk                    (0xffffUL << FMC_FMC_WARNING_SRAM_1BIT_ECC_CNT_RSV_Pos)



#define FMC_FMC_WARNING_PFLASH_WR_PROTECT_STAT_SECTOR_SEL_Pos        (0U)
#define FMC_FMC_WARNING_PFLASH_WR_PROTECT_STAT_SECTOR_SEL_Msk        (0x3UL << FMC_FMC_WARNING_PFLASH_WR_PROTECT_STAT_SECTOR_SEL_Pos)
#define FMC_FMC_WARNING_PFLASH_WR_PROTECT_STAT_REDUND_SEL_Pos        (2U)
#define FMC_FMC_WARNING_PFLASH_WR_PROTECT_STAT_REDUND_SEL_Msk        (0x1UL << FMC_FMC_WARNING_PFLASH_WR_PROTECT_STAT_REDUND_SEL_Pos)
#define FMC_FMC_WARNING_PFLASH_WR_PROTECT_STAT_AHB_SEL_Pos           (3U)
#define FMC_FMC_WARNING_PFLASH_WR_PROTECT_STAT_AHB_SEL_Msk           (0x1UL << FMC_FMC_WARNING_PFLASH_WR_PROTECT_STAT_AHB_SEL_Pos)
#define FMC_FMC_WARNING_PFLASH_WR_PROTECT_STAT_FLASH_ADDR_Pos        (4U)
#define FMC_FMC_WARNING_PFLASH_WR_PROTECT_STAT_FLASH_ADDR_Msk        (0xfffffUL << FMC_FMC_WARNING_PFLASH_WR_PROTECT_STAT_FLASH_ADDR_Pos)
#define FMC_FMC_WARNING_PFLASH_WR_PROTECT_STAT_ERASE_SEL_Pos         (24U)
#define FMC_FMC_WARNING_PFLASH_WR_PROTECT_STAT_ERASE_SEL_Msk         (0x1UL << FMC_FMC_WARNING_PFLASH_WR_PROTECT_STAT_ERASE_SEL_Pos)
#define FMC_FMC_WARNING_PFLASH_WR_PROTECT_STAT_RSV_Pos               (25U)
#define FMC_FMC_WARNING_PFLASH_WR_PROTECT_STAT_RSV_Msk               (0x7fUL << FMC_FMC_WARNING_PFLASH_WR_PROTECT_STAT_RSV_Pos)

#define FMC_FMC_WARNING_DFLASH_WR_PROTECT_STAT_SECTOR_SEL_Pos        (0U)
#define FMC_FMC_WARNING_DFLASH_WR_PROTECT_STAT_SECTOR_SEL_Msk        (0x3UL << FMC_FMC_WARNING_DFLASH_WR_PROTECT_STAT_SECTOR_SEL_Pos)
#define FMC_FMC_WARNING_DFLASH_WR_PROTECT_STAT_REDUND_SEL_Pos        (2U)
#define FMC_FMC_WARNING_DFLASH_WR_PROTECT_STAT_REDUND_SEL_Msk        (0x1UL << FMC_FMC_WARNING_DFLASH_WR_PROTECT_STAT_REDUND_SEL_Pos)
#define FMC_FMC_WARNING_DFLASH_WR_PROTECT_STAT_AHB_SEL_Pos           (3U)
#define FMC_FMC_WARNING_DFLASH_WR_PROTECT_STAT_AHB_SEL_Msk           (0x1UL << FMC_FMC_WARNING_DFLASH_WR_PROTECT_STAT_AHB_SEL_Pos)
#define FMC_FMC_WARNING_DFLASH_WR_PROTECT_STAT_FLASH_ADDR_Pos        (4U)
#define FMC_FMC_WARNING_DFLASH_WR_PROTECT_STAT_FLASH_ADDR_Msk        (0xfffffUL << FMC_FMC_WARNING_DFLASH_WR_PROTECT_STAT_FLASH_ADDR_Pos)
#define FMC_FMC_WARNING_DFLASH_WR_PROTECT_STAT_ERASE_SEL_Pos         (24U)
#define FMC_FMC_WARNING_DFLASH_WR_PROTECT_STAT_ERASE_SEL_Msk         (0x1UL << FMC_FMC_WARNING_DFLASH_WR_PROTECT_STAT_ERASE_SEL_Pos)
#define FMC_FMC_WARNING_DFLASH_WR_PROTECT_STAT_RSV_Pos               (25U)
#define FMC_FMC_WARNING_DFLASH_WR_PROTECT_STAT_RSV_Msk               (0x7fUL << FMC_FMC_WARNING_DFLASH_WR_PROTECT_STAT_RSV_Pos)



#define FMC_FMC_ERROR_INT_RAW_PFLASH_2BIT_ECC_Pos                    (0U)
#define FMC_FMC_ERROR_INT_RAW_PFLASH_2BIT_ECC_Msk                    (0x1UL << FMC_FMC_ERROR_INT_RAW_PFLASH_2BIT_ECC_Pos)
#define FMC_FMC_ERROR_INT_RAW_DFLASH_2BIT_ECC_Pos                    (1U)
#define FMC_FMC_ERROR_INT_RAW_DFLASH_2BIT_ECC_Msk                    (0x1UL << FMC_FMC_ERROR_INT_RAW_DFLASH_2BIT_ECC_Pos)
#define FMC_FMC_ERROR_INT_RAW_SRAM_2BIT_ECC_Pos                      (2U)
#define FMC_FMC_ERROR_INT_RAW_SRAM_2BIT_ECC_Msk                      (0x1UL << FMC_FMC_ERROR_INT_RAW_SRAM_2BIT_ECC_Pos)
#define FMC_FMC_ERROR_INT_RAW_OTP_CMP_ERR_Pos                        (3U)
#define FMC_FMC_ERROR_INT_RAW_OTP_CMP_ERR_Msk                        (0x1UL << FMC_FMC_ERROR_INT_RAW_OTP_CMP_ERR_Pos)
#define FMC_FMC_ERROR_INT_RAW_OTP_ECC_ERR_Pos                        (4U)
#define FMC_FMC_ERROR_INT_RAW_OTP_ECC_ERR_Msk                        (0x1UL << FMC_FMC_ERROR_INT_RAW_OTP_ECC_ERR_Pos)
#define FMC_FMC_ERROR_INT_RAW_RSV_Pos                                (5U)
#define FMC_FMC_ERROR_INT_RAW_RSV_Msk                                (0x7ffffffUL << FMC_FMC_ERROR_INT_RAW_RSV_Pos)




#define FMC_FMC_ERROR_PFLASH_2BIT_ECC_STAT_SECTOR_SEL_Pos            (0U)
#define FMC_FMC_ERROR_PFLASH_2BIT_ECC_STAT_SECTOR_SEL_Msk            (0x3UL << FMC_FMC_ERROR_PFLASH_2BIT_ECC_STAT_SECTOR_SEL_Pos)
#define FMC_FMC_ERROR_PFLASH_2BIT_ECC_STAT_REDUND_SEL_Pos            (2U)
#define FMC_FMC_ERROR_PFLASH_2BIT_ECC_STAT_REDUND_SEL_Msk            (0x1UL << FMC_FMC_ERROR_PFLASH_2BIT_ECC_STAT_REDUND_SEL_Pos)
#define FMC_FMC_ERROR_PFLASH_2BIT_ECC_STAT_RSV1_Pos                  (3U)
#define FMC_FMC_ERROR_PFLASH_2BIT_ECC_STAT_RSV1_Msk                  (0x1UL << FMC_FMC_ERROR_PFLASH_2BIT_ECC_STAT_RSV1_Pos)
#define FMC_FMC_ERROR_PFLASH_2BIT_ECC_STAT_FLASH_ADDR_Pos            (4U)
#define FMC_FMC_ERROR_PFLASH_2BIT_ECC_STAT_FLASH_ADDR_Msk            (0xfffffUL << FMC_FMC_ERROR_PFLASH_2BIT_ECC_STAT_FLASH_ADDR_Pos)
#define FMC_FMC_ERROR_PFLASH_2BIT_ECC_STAT_RSV0_Pos                  (28U)
#define FMC_FMC_ERROR_PFLASH_2BIT_ECC_STAT_RSV0_Msk                  (0xfUL << FMC_FMC_ERROR_PFLASH_2BIT_ECC_STAT_RSV0_Pos)

#define FMC_FMC_ERROR_DFLASH_2BIT_ECC_STAT_SECTOR_SEL_Pos            (0U)
#define FMC_FMC_ERROR_DFLASH_2BIT_ECC_STAT_SECTOR_SEL_Msk            (0x3UL << FMC_FMC_ERROR_DFLASH_2BIT_ECC_STAT_SECTOR_SEL_Pos)
#define FMC_FMC_ERROR_DFLASH_2BIT_ECC_STAT_REDUND_SEL_Pos            (2U)
#define FMC_FMC_ERROR_DFLASH_2BIT_ECC_STAT_REDUND_SEL_Msk            (0x1UL << FMC_FMC_ERROR_DFLASH_2BIT_ECC_STAT_REDUND_SEL_Pos)
#define FMC_FMC_ERROR_DFLASH_2BIT_ECC_STAT_RSV1_Pos                  (3U)
#define FMC_FMC_ERROR_DFLASH_2BIT_ECC_STAT_RSV1_Msk                  (0x1UL << FMC_FMC_ERROR_DFLASH_2BIT_ECC_STAT_RSV1_Pos)
#define FMC_FMC_ERROR_DFLASH_2BIT_ECC_STAT_FLASH_ADDR_Pos            (4U)
#define FMC_FMC_ERROR_DFLASH_2BIT_ECC_STAT_FLASH_ADDR_Msk            (0xfffffUL << FMC_FMC_ERROR_DFLASH_2BIT_ECC_STAT_FLASH_ADDR_Pos)
#define FMC_FMC_ERROR_DFLASH_2BIT_ECC_STAT_RSV0_Pos                  (28U)
#define FMC_FMC_ERROR_DFLASH_2BIT_ECC_STAT_RSV0_Msk                  (0xfUL << FMC_FMC_ERROR_DFLASH_2BIT_ECC_STAT_RSV0_Pos)

#define FMC_FMC_ERROR_SRAM_2BIT_ECC_STAT_FLASH_ADDR_Pos              (0U)
#define FMC_FMC_ERROR_SRAM_2BIT_ECC_STAT_FLASH_ADDR_Msk              (0x1ffffUL << FMC_FMC_ERROR_SRAM_2BIT_ECC_STAT_FLASH_ADDR_Pos)
#define FMC_FMC_ERROR_SRAM_2BIT_ECC_STAT_RSV_Pos                     (17U)
#define FMC_FMC_ERROR_SRAM_2BIT_ECC_STAT_RSV_Msk                     (0x7fffUL << FMC_FMC_ERROR_SRAM_2BIT_ECC_STAT_RSV_Pos)


















#define FMC_FMC_EEPROM_SECTOR_CFG_SECTOR_BEGIN0_Pos                  (0U)
#define FMC_FMC_EEPROM_SECTOR_CFG_SECTOR_BEGIN0_Msk                  (0xffUL << FMC_FMC_EEPROM_SECTOR_CFG_SECTOR_BEGIN0_Pos)
#define FMC_FMC_EEPROM_SECTOR_CFG_SECTOR_NUM0_Pos                    (8U)
#define FMC_FMC_EEPROM_SECTOR_CFG_SECTOR_NUM0_Msk                    (0xffUL << FMC_FMC_EEPROM_SECTOR_CFG_SECTOR_NUM0_Pos)
#define FMC_FMC_EEPROM_SECTOR_CFG_SECTOR_BEGIN1_Pos                  (16U)
#define FMC_FMC_EEPROM_SECTOR_CFG_SECTOR_BEGIN1_Msk                  (0xffUL << FMC_FMC_EEPROM_SECTOR_CFG_SECTOR_BEGIN1_Pos)
#define FMC_FMC_EEPROM_SECTOR_CFG_SECTOR_NUM1_Pos                    (24U)
#define FMC_FMC_EEPROM_SECTOR_CFG_SECTOR_NUM1_Msk                    (0xffUL << FMC_FMC_EEPROM_SECTOR_CFG_SECTOR_NUM1_Pos)




#define FMC_FMC_EEPROM_COPY_CMD_CFG_SECTOR_BEGIN0_Pos                (0U)
#define FMC_FMC_EEPROM_COPY_CMD_CFG_SECTOR_BEGIN0_Msk                (0xffUL << FMC_FMC_EEPROM_COPY_CMD_CFG_SECTOR_BEGIN0_Pos)
#define FMC_FMC_EEPROM_COPY_CMD_CFG_SECTOR_NUM0_Pos                  (8U)
#define FMC_FMC_EEPROM_COPY_CMD_CFG_SECTOR_NUM0_Msk                  (0xffUL << FMC_FMC_EEPROM_COPY_CMD_CFG_SECTOR_NUM0_Pos)
#define FMC_FMC_EEPROM_COPY_CMD_CFG_SECTOR_BEGIN1_Pos                (16U)
#define FMC_FMC_EEPROM_COPY_CMD_CFG_SECTOR_BEGIN1_Msk                (0xffUL << FMC_FMC_EEPROM_COPY_CMD_CFG_SECTOR_BEGIN1_Pos)
#define FMC_FMC_EEPROM_COPY_CMD_CFG_SECTOR_NUM1_Pos                  (24U)
#define FMC_FMC_EEPROM_COPY_CMD_CFG_SECTOR_NUM1_Msk                  (0xffUL << FMC_FMC_EEPROM_COPY_CMD_CFG_SECTOR_NUM1_Pos)

#define FMC_FMC_EEPROM_COPY_CMD_CTRL_SECTOR0_EN_Pos                  (0U)
#define FMC_FMC_EEPROM_COPY_CMD_CTRL_SECTOR0_EN_Msk                  (0x1UL << FMC_FMC_EEPROM_COPY_CMD_CTRL_SECTOR0_EN_Pos)
#define FMC_FMC_EEPROM_COPY_CMD_CTRL_SECTOR1_EN_Pos                  (1U)
#define FMC_FMC_EEPROM_COPY_CMD_CTRL_SECTOR1_EN_Msk                  (0x1UL << FMC_FMC_EEPROM_COPY_CMD_CTRL_SECTOR1_EN_Pos)
#define FMC_FMC_EEPROM_COPY_CMD_CTRL_RSV_Pos                         (2U)
#define FMC_FMC_EEPROM_COPY_CMD_CTRL_RSV_Msk                         (0x3fffffffUL << FMC_FMC_EEPROM_COPY_CMD_CTRL_RSV_Pos)

#define FMC_FMC_EEPROM_COPY_CMD_STATUS_SECTOR0_COPY_DONE_Pos         (0U)
#define FMC_FMC_EEPROM_COPY_CMD_STATUS_SECTOR0_COPY_DONE_Msk         (0x1UL << FMC_FMC_EEPROM_COPY_CMD_STATUS_SECTOR0_COPY_DONE_Pos)
#define FMC_FMC_EEPROM_COPY_CMD_STATUS_SECTOR1_COPY_DONE_Pos         (1U)
#define FMC_FMC_EEPROM_COPY_CMD_STATUS_SECTOR1_COPY_DONE_Msk         (0x1UL << FMC_FMC_EEPROM_COPY_CMD_STATUS_SECTOR1_COPY_DONE_Pos)
#define FMC_FMC_EEPROM_COPY_CMD_STATUS_RSV_Pos                       (2U)
#define FMC_FMC_EEPROM_COPY_CMD_STATUS_RSV_Msk                       (0x3fffffffUL << FMC_FMC_EEPROM_COPY_CMD_STATUS_RSV_Pos)


#define FMC_FMC_EEPROM_WRITE_CMD_CFG_SRAM_ADDR_Pos                   (0U)
#define FMC_FMC_EEPROM_WRITE_CMD_CFG_SRAM_ADDR_Msk                   (0xfffUL << FMC_FMC_EEPROM_WRITE_CMD_CFG_SRAM_ADDR_Pos)
#define FMC_FMC_EEPROM_WRITE_CMD_CFG_DB_LENGTH_Pos                   (12U)
#define FMC_FMC_EEPROM_WRITE_CMD_CFG_DB_LENGTH_Msk                   (0xfUL << FMC_FMC_EEPROM_WRITE_CMD_CFG_DB_LENGTH_Pos)
#define FMC_FMC_EEPROM_WRITE_CMD_CFG_FLASH_ADDR_Pos                  (16U)
#define FMC_FMC_EEPROM_WRITE_CMD_CFG_FLASH_ADDR_Msk                  (0xffffUL << FMC_FMC_EEPROM_WRITE_CMD_CFG_FLASH_ADDR_Pos)

#define FMC_FMC_EEPROM_WRITE_CMD_STATUS_WRITE_DONE_Pos               (0U)
#define FMC_FMC_EEPROM_WRITE_CMD_STATUS_WRITE_DONE_Msk               (0x1UL << FMC_FMC_EEPROM_WRITE_CMD_STATUS_WRITE_DONE_Pos)
#define FMC_FMC_EEPROM_WRITE_CMD_STATUS_STATUS_Pos                   (1U)
#define FMC_FMC_EEPROM_WRITE_CMD_STATUS_STATUS_Msk                   (0x1UL << FMC_FMC_EEPROM_WRITE_CMD_STATUS_STATUS_Pos)
#define FMC_FMC_EEPROM_WRITE_CMD_STATUS_RSV_Pos                      (2U)
#define FMC_FMC_EEPROM_WRITE_CMD_STATUS_RSV_Msk                      (0x3fffffffUL << FMC_FMC_EEPROM_WRITE_CMD_STATUS_RSV_Pos)



#define FMC_FMC_PFLASH_AHB_ACES_CNT_CNT_Pos                          (0U)
#define FMC_FMC_PFLASH_AHB_ACES_CNT_CNT_Msk                          (0xffffffffUL << FMC_FMC_PFLASH_AHB_ACES_CNT_CNT_Pos)

#define FMC_FMC_PFLASH_AHB_PREF_CNT_CNT_Pos                          (0U)
#define FMC_FMC_PFLASH_AHB_PREF_CNT_CNT_Msk                          (0xffffffffUL << FMC_FMC_PFLASH_AHB_PREF_CNT_CNT_Pos)

#define FMC_FMC_PFLASH_AHB_CONFLICT_CNT_CNT_Pos                      (0U)
#define FMC_FMC_PFLASH_AHB_CONFLICT_CNT_CNT_Msk                      (0xffffffffUL << FMC_FMC_PFLASH_AHB_CONFLICT_CNT_CNT_Pos)


#define FMC_FMC_DFLASH_AHB_ACES_CNT_CNT_Pos                          (0U)
#define FMC_FMC_DFLASH_AHB_ACES_CNT_CNT_Msk                          (0xffffffffUL << FMC_FMC_DFLASH_AHB_ACES_CNT_CNT_Pos)

#define FMC_FMC_DFLASH_AHB_PREF_CNT_CNT_Pos                          (0U)
#define FMC_FMC_DFLASH_AHB_PREF_CNT_CNT_Msk                          (0xffffffffUL << FMC_FMC_DFLASH_AHB_PREF_CNT_CNT_Pos)

#define FMC_FMC_DFLASH_AHB_CONFLICT_CNT_CNT_Pos                      (0U)
#define FMC_FMC_DFLASH_AHB_CONFLICT_CNT_CNT_Msk                      (0xffffffffUL << FMC_FMC_DFLASH_AHB_CONFLICT_CNT_CNT_Pos)
// clang-format on

#endif // __FMC_REGISTER_H__
