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

#ifndef __RISCV_ENCODING_H__
#define __RISCV_ENCODING_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* === Include RISC-V standard U/S/M Mode CSR defination === */
/* Include RISC-V standard M Mode CSR defination */
#include "csr/riscv_encoding_m.h"
/* Include RISC-V standard S Mode CSR defination */
#include "csr/riscv_encoding_s.h"
/* Include RISC-V standard U Mode CSR defination */
#include "csr/riscv_encoding_u.h"

#ifdef __cplusplus
}
#endif
#endif /* __RISCV_ENCODING_H__ */
