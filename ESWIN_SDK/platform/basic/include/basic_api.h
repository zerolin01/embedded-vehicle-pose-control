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
 * Title:        basic_api.h header file
 *
 * Version 0.0.0
 *    Initial Release
 *---------------------------------------------------------------------------
 */

#ifndef __BASIC_API_H
#define __BASIC_API_H

#include "basic_io.h"
#include "basic_struct.h"
#include "platform.h"

#define BASIC_ASSERT(x)                                                                                                \
    do {                                                                                                               \
        if (!(x)) {                                                                                                    \
            printf("ASSERT at %s() in %s, %d\n", __FUNCTION__, __FILE__, __LINE__);                                    \
            while (1)                                                                                                  \
                ;                                                                                                      \
        }                                                                                                              \
    } while (0)

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
uint32_t basic_get_hartid(void);

/*--------------------------------------------------------------------------*/
/* Interrupt */
/****************************************************************************
 * Name: basic_intr_init
 *
 * Description:
 *   Initialization interrupt subsystem, including interrupt controller and software
 *   interrupt processing subsystem.
 *   Direct interrupt processing mode is used by default, and each interrupt default
 *   processing function is the default processing function.
 *   If you need to use other interrupt processing modes, please use basic_intr_select_mode.
 *   If you need to modify the interrupt handling function, please use basic_intr_register_handler
 *   function.
 *
 *   Input Parameters:
 *   None.
 *
 * Returned Value:
 *   0- Normal initialization return;
 *   <0- Initialization failed, error code returned.
 *
 ****************************************************************************/
int basic_intr_init(void);

/****************************************************************************
 * Name: basic_intr_enable
 *
 * Description:
 *   Enable disable global interrupt.
 *
 * Input Parameters:
 *   enable- True: enable global interrupt; False: disable global interrupt.
 *
 * Returned Value:
 *   0- Global interrupt enabled successfully.
 *   <0- Failed to enable global interrupt, error code returned.
 *
 ****************************************************************************/
int basic_intr_enable(int enable);

/****************************************************************************
 * Name:  basic_intr_select_mode
 *
 * Description:
 *   Interrupt processing mode is set, and the specific SOC supports different interrupt modes.
 *
 * Input Parameters:
 *   mode- For interrupt mode, see data structure enum intr_mode.
 *
 * Returned Value:
 *   0- Set successfully.
 *   <0- Setting failed, error code returned.
 *
 ****************************************************************************/
int basic_intr_select_mode(inte_mode_t mode);

/****************************************************************************
 * Name:  basic_intr_get_mode
 *
 * Description:
 *   Acquire interrupt processing mode.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Interrupt mode.
 *
 ****************************************************************************/
int basic_intr_get_mode(void);

/****************************************************************************
 * Name: basic_intr_register_handler
 *
 * Description:
 *   Register interrupt handler for interrupt with IRQN.
 *
 * Input Parameters:
 *   irq- Interrupt number.
 *   handler- Processing function.
 *
 * Returned Value:
 *   Returns the original interrupt handler, and returns NULL
 *   to indicate that the interrupt handler has not been registered before.
 *
 ****************************************************************************/
irq_handler basic_intr_register_handler(IRQn_Type irq, irq_handler handler, register_type_t *type);

/****************************************************************************
 * Name: basic_intr_unregister_handler
 *
 * Description:
 *   If the interrupt processing function with IRQ is cancelled, the corresponding
 *   interrupt processing function will be set as the default processing function,
 *
 * Input Parameters:
 *   irq- Interrupt number.
 *
 * Returned Value:
 *   Original interrupt processing function.
 *
 ****************************************************************************/
irq_handler basic_intr_unregister_handler(IRQn_Type irq);

/****************************************************************************
 * Name: basic_intr_enable_irq
 *
 * Description:
 *   Interrupt number corresponding to enable interrupt number IRQN.
 *
 * Input Parameters:
 *   irq- Interrupt number.
 *   enable- True: enable interrupt. False: disable interrupt.
 *
 * Returned Value:
 *   0- Enable interrupt IRQN succeeded.
 *   <0- Enable interrupt IRQN failed, return error code.
 *
 ****************************************************************************/
int basic_intr_enable_irq(IRQn_Type irq, int enable);

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
void basic_lock_init(struct __lock **lock);

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
void basic_lock_close(struct __lock *lock);

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
void basic_lock_acquire(struct __lock *lock);

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
void basic_lock_acquire_recursive(struct __lock *lock);

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
int basic_lock_try_acquire(struct __lock *lock);

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
int basic_lock_try_acquire_recursive(struct __lock *lock);

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
void basic_lock_release(struct __lock *lock);

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
void basic_lock_release_recursive(struct __lock *lock);

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
void basic_apool_init(void);

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
struct __lock *basic_apool_alloc(void);

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
void basic_apool_release(struct __lock *lock);

/*--------------------------------------------------------------------------*/
/* Memory */
/****************************************************************************
 * Name: basic_pool_init
 *
 * Description:
 *   Initialize memory pool.
 *
 * Input Parameters:
 *   cp- The first address of the space occupied by the memory pool.
 *   size- The amount of space occupied by the memory pool.
 *
 * Returned Value:
 *   0- Initialization of memory pool succeeded.
 *   ERRNO- Error code.
 *
 ****************************************************************************/
int basic_pool_init(char *cp, uint32_t size);

/****************************************************************************
 * Name: basic_malloc
 *
 * Description:
 *   Apply for memory
 *
 * Input Parameters:
 *   pPool:Allocate memory from the memory pool pointed by ppool.
 *         When pPool == null, allocate memory from the default memory pool.
 *   nbytes:Required memory size (bytes).
 *
 * Returned Value:
 *   NULL- Failed to apply for memory.
 *   !NULL- If the memory application is successful,
 *          return the first memory address allocated.
 *
 ****************************************************************************/
void *basic_malloc(void *pPool, uint32_t nbytes);

/****************************************************************************
 * Name: basic_free
 *
 * Description:
 *   Free memory.
 *
 * Input Parameters:
 *   data- Allocated memory blocks to be released.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
void basic_free(void *data);
/****************************************************************************
 * Name: memcopy
 *
 * Description:
 *    memory copy.
 *
 * Input Parameters:
 *   dest- destination address.
 *   src- source address
 *   len- size of data 
 * Returned Value:
 *   dest.
 *
 ****************************************************************************/
void * memcopy(void * dest, void * src, uint32_t len);
/*--------------------------------------------------------------------------*/
/* Tick */
/****************************************************************************
 * Name: basic_get_tick
 *
 * Description:
 *   Get the number of system ticks.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   The current tick number of the system increases monotonously from power on.
 *
 ****************************************************************************/
uint64_t basic_get_tick(void);

/****************************************************************************
 * Name: basic_get_freq
 *
 * Description:
 *   Get tick count cycle.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   Number of ticks per second.
 *
 ****************************************************************************/
uint64_t basic_get_freq(void);

/****************************************************************************
 * Name: basic_set_tick
 *
 * Description:
 *   Set the number of ticks riggered by the next timer interrupt.
 *
 * Input Parameters:
 *   tick- The number of ticks to be set in timer.
 *
 * Returned Value:
 *   Number of ticks before setting.
 *
 ****************************************************************************/
uint64_t basic_set_tick(uint64_t tick);

/****************************************************************************
 * Name: basic_set_tick_interval
 *
 * Description:
 *   Set the next timer interrupt trigger time to tick.
 *
 * Input Parameters:
 *   tick- Time interval to be set.
 *
 * Returned Value:
 *   Time interval from next timer interrupt trigger.
 *
 ****************************************************************************/
uint64_t basic_set_tick_interval(uint64_t tick);

/**
 * @brief BASIC_Msec2Tick
 * @details
 * 		The millisecond is converted to tick.
 * @param
 * 		millisecond - how many millisecond need to be converted.
 * @retval
 * 		return ticks.
 * */
uint64_t BASIC_Msec2Tick(uint32_t milliseconds);

/**
 * @brief BASIC_TimerCountReset
 * @details
 * 		reset the counter to zero.
 * @retval
 * 		return void.
 * */
void BASIC_TimerCountReset(void);

/**
 * @brief BASIC_DelayS
 * @details
 * 		Wait for a specified time.The accuracy is seconds.
 * @param
 * 		seconds - how many seconds need to be wait.
 * @retval
 * 		status code that indicates the execution status of the function.
 * */

int BASIC_DelayS(uint32_t seconds);

/**
 * @brief BASIC_DelayMs
 * @details
 * 		Wait for a specified time.The accuracy is milliseconds.
 * @param
 * 		milliseconds - how many milliseconds need to be wait.
 * @retval
 * 		status code that indicates the execution status of the function.
 * */
int BASIC_DelayMs(uint32_t milliseconds);

/**
 * @brief BASIC_DelayUs
 * @details
 * 		Wait for a specified time.The accuracy is microseconds.
 * @param
 * 		microseconds - how many microseconds need to be wait.
 * @retval
 * 		status code that indicates the execution status of the function.
 * */
int BASIC_DelayUs(uint32_t microseconds);

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
basic_timer_t *basic_timer_init(void *tp, timer_callback cb, void *arg, uint32_t tick, timer_flags_t flags);

/****************************************************************************
 * Name: basic_timer_add
 *
 * Description:
 *   Add timers.
 *
 * Input Parameters:
 *   timer- Timer to add.
 *
 * Returned Value:
 *   0- Timer added successfully.
 *   ERRNO- Error codie.
 *
 ****************************************************************************/
int basic_timer_add(basic_timer_t *timer);

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
int basic_timer_enable(basic_timer_t *timer, int enable);

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
int basic_timer_delete(basic_timer_t *timer);

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
int basic_hpm_init(void);

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
int basic_hpm_enable(basic_hpm_counter_t counter, int enable);

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
uint64_t basic_hpm_get(basic_hpm_counter_t counter);

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
int basic_hpm_set(basic_hpm_counter_t counter);

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
int basic_pmp_init(void);

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
int basic_pmp_add(void *addr, uint32_t num, uint32_t size, basic_pmp_perm_t perm, basic_pmp_type_t type);

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
int basic_pmp_remove(int id);

/*--------------------------------------------------------------------------*/
/* USART */
/****************************************************************************
 * Name: basic_usart_read(uint8_t *readbuf, uint8_t num)
 *
 * Description:
 *   Get hardware hard ID.

 * Input Parameters:
 *   readbuf-The usart read buf.
 *   num-The size of buf.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
void basic_usart_read(uint8_t *readbuf, uint8_t num);

/****************************************************************************
 * Name: basic_usart_write(uint8_t *writebuf)
 *
 * Description:
 *   Get hardware hard ID.

 * Input Parameters:
 *   writebuf-The usart write buf.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/
void basic_usart_write(uint8_t *writebuf, uint8_t num);

/**
 * @brief BASIC_InvalidCache
 * @details
 * 		An invalidate operation deallocates the copy of the cache block.
 * @param
 * 		addr The address if cache block.
 * @retval
 * 		null
 * */
void BASIC_InvalidCache(void *addr);

/**
 * @brief BASIC_FlushCache
 * @details
 * 		An flush operation deallocates the copy of the cache block.
 * @param
 * 		addr The address if cache block.
 * @retval
 * 		null
 * */
void BASIC_FlushCache(void *addr);

typedef uint64_t cycles_t;

static inline cycles_t get_cycles(void)
{
	uint32_t hi, lo;
	do {
		hi = __RV_CSR_READ(CSR_MCYCLEH);
		lo = __RV_CSR_READ(CSR_MCYCLE);
	} while (hi != __RV_CSR_READ(CSR_MCYCLEH));

	return ((cycles_t)hi << 32) | lo;
}

#endif /* __BASIC_API_H  */
