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
 * Title:        basic_platform.h header file
 *
 * Version 0.0.0
 *    Initial Release
 *---------------------------------------------------------------------------
 */

#ifndef __BASIC_PLATFORM_H
#define __BASIC_PLATFORM_H

// #include "basic_common.h"

/*--------------------------------------------------------------------------*/
/* Platform functions */
struct basic_platform_operations_s {
#ifdef CONFIG_BASIC_HPM
	/* Judge whether HPM counter exists. */
	int (*hpm_exist)(basic_hpm_counter_t counter);
	/* Judge whether HPM counter is writable. */
	int (*hpm_writeable)(basic_hpm_counter_t counter);
#endif
#ifdef CONFIG_BASIC_PMP
	/* get pmp counter */
	uint32_t (*get_pmp_counter)(void);
#endif
	/* get freq. */
	uint32_t (*get_timer_freq)(void);
	/* Set the number of ticks riggered by the next timer interrupt. */
	uint64_t (*set_tick)(uint64_t tick);
	/* Set the next timer interrupt trigger time to tick. */
	uint64_t (*set_tick_interval)(uint64_t tick);
	/* Usart read. */
	void (*usart_read)(uint8_t *readbuf, uint8_t num);
	/* Usart write. */
	void (*usart_write)(uint8_t *writebuf, uint8_t num);
};
#endif /* __BASIC_PLATFORM_H  */
