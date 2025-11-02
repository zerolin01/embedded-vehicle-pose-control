
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
 * @file fmc_flash_driver.c
 * @brief FMC flash driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "fmc_hw_access.h"
#include "fmc_flash_driver.h"
#include "fmc_eeprom_driver.h"
#include "clock_driver.h"

/**************************************flash layout***************************************************
 * ------------------------------pflash layout-------------------------------------------------
 * The pflash from the user's perspective consists of main and nvr.
 *
 *           MAIN SIZE     NVR SIZE      SECTOR SIZE   REDUNDANT SECTORS
 *
 * EAM2011    1Mbytes      32Kbytes        2Kbytes         6(pFLash p0, p1 have 2 redundant sectors, dFlash has 2 redundant sectors)
 *
 * Damaged sectors are automatically replaced with redundant
 * sectors. And the redundant sectors are not visible to the user.
 *
 * EAM2011
 *   _____P0______
 *   | NVR(16K)  |
 *   |———————————|  \
 *   | RDN(2k)   |    \
 *   |___________|      \    User perspective Pflash
 *   | MAIN(512K)| \      \ _____________
 *   |___________|   \   /  | NVR(32k)  |
 *                     /    |———————————|
 *   ______P1_____   /  \   |           |
 *   | NVR(16K)  | /      \ |  MAIN(1M) |
 *   |———————————|        / |           |
 *   | RDN(2k)   |      /   |___________|
 *   |___________|    /
 *   | MAIN(512K)|  /
 *   |___________|
 *
 *
 *---------------------------dflash layout----------------------------------------------------
 * The dflash contains the main nvr rdn 3 parts, where the size of main is 128Kbytes, the size of
 * nvr is 16Kbyes, and the size of each sector is 1Kbytes.
 * The dflash has two redundant sectors. Damaged sectors are automatically replaced with redundant
 * sectors. And the redundant sectors are not visible to the user.
 *                           _____________
 *                           | NVR(16K)  |
 *                           |———————————|
 *                           | RDN(2k)   |
 *                           |___________|
 *                           | MAIN(128K)|
 *                           |___________|
 *
 *************************************************************************************************
 */

/*
 * @brief FMC warning interrupt event
 * register:
 * FMC_WARNING_INT_MSK
 */
typedef enum {
    FMC_EVENT_PFLASH_ONE_BIT_ECC       = 0x1,  /* Enable or disable pFlash 1-bit ECC error interrupt */
    FMC_EVENT_PFLASH_MULTI_ONT_BIT_ECC = 0x2,  /* Enable or disable pFlash multiple 1-bit error interrupt */
    FMC_EVENT_DFLASH_ONE_BIT_ECC       = 0x4,  /* Enable or disable dFlash 1-bit ECC error interrupt */
    FMC_EVENT_DFLASH_MULTI_ONE_BIT_ECC = 0x8,  /* Enable or disable dFlash multiple 1-bit ECC error interrupt */
    FMC_EVENT_SRAM_ONE_BIT_ECC         = 0x40, /* Enable or disable the interrupt triggered by sRam 1-bit ECC */
} fmc_warning_event_t;

/*
 * @brief FMC error interrupt callback
 */
typedef struct {
    fmc_errCallback_t errCallBack;
    void *errParameter;
} fmc_irq_t;

/** Global variables. */
static const unsigned int s_protectArea[32] = {
    0x1,
    0x3,
    0x7,
    0xf,
    0x1f,
    0x3f,
    0x7f,
    0xff,
    0x1ff,
    0x3ff,
    0x7ff,
    0xfff,
    0x1fff,
    0x3fff,
    0x7fff,
    0xffff,
    0x1ffff,
    0x3ffff,
    0x7ffff,
    0xfffff,
    0x1fffff,
    0x3fffff,
    0x7fffff,
    0xffffff,
    0x1ffffff,
    0x3ffffff,
    0x7ffffff,
    0xfffffff,
    0x1fffffff,
    0x3ffffff,
    0x7ffffff,
    0xffffffff,
};

/**
 *
 * Function Name : FMC_ChangeFre
 * Description   :  FMC change frequency.
 *
 */
__TIM_CODE void FMC_ChangeFre(uint32_t frequency, uint32_t clockMode, uint32_t mode)
{
    fmc_ctrl_cfg_type_t cfg;

    /*If frequency less than or equal to 80000000,do nothing */
    if ((frequency < 80000000) || (frequency == 80000000)) {
        return;
    }

    /* Modify frequency */
    switch (mode) {
    case 1: //upturn
        cfg.w = FMC->FMC_CTRL_CFG;
        cfg.b.fmc_clock_mode = 0;
        FMC->FMC_CTRL_CFG = cfg.w;
        CLOCK_SYS_SetClkDivider(MAIN_BUS_CLK, DIVIDE_BY_1);
        CLOCK_SYS_SetClkDivider(MAIN_BUS_MOTOR_CTRL_CLK, DIVIDE_BY_2);
        break;
    case 0: //underclocking
        CLOCK_SYS_SetClkDivider(MAIN_BUS_CLK, DIVIDE_BY_2);
        CLOCK_SYS_SetClkDivider(MAIN_BUS_MOTOR_CTRL_CLK, DIVIDE_BY_1);
        break;

    default:
        break;
    }
}

__TIM_CODE status_t FMC_DRV_SetClockMode(fmc_clocksource_t clockSource)
{
    fmc_ctrl_cfg_type_t cfg;
    cfg.w                = FMC->FMC_CTRL_CFG;
    cfg.b.fmc_clock_mode = clockSource;
    FMC->FMC_CTRL_CFG    = cfg.w;

    return STATUS_SUCCESS;
}

__TIM_CODE status_t FMC_DRV_UpdateClockMode(void)
{
    static struct {
        uint8_t val;
        uint32_t freq;
    } FMC_Freq[4] = {
        {FMC_CLOCKSOURCE_160M, 160000000},
        {FMC_CLOCKSOURCE_80M, 80000000},
        {FMC_CLOCKSOURCE_48M, 48000000},
        {FMC_CLOCKSOURCE_40M, 40000000},
    };
    uint32_t frequency, clockMode = FMC_CLOCKSOURCE_160M;
    clock_names_t fmcClocks[FMC_INSTANCE_COUNT] = FEATURE_FMC_CLOCK_NAMES;
    status_t status                             = CLOCK_SYS_GetFreq(fmcClocks[0], &frequency);
    if (status == STATUS_SUCCESS) {
        for (uint32_t i = 1; i < 4; i++) {
            if (frequency > FMC_Freq[i].freq) {
                clockMode = FMC_Freq[i - 1].val;
                break;
            } else {
                clockMode = FMC_Freq[i].val;
            }
        }
        fmc_ctrl_cfg_type_t cfg;
        cfg.w = FMC->FMC_CTRL_CFG;

        cfg.b.fmc_clock_mode = clockMode; // workaround reduce frequency (clockMode)

        FMC->FMC_CTRL_CFG = cfg.w;
    }

    return status;
}

/********************************************************************
 *  PFLASH  interface.
 * ******************************************************************/

/**
 *
 * Function Name : FMC_DRV_PflashEraseSector
 * Description   : This function erase pflash sector.
 *
 * STATUS_SUCCESS: erase success
 */
status_t
    FMC_DRV_PflashEraseSector(flash_area_t flashArea, uint32_t flashAddr, uint32_t eraseSectorNum)
{
    uint32_t cnt = 0;
    uint32_t addr;
    uint32_t flash_size   = 0;
    uint32_t sector_num   = 0;
    uint32_t fmcFrequency = 0;
    fmc_ctrl_cfg_type_t cfg;

    /*Check FMC clock, if the clock is 160M, changes it to 80M */
    cfg.w = FMC->FMC_CTRL_CFG;
    CLOCK_SYS_GetFreq(FMC_CLK, &fmcFrequency);

    switch (flashArea) {
    case MAIN_SECTOR:
        addr       = flashAddr & PFLASH_MAIN_SECTOR_MASK;
        flash_size = PFLASH_MAIN_SECTOR_SIZE;
        sector_num = PFLASH_MAIN_SECTOR_NUM;
        break;

    case NVR_SECTOR:
        addr       = flashAddr & PFLASH_NVR_SECTOR_MASK;
        flash_size = PFLASH_NVR_SECTOR_SIZE;
        sector_num = PFLASH_NVR_SECTOR_NUM;
        break;

    default:
        flash_size = 0;
        break;
    }

    if (addr >= (flash_size - 1) || (addr % PFLASH_SECTOR_UNIT_SIZE != 0)) {
        return STATUS_FLASH_OUT_OF_RANGE;
    }
    FMC_ChangeFre(fmcFrequency, cfg.b.fmc_clock_mode, 0);

    eraseSectorNum = eraseSectorNum > sector_num ? sector_num : eraseSectorNum;

    while (cnt < (eraseSectorNum)) {
        if (addr < flash_size) {
            if (FMC_PflashEraseCmd(SECTOR_ERASE, flashArea, addr) != STATUS_SUCCESS) {
                FMC_ChangeFre(fmcFrequency, cfg.b.fmc_clock_mode, 1);
                return STATUS_FLASH_ERASE_FAIL;
            }
            addr += PFLASH_SECTOR_UNIT_SIZE;
        }
        cnt++;
    }

    /* Recovery clock rate */
    FMC_ChangeFre(fmcFrequency, cfg.b.fmc_clock_mode, 1);

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : FMC_DRV_PflashEraseEntireSector
 * Description   : This function erase entire area in pflash.
 *
 * The size of pflash main area is 1Mbytes .
 *
 * STATUS_SUCCESS: erase success
 *
 */
status_t FMC_DRV_PflashEraseEntireSector(flash_erase_area_t flashArea)
{
    uint32_t fmcFrequency = 0;
    status_t reStatus     = STATUS_SUCCESS;

    fmc_ctrl_cfg_type_t cfg;

    /*Check FMC clock, if the clock is 160M, changes it to 80M */
    cfg.w = FMC->FMC_CTRL_CFG;
    CLOCK_SYS_GetFreq(FMC_CLK, &fmcFrequency);
    FMC_ChangeFre(fmcFrequency, cfg.b.fmc_clock_mode, 0);

    reStatus = FMC_PflashEraseCmd(flashArea, MAIN_SECTOR, 0);

    /* Recovery clock rate */
    FMC_ChangeFre(fmcFrequency, cfg.b.fmc_clock_mode, 1);

    return reStatus;
}

/**
 *
 * Function Name : FMC_DRV_PflashSectorRead
 * Description   : This function read specified bytes from pflash sector.
 *
 * pflash main sector address range is 0-0x1ffff
 * pflash nvr address range is 0-0x3fff
 *
 * STATUS_SUCCESS: read success
 *
 */
__TIM_CODE status_t FMC_DRV_PflashSectorRead(flash_area_t flashArea, uint32_t flashAddr, void *recvBuf, uint32_t size)
{
    return FMC_PflashSectorRead(NORMAL_READ, flashArea, flashAddr, recvBuf, size);
}

/**
 *
 * Function Name : FMC_DRV_PflashSectorWrite
 * Description   : This function write pflash .
 *
 * pflash main sector total size is 1MBytes and one sector is 2K bytes.
 * pflash main sector address range is 0-0xfffff
 * pflash nvr sector total size is 32KBytes and one sector is 2K bytes.
 * pflash nvr address range is 0-0x7fff
 *
 * STATUS_SUCCESS: program success
 */
__TIM_CODE status_t FMC_DRV_PflashSectorWrite(flash_area_t flashArea, uint32_t flashAddr, void *writeBuf, uint32_t size)
{
    uint32_t nbytes = 0;
    uint32_t addrAlign8;
    uint32_t offset;
    uint32_t length;
    uint8_t u8Data[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    uint32_t i;
    uint32_t addr;
    uint8_t *u8Buf        = (uint8_t *)writeBuf;
    uint32_t flash_size   = 0;
    uint32_t fmcFrequency = 0;
    fmc_ctrl_cfg_type_t cfg;

    /*Check FMC clock, if the clock is 160M, changes it to 80M */

    cfg.w = FMC->FMC_CTRL_CFG;
    CLOCK_SYS_GetFreq(FMC_CLK, &fmcFrequency);

    switch (flashArea) {
    case MAIN_SECTOR:
        addr       = flashAddr & PFLASH_MAIN_SECTOR_MASK;
        flash_size = PFLASH_MAIN_SECTOR_SIZE;
        break;

    case NVR_SECTOR:
        addr       = flashAddr & PFLASH_NVR_SECTOR_MASK;
        flash_size = PFLASH_NVR_SECTOR_SIZE;
        break;

    default:
        flash_size = 0;
        break;
    }

    if ((addr + size) > flash_size) {
        return STATUS_FLASH_OUT_OF_RANGE;
    }

    FMC_ChangeFre(fmcFrequency, cfg.b.fmc_clock_mode, 0);

    /** Align address to 8 byte */
    addrAlign8 = (addr / 8) * 8;

    /** offset from base addrAlign8 to addr*/
    offset = addr % 8;
    /** length to be read in one block(8byte) */
    length = 8 - offset;
    /** read data len  remainder 8 */
    if (size > length) {
        nbytes = (size - length) % 8;
    } else {
        nbytes = size;
    }

    for (i = 0; i < size;) {
        if (offset != 0 && (i == 0)) {
            if (length >= size) {
                memcopy(&u8Data[offset], &u8Buf[i], size);
            } else {
                memcopy(&u8Data[offset], &u8Buf[i], length);
            }
        } else {
            length = (i + nbytes) > (size - 1) ? nbytes : 8;
            memcopy(&u8Data[0], &u8Buf[i], length);
        }

        if (FMC_PflashProgramCmd(flashArea, addrAlign8, *(uint32_t *)(&u8Data[0]), *(uint32_t *)(&u8Data[4])) != STATUS_SUCCESS) {
            FMC_ChangeFre(fmcFrequency, cfg.b.fmc_clock_mode, 1);
            return STATUS_FLASH_PROFRAM_FAIL;
        }
        i                         = i + length;
        addrAlign8                = addrAlign8 + 8;
        *(uint32_t *)(&u8Data[0]) = 0xffffffff;
        *(uint32_t *)(&u8Data[4]) = 0xffffffff;
    }

    /* Recovery clock rate */
    FMC_ChangeFre(fmcFrequency, cfg.b.fmc_clock_mode, 1);

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : FMC_DRV_PflashSectorProtect
 * Description   : This function enables write protection for the specified
 * area of the pflash main sector.
 *
 * The pflash main sector is divided into 32 protected areas, each with a size of 32KBytes.
 * Protect area range is 0-31.
 *
 */
status_t FMC_DRV_PflashSectorProtect(uint32_t startProtectArea, uint32_t endProtectArea)
{
    fmc_ctrl_cfg_type_t cfg;
    uint32_t wp;
    uint32_t index;

    if (startProtectArea > 31 || endProtectArea > 31 || startProtectArea > endProtectArea) {
        return STATUS_FLASH_OUT_OF_RANGE;
    }

    cfg.w                      = FMC->FMC_CTRL_CFG;
    cfg.b.pflash_wr_protect_en = 1;
    FMC->FMC_CTRL_CFG          = cfg.w;

    wp                                = FMC->FMC_PFLASH_WR_PROTECT_BITMAP;
    index                             = (endProtectArea - startProtectArea);
    wp                                = wp | (s_protectArea[index] << startProtectArea);
    FMC->FMC_PFLASH_WR_PROTECT_BITMAP = wp;

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : FMC_DRV_PflashSectorUnprotected
 * Description   : This function disable write protection for the specified
 * area of the pflash main sector.
 *
 * The pflash main sector is divided into 32 protected areas, each with a size of 32KBytes.
 * Protect area range is 0-31.
 *
 */
status_t FMC_DRV_PflashSectorUnprotected(uint32_t startProtectArea, uint32_t endProtectArea)
{
    fmc_ctrl_cfg_type_t cfg;
    uint32_t wp;
    uint32_t index;

    if (startProtectArea > 31 || endProtectArea > 31 || startProtectArea > endProtectArea) {
        return STATUS_FLASH_OUT_OF_RANGE;
    }

    cfg.w                      = FMC->FMC_CTRL_CFG;
    cfg.b.pflash_wr_protect_en = 1;
    FMC->FMC_CTRL_CFG          = cfg.w;

    wp                                = FMC->FMC_PFLASH_WR_PROTECT_BITMAP;
    index                             = (endProtectArea - startProtectArea);
    wp                                = wp & (~(s_protectArea[index] << startProtectArea));
    FMC->FMC_PFLASH_WR_PROTECT_BITMAP = wp;

    return STATUS_SUCCESS;
}

/**
*********************************************************************************************
 *  DFLASH  interface.
 *
 *********************************************************************************************
*/

/**
 *
 * Function Name : FMC_DRV_DflashSectorProtect
 * Description   : This function enable write protection for the specified
 * area of the dflash main sector.
 *
 * The dflash main sector is divided into 8 protected areas, each with a size of 16KBytes.
 * Protect area range is 0-7.
 *
 */
status_t FMC_DRV_DflashSectorProtect(uint32_t startProtectArea, uint32_t endProtectArea)
{
    fmc_ctrl_cfg_type_t cfg;
    uint32_t wp;
    uint32_t index;

    if (startProtectArea > 7 || endProtectArea > 7 || startProtectArea > endProtectArea) {
        return STATUS_FLASH_OUT_OF_RANGE;
    }

    cfg.w                      = FMC->FMC_CTRL_CFG;
    cfg.b.dflash_wr_protect_en = 1;
    FMC->FMC_CTRL_CFG          = cfg.w;

    wp    = FMC->FMC_DFLASH_WR_PROTECT_BITMAP;
    index = (endProtectArea - startProtectArea);

    wp = wp | (s_protectArea[index] << startProtectArea);

    FMC->FMC_DFLASH_WR_PROTECT_BITMAP = wp;

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : FMC_DRV_DflashSectorUnprotected
 * Description   : This function disable write protection for the specified
 * area of the dflash main sector.
 *
 * The dflash main sector is divided into 8 protected areas, each with a size of 16KBytes.
 * Protect area range is 0-7.
 *
 */
status_t FMC_DRV_DflashSectorUnprotected(uint32_t startProtectArea, uint32_t endProtectArea)
{
    fmc_ctrl_cfg_type_t cfg;
    uint32_t wp;
    uint32_t index;

    if (startProtectArea > 7 || endProtectArea > 7 || startProtectArea > endProtectArea) {
        return STATUS_FLASH_OUT_OF_RANGE;
    }

    cfg.w                      = FMC->FMC_CTRL_CFG;
    cfg.b.dflash_wr_protect_en = 1;
    FMC->FMC_CTRL_CFG          = cfg.w;

    wp    = FMC->FMC_DFLASH_WR_PROTECT_BITMAP;
    index = (endProtectArea - startProtectArea);

    wp = wp & (~(s_protectArea[index] << startProtectArea));

    FMC->FMC_DFLASH_WR_PROTECT_BITMAP = wp;

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : FMC_DRV_DflashEraseSector
 * Description   : This function erase dflash sector .
 *
 * STATUS_FLASH_OUT_OF_RANGE: dflash address range error
 * STATUS_FLASH_ERASE_FAIL: dflash erase error
 * STATUS_SUCCESS: erase success
 *
 */
status_t FMC_DRV_DflashEraseSector(flash_area_t flashArea, uint32_t flashAddr, uint32_t eraseSectorNum)
{
    uint32_t cnt = 0;
    uint32_t addr;
    uint32_t flash_size   = 0;
    uint32_t sector_num   = 0;
    uint32_t fmcFrequency = 0;
    fmc_ctrl_cfg_type_t cfg;

    /*Check FMC clock, if the clock is 160M, changes it to 80M */
    cfg.w = FMC->FMC_CTRL_CFG;
    CLOCK_SYS_GetFreq(FMC_CLK, &fmcFrequency);

    switch (flashArea) {
    case MAIN_SECTOR:
        addr       = flashAddr & DFLASH_MAIN_SECTOR_MASK;
        flash_size = DFLASH_MAIN_SECTOR_SIZE;
        sector_num = DFLASH_MAIN_SECTOR_NUM;
        break;
    case NVR_SECTOR:
        addr       = flashAddr & DFLASH_NVR_SECTOR_MASK;
        flash_size = DFLASH_NVR_SECTOR_SIZE;
        sector_num = DFLASH_NVR_SECTOR_NUM;
        break;

    default:
        flash_size = 0;
        break;
    }

    if (addr >= (flash_size - 1) || (addr % 1024 != 0)) {
        return STATUS_FLASH_OUT_OF_RANGE;
    }

    FMC_ChangeFre(fmcFrequency, cfg.b.fmc_clock_mode, 0);

    eraseSectorNum = eraseSectorNum > sector_num ? sector_num : eraseSectorNum;

    while (cnt < (eraseSectorNum)) {
        if (addr < flash_size) {
            if (FMC_DflashEraseCmd(SECTOR_ERASE, flashArea, addr) != STATUS_SUCCESS) {
                FMC_ChangeFre(fmcFrequency, cfg.b.fmc_clock_mode, 1);
                return STATUS_FLASH_ERASE_FAIL;
            }
            addr += 1024;
        }
        cnt++;
    }

    /* Recovery clock rate */
    FMC_ChangeFre(fmcFrequency, cfg.b.fmc_clock_mode, 1);

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : FMC_DRV_DflashEraseEntireSector
 * Description   : This function erases entire dflash include main, rdn and nvr sector.
 *
 * dflash consists of main, nvr and rdn. rdn is invisible to users
 * main sector total size is 1M bytes.
 * nvr sector total size is 16K bytes.
 * rnd sector total size is 2K bytes.
 * dflash has two(2k bytes) redundant sectors. Damaged sectors are automatically
 * replaced with redundant sectors.
 *
 * STATUS_SUCCESS if erase successful.
 *
 */
status_t FMC_DRV_DflashEraseEntireSector(flash_erase_area_t flashArea)
{

    status_t reStatus     = STATUS_SUCCESS;
    uint32_t fmcFrequency = 0;
    fmc_ctrl_cfg_type_t cfg;

    /*Check FMC clock, if the clock is 160M, changes it to 80M */
    cfg.w = FMC->FMC_CTRL_CFG;

    CLOCK_SYS_GetFreq(FMC_CLK, &fmcFrequency);

    FMC_ChangeFre(fmcFrequency, cfg.b.fmc_clock_mode, 0);

    reStatus = FMC_DflashEraseCmd(flashArea, MAIN_SECTOR, (uint32_t)0);

    /* Recovery clock rate */
    FMC_ChangeFre(fmcFrequency, cfg.b.fmc_clock_mode, 1);

    return reStatus;
}

/**
 *
 * Function Name : FMC_DRV_DflashSectorRead
 * Description   : This function read specified bytes from dflash  sector.
 *
 * dflash main sector address range is 0-0x1ffff
 * dflash nvr address range is 0-0x3fff
 *
 * STATUS_SUCCESS: read success
 *
 */
status_t FMC_DRV_DflashSectorRead(flash_area_t flashArea, uint32_t flashAddr, void *recvBuf, uint32_t size)
{
    return FMC_DflashSectorRead(NORMAL_READ, flashArea, flashAddr, recvBuf, size);
}

/**
 *
 * Function Name : FMC_DRV_DflashSectorWrite
 * Description   : This function program specified bytes to dflash.
 *
 * STATUS_SUCCESS: program success
 *
 */
status_t FMC_DRV_DflashSectorWrite(flash_area_t flashArea, uint32_t flashAddr, void *writeBuf, uint32_t size)
{
    uint32_t nbytes = 0;
    uint32_t addrAlign8;
    uint32_t offset;
    uint32_t length;
    uint32_t i;
    uint32_t addr;
    uint8_t u8Data[8]     = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    uint8_t *u8Buf        = (uint8_t *)writeBuf;
    uint32_t flash_size   = 0;
    uint32_t fmcFrequency = 0;
    fmc_ctrl_cfg_type_t cfg;

    /*Check FMC clock, if the clock is 160M, changes it to 80M */
    cfg.w = FMC->FMC_CTRL_CFG;
    CLOCK_SYS_GetFreq(FMC_CLK, &fmcFrequency);

    switch (flashArea) {
    case MAIN_SECTOR:
        addr       = flashAddr & DFLASH_MAIN_SECTOR_MASK;
        flash_size = DFLASH_MAIN_SECTOR_SIZE;
        break;
    case NVR_SECTOR:
        addr       = flashAddr & DFLASH_NVR_SECTOR_MASK;
        flash_size = DFLASH_NVR_SECTOR_SIZE;
        break;

    default:
        flash_size = 0;
        break;
    }

    if ((addr + size) > flash_size) {
        return STATUS_FLASH_OUT_OF_RANGE;
    }
    FMC_ChangeFre(fmcFrequency, cfg.b.fmc_clock_mode, 0);

    /** Align address to 8 byte */
    addrAlign8 = (addr / 8) * 8;

    /** offset from base addrAlign8 to addr*/
    offset = addr % 8;
    /** length to be read in one block(8byte) */
    length = 8 - offset;
    /** read data len  remainder 8 */
    if (size > length) {
        nbytes = (size - length) % 8;
    } else {
        nbytes = size;
    }
    for (i = 0; i < size;) {

        if (offset != 0 && (i == 0)) {
            if (length >= size) {
                memcopy(&u8Data[offset], &u8Buf[i], size);

            } else {
                memcopy(&u8Data[offset], &u8Buf[i], length);
            }
        } else {
            length = (i + nbytes) > (size - 1) ? nbytes : 8;
            memcopy(&u8Data[0], &u8Buf[i], length);
        }

        if (FMC_DflashProgramCmd(flashArea, addrAlign8, *(uint32_t *)(&u8Data[0]), *(uint32_t *)(&u8Data[4])) != STATUS_SUCCESS) {
            FMC_ChangeFre(fmcFrequency, cfg.b.fmc_clock_mode, 1);
            return STATUS_FLASH_PROFRAM_FAIL;
        }
        i = i + length;

        addrAlign8                = addrAlign8 + 8;
        *(uint32_t *)(&u8Data[0]) = 0xffffffff;
        *(uint32_t *)(&u8Data[4]) = 0xffffffff;
    }

    /* Recovery clock rate */
    FMC_ChangeFre(fmcFrequency, cfg.b.fmc_clock_mode, 1);

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : FMC_DRV_FMCEcc
 * Description   : Enable/Disable ECC.
 *
 */
void FMC_DRV_FMCEcc(flash_type_t fmcType, bool EccEnable)
{
    fmc_ctrl_cfg_type_t cfg;

    cfg.w = FMC->FMC_CTRL_CFG;

    switch (fmcType) {
    case PFLASH_TYPE:
        if (EccEnable) {
            cfg.b.pflash_ecc_en = 1;
        } else {
            cfg.b.pflash_ecc_en = 0;
        }
        break;

    case DFLASH_TYPE:
        if (EccEnable) {
            cfg.b.dflash_ecc_en = 1;
        } else {
            cfg.b.dflash_ecc_en = 0;
        }
        break;

    case ENVRAM_TYPE:
        if (EccEnable) {
            cfg.b.sram_ecc_en = 1;
        } else {
            cfg.b.sram_ecc_en = 0;
        }
        break;

    default:
        break;
    }

    FMC->FMC_CTRL_CFG = cfg.w;
}
