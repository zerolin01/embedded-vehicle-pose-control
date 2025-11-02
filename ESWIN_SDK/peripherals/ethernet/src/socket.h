/**
  * *****************************************************************************
 * @file                socket.h
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
 * @file socket.h
 * @brief socket header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */
#ifndef    _SOCKET_H_
#define    _SOCKET_H_

#include "types.h"

#define SOCK_TCPS             0
#define SOCK_HUMTEM           0
#define SOCK_PING             0
#define SOCK_TCPC             1
#define SOCK_UDPS             2
#define SOCK_WEIBO            2
#define SOCK_DHCP             3
#define SOCK_HTTPS            4
#define SOCK_DNS              5
#define SOCK_SMTP             6
#define SOCK_NTP              7
//#define NETBIOS_SOCK        6

// Opens a socket(TCP or UDP or IP_RAW mode)
extern uint8 socket(SOCKET s, uint8 protocol, uint16 port, uint8 flag);
// Close socket
//extern void close(SOCKET s);
// Establish TCP connection (Active connection)
extern uint8 connect(SOCKET s, uint8 * addr, uint16 port);
// disconnect the connection
extern void disconnect(SOCKET s);
// Establish TCP connection (Passive connection)
extern uint8 listen(SOCKET s);
// Send data (TCP)
extern uint16 send(SOCKET s, const uint8 * buf, uint16 len);
// Receive data (TCP)
extern uint16 recv(SOCKET s, uint8 * buf, uint16 len);
// Send data (UDP/IP RAW)
extern uint16 sendto(SOCKET s, const uint8 * buf, uint16 len, uint8 * addr, uint16 port);
// Receive data (UDP/IP RAW)
extern uint16 recvfrom(SOCKET s, uint8 * buf, uint16 len, uint8 * addr, uint16  *port);

#ifdef __MACRAW__
void macraw_open(void);
//Send data (MACRAW)
uint16 macraw_send( const uint8 * buf, uint16 len );
//Recv data (MACRAW)
uint16 macraw_recv( uint8 * buf, uint16 len );
#endif

#endif /* _SOCKET_H_ */
