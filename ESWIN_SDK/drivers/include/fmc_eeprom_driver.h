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
 * @file fmc_eeprom_driver.h
 * @brief FMC eeprom driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __FMC_EEPROM_DRIVER_H__
#define __FMC_EEPROM_DRIVER_H__

#include "../src/fmc/fmc_register.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Erase cnt max is current sector, and db=0xffffffff is current db offset.
 *
 */
typedef struct datablok_status {
    uint32_t sectorIndex; /**< Current dflash sector index. */
    uint32_t dbOffset;    /**< Data block byte offset in current sector. */
} data_block_status_t;

/**
 * @brief Data block type.
 *
 */
typedef struct data_block {
    uint16_t eeAddr : 12;
    uint16_t dbLength : 4;
    uint8_t data[6];
} data_block_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief EEPROM initialization
 *
 */
void FMC_DRV_EepromInit(void);

/**
 * @brief Get status of copy eeprom data from dflash to sram.
 *
 * @param  SectorArea Flash area, 0--frequently area or 1--slow area.
 * @return true: Done
 * @return false: not done
 */
bool FMC_DRV_EepromGetCopyDoneStatus(bool SectorArea);

/**
 * @brief Copy eeprom data from dflash to sram.
 *
 * @param  startSector Dflash start sector which copy from.
 * @param  SectorNum Number of sector to copy.
 * @param  SectorArea Flash area , 0--frequently area or  1--slow area.
 * @return status_t Status return codes.
 */
status_t FMC_DRV_EepromCopyFromDflashToNvram(uint8_t startSector, uint8_t SectorNum, bool SectorArea);

/**
 * @brief Get data_block_status_t which record the update status for dflash.
 *
 * @param  area 0 --frequency area, 1--fixed area.
 * @return data_block_status_t Current dflash sector index, and data block byte offset in current sector.
 */
data_block_status_t *FMC_DRV_GetEEPStatusInfo(bool area);

/**
 * @brief Write data to eeprom.
 *
 * @param  eepromAddr Eeprom address to be read, range (0-0xfff).
 * @param  pdata Pointer to data to be write.
 * @param  size Data size to be write.
 * @return status_t Status return codes.
 */
status_t FMC_DRV_EepromWrite(uint32_t eepromAddr, void *pdata, uint32_t size);

/**
 * @brief Read data from eeprom.
 *
 * @param  eepromAddr Eeprom address to be read, range (0-0xfff).
 * @param  pdata Pointer to data to be read.
 * @param  size Data size to be read.
 * @return status_t Status return codes.
 */
status_t FMC_DRV_EepromRead(uint32_t eepromAddr, void *pdata, uint32_t size);

/**
 * @brief This function clear all EEPROM data.
 * 
 * If the EEPROM or dflash bankup area is written full, the user 
 * may erase the EEPROM and backup area to achieve better EEPROM write speed.
 *
*/
status_t FMC_DRV_EepromClear(void);

#ifdef __cplusplus
}
#endif

#endif