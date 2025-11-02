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
 * @file fmc_hw_access.h
 * @brief FMC driver hardware access header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-12
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __FMC_HW_ACCESS_H__
#define __FMC_HW_ACCESS_H__

#include "fmc_flash_driver.h"
#include "fmc_register.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define FMC_ERR_LOOP        0x1000
#define FMC_OPERATION_DELAY 8
typedef enum {
    NORMAL_READ = 0,
    RECALL_READ,
    VERIFY_READ,
} flash_read_mode_t;

/**
 * @brief  This function executes pFlash programming commands.
 *
 * @param  flashArea  MAIN/NVR area.
 * @param  flashAddr  pFlash address begin to write.
 * @param  data0      The first 4 bytes to write.
 * @param  data1      The second 4 bytes to write.
 * @return ** status_t STATUS_SUCCESS if write successfully.
 */
status_t FMC_PflashProgramCmd(flash_area_t flashArea, uint32_t flashAddr, uint32_t data0, uint32_t data1);

/**
 * @brief  This function executes pFlash read command. Each read operation reads 8 bytes.
 *
 * NORMAL_READ is normal read operation, can be used to read all pFlash area.
 * RECALL_READ is a reliable read mode with a relatively slow read speed. It is generally used for nvr read operations.
 * VERIFY_READ is used to check the erase result.
 *
 * @param  readMode    NORMAL_READ/RECALL_READ or VERIFY_READ.
 * @param  flashArea   MAIN/NVR area.
 * @param  flashAddr   pFlash address begin to read.
 * @param  data0       The fist 4 bytes.
 * @param  data1       The second 4 bytes.
 * @return ** status_t STATUS_SUCCESS if read successfully.
 */
status_t FMC_PflashReadCmd(flash_read_mode_t readMode, flash_area_t flashArea, uint32_t flashAddr,
                           uint32_t *data0, uint32_t *data1);

/**
 * @brief  Read pFlash using verify read mode.
 *
 * verify read is used to check the erase result by applying "and" operator
 * to the specified sector data. If the read data0 is not 0xffffffff or data1 is
 * not 0xffffffff, means the erase failed.
 *
 * @param  flashArea  MAIN/NVR sector
 * @param  flashAddr  pFlash address begin to read.
 * @return ** status_t   STATUS_SUCCESS if read successfully.
 */
status_t FMC_PflashSectorVerifyRead(flash_area_t flashArea, uint32_t flashAddr);

/**
 * @brief  pFlash retry erase function
 *
 * @param  eraseMode  Sector erase or entire sector erase.
 * @param  retry      Retry count.
 * @param  flashArea  Main area or NVR area.
 * @param  flashAddr  pFlash address begin to erase.
 */
void FMC_PflashEraseRetryCmd(flash_erase_area_t eraseMode, uint8_t retry, flash_area_t flashArea,
                             uint32_t flashAddr);

/**
 * @brief pFlash erase function.
 *
 * @param  eraseMode   Sector erase or entire sector erase.
 * @param  flashArea   MAIN area or NVR area.
 * @param  flashAddr   pFlash address begin to erase.
 * @return status_t    STATUS_SUCCESS if read successfully.
 */
status_t FMC_PflashEraseCmd(flash_erase_area_t eraseMode, flash_area_t flashArea, uint32_t flashAddr);

/**
 * @brief  pFlash read function.
 *
 * @param  readMode    NORMAL_READ/RECALL_READ or VERIFY_READ.
 * @param  flashArea   MAIN/NVR area.
 * @param  flashAddr   pFlash address begin to read.
 * @param  recvBuf     The data obtained by the flash is read.
 * @param  size        Read data length.
 * @return status_t    STATUS_SUCCESS if read successfully.
 */
status_t FMC_PflashSectorRead(flash_read_mode_t readMode, flash_area_t flashArea, uint32_t flashAddr,
                              void *recvBuf, uint32_t size);

/**
 * @brief  This function executes dFlash programming command.
 *
 * dFlash contains NVR(16Kbyte) and MAIN(128Kbyets) sectors.
 * @param  flashArea  MAIN/NVR sector.
 *
 * @param  flashAddr  dFlash address begin to write.
 * @param  data0      The first 4 bytes to write.
 * @param  data1      The second 4 bytes to write.
 * @return true
 * @return false
 */
status_t FMC_DflashProgramCmd(flash_area_t flashArea, uint32_t flashAddr, uint32_t data0, uint32_t data1);

/**
 * @brief  This function executes dFlash read command. Each read operation reads 8 bytes.
 *
 * NORMAL_READ is normal read operation, can be used to read all dFlash area.
 * RECALL_READ is a reliable read mode with a relatively slow read speed. It is generally used for nvr read operations.
 * VERIFY_READ is used to check the erase result.
 *
 * @param  readMode    NORMAL_READ/RECALL_READ or VERIFY_READ.
 * @param  flashArea   MAIN/NVR area.
 * @param  flashAddr   dFlash address begin to read.
 * @param  data0       The fist 4 bytes.
 * @param  data1       The second 4 bytes.
 * @return ** status_t  STATUS_SUCCESS if read successfully.
 */
status_t FMC_DflashReadCmd(flash_read_mode_t readMode, flash_area_t flashArea, uint32_t flashAddr,
                           uint32_t *data0, uint32_t *data1);

/**
 * @brief  Read dFlash using verify read mode.
 *
 * verify read is used to check the erase result by applying "and" operator
 * to the specified sector data. If the read data0 is not 0xffffffff or data1 is
 * not 0xffffffff, means the erase failed.
 *
 * @param  flashArea  MAIN/NVR sector
 * @param  flashAddr  dFlash address begin to read.
 * @return status_t STATUS_SUCCESS if read successfully.
 */
status_t FMC_DflashSectorVerifyRead(flash_area_t flashArea, uint32_t flashAddr);

/**
 * @brief  dFlash retry erase function
 *
 * @param  eraseMode  Sector erase or entire sector erase.
 * @param  retry      Retry count.
 * @param  flashArea  Main area or NVR area.
 * @param  flashAddr  dFlash address begin to erase.
 */
void FMC_DflashEraseRetryCmd(flash_erase_area_t eraseMode, uint32_t retry, flash_area_t flashArea,
                             uint32_t flashAddr);

/**
 * @brief dFlash erase function.
 *
 * @param  eraseMode   Sector erase or entire sector erase.
 * @param  flashArea   MAIN area or NVR area.
 * @param  flashAddr   dFlash address begin to erase.
 * @return status_t    STATUS_SUCCESS if read successfully.
 */
status_t FMC_DflashEraseCmd(flash_erase_area_t eraseMode, flash_area_t flashArea, uint32_t flashAddr);

/**
 * @brief  dFlash read function.
 *
 * @param  readMode    NORMAL_READ/RECALL_READ or VERIFY_READ.
 * @param  flashArea   MAIN/NVR area.
 * @param  flashAddr   dFlash address begin to read.
 * @param  recvBuf     The data obtained by the flash is read.
 * @param  size        Read data length.
 * @return status_t    STATUS_SUCCESS if read successfully.
 */
status_t FMC_DflashSectorRead(flash_read_mode_t readMode, flash_area_t flashArea, uint32_t flashAddr,
                              void *recvBuf, uint32_t size);

#if 0
/**
 * @brief  FMC change frequency.
 *
 * @param  frequency   FMC frequency.
 * @param  clockSelect FMC ClockSelect.
 */
void FMC_ChangeFre(uint32_t frequency, fmc_clocksource_t clockSelect);
#endif
#if defined(__cplusplus)
}
#endif

#endif
