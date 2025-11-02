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
 * @file lora_driver.h
 * @brief LORA driver header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-17
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __LORA_DRIVER_H__
#define __LORA_DRIVER_H__

#include "callbacks.h"
#include "emps_platform.h"
#include "osal.h"
#include "status.h"
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief  Configures lora send complete.
 */
void OnTxDone(void);

/**
 * @brief  Configures lora receive complete.
 *
 * @param  payload receive data.
 * @param  size    receive size.
 * @param  rssi    rssi.
 * @param  snr     snr.
 */
void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);

/**
 * @brief  Configures lora send timeout.
 */
void OnTxTimeout(void);

/**
 * @brief  Configures lora receive timeout.
 */
void OnRxTimeout(void);

/**
 * @brief  Configures lora receive error.
 */
void OnRxError(void);

/**
 * @brief  Configures lora send data.
 *
 * @param  data send data.
 * @param  size   send size.
 */
void LoRa_DRV_Send(uint8_t *data, uint8_t size);

/**
 * @brief  Configures lora receive data.
 */
void LoRa_DRV_Receive(void);

/**
 * @brief  Configures lora sleep.
 */
void LoRa_DRV_Sleep(void);

/**
 * @brief  Configures lora send continuous wave.
 */
void LoRa_DRV_TxContinuousWave(void);

/**
 * @brief  Configures lora init.
 */
void LoRa_DRV_Init(void);

#endif /* __LORA_DRIVER_H__ */
