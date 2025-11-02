
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
 * @file fmc_eeprom_driver.c
 * @brief FMC eeprom driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "fmc_eeprom_driver.h"
#include "fmc_flash_driver.h"

/****************************************Emulated EEPROM ***********************************************************
 * EEPROM is implemented through nvram and dflash simulation, which realizes the random read and write characteristic
 * of EEPROM. 
 * The size of the EEPROM is configured using the NVR. The maximum size is 4Kbytes.   
 * 
 * The EEPROM data will be updated to dflash in real time, and the data will be divided into two types, frequency 
 * updated data and infrequent updated data, respectively updated to different sectors of dflash. 
 * 
 *
 *         NVRAM                                    Dflash
 *     _____________  Frequent data update     ______________________             
 *    |             | ------------------>     |  Frequent data area |
 *    | EEPROM data |                         |—————————————————————|         
 *    |             |  ------------------>    | Fixed data area     |             
 *    |_____________|   Fixed data update     |_____________________|           
 *                   
 * 
 ************************************************************************************************************************           
*/

/** Global variables. */
/** Dflash start sector which store eeprom frequent change data. */
uint32_t g_frequentAreaStartSector = 0;
uint32_t g_frequentAreaSectorNum   = 0;
uint32_t g_frequentAreaEndSector   = 0;

/** Dflash start sector which store eeprom fixed data. */
uint32_t g_fixedAreaStartSector = 0;
uint32_t g_fixedAreaSectorNum   = 0;
uint32_t g_fixedAreaEndSector   = 0;
uint32_t g_magicNumber          = 0xdeadbeef;
/** EEPROM size which configured by user. */
uint32_t g_eepSize            = 0;
bool g_frequentAreaEnable     = false;
bool g_fixedAreaEnable        = false;
bool g_eepEnable              = false;
bool g_eepromFrequentHaveUsed = false;
bool g_eepromFixedHaveUsed    = false;
/** Dflash sector which store eeprom data erase count. */
uint32_t g_frequentAreaEraseCnt = 0;
uint32_t g_fixedAreaEraseCnt    = 0;

data_block_status_t g_frequentAreaStatus;
data_block_status_t g_fixedAreaStatus;

#define DB_DATA_SIZE                       (6U)
#define DFLASH_SECTOR_SIZE                 (1024U)
#define FLASH_SECTOR_TO_ADDRESS(sectorNum) (DFLASH_PERIPH_BASEADDR + ((sectorNum)*DFLASH_SECTOR_SIZE))
#define DFLASH_READ(addr)                  (*(volatile uint32_t *)(addr))
#define EEPROM_READ_4BYTE(addr)            (*(volatile uint32_t *)(ENVRAM_PERIPH_BASEADDR + ((addr)&0xfff)))
#define EEPROM_READ_2BYTE(addr)            (*(volatile uint16_t *)(ENVRAM_PERIPH_BASEADDR + ((addr)&0xfff)))
#define EEPROM_READ_1BYTE(addr)            (*(volatile uint8_t *)(ENVRAM_PERIPH_BASEADDR + ((addr)&0xfff)))
#define EEPROM_WRITE_4BYTE(addr, val)      (*(volatile uint32_t *)(ENVRAM_PERIPH_BASEADDR + ((addr)&0xfff))) = (val)
#define EEPROM_WRITE_2BYTE(addr, val)      (*(volatile uint16_t *)(ENVRAM_PERIPH_BASEADDR + ((addr)&0xfff))) = (val)
#define EEPROM_WRITE_1BYTE(addr, val)      (*(volatile uint8_t *)(ENVRAM_PERIPH_BASEADDR + ((addr)&0xfff))) = (val)

#define USER_EEPROM_CONFIG_ADDR (0x98 * 2)

#define USER_EEP_EN_MASK  (1U)
#define USER_EEP_EN_SHIFT (0U)
#define USER_EEP_EN_WIDTH (1U)
#define USER_EEP_EN(x)    (((uint32_t)(((uint32_t)(x)) << USER_EEP_EN_SHIFT)) & USER_EEP_EN_MASK)

#define USER_EEP_S0_EN_MASK  (1U)
#define USER_EEP_S0_EN_SHIFT (1U)
#define USER_EEP_S0_EN_WIDTH (1U)
#define USER_EEP_S0_EN(x)    (((uint32_t)(((uint32_t)(x)) << USER_EEP_S0_EN_SHIFT)) & USER_EEP_S0_EN_MASK)

#define USER_EEP_S1_EN_MASK  1U
#define USER_EEP_S1_EN_SHIFT 2U
#define USER_EEP_S1_EN_WIDTH 1U
#define USER_EEP_S1_EN(x)    (((uint32_t)(((uint32_t)(x)) << USER_EEP_S1_EN_SHIFT)) & USER_EEP_S1_EN_MASK)

#define USER_EEP_SIZE_MASK     0xffffU
#define USER_EEP_SIZE_SHIFT    16U
#define USER_EEP_SIZE_EN_WIDTH 16U
#define USER_EEP_SIZE_EN(x)    (((uint32_t)(((uint32_t)(x)) << USER_EEP_S1_EN_SHIFT)) & USER_EEP_S1_EN_MASK)

/** Static function. */
void FMC_DRV_EepromInit(void);
static void FMC_DRV_FrequentAreaStatusInit(void);
static void FMC_DRV_FixedAreStatusInit(void);
static status_t FMC_DRV_FrequentAreaUpdate(uint32_t sramAddr, uint32_t dbSize);
static status_t FMC_DRV_ProcessFrequentAreaNextSector(uint32_t nextSector);
static status_t FMC_DRV_FixedAreaUpdate(void *data);
static status_t FMC_DRV_EepromBackupFromSramToFlash(uint32_t sramAddr, uint32_t flashAddr, uint8_t size);
status_t FMC_DRV_EepromCopyFromDflashToNvram(uint8_t startSector, uint8_t SectorNum, bool SectorArea);
static uint32_t FMC_DRV_GetEraseCount(uint32_t sectorNum);
bool FMC_DRV_EepromGetCopyDoneStatus(bool SectorArea);
static bool FMC_DRV_GetEEPUserConfig(void);

/**
 * 
 * Function Name : FMC_DRV_EECompare
 * Description   : This function Compare the data in the 
 * EEPROM with the specified data. 
 * 
 */
static status_t FMC_DRV_EECompare(uint32_t eeAddr, uint8_t *str2, uint32_t len)
{
    uint32_t i;
    uint32_t val;

    for (i = 0; i < len; i++) {
        val = EEPROM_READ_1BYTE(eeAddr + i);
        if (val != str2[i]) {
            return STATUS_ERROR;
        }
    }
    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : FMC_DRV_EEWrite
 * Description   : This function writes the specified length of data 
 * to the specified EEPROM address.
 * 
 */
static void FMC_DRV_EEWrite(uint32_t eeAddr, uint8_t *pdata, uint32_t len)
{
    uint32_t i;

    for (i = 0; i < len; i++) {
        EEPROM_WRITE_1BYTE(eeAddr + i, pdata[i]);
    }
}

/**
 *
 * Function Name : FMC_DRV_GetEraseCount
 * Description   : This function get the erase count of specified dflash sector.
 * Erase count only use in Emulated EEPROM mode.
 * 
 */
static uint32_t FMC_DRV_GetEraseCount(uint32_t sectorNum)
{
    uint32_t eraseCnt;
    uint32_t dflashAddr;

    dflashAddr = FLASH_SECTOR_TO_ADDRESS(sectorNum);
    eraseCnt   = DFLASH_READ(dflashAddr);

    return eraseCnt;
}

/**
 *
 * Function Name : FMC_DRV_GetEEPStatusInfo
 * Description   : This function get the data_block_status_t which 
 * record the update status for dflash.
 * 
 * data_block_status_t contains the following information: 
 * --dflash sector index which will be writing
 * --data block byte offset in the sector
 * 
 * area 0  is frequency area and area 1 is fixed area.
 * 
 */
data_block_status_t *FMC_DRV_GetEEPStatusInfo(bool area)
{
    if (area) {
        return &g_fixedAreaStatus;
    } else {
        return &g_frequentAreaStatus;
    }
}

#if FPGA_PLT
/**
 *
 * Function Name : FMC_DRV_GetEEPUserConfig
 * Description   : This function obtain the Emulated EEPROM configuration 
 * parameters from the NVR.
 * 
 */
static bool FMC_DRV_GetEEPUserConfig(void)
{
    uint32_t eeConfig = 0x10000007;

    //    FMC_DRV_PflashNvrSectorRead(USER_EEPROM_CONFIG_ADDR, &eeConfig, 4);

    g_eepEnable          = (eeConfig >> USER_EEP_EN_SHIFT) & USER_EEP_EN_MASK;
    g_frequentAreaEnable = (eeConfig >> USER_EEP_S0_EN_SHIFT) & USER_EEP_S0_EN_MASK;
    g_fixedAreaEnable    = (eeConfig >> USER_EEP_S1_EN_SHIFT) & USER_EEP_S1_EN_MASK;
    g_eepSize            = (eeConfig >> USER_EEP_SIZE_SHIFT) & USER_EEP_SIZE_MASK;

    return true;
}
#else

/**
 *
 * Function Name : FMC_DRV_GetEEPUserConfig
 * Description   : This function obtain the Emulated EEPROM configuration 
 * parameters from the NVR.
 * 
 */
static bool FMC_DRV_GetEEPUserConfig(void)
{
    uint32_t eeConfig;
    // flash_area_t flash = NVR_SECTOR;
    // uint32_t eeConfig = 0x10000008;

    FMC_DRV_PflashSectorRead(NVR_SECTOR, USER_EEPROM_CONFIG_ADDR, &eeConfig, 4);
    g_eepEnable          = (~(eeConfig >> USER_EEP_EN_SHIFT)) & USER_EEP_EN_MASK;
    g_frequentAreaEnable = (~(eeConfig >> USER_EEP_S0_EN_SHIFT)) & USER_EEP_S0_EN_MASK;
    g_fixedAreaEnable    = (~(eeConfig >> USER_EEP_S1_EN_SHIFT)) & USER_EEP_S1_EN_MASK;
    g_eepSize            = (eeConfig >> USER_EEP_SIZE_SHIFT) & USER_EEP_SIZE_MASK;

    return true;
}
#endif

/**
 *
 * Function Name : FMC_DRV_FrequentAreaStatusInit
 * Description   : This function Initialize  g_frequentAreaStatus which contains 
 * secIndex-    the sector number which will be update by next update. 
 * dbOffset-    the data block offset in the sector which will be update by next update. 
 * g_frequentAreaEraseCnt - newest erase counter of the Frequent Area.
 *       
 */
static void FMC_DRV_FrequentAreaStatusInit(void)
{
    uint32_t i;
    uint32_t dflashAddr;
    uint32_t secIndex = g_frequentAreaStartSector;
    uint32_t eraseCnt = 0;
    uint32_t data     = 0;
    uint32_t ffCnt    = 0;
    uint32_t dbOffset = 8;

    for (i = 0; i < g_frequentAreaSectorNum; i++) {
        dflashAddr = FLASH_SECTOR_TO_ADDRESS(g_frequentAreaStartSector + i);
        data       = DFLASH_READ(dflashAddr);
        if (data == 0xffffffffUL) {
            ffCnt++;
            if (ffCnt >= 2) {
                break;
            }
        } else {
            if (data > eraseCnt) {
                eraseCnt = data;
                secIndex = g_frequentAreaStartSector + i;
            }
        }
    }
    g_frequentAreaStatus.sectorIndex = secIndex;
    g_frequentAreaEraseCnt           = eraseCnt;

    dflashAddr = FLASH_SECTOR_TO_ADDRESS(g_frequentAreaStartSector + i);
    for (i = 8; i < DFLASH_SECTOR_SIZE; i += 8) {
        data = DFLASH_READ((dflashAddr + i));
        if (data == 0xffffffffUL) {
            dbOffset = i;
            break;
        }
    }
    g_frequentAreaStatus.dbOffset = dbOffset;
    if (g_frequentAreaEraseCnt != 0) {
        g_eepromFrequentHaveUsed = true;
    } else {
        g_eepromFrequentHaveUsed = false;
    }
}

/**
 *
 * Function Name : FMC_DRV_FixedAreStatusInit
 * Description   : This function Initialize g_fixedAreaStatus which contains 
 * secIndex-    the sector number which will be update by FMC_DRV_FixedAreaUpdate
 * dbOffset-    the data block offset in the sector 
 * g_fixedAreaEraseCnt - newest erase counter of the fixed Area.
 *       
 */
static void FMC_DRV_FixedAreStatusInit(void)
{
    uint32_t i;
    uint32_t dflashAddr;
    uint32_t secIndex = g_fixedAreaStartSector;
    uint32_t eraseCnt = 0;
    uint32_t data     = 0;
    uint32_t ffCnt    = 0;
    uint32_t dbOffset = 8;

    for (i = 0; i < g_fixedAreaSectorNum; i++) {
        dflashAddr = FLASH_SECTOR_TO_ADDRESS(g_fixedAreaStartSector + i);
        data       = DFLASH_READ(dflashAddr);
        if (data == 0xffffffffUL) {
            ffCnt++;
            if (ffCnt >= 2) {
                break;
            }
        } else {
            if (data > eraseCnt) {
                eraseCnt = data;
                secIndex = g_fixedAreaStartSector + i;
            }
        }
    }

    g_fixedAreaStatus.sectorIndex = secIndex;
    g_fixedAreaEraseCnt           = eraseCnt;

    dflashAddr = FLASH_SECTOR_TO_ADDRESS(g_fixedAreaStartSector + i);
    for (i = 8; i < DFLASH_SECTOR_SIZE; i += 8) {
        data = DFLASH_READ((dflashAddr + i));
        if (data == 0xffffffffUL) {
            dbOffset = i;
            break;
        }
    }

    g_fixedAreaStatus.dbOffset = dbOffset;
    if (g_fixedAreaEraseCnt != 0) {
        g_eepromFixedHaveUsed = true;
    } else {
        g_eepromFixedHaveUsed = false;
    }
}

#if FPGA_PLT
/**
 *
 * Function Name : FMC_DRV_EepromInit
 * Description   : This function initialize configration for the Emulated EEPROM.
 *       
 */
void FMC_DRV_EepromInit(void)
{

    g_frequentAreaStartSector = 0;
    g_frequentAreaSectorNum   = 96;
    g_frequentAreaEndSector   = 95;
    g_fixedAreaStartSector    = 96;
    g_fixedAreaSectorNum      = 32;
    g_fixedAreaEndSector      = 127;

    FMC_DRV_GetEEPUserConfig();
    if (g_frequentAreaEnable == true) {

        FMC_DRV_FrequentAreaStatusInit();
    }
    if (g_fixedAreaEnable == true) {
        FMC_DRV_FixedAreStatusInit();
    }
}
#else
/**
 *
 * Function Name : FMC_DRV_EepromInit
 * Description   : This function initialize configration for the Emulated EEPROM.
 *       
 */
void FMC_DRV_EepromInit(void)
{
    fmc_eeprom_sector_cfg_type_t cfg;
    cfg.w = FMC->FMC_EEPROM_SECTOR_CFG;

    FMC_DRV_GetEEPUserConfig();

    if (g_frequentAreaEnable == true) {
        g_frequentAreaStartSector = cfg.b.sector_begin1;
        g_frequentAreaSectorNum   = cfg.b.sector_num1;
        g_frequentAreaEndSector   = g_frequentAreaStartSector + g_frequentAreaSectorNum - 1;
        FMC_DRV_FrequentAreaStatusInit();
    }
    if (g_fixedAreaEnable == true) {
        g_fixedAreaStartSector = cfg.b.sector_begin0;
        g_fixedAreaSectorNum   = cfg.b.sector_num0;
        g_fixedAreaEndSector   = g_fixedAreaStartSector + g_fixedAreaSectorNum - 1;
        FMC_DRV_FixedAreStatusInit();
    }
}
#endif

/**
 *
 * Function Name : FMC_DRV_EepromCopyFromDflashToNvram
 * Description   : This function restores EEPROM data from dflash to nvram.
 * 
 * SectorArea: true means sector0 area which is fixed data area
 *             false means sector1 area which is frequent data area 
 */
status_t FMC_DRV_EepromCopyFromDflashToNvram(uint8_t startSector, uint8_t SectorNum, bool SectorArea)
{
    fmc_eeprom_copy_cmd_cfg_type_t cfg;
    uint8_t sector_en;

    trace("FMC_DRV_EepromCopyFromDflashToNvram,startSec %x num %x area %x\r\n", startSector, SectorNum, SectorArea);

    if (SectorArea) {
        cfg.b.sector_begin0 = startSector;
        cfg.b.sector_num0   = SectorNum;
        sector_en           = 0x1;
    } else {
        cfg.b.sector_begin1 = startSector;
        cfg.b.sector_num1   = SectorNum;
        sector_en           = 0x2;
    }

    FMC->FMC_EEPROM_COPY_CMD_CFG = cfg.w;
    /* Start copy. */
    FMC->FMC_EEPROM_COPY_CMD_CTRL = sector_en;

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : FMC_DRV_EepromGetCopyDoneStatus
 * Description   : This function get status which copy from dflash to nvram.
 *       
 */
bool FMC_DRV_EepromGetCopyDoneStatus(bool SectorArea)
{
    fmc_eeprom_copy_cmd_status_type_t status;
    status.w = FMC->FMC_EEPROM_COPY_CMD_STATUS;
    trace("SectorArea = %d status.w = %x\r\n", SectorArea, status.w);

    if (SectorArea == 1) {
        trace("status.b.sector0_copy_done = %x\r\n", status.b.sector0_copy_done);
        return status.b.sector0_copy_done;
    } else {
        trace("status.b.sector1_copy_done = %x\r\n", status.b.sector1_copy_done);
        return status.b.sector1_copy_done;
    }
}

/** Don't use this interface in bootRom. */
#if FPGA_PLT
/**
 *
 * Function Name : FMC_DRV_EepromBackupFromSramToFlash
 * Description   : This function backup EEPROM data from nvram to dflash.
 *       
 */
static status_t FMC_DRV_EepromBackupFromSramToFlash(uint32_t sramAddr, uint32_t flashAddr, uint8_t size)
{
    data_block_t db;
    uint32_t i;

    db.eeAddr   = sramAddr & 0xfff;
    db.dbLength = size & 0xf;

    for (i = 0; i < size; i++) {
        db.data[i] = *(volatile uint8_t *)(ENVRAM_DCACHE_BASEADDR + sramAddr + i);
    }

    FMC_DRV_DflashMainSectorProgram(flashAddr, &db, 8);
}
#else
/**
 *
 * Function Name : FMC_DRV_EepromBackupFromSramToFlash
 * Description   : This function backup EEPROM data from nvram to dflash.
 *       
 */
static status_t FMC_DRV_EepromBackupFromSramToFlash(uint32_t sramAddr, uint32_t flashAddr, uint8_t size)
{
    fmc_eeprom_write_cmd_cfg_type_t writeCfg;
    // fmc_eeprom_sector_cfg_type_t sectorCfg;

    // sectorCfg.w = FMC->FMC_EEPROM_SECTOR_CFG;
    if (sramAddr + size > 0x1000) {
        return STATUS_FLASH_OUT_OF_RANGE;
    }

    writeCfg.b.flash_addr = (flashAddr & 0x1ffff) >> 3;
    writeCfg.b.sram_addr  = sramAddr;
    writeCfg.b.db_length  = size;

    FMC->FMC_EEPROM_WRITE_CMD_CFG = writeCfg.w;

    while (!(FMC->FMC_EEPROM_WRITE_CMD_STATUS & 0x1))
        ;

    return STATUS_SUCCESS;
}
#endif

/**
 *
 * Function Name : FMC_DRV_FixedAreaUpdate
 * Description   : This function backup one block EEPROM data from nvram to dflash.
 * |------------------One data block is 8 byte------------------------| 
 * | nvram addr(12 bit) | data length(4bit) | EEPROM data (max 6bytes)| 
 *  
 */
static status_t FMC_DRV_FixedAreaUpdate(void *data)
{
    data_block_status_t *status = &g_fixedAreaStatus;

    flash_area_t flashArea = MAIN_SECTOR;
    uint32_t eraseCnt;
    uint32_t currentSector;
    uint32_t nextSector;
    uint32_t dbOffset;
    uint32_t dflashAddr;
    uint32_t sectorHead[2];

    //status_t FMC_DRV_DflashSectorWrite(flash_area_t flashArea, uint32_t flashAddr, void *writeBuf, uint32_t size);

    currentSector = status->sectorIndex;

    if (currentSector < (g_fixedAreaEndSector)) {
        nextSector = status->sectorIndex + 1;
    } else {
        nextSector = g_fixedAreaStartSector;
    }

    eraseCnt = FMC_DRV_GetEraseCount(nextSector);
    /* If all sector is full , erase next sector.
    TODO: EEPROM maybe lost */
    if (eraseCnt != 0xffffffffUL) {
        dflashAddr = FLASH_SECTOR_TO_ADDRESS(nextSector);
        //FMC_DRV_DflashMainSectorErase(dflashAddr, 1);
        FMC_DRV_DflashEraseSector(flashArea, dflashAddr, 1);
    }

    dbOffset = status->dbOffset;

    if (dbOffset == DFLASH_SECTOR_SIZE) {
        dflashAddr    = FLASH_SECTOR_TO_ADDRESS(nextSector);
        sectorHead[0] = (++g_fixedAreaEraseCnt);
        sectorHead[1] = g_magicNumber;
        /* Write sector header (erase count + magic number). */
        FMC_DRV_DflashSectorWrite(flashArea, dflashAddr, &sectorHead[0], 8);
        status->sectorIndex = nextSector;
        status->dbOffset    = 8;
        trace("FMC_DRV_FixedAreaUpdate sector align header flashAddr 0x%x \r\n", dflashAddr);
    } else {
        dflashAddr = FLASH_SECTOR_TO_ADDRESS(currentSector);
        /* First use eeprom, write erase count to start sector. */
        if (currentSector == g_fixedAreaStartSector && dbOffset == 8 && g_eepromFixedHaveUsed == false) {
            trace("FMC_DRV_FixedAreaUpdate fixed header flashAddr 0x%x \r\n", dflashAddr);
            sectorHead[0] = (g_fixedAreaEraseCnt);
            sectorHead[1] = g_magicNumber;
            FMC_DRV_DflashSectorWrite(flashArea, dflashAddr, &sectorHead[0], 8);
            g_eepromFixedHaveUsed = true;
        }
    }

    dflashAddr += dbOffset;
    FMC_DRV_DflashSectorWrite(flashArea, dflashAddr, data, 8);
    status->dbOffset += 8;

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : FMC_DRV_ProcessFrequentAreaNextSector
 * Description   : This function processes the next sector in preparation 
 * for when the current sector is full.
 */
static status_t FMC_DRV_ProcessFrequentAreaNextSector(uint32_t nextSector)
{
    uint32_t dflashAddr;
    uint32_t dbOffset      = 8;
    flash_area_t flashArea = MAIN_SECTOR;
    uint32_t dbData[2];
    data_block_t *pDb;
    bool comRes = true;
    uint32_t i;

    dflashAddr = FLASH_SECTOR_TO_ADDRESS(nextSector);
    /* If no fixed record area , just erase the sector.
    TODO: this maybe lost data */
    if (g_fixedAreaEnable == false) {

        trace("erase fixed area ,flash address: 0x%x, \r\n", dflashAddr);
        //return FMC_DRV_DflashMainSectorErase(dflashAddr, 1);
        return FMC_DRV_DflashEraseSector(flashArea, dflashAddr, 1);
    } else {
        for (dbOffset = 8; dbOffset < DFLASH_SECTOR_SIZE; dbOffset += 8) {
            dflashAddr += 8;
            dbData[0] = DFLASH_READ(dflashAddr);
            dbData[1] = DFLASH_READ(dflashAddr + 4);
            pDb       = (data_block_t *)&dbData[0];

            for (i = 0; i < pDb->dbLength; i++) {
                if (pDb->data[i] != EEPROM_READ_1BYTE(pDb->eeAddr + i)) {
                    comRes = false;
                    break;
                }
            }
            if (comRes == true) {
                trace("update fixed area,nvram addr 0x%x,dflash addr 0x%x\r\n", pDb->eeAddr, dflashAddr);
                FMC_DRV_FixedAreaUpdate(&dbData[0]);
            } else {
                comRes = true;
            }
        }
        /* Erase next sector prepare for next write. */
        dflashAddr = FLASH_SECTOR_TO_ADDRESS(nextSector);
        trace("erase next sector,flash address: 0x%x, \r\n", dflashAddr);
        //  return FMC_DRV_DflashMainSectorErase(dflashAddr, 1);
        return FMC_DRV_DflashEraseSector(flashArea, dflashAddr, 1);
    }
}

/**
 *
 * Function Name : FMC_DRV_FrequentAreaUpdate
 * Description   : This function backup one block EEPROM data from nvram to dflash.
 * |------------------One data block is 8 byte------------------------| 
 * | nvram addr(12 bit) | data length(4bit) | EEPROM data (max 6bytes)| 
 *  
 */
static status_t FMC_DRV_FrequentAreaUpdate(uint32_t sramAddr, uint32_t dbSize)
{
    data_block_status_t *status = &g_frequentAreaStatus;
    uint32_t eraseCnt;
    uint32_t currentSector;
    uint32_t nextSector;
    uint32_t dbOffset;
    uint32_t dflashAddr;
    uint32_t sectorHead[2];
    status_t ret           = STATUS_SUCCESS;
    flash_area_t flashArea = MAIN_SECTOR;

    if (dbSize == 0) {
        return STATUS_SUCCESS;
    }

    currentSector = status->sectorIndex;

    if (currentSector < (g_frequentAreaEndSector)) {
        nextSector = status->sectorIndex + 1;
    } else {
        nextSector = g_frequentAreaStartSector;
    }

    eraseCnt = FMC_DRV_GetEraseCount(nextSector);

    /** If next sector is not empty, need process it*/
    if (eraseCnt != 0xffffffffUL) {
        FMC_DRV_ProcessFrequentAreaNextSector(nextSector);
    }

    dbOffset = status->dbOffset;
    /** if the current sector if full ,switch to next sector to write */
    if (dbOffset == DFLASH_SECTOR_SIZE) {
        dflashAddr    = FLASH_SECTOR_TO_ADDRESS(nextSector);
        sectorHead[0] = (++g_frequentAreaEraseCnt);
        sectorHead[1] = g_magicNumber;
        /* Write sector header (erase count + magic number). */
        ret = FMC_DRV_DflashSectorWrite(flashArea, dflashAddr, &sectorHead[0], 8);

        status->sectorIndex = nextSector;
        status->dbOffset    = 8;
        dbOffset            = 8;
    } else {
        dflashAddr = FLASH_SECTOR_TO_ADDRESS(currentSector);

        /* if first update eeprom data to dflash, write erase count to start sector. */
        if (currentSector == g_frequentAreaStartSector && dbOffset == 8 && g_eepromFrequentHaveUsed == false) {
            sectorHead[0]            = (g_frequentAreaEraseCnt);
            sectorHead[1]            = g_magicNumber;
            ret                      = FMC_DRV_DflashSectorWrite(flashArea, dflashAddr, &sectorHead[0], 8);
            g_eepromFrequentHaveUsed = true;
        }
    }

    if (ret != STATUS_SUCCESS) {
        return ret;
    }
    /** Adjust the flash address to the data write location */
    dflashAddr += dbOffset;
    ret = FMC_DRV_EepromBackupFromSramToFlash(sramAddr, dflashAddr, dbSize);

    status->dbOffset += 8;
    return ret;
}

/**
 *
 * Function Name : FMC_DRV_ProcessNbytes
 * Description   : This function processes the remaining data of less than 1 data block.
 *  
 */
static status_t FMC_DRV_ProcessNbytes(uint32_t eeAddr, uint8_t *pdata, uint8_t size)
{
    status_t ret;
    uint32_t dbSize;

    ret = FMC_DRV_EECompare(eeAddr, &pdata[0], size);
    if (STATUS_SUCCESS == ret) {
        return ret;
    }

    FMC_DRV_EEWrite(eeAddr, &pdata[0], size);
    dbSize = size;

    FMC_DRV_FrequentAreaUpdate(eeAddr, dbSize);

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : FMC_DRV_EepromWrite 
 * Description   : This function write EEPROM data to nvram .
 *  
 */
status_t FMC_DRV_EepromWrite(uint32_t eepromAddr, void *pdata, uint32_t size)
{
    uint32_t dbCnt  = 0;
    uint32_t nbytes = 0;
    uint32_t i, j;
    uint32_t addr    = eepromAddr & 0xfff;
    uint8_t *pU8Data = (uint8_t *)pdata;
    status_t ret     = STATUS_ERROR;

    if (g_eepEnable == false) {
        return STATUS_FLASH_ERROR_ENABLE;
    }

    if (addr + size > (g_eepSize)) {
        return STATUS_FLASH_OUT_OF_RANGE;
    }

    dbCnt  = size / DB_DATA_SIZE;
    nbytes = size % DB_DATA_SIZE;
    j      = 0;

    for (i = 0; i < dbCnt; i++) {
        ret = FMC_DRV_EECompare(addr, &pU8Data[j], 6);
        if (STATUS_SUCCESS == ret) {
            trace("eeprom addr 0x%0x data is not change\r\n", addr);
            addr += 6;
            j += 6;
            continue;
        }

        FMC_DRV_EEWrite(addr, &pU8Data[j], 6);
        FMC_DRV_FrequentAreaUpdate(addr, 6);
        addr += 6;
        j += 6;
    }
    if (nbytes > 0) {
        FMC_DRV_ProcessNbytes(addr, &pU8Data[j], nbytes);
    } else {
    }

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : FMC_DRV_EepromRead
 * Description   : This function read EEPROM data from nvram .
 *  
 */
status_t FMC_DRV_EepromRead(uint32_t eepromAddr, void *pdata, uint32_t size)
{
    uint32_t i;
    uint32_t addr   = eepromAddr & 0xfff;
    uint8_t *u8Data = (uint8_t *)pdata;

    if (g_eepEnable == false) {
        return STATUS_FLASH_ERROR_ENABLE;
    }

    if (addr + size > (g_eepSize)) {
        return STATUS_FLASH_OUT_OF_RANGE;
    }

    for (i = 0; i < size; i++) {
        u8Data[i] = EEPROM_READ_1BYTE(addr);
        addr += 1;
    }

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : FMC_DRV_EepromClear
 * Description   : This function clear all EEPROM data .
 *   
 * If the EEPROM or dflash bankup area is written full, the user 
 * may erase the EEPROM and backup area to achieve better EEPROM write speed.
 * 
 */
status_t FMC_DRV_EepromClear(void)
{
    uint32_t i;
    uint32_t cnt;
    uint32_t dflashAddr;
    flash_area_t flashArea = MAIN_SECTOR;

    if (g_eepEnable == false) {
        return STATUS_FLASH_ERROR_ENABLE;
    }

    /** clear EEPROM data on NVRAM */
    for (i = 0; i < g_eepSize; i++) {
        EEPROM_WRITE_1BYTE(i, 0);
    }

    /** erase backup arae in dflash*/
    cnt = g_frequentAreaEndSector - g_frequentAreaStartSector + 1;

    dflashAddr = FLASH_SECTOR_TO_ADDRESS(g_frequentAreaStartSector);
    if (STATUS_SUCCESS != FMC_DRV_DflashEraseSector(flashArea, dflashAddr, cnt)) {
        return STATUS_FLASH_ERASE_FAIL;
    }

    cnt        = g_fixedAreaEndSector - g_fixedAreaStartSector + 1;
    dflashAddr = FLASH_SECTOR_TO_ADDRESS(g_fixedAreaStartSector);
    if (FMC_DRV_DflashEraseSector(flashArea, dflashAddr, cnt)) {
        return STATUS_FLASH_ERASE_FAIL;
    }

    /** reset frequent area status to inital state*/
    g_frequentAreaStatus.sectorIndex = g_frequentAreaStartSector;
    g_frequentAreaEraseCnt           = 0;
    g_eepromFrequentHaveUsed         = false;

    /** reset fix area status to inital state*/
    g_fixedAreaStatus.sectorIndex = g_fixedAreaStartSector;
    g_fixedAreaEraseCnt           = 0;
    g_eepromFixedHaveUsed         = false;

    return STATUS_SUCCESS;
}