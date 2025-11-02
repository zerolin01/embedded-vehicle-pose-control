/******************************************************************************
 * @file     kittychip.h
 * @brief    EMSIS Core Peripheral Access Layer Header File for
 *           ESWIN common evaluation SoC which support ESWIN KittyHawk cores
 * @version  V0.1.0
 * @date     4 Jan. 2021
 ******************************************************************************/
/*
 * Copyright (c) 2020-2021 ESWIN Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __E320_H__
#define __E320_H__

#ifndef __ASSEMBLY__
#include <stddef.h>
#endif

#include "EAM2011.h"

#ifndef USE_OLD_CONFIG
#include "e320_config.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup ESWIN
 * @{
 */

/** @addtogroup common
 * @{
 */

/** @addtogroup Configuration_of_EMSIS
 * @{
 */

/* =========================================================================================================================== */
/* ================                           Processor and Core Peripheral Section ================ */
/* =========================================================================================================================== */
#ifdef SOC_CONFIG_HEADER
#include SOC_CONFIG_HEADER
#endif
#ifdef BOARD_CONFIG_HEADER
#include BOARD_CONFIG_HEADER
#endif

#ifdef APP_CONFIG_HEADER
#include APP_CONFIG_HEADER
#endif

// typedef unsigned int uint32_t;
// typedef uint32_t reg_t;

/* =========================================================================================================================== */
/* ================                                  Exception Code Definition ================ */
/* =========================================================================================================================== */
#ifndef __ASSEMBLY__

#define CLIC_MODE_NMI_IRQn SOC_INT_MAX
#define CLINT_MODE_NMI_IRQn Reserved15_IRQn

typedef enum EXCn {
    /* =======================================  ESWIN KittyHawk Specific Exception Code ======================================== */
    InsUnalign_EXCn          =   0,              /**  Instruction address misaligned */
    InsAccFault_EXCn         =   1,              /**  Instruction access fault */
    IlleIns_EXCn             =   2,              /**  Illegal instruction */
    Break_EXCn               =   3,              /**  Beak point */
    LdAddrUnalign_EXCn       =   4,              /**  Load address misaligned */
    LdFault_EXCn             =   5,              /**  Load access fault */
    StAddrUnalign_EXCn       =   6,              /**  Store or AMO address misaligned */
    StAccessFault_EXCn       =   7,              /**  Store or AMO access fault */
    UmodeEcall_EXCn          =   8,              /**  Environment call from User mode */
    MmodeEcall_EXCn          =  11,              /**  Environment call from Machine mode */
} EXCn_Type;

#define SYSTEM_MAX_EXCEPTION_NUM 16

/**
 * @}
 */ /* End of group Configuration_of_EMSIS */

// #include <core_emsis.h>                        /** ESWIN Microprocessor and peripherals */
// #include "system-e320.h"                   /** common System */
#endif

/* ========================================  Start of section using anonymous unions ======================================== */
#if defined(__GNUC__)
/* anonymous unions are enabled by default */
#else
//#warning Not supported compiler type
#endif

#ifndef __ASSEMBLY__
extern uint32_t SystemCoreClock;
extern uint32_t SystemTimerClock;
#endif

/**
 * @}
 */ /* End of group common */

#if 0
struct context {
    /* general purpose registers */
/* ignore x0 */
    reg_t ra;
    reg_t sp;
    reg_t gp;
    reg_t tp;
    reg_t t0;
    reg_t t1;
    reg_t t2;
    reg_t s0;
    reg_t s1;
    reg_t a0;
    reg_t a1;
    reg_t a2;
    reg_t a3;
    reg_t a4;
    reg_t a5;
    reg_t a6;
    reg_t a7;
    reg_t s2;
    reg_t s3;
    reg_t s4;
    reg_t s5;
    reg_t s6;
    reg_t s7;
    reg_t s8;
    reg_t s9;
    reg_t s10;
    reg_t s11;
    reg_t t3;
    reg_t t4;
    reg_t t5;
    reg_t t6;

    /* floating point scratch registers */
    reg_t ft0;
    reg_t ft1;
    reg_t ft2;
    reg_t ft3;
    reg_t ft4;
    reg_t ft5;
    reg_t ft6;
    reg_t ft7;
    reg_t fs0;
    reg_t fs1;
    reg_t fa0;
    reg_t fa1;
    reg_t fa2;
    reg_t fa3;
    reg_t fa4;
    reg_t fa5;
    reg_t fa6;
    reg_t fa7;
    reg_t fs2;
    reg_t fs3;
    reg_t fs4;
    reg_t fs5;
    reg_t fs6;
    reg_t fs7;
    reg_t fs8;
    reg_t fs9;
    reg_t fs10;
    reg_t fs11;
    reg_t ft8;
    reg_t ft9;
    reg_t ft10;
    reg_t ft11;
    reg_t fcsr;         // float csr

    /* control status registers(csr) */
    reg_t mepc;
    reg_t mstatus;
    reg_t mtvec;
};
#endif


/**
 * @}
 */ /* End of group ESWIN */

#ifdef __cplusplus
}
#endif

#endif /* __E320_H__ */
