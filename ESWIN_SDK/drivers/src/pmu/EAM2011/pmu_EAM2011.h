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
 * @file pmu_EAM2011.h
 * @brief PMU EAM2011 driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __PMU_EAM2011_H__
#define __PMU_EAM2011_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "emps_platform.h"
#include "status.h"
#include "wakeup_EAM2011.h"

#define PMU_BOR_MIN_DETECTION_VOL ((float)2.4)
#define PMU_BOR_VOL_TRIMMING_STEP (0.2)
#define PMU_PVD_MIN_DETECTION_VOL ((float)2.4)
#define PMU_PVD_VOL_TRIMMING_STEP (0.2)

/**
 * @brief Definition for the PMU callback function.
 *
 * Prototype for the callback function registered in the PMU driver.
 */
typedef void (*pmu_callback_t)(void *parameter, uint32_t intrStatus);

/**
 * @brief Brown-out rest detection voltage selection
 *
 * Trimming step is 200mV
 */
typedef enum {
    PMU_BOR_DET_VOL_2_4V = 0x00U, /**< Detection voltage 2.4V. */
    PMU_BOR_DET_VOL_3_6V = 0x06U, /**< Detection voltage 3.6V. */
    PMU_BOR_DET_VOL_4_2V = 0x09U, /**< Detection voltage 4.2V. */
    PMU_BOR_DET_VOL_5_4V = 0x0FU, /**< Detection voltage 5.4V. */
} pmu_bor_detection_vol_t;

/**
 * @brief Programmable Voltage Detector detection voltage selection
 *
 * Trimming step is 200mV
 */
typedef enum {
    PMU_PVD_DET_VOL_2_4V = 0x00U, /**< Detection voltage 2.4V. */
    PMU_PVD_DET_VOL_3_6V = 0x06U,
    PMU_PVD_DET_VOL_4_2V = 0x09U, /**< Detection voltage 3.6V. */
    PMU_PVD_DET_VOL_5_4V = 0x0FU, /**< Detection voltage 5.4V. */
} pmu_pvd_detection_vol_t;

/**
 * @brief Programmable Voltage Detector detection voltage selection
 *
 * Trimming step is 200mV
 */
typedef enum {
    PMU_LDOC_VOL_0_85V  = 0x00U, /**< Detection voltage 2.4V. */
    PMU_LDOC_VOL_1_1V   = 0x0AU, /**< Detection voltage 3.6V. */
    PMU_LDOC_VOL_1_225V = 0x0FU, /**< Detection voltage 5.4V. */
} pmu_ldoc_output_vol_t;

/**
 * @brief Programmable Voltage Detector detection voltage selection
 *
 * Trimming step is 200mV
 */
typedef enum {
    PMU_LDOF_VOL_1_8V = 0x00U, /**< Detection voltage 1.8V. */
    PMU_LDOF_VOL_3_0V = 0x0AU, /**< Detection voltage 3.0V. */
    PMU_LDOF_VOL_3_6V = 0x0FU, /**< Detection voltage 3.6V. */
} pmu_ldof_output_vol_t;

/**
 * @brief Programmable Voltage Detector detection voltage selection
 *
 * Trimming step is 200mV
 */
typedef enum {
    PMU_VREF10_VOL_0_85V   = 0x00U, /**< VREF10 voltage 0.85V. */
    PMU_VREF10_VOL_1_0V    = 0x08U, /**< VREF10 voltage 1.0V. */
    PMU_VREF10_VOL_1_0875V = 0x0FU, /**< VREF10 voltage 1.0875V. */
} pmu_verf10_output_vol_t;

/**
 * @brief Programmable Voltage Detector detection voltage selection
 *
 * Trimming step is 200mV
 */
typedef enum {
    PMU_BGP_REF_VOL_0_498V = 0x00U, /**< Detection voltage 2.4V. */
    PMU_BGP_REF_VOL_0_600V = 0x10U, /**< Detection voltage 3.6V. */
    PMU_BGP_REF_VOL_0_697V = 0x1FU, /**< Detection voltage 5.4V. */
} pmu_bgp_ref_vol_t;

/**
 * @brief Source selection for the sleep mode
 */
typedef enum {
    PMU_SLEEP_MODE_SW_TRG  = 0x00U, /**< Software trigger. */
    PMU_SLEEP_MODE_WFI_TRG = 0x01U, /**< WFI trigger. */
} pmu_sleep_mode_trigger_t;

/**
 * @brief Source selection for the sleep mode
 */
typedef enum {
    PMU_STANDBY_WAKEUP_RTC          = 0x00U, /**< RTC. */
    PMU_STANDBY_WAKEUP_PORTD_28_PIN = 0x01U, /**< PORTD_28. */
    PMU_STANDBY_WAKEUP_PORTD_21_PIN = 0x02U, /**< PORTD_21. */
    PMU_STANDBY_WAKEUP_PORTD_13_PIN = 0x03U, /**< PORTD_13. */
    PMU_STANDBY_WAKEUP_PORTD_07_PIN = 0x04U, /**< PORTD_07. */
    PMU_STANDBY_WAKEUP_PORTC_19_PIN = 0x05U, /**< PORTC_19. */
    PMU_STANDBY_WAKEUP_PORTC_02_PIN = 0x06U, /**< PORTC_02. */
    PMU_STANDBY_WAKEUP_PORTA_24_PIN = 0x07U, /**< PORTA_24. */
    PMU_STANDBY_WAKEUP_PORTA_18_PIN = 0x08U, /**< PORTA_18. */
    PMU_STANDBY_WAKEUP_PORTA_09_PIN = 0x09U, /**< PORTA_09. */
} pmu_standby_wakeup_source_t;

/**
 * @brief PMU BOR configuration.
 */
typedef struct {
    bool enable;             /**< BOR Voltage Detection state */
    bool rstnSource;         /**< BOR_RSTN as a reset source or not. */
    bool interruptEnable;    /**< BOR interrupt enable. */
    uint32_t detectionVol;   /**< BOR Detection Voltage 2.V-5.4V (Trimming step = 200mv). */
    void *callbackParam;     /**< callback parameters */
    pmu_callback_t callback; /**< The callback of interrupt */
} pmu_bor_config_t;

/**
 * @brief PMU PVD configuration.
 */
typedef struct {
    bool enable;             /**< PVD Voltage Detection state */
    bool interruptEnable;    /**< PVD interrupt enable. */
    uint32_t detectionVol;   /**< PVD Detection Voltage 2.V-5.4V (Trimming step = 200mv). */
    void *callbackParam;     /**< callback parameters */
    pmu_callback_t callback; /**< The callback of interrupt */
} pmu_pvd_config_t;

/**
 * @brief PMU MBGP configuration.
 */
typedef struct {
    uint32_t referenceVol; /**< MBGP Reference Voltage 498mV-697mV (Trimming step = 6.4mv). */
} pmu_mbgp_config_t;

/**
 * @brief PMU LPBGP configuration.
 */
typedef struct {
    uint32_t referenceVol; /**< MBGP Reference Voltage 498mV-697mV (Trimming step = 6.4mv). */
} pmu_lpbgp_config_t;

/**
 * @brief PMU LPLDO configuration.
 */
typedef struct {
    uint32_t referenceVol; /**< MBGP Reference Voltage 0.85V-1.225V (Trimming step = 6.4mv). */
} pmu_lpldo_config_t;

/**
 * @brief PMU LDOC configuration.
 */
typedef struct {
    bool currentLimitEnable; /**< Enable current limit */
    bool enableVref10A;      /**< VREF10A buffering state */
    bool enableVref12A;      /**< VREF12A buffering state. */
    uint32_t vref10AVol;     /**< PVD Detection Voltage 0.9V-1.0875V (Trimming step = 12.5mv). */
    uint32_t v11CoreVol;     /**< MBGP Reference Voltage 0.85V-1.225V (Trimming step = 6.4mv). */
} pmu_ldoc_config_t;

/**
 * @brief PMU LDOF configuration.
 */
typedef struct {
    bool currentLimitEnable; /**< Enable current limit */
    uint32_t v33FlashVol;    /**< MBGP Reference Voltage 0.85V-1.225V (Trimming step = 6.4mv). */
} pmu_ldof_config_t;

/**
 * @brief PMU FSM configuration.
 */
typedef struct {
    bool fsmCntValOverride;
    bool fsmCountingEnable;
    uint32_t fsmCountVal;
} pmu_fsm_config_t;

/**
 * @brief PMU Standby configuration.
 */
typedef struct {
    uint32_t wakeupSource;
    void *callbackParam;     /**< callback parameters */
    pmu_callback_t callback; /**< The callback of interrupt */
} pmu_standby_config_t;

/**
 * @brief PMU Sleep configuration.
 */
typedef struct {
    pmu_sleep_mode_trigger_t source;
} pmu_sleep_config_t;

/**
 * @brief PMU reboot configuration.
 */
typedef struct {
    bool rebootCntEnable;    /**< Enable reboot counter */
    bool interruptEnable;    /**< Enable interrupts */
    void *callbackParam;     /**< callback parameters */
    pmu_callback_t callback; /**< The callback of interrupt */
} pmu_reboot_config_t;

/**
 * @brief PMU Voltage Detector configuration.
 */
typedef struct {
    pmu_bor_config_t borConfig; /**< BOR configuration. */
    pmu_pvd_config_t pvdConfig; /**< PVD configuration. */
} pmu_vol_det_config_t;

/**
 * @brief PMU LDO configuration.
 */
typedef struct {
    pmu_ldoc_config_t ldocConfig;   /**< LDOC configuration. */
    pmu_ldof_config_t ldofConfig;   /**< LDOF configuration. */
    pmu_lpldo_config_t lpldoConfig; /**< LPLDO configuration. */
} pmu_ldo_config_t;

/**
 * @brief PMU LDO configuration.
 */
typedef struct {
    pmu_mbgp_config_t mbgpConfig;   /**< MBGP configuration. */
    pmu_lpbgp_config_t lpbgpConfig; /**< LPBGP configuration. */
} pmu_bandgap_config_t;

/**
 * @brief PMU Mode configuration.
 */
typedef struct {
    pmu_sleep_config_t sleepConfig;     /**< Sleep mode configuration. */
    pmu_standby_config_t standbyConfig; /**< Standby mode configuration. */
} pmu_mode_config_t;

/**
 * @brief PMU CSR configuration.
 */
typedef struct {
    bool oscEnable;                   /**< Enable osc. */
    pmu_reboot_config_t rebootConfig; /**< Reboot configuration. */
} pmu_csr_config_t;

/**
 * @brief PMU user configuration structure.
 * This structure is used when calling the PMU_SYS_Init function.
 */
typedef struct {
    bool lbistClampEnable;
    pmu_vol_det_config_t voldetConfig;
    pmu_bandgap_config_t bgpConfig;
    pmu_ldo_config_t ldoConfig;
    pmu_fsm_config_t fsmConfig;
    pmu_mode_config_t modeConfig;
    pmu_csr_config_t csrConfig;
} pmu_user_config_t;

typedef enum {
    POWER_MANAGER_RUN,     /**< Run mode. */
    POWER_MANAGER_SLEEP,   /**< Sleep mode.  */
    POWER_MANAGER_STANDBY, /**< Standby mode.  */
    POWER_MANAGER_MAX
} power_manager_modes_t;

/**
 * @brief Power mode user configuration structure.
 *
 * List of power mode configuration structure members depends on power options available
 * for the specific chip. Complete list contains:
 * mode - EAM2011 power mode. List of available modes is chip-specific. See power_manager_modes_t
 * list of modes.
 * sleepOnExitOption - Controls whether the sleep-on-exit option value is used(when set to true)
 * or ignored(when set to false). See sleepOnExitValue.
 * sleepOnExitValue - When set to true, ARM core returns to sleep (EAM2011 wait modes) or deep sleep
 * state (EAM2011 stop modes) after interrupt service finishes. When set to false, core stays
 * woken-up.
 */
typedef struct {
    power_manager_modes_t powerMode; /**< Power modes. */
} power_manager_user_config_t;

/**
 * @brief Initializes.
 *
 * This function performs the actual implementation-specific initialization.
 *
 * @return STATUS_ERROR or STATUS_SUCCESS.
 */
status_t POWER_SYS_DoInit(void);

/**
 * @brief Deinitializes.
 *
 * This function performs the actual implementation-specific de-initialization.
 *
 * @return STATUS_ERROR or STATUS_SUCCESS.
 */
status_t POWER_SYS_DoDeinit(void);


/**
 * @brief Configuration the PMU mode.
 *
 * This function configuration the PMU mode (sleep or standby).
 *
 * @param config User configuration structure for power manager.
 * @return status_t Error code.
 */
status_t POWER_SYS_DoSetMode(const power_manager_user_config_t *const config);

/**
 * @brief Configuration the PMU BOR.
 *
 * This function configuration the PMU BOR.
 *
 * @param borConfig User configuration structure for BOR.
 * @return status_t Error code.
 */
status_t PMU_SYS_SetBorConfiguration(const pmu_bor_config_t *borConfig);

/**
 * @brief Configuration the PVD.
 *
 * This function configuration the PVD.
 *
 * @param pvdConfig User configuration structure for PVD.
 *
 * @return status_t Error code.
 */
status_t PMU_SYS_SetPvdConfiguration(const pmu_pvd_config_t *pvdConfig);

#if 0
/*
 *
 * Function Name : PMU_SYS_SetMbgpConfiguration
 * Description   : Configuration the PMU MBGP.
 *
 */
status_t PMU_SYS_SetMbgpConfiguration(const pmu_mbgp_config_t *mbgpConfig);

/*
 *
 * Function Name : PMU_SYS_SetLpbgpConfiguration
 * Description   : Configuration the PMU LPBGP.
 *
 */
status_t PMU_SYS_SetLpbgpConfiguration(const pmu_lpbgp_config_t *lpbgpConfig);

/*
 *
 * Function Name : PMU_SYS_SetLdocConfiguration
 * Description   : Configuration the PMU LDO Core.
 *
 */
status_t PMU_SYS_SetLdocConfiguration(const pmu_ldoc_config_t *ldocConfig);

/*
 *
 * Function Name : PMU_SYS_SetLdofConfiguration
 * Description   : Configuration the PMU LDO Flash.
 *
 */
status_t PMU_SYS_SetLdofConfiguration(const pmu_ldof_config_t *ldofConfig);

/*
 *
 * Function Name : PMU_SYS_SetFsmConfiguration
 * Description   : Configuration the PMU FSM.
 *
 */
status_t PMU_SYS_SetFsmConfiguration(const pmu_fsm_config_t *fsmConfig);
#endif

/**
 * @brief Configuration the PMU sleep mode.
 *
 * This function configuration the PMU mode sleep mode.
 *
 * @param sleepConfig User configuration structure for sleep config.
 * @return status_t Error code.
 */
status_t PMU_SYS_SetSleepModeConfiguration(const pmu_sleep_config_t *sleepConfig);

/**
 * @brief Configuration the PMU standby mode.
 *
 * This function configuration the PMU mode standby mode.
 *
 * @param standbyConfig User configuration structure for standby config.
 * @return status_t Error code.
 */
status_t PMU_SYS_SetStandbyConfiguration(const pmu_standby_config_t *standbyConfig);

/*
 *
 * Function Name : PMU_SYS_SetCsrConfiguration
 * Description   : Configuration the PMU CSR.
 *
 */
status_t PMU_SYS_SetCsrConfiguration(const pmu_csr_config_t *csrConfig);

/**
 * @brief Configure the detector voltage of the BOR.
 *
 * This function configuration the detector voltage of the BORs.
 *
 * @param voltage Detector voltage.
 * @return status_t Error code.
 */
status_t PMU_SYS_SetBorDetectorVol(const uint32_t voltage);

/*
 *
 * Function Name : PMU_SetBorRsntSourceState
 * Description   : Enable the wakeup sourced for STANDBY mode
 *
 */
/**
 * @brief Enable/disable the BOR as reset source.
 *
 * This function configuration the BOR reset source.
 *
 * @param state new enable/disable state.
 * @return status_t Error code.
 */
status_t PMU_SYS_SetBorRsntSourceEnable(const bool state);

/**
 * @brief Configure the detector voltage of the PVD.
 *
 * This function configuration the detector voltage of the PVD.
 *
 * @param voltage Detector voltage.
 * @return status_t Error code.
 */
status_t PMU_SYS_SetPvdDetectorVol(const uint32_t voltage);

/**
 * @brief Configure trigger source for sleep mode.
 *
 * This function configuration the trigger source for sleep mode.
 *
 * @param source trigger source.
 * @return status_t Error code.
 */
status_t PMU_SYS_SetSleepModeTriggerSource(const pmu_sleep_mode_trigger_t source);

/**
 * @brief Trigger the chip run into Sleep mode..
 *
 * This function trigger the chip run into Sleep mode.
 *
 * @return status_t Error code.
 */
status_t PMU_SYS_TriggerSleepMode(void);

/**
 * @brief Enable the wakeup sources for STANDBY mode.
 *
 * This function configuration the wakeup sources for STANDBY mode.
 *
 * @param sources wakeup sources.
 * @return status_t Error code.
 */
status_t PMU_SYS_SetWakeupSource(const uint32_t sources);

/**
 * @brief Disable the wakeup sources for STANDBY mode.
 *
 * This function configuration the wakeup sources for STANDBY mode.
 *
 * @param sources wakeup sources.
 * @return status_t Error code.
 */
status_t PMU_SYS_DisableWakeupSource(const uint32_t sources);

/**
 * @brief Trigger the chip run into STANDBY mode.
 *
 * This function trigger the chip run into STANDBY mode.
 *
 * @return status_t Error code.
 */
status_t PMU_SYS_TriggerStandbyMode(void);

/**
 * @brief Gets standy mode counter
 *
 * This function returns standby counter.
 *
 * @param count  standby counter.
 * @return status_t Error code.
 */
status_t PMU_SYS_GetStandbyCount(uint32_t *count);

/**
 * @brief Trigger software reset.
 *
 * This function trigger the chip run software reset.
 *
 * @return status_t Error code.
 */
status_t PMU_SYS_TriggerSoftwareReset(void);

#endif
