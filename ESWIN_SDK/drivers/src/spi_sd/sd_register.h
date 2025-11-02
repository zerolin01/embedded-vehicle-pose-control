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
 * @file sd_register.h
 * @brief SD driver register header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __SD_REGISTER_H__
#define __SD_REGISTER_H__

#include "osal.h"
#include "platform.h"
#include <stddef.h>

/* return value */
#define SD_ERROR                            1
#define SD_SUCCESS                          0
/* SD Card Type*/
#define SD_TYPE_ERR                         0X00
#define SD_TYPE_MMC                         0X01
#define SD_TYPE_V1                          0X02
#define SD_TYPE_V2                          0X04
#define SD_TYPE_V2HC                        0X06
/* default value */
#define CMD_AND_ACMD_DIS_VALUE              0x80
#define APPCOMMAND_CLEAR_VALUE              0x7F
#define CRC_DEFAULT_VALUE                   0x01
#define SD_DEFAULT_SIZE                     0x200
#define SD_RESPONSE_READ_VALUE              0xFE
#define SD_WAIT_RESPONSE_COUNT              0xFFFF
#define SD_SEND_GINGLE_BLOCK_TOKEN          0xFE
#define SD_SEND_MULTI_BLOCK_TOKEN           0xFC
#define SD_END_SEND_MULTI_BLOCK_TOKEN       0xFD
#define SD_SEND_MULTI_BLOCK_SUCCESS         0x5
/* command byte mask value */
#define COMMAND_BYTE1_CMD_MASK              0x40
#define COMMAND_BYTE2_AGR_MASK              24
#define COMMAND_BYTE3_AGR_MASK              16
#define COMMAND_BYTE4_AGR_MASK              8
#define COMMAND_BYTE5_AGR_MASK              0
#define COMMAND_BYTE6_CRC_MASK              0x1
#define COMMAND_CMD0_CRC_MASK               0x95
#define COMMAND_CMD8_CRC_MASK               0x87
#define SD_OCR_CCS_MASK                     0x40
#define SD_SEND_MULTI_BLOCK_RESPONSE_MASK   0x1F
/* command argument vaule */
#define COMMAND_CMD8_ARG                    0x1AA
#define COMMAND_CMD0_ARG                    0x0
#define COMMAND_ACMD41_ARG_HCS              0x40000000
#define COMMAND_CMD58_ARG                   0x0
#define COMMAND_CMD16_ARG                   0x200
#define COMMAND_CMD12_ARG                   0x0
#define COMMAND_CMD9_ARG                    0x0
#define COMMAND_CMD10_ARG                   0x0
#define COMMAND_ACMD13_ARG                  0x0
/* command response vaule */
#define COMMAND_CMD8_R7_VOLTAGE_ACCEPTED    0x1
#define COMMAND_CMD8_R7_CHECK_PATTERN       0xAA
/* command vaule */
#define CMD0                                (0)
#define CMD1                                (1)
#define ACMD41                              (CMD_AND_ACMD_DIS_VALUE + 41)
#define CMD8                                (8)
#define CMD9                                (9)
#define CMD10                               (10)
#define CMD12                               (12)
#define ACMD13                              (CMD_AND_ACMD_DIS_VALUE + 13)
#define CMD16                               (16)
#define CMD17                               (17)
#define CMD18                               (18)
#define CMD23                               (23)
#define ACMD23                              (CMD_AND_ACMD_DIS_VALUE + 23)
#define CMD24                               (24)
#define CMD25                               (25)
#define CMD32                               (32)
#define CMD33                               (33)
#define CMD38                               (38)
#define CMD55                               (55)
#define CMD58                               (58)

#endif /* __SD_REGISTER_H__ */
