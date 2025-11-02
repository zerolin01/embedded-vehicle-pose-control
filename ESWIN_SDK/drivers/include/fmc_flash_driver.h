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
 * @file fmc_flash_driver.h
 * @brief FMC flash driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 *
 *
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __FMC_FLASH_DRIVER_H__
#define __FMC_FLASH_DRIVER_H__

#include "../src/fmc/fmc_register.h"
#include "osal.h"
#include <stdbool.h>
#include <stdint.h>

/*
 * @brief Flash type.
 */
typedef enum {
    PFLASH_TYPE = 0,
    DFLASH_TYPE,
    ENVRAM_TYPE,
} flash_type_t;

#define STATUS_RANGE_ERROR (-1)

/** pflash MAIN sector size is 2Kbytes */
#if (defined(PLATFORM_EAM2011))
#define PFLASH_SECTOR_UNIT_SIZE 0x800
#define PFLASH_MAIN_SECTOR_SIZE 0x100000
#define PFLASH_MAIN_SECTOR_MASK 0xFFFFF
#define PFLASH_MAIN_SECTOR_NUM  512
#endif

/**
 * @brief Fmc controller init.
 * TODO:  confirm APB, AHB ,FMC configuration
 */

/** pflash NVR sector size is 32Kbytes */
#if (defined(PLATFORM_EAM2011))
#define PFLASH_NVR_SECTOR_SIZE 0x8000
#define PFLASH_NVR_SECTOR_MASK 0x7fff
#define PFLASH_NVR_SECTOR_NUM  16

#endif

/** dflash main sector size is 128Kbytes */
#define DFLASH_SECTOR_UNIT_SIZE 0x400
#define DFLASH_MAIN_SECTOR_SIZE 0x20000
#define DFLASH_MAIN_SECTOR_MASK 0x1ffff
#define DFLASH_MAIN_SECTOR_NUM  128

/** dflash nvr sector size is 16Kbytes */
#define DFLASH_NVR_SECTOR_SIZE 0x4000
#define DFLASH_NVR_SECTOR_MASK 0x3fff
#define DFLASH_NVR_SECTOR_NUM  16

/*
 * @brief Flash sector type.
 */
typedef enum {
    MAIN_SECTOR = 0,
    NVR_SECTOR,
} flash_area_t;

typedef enum {
    SECTOR_ERASE      = 0X0, /* sector erase */
    MAIN_SECTOR_ERASE = 0x1, /** Erase the entire main sector*/
    ALL_SECTOR_ERASE  = 0x3, /** Erase the entire flash contains main, nvr*/
} flash_erase_area_t;

/*
 * @brief FMC normal interrupt event
 * register:
 * FMC_NORMAL_INT_MSK
 */
typedef enum {
    FMC_EVENT_PFLASH_ERASE_DONE   = 0x1,  /* Enable or disable the interrupt triggered by PFlash erase operation */
    FMC_EVENT_DFLASH_ERASE_DONE   = 0x2,  /* Enable or disable the interrupt triggered by DFlash erase operation */
    FMC_EVENT_PFLASH_PROGRAM_DONE = 0x4,  /* Enable or disable the interrupt triggered by PFlash program operation */
    FMC_EVENT_DFLASH_PROGRAM_DONE = 0x8,  /* Enable or disable the interrupt triggered by DFlash program operation */
    FMC_EVENT_EEPROM_COPY_DONE    = 0x10, /* Enable or disable the interrupt triggered by EEPROM copy operation */
} fmc_event_t;

/**
 * @brief FMC error interrupt error callback
 *  reg.FMC_WARNING_INT_MSK
 *  reg.FMC_ERROR_INT_RAW
 */
typedef enum {

    FMC_EVENT_PFLASH_WR_PROTECT     = 0x10, /* Enable or disable pFlash write protection violation interrupt */
    FMC_EVENT_DFLASH_WR_PROTECT     = 0x20, /* Enable or disable dFlash write protection violation interrupt */
    FMC_EVENT_PFLASH_MULTI_1BIT_ECC = 0x2,
    FMC_EVENT_DFLASH_MULTI_1BIT_ECC = 0x8,
    FMC_EVENT_OTP_CMP_ERR_EVENT     = 0x2, /* Enable or disable interrupt triggered by OTP compare error */
    FMC_EVENT_OTP_ECC_ERR_EVENT     = 0x3, /* Enable or disable interrupt triggered by OTP ECC error */
    FMC_EVENT_PFLASH_2BIT_ECC       = 0x4, /* Enable or disable interrupt triggered by pFlash 2-bit ECC error */
    FMC_EVENT_DFLASH_2BIT_ECC       = 0x5, /* Enable or disable interrupt triggered by dFlash 2-bit ECC error */
    FMC_EVENT_SRAM_2BIT_ECC         = 0x6, /* Enable or disable interrupt triggered by SRAM 2-bit ECC error */
} fmc_error_event_t;

/**
 *
 * @brief FMC Clock Source selection.
 */
typedef enum {
    FMC_CLOCKSOURCE_40M  = 0x03U, /* 40MHz */
    FMC_CLOCKSOURCE_48M  = 0x02U, /* 48MHz */
    FMC_CLOCKSOURCE_80M  = 0x01U, /* 80MHz */
    FMC_CLOCKSOURCE_160M = 0x00U, /* 160MHz */
} fmc_clocksource_t;

/**
 * @brief The type of interrupt function to register.
 *
 * FMC_INT_NORMAL:
 *  FMC_NORMAL_INT_MSK register correlation.
 *
 * FMC_INT_WARNING:
 *  FMC_WARNING_INT_MSK register correlation.
 *
 * FMC_INT_OTP:
 *  FMC_ERROR_INT_RAW[otp_ecc_err] and FMC_ERROR_INT_RAW[otp_cmp_err] register correlation.
 *
 * FMC_INT_NMI:
 *   FMC_ERROR_INT_RAW[sram_2bit_ecc], FMC_ERROR_INT_RAW[dFlash_2bit_ecc] and
 *   FMC_ERROR_INT_RAW[pFlash_2bit_ecc] register correlation.
 *
 */
typedef enum {
    FMC_INT_NORMAL  = 0x00U,
    FMC_INT_WARNING = 0x01U,
    FMC_INT_OTP_NMI = 0x02U,
} fmc_interrupt_type_t;

/*
 * @brief Definition for the fmc otp interrupt callback function
 */
typedef void (*fmc_errCallback_t)(void *parameter, fmc_error_event_t fmcErrorEvent);

/* FMC user config struct */
typedef struct {

    fmc_clocksource_t clockSelect; /* FMC clock selection */
    bool pFlashEcc;                /* Enable or disable the ECC for the program flash */
    bool dFlashEcc;                /* Enable or disable the ECC for the data flash*/
    bool sRAMEcc;                  /* Enable or disable the ECC fot the SRAM */
    bool pFlashPreFetch;           /* Enable or disable the prefetch for the program flash */
    bool dFlashPreFetch;           /* Enable or disable the prefetch for the data flash */
    uint32_t eccOneBitErrThod;     /* 1 bit ECC Error threshold*/
    fmc_errCallback_t errCallBack; /* if an error happened, this is the error interrupt callback function */
    void *errParameter;            /* Arguments to the errorCallBack function */
} fmc_config_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  FMC_DRV_InstallErrCallback
 *
 * @param  errCallback   Callback function.
 * @param  errParameter  The argument to the callback function.
 * @return ** status_t
 *         STATUS_SUCCESS  if succeed.
 *         STATUS_ERRORPAR if errCallback is NULL.
 */
status_t FMC_DRV_InstallErrCallback(fmc_errCallback_t errCallback, void *errParameter);

/**
 * @brief  FMC_DRV_PflashEraseSector
 *
 * pFlash sector erase, every erase sector size is 2K bytes.
 *
 * @param  flashArea        MAIN sector or NVR sector.
 * @param  flashAddr        Flash address
 * @param  eraseSectorNum   Number of erase sectors.
 * @return ** status_t   Generic operation status.
 */
status_t FMC_DRV_PflashEraseSector(flash_area_t flashArea, uint32_t flashAddr, uint32_t eraseSectorNum);

/**
 * @brief Erase an area of pFlash
 *
 * @param  flashArea   MAIN area.
 *                     MAIN and NVR area.
 * @return ** status_t Generic operation status.
 */
status_t FMC_DRV_PflashEraseEntireSector(flash_erase_area_t flashArea);

/**
 * @brief Pflash sector read.
 *
 * @param  flashArea   MAIN or NVR sector.
 * @param  flashAddr   Flash address.
 * @param  recvBuf     Receive buffer.
 * @param  size        Data size to read.
 * @return ** status_t
 */
status_t FMC_DRV_PflashSectorRead(flash_area_t flashArea, uint32_t flashAddr, void *recvBuf, uint32_t size);

/**
 * @brief Pflash sector write.
 *
 * MAIN sector total size 1MByte and one sector is 2K bytes,addr(0-0x100000-1),.
 * NVR sector total size 32KByte and one sector is 2K bytes,addr(0-0x8000-1).
 *
 * @param  flashArea  MAIN sector or NVR sector.
 * @param  flashAddr  MAIN sector(0-0x100000-1), NVR sector(0-0x8000-1).
 * @param  writeBuf   Pointer to data buffer.
 * @param  size       writeBuf size.
 * @return ** status_t Status return codes.
 */
status_t FMC_DRV_PflashSectorWrite(flash_area_t flashArea, uint32_t flashAddr, void *writeBuf, uint32_t size);

/**
 * @brief Pflash sector protect, total 32(range 0-31) protect area, every area is 32K.
 *
 * @param  startProtectArea (0-31).
 * @param  endProtectArea (0-31).
 * @return uint32_t Generic operation status. -1: address range error.
 */
status_t FMC_DRV_PflashSectorProtect(uint32_t startProtectArea, uint32_t endProtectArea);

/**
 * @brief Pflash sector unprotected, total 32(range 0-31) protect area, every area is 32K.
 *
 * @param  startProtectArea (0-31).
 * @param  endProtectArea (0-31).
 * @return status_t Generic operation status.
 */
status_t FMC_DRV_PflashSectorUnprotected(uint32_t startProtectArea, uint32_t endProtectArea);

/**
 * @brief Dflash sector protect, total 8(range 0-7) protect area, every area is 2K.
 *
 * @param  startProtectArea (0-7).
 * @param  endProtectArea (0-7).
 * @return status_t Generic operation status.
 */
status_t FMC_DRV_DflashSectorProtect(uint32_t startProtectArea, uint32_t endProtectArea);

/**
 * @brief Dflash sector unprotected, total 8(range 0-7) protect area, every area is 2K.
 *
 * @param  startProtectArea (0-7).
 * @param  endProtectArea (0-7).
 * @return status_t Generic operation status.
 */
status_t FMC_DRV_DflashSectorUnprotected(uint32_t startProtectArea, uint32_t endProtectArea);

/** DFLASH  interface. */

/**
 * @brief FMC_DRV_DflashEraseSector.
 *
 * Pflash sector erase, every sector size is 2K bytes.
 *
 * @param  flashArea      MAIN or NVR sector.
 * @param  flashAddr      Flash address.
 * @param  eraseSectorNum Number of erase sectors.
 * @return ** status_t  Generic operation status. STATUS_FLASH_ERASE_FAIL: Erase fail.
 */
status_t FMC_DRV_DflashEraseSector(flash_area_t flashArea, uint32_t flashAddr, uint32_t eraseSectorNum);

/**
 * @brief Erase an area of pFlash
 *
 * @param  flashArea   MAIN area.
 *                     MAIN and NVR area.
 * @return ** status_t Generic operation status.
 */
status_t FMC_DRV_DflashEraseEntireSector(flash_erase_area_t flashArea);

/**
 * @brief Dflash sector read.
 *
 * @param  flashArea MAIN sector or NVR sector.
 * @param  flashAddr MAIN(0x0 ~ 0x1FFFF),NVR(0x0 ~ 0x3FFF).
 * @param  recvBuf   Receive buffer.
 * @param  size      Data size to read.
 * @return ** status_t Generic operation status.
 */
status_t FMC_DRV_DflashSectorRead(flash_area_t flashArea, uint32_t flashAddr, void *recvBuf, uint32_t size);

/**
 * @brief dFlash sector write.
 *
 * @param  flashArea  MAIN sector or NVR sector.
 * @param  flashAddr  MAIN sector(0-0x20000-1),nvr sector(0-0x4000-1)
 * @param  writeBuf   pointer to data to be write
 * @param  size       writeBuf size
 * @return status_t  Status return codes.
 */
status_t FMC_DRV_DflashSectorWrite(flash_area_t flashArea, uint32_t flashAddr, void *writeBuf, uint32_t size);

/**
 * @brief Enable/Disable FMC ECC.
 *
 * @param  fmcType   pFlash, dFlash or eNVRAM.
 * @param  EccStatus  true or false.
 * @return ** void
 */
void FMC_DRV_FMCEcc(flash_type_t fmcType, bool EccEnable);

/**
 * @brief Set the fmc clock
 *
 * Users do not need to call this function
 *
 * @return status_t Status return codes.
 */
status_t FMC_DRV_UpdateClockMode(void);
status_t FMC_DRV_SetClockMode(fmc_clocksource_t clockSource);

#ifdef __cplusplus
}
#endif

#endif