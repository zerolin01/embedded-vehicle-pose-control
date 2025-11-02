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
 * @file supertmr_common.h
 * @brief SUPERTMR common driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __SUPERTMR_COMMON_H__
#define __SUPERTMR_COMMON_H__

#include "callbacks.h"
#include "clock_driver.h"
#include "osal.h"
#include "status.h"
#include "supertmr_register.h"

#include <stddef.h>

/**
 *  @brief Table of base addresses for SUPERTMR instances.
 */
extern supertmr_type_t *const g_pstSupertmrBase[SUPERTMR_INSTANCE_COUNT];

/**
 * @brief  Interrupt vectors for the SUPERTMR peripheral.
 */
extern const IRQn_Type g_euSupertmrIrqId[SUPERTMR_INSTANCE_COUNT][SUPERTMR_CHANNEL_COUNT];
extern const IRQn_Type g_euSupertmrFaultIrqId[SUPERTMR_INSTANCE_COUNT];
extern const IRQn_Type g_euSupertmrOverflowIrqId[SUPERTMR_INSTANCE_COUNT];
extern const IRQn_Type g_euSupertmrReloadIrqId[SUPERTMR_INSTANCE_COUNT];

/**
 * @brief SUPERTMR_SC - Read and modify and write to Status And Control (RW).
 */
#define SUPERTMR_RMW_SC(base, mask, value) (((base)->SC) = ((((base)->SC) & ~(mask)) | (value)))

/**
 * @brief SUPERTMR_CNT - Read and modify and write to Counter (RW).
 */
#define SUPERTMR_RMW_CNT(base, mask, value) (((base)->CNT) = ((((base)->CNT) & ~(mask)) | (value)))

/**
 * @brief SUPERTMR_MOD - Read and modify and write Modulo (RW).
 */
#define SUPERTMR_RMW_MOD(base, mask, value) (((base)->MOD) = ((((base)->MOD) & ~(mask)) | (value)))

/**
 * @brief SUPERTMR_CNTIN - Read and modify and write Counter Initial Value (RW).
 */
#define SUPERTMR_RMW_CNTIN(base, mask, value) (((base)->CNTIN) = ((((base)->CNTIN) & ~(mask)) | (value)))

/**
 * @brief SUPERTMR_STATUS - Read and modify and write Capture And Compare Status (RW).
 */
#define SUPERTMR_RMW_STATUS(base, mask, value) (((base)->STATUS) = ((((base)->STATUS) & ~(mask)) | (value)))

/**
 * @brief SUPERTMR_MODE -  Read and modify and write Counter Features Mode Selection (RW).
 */
#define SUPERTMR_RMW_MODE(base, mask, value) (((base)->MODE) = ((((base)->MODE) & ~(mask)) | (value)))

/**
 * @brief SUPERTMR_CnSCV -  Read and modify and write Channel (n) Status And Control (RW).
 */
#define SUPERTMR_RMW_CnSCV_REG(base, channel, mask, value) \
    (((base)->CONTROLS[channel].CnSC) = ((((base)->CONTROLS[channel].CnSC) & ~(mask)) | (value)))

/**
 * @brief SUPERTMR_DEADTIME - Read and modify and write Dead-time Insertion Control (RW).
 */
#define SUPERTMR_RMW_DEADTIME(base, mask, value) (((base)->DEADTIME) = ((((base)->DEADTIME) & ~(mask)) | (value)))

/**
 * @brief SUPERTMR_EXTTRIG - Read and modify and write External Trigger Control (RW).
 */
#define SUPERTMR_RMW_EXTTRIG_REG(base, mask, value) (((base)->EXTTRIG) = ((((base)->EXTTRIG) & ~(mask)) | (value)))

/**
 * @brief SUPERTMR_FLTCTRL -  Read and modify and write Fault Control (RW).
 */
#define SUPERTMR_RMW_FLTCTRL(base, mask, value) (((base)->FLTCTRL) = ((((base)->FLTCTRL) & ~(mask)) | (value)))

/**
 * @brief SUPERTMR_FMS -  Read and modify and write Fault Mode Status (RW).
 */
#define SUPERTMR_RMW_FMS(base, mask, value) (((base)->FMS) = ((((base)->FMS) & ~(mask)) | (value)))

/**
 * @brief SUPERTMR_CONF -  Read and modify and write Configuration (RW).
 */
#define SUPERTMR_RMW_CONF(base, mask, value) (((base)->CONF) = ((((base)->CONF) & ~(mask)) | (value)))

/**
 * @brief POL -  Read and modify and write Polarity (RW).
 */
#define SUPERTMR_RMW_POL(base, mask, value) (((base)->POL) = ((((base)->POL) & ~(mask)) | (value)))

/**
 * @brief FILTER -  Read and modify and write Filter (RW).
 */
#define SUPERTMR_RMW_FILTER(base, mask, value) (((base)->FILTER) = ((((base)->FILTER) & ~(mask)) | (value)))

/**
 * @brief SYNC -  Read and modify and write Synchronization (RW).
 */
#define SUPERTMR_RMW_SYNC(base, mask, value) (((base)->SYNC) = ((((base)->SYNC) & ~(mask)) | (value)))

/**
 * @brief QDCTRL -  Read and modify and write Quadrature Decoder Control And Status (RW).
 */
#define SUPERTMR_RMW_QDCTRL(base, mask, value) (((base)->QDCTRL) = ((((base)->QDCTRL) & ~(mask)) | (value)))

/**
 * @brief SUPERTMR_PAIR0DEADTIME - Read and modify and write Dead-time Insertion Control for the pair 0 (RW).
 */
#define SUPERTMR_RMW_PAIR0DEADTIME(base, mask, value) \
    (((base)->PAIR0DEADTIME) = ((((base)->PAIR0DEADTIME) & ~(mask)) | (value)))

/**
 * @brief SUPERTMR_PAIR1DEADTIME - Read and modify and write Dead-time Insertion Control for the pair 1 (RW).
 */
#define SUPERTMR_RMW_PAIR1DEADTIME(base, mask, value) \
    (((base)->PAIR1DEADTIME) = ((((base)->PAIR1DEADTIME) & ~(mask)) | (value)))

/**
 * @brief SUPERTMR_PAIR2DEADTIME - Read and modify and write Dead-time Insertion Control for the pair 2 (RW).
 */
#define SUPERTMR_RMW_PAIR2DEADTIME(base, mask, value) \
    (((base)->PAIR2DEADTIME) = ((((base)->PAIR2DEADTIME) & ~(mask)) | (value)))

/**
 * @brief SUPERTMR_PAIR3DEADTIME - Read and modify and write Dead-time Insertion Control for the pair 3 (RW).
 */
#define SUPERTMR_RMW_PAIR3DEADTIME(base, mask, value) \
    (((base)->PAIR3DEADTIME) = ((((base)->PAIR3DEADTIME) & ~(mask)) | (value)))

/**
 * @brief SUPERTMR_MOD_MIRROR - Read and modify and write mirror of modulo value for the SUPERTMR counter (RW).
 */
#define SUPERTMR_RMW_MOD_MIRROR(base, mask, value) (((base)->MOD_MIRROR) = ((((base)->MOD_MIRROR) & ~(mask)) | (value)))

/**
 * @brief SUPERTMR_CnV_MIRROR -  Read and modify and write mirror of channel (n) match value (RW).
 */
#define SUPERTMR_RMW_CnV_MIRROR(base, channel, mask, value) \
    (((base)->CV_MIRROR[channel]) = ((((base)->CV_MIRROR[channel]) & ~(mask)) | (value)))

/**
 * @brief Channel number for CHAN0.
 */
#define CHAN0_IDX (0U)
/**
 * @brief Channel number for CHAN1.
 */
#define CHAN1_IDX (1U)
/**
 * @brief Channel number for CHAN2.
 */
#define CHAN2_IDX (2U)
/**
 * @brief Channel number for CHAN3.
 */
#define CHAN3_IDX (3U)
/**
 * @brief Channel number for CHAN4.
 */
#define CHAN4_IDX (4U)
/**
 * @brief Channel number for CHAN5.
 */
#define CHAN5_IDX (5U)
/**
 * @brief Channel number for CHAN6.
 */
#define CHAN6_IDX (6U)
/**
 * @brief Channel number for CHAN7.
 */
#define CHAN7_IDX (7U)

/**
 * @brief SuperTimer operation mode.
 */
typedef enum {
    SUPERTMR_MODE_NOT_INITIALIZED                    = 0x00U, /**< The driver is not initialized. */
    SUPERTMR_MODE_INPUT_CAPTURE                      = 0x01U, /**< Input capture. */
    SUPERTMR_MODE_OUTPUT_COMPARE                     = 0x02U, /**< Output compare. */
    SUPERTMR_MODE_EDGE_ALIGNED_PWM                   = 0x03U, /**< Edge aligned PWM. */
    SUPERTMR_MODE_CEN_ALIGNED_PWM                    = 0x04U, /**< Center aligned PWM. */
    SUPERTMR_MODE_QUADRATURE_DECODER                 = 0x05U, /**< Quadrature decoder. */
    SUPERTMR_MODE_UP_TIMER                           = 0x06U, /**< Timer with up counter. */
    SUPERTMR_MODE_UP_DOWN_TIMER                      = 0x07U, /**< timer with up-down counter. */
    SUPERTMR_MODE_EDGE_ALIGNED_PWM_AND_INPUT_CAPTURE = 0x08U, /**< Edge aligned PWM and input capture. */
} supertmr_config_mode_t;

/**
 * @brief SuperTimer clock source selection.
 */
typedef enum {
    SUPERTMR_CLOCK_SOURCE_NONE        = 0x00U, /**< None use clock for SUPERTMR.  */
    SUPERTMR_CLOCK_SOURCE_SYSTEMCLK   = 0x01U, /**< System clock.            */
    SUPERTMR_CLOCK_SOURCE_FIXEDCLK    = 0x02U, /**< Fixed clock.             */
    SUPERTMR_CLOCK_SOURCE_EXTERNALCLK = 0x03U  /**< External clock .         */
} supertmr_clock_source_t;

/**
 * @brief SuperTimer pre-scaler factor selection for the clock source.
 *
 * In quadrature decoder mode set SUPERTMR_CLOCK_DIVID_BY_1.
 */
typedef enum {
    SUPERTMR_CLOCK_DIVID_BY_1   = 0x00U, /**< Divide by   1. */
    SUPERTMR_CLOCK_DIVID_BY_2   = 0x01U, /**< Divide by   2. */
    SUPERTMR_CLOCK_DIVID_BY_4   = 0x02U, /**< Divide by   4. */
    SUPERTMR_CLOCK_DIVID_BY_8   = 0x03U, /**< Divide by   8. */
    SUPERTMR_CLOCK_DIVID_BY_16  = 0x04U, /**< Divide by  16. */
    SUPERTMR_CLOCK_DIVID_BY_32  = 0x05U, /**< Divide by  32. */
    SUPERTMR_CLOCK_DIVID_BY_64  = 0x06U, /**< Divide by  64. */
    SUPERTMR_CLOCK_DIVID_BY_128 = 0x07U  /**< Divide by 128. */
} supertmr_clock_ps_t;

/**
 * @brief List of SUPERTMR interrupts.
 */
typedef enum {
    SUPERTMR_CHANNEL0_INT_ENABLE       = 0x00000001U, /**< Channel 0 interrupt. */
    SUPERTMR_CHANNEL1_INT_ENABLE       = 0x00000002U, /**< Channel 1 interrupt. */
    SUPERTMR_CHANNEL2_INT_ENABLE       = 0x00000004U, /**< Channel 2 interrupt. */
    SUPERTMR_CHANNEL3_INT_ENABLE       = 0x00000008U, /**< Channel 3 interrupt. */
    SUPERTMR_CHANNEL4_INT_ENABLE       = 0x00000010U, /**< Channel 4 interrupt. */
    SUPERTMR_CHANNEL5_INT_ENABLE       = 0x00000020U, /**< Channel 5 interrupt. */
    SUPERTMR_CHANNEL6_INT_ENABLE       = 0x00000040U, /**< Channel 6 interrupt. */
    SUPERTMR_CHANNEL7_INT_ENABLE       = 0x00000080U, /**< Channel 7 interrupt. */
    SUPERTMR_FAULT_INT_ENABLE          = 0x00000100U, /**< Fault interrupt. */
    SUPERTMR_TIME_OVER_FLOW_INT_ENABLE = 0x00000200U, /**< Time overflow interrupt. */
    SUPERTMR_RELOAD_INT_ENABLE         = 0x00000400U  /**< Reload interrupt; Available only on certain SoC's. */
} supertmr_interrupt_option_t;

/**
 * @brief List of SUPERTMR flags.
 */
typedef enum {
    SUPERTMR_CHANNEL0_FLAG        = 0x00000001U, /**< Channel 0 Flag. */
    SUPERTMR_CHANNEL1_FLAG        = 0x00000002U, /**< Channel 1 Flag. */
    SUPERTMR_CHANNEL2_FLAG        = 0x00000004U, /**< Channel 2 Flag. */
    SUPERTMR_CHANNEL3_FLAG        = 0x00000008U, /**< Channel 3 Flag. */
    SUPERTMR_CHANNEL4_FLAG        = 0x00000010U, /**< Channel 4 Flag. */
    SUPERTMR_CHANNEL5_FLAG        = 0x00000020U, /**< Channel 5 Flag. */
    SUPERTMR_CHANNEL6_FLAG        = 0x00000040U, /**< Channel 6 Flag. */
    SUPERTMR_CHANNEL7_FLAG        = 0x00000080U, /**< Channel 7 Flag. */
    SUPERTMR_FAULT_FLAG           = 0x00000100U, /**< Fault Flag. */
    SUPERTMR_TIME_OVER_FLOW_FLAG  = 0x00000200U, /**< Time overflow Flag. */
    SUPERTMR_RELOAD_FLAG          = 0x00000400U, /**< Reload Flag; Available only on certain SoC's. */
    SUPERTMR_CHANNEL_TRIGGER_FLAG = 0x00000800U  /**< Channel trigger Flag. */
} supertmr_status_flag_t;

/**
 * @brief SUPERTMR sync source.
 */
typedef enum {
    SUPERTMR_SYSTEM_CLOCK = 0U, /**< Register is updated with its buffer value at all rising edges of system clock. */
    SUPERTMR_PWM_SYNC     = 1U  /**< Register is updated with its buffer value at the SUPERTMR synchronization. */
} supertmr_reg_update_t;

/**
 * @brief SUPERTMR update register.
 */
typedef enum {
    SUPERTMR_WAIT_LOADING_POINTS = 0U, /**< SUPERTMR register is updated at first loading point. */
    SUPERTMR_UPDATE_NOW          = 1U  /**< SUPERTMR register is updated immediately. */
} supertmr_pwm_sync_mode_t;

/**
 * @brief SuperTimer pre-scaler factor for the dead-time insertion.
 */
typedef enum {
    SUPERTMR_DEADTIME_DIVID_BY_1  = 0x01U, /**< Divide by  1. */
    SUPERTMR_DEADTIME_DIVID_BY_4  = 0x02U, /**< Divide by  4. */
    SUPERTMR_DEADTIME_DIVID_BY_16 = 0x03U  /**< Divide by 16. */
} supertmr_deadtime_ps_t;

/**
 * @brief Options for the SuperTimer behavior in BDM Mode.
 */
typedef enum {
    SUPERTMR_BDM_MODE_00 = 0x00U, /**< SUPERTMR counter stopped, CH(n)F bit can be set, SUPERTMR channels
                                   *   in functional mode, writes to MOD,CNTIN and C(n)V registers bypass
                                   *   the register buffers. */
    SUPERTMR_BDM_MODE_01 = 0x01U, /**< SUPERTMR counter stopped, CH(n)F bit is not set, SUPERTMR channels
                                   *   outputs are forced to their safe value , writes to MOD,CNTIN and
                                   *   C(n)V registers bypass the register buffers. */
    SUPERTMR_BDM_MODE_10 = 0x02U, /**< SUPERTMR counter stopped, CH(n)F bit is not set, SUPERTMR channels
                                   *    outputs are frozen when chip enters in BDM mode, writes to MOD,
                                   *    CNTIN and C(n)V registers bypass the register buffers. */
    SUPERTMR_BDM_MODE_11 = 0x03U  /**< SUPERTMR counter in functional mode, CH(n)F bit can be set,
                                   *   SUPERTMR channels in functional mode, writes to MOD,CNTIN and C(n)V
                                   *   registers is in fully functional mode. */
} supertmr_bdm_mode_t;

/**
 * @brief Options for the SuperTimer2 and SuperTimer1 channel 0 input
 */
typedef enum {
    SUPERTMRX_CH0_SEL_CH0  = 0x00U, /**< SUPERTMRx channel0 is channel 0 input */
    SUPERTMRX_CH0_SEL_CMP0 = 0x01U, /**< SUPERTMRx channel0 is CMP0 output */
} supertmr_ch0_sel_t;

/**
 * @brief Options for the SuperTimer fault select
 */
typedef enum {
    SUPERTMR_FLT_SEL_FLT_PIN         = 0x00U, /**< select fault pin as fault input */
    SUPERTMR_FLT_SEL_TRGMUX_SUPERTMR = 0x01U, /**< select supertmr trgmux out as fault input */
} supertmr_fault_sel_t;

/**
 * @brief Configuration structure that the user needs to set.
 */
typedef struct {
    uint32_t instance; /**< Instance. */
    uint32_t channel;  /**< Channel. */
} supertmr_interrupt_t;

/**
 * @brief SuperTimer state structure of the driver.
 */
typedef struct {
    supertmr_clock_source_t supertmrClockSource;             /**< Clock source used by SUPERTMR counter. */
    supertmr_config_mode_t supertmrMode;                     /**< Mode of operation for SUPERTMR. */
    uint16_t supertmrModValue;                               /**< This field is used only in input capture mode to store MOD value. */
    uint16_t supertmrPeriod;                                 /**< This field is used only in PWM mode to store signal period. */
    uint32_t supertmrSourceClockFrequency;                   /**< The clock frequency is used for counting. */
    uint16_t measurementResults[SUPERTMR_CHANNEL_COUNT];     /**< This field is used only in input capture mode to
                                                                      store edges time stamps. */
    void *channelsCallbacksParams[SUPERTMR_CHANNEL_COUNT];   /**< The parameters of callback function for channels
                                                                      events. */
    ic_callback_t channelsCallbacks[SUPERTMR_CHANNEL_COUNT]; /**< The callback function for channels events. */
    bool enableNotification[SUPERTMR_CHANNEL_COUNT];         /**< To save channels enable the notification on the
                                                                callback application. */
    supertmr_interrupt_t int_param[SUPERTMR_CHANNEL_COUNT];  /**< To save low level sdk channels interrupt parameters*/
    supertmr_callback_t supertmrCallback;                    /**< The callback function for supertmr events. \D{NULL} */
    void *supertmrCallbackParams;                            /**< The parameters of callback functions for supertmr events. *\D{NULL} */
} supertmr_state_t;

/**
 * @brief SuperTimer Registers sync parameters.
 *        Please don't use software and hardware trigger simultaneously.
 */
typedef struct {
    bool softwareSync;                     /**< True - enable software sync,
                                            *   False - disable software sync. \D{true} */
    bool hardwareSync0;                    /**< True - enable hardware 0 sync,
                                            *   False - disable hardware 0 sync. \D{false} */
    bool hardwareSync1;                    /**< True - enable hardware 1 sync,
                                            *   False - disable hardware 1 sync. \D{false} */
    bool hardwareSync2;                    /**< True - enable hardware 2 sync,
                                            *   False - disable hardware 2 sync. \D{false} */
    bool maxLoadingPoint;                  /**< True - enable maximum loading point,
                                            *   False - disable maximum loading point. \D{false} */
    bool minLoadingPoint;                  /**< True - enable minimum loading point,
                                            *   False - disable minimum loading point. \D{false} */
    supertmr_reg_update_t inverterSync;    /**< Configures INVCTRL sync. \D{SUPERTMR_PWM_SYNC} */
    supertmr_reg_update_t outRegSync;      /**< Configures SWOCTRL sync. \D{SUPERTMR_PWM_SYNC} */
    supertmr_reg_update_t maskRegSync;     /**< Configures OUTMASK sync. \D{SUPERTMR_PWM_SYNC} */
    supertmr_reg_update_t initCounterSync; /**< Configures CNTIN sync. \D{SUPERTMR_PWM_SYNC} */
    bool autoClearTrigger;                 /**< Available only for hardware trigger. \D{true} */
    supertmr_pwm_sync_mode_t syncPoint;    /**< Configure synchronization method.
                                            *   (waiting next loading point or immediate). \D{SUPERTMR_UPDATE_NOW} */
} supertmr_pwm_sync_t;

/**
 * @brief Configuration structure that the user needs to set.
 */
typedef struct {
    supertmr_pwm_sync_t syncMethod;              /**< Register sync options available in the
                                                    *   supertmr_sync_method_t enumeration. */
    supertmr_config_mode_t supertmrMode;         /**< Mode of operation for SUPERTMR. \D{SUPERTMR_MODE_EDGE_ALIGNED_PWM} */
    supertmr_clock_ps_t supertmrPrescaler;       /**< Register pre-scaler options available in the
                                                    *   supertmr_clock_ps_t enumeration. \D{SUPERTMR_CLOCK_DIVID_BY_1} */
    supertmr_clock_source_t supertmrClockSource; /**< Select clock source for SUPERTMR. \D{SUPERTMR_CLOCK_SOURCE_SYSTEMCLK} */
    supertmr_bdm_mode_t BDMMode;                 /**< Select SUPERTMR behavior in BDM mode. \D{SUPERTMR_BDM_MODE_11} */
    bool isTofIsrEnabled;                        /**< true: enable interrupt,
                                                    * false: write interrupt is disabled. \D{false} */
    bool enableInitializationTrigger;            /**< true: enable the generation of initialization trigger,
                                                    * false: disable the generation of initialization trigger. \D{false} */
    supertmr_callback_t callback;                /**< The callback function for supertmr events. \D{NULL} */
    void *cbParams;                              /**< The parameters of callback functions for supertmr events. *\D{NULL} */
} supertmr_user_config_t;

/**
 * @brief Pointer to runtime state structure.
 */
extern supertmr_state_t *supertmrStatePtr[SUPERTMR_INSTANCE_COUNT];

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Sets the filter Pre-scaler divider.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  filterPrescale The SUPERTMR peripheral clock pre-scale divider.
 */
static inline void SUPERTMR_DRV_SetClockFilterPs(supertmr_type_t *const supertmrBase, uint8_t filterPrescale)
{
    SUPERTMR_RMW_SC(supertmrBase, SUPERTMR_SC_FLTPS_MASK, SUPERTMR_SC_FLTPS(filterPrescale));
}

/**
 * @brief Reads the SUPERTMR filter clock divider.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @return uint8_t The SUPERTMR filter clock pre-scale divider.
 */
static inline uint8_t SUPERTMR_DRV_GetClockFilterPs(const supertmr_type_t *supertmrBase)
{
    return (uint8_t)((((supertmrBase)->SC) & SUPERTMR_SC_FLTPS_MASK) >> SUPERTMR_SC_FLTPS_SHIFT);
}

/**
 * @brief Returns the SUPERTMR peripheral current counter value.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @return uint16_t The current SUPERTMR timer counter value.
 */
static inline uint16_t SUPERTMR_DRV_GetCounter(const supertmr_type_t *supertmrBase)
{
    return (uint16_t)((((supertmrBase)->CNT) & SUPERTMR_CNT_COUNT_MASK) >> SUPERTMR_CNT_COUNT_SHIFT);
}

/**
 * @brief Returns the SUPERTMR peripheral counter modulo value.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @return uint16_t SUPERTMR timer modulo value.
 */
static inline uint16_t SUPERTMR_DRV_GetMod(const supertmr_type_t *supertmrBase)
{
    return (uint16_t)((((supertmrBase)->MOD) & SUPERTMR_MOD_MOD_MASK) >> SUPERTMR_MOD_MOD_SHIFT);
}

/**
 * @brief Returns the SUPERTMR peripheral counter initial value.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @return uint16_t SUPERTMR timer counter initial value.
 */
static inline uint16_t SUPERTMR_DRV_GetCounterInitVal(const supertmr_type_t *supertmrBase)
{
    return (uint16_t)((((supertmrBase)->CNTIN) & SUPERTMR_CNTIN_INIT_MASK) >> SUPERTMR_CNTIN_INIT_SHIFT);
}

/**
 * @brief Clears the content of Channel (n) Status And Control.
 *
 * @param  supertmrBase supertmrBase The SUPERTMR base address pointer.
 * @param  channel The SUPERTMR peripheral channel number.
 */
static inline void SUPERTMR_DRV_ClearChSC(supertmr_type_t *const supertmrBase, uint8_t channel)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    ((supertmrBase)->CONTROLS[channel].CnSC) = 0U;
}

/**
 * @brief Gets the SUPERTMR peripheral timer channel edge level.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channel The SUPERTMR peripheral channel number.
 * @return uint8_t The ELSnB:ELSnA mode value, will be 00, 01, 10, 11.
 */
static inline uint8_t SUPERTMR_DRV_GetChnEdgeLevel(const supertmr_type_t *supertmrBase, uint8_t channel)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);
    uint8_t retValue;

    retValue = (uint8_t)((((supertmrBase)->CONTROLS[channel].CnSC) & SUPERTMR_CnSC_ELSA_MASK) >> SUPERTMR_CnSC_ELSA_SHIFT);

    retValue |= (uint8_t)(((((supertmrBase)->CONTROLS[channel].CnSC) & SUPERTMR_CnSC_ELSB_MASK) >> SUPERTMR_CnSC_ELSB_SHIFT) << 1U);

    return retValue;
}

/**
 * @brief Configure the feature of SUPERTMR counter reset by the selected input capture event.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channel The SUPERTMR peripheral channel number.
 * @param  enable Enable the SUPERTMR counter reset
 * true: SUPERTMR counter is reset.
 * false: SUPERTMR counter is not reset.
 */
static inline void SUPERTMR_DRV_SetChnIcrstCmd(supertmr_type_t *const supertmrBase, uint8_t channel, bool enable)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    /*** Write ICRST bit. */
    SUPERTMR_RMW_CnSCV_REG(supertmrBase, channel, SUPERTMR_CnSC_ICRST_MASK, SUPERTMR_CnSC_ICRST(enable));
}

/**
 * @brief Returns whether the SUPERTMR SUPERTMR counter is reset.
 *
 * Return State of the SUPERTMR peripheral timer channel ICRST.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channel The SUPERTMR peripheral channel number.
 * @return true: Enabled the SUPERTMR counter reset.
 * @return false: Disabled the SUPERTMR counter reset.
 */
static inline bool SUPERTMR_DRV_IsChnIcrst(const supertmr_type_t *supertmrBase, uint8_t channel)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    return (((supertmrBase)->CONTROLS[channel].CnSC) & SUPERTMR_CnSC_ICRST_MASK) != 0U;
}

/**
 * @brief Enables or disables the SUPERTMR peripheral timer channel PDMA.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channel The SUPERTMR peripheral channel number.
 * @param  enable Enable PDMA transfers for the channel
 * true: Enabled PDMA transfers.
 * false: Disabled PDMA transfers.
 */
static inline void SUPERTMR_DRV_SetChnDmaCmd(supertmr_type_t *const supertmrBase, uint8_t channel, bool enable)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    /*** Write PDMA bit. */
    SUPERTMR_RMW_CnSCV_REG(supertmrBase, channel, SUPERTMR_CnSC_DMA_MASK, SUPERTMR_CnSC_DMA(enable));
}

/**
 * @brief Returns whether the SUPERTMR peripheral timer channel PDMA is enabled.
 *
 * Return state of the SUPERTMR peripheral timer channel PDMA.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channel The SUPERTMR peripheral channel number.
 * @return true: Enabled PDMA transfers.
 * @return false: Disabled PDMA transfers.
 */
static inline bool SUPERTMR_DRV_IsChnDma(const supertmr_type_t *supertmrBase, uint8_t channel)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    return (((supertmrBase)->CONTROLS[channel].CnSC) & SUPERTMR_CnSC_DMA_MASK) != 0U;
}

/**
 * @brief Enables or disables the trigger generation on SUPERTMR channel outputs.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channel The SUPERTMR peripheral channel number.
 * @param  enable Trigger mode control
 * true: Disable a trigger generation on channel output.
 * false: Enable PWM output without generating a pulse.
 */
static inline void SUPERTMR_DRV_SetTrigModeControlCmd(supertmr_type_t *const supertmrBase, uint8_t channel, bool enable)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    /*** Write TRIGMODE bit. */
    SUPERTMR_RMW_CnSCV_REG(supertmrBase, channel, SUPERTMR_CnSC_TRIGMODE_MASK, SUPERTMR_CnSC_TRIGMODE((enable)));
}

/**
 * @brief Returns whether the trigger mode is enabled.
 *
 * Return state of the channel outputs.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channel The SUPERTMR peripheral channel number.
 * @return true: Enabled a trigger generation on channel output.
 * @return false: PWM outputs without generating a pulse.
 */
static inline bool SUPERTMR_DRV_GetTriggerControled(const supertmr_type_t *supertmrBase, uint8_t channel)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    return (((supertmrBase)->CONTROLS[channel].CnSC) & SUPERTMR_CnSC_TRIGMODE_MASK) != 0U;
}

/**
 * @brief Get the state of channel input.
 *
 * Return state of the channel inputs.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channel The SUPERTMR peripheral channel number.
 * @return true: The channel input is one.
 * @return false: The channel input is zero.
 */
static inline bool SUPERTMR_DRV_GetChInputState(const supertmr_type_t *supertmrBase, uint8_t channel)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    return (((supertmrBase)->CONTROLS[channel].CnSC) & SUPERTMR_CnSC_CHIS_MASK) != 0U;
}

/**
 * @brief Get the value of channel output.
 *
 * Return value of the channel outputs.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channel The SUPERTMR peripheral channel number.
 * @return true: The channel output is one.
 * @return false: The channel output is zero.
 */
static inline bool SUPERTMR_DRV_GetChOutputValue(const supertmr_type_t *supertmrBase, uint8_t channel)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    return (((supertmrBase)->CONTROLS[channel].CnSC) & SUPERTMR_CnSC_CHOV_MASK) != 0U;
}

/**
 * @brief Gets the SUPERTMR peripheral timer channel counter value.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channel The SUPERTMR peripheral channel number.
 * @return uint16_t Channel counter value.
 */
static inline uint16_t SUPERTMR_DRV_GetChnCountVal(const supertmr_type_t *supertmrBase, uint8_t channel)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    return (uint16_t)((supertmrBase)->CONTROLS[channel].CnV);
}

/**
 * @brief Gets the SUPERTMR peripheral timer  channel event status.
 *
 * Return channel event status.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channel The SUPERTMR peripheral channel number.
 * @return true: A channel event has occurred.
 * @return false: No channel event has occurred.
 */
static inline bool SUPERTMR_DRV_GetChnEventStatus(const supertmr_type_t *supertmrBase, uint8_t channel)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    return (((supertmrBase)->STATUS) & (1UL << channel)) != 0U;
}

/**
 * @brief Gets the SUPERTMR peripheral timer status info for all channels.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @return uint32_t Channel event status value.
 */
static inline uint32_t SUPERTMR_DRV_GetEventStatus(const supertmr_type_t *supertmrBase)
{
    return ((supertmrBase)->STATUS) & (0xFFU);
}

/**
 * @brief Clears the SUPERTMR peripheral timer all channel event status.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channel The SUPERTMR peripheral channel number.
 */
static inline void SUPERTMR_DRV_ClearChnEventStatus(supertmr_type_t *const supertmrBase, uint8_t channel)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    ((supertmrBase)->STATUS) &= (~(1UL << channel));
}

/**
 * @brief Sets the SUPERTMR peripheral timer channel output mask.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channel The SUPERTMR peripheral channel number.
 * @param  mask Value to set Output Mask
 * true: Channel output is masked.
 * false: Channel output is not masked.
 */
static inline void SUPERTMR_DRV_SetChnOutputMask(supertmr_type_t *const supertmrBase, uint8_t channel, bool mask)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    if (mask) {
        ((supertmrBase)->OUTMASK) |= (1UL << channel);
    } else {
        ((supertmrBase)->OUTMASK) &= ~(1UL << channel);
    }
}

/**
 * @brief Sets the SUPERTMR peripheral timer channel output initial state 0 or 1.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channel The SUPERTMR peripheral channel number.
 * @param  state Initial state for channels output
 * true: The initialization value is 1.
 * false: The initialization value is 0.
 */
static inline void SUPERTMR_DRV_SetChnOutputInitStateCmd(supertmr_type_t *const supertmrBase, uint8_t channel,
                                                         bool state)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    if (state) {
        ((supertmrBase)->OUTINIT) |= (1UL << channel);
    } else {
        ((supertmrBase)->OUTINIT) &= ~(1UL << channel);
    }
}

/**
 * @brief Disables the SUPERTMR peripheral timer fault interrupt.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 */
static inline void SUPERTMR_DRV_DisableFaultInt(supertmr_type_t *const supertmrBase)
{
    SUPERTMR_RMW_MODE(supertmrBase, SUPERTMR_MODE_FAULTIE_MASK, SUPERTMR_MODE_FAULTIE(0U));
}

/**
 * @brief Enables or disables the SUPERTMR peripheral timer capture test mode.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  enable Capture Test Mode Enable
 * true: Capture test mode is enabled.
 * false: Capture test mode is disabled.
 */
static inline void SUPERTMR_DRV_SetCaptureTestCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    SUPERTMR_RMW_MODE(supertmrBase, SUPERTMR_MODE_CAPTEST_MASK, SUPERTMR_MODE_CAPTEST(enable));
}

/**
 * @brief Get status of the SUPERTMREN bit in the SUPERTMR_MODE register.
 *
 * Return the SUPERTMR Enable status.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @return true: TPM compatibility. Free running counter and synchronization compatible with TPM.
 * @return false: Free running counter and synchronization are different from TPM behavior.
 */
static inline bool SUPERTMR_DRV_IsSuperTmrEnable(const supertmr_type_t *supertmrBase)
{
    return ((supertmrBase->MODE & SUPERTMR_MODE_SUPERTMREN_MASK) >> SUPERTMR_MODE_SUPERTMREN_SHIFT) != 0U;
}

/**
 * @brief Determines if the SUPERTMR counter is re-initialized when the selected trigger for
 * synchronization is detected.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  enable SUPERTMR counter re-initialization selection
 * true: To update SUPERTMR counter when triggered.
 * false: To count normally.
 */
static inline void SUPERTMR_DRV_SetCountReinitSyncCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    SUPERTMR_RMW_SYNC(supertmrBase, SUPERTMR_SYNC_REINIT_MASK, SUPERTMR_SYNC_REINIT(enable));
}

/**
 * @brief Checks whether the write protection is enabled.
 *
 * Return write-protection status.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @return true: If enabled.
 * @return false: If not.
 */
static inline bool SUPERTMR_DRV_IsWriteProtectionEnabled(const supertmr_type_t *supertmrBase)
{
    return (supertmrBase->FMS & SUPERTMR_FMS_WPEN_MASK) != 0U;
}

/**
 * @brief Checks whether the logic OR of the fault inputs is enabled.
 *
 * Return the enabled fault inputs status.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @return true: The logic OR of the enabled fault inputs is 1.
 * @return false: The logic OR of the enabled fault inputs is 0.
 */
static inline bool SUPERTMR_DRV_IsFaultInputEnabled(const supertmr_type_t *supertmrBase)
{
    return (supertmrBase->FMS & SUPERTMR_FMS_FAULTIN_MASK) != 0U;
}

/**
 * @brief Checks whether a fault condition is detected at the fault input.
 *
 * Return the fault condition status。
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channel The SUPERTMR peripheral channel.
 * @return true: A fault condition was detected at the fault input.
 * @return false: No fault condition was detected at the fault input.
 */
static inline bool SUPERTMR_DRV_IsFaultFlagDetected(const supertmr_type_t *supertmrBase, uint8_t channel)
{
    OS_ASSERT(channel < CHAN4_IDX);

    return (supertmrBase->FMS & (SUPERTMR_FMS_FAULTF0_MASK << channel)) != 0U;
}

/**
 * @brief Clear a fault condition is detected at the fault input.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channel The SUPERTMR peripheral channel.
 */
static inline void SUPERTMR_DRV_ClearFaultFlagDetected(supertmr_type_t *const supertmrBase, uint8_t channel)
{
    OS_ASSERT(channel < CHAN4_IDX);

    ((supertmrBase)->FMS) &= (~(1UL << channel));
}

/**
 * @brief Enables or disables the channel invert for a channel pair.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  chnlPairNum The SUPERTMR peripheral channel pair number.
 * @param  enable State of channel invert for a channel pair
 * true: To enable channel inverting.
 * false: To disable channel inversion.
 */
static inline void SUPERTMR_DRV_SetDualChnInvertCmd(supertmr_type_t *const supertmrBase, uint8_t chnlPairNum,
                                                    bool enable)
{
    OS_ASSERT(chnlPairNum < (SUPERTMR_CHANNEL_COUNT >> 1U));

    if (enable) {
        ((supertmrBase)->INVCTRL) |= (1UL << chnlPairNum);
    } else {
        ((supertmrBase)->INVCTRL) &= ~(1UL << chnlPairNum);
    }
}

/**
 * @brief Enables or disables the channel software output control.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channel Channel to be enabled or disabled.
 * @param  enable State of channel software output control
 * true: To enable the channel output will be affected by software output control.
 * false: To disable the channel output is unaffected.
 */
static inline void SUPERTMR_DRV_SetChnSoftwareCtrlCmd(supertmr_type_t *const supertmrBase, uint8_t channel, bool enable)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    if (enable) {
        ((supertmrBase)->SWOCTRL) |= (1UL << channel);
    } else {
        ((supertmrBase)->SWOCTRL) &= ~(1UL << channel);
    }
}

/**
 * @brief Sets the channel software output control value.
 * Despite the odd channels are configured as HIGH/LOW, they will be inverted in the following
 * configuration: COMP bit = 1 and CH(n)OCV and CH(n+1)OCV are HIGH. Please check Software
 * output control behavior chapter from RM.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channel Channel to be configured.
 * @param  enable State of software output control value
 * true: to force 1 to the channel output.
 * false: to force 0 to the channel output.
 */
static inline void SUPERTMR_DRV_SetChnSoftwareCtrlVal(supertmr_type_t *const supertmrBase, uint8_t channel, bool enable)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    if ((uint32_t)enable != (((uint32_t)supertmrBase->POL >> (uint32_t)channel) & (uint32_t)1U)) {
        ((supertmrBase)->SWOCTRL) |= (1UL << (channel + SUPERTMR_SWOCTRL_CH0OCV_SHIFT));
    } else {
        ((supertmrBase)->SWOCTRL) &= ~(1UL << (channel + SUPERTMR_SWOCTRL_CH0OCV_SHIFT));
    }
}

/** SUPERTMR PWM load control*/
/**
 * @brief Set the global load mechanism.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 */
static inline void SUPERTMR_DRV_SetGlobalLoadCmd(supertmr_type_t *const supertmrBase)
{
    ((supertmrBase)->PWMLOAD) |= (1UL << SUPERTMR_PWMLOAD_GLDOK_SHIFT);
}

/**
 * @brief Enable the global load.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  enable State of the global load mechanism
 * true: Global Load OK enabled.
 * false: Global Load OK disabled.
 */
static inline void SUPERTMR_DRV_SetLoadCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    if (enable) {
        ((supertmrBase)->PWMLOAD) |= (1UL << SUPERTMR_PWMLOAD_GLEN_SHIFT);
    } else {
        ((supertmrBase)->PWMLOAD) &= ~(1UL << SUPERTMR_PWMLOAD_GLEN_SHIFT);
    }
}

/**
 * @brief Enable the half cycle reload.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  enable State of the half cycle match as a reload opportunity
 * true: Half cycle reload is enabled.
 * false: Half cycle reload is disabled.
 */
static inline void SUPERTMR_DRV_SetHalfCycleCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    if (enable) {
        ((supertmrBase)->PWMLOAD) |= (1UL << SUPERTMR_PWMLOAD_HCSEL_SHIFT);
    } else {
        ((supertmrBase)->PWMLOAD) &= ~(1UL << SUPERTMR_PWMLOAD_HCSEL_SHIFT);
    }
}

/**
 * @brief Enables or disables the loading of MOD, CNTIN and CV with values of their write buffer.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  enable State of loading updated values
 * true: To enable the loading of value of their buffer.
 * false: To disable the loading of value of their buffer.
 */
static inline void SUPERTMR_DRV_SetPwmLoadCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    if (enable) {
        ((supertmrBase)->PWMLOAD) |= (1UL << SUPERTMR_PWMLOAD_LDOK_SHIFT);
    } else {
        ((supertmrBase)->PWMLOAD) &= ~(1UL << SUPERTMR_PWMLOAD_LDOK_SHIFT);
    }
}

/**
 * @brief Includes or excludes the channel in the matching process.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channel Channel to be configured.
 * @param  enable State of channel
 * true: means include the channel in the matching process.
 * false: means do not include channel in the matching process.
 */
static inline void SUPERTMR_DRV_SetPwmLoadChnSelCmd(supertmr_type_t *const supertmrBase, uint8_t channel, bool enable)
{
    OS_ASSERT(channel < SUPERTMR_CHANNEL_COUNT);

    if (enable) {
        ((supertmrBase)->PWMLOAD) |= (1UL << channel);
    } else {
        ((supertmrBase)->PWMLOAD) &= ~(1UL << channel);
    }
}

/** SUPERTMR configuration*/
/**
 * @brief Enables or disables the SUPERTMR initialization trigger on Reload Point.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  enable bit controls whether an initialization trigger is generated
 * true: Trigger is generated when a reload point is reached.
 * false: Trigger is generated on counter wrap events.
 */
static inline void SUPERTMR_DRV_SetInitTrigOnReloadCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    supertmrBase->CONF = (supertmrBase->CONF & ~SUPERTMR_CONF_ITRIGR_MASK) | SUPERTMR_CONF_ITRIGR(enable);
}

/**
 * @brief Enables or disables the SUPERTMR global time base signal generation to other SUPERTMR's.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  enable State of global time base signal
 * true: To enable the global time base generation to other SUPERTMR instances.
 * false: To disable the global time base generation to other SUPERTMR instances.
 */
static inline void SUPERTMR_DRV_SetGlobalTimeBaseOutputCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    supertmrBase->CONF = (supertmrBase->CONF & ~SUPERTMR_CONF_GTBEOUT_MASK) | SUPERTMR_CONF_GTBEOUT(enable);
}

/**
 * @brief Enables or disables the SUPERTMR timer global time base.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  enable State of global time base
 * true: To enable an external global time base signal.
 * false: To disable an external global time base signal.
 */
static inline void SUPERTMR_DRV_SetGlobalTimeBaseCmd(supertmr_type_t *const supertmrBase, bool enable)
{
    supertmrBase->CONF = (supertmrBase->CONF & ~SUPERTMR_CONF_GTBEEN_MASK) | SUPERTMR_CONF_GTBEEN(enable);
}

/**
 * @brief Sets the frequency of reload points.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  val Value of the TOF bit set frequency.
 */
static inline void SUPERTMR_DRV_SetLoadFreq(supertmr_type_t *const supertmrBase, uint8_t val)
{
    SUPERTMR_RMW_CONF(supertmrBase, SUPERTMR_CONF_LDFQ_MASK, SUPERTMR_CONF_LDFQ(val));
}

/**
 * @brief Sets the SUPERTMR extended dead-time value for the channel pair.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channelPair The SUPERTMR peripheral channel pair (n).
 * @param  value The SUPERTMR peripheral extend pre-scale divider using the concatenation with the dead-time value.
 */
static inline void SUPERTMR_DRV_SetExtPairDeadtimeValue(supertmr_type_t *const supertmrBase, uint8_t channelPair,
                                                        uint8_t value)
{
    OS_ASSERT(value < 16U);
    OS_ASSERT(channelPair < CHAN4_IDX);

    switch (channelPair) {
    case CHAN0_IDX:
        SUPERTMR_RMW_PAIR0DEADTIME(supertmrBase, SUPERTMR_PAIR0DEADTIME_DTVALEX_MASK,
                                   SUPERTMR_PAIR0DEADTIME_DTVALEX(value));
        break;
    case CHAN1_IDX:
        SUPERTMR_RMW_PAIR1DEADTIME(supertmrBase, SUPERTMR_PAIR1DEADTIME_DTVALEX_MASK,
                                   SUPERTMR_PAIR1DEADTIME_DTVALEX(value));
        break;
    case CHAN2_IDX:
        SUPERTMR_RMW_PAIR2DEADTIME(supertmrBase, SUPERTMR_PAIR2DEADTIME_DTVALEX_MASK,
                                   SUPERTMR_PAIR2DEADTIME_DTVALEX(value));
        break;
    case CHAN3_IDX:
        SUPERTMR_RMW_PAIR3DEADTIME(supertmrBase, SUPERTMR_PAIR3DEADTIME_DTVALEX_MASK,
                                   SUPERTMR_PAIR3DEADTIME_DTVALEX(value));
        break;
    default:
        /*** Nothing to do. */
        break;
    }
}

/**
 * @brief Sets the SUPERTMR dead time divider for the channel pair.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channelPair The SUPERTMR peripheral channel pair (n).
 * @param  divider The SUPERTMR peripheral pre-scaler divider
 * SUPERTMR_DEADTIME_DIVID_BY_1 : Divide by  1.
 * SUPERTMR_DEADTIME_DIVID_BY_4 : Divide by  4.
 * SUPERTMR_DEADTIME_DIVID_BY_16: Divide by 16.
 */
static inline void SUPERTMR_DRV_SetPairDeadtimePrescale(supertmr_type_t *const supertmrBase, uint8_t channelPair,
                                                        supertmr_deadtime_ps_t divider)
{
    OS_ASSERT(channelPair < CHAN4_IDX);

    switch (channelPair) {
    case CHAN0_IDX:
        SUPERTMR_RMW_PAIR0DEADTIME(supertmrBase, SUPERTMR_PAIR0DEADTIME_DTPS_MASK,
                                   SUPERTMR_PAIR0DEADTIME_DTPS((uint8_t)divider));
        break;
    case CHAN1_IDX:
        SUPERTMR_RMW_PAIR1DEADTIME(supertmrBase, SUPERTMR_PAIR1DEADTIME_DTPS_MASK,
                                   SUPERTMR_PAIR1DEADTIME_DTPS((uint8_t)divider));
        break;
    case CHAN2_IDX:
        SUPERTMR_RMW_PAIR2DEADTIME(supertmrBase, SUPERTMR_PAIR2DEADTIME_DTPS_MASK,
                                   SUPERTMR_PAIR2DEADTIME_DTPS((uint8_t)divider));
        break;
    case CHAN3_IDX:
        SUPERTMR_RMW_PAIR3DEADTIME(supertmrBase, SUPERTMR_PAIR3DEADTIME_DTPS_MASK,
                                   SUPERTMR_PAIR3DEADTIME_DTPS((uint8_t)divider));
        break;
    default:
        /*** Nothing to do. */
        break;
    }
}

/**
 * @brief Sets the SUPERTMR dead-time value for the channel pair.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channelPair The SUPERTMR peripheral channel pair (n).
 * @param  count The SUPERTMR peripheral selects the dead-time value
 * 0U : no counts inserted.
 * 1U : 1 count is inserted.
 * 2U : 2 count is inserted.
 * ... up to a possible 63 counts.
 */
static inline void SUPERTMR_DRV_SetPairDeadtimeCount(supertmr_type_t *const supertmrBase, uint8_t channelPair,
                                                     uint8_t count)
{
    OS_ASSERT(channelPair < CHAN4_IDX);
    OS_ASSERT(count < 64U);

    switch (channelPair) {
    case CHAN0_IDX:
        SUPERTMR_RMW_PAIR0DEADTIME(supertmrBase, SUPERTMR_PAIR0DEADTIME_DTVAL_MASK,
                                   SUPERTMR_PAIR0DEADTIME_DTVAL(count));
        break;
    case CHAN1_IDX:
        SUPERTMR_RMW_PAIR1DEADTIME(supertmrBase, SUPERTMR_PAIR1DEADTIME_DTVAL_MASK,
                                   SUPERTMR_PAIR1DEADTIME_DTVAL(count));
        break;
    case CHAN2_IDX:
        SUPERTMR_RMW_PAIR2DEADTIME(supertmrBase, SUPERTMR_PAIR2DEADTIME_DTVAL_MASK,
                                   SUPERTMR_PAIR2DEADTIME_DTVAL(count));
        break;
    case CHAN3_IDX:
        SUPERTMR_RMW_PAIR3DEADTIME(supertmrBase, SUPERTMR_PAIR3DEADTIME_DTVAL_MASK,
                                   SUPERTMR_PAIR3DEADTIME_DTVAL(count));
        break;
    default:
        /*** Nothing to do. */
        break;
    }
}

/**
 * @brief Sets the mirror of the modulo integer value.
 *
 * @param  supertmrBase supertmrBase The SUPERTMR base address pointer.
 * @param  value The value to be set to the timer modulo.
 */
static inline void SUPERTMR_DRV_SetMirrorMod(supertmr_type_t *const supertmrBase, uint16_t value)
{
    SUPERTMR_RMW_MOD_MIRROR(supertmrBase, SUPERTMR_MOD_MIRROR_MOD_MASK, SUPERTMR_MOD_MIRROR_MOD(value));
}

/**
 * @brief Returns the mirror of the SUPERTMR peripheral counter modulo value.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @return uint16_t The mirror of the SUPERTMR timer modulo value.
 */
static inline uint16_t SUPERTMR_DRV_GetMirrorMod(const supertmr_type_t *supertmrBase)
{
    return (uint16_t)((((supertmrBase)->MOD_MIRROR) & SUPERTMR_MOD_MIRROR_MOD_MASK) >> SUPERTMR_MOD_MIRROR_MOD_SHIFT);
}

/**
 * @brief Returns The modulo fractional value is used in the PWM period dithering.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @return uint8_t The modulo fractional value.
 */
static inline uint8_t SUPERTMR_DRV_GetModFracVal(const supertmr_type_t *supertmrBase)
{
    return (uint8_t)((((supertmrBase)->MOD_MIRROR) & SUPERTMR_MOD_MIRROR_FRACMOD_MASK) >>
                     SUPERTMR_MOD_MIRROR_FRACMOD_SHIFT);
}

/**
 * @brief Sets the mirror of the channel (n) match integer value.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channel The SUPERTMR peripheral channel (n).
 * @param  value The value to be set to the mirror of the channel (n) match integer value.
 */
static inline void SUPERTMR_DRV_SetMirrorChnMatchVal(supertmr_type_t *const supertmrBase, uint8_t channel,
                                                     uint16_t value)
{
    SUPERTMR_RMW_CnV_MIRROR(supertmrBase, channel, SUPERTMR_CV_MIRROR_VAL_MASK, SUPERTMR_CV_MIRROR_VAL(value));
}

/**
 * @brief Returns the mirror of the channel (n) match integer value.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channel The SUPERTMR peripheral channel (n).
 * @return uint16_t The mirror of the channel (n) match value.
 */
static inline uint16_t SUPERTMR_DRV_GetMirrorChnMatchVal(const supertmr_type_t *supertmrBase, uint8_t channel)
{
    return (uint16_t)((((supertmrBase)->CV_MIRROR[channel]) & SUPERTMR_CV_MIRROR_VAL_MASK) >>
                      SUPERTMR_CV_MIRROR_VAL_SHIFT);
}

/**
 * @brief Returns the channel (n) match fractional value.
 *
 * @param  supertmrBase The SUPERTMR base address pointer.
 * @param  channel The SUPERTMR peripheral channel (n).
 * @return uint8_t The channel (n) match fractional value is used in the PWM edge dithering.
 */
static inline uint8_t SUPERTMR_DRV_GetChnMatchFracVal(const supertmr_type_t *supertmrBase, uint8_t channel)
{
    return (uint8_t)((((supertmrBase)->CV_MIRROR[channel]) & SUPERTMR_CV_MIRROR_FRACVAL_MASK) >>
                     SUPERTMR_CV_MIRROR_FRACVAL_SHIFT);
}

/**
 * @brief Initializes the SUPERTMR driver.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  info The SUPERTMR user configuration structure, see #supertmr_user_config_t.
 * @param  state The SUPERTMR state structure of the driver.
 * @return status_t Operation status
 * STATUS_SUCCESS : Completed successfully.
 * STATUS_ERROR : Error occurred.
 */
status_t SUPERTMR_DRV_Init(uint32_t instance, const supertmr_user_config_t *info, supertmr_state_t *state);

/**
 * @brief Shuts down the SUPERTMR driver.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @return status_t Operation status
 * STATUS_SUCCESS : Completed successfully.
 * STATUS_ERROR : Error occurred.
 */
status_t SUPERTMR_DRV_Deinit(uint32_t instance);

/**
 * @brief This function will get the default configuration values
 *        in the structure which is used as a common use-case.
 *
 * @param  config Pointer to the structure in which the configuration will be saved.
 */
void SUPERTMR_DRV_GetDefaultConfig(supertmr_user_config_t *const config);

/**
 * @brief This function will mask the output of the channels and at match events will be ignored
 * by the masked channels.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  channelsMask The mask which will select which channels will ignore match events.
 * @param  softwareTrigger If true a software trigger is generate to update PWM parameters.
 * @return status_t STATUS_SUCCESS : Completed successfully.
 */
status_t SUPERTMR_DRV_MaskOutputChannels(uint32_t instance, uint32_t channelsMask, bool softwareTrigger);

/**
 * @brief This function configure the initial counter value. The counter will get this
 * value after an overflow event.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  counterValue Initial counter value.
 * @param  softwareTrigger If true a software trigger is generate to update parameters.
 * @return status_t STATUS_SUCCESS : Completed successfully.
 */
status_t SUPERTMR_DRV_SetInitialCounterValue(uint32_t instance, uint16_t counterValue, bool softwareTrigger);

/**
 * @brief This function configure the value of the counter which will generates an reload point.
 *
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  reloadPoint Counter value which generates the reload point.
 * @param  softwareTrigger If true a software trigger is generate to update parameters.
 * @return status_t STATUS_SUCCESS : Completed successfully.
 */
status_t SUPERTMR_DRV_SetHalfCycleReloadPoint(uint32_t instance, uint16_t reloadPoint, bool softwareTrigger);

/**
 * @brief This function will force the output value of a channel to a specific value.
 * Before using this function it's mandatory to mask the match events using
 * SUPERTMR_DRV_MaskOutputChannels and to enable software output control using
 * SUPERTMR_DRV_SetSoftwareOutputChannelControl.
 * @note: When the PWM signal is configured with LOW/HIGH polarity on the channel (n).
 * It should be set the safe state as LOW level state. However, We will have an issue
 * with COMP bit is zero and CH(n)OCV is HIGH and CH(n+1)OCV is LOW.in the independent channel configuration.
 * Code configuration:
 * {
 *      .polarity = SUPERTMR_POLARITY_HIGH,
 *      .safeState = SUPERTMR_POLARITY_LOW,
 *      .enableSecondChannelOutput = true,
 *      .secondChannelPolarity = SUPERTMR_MAIN_DUPLICATED,
 * }
 *
 * Workaround: Configure the safe state as HIGH level state. The expected output will be correctly controlling
 * Should change configuration as following:
 * {
 *      .polarity = SUPERTMR_POLARITY_HIGH,
 *      .safeState = SUPERTMR_HIGH_STATE,
 *      .enableSecondChannelOutput = true,
 *      .secondChannelPolarity = SUPERTMR_MAIN_DUPLICATED,
 * }
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  channelsValues The values which will be software configured for channels.
 * @param  softwareTrigger If true a software trigger is generate to update registers.
 * @return status_t STATUS_SUCCESS : Completed successfully.
 */
status_t SUPERTMR_DRV_SetSoftOutChnValue(uint32_t instance, uint8_t channelsValues, bool softwareTrigger);

/**
 * @brief This function will configure which output channel can be software controlled.
 * Software output control forces the following values on channels (n) and (n+1) when the
 * COMP bit is zero and POL bit is zero.
 * CH(n)OC|CH(n+1)OC|CH(n)OCV|CH(n+1)OCV|Channel (n) Output      | Channel (n+1) Output
 *    0   |    0    |   X    |    X     | is not modified by SWOC| is not modified by SWOC
 *    1   |    1    |   0    |    0     | is forced to zero      | is forced to zero
 *    1   |    1    |   0    |    1     | is forced to zero      | is forced to one
 *    1   |    1    |   1    |    0     | is forced to one       | is forced to zero
 *    1   |    1    |   1    |    1     | is forced to one       | is forced to one
 *
 * Software output control forces the following values on channels (n) and (n+1) when the
 * COMP bit is one and POL bit is zero.
 * CH(n)OC|CH(n+1)OC|CH(n)OCV|CH(n+1)OCV|Channel (n) Output      | Channel (n+1) Output
 *    0   |    0    |   X    |    X     | is not modified by SWOC| is not modified by SWOC
 *    1   |    1    |   0    |    0     | is forced to zero      | is forced to zero
 *    1   |    1    |   0    |    1     | is forced to zero      | is forced to one
 *    1   |    1    |   1    |    0     | is forced to one       | is forced to zero
 *    1   |    1    |   1    |    1     | is forced to one       | is forced to zero
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  channelsMask The mask which will configure the channels which can be software controlled.
 * @param  softwareTrigger If true a software trigger is generate to update registers.
 * @return status_t STATUS_SUCCESS : Completed successfully.
 */
status_t SUPERTMR_DRV_SetSoftwareOutputChannelControl(uint32_t instance, uint8_t channelsMask, bool softwareTrigger);

/**
 * @brief This function will control list of channels by software to force
 * the output to specified value.
 * Despite the odd channels are configured as HIGH/LOW, they will be inverted in the following
 * configuration: COMP bit = 1 and CH(n)OCV and CH(n+1)OCV are HIGH. Please check software
 * output control behavior chapter from reference manual.
 * @note: When the PWM signal is configured with LOW/HIGH polarity on the channel (n).
 * It should be set the safe state as LOW level state. However, We will have an issue
 * with COMP bit is zero and CH(n)OCV is HIGH and CH(n+1)OCV is LOW.in the independent channel configuration.
 * Code configuration:
 * {
 *      .polarity = SUPERTMR_POLARITY_HIGH,
 *      .safeState = SUPERTMR_POLARITY_LOW,
 *      .enableSecondChannelOutput = true,
 *      .secondChannelPolarity = SUPERTMR_MAIN_DUPLICATED,
 * }
 *
 * Workaround: Configure the safe state as HIGH level state. The expected output will be correctly controlling
 * Should change configuration as following:
 * {
 *      .polarity = SUPERTMR_POLARITY_HIGH,
 *      .safeState = SUPERTMR_HIGH_STATE,
 *      .enableSecondChannelOutput = true,
 *      .secondChannelPolarity = SUPERTMR_MAIN_DUPLICATED,
 * }
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  channelMask The mask which will configure the channels which can be software controlled.
 * @param  channelValueMask The values which will be software configured for channels.
 * @param  softwareTrigger If true a software trigger is generate to update registers.
 * @return status_t STATUS_SUCCESS : Completed successfully.
 */
status_t SUPERTMR_DRV_SetAllChnSoftwareOutputControl(uint32_t instance, uint8_t channelMask, uint8_t channelValueMask,
                                                     bool softwareTrigger);

/**
 * @brief This function will configure if the second channel of a pair will be inverted or not.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  channelsPairMask The mask which will configure which channel pair will invert the second channel.
 * @param  softwareTrigger If true a software trigger is generate to update registers.
 * @return status_t STATUS_SUCCESS : Completed successfully.
 */
status_t SUPERTMR_DRV_SetInvertingControl(uint32_t instance, uint8_t channelsPairMask, bool softwareTrigger);

/**
 * @brief This function configure the maximum counter value.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  counterValue Maximum counter value.
 * @param  softwareTrigger If true a software trigger is generate to update parameters.
 * @return status_t STATUS_SUCCESS : Completed successfully.
 */
status_t SUPERTMR_DRV_SetModuloCounterValue(uint32_t instance, uint16_t counterValue, bool softwareTrigger);

/**
 * @brief This function will set the channel edge or level on the selection
 * of the channel mode.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  channel The channel number.
 * @param  level The level or edge selection for channel mode.
 * @return status_t STATUS_SUCCESS : Completed successfully.
 */
status_t SUPERTMR_DRV_SetOutputLevel(uint32_t instance, uint8_t channel, uint8_t level);

/**
 * @brief This function configures sync mechanism for some SUPERTMR registers (MOD, CNINT, HCR,
 *          CnV, OUTMASK, INVCTRL, SWOCTRL).
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  param The sync configuration structure.
 * @return status_t Operation status
 * STATUS_SUCCESS : Completed successfully.
 * STATUS_ERROR : Error occurred.
 */
status_t SUPERTMR_DRV_SetSync(uint32_t instance, const supertmr_pwm_sync_t *param);

/**
 * @brief This function is used to configure a trigger source for SUPERTMR instance.
 * This allow a hardware trigger input which can be used in PWM synchronization.
 * Note that the hardware trigger is implemented only on trigger 1 for each instance.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @return status_t STATUS_SUCCESS : Completed successfully.
 */
status_t SUPERTMR_DRV_GenerateHardwareTrigger(uint32_t instance);

/**
 * @brief This function is used to configure a trigger source for SUPERTMRx fault.
 * This allow the supertmr fault comes from trgmux.
 * Note that only SUPERTMR0/1/2/3 support.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  flt0 True if select source trgmux.
 * @param  flt1 True if select source trgmux.
 * @param  flt2 True if select source trgmux.
 * @return status_t STATUS_SUCCESS : Completed successfully.
 */
status_t SUPERTMR_DRV_FaultSelectSourceTrgmux(uint32_t instance, bool flt0, bool flt1, bool flt2);

/**
 * @brief This function will enable the generation a list of interrupts.
 * It includes the SUPERTMR overflow interrupts, the reload point interrupt, the fault
 * interrupt and the channel (n) interrupt.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  interruptMask The mask of interrupt. This is a logical OR of members of the
 *            enumeration ::supertmr_interrupt_option_t.
 * @return status_t STATUS_SUCCESS : Completed successfully.
 */
status_t SUPERTMR_DRV_EnableInterrupts(uint32_t instance, uint32_t interruptMask);

/**
 * @brief This function is used to disable some interrupts.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  interruptMask The mask of interrupt. This is a logical OR of members of the
 *            enumeration ::supertmr_interrupt_option_t.
 */
void SUPERTMR_DRV_DisableInterrupts(uint32_t instance, uint32_t interruptMask);

/**
 * @brief This function will get the enabled SUPERTMR interrupts.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @return uint32_t The enabled interrupts. This is the logical OR of members of the
 *         enumeration ::supertmr_interrupt_option_t.
 */
uint32_t SUPERTMR_DRV_GetEnabledInterrupts(uint32_t instance);

/**
 * @brief This function will get the SUPERTMR status flags.
 * @note: Regarding the duty cycle is 100% at the channel output, the match interrupt
 * has no event due to the C(n)V and C(n+1)V value are not between CNTIN value and MOD value.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @return uint32_t The status flags. This is the logical OR of members of the
 *         enumeration ::supertmr_status_flag_t.
 */
uint32_t SUPERTMR_DRV_GetStatusFlags(uint32_t instance);

/**
 * @brief This function is used to clear the SUPERTMR status flags.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  flagMask The status flags to clear. This is a logical OR of members of the
 *            enumeration ::supertmr_status_flag_t.
 */
void SUPERTMR_DRV_ClearStatusFlags(uint32_t instance, uint32_t flagMask);

/**
 * @brief Retrieves the frequency of the clock source feeding the SUPERTMR counter.
 *
 * Function will return a 0 if no clock source is selected and the SUPERTMR counter is disabled.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @return uint32_t The frequency of the clock source running the SUPERTMR counter (0 if counter is disabled).
 */
uint32_t SUPERTMR_DRV_GetFrequency(uint32_t instance);

/**
 * @brief This function is used to covert the given frequency to period in ticks.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  frequencyHz Frequency value in Hz.
 * @return uint16_t The value in ticks of the frequency.
 */
uint16_t SUPERTMR_DRV_ConvertFreqToPeriodTicks(uint32_t instance, uint32_t frequencyHz);

/**
 * @brief This function will allow the SUPERTMR to restart the counter to
 * its initial counting value in the register.
 * Note that the configuration is set in the SUPERTMR_DRV_SetSync() function to make
 * sure that the SUPERTMR registers are updated by software trigger or hardware trigger.
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  softwareTrigger Selects the software trigger or hardware trigger to update COUNT register.
 * true: A software trigger is generate to update register
 * false: A software trigger is not implemented and need to update later or
 *        select a hardware trigger and waiting an external trigger for updating register.
 * @return status_t STATUS_SUCCESS : Completed successfully.
 */
status_t SUPERTMR_DRV_CounterReset(uint32_t instance, bool softwareTrigger);

/**
 * @brief This function set supertmr output modulation with supertmr2 ch1
 *
 * @param  instance SUPERTMR 3 or SUPERTMR 0
 * @param  channelMask The mask which will configure the channels which will be modulated with supertmr2 ch1
 * @return status_t STATUS_SUCCESS : Completed successfully.
 */
static inline status_t SUPERTMR_DRV_OutputModulationWithSuperTmr2Ch1(uint32_t instance, uint8_t channelMask)
{
    OS_ASSERT(3 == instance || 0 == instance);

    if (3 == instance) {
        TCSR->SUPERTMROPT1 = (TCSR->SUPERTMROPT1 & 0x00FFFFFF) | (uint32_t)channelMask << 24;
    } else if (0 == instance) {
        TCSR->SUPERTMROPT1 = (TCSR->SUPERTMROPT1 & 0xFF00FFFF) | (uint32_t)channelMask << 16;
    } else {
        /* Nothing to do */
    }
    return STATUS_SUCCESS;
}

/**
 * @brief This function set supertmr sync bit,
 *
 * @param  instance The SUPERTMR peripheral instance number.
 * @param  enable true will generate a trigger signal
 * @return status_t STATUS_SUCCESS : Completed successfully.
 */
static inline status_t SUPERTMR_DRV_SetSyncBit(uint32_t instance, bool enable)
{
    OS_ASSERT(instance < SUPERTMR_INSTANCE_COUNT);
    switch (instance) {
    case 0:
        TCSR->SUPERTMROPT1 = (TCSR->SUPERTMROPT1 & (~TCSR_SUPERTMROPT1_SUPERTMR0SYNCBIT_MASK)) | TCSR_SUPERTMROPT1_SUPERTMR0SYNCBIT(enable);
        break;
    case 1:
        TCSR->SUPERTMROPT1 = (TCSR->SUPERTMROPT1 & (~TCSR_SUPERTMROPT1_SUPERTMR1SYNCBIT_MASK)) | TCSR_SUPERTMROPT1_SUPERTMR1SYNCBIT(enable);
        break;
    case 2:
        TCSR->SUPERTMROPT1 = (TCSR->SUPERTMROPT1 & (~TCSR_SUPERTMROPT1_SUPERTMR2SYNCBIT_MASK)) | TCSR_SUPERTMROPT1_SUPERTMR2SYNCBIT(enable);
        break;
#if (SUPERTMR_INSTANCE_COUNT > 3)
    case 3:
        TCSR->SUPERTMROPT1 = (TCSR->SUPERTMROPT1 & (~TCSR_SUPERTMROPT1_SUPERTMR3SYNCBIT_MASK)) | TCSR_SUPERTMROPT1_SUPERTMR3SYNCBIT(enable);
        break;
#endif
#if (SUPERTMR_INSTANCE_COUNT > 4)
    case 4:
        TCSR->SUPERTMROPT1 = (TCSR->SUPERTMROPT1 & (~TCSR_SUPERTMROPT1_SUPERTMR4SYNCBIT_MASK)) | TCSR_SUPERTMROPT1_SUPERTMR4SYNCBIT(enable);
        break;
#endif
#if (SUPERTMR_INSTANCE_COUNT > 5)
    case 5:
        TCSR->SUPERTMROPT1 = (TCSR->SUPERTMROPT1 & (~TCSR_SUPERTMROPT1_SUPERTMR5SYNCBIT_MASK)) | TCSR_SUPERTMROPT1_SUPERTMR5SYNCBIT(enable);
        break;
#endif
    default:
        break;
    }

    return STATUS_SUCCESS;
}

/**
 * @brief This function set supertmr global load
 *
 * @param  enable true - enable supertmr global load, false - disable supertmr global load
 * @return status_t STATUS_SUCCESS : Completed successfully.
 */
static inline status_t SUPERTMR_DRV_EnableGlobalLoad(bool enable)
{
    TCSR->SUPERTMROPT1 = (TCSR->SUPERTMROPT1 & (~TCSR_SUPERTMROPT1_SUPERTMRGLDOK_MASK)) | TCSR_SUPERTMROPT1_SUPERTMRGLDOK(enable);
    return STATUS_SUCCESS;
}

/**
 * @brief This function set supertmr1 channel0 input source
 *
 * @param  sel SUPERTMRX_CH0_SEL_CH0 use channel0, SUPERTMRX_CH0_SEL_CMP0 use CMP0
 * @return status_t STATUS_SUCCESS : Completed successfully.
 */
static inline status_t SUPERTMR_DRV_Supertmr1CH0Sel(supertmr_ch0_sel_t sel)
{
    TCSR->SUPERTMROPT1 = (TCSR->SUPERTMROPT1 & (~TCSR_SUPERTMROPT1_SUPERTMR1CH0SEL_MASK)) | TCSR_SUPERTMROPT1_SUPERTMR1CH0SEL(sel);
    return STATUS_SUCCESS;
}

/**
 * @brief This function set supertmr2 channel0 input source
 *
 * @param  sel SUPERTMRX_CH0_SEL_CH0 use channel0, SUPERTMRX_CH0_SEL_CMP0 use CMP0
 * @return status_t STATUS_SUCCESS : Completed successfully.
 */
static inline status_t SUPERTMR_DRV_Supertmr2CH0Sel(supertmr_ch0_sel_t sel)
{
    TCSR->SUPERTMROPT1 = (TCSR->SUPERTMROPT1 & (~TCSR_SUPERTMROPT1_SUPERTMR2CH0SEL_MASK)) | TCSR_SUPERTMROPT1_SUPERTMR2CH0SEL(sel);
    return STATUS_SUCCESS;
}

/**
 * @brief This function set supertmr fault input source
 *
 * @param instance The SUPERTMR peripheral instance number.
 * @param  sel see supertmr_fault_sel_t
 * @return status_t STATUS_SUCCESS : Completed successfully.
 */
static inline status_t SUPERTMR_DRV_SupertmrFaultSel(uint32_t instance, supertmr_fault_sel_t sel)
{
    OS_ASSERT(3 >= instance);
    uint32_t shift     = 4 * instance;
    uint32_t mask      = 0x00000007 << shift;
    TCSR->SUPERTMROPT0 = (TCSR->SUPERTMROPT0 & (~mask)) | ((sel << shift) & mask);
    return STATUS_SUCCESS;
}


#if defined(__cplusplus)
}
#endif

#endif // __SUPERTMR_COMMON_H__
