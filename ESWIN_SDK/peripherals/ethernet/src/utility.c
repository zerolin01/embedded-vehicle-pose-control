/**
  * *****************************************************************************
 * @file                utility.c
 * @author              WIZnet Software Team
 * @version             V1.0
 * @date                2015-02-14
 * @brief               public function
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
 * @file utility.c
 * @brief utility source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */
#include "w5500.h"
#include "w5500_conf.h"
#include "utility.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/**
*@brief         char to 8 bit integer function
*@param         str, base
*@return        num: integer
*/
uint16 atoi16(char* str,uint16 base)
{
    unsigned int num = 0;
    while (*str !=0)
            num = num * base + c2d(*str++);
    return num;
}

/**
*@brief         char to 32 bit integer function
*@param         str, base
*@return        num: integer
*/
uint32 atoi32(char* str,uint16 base    )
{
    uint32 num = 0;
    while (*str !=0)
            num = num * base + c2d(*str++);
    return num;
}

/**
*@brief         string to decimal or hexadecimal function
*@param         str;len
*@return        success - 1, fail - 0
*/
int validatoi(char* str, int base,int* ret)
{
    int c;
    char* tstr = str;
    if(str == 0 || *str == '\0') return 0;
    while(*tstr != '\0')
    {
        c = c2d(*tstr);
        if( c >= 0 && c < base) tstr++;
        else    return 0;
    }

    *ret = atoi16(str,base);
    return 1;
}

/**
*@brief         replace a special char in a string with a new char
*@param         str; oldchar; newchar
*@return        none
*/
void replacetochar(char * str,    char oldchar,char newchar    )
{
    int x;
    for (x = 0; str[x]; x++)
        if (str[x] == oldchar) str[x] = newchar;
}

/**
*@brief        decimal to char function
*@param        c
*@return       char data
*/
char c2d(uint8 c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return 10 + c -'a';
    if (c >= 'A' && c <= 'F')
        return 10 + c -'A';

    return (char)c;
}

/**
*@brief         16 bit character high 8 bit to low 8 bit conversion
*@param         i
*@return        conversion data
*/
uint16 swaps(uint16 i)
{
    uint16 ret=0;
    ret = (i & 0xFF) << 8;
    ret |= ((i >> 8)& 0xFF);
    return ret;
}
/**
*@brief         32 bit character high-low bit conversion
*@param         i
*@return        conversion data
*/
uint32 swapl(uint32 l)
{
    uint32 ret=0;
    ret = (l & 0xFF) << 24;
    ret |= ((l >> 8) & 0xFF) << 16;
    ret |= ((l >> 16) & 0xFF) << 8;
    ret |= ((l >> 24) & 0xFF);
    return ret;
}

/**
*@brief         string manipulation
*@param         src: target string; s1,s2: operation string
*@return        none
*/
void mid(int8* src, int8* s1, int8* s2, int8* sub)
{
    int8* sub1;
    int8* sub2;
    uint16 n;

    sub1=strstr(src,s1);
    sub1+=strlen(s1);
    sub2=strstr(sub1,s2);
    n=sub2-sub1;
    strncpy(sub,sub1,n);
    sub[n]=0;
}

/**
*@brief         ip address translation
*@param         adr: address; ip: ip
*@return        none
*/
void inet_addr_(unsigned char* addr,unsigned char *ip)
{
    int i;
    char taddr[30];
    char * nexttok;
    char num;
    strcpy(taddr,(char *)addr);

    nexttok = taddr;
    for(i = 0; i < 4 ; i++)
    {
        nexttok = strtok(nexttok,".");
        if(nexttok[0] == '0' && nexttok[1] == 'x') num = atoi16(nexttok+2,0x10);
        else num = atoi16(nexttok,10);

        ip[i] = num;
        nexttok = NULL;
    }
}

/**
*@brief         convert 32 bit address to decimal
*@param         addr: address
*@return        decimal data
*/
char* inet_ntoa(
    unsigned long addr
    )
{
    static char addr_str[32];
    memset(addr_str,0,32);
    sprintf(addr_str,"%d.%d.%d.%d",(int)(addr>>24 & 0xFF),(int)(addr>>16 & 0xFF),(int)(addr>>8 & 0xFF),(int)(addr & 0xFF));
    return addr_str;
}

/**
*@brief         convert hexadeccimal address to decimal
*@param         addr: address
*@return        decimal data
*/
char* inet_ntoa_pad(unsigned long addr)
{
    static char addr_str[16];
    memset(addr_str,0,16);
    printf(addr_str,"%03d.%03d.%03d.%03d",(int)(addr>>24 & 0xFF),(int)(addr>>16 & 0xFF),(int)(addr>>8 & 0xFF),(int)(addr & 0xFF));
    return addr_str;
}

/**
*@brief         verify ip address
*@param         ip: addr
*@return        success: 1; fail: 0
*/
char verify_ip_address(char* src, uint8 * ip)
{
    int i;
    int tnum;
    char tsrc[50];
    char* tok = tsrc;

    strcpy(tsrc,src);

    for(i = 0; i < 4; i++)
    {
        tok = strtok(tok,".");
        if ( !tok ) return 0;
        if(tok[0] == '0' && tok[1] == 'x')
        {
            if(!validatoi(tok+2,0x10,&tnum)) return 0;
        }
        else if(!validatoi(tok,10,&tnum)) return 0;

        ip[i] = tnum;

        if(tnum < 0 || tnum > 255) return 0;
        tok = NULL;
    }
    return 1;
}

/**
*@brief        convert unsigned short to big-endian data
*@param        hostshort: target data
*@return       big-endian data
*/
uint16 htons(uint16 hostshort)
{
    /**< A 16-bit number in host byte order.  */
    return swaps(hostshort);
}

/**
*@brief        convert unsigned long to big-endian data
*@param        hostlong: target data
*@return       big-endian data
*/
unsigned long htonl(unsigned long hostlong)
{
    /**< hostlong  - A 32-bit number in host byte order.  */
    return swapl(hostlong);
}

/**
*@brief        convert big-endian data to unsigned long
*@param        netshort: big-endian data
*@return       unsigned long
*/
unsigned long ntohs(unsigned short netshort)
{
    /**< netshort - network odering 16bit value */
    return htons(netshort);
}

/**
*@brief        convert big-endian data to unsigned long
*@param        netshort: big-endian data
*@return       unsigned long
*/
unsigned long ntohl(unsigned long netlong)
{
    return htonl(netlong);
}

/**
*@brief        calculates the string check value
*@param        src: convert datal len: data length
*@return       check value
*/
unsigned short checksum(unsigned char * src, unsigned int len)
{
    u_int1 sum, tsum, i, j;
    u_long lsum;

    j = len >> 1;

    lsum = 0;

    for (i = 0; i < j; i++)
    {
        tsum = src[i * 2];
        tsum = tsum << 8;
        tsum += src[i * 2 + 1];
        lsum += tsum;
    }

    if (len % 2)
    {
        tsum = src[i * 2];
        lsum += (tsum << 8);
    }


    sum = lsum;
    sum = ~(sum + (lsum >> 16));
    return (u_short) sum;
}
