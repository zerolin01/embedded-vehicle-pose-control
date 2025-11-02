/**
  * *****************************************************************************
 * @file                ping.h
 * @author              WIZnet Software Team
 * @version             V1.0
 * @date                2015-02-14
 * @brief               ping.h
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
 * @file ping.h
 * @brief ping header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */
#ifndef    _PING_H_
#define    _PING_H_

#include "w5500_conf.h"
#include "w5500.h"
#include "socket.h"
#include "types.h"
#include "utility.h"

#define BUF_LEN       128
#define PING_REQUEST  8
#define PING_REPLY    0
#define CODE_ZERO     0
#define SOCKET_ERROR  1
#define TIMEOUT_ERROR 2
#define SUCCESS       3
#define REPLY_ERROR   4
extern uint8 req;

typedef struct pingmsg
{
  uint8  Type;           /* 0 - Ping Reply, 8 - Ping Request */
  uint8  Code;           /* Always 0 */
  uint16  CheckSum;      /* Check sum */
  uint16  ID;            /* Identification */
  uint16  SeqNum;        /* Sequence Number */
  int8_t  Data[BUF_LEN]; /* Ping Data  : 1452 = IP RAW MTU - sizeof(Type+Code+CheckSum+ID+SeqNum) */
} PINGMSGR;

uint8 ping_auto(uint8 s, uint8 *addr);
void ping_count(uint8 s, uint16_t pCount, uint8 *addr);
uint8 ping_request(uint8 s, uint8 *addr);
uint8 ping_reply(uint8 s,  uint8 *addr, uint16_t rlen);
uint8 do_ping(void);

#endif /* _PING_H_ */
