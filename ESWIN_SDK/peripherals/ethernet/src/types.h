/**
  * *****************************************************************************
 * @file                types.h
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
 * @file types.h
 * @brief types header file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-16
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */
#ifndef _TYPE_H_
#define _TYPE_H_

/**< Maxmium number of socket  */
#define MAX_SOCK_NUM             8
typedef char                     int8;
typedef volatile char            vint8;
typedef unsigned char            uint8;
typedef volatile unsigned char   vuint8;
typedef int                      int16;
typedef unsigned short           uint16;
typedef long                     int32;
typedef unsigned long            uint32;
typedef uint8                    u_char;        /**< 8-bit value */
typedef uint8                    SOCKET;
typedef uint16                   u_short;       /**< 16-bit value */
typedef uint16                   u_int1;        /**< 16-bit value */
typedef uint32                   u_long;        /**< 32-bit value */

typedef union _un_l2cval
{
    u_long    lVal;
    u_char    cVal[4];
}un_l2cval;

typedef union _un_i2cval
{
    u_int1    iVal;
    u_char    cVal[2];
}un_i2cval;

#endif        /* _TYPE_H_ */
