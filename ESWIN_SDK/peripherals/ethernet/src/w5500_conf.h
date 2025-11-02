/**
  * *****************************************************************************
 * @file                w5500_conf.h
 * @author              WIZnet Software Team
 * @version             V1.0
 * @date                2015-02-14
 * *****************************************************************************
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
 * @file w5500_conf.h
 * @brief w5500_conf header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */
#ifndef _W5500_CONF_H_
#define _W5500_CONF_H_

#include "pins_driver.h"
#include "spi_master_driver.h"
#include "stdio.h"
#include "types.h"
#include <osal.h>

typedef  void (*pFunction)(void);

extern uint8    remote_ip[4];
extern uint16   remote_port;
extern uint16   local_port;
extern uint8    use_dhcp;
extern uint8    use_eeprom;

#define FW_VER_HIGH               1
#define FW_VER_LOW                0
#define ON                        1
#define OFF                       0
#define HIGH                      1
#define LOW                       0
/* size of data */
#define MAX_BUF_SIZE              1460
/* 30s */
#define KEEP_ALIVE_TIME           30
#define TX_RX_MAX_BUF_SIZE        2048
#define EEPROM_MSG_LEN            sizeof(EEPROM_MSG)
/* use define ip */
#define IP_FROM_DEFINE            0
/* use dhcp get ip */
#define IP_FROM_DHCP              1
/* use eeprom define ip */
#define IP_FROM_EEPROM            2
/* select ip */
extern uint8    ip_from;
#pragma pack(1)

typedef struct _CONFIG_MSG
{
  uint8 mac[6];                       /* mac */
  uint8 lip[4];                       /* local ip */
  uint8 sub[4];                       /* subnet mask */
  uint8 gw[4];                        /* gateway */
  uint8 dns[4];                       /* DNS server address */
  uint8 rip[4];                       /* remote ip */
    uint8 sw_ver[2];                  /* software version */

}CONFIG_MSG;
#pragma pack()

#pragma pack(1)

typedef struct _EEPROM_MSG
{
    uint8 mac[6];                     /* mac */
  uint8 lip[4];                       /* local ip */
  uint8 sub[4];                       /* subnet mask */
  uint8 gw[4];                        /* gateway */
}EEPROM_MSG_STR;
#pragma pack()

extern EEPROM_MSG_STR EEPROM_MSG;
extern CONFIG_MSG      ConfigMsg;
extern uint8 dhcp_ok;
extern uint32    dhcp_time;
extern volatile uint8_t        ntptimer;

void IINCHIP_WRITE( uint32 addrbsb,  uint8 data);
uint8 IINCHIP_READ(uint32 addrbsb);
uint16 wiz_write_buf(uint32 addrbsb,uint8* buf,uint16 len);
uint16 wiz_read_buf(uint32 addrbsb, uint8* buf,uint16 len);
void reset_w5500(void);
void set_w5500_mac(void);
void set_w5500_ip(void);

#endif /* _W5500_CONF_H_ */
