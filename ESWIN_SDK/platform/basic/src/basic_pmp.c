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
 * Title:        basic_pmp.c file
 *
 * Version 0.0.0
 *    Initial Release
 *---------------------------------------------------------------------------
 */

#include "basic_common.h"

#ifdef CONFIG_BASIC_PMP
extern struct basic_platform_operations_s platform_ops;
/*--------------------------------------------------------------------------*/
/* PMP */
/****************************************************************************
 * Name: basic_pmp_init
 *
 * Description:
 *   Initialize PMP settings: close all settings and obtain PMP hardware
 *   parameters at the same time.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   0- Initialization successful.
 *   ERRNO- Error code.
 *
 ****************************************************************************/
int basic_pmp_init(void)
{
	uint32_t i;
	uint32_t pmp_num;
	rv_csr_t init_pmp_cfg = 0;

	pmp_num = platform_ops.get_pmp_counter();

	for (i = 0; i < pmp_num; i++) {
		__set_PMPxCFG(i, init_pmp_cfg);
		__set_PMPADDRx(i, (uint32_t) -1);
	}

	return 0;
}

/****************************************************************************
 * Name: basic_pmp_add
 *
 * Description:
 *   Add access control
 *
 * Input Parameters:
 *   addr- First address of address space to be restricted.
 *   num- The number of the PMPxCFg hand PMPADDRx.
 *   size- The size of the address space that needs to be limited. The size is 2^(size+3) byte.
 *         this parameters is only available when the mode is PMP_NAPOT is valid.
 *   perm- Permission setting.
 *   addr- PMP type
 *
 * Returned Value:
 *   0-16- Returns the access control restriction handle.
 *   ERROR- Error code.
 *
 ****************************************************************************/
int basic_pmp_add(void *addr, uint32_t num, uint32_t size, basic_pmp_perm_t perm, basic_pmp_type_t type)
{
	rv_csr_t pmp_addr;
	rv_csr_t pmp_perm = 0;
	rv_csr_t pmp_size = 0;
	uint32_t i;
	uint32_t pmp_num;

	BASIC_ASSERT(addr);
	BASIC_ASSERT(perm);

	pmp_num = platform_ops.get_pmp_counter();

	for (i = 0; i < size; i++)
		pmp_size |= (0x1 << i);

	if ((num > pmp_num))
		return -BASICEINVAL;

	pmp_perm = perm;

	switch (type) {
		case BASIC_PMP_OFF:
			__set_PMPxCFG(num, PMP_A_OFF);
			break;
		case BASIC_PMP_TOR:
			pmp_addr = (rv_csr_t) addr >> 2;
			__set_PMPADDRx(num, pmp_addr);
			__set_PMPxCFG(num, PMP_A_TOR | pmp_perm);
			break;
		case BASIC_PMP_NA4:
			pmp_addr = (rv_csr_t) addr >> 2;
			__set_PMPADDRx(num, pmp_addr);
			__set_PMPxCFG(num, PMP_A_NA4 | pmp_perm);
			break;
		case BASIC_PMP_NAPOT:
			pmp_addr = (rv_csr_t) addr >> 2 | pmp_size;
			__set_PMPADDRx(num, pmp_addr);
			__set_PMPxCFG(num, PMP_A_NAPOT | pmp_perm);
			break;
		default:
			return -BASICEINVAL;
	}

	return num;
}

/****************************************************************************
 * Name: basic_pmp_remove
 *
 * Description:
 *   Remove access control
 *
 * Input Parameters:
 *   id- The ID returned when adding access control.
 *
 * Returned Value:
 *   0- Access control removed successfully.
 *   ERRNO:Error code.
 *
 ****************************************************************************/
int basic_pmp_remove(int id)
{
	uint32_t pmp_num;
	rv_csr_t init_pmp_cfg = 0;

	pmp_num = platform_ops.get_pmp_counter();

	if ((id < 0) || (id > pmp_num))
		return -BASICEINVAL;

	__set_PMPxCFG(id, init_pmp_cfg);
	__set_PMPADDRx(id, (uint32_t) -1);
	return 0;
}
#endif /* CONFIG_BASIC_PMP */
