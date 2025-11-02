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
 * @file tpiu_driver.c
 * @brief TPIU driver source file
 * @author abu (abu@eswincomputing.com)
 * @date 2023-01-30
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "tpiu_register.h"

/**
 *
 * Function Name : TPIU_DRV_TraceEnable
 * Description   : trace enable or disable
 *
 */
void TPIU_DRV_TraceEnable(bool Enable)
{
    te_control_t teControl;
    teControl.w           = 0;
    teControl.b.teActive  = 1;
    teControl.b.teEnable  = 1;
    teControl.b.teTracing = Enable;

    TRACE->teControl = teControl.w;
}

/**
 *
 * Function Name : TPIU_DRV_TracePath
 * Description   : select trace out path
 *
 */
void TPIU_DRV_TracePath(bool path)
{
    tpiu_control_reg_type_t ctl;

    ctl.w                = 0;
    ctl.b.trace_path_sel = path;

    TPIU->tpiu_control_reg = ctl.w;
}

/**
 *
 * Function Name : TPIU_DRV_TracePortSize
 * Description   : The meaning of bit N(0~31) is
 * 0: trace out port data width N+1 not support
 * 1: trace out port data width N+1 support
 *
 */
void TPIU_DRV_TracePortSize(uint8_t portWidth)
{
    uint32_t data;

    data                    = (1 << (portWidth - 1));
    TPIU->tpiu_curport_size = data;
}

/**
 *
 * Function Name : TPIU_DRV_TraceRamBase
 * Description   : The ram base address for tpiu data
 *
 */
void TPIU_DRV_TraceRamBase(uint32_t ramBase)
{
    TPIU->tpiu_base_address = ramBase;
}

/**
 *
 * Function Name : TPIU_DRV_GetRamPointer
 * Description   : Get the ram write pointer.
 *
 */
uint32_t TPIU_DRV_GetRamPointer(void)
{
    return TPIU->tpiu_ram_write_pointer;
}

/**
 *
 * Function Name : TPIU_DRV_TraceRamSize
 * Description   : The address range for tpiu data is from base_address to base_address + max_time*4
 *
 */
void TPIU_DRV_TraceRamSize(uint16_t maxTime)
{
    TPIU->tpiu_max_time = maxTime;
}

/**
 *
 * Function Name : TPIU_DRV_EnableFormatter
 * Description   : enable or disable formatter bypass mode
 *
 */
void TPIU_DRV_EnableFormatter(bool Enable)
{
    tpiu_format_ctl_type_t format;

    format.w                  = TPIU->tpiu_format_ctl;
    format.b.enable_formatter = Enable;

    TPIU->tpiu_format_ctl = format.w;
}

/**
 *
 * Function Name : TPIU_DRV_SetFormatterMode
 * Description   : configure formatter mode
 *
 */
void TPIU_DRV_SetFormatterMode(bool mode)
{
    tpiu_format_ctl_type_t format;

    format.w                 = TPIU->tpiu_format_ctl;
    format.b.continuous_mode = mode;
    TPIU->tpiu_format_ctl    = format.w;
}

/**
 *
 * Function Name : TPIU_DRV_ConfigTraceOutputRam
 * Description   : Configure memory space for traces output
 *
 */
void TPIU_DRV_ConfigTraceOutputRam(uint32_t ramBase, uint16_t ramSize)
{
    TPIU_DRV_SetFormatterMode(0);
    TPIU_DRV_TracePath(1);
    TPIU_DRV_TraceRamBase(ramBase);
    TPIU_DRV_TraceRamSize(ramSize / 4);
    TPIU_DRV_TracePortSize(32);
}

/**
 *
 * Function Name : TPIU_DRV_StartTrace
 * Description   : enable trace module
 *
 */
void TPIU_DRV_StartTrace(void)
{
    TPIU_DRV_TraceEnable(1);
    TPIU_DRV_EnableFormatter(1);
}

/**
 *
 * Function Name : TPIU_DRV_StopTrace
 * Description   : disable trace module
 *
 */
void TPIU_DRV_StopTrace(void)
{
    TPIU_DRV_EnableFormatter(0);
    TPIU_DRV_TraceEnable(0);
}

/**
 *
 * Function Name : TPIU_DRV_ConfigTraceOutputTracePort
 * Description   : configure trace mode
 *
 */
void TPIU_DRV_ConfigTraceOutputTracePort(void)
{
    TPIU_DRV_SetFormatterMode(0);
    TPIU_DRV_TracePath(0);
    TPIU_DRV_TracePortSize(4);
}
