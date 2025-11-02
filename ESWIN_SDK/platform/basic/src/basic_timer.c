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
 * Title:        basic_timer.c file
 *
 * Version 0.0.0
 *    Initial Release
 *---------------------------------------------------------------------------
 */

#include "basic_common.h"

#ifdef CONFIG_BASIC_TIMER
static struct __lock __lock___timer_mutex __attribute__((__section__(".amo"))) = {0, -1, {0}};
static basic_timer_t g_timer_enable_head = {NULL, NULL, NULL, 0, 0};
static basic_timer_t g_timer_disable_head = {NULL, NULL, NULL, 0, 0};

static void timer_enable_list_insert(basic_timer_t *timer)
{
	BASIC_ASSERT(timer);

	basic_timer_t *p;
	basic_timer_t *q;

	basic_lock_acquire(&__lock___timer_mutex);
	q = &g_timer_enable_head;
	p = q->tnext;

	while ((p != NULL) && (timer->expired > p->expired)) {
		q = p;
		p = p->tnext;
	}

	timer->tnext = p;
	q->tnext = timer;
	basic_lock_release(&__lock___timer_mutex);
}

static void timer_disable_list_insert(basic_timer_t *timer)
{
	BASIC_ASSERT(timer);

	basic_timer_t *p;
	basic_timer_t *q;

	basic_lock_acquire(&__lock___timer_mutex);
	q = &g_timer_disable_head;
	p = q->tnext;
	q->tnext = timer;
	timer->tnext = p;
	basic_lock_release(&__lock___timer_mutex);
}

static void timer_list_del(basic_timer_t *head, basic_timer_t *timer)
{
	BASIC_ASSERT(head);
	BASIC_ASSERT(timer);

	basic_timer_t *p;
	basic_timer_t *q;

	/* delete from enable list */
	q = head;
	p = q->tnext;

	while ((p != NULL) && (p != timer)) {
		q = p;
		p = p->tnext;
	}

	if (p != NULL) {
		p = p->tnext;
		q->tnext = p;
	} else {
		return;
	}
}

static int timer_list_find(basic_timer_t *head, basic_timer_t *timer)
{
	BASIC_ASSERT(head);
	BASIC_ASSERT(timer);

	basic_timer_t *p;

	p = head->tnext;
	while ((p != NULL) && (p != timer))
		p = p->tnext;

	if (p == timer)
		return 1;
	else
		return 0;
}

static void basic_timer_interrupt_handler(void *data)
{
	basic_timer_t *q;
	timer_callback timern_fun = NULL;
	void *timern_arg = NULL;

	q = (&g_timer_enable_head)->tnext;
	if (q != NULL) {
		if (q->flags == FLAG_ONE_SHOT) {
			if ((&g_timer_enable_head)->tnext->tnext == NULL)
				basic_intr_unregister_handler(Timer_IRQn);
			timer_list_del(&g_timer_enable_head, q);
			timer_disable_list_insert(q);
		} else {
			timer_list_del(&g_timer_enable_head, q);
			q->expired = basic_get_tick() + q->tick;
			timer_enable_list_insert(q);
		}
		timern_fun = q->cb;
		timern_arg = q->arg;

		q = (&g_timer_enable_head)->tnext;
		if (q != NULL)
			basic_set_tick(q->expired);
	}

	if (timern_fun != NULL)
		(timern_fun)(timern_arg);
	else
		return;
}

static void timer_add_readd(basic_timer_t *timer)
{
	uint32_t get_value;

	get_value = SysTimer_GetCompareValue();

	if ((&g_timer_enable_head)->tnext == NULL) {
		/* initialize software interrupt as vector interrupt */
		register_type_t mtType;

		mtType.trig_mode = CLIC_LEVEL_TRIGGER;
		mtType.lvl = 1;
		mtType.priority = 0;
		mtType.data_ptr = NULL;
		basic_intr_register_handler(Timer_IRQn, basic_timer_interrupt_handler, &mtType);

		basic_set_tick(timer->expired = basic_get_tick() + timer->tick);
		timer_enable_list_insert(timer);
	} else {
		timer->expired = basic_get_tick() + timer->tick;
		if (timer->expired < get_value) {
			basic_set_tick(timer->expired = basic_get_tick() + timer->tick);
			timer_enable_list_insert(timer);
		} else {
			timer->expired = basic_get_tick() + timer->tick;
			timer_enable_list_insert(timer);
		}
	}
}

/*--------------------------------------------------------------------------*/
/* Timer */
/****************************************************************************
 * Name: basic_timer_init
 *
 * Description:
 *   Initialize timer
 *
 * Input Parameters:
 *   tp- The first address of the memory block allocated for the timer structure.
 *   cb- Timer callback function.
 *   arg- Timer callback function parameters.
 *   tick - The time interval that this timer triggers.
 *   flags - Timer type.
 *
 * Returned Value:
 *   Generated timer structure.
 *
 ****************************************************************************/
basic_timer_t *basic_timer_init(void *tp, timer_callback cb, void *arg, uint32_t tick, timer_flags_t flags)
{
	BASIC_ASSERT(tp);
	BASIC_ASSERT(cb);
	BASIC_ASSERT(tick);

	uint32_t set_value;

	if (flags < 0 || flags > 1)
		return NULL;

	set_value = (SystemTimerClock / 1000) * tick;

	basic_timer_t *pTimer = (basic_timer_t *) tp;

	pTimer->tnext = NULL;
	pTimer->arg = arg;
	pTimer->cb = cb;
	pTimer->tick = set_value;
	pTimer->flags = flags;

	return pTimer;
}

/****************************************************************************
 * Name: basic_timer_add
 *
 * Description:
 *   Add timer
 *
 * Input Parameters:
 *   timer- Timer to be operated.
 *   enable- 0: disable timer, 1: Enable timer.
 *
 * Returned Value:
 *   0- Enable timer.
 *   ERRNO- Error code.
 *
 ****************************************************************************/
int basic_timer_add(basic_timer_t *timer)
{
	BASIC_ASSERT(timer);

	int re_value;

	/* If it already exists in disable-list, return.*/
	re_value = timer_list_find(&g_timer_disable_head, timer);
	if (re_value == 1)
		return -BASICTIMER;

	/*
	 * If it does not exist, it is added to the enable-list.
	 * If it already exists in disable-list, return.
	 */
	re_value = timer_list_find(&g_timer_enable_head, timer);
	if (re_value == 0)
		timer_add_readd(timer);
	else
		return -BASICTIMER;

	return 0;
}

/****************************************************************************
 * Name: basic_timer_enable
 *
 * Description:
 *   Enable timer
 *
 * Input Parameters:
 *   timer- Timer to be operated.
 *   enable- 0: disable timer, 1: Enable timer.
 *
 * Returned Value:
 *   0- Enable timer.
 *   ERRNO- Error code.
 *
 ****************************************************************************/
int basic_timer_enable(basic_timer_t *timer, int enable)
{
	BASIC_ASSERT(timer);

	int re_value;

	if (enable < 0 && enable > 1)
		return -BASICEINIT;

	if (enable == 1) {
		/* If it does not exist in disable-list, return.*/
		re_value = timer_list_find(&g_timer_disable_head, timer);
		if (re_value == 0)
			return -BASICTIMER;

		/* delete from disable timer list */
		timer_list_del(&g_timer_disable_head, timer);

		/* add to enable timer list */
		re_value = timer_list_find(&g_timer_enable_head, timer);
		if (re_value == 0)
			timer_add_readd(timer);
		else
			return -BASICTIMER;

	} else {
		/* If it does not exist in enable-list, return.*/
		re_value = timer_list_find(&g_timer_enable_head, timer);
		if (re_value == 0)
			return -BASICTIMER;

		if (timer == (&g_timer_enable_head)->tnext) {
			if ((&g_timer_enable_head)->tnext->tnext != NULL)
				basic_set_tick((&g_timer_enable_head)->tnext->tnext->expired);
			else
				basic_intr_unregister_handler(Timer_IRQn);
		}

		/* delete from enable timer list */
		timer_list_del(&g_timer_enable_head, timer);

		/* add to disable timer list */
		re_value = timer_list_find(&g_timer_disable_head, timer);
		if (re_value == 0) {
			timer->expired = 0;
			timer_disable_list_insert(timer);
		} else {
			return -BASICTIMER;
		}
	}
	return 0;
}

/****************************************************************************
 * Name: basic_timer_delete
 *
 * Description:
 *   Delete timer.
 *
 * Input Parameters:
 *   timer- Timer to delete.
 *
 * Returned Value:
 *   0- Deleted successfully.
 *   ERRNO:Error code.
 *
 ****************************************************************************/
int basic_timer_delete(basic_timer_t *timer)
{
	BASIC_ASSERT(timer);

	if (timer == (&g_timer_enable_head)->tnext) {
		if ((&g_timer_enable_head)->tnext->tnext != NULL)
			basic_set_tick((&g_timer_enable_head)->tnext->tnext->expired);
		else
			/* CLIC_DisableIRQ(Timer_IRQn); */
			basic_intr_unregister_handler(Timer_IRQn);

	}

	/* delete from enable timer list */
	timer_list_del(&g_timer_enable_head, timer);
	/*TODO free memory */

	/* delete from disable timer list */
	timer_list_del(&g_timer_disable_head, timer);
	/*TODO free memory */

	return 0;
}
#endif /* CONFIG_BASIC_TIMER */
