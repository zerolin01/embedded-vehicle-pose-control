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
 * @file spi_sd_driver.h
 * @brief SPI SD driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __SPI_SD_DRIVER_H__
#define __SPI_SD_DRIVER_H__

#include "callbacks.h"
#include "emps_platform.h"
#include "osal.h"
#include "status.h"
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Initializes a SPI instance for a sd mode operation.
 *
 * @param  instance The instance number of the SPI peripheral.
 * @return uint8_t
 * SD_ERROR The initializes sd is error.
 * SD_SUCCESS The initializes sd is success.
 */
uint8_t SD_DRV_Init(uint32_t instance);

/**
 * @brief Read disk from sd card.
 *
 * @param  buffer The pointer to the data buffer of the data to read.
 * @param  addr The address of disk.
 * @param  blockCount The number of read dist block.
 * @return uint8_t
 * SD_ERROR The read disk is error.
 * SD_SUCCESS The read disk is success.
 */
uint8_t SD_DRV_ReadDisk(uint32_t instance, uint8_t *buffer, uint32_t addr, uint32_t blockCount);

/**
 * @brief Write disk from sd card.
 *
 * @param  buffer The pointer to the data buffer of the data to write.
 * @param  addr The address of disk.
 * @param  blockCount The number of write dist block.
 * @return uint8_t
 * SD_ERROR The write disk is error.
 * SD_SUCCESS The write disk is success.
 */
uint8_t SD_DRV_WriteDisk(uint32_t instance, uint8_t *buffer, uint32_t addr, uint32_t blockCount);

/**
 * @brief Get the total number of sectors of the sd card or the number of bytes of actual capacity.
 *
 * @param  flag The flag = 0/1, return sectors/capacity..
 * @return uint32_t
 * Sectors or Capacity.
 */
uint32_t SD_DRV_GetSecOrCapCount(uint32_t instance, uint8_t flag);

#endif /* __SPI_SD_DRIVER_H__ */
