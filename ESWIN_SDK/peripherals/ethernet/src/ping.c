/**
  * *****************************************************************************
 * @file                ping.c
 * @author              WIZnet Software Team
 * @version             V1.0
 * @date                2015-02-14
 * @brief               ping demo
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
 * @file ping.c
 * @brief ping source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */
#include "ping.h"

#define       PING_DEBUG
PINGMSGR      PingRequest;
PINGMSGR      PingReply;
static uint16 RandomID = 0x1234;
static uint16 RandomSeqNum = 0x4321;
uint8         ping_reply_received = 0;
uint8         req=0;
uint8         rep=0;

/**
*@brief        automatic ping ip address function
*@param        s: socket number
*@param        addr：ip address
*@return       ping_res:0 success/1 error
*/
uint8 ping_auto(uint8 s, uint8 *addr)
{
    uint8 i;
    int32_t len = 0;
    uint8 cnt=0;
    uint8 ping_res = 1;
    for(i = 0; i<=3;i++)
    {
        OS_DelayMs(10);
        switch(getSn_SR(s))
        {
            case SOCK_CLOSED:
                close(s);
                IINCHIP_WRITE(Sn_PROTO(s), IPPROTO_ICMP);
                if(socket(s,Sn_MR_IPRAW,3000,0)!=0)
                { }
                while(getSn_SR(s)!=SOCK_IPRAW);
                OS_DelayUs(1000);
                OS_DelayUs(1000);
            break;
            case SOCK_IPRAW:
                ping_request(s, addr);
                req++;
                while(1)
                {
                    if ( (len = getSn_RX_RSR(s) ) > 0)
                    {
                        ping_reply(s, addr, len);
                        OS_DelayUs(500);
                        rep++;
                        break;
                    }
                    else if(cnt > 200)
                    {
                        printf( "Request Time out. \r\n");
                        cnt = 0;
                        break;
                    }
                    else
                    {
                        cnt++;
                        OS_DelayMs(1);
                    }
                    /* wait_time for 2 seconds, Break on fail */
                }

            break;
            default:
            break;
        }

        #ifdef PING_DEBUG
        if(rep!=0)
        {
            printf("Ping Request = %d, PING_Reply = %d\r\n",req,rep);
            if(rep == req) {
                ping_res = 0;
                printf( "PING SUCCESS\r\n" );
            } else {
                ping_res = 2;
                printf( "REPLY_ERROR\r\n" );
            }
        }
        #endif
    }

    return ping_res;
}

/**
*@brief        set numbers of ping ip
*@param        s: socket number
*@param        addr:  ip address
*@param        pCount: number of ping
*@return       none
*/
void ping_count(uint8 s, uint16 pCount, uint8 *addr)
{
    uint16 rlen, cnt,i;
    cnt = 0;

    for(i=0; i<pCount+1;i++)
    {

        if(i!=0)
        {
            /* Output count number */
            printf( "\r\nNo.%d\r\n",   (i-1));
        }
        switch(getSn_SR(s))
        {
            case SOCK_CLOSED:
                close(s);
                /* Create Socket */
                IINCHIP_WRITE(Sn_PROTO(s), IPPROTO_ICMP);
                /* Check socket register */
                while(getSn_SR(s)!=SOCK_IPRAW);
                OS_DelayUs(1000);
                OS_DelayUs(1000);
            break;
            case SOCK_IPRAW:
                ping_request(s, addr);
                req++;
                while(1)
                {
                    if ( (rlen = getSn_RX_RSR(s) ) > 0)
                    {
                        ping_reply(s, addr, rlen);
                        rep++;
                        if (ping_reply_received)  break;
                    }
                    if ( (cnt > 100) )
                    {
                        printf( "\r\nRequest Time out. \r\n") ;
                        cnt = 0;
                        break;
                    }
                    else
                    {
                        cnt++;
                        OS_DelayUs(50);
                    }
                }
            break;
            default:
            break;
        }
        if(req>=pCount)
        {
            #ifdef PING_DEBUG
            printf("Ping Request = %d, PING_Reply = %d\r\n",req,rep);
            if(rep == req)
            printf( "PING SUCCESS\r\n " );
            else
            printf( "REPLY_ERROR\r\n " );
            #endif
        }
    }
}

/**
*@brief        ping response function
*@param        s: socket number
*@param        addr: ip address
*@return       none
*/
uint8 ping_request(uint8 s, uint8 *addr)
{
    uint16 i;
    ping_reply_received = 0;
    PingRequest.Type = PING_REQUEST;
    PingRequest.Code = CODE_ZERO;
    PingRequest.ID = htons(RandomID++);
    PingRequest.SeqNum =htons(RandomSeqNum++);
    for(i = 0 ; i < BUF_LEN; i++)
    {
        PingRequest.Data[i] = (i) % 8;
    }
    PingRequest.CheckSum = 0;

    PingRequest.CheckSum = htons(checksum((uint8*)&PingRequest,sizeof(PingRequest)));

    if(sendto(s,(uint8 *)&PingRequest,sizeof(PingRequest),addr,3000)==0)
    {
        printf( "\r\n Fail to send ping-reply packet  r\n") ;
    }
    else
    {
        printf("Ping:%d.%d.%d.%d\r\n", (addr[0]), (addr[1]), (addr[2]), (addr[3]));
    }
    return 0;
}

/**
*@brief        parsing ping reply
*@param        s: socket number
*@param        addr: ping address
*@return       none
*/
uint8 ping_reply(uint8 s, uint8 *addr,  uint16 rlen)
{
    uint16 tmp_checksum;
    uint16 len;
    uint16 i;
    uint8 data_buf[128];

    uint16 port = 3000;
    // PINGMSGR PingReply;
    len = recvfrom(s, (uint8 *)data_buf,rlen,addr,&port);
    if(data_buf[0] == PING_REPLY)
    {
        PingReply.Type          = data_buf[0];
        PingReply.Code          = data_buf[1];
        PingReply.CheckSum   = (data_buf[3]<<8) + data_buf[2];
        PingReply.ID          = (data_buf[5]<<8) + data_buf[4];
        PingReply.SeqNum      = (data_buf[7]<<8) + data_buf[6];

        for(i=0; i<len-8 ; i++)
        {
            PingReply.Data[i] = data_buf[8+i];
        }
        tmp_checksum = ~checksum(data_buf,len);
        if(tmp_checksum != 0xffff)
            printf("tmp_checksum = %x\r\n",tmp_checksum);
        else
        {
            printf("%d.%d.%d.%d reply: ID=%x byte=%d \r\n",
            (addr[0]),  (addr[1]),  (addr[2]),  (addr[3]),htons(PingReply.ID),(rlen+6));
            ping_reply_received =1;
        }
    }
    else if(data_buf[0] == PING_REQUEST)
    {
        PingReply.Code      = data_buf[1];
        PingReply.Type      = data_buf[2];
        PingReply.CheckSum  = (data_buf[3]<<8) + data_buf[2];
        PingReply.ID          = (data_buf[5]<<8) + data_buf[4];
        PingReply.SeqNum      = (data_buf[7]<<8) + data_buf[6];
        for(i=0; i<len-8 ; i++)
        {
            PingReply.Data[i] = data_buf[8+i];
        }
        tmp_checksum = PingReply.CheckSum;
        PingReply.CheckSum = 0;
        if(tmp_checksum != PingReply.CheckSum)
        {
            printf( " \n CheckSum is in correct %x shold be %x \n",   (tmp_checksum),  htons(PingReply.CheckSum)) ;
        }
        else
        {        }
        printf("Request from %d.%d.%d.%d  ID:%x SeqNum:%x  :data size %d bytes\r\n",
        (addr[0]),  (addr[1]),  (addr[2]),  (addr[3]),  (PingReply.ID),  (PingReply.SeqNum),  (rlen+6) );
        ping_reply_received =1;
    }
    else
    {
        printf(" Unkonwn msg. \n");
    }
    return 0;
}

/**
*@brief        execute ping function
*@param        none
*@return       do_ping_res:0 success/1 error
*/
uint8 do_ping(void)
{
    uint8 do_ping_res;
    printf("------------start ping-----------------------\r\n");
    OS_DelayMs(1000);
    do_ping_res = ping_auto(SOCK_PING,remote_ip);
    printf("------------end ping------------------------\r\n");
    return do_ping_res;
}
