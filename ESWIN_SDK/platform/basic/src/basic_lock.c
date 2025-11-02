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
 * Title:        basic_lock.c file
 *
 * Version 0.0.0
 *    Initial Release
 *---------------------------------------------------------------------------
 */

#include "basic_common.h"

#ifdef CONFIG_BASIC_LOCK
/*--------------------------------------------------------------------------*/
/* Lock */
/****************************************************************************
 * Name: basic_lock_init
 *
 * Description:
 *   Assign and initialize lock.
 *
 * Input Parameters:
 *   lock- Lock is an instruction variable, and the allocated lock
 *         will be returned to the caller through lock.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
void basic_lock_init(struct __lock **lock)
{
	if (lock == NULL)
		return;

	struct __lock *lock_tmp = basic_apool_alloc();

	if (lock_tmp != NULL) {
		lock_tmp->lock = 0;
		lock_tmp->u.cnt = 0;
		lock_tmp->owner = -1;
	}

	*lock = lock_tmp;
}

/****************************************************************************
 * Name: basic_lock_close
 *
 * Description:
 *   Release lock resource
 *
 * Input Parameters:
 *   lock- Lock pointer to be released.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
void basic_lock_close(struct __lock *lock)
{
	if (lock != NULL)
		basic_apool_release(lock);
}

/****************************************************************************
 * Name: basic_lock_acquire
 *
 * Description:
 *   Grasp the lock in a non reentrant way, and die when the lock cannot be obtained.
 *
 * Input Parameters:
 *   lock:- Lock pointer requiring contention.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
void basic_lock_acquire(struct __lock *lock)
{
	uint32_t locked = 1, mstatus;

	if (lock == NULL)
		return;

retry:
	while (lock->lock == 1)
		;
	mstatus = __RV_CSR_READ_CLEAR(CSR_MSTATUS, MSTATUS_MIE);
	locked = __AMOSWAP_W_AQ(&lock->lock, locked);
	if (locked == 1) {
		__RV_CSR_READ_SET(CSR_MSTATUS, mstatus & MSTATUS_MIE);
		goto retry;
	}

#ifdef CONFIG_LOOP_LOCK
	lock->lock = 1;
#endif
	lock->u.mstatus = mstatus;
}

/****************************************************************************
 * Name: basic_lock_acquire_recursive
 *
 * Description:
 *   Grasp the lock in the way of reentry, and die when the lock cannot be obtained.
 *
 * Input Parameters:
 *   lock- Lock pointer requiring contention.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
void basic_lock_acquire_recursive(struct __lock *lock)
{
	uint32_t locked = 1, mstatus;
	uint32_t hartid = basic_get_hartid();

	if (lock == NULL)
		return;

	mstatus = __RV_CSR_READ_CLEAR(CSR_MSTATUS, MSTATUS_MIE);
	if (lock->lock == 1 && lock->owner == hartid) {
		lock->u.cnt++;
		__RV_CSR_READ_SET(CSR_MSTATUS, mstatus & MSTATUS_MIE);
		return;
	}

	__RV_CSR_READ_SET(CSR_MSTATUS, mstatus & MSTATUS_MIE);
retry:
	while (lock->lock == 1)
		;
	mstatus = __RV_CSR_READ_CLEAR(CSR_MSTATUS, MSTATUS_MIE);
	locked = __AMOSWAP_W_AQ(&lock->lock, locked);
	if (locked == 1) {
		__RV_CSR_READ_SET(CSR_MSTATUS, mstatus & MSTATUS_MIE);
		goto retry;
	}

	// this hart get lock firstly
#ifdef CONFIG_LOOP_LOCK
	lock->lock = 1;
#endif
	lock->u.cnt = 1;
	lock->owner = hartid;
	__RV_CSR_READ_SET(CSR_MSTATUS, mstatus & MSTATUS_MIE);
}

/****************************************************************************
 * Name: basic_lock_try_acquire
 *
 * Description:
 *   Try to grasp the lock in a non reentrant way and return immediately regardless
 *   of whether the lock can be acquired or not.
 *
 * Input Parameters:
 *   lock- Lock pointer requiring contention.
 *
 * Returned Value:
 *   0- Failed to grasp the lock.
 *   1- Success in grasping the lock.
 *
 ****************************************************************************/
int basic_lock_try_acquire(struct __lock *lock)
{
	uint32_t locked = 1, mstatus;

	if (lock == NULL)
		return 0;

	if (lock->lock == 1)
		return 0;

	mstatus = __RV_CSR_READ_CLEAR(CSR_MSTATUS, MSTATUS_MIE);
	locked = __AMOSWAP_W_AQ(&lock->lock, locked);
	if (locked == 1) {
		__RV_CSR_READ_SET(CSR_MSTATUS, mstatus & MSTATUS_MIE);
		return 0;
	}

#ifdef CONFIG_LOOP_LOCK
	lock->lock = 1;
#endif
	lock->u.mstatus = mstatus;

	return 1;
}

/****************************************************************************
 * Name: basic_lock_try_acquire_recursive
 *
 * Description:
 *   Try to grab the lock in reentrant mode, and it will return immediately regardless
 *   of whether the lock is acquired or not.
 *
 * Input Parameters:
 *   lock- Lock pointer requiring contentio
 *
 * Returned Value:
 *   0- Failed to grasp the lock.
 *   1- Success in grasping the lock
 *
 ****************************************************************************/
int basic_lock_try_acquire_recursive(struct __lock *lock)
{
	uint32_t locked = 1, mstatus;
	uint32_t hartid = basic_get_hartid();

	if (lock == NULL)
		return 0;

	mstatus = __RV_CSR_READ_CLEAR(CSR_MSTATUS, MSTATUS_MIE);
	if (lock->lock == 1 && lock->owner == hartid) {
		// already taked the lock before
		lock->u.cnt++;
		__RV_CSR_READ_SET(CSR_MSTATUS, mstatus & MSTATUS_MIE);
		return 1;
	}

	locked = __AMOSWAP_W_AQ(&lock->lock, locked);
	if (locked == 1) {
		__RV_CSR_READ_SET(CSR_MSTATUS, mstatus & MSTATUS_MIE);
		return 0;
	}

	// get the lock
#ifdef CONFIG_LOOP_LOCK
	lock->lock = 1;
#endif
	lock->u.cnt = 1;
	lock->owner = hartid;
	__RV_CSR_READ_SET(CSR_MSTATUS, mstatus & MSTATUS_MIE);
	return 1;
}

/****************************************************************************
 * Name: basic_lock_release
 *
 * Description:
 *   Release the non reentrant lock.
 *
 * Input Parameters:
 *   lock- Lock pointer to be released
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
void basic_lock_release(struct __lock *lock)
{
	uint32_t locked = 0;

	if (lock == NULL)
		return;

#ifdef CONFIG_LOOP_LOCK
	lock->lock = locked;
#else
	__AMOSWAP_W_RL(&lock->lock, locked);
#endif
	__RV_CSR_READ_SET(CSR_MSTATUS, lock->u.mstatus & MSTATUS_MIE);
}

/****************************************************************************
 * Name: basic_lock_release_recursive
 *
 * Description:
 *   Release the reentrant lock.
 *
 * Input Parameters:
 *   lock- Lock pointer to be released.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
void basic_lock_release_recursive(struct __lock *lock)
{
	uint32_t locked = 0, mstatus;
	uint32_t hartid = basic_get_hartid();

	if (lock == NULL)
		return;

	mstatus = __RV_CSR_READ_CLEAR(CSR_MSTATUS, MSTATUS_MIE);
	if (lock->lock == 0 || lock->owner != hartid) {
		// not locked or the owner is not local hart
		__RV_CSR_READ_SET(CSR_MSTATUS, mstatus & MSTATUS_MIE);
		return;
	}

	// now we are sure we take the lock before, try to release
	lock->u.cnt--;
	if (lock->u.cnt <= 0) {
		// release the lock
		lock->u.cnt = 0;
		lock->owner = -1;
#ifdef CONFIG_LOOP_LOCK
		lock->lock = locked;
#else
		__AMOSWAP_W_RL(&lock->lock, locked);
#endif
	}
	__RV_CSR_READ_SET(CSR_MSTATUS, mstatus & MSTATUS_MIE);
}
#endif /* CONFIG_BASIC_LOCK */
