/*
 * Copyright (c) 2013-2020 Arm Limited. All rights reserved.
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
 *
 * ----------------------------------------------------------------------
 *
 * $Date:        19. July 2021
 * $Revision:    V0.0.0
 *
 * Project:      EMSIS-BASIC API
 * Title:        basic_base.c file
 *
 * Version 0.0.0
 *    Initial Release
 *---------------------------------------------------------------------------
 */

#include "basic_common.h"

/*--------------------------------------------------------------------------*/
/* Base */
/****************************************************************************
 * Name: basic_get_hartid
 *
 * Description:
 *   Get hardware hard ID.

 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   Hardware hard ID.
 *
 ****************************************************************************/
uint32_t basic_get_hartid(void)
{
	return (uint32_t) __RV_CSR_READ(CSR_MHARTID);
}
