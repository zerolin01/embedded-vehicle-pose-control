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

#ifndef __EMPS_COMPILER_H
#define __EMPS_COMPILER_H

#include <stdint.h>

/**
 * @file     emsis_compiler.h
 * @brief    EMSIS compiler generic header file
 */
#if defined ( __GNUC__ )
  /** GNU GCC Compiler */
  #include "compiler/emsis_gcc.h"
  #ifndef   __TIM_CODE
    #ifndef EMPS_LINKER_RAM
      #define __TIM_CODE  __attribute__((section(".itimcode")))
    #else
      #define __TIM_CODE
    #endif
  #endif

#elif defined (__IAR_SYSTEMS_ICC__)
  #include "compiler/emsis_iar.h"
  #define __TIM_CODE  __attribute__((section(".itimcode")))
#endif


#endif /* __EMSIS_COMPILER_H */

