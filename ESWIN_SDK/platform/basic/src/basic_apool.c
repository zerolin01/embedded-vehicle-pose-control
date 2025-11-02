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
 * Title:        basic_apool.c file
 *
 * Version 0.0.0
 *    Initial Release
 *---------------------------------------------------------------------------
 */

#include "basic_common.h"

/* The definition of software apool structure. */
struct item {
	volatile int malloced;
	struct __lock lock;
};

extern char apool_start[];
extern char apool_end[];
struct __lock __lock___apool_mutex __attribute__((__section__(".amo"))) = {0, -1, {0}};
/* extern struct __lock __lock___apool_mutex; */

static struct __lock *apool_mutex = &__lock___apool_mutex;
static int apool_num;
static struct item *apool;

/****************************************************************************
 * Name: basic_apool_init
 *
 * Description:
 *   This function using amo section as pool for atomic lock.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
void basic_apool_init(void)
{
	int i = 0;

	apool = (struct item *) apool_start;

	apool_num = (apool_end - apool_start) / sizeof(struct item);

	for (i = 0; i < apool_num; i++) {
		apool[i].malloced = 0;
		apool[i].lock.lock = 0;
		apool[i].lock.owner = -1;
		apool[i].lock.u.cnt = 0;
	}
}

/****************************************************************************
 * Name: basic_apool_alloc
 *
 * Description:
 *   This function malloc __lock in atomic lock pool
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   The pointer of basic_lock
 *
 ****************************************************************************/
struct __lock *basic_apool_alloc(void)
{
	int i = 0;

	basic_lock_acquire(apool_mutex);
	for (i = 0; i < apool_num; i++) {
		if (apool[i].malloced == 1)
			continue;

		apool[i].malloced = 1;
		basic_lock_release(apool_mutex);
		return &(apool[i].lock);
	}

	basic_lock_release(apool_mutex);
	return NULL;
}

/****************************************************************************
 * Name: basic_apool_release
 *
 * Description:
 *   This function return the lock to atomic lock pool, the lock must be
 *   released before return to the pool.
 *
 * Input Parameters:
 *   lock- LLock pointer requiring contention.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
void basic_apool_release(struct __lock *lock)
{
	BASIC_ASSERT(lock);

	basic_lock_acquire(apool_mutex);
	struct item *item = container_of(lock, struct item, lock);

	item->malloced = 0;
	/* __lock member already initialized by lock release function */
	basic_lock_release(apool_mutex);
}
