/**
  * *****************************************************************************
 * @file                w5500_conf.c
 * @author              WIZnet Software Team
 * @version             V1.0
 * @date                2015-02-14
 * @brief               configuration function
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
 * @file w5500_conf.c
 * @brief w5500_conf source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */
#include <stdio.h>
#include <string.h>
#include "w5500_conf.h"
#include "utility.h"
#include "w5500.h"
#include "dhcp.h"

#define SPI_NUM 3
/* remote ip */
uint8  remote_ip[4]={192,168,1,100};
/* remote port */
uint16 remote_port=1900;
/* configuration structure */
CONFIG_MSG  ConfigMsg;
/* EEPROM information structure */
EEPROM_MSG_STR EEPROM_MSG;
/* mac address, if multiple w5500 work on the same site, mac address must be different */
uint8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};
/* w5500 default ip */
uint8 local_ip[4]  ={192,168,1,88};
/* w5500 default subnet mask */
uint8 subnet[4]    ={255,255,255,0};
/* w5500 default gateway */
uint8 gateway[4]   ={192,168,1,1};
/* w5500 default DNS */
uint8 dns_server[4]={114,114,114,114};
/* local port */
uint16 local_port=5000;
/* ip configuration */
uint8    ip_from=IP_FROM_DEFINE;
/* dhcp success get ip */
uint8   dhcp_ok   = 0;
/* count ms*/
uint32    ms        = 0;
/* dhcp run count */
uint32    dhcp_time = 0;
/* npt coount/s */
volatile uint8_t        ntptimer  = 0;

/**
*@brief        configuration w5500 ip address
*@param        none
*@return       none
*/
void set_w5500_ip(void)
{
    memcpy(ConfigMsg.mac, mac, 6);
    memcpy(ConfigMsg.lip,local_ip,4);
    memcpy(ConfigMsg.sub,subnet,4);
    memcpy(ConfigMsg.gw,gateway,4);
    memcpy(ConfigMsg.dns,dns_server,4);
    if(ip_from==IP_FROM_DEFINE)
        printf("W5500 IP Information Configuration:\r\n");

    if(ip_from==IP_FROM_EEPROM)
    {
        // read_config_from_eeprom();
        if( *(EEPROM_MSG.mac)==0x00&& *(EEPROM_MSG.mac+1)==0x08&&*(EEPROM_MSG.mac+2)==0xdc)
        {
            printf(" IP from EEPROM\r\n");
            memcpy(ConfigMsg.lip,EEPROM_MSG.lip, 4);
            memcpy(ConfigMsg.sub,EEPROM_MSG.sub, 4);
            memcpy(ConfigMsg.gw, EEPROM_MSG.gw, 4);
        }
        else
        {
            printf(" not config EEPROM\r\n");
            // write_config_to_eeprom();
        }
    }

    if(ip_from==IP_FROM_DHCP)
    {
        if(dhcp_ok==1)
        {
            printf(" IP from DHCP\r\n");
            memcpy(ConfigMsg.lip,DHCP_GET.lip, 4);
            memcpy(ConfigMsg.sub,DHCP_GET.sub, 4);
            memcpy(ConfigMsg.gw,DHCP_GET.gw, 4);
            memcpy(ConfigMsg.dns,DHCP_GET.dns,4);
        }
        else
        {
            printf(" dhcp fail running\r\n");
            printf(" use define ip config w5500\r\n");
        }
    }

    ConfigMsg.sw_ver[0]=FW_VER_HIGH;
    ConfigMsg.sw_ver[1]=FW_VER_LOW;

    setSUBR(ConfigMsg.sub);
    setGAR(ConfigMsg.gw);
    setSIPR(ConfigMsg.lip);

    getSIPR (local_ip);
    printf("W5500 IP Address  : %d.%d.%d.%d\r\n", local_ip[0],local_ip[1],local_ip[2],local_ip[3]);
    getSUBR(subnet);
    printf("W5500 Subnet Mask : %d.%d.%d.%d\r\n", subnet[0],subnet[1],subnet[2],subnet[3]);
    getGAR(gateway);
    printf("W5500 Gateway     : %d.%d.%d.%d\r\n", gateway[0],gateway[1],gateway[2],gateway[3]);
}

/**
*@brief        configuration w5500 mac address
*@param        none
*@return       none
*/
void set_w5500_mac(void)
{
    memcpy(ConfigMsg.mac, mac, 6);
    setSHAR(ConfigMsg.mac);
    memcpy(DHCP_GET.mac, mac, 6);
}


/**
*@brief        reset w5500
*@param        none
*@return       none
*/
void reset_w5500(void)
{
    PINS_DRV_WritePin(PORTC, 22, 0);
    OS_DelayMs(2);
    PINS_DRV_WritePin(PORTC, 22, 1);
    OS_DelayMs(1600);
}

/**
*@brief        write a 8 bit data to w5500
*@param        addrbsb: address
*@param        data: 8 bit data
*@return       none
*/
void IINCHIP_WRITE( uint32 addrbsb,  uint8 data)
{
    uint8 txdata[4] = {0};
    uint8 rxdata[4] = {0};
    txdata[0] = (addrbsb & 0x00FF0000)>>16;
    txdata[1] = (addrbsb & 0x0000FF00)>> 8;
    txdata[2] = (addrbsb & 0x000000F8) + 4;
    txdata[3] = data;
    SPI_DRV_MasterTransferBlocking(SPI_NUM, txdata, rxdata, 4, 1000);
}

/**
*@brief        read a 8 bit data from w5500
*@param        addrbsb: address
*@param        data: read data
*@return       none
*/
uint8 IINCHIP_READ(uint32 addrbsb)
{
    uint8 txdata[4] = {0};
    uint8 rxdata[4] = {0};
    txdata[0] = (addrbsb & 0x00FF0000)>>16;
    txdata[1] = (addrbsb & 0x0000FF00)>> 8;
    txdata[2] = addrbsb & 0x000000F8;
    txdata[3] = 0;
    SPI_DRV_MasterTransferBlocking(SPI_NUM, txdata, rxdata, 4, 1000);
    return rxdata[3];
}

/**
*@brief        write number of len byte to w5500
*@param        addrbsb: address
*@param        buf: write data
*@param        len: write data length
*@return       len: length of data
*/
uint16 wiz_write_buf(uint32 addrbsb,uint8* buf,uint16 len)
{
    uint16 idx = 0;
    if(len == 0) printf("Unexpected2 length 0\r\n");

    uint8 txdata[len+3];
    uint8 rxdata[len+3];
    txdata[0] = (addrbsb & 0x00FF0000)>>16;
    txdata[1] = (addrbsb & 0x0000FF00)>> 8;
    txdata[2] = (addrbsb & 0x000000F8) + 4;

    for(idx = 0; idx < len; idx++)
    {
        txdata[idx+3] = buf[idx];
    }

    SPI_DRV_MasterTransferBlocking(SPI_NUM, txdata, rxdata, len+3, 1000);
    return len;
}

/**
*@brief        read number of len byte data from w5500
*@param        addrbsb: address
*@param        buf: read data buffer
*@param        len: length of data
*@return       len: length of data
*/
uint16 wiz_read_buf(uint32 addrbsb, uint8* buf,uint16 len)
{
    uint16 idx = 0;
    if(len == 0)
    {
        printf("Unexpected2 length 0\r\n");
    }

    uint8 txdata[len+3];
    uint8 rxdata[len+3];
    txdata[0] = (addrbsb & 0x00FF0000)>>16;
    txdata[1] = (addrbsb & 0x0000FF00)>> 8;
    txdata[2] = addrbsb & 0x000000F8;

    for(idx = 0; idx < len; idx++)
    {
        txdata[idx+3] = 0x00;
    }
    SPI_DRV_MasterTransferBlocking(SPI_NUM, txdata, rxdata, len+3, 1000);
    for(idx = 0; idx < len; idx++)
    {
        buf[idx] = rxdata[idx+3];
    }

    return len;
}
