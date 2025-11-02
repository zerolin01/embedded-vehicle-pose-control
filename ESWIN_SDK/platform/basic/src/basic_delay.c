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
 * @file basic_delay.c
 * @brief emps delay
 * @author abu (abu@eswincomputing.com)
 * @date 2023-03-24
 * 
 * Modification History :
 * Date:               Version:                    Author:     
 * Changes: 
 * 
 */

#include "basic_common.h"

extern struct basic_platform_operations_s platform_ops;

/**
 * @brief BASIC_DelayS
 * @details
 * 		Wait for a specified time.The accuracy is seconds.
 * @param
 * 		seconds - how many seconds need to be wait.
 * @retval
 * 		status code that indicates the execution status of the function.
 * */

int BASIC_DelayS(uint32_t seconds)
{
    uint64_t start_mtime, delta_mtime;
    uint32_t mtime_freq = platform_ops.get_timer_freq();

    uint64_t delay_ticks = (mtime_freq * (uint64_t)seconds);

    start_mtime = SysTimer_GetLoadValue();

    do {
        delta_mtime = SysTimer_GetLoadValue() - start_mtime;
    } while (delta_mtime < delay_ticks);

    return 0;
}

/**
 * @brief BASIC_DelayMs
 * @details
 * 		Wait for a specified time.The accuracy is milliseconds.
 * @param
 * 		milliseconds - how many milliseconds need to be wait.
 * @retval
 * 		status code that indicates the execution status of the function.
 * */
int BASIC_DelayMs(uint32_t milliseconds)
{
    uint64_t start_mtime, delta_mtime;
    uint32_t mtime_freq = platform_ops.get_timer_freq();

    uint64_t delay_ticks = (mtime_freq * (uint64_t)milliseconds) / 1000;

    start_mtime = SysTimer_GetLoadValue();

    do {
        delta_mtime = SysTimer_GetLoadValue() - start_mtime;
    } while (delta_mtime < delay_ticks);

    return 0;
}

/**
 * @brief BASIC_DelayUs
 * @details
 * 		Wait for a specified time.The accuracy is microseconds.
 * @param
 * 		microseconds - how many microseconds need to be wait.
 * @retval
 * 		status code that indicates the execution status of the function.
 * */
int BASIC_DelayUs(uint32_t microseconds)
{
    uint64_t start_mtime, delta_mtime;
    uint32_t mtime_freq = platform_ops.get_timer_freq();

    uint64_t delay_ticks = (mtime_freq * (uint64_t)microseconds) / 1000 / 1000;

    start_mtime = SysTimer_GetLoadValue();

    do {
        delta_mtime = SysTimer_GetLoadValue() - start_mtime;
    } while (delta_mtime < delay_ticks);

    return 0;
}
