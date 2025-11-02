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
 * Title:        basic_hpm.c file
 *
 * Version 0.0.0
 *    Initial Release
 *---------------------------------------------------------------------------
 */

#include "basic_common.h"

#ifdef CONFIG_BASIC_HPM
extern struct basic_platform_operations_s platform_ops;

/*--------------------------------------------------------------------------*/
/* HPM */
/****************************************************************************
 * Name: basic_hpm_init
 *
 * Description:
 *   Initialize HPM settings and close all.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   0- Initialization successful.
 *   ERRNO- Error code.
 *
 ****************************************************************************/
int basic_hpm_init(void)
{
	uint32_t disable_counter = 0xFFFFFFFF;

	__RV_CSR_WRITE(CSR_MCOUNTINHIBIT, disable_counter);
	return 0;
}

/****************************************************************************
 * Name: basic_hpm_enable
 *
 * Description:
 *   Enable/disable HPM counter.
 *
 * Input Parameters:
 *   counter- Counter ID.
 *   enable- 0: turn off HPM counter. 1: Enable HPM counter.
 *
 * Returned Value:
 *   0- Operation of HPM counter succeeded.
 *   ERRNO- Error code.
 *
 ****************************************************************************/
int basic_hpm_enable(basic_hpm_counter_t counter, int enable)
{
	if ((enable < 0) || (enable > 1))
		return -BASICEINVAL;

	if ((counter < BASIC_HPM_CYCLE) || (counter > BASIC_HPM_COUNTER31))
		return -BASICEINVAL;

	if ((platform_ops.hpm_exist(counter) < 0) || (platform_ops.hpm_writeable(counter) < 0))
		return -BASICEINVAL;

	if (enable == 1) {
		__RV_CSR_CLEAR(CSR_MCOUNTINHIBIT, (1 << counter));
		if (!(__RV_CSR_READ(CSR_MCOUNTINHIBIT) & (1 << counter)))
			return 0;
		else
			return -BASICEREGRW;
	} else {
		__RV_CSR_WRITE(CSR_MCOUNTINHIBIT, (1 << counter));
		if (__RV_CSR_READ(CSR_MCOUNTINHIBIT) & (1 << counter))
			return 0;
		else
			return -BASICEREGRW;
	}
}

/****************************************************************************
 * Name: basic_hpm_get
 *
 * Description:
 *   Get HPM counter value.
 *
 * Input Parameters:
 *   counter- HPM counter ID to get.
 *
 * Returned Value:
 *   HPM counter value, 64 bit data.
 *
 ****************************************************************************/
uint64_t basic_hpm_get(basic_hpm_counter_t counter)
{
	uint32_t high0, low, high;
	uint64_t re_value;

	if ((counter < BASIC_HPM_CYCLE) || (counter > BASIC_HPM_COUNTER31))
		return -BASICEINVAL;

	if (platform_ops.hpm_exist(counter) < 0)
		return -BASICEINVAL;

	switch (counter) {
		case BASIC_HPM_CYCLE:
			high0 = __RV_CSR_READ(CSR_MCYCLEH);
			low = __RV_CSR_READ(CSR_MCYCLE);
			high = __RV_CSR_READ(CSR_MCYCLEH);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MCYCLE);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_MINSTRET:
			high0 = __RV_CSR_READ(CSR_MINSTRETH);
			low = __RV_CSR_READ(CSR_MINSTRET);
			high = __RV_CSR_READ(CSR_MINSTRETH);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MINSTRET);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER3:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER3H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER3);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER3H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER3);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER4:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER4H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER4);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER4H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER4);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER5:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER5H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER5);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER5H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER5);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER6:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER6H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER6);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER6H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER6);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER7:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER7H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER7);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER7H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER7);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER8:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER8H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER8);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER8H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER8);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER9:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER9H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER9);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER9H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER9);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER10:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER10H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER10);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER10H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER10);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER11:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER11H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER11);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER11H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER11);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER12:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER12H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER12);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER12H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER12);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER13:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER13H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER13);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER13H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER13);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER14:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER14H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER14);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER14H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER14);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER15:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER15H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER15);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER15H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER15);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER16:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER16H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER16);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER16H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER16);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER17:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER17H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER17);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER17H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER17);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER18:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER18H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER18);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER18H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER18);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER19:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER19H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER19);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER19H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER19);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER20:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER20H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER20);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER20H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER20);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER21:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER21H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER21);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER21H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER21);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER22:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER22H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER22);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER22H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER22);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER23:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER23H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER23);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER23H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER23);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER24:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER24H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER24);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER24H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER24);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER25:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER25H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER25);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER25H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER25);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER26:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER26H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER26);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER26H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER26);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER27:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER27H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER27);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER27H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER27);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER28:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER28H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER28);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER28H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER28);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER29:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER29H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER29);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER29H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER29);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER30:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER30H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER30);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER30H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER30);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		case BASIC_HPM_COUNTER31:
			high0 = __RV_CSR_READ(CSR_MHPMCOUNTER31H);
			low = __RV_CSR_READ(CSR_MHPMCOUNTER31);
			high = __RV_CSR_READ(CSR_MHPMCOUNTER31H);
			if (high0 != high)
				low = __RV_CSR_READ(CSR_MHPMCOUNTER31);

			re_value = (((uint64_t) high) << 32) | low;
			break;
		default:
			re_value = 0;
	}
	return re_value;
}

/****************************************************************************
 * Name: basic_hpm_set
 *
 * Description:
 *   Clear the HPM counter value.
 *
 * Input Parameters:
 *   counter- HPM counter ID to clear 0.
 *
 * Returned Value:
 *   0- Operation of HPM counter succeeded.
 *   ERRNO- Error code.
 *
 ****************************************************************************/
int basic_hpm_set(basic_hpm_counter_t counter)
{
	if ((counter < BASIC_HPM_CYCLE) || (counter > BASIC_HPM_COUNTER31))
		return -BASICEINVAL;

	if ((platform_ops.hpm_exist(counter) < 0) || (platform_ops.hpm_writeable(counter) < 0))
		return -BASICEINVAL;

	switch (counter) {
		case BASIC_HPM_CYCLE:
			__RV_CSR_WRITE(CSR_MCYCLE, 0);
			__RV_CSR_WRITE(CSR_MCYCLEH, 0);
			break;
		case BASIC_HPM_MINSTRET:
			__RV_CSR_WRITE(CSR_MINSTRET, 0);
			__RV_CSR_WRITE(CSR_MINSTRETH, 0);
			break;
		case BASIC_HPM_COUNTER3:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER3, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER3H, 0);
			break;
		case BASIC_HPM_COUNTER4:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER4, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER4H, 0);
			break;
		case BASIC_HPM_COUNTER5:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER5, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER5H, 0);
			break;
		case BASIC_HPM_COUNTER6:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER6, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER6H, 0);
			break;
		case BASIC_HPM_COUNTER7:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER7, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER7H, 0);
			break;
		case BASIC_HPM_COUNTER8:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER8, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER8H, 0);
			break;
		case BASIC_HPM_COUNTER9:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER9, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER9H, 0);
			break;
		case BASIC_HPM_COUNTER10:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER10, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER10H, 0);
			break;
		case BASIC_HPM_COUNTER11:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER11, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER11H, 0);
			break;
		case BASIC_HPM_COUNTER12:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER12, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER12H, 0);
			break;
		case BASIC_HPM_COUNTER13:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER13, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER13H, 0);
			break;
		case BASIC_HPM_COUNTER14:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER14, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER14H, 0);
			break;
		case BASIC_HPM_COUNTER15:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER15, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER15H, 0);
			break;
		case BASIC_HPM_COUNTER16:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER16, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER16H, 0);
			break;
		case BASIC_HPM_COUNTER17:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER17, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER17H, 0);
			break;
		case BASIC_HPM_COUNTER18:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER18, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER18H, 0);
			break;
		case BASIC_HPM_COUNTER19:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER19, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER19H, 0);
			break;
		case BASIC_HPM_COUNTER20:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER20, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER20H, 0);
			break;
		case BASIC_HPM_COUNTER21:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER21, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER21H, 0);
			break;
		case BASIC_HPM_COUNTER22:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER22, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER22H, 0);
			break;
		case BASIC_HPM_COUNTER23:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER23, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER23H, 0);
			break;
		case BASIC_HPM_COUNTER24:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER24, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER24H, 0);
			break;
		case BASIC_HPM_COUNTER25:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER25, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER25H, 0);
			break;
		case BASIC_HPM_COUNTER26:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER26, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER26H, 0);
			break;
		case BASIC_HPM_COUNTER27:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER27, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER27H, 0);
			break;
		case BASIC_HPM_COUNTER28:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER28, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER28H, 0);
			break;
		case BASIC_HPM_COUNTER29:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER29, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER29H, 0);
			break;
		case BASIC_HPM_COUNTER30:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER30, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER30H, 0);
			break;
		case BASIC_HPM_COUNTER31:
			__RV_CSR_WRITE(CSR_MHPMCOUNTER31, 0);
			__RV_CSR_WRITE(CSR_MHPMCOUNTER31H, 0);
			break;
		default:
			break;
	}

	return 0;
}
#endif /* CONFIG_BASIC_HPM */
