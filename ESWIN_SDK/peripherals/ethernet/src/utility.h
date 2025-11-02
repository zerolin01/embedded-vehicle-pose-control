/**
  * *****************************************************************************
 * @file                utility.h
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
 * @file utility.h
 * @brief utility header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */
#ifndef _UTILITY_H
#define _UTILITY_H
#include "stdio.h"
#include "types.h"

/* Convert a string to integer number */
uint16 atoi16(char* str,uint16 base);
/* Convert a string to integer number */
uint32 atoi32(char* str,uint16 base);
//void itoa(uint16 n,uint8* str, uint8 len);
/* Verify character string and Convert it to (hexa-)decimal. */
int validatoi(char* str, int base, int* ret);
/* Convert a character to HEX */
char c2d(u_char c);
uint16 swaps(uint16 i);
uint32 swapl(uint32 l);
void replacetochar(char * str, char oldchar, char newchar);
void mid(int8* src, int8* s1, int8* s2, int8* sub);
void inet_addr_(unsigned char* addr,unsigned char *ip);
/* Convert 32bit Address into Dotted Decimal Format */
char* inet_ntoa(unsigned long addr);
char* inet_ntoa_pad(unsigned long addr);
/* Converts a string containing an (Ipv4) Internet Protocol decimal dotted address into a 32bit address */
uint32 inet_addr(unsigned char* addr);
/* Verify dotted notation IP address string */
char verify_ip_address(char* src, uint8 * ip);
/* htons function converts a unsigned short from host to TCP/IP network byte order (which is big-endian).*/
uint16 htons( unsigned short hostshort);
/* htonl function converts a unsigned long from host to TCP/IP network byte order (which is big-endian). */
uint32 htonl(unsigned long hostlong);
/* ntohs function converts a unsigned short from TCP/IP network byte order to host byte order (which is little-endian on Intel processors). */
uint32 ntohs(unsigned short netshort);
/* ntohl function converts a u_long from TCP/IP network order to host byte order (which is little-endian on Intel processors). */
uint32 ntohl(unsigned long netlong);
/* Calculate checksum of a stream */
uint16 checksum(unsigned char * src, unsigned int len);
/* Check Destination in local or remote */
// uint8 check_dest_in_local(u_long destip);

#endif /* _UTILITY_H */
