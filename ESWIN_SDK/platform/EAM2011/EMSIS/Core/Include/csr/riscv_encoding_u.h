/*
 * Copyright (c) 2020 ESWIN Limited. All rights reserved.
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

#ifndef __RISCV_ENCODING_U_H__
#define __RISCV_ENCODING_U_H__

/**
 * CSR Defination For S Mode Configuration Macro:
 * 1. __PRIVILEGE_UMODE_SUPPORT
 */

#include "riscv_bits.h"

#ifdef __cplusplus
 extern "C" {
#endif

#ifdef CONFIG_RV_U_MODE

/**
 * \brief  User Trap Setup
 */
#define CSR_USTATUS         0x000
#define CSR_UIE             0x004
#define CSR_UTVEC           0x005

/**
 * \brief User Trap Handling
 */
#define CSR_USCRATCH        0x040
#define CSR_UEPC            0x041
#define CSR_UCAUSE          0x042
#define CSR_UTVAL           0x043
#define CSR_UIP             0x044

/**
 * \brief User Floating-Point CSRs
 */
#define CSR_FFLAGS          0x001
#define CSR_FRM             0x002
#define CSR_FCSR            0x003

/**
 * \brief User Counter/Timers CSRs
 */
#define CSR_CYCLE           0xC00
#define CSR_TIME            0xC01
#define CSR_INSTRET         0xC02
#define CSR_HPMCOUNTER3     0xC03
#define CSR_HPMCOUNTER4     0xC04
#define CSR_HPMCOUNTER5     0xC05
#define CSR_HPMCOUNTER6     0xC06
#define CSR_HPMCOUNTER7     0xC07
#define CSR_HPMCOUNTER8     0xC08
#define CSR_HPMCOUNTER9     0xC09
#define CSR_HPMCOUNTER10    0xC0A
#define CSR_HPMCOUNTER11    0xC0B
#define CSR_HPMCOUNTER12    0xC0C
#define CSR_HPMCOUNTER13    0xC0D
#define CSR_HPMCOUNTER14    0xC0E
#define CSR_HPMCOUNTER15    0xC0F
#define CSR_HPMCOUNTER16    0xC10
#define CSR_HPMCOUNTER17    0xC11
#define CSR_HPMCOUNTER18    0xC12
#define CSR_HPMCOUNTER19    0xC13
#define CSR_HPMCOUNTER20    0xC14
#define CSR_HPMCOUNTER21    0xC15
#define CSR_HPMCOUNTER22    0xC16
#define CSR_HPMCOUNTER23    0xC17
#define CSR_HPMCOUNTER24    0xC18
#define CSR_HPMCOUNTER25    0xC19
#define CSR_HPMCOUNTER26    0xC1A
#define CSR_HPMCOUNTER27    0xC1B
#define CSR_HPMCOUNTER28    0xC1C
#define CSR_HPMCOUNTER29    0xC1D
#define CSR_HPMCOUNTER30    0xC1E
#define CSR_HPMCOUNTER31    0xC1F
#define CSR_CYCLEH          0xC80
#define CSR_TIMEH           0xC81
#define CSR_INSTRETH        0xC82
#define CSR_HPMCOUNTER3H    0xC83
#define CSR_HPMCOUNTER4H    0xC84
#define CSR_HPMCOUNTER5H    0xC85
#define CSR_HPMCOUNTER6H    0xC86
#define CSR_HPMCOUNTER7H    0xC87
#define CSR_HPMCOUNTER8H    0xC88
#define CSR_HPMCOUNTER9H    0xC89
#define CSR_HPMCOUNTER10H   0xC8A
#define CSR_HPMCOUNTER11H   0xC8B
#define CSR_HPMCOUNTER12H   0xC8C
#define CSR_HPMCOUNTER13H   0xC8D
#define CSR_HPMCOUNTER14H   0xC8E
#define CSR_HPMCOUNTER15H   0xC8F
#define CSR_HPMCOUNTER16H   0xC90
#define CSR_HPMCOUNTER17H   0xC91
#define CSR_HPMCOUNTER18H   0xC92
#define CSR_HPMCOUNTER19H   0xC93
#define CSR_HPMCOUNTER20H   0xC94
#define CSR_HPMCOUNTER21H   0xC95
#define CSR_HPMCOUNTER22H   0xC96
#define CSR_HPMCOUNTER23H   0xC97
#define CSR_HPMCOUNTER24H   0xC98
#define CSR_HPMCOUNTER25H   0xC99
#define CSR_HPMCOUNTER26H   0xC9A
#define CSR_HPMCOUNTER27H   0xC9B
#define CSR_HPMCOUNTER28H   0xC9C
#define CSR_HPMCOUNTER29H   0xC9D
#define CSR_HPMCOUNTER30H   0xC9E
#define CSR_HPMCOUNTER31H   0xC9F


#endif

#ifdef __cplusplus
}
#endif
#endif /* __RISCV_ENCODING_U_H__ */
