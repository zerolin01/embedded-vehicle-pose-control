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
 * @file fmc_hw_access.c
 * @brief FMC driver hardware access source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-12
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "fmc_hw_access.h"

/**
 *
 * Function Name : FMC_PflashProgramCmd
 * Description   : This function executes pflash programming commands.
 *
 * The pflash contains NVR(32Kbyte) and main sector(1Mbyte).
 *
 */
__TIM_CODE status_t FMC_PflashProgramCmd(flash_area_t flashArea, uint32_t flashAddr, uint32_t data0, uint32_t data1)
{
    uint32_t loopCount = 0;
    status_t osifError = STATUS_SUCCESS;
    fmc_pflash_program_cmd_status_type_t status;
    fmc_pflash_program_cmd_cfg_type_t cmdCfg;

#ifdef PLATFORM_EAM2011
    /*If it is the high 8 sector of the NVR, address conversion is required*/
    if ((flashAddr > 0x3FFF) && (flashArea == NVR_SECTOR)) {
        flashAddr |= 0x80000;
    } else {
        // flashAddr = flashAddr;
    }
#endif
    FMC->FMC_PFLASH_PROGRAM_CMD_DATA0 = data0;
    FMC->FMC_PFLASH_PROGRAM_CMD_DATA1 = data1;

    cmdCfg.w            = 0;
    cmdCfg.b.flash_addr = flashAddr;
    cmdCfg.b.sector_sel = flashArea;

    FMC->FMC_PFLASH_PROGRAM_CMD_CFG = cmdCfg.w;

    do {
        status.w = FMC->FMC_PFLASH_PROGRAM_CMD_STATUS;
        OS_DelayUs(FMC_OPERATION_DELAY);
    } while ((!status.b.cmd_stat) && (loopCount++ < FMC_ERR_LOOP));

    /*checkout status register*/
    if ((status.b.err_stat == 1) || (loopCount > FMC_ERR_LOOP)) {
        osifError = STATUS_FLASH_PROFRAM_FAIL;
    } else {
        osifError = STATUS_SUCCESS;
    }

    return osifError;
}

/**
 *
 * Function Name : FMC_PflashReadCmd
 * Description   : This function executes pflash read commands.
 *
 */
__TIM_CODE status_t FMC_PflashReadCmd(flash_read_mode_t readMode, flash_area_t flashArea, uint32_t flashAddr,
                           uint32_t *data0, uint32_t *data1)
{
    status_t osifError = STATUS_SUCCESS;
    fmc_pflash_read_cmd_status_type_t status;
    fmc_pflash_read_cmd_cfg_type_t cmdCfg;

#ifdef PLATFORM_EAM2011
    /*If it is the high 8 sector of the NVR, address conversion is required*/
    if ((flashAddr > 0x3FFF) && (flashArea == NVR_SECTOR)) {
        flashAddr |= 0x80000;
    } else {
        // flashAddr = flashAddr;
    }
#endif
    cmdCfg.w            = 0;
    cmdCfg.b.flash_addr = flashAddr;
    cmdCfg.b.sector_sel = flashArea;
    cmdCfg.b.read_mode  = readMode;

    FMC->FMC_PFLASH_READ_CMD_CFG = cmdCfg.w;

    /* Read until the operation is complete */
    do {
        status.w = FMC->FMC_PFLASH_READ_CMD_STATUS;
    } while (!status.b.cmd_stat);

    *data0 = FMC->FMC_PFLASH_READ_CMD_DATA0;
    *data1 = FMC->FMC_PFLASH_READ_CMD_DATA1;

    return osifError;
}

/**
 *
 * Function Name : FMC_PflashSectorVerifyRead
 * Description   : This function read pflash using verify read mode.
 *
 * verify read is used to check the erase result by applying "and" operator
 * to the specified sector data. If the read data0 is not 0xffffffff or data1 is
 * not 0xffffffff, means the erase failed.
 *
 */
status_t FMC_PflashSectorVerifyRead(flash_area_t flashArea, uint32_t flashAddr)
{
    uint32_t data0, data1;

    FMC_PflashReadCmd(VERIFY_READ, flashArea, flashAddr, &data0, &data1);

    if (data0 != 0xFFFFFFFF || data1 != 0xFFFFFFFF) {
        return STATUS_FLASH_READ_FAIL;
    }

    FMC_PflashReadCmd(VERIFY_READ, flashArea, (flashAddr + 8), &data0, &data1);

    if (data0 != 0xFFFFFFFF || data1 != 0xFFFFFFFF) {
        return STATUS_FLASH_READ_FAIL;
    }

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : FMC_PflashEraseRetryCmd
 * Description   : This function executes pflash erase cmd using retry mode.
 *
 * If nomal erase fail, will use retry mode to erase again.
 *
 */
void FMC_PflashEraseRetryCmd(flash_erase_area_t eraseMode, uint8_t retry, flash_area_t flashArea,
                             uint32_t flashAddr)
{
    uint32_t loopCount = 0;
    fmc_pflash_erase_cmd_status_type_t status;
    fmc_pflash_erase_cmd_cfg_type_t cmdCfg;

    cmdCfg.w            = 0;
    cmdCfg.b.flash_addr = flashAddr;
    cmdCfg.b.sector_sel = flashArea;
    cmdCfg.b.erase_mode = eraseMode;
    cmdCfg.b.retry      = retry & 0x3;

    FMC->FMC_PFLASH_ERASE_CMD_CFG = cmdCfg.w;

    do {
        status.w = FMC->FMC_PFLASH_ERASE_CMD_STATUS;
        OS_DelayUs(FMC_OPERATION_DELAY);
    } while ((!status.b.cmd_stat) && (loopCount++ < FMC_ERR_LOOP));
}

/**
 *
 * Function Name : FMC_PflashEraseCmd
 * Description   : This function executes pflash erase cmd .
 *
 */
status_t FMC_PflashEraseCmd(flash_erase_area_t eraseMode, flash_area_t flashArea, uint32_t flashAddr)
{
    fmc_pflash_erase_cmd_status_type_t status;
    fmc_pflash_erase_cmd_cfg_type_t cmdCfg;
    static uint8_t pRetry = 1;
    uint32_t loopCount    = 0;

#ifdef PLATFORM_EAM2011
    /*If it is the high 8 sector of the NVR, address conversion is required*/
    if ((flashAddr > 0x3FFF) && (flashArea == NVR_SECTOR)) {
        flashAddr |= 0x80000;
    } else {
        // flashAddr = flashAddr;
    }
#endif

    cmdCfg.w            = 0;
    cmdCfg.b.flash_addr = flashAddr;
    cmdCfg.b.sector_sel = flashArea;
    cmdCfg.b.erase_mode = eraseMode;
    cmdCfg.b.retry      = 0x0;

    FMC->FMC_PFLASH_ERASE_CMD_CFG = cmdCfg.w;

    /* Wait until pflash erase finished */
    do {
        status.w = FMC->FMC_PFLASH_ERASE_CMD_STATUS;
        OS_DelayUs(FMC_OPERATION_DELAY);
    } while ((!status.b.cmd_stat) && (loopCount++ < FMC_ERR_LOOP));

    /* check for a normal exit */
    if ((status.b.err_stat == 1) || (loopCount > FMC_ERR_LOOP)) {
        return STATUS_ERROR;
    } else {
    }

    /* Verify whether the erase was successful */
    while (FMC_PflashSectorVerifyRead(flashArea, flashAddr) != STATUS_SUCCESS && pRetry < 4) {
        FMC_PflashEraseRetryCmd(eraseMode, pRetry++, flashArea, flashAddr);
    }

    if (pRetry >= 4) {
        pRetry = 1;
        return STATUS_FLASH_ERASE_FAIL;
    } else {
        pRetry = 1;
        return STATUS_SUCCESS;
    }
}

/**
 *
 * Function Name : FMC_PflashSectorRead
 * Description   : This function read pflash sector.
 *
 * STATUS_SUCCESS: read success
 *
 */
__TIM_CODE status_t FMC_PflashSectorRead(flash_read_mode_t readMode, flash_area_t flashArea, uint32_t flashAddr,
                              void *recvBuf, uint32_t size)
{
    uint32_t nbytes = 0;
    uint8_t u8Data[8];
    uint32_t addrAlign8;
    uint32_t offset;
    uint32_t length;
    uint32_t i;
    uint32_t addr;
    uint8_t *u8Buf      = (uint8_t *)recvBuf;
    uint32_t flash_size = 0;

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
        FMC_PflashReadCmd(readMode, flashArea, addrAlign8, (uint32_t *)&u8Data[0], (uint32_t *)&u8Data[4]);

        /* readout data on nonAlign address */
        if (offset != 0 && i == 0) {
            if (length >= size) {
                memcopy(&u8Buf[0], &u8Data[offset], size);
                i = i + size;
                break;
            } else {
                memcopy(&u8Buf[0], &u8Data[offset], length);
                i = i + length;
                addrAlign8 += 8;
                continue;
            }
        }
        length = (i + nbytes) > (size - 1) ? nbytes : 8;
        memcopy(&u8Buf[i], &u8Data[0], length);
        i = i + length;
        addrAlign8 += 8;
    }
    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : FMC_DflashProgramCmd
 * Description   : This function executes dflash programming command.
 *
 * The dflash contains NVR(16Kbyte) and main sector(128Kbyte).
 *
 * If program command generates an error will return false, Otherwise return true.
 *
 */
status_t FMC_DflashProgramCmd(flash_area_t flashArea, uint32_t flashAddr, uint32_t data0, uint32_t data1)
{

    status_t osifError = STATUS_SUCCESS;
    fmc_dflash_program_cmd_status_type_t status;
    fmc_dflash_program_cmd_cfg_type_t cmdCfg;
    uint32_t loopCount = 0;

#ifdef PLATFORM_EAM2011
    /*If it is the high 8 sector of the NVR, address conversion is required*/
    if ((flashAddr > 0x1FFF) && (flashArea == NVR_SECTOR)) {
        flashAddr |= 0x10000;
    } else {
        // flashAddr = flashAddr;
    }
#endif
    FMC->FMC_DFLASH_PROGRAM_CMD_DATA0 = data0;
    FMC->FMC_DFLASH_PROGRAM_CMD_DATA1 = data1;

    cmdCfg.w                        = 0;
    cmdCfg.b.flash_addr             = flashAddr;
    cmdCfg.b.sector_sel             = flashArea;
    FMC->FMC_DFLASH_PROGRAM_CMD_CFG = cmdCfg.w;

    do {
        status.w = FMC->FMC_DFLASH_PROGRAM_CMD_STATUS;
        OS_DelayUs(FMC_OPERATION_DELAY);
    } while ((!status.b.cmd_stat) && (loopCount++ < FMC_ERR_LOOP));

    /*checkout status register*/
    if ((status.b.err_stat == 1) || (loopCount > FMC_ERR_LOOP)) {
        osifError = STATUS_FLASH_PROFRAM_FAIL;
    } else {
        osifError = STATUS_SUCCESS;
    }
    return osifError;
}

/**
 *
 * Function Name : FMC_DflashReadCmd
 * Description   : This function executes dflash read command.
 *
 * Each read operation reads 8 bytes.
 *
 * NORMAL_READ is normal read operation, can be used to read all dffalsh area.
 * RECALL_READ is a reliable read mode with a relatively slow read speed. It is generally used for nvr read operations.
 * VERIFY_READ is used to check the erase result.
 *
 */
status_t FMC_DflashReadCmd(flash_read_mode_t readMode, flash_area_t flashArea, uint32_t flashAddr,
                           uint32_t *data0, uint32_t *data1)
{
    status_t osifError = STATUS_SUCCESS;
    fmc_dflash_read_cmd_status_type_t status;
    fmc_dflash_read_cmd_cfg_type_t cmdCfg;

#ifdef PLATFORM_EAM2011
    /*If it is the high 8 sector of the NVR, address conversion is required*/
    if ((flashAddr > 0x1FFF) && (flashArea == NVR_SECTOR)) {
        flashAddr |= 0x10000;
    } else {
        // flashAddr = flashAddr;
    }
#endif

    cmdCfg.w                     = 0;
    cmdCfg.b.flash_addr          = flashAddr;
    cmdCfg.b.sector_sel          = flashArea;
    cmdCfg.b.read_mode           = readMode;
    FMC->FMC_DFLASH_READ_CMD_CFG = cmdCfg.w;

    do {
        status.w = FMC->FMC_DFLASH_READ_CMD_STATUS;
    } while (!status.b.cmd_stat);

    *data0 = FMC->FMC_DFLASH_READ_CMD_DATA0;
    *data1 = FMC->FMC_DFLASH_READ_CMD_DATA1;

    return osifError;
}

/**
 *
 * Function Name : FMC_DflashSectorVerifyRead
 * Description   : This function read dflash using verify read mode.
 *
 * verify read is used to check the erase result by applying "and" operator
 * to the specified sector data. If the read data0 is not 0xffffffff or data1 is
 * not 0xffffffff, means the erase failed.
 *
 */
status_t FMC_DflashSectorVerifyRead(flash_area_t flashArea, uint32_t flashAddr)
{
    uint32_t data0, data1;

    FMC_DflashReadCmd(VERIFY_READ, flashArea, flashAddr, &data0, &data1);

    if (data0 != 0xFFFFFFFF || data1 != 0xFFFFFFFF) {
        return STATUS_FLASH_READ_FAIL;
    }

    return STATUS_SUCCESS;
}

/**
 *
 * Function Name : FMC_DflashEraseRetryCmd
 * Description   : This function executes dflash erase cmd using retry mode.
 *
 * If nomal erase fail, will use retry mode to erase again.
 * Retry times is max three.
 *
 */
void FMC_DflashEraseRetryCmd(flash_erase_area_t eraseMode, uint32_t retry, flash_area_t flashArea,
                             uint32_t flashAddr)
{
    fmc_dflash_erase_cmd_status_type_t status;
    fmc_dflash_erase_cmd_cfg_type_t cmdCfg;
    uint32_t loopCount = 0;

    cmdCfg.w            = 0;
    cmdCfg.b.flash_addr = flashAddr;
    cmdCfg.b.sector_sel = flashArea;
    cmdCfg.b.erase_mode = eraseMode;
    cmdCfg.b.retry      = retry & 0x3;

    FMC->FMC_DFLASH_ERASE_CMD_CFG = cmdCfg.w;

    do {
        status.w = FMC->FMC_DFLASH_ERASE_CMD_STATUS;
        OS_DelayUs(FMC_OPERATION_DELAY);
    } while ((!status.b.cmd_stat) && (loopCount++ < FMC_ERR_LOOP));
}

/**
 *
 * Function Name : FMC_DflashEraseCmd
 * Description   : This function executes dflash erase cmd .
 *
 */
status_t FMC_DflashEraseCmd(flash_erase_area_t eraseMode, flash_area_t flashArea, uint32_t flashAddr)
{
    fmc_dflash_erase_cmd_status_type_t status;
    fmc_dflash_erase_cmd_cfg_type_t cmdCfg;
    uint8_t dRetry     = 1;
    uint32_t loopCount = 0;

#ifdef PLATFORM_EAM2011
    /*If it is the high 8 sector of the NVR, address conversion is required*/
    if ((flashAddr > 0x1FFF) && (flashArea == NVR_SECTOR)) {
        flashAddr |= 0x10000;
    } else {
        // flashAddr = flashAddr;
    }
#endif

    cmdCfg.w            = 0;
    cmdCfg.b.flash_addr = flashAddr;
    cmdCfg.b.sector_sel = flashArea;
    cmdCfg.b.erase_mode = eraseMode;
    cmdCfg.b.retry      = 0;

    FMC->FMC_DFLASH_ERASE_CMD_CFG = cmdCfg.w;

    /* Wait until dFlash erase finished */
    do {
        status.w = FMC->FMC_DFLASH_ERASE_CMD_STATUS;
        OS_DelayUs(FMC_OPERATION_DELAY);
    } while ((!status.b.cmd_stat) && (loopCount++ < FMC_ERR_LOOP));

    /* check for a normal exit */
    if ((status.b.err_stat == 1) || (loopCount > FMC_ERR_LOOP)) {
        return STATUS_ERROR;
    }

    while (FMC_DflashSectorVerifyRead(flashArea, flashAddr) != STATUS_SUCCESS && dRetry < 4) {
        FMC_DflashEraseRetryCmd(eraseMode, dRetry++, flashArea, flashAddr);
    }

    if (dRetry >= 4) {
        dRetry = 1;
        return STATUS_FLASH_ERASE_FAIL;
    } else {
        dRetry = 1;
        return STATUS_SUCCESS;
    }
}

/**
 *
 * Function Name : FMC_DflashSectorRead
 * Description   : This function read dflash.
 *
 * STATUS_SUCCESS: read success
 *
 */
status_t FMC_DflashSectorRead(flash_read_mode_t readMode, flash_area_t flashArea, uint32_t flashAddr,
                              void *recvBuf, uint32_t size)
{
    uint32_t nbytes = 0;
    uint32_t addrAlign8;
    uint32_t length;
    uint32_t offset;
    uint32_t i;
    uint32_t addr;
    uint8_t u8Data[8];
    uint8_t *u8Buf      = (uint8_t *)recvBuf;
    uint32_t flash_size = 0;

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
        FMC_DflashReadCmd(readMode, flashArea, addrAlign8, (uint32_t *)&u8Data[0], (uint32_t *)&u8Data[4]);
        /* readout data on nonAlign address */
        if (offset != 0 && i == 0) {
            if (length >= size) {
                memcopy(&u8Buf[0], &u8Data[offset], size);
                i = i + size;
                break;
            } else {
                memcopy(&u8Buf[0], &u8Data[offset], length);
                i = i + length;
                addrAlign8 += 8;
                continue;
            }
        }

        length = (i + nbytes) > (size - 1) ? nbytes : 8;
        memcopy(&u8Buf[i], &u8Data[0], length);
        i = i + length;
        addrAlign8 += 8;
    }

    return STATUS_SUCCESS;
}
