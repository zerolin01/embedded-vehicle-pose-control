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

#ifndef __CORE_EMSIS_H__
#define __CORE_EMSIS_H__


#ifdef __cplusplus
 extern "C" {
#endif

/* EMSIS version */
#include "emsis_version.h"
/* EMSIS CSR encoding */
#include "riscv_encoding.h"

#ifndef __ASSEMBLY__
/* EMSIS compiler specific defines */
#include "emsis_compiler.h"     

/* === Include Eswin Core Related Headers === */
/* Include core base feature header file */
#include "features/core_feature_base.h"

#ifndef __EMSIS_GENERIC
/* Include core clic feature header file */
#include "features/core_feature_clic.h"
/* Include core systimer feature header file */
#include "features/core_feature_timer.h"
#endif

/* Include core fpu feature header file */
#include "features/core_feature_fpu.h"
/* Include core dsp feature header file */
#include "features/core_feature_dsp.h"
/* Include core pmp feature header file */
#include "features/core_feature_pmp.h"
/* Include core cache feature header file */
#include "features/core_feature_cache.h"
/* Include core pmu feature header file */
#include "features/core_feature_pmu.h"

/* Include compatiable functions header file */
#include "core_compatiable.h"

/* Export Interfaces */
void Core_Init(void);
#endif

#ifdef __cplusplus
}
#endif
#endif /* __CORE_EMSIS_H__ */
