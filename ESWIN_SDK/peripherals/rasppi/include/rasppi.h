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
 * @file rasppi.h
 * @brief
 * @author
 * @date 2023-12-14
 *
 * @details
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __RASPPI_H__
#define __RASPPI_H__

#include "osal.h"
#include "pins_driver.h"
#include "spi_common.h"
#include "spi_sd_driver.h"
#include "spi_master_driver.h"
#include "pwm_driver.h"
#include "pdma_driver.h"
#include "i2c_driver.h"

#define NUM_OF_CONFIGURED_PINS (12U)
#define INST_SPI_3 (3U)
#define INST_PWM_1 (1U)
#define PDMA_CHN_STATE_COUNT      (1U)
#define PDMA_CHANNEL_CONFIG_COUNT (1U)
#define INST_PDMA_0 (0U)
#define INST_I2C_1 (1U)

extern pin_settings_config_t g_stPinmuxConfigArr[NUM_OF_CONFIGURED_PINS];

extern spi_state_t g_stSpiState_3;
extern spi_master_config_t g_stSpi3MasterConfig0;

extern pwm_state_t g_stPwmState_1;
extern pwm_config_t g_stPwm1Config0;

extern pdma_state_t g_stPdmaState0;
extern pdma_channel_config_t g_stPdma0ChannelConfig0;
extern pdma_chn_state_t g_stPdma0ChnState0;
extern pdma_chn_state_t *g_stPdma0ChnStateArray[PDMA_CHN_STATE_COUNT];
extern const pdma_channel_config_t *g_stPdma0ChannelConfigArray[PDMA_CHANNEL_CONFIG_COUNT];
extern pdma_user_config_t g_stPdma0UserConfig0;

extern i2c_master_user_config_t g_stI2c1MasterUserConfig0;

// Examples Config
#define CONFIG_RASPPI_LED       1
#define CONFIG_RASPPI_BUZZER    1
#define CONFIG_RASPPI_PWM       1
#define CONFIG_RASPPI_I2C       1
#define CONFIG_RASPPI_SPISD     1

void Rasppi_Led_Example(void);
void RaspPi_Buzzer_Example(void);
void RaspPi_Pwm_Example(void);
bool RaspPi_I2C_Example(void);
bool RaspPi_SpiSd_Example(void);

#endif /* __RASPPI_H__ */
