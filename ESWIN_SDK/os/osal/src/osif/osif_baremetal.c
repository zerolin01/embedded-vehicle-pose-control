/**
 * Copyright Statement:
 * This software and related documentation (ESWIN SOFTWARE) are protected under relevant copyright laws.
 * The information contained herein is confidential and proprietary to
 * Beijing ESWIN Computing Technology Co., Ltd.(ESWIN)and/or its licensors.
 * Without the prior written permission of ESWIN and/or its licensors, any reproduction, modification,
 * use or disclosure Software, and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * Copyright ©[2023] [Beijing ESWIN Computing Technology Co., Ltd.]. All rights reserved.
 *
 * RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES THAT THE SOFTWARE
 * AND ITS DOCUMENTATIONS (ESWIN SOFTWARE) RECEIVED FROM ESWIN AND / OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. ESWIN EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON INFRINGEMENT.
 * NEITHER DOES ESWIN PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY
 * WHICH MAY BE USED BY,INCORPORATED IN, OR SUPPLIED WITH THE ESWIN SOFTWARE,
 * AND RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ESWIN BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @file osif_baremetal.c
 * @brief emps osal
 * @author abu (abu@eswincomputing.com)
 * @date 2023-03-24
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#include "osal.h"

#include "../../../../log/include/log.h"
#include "platform.h"

OS_BM_Sema_t g_sema_t[SEMA_CNT] = {0};
/*  ==== Kernel Management Functions ==== */

/* Initialize the RTOS Kernel.*/
/* \return status code that indicates the execution status of the function.
 *OS_Status_t OS_KernelInitialize(void);
 */

/*  Get RTOS Kernel Information.
 * \param[out]    version       pointer to buffer for retrieving version information.
 * \param[out]    id_buf        pointer to buffer for retrieving kernel identification string.
 * \param[in]     id_size       size of buffer for kernel identification string.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_KernelGetInfo(OS_Version_t *version, char *id_buf, uint32_t id_size)
{
    return OS_OK;
}

/* Get the current RTOS Kernel state.
 * \return current RTOS Kernel state.
 */
OS_KernelState_t OS_KernelGetState(void)
{
    return OS_KernelInactive;
}

/* Start the RTOS Kernel scheduler.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_KernelStart(void)
{
    return OS_OK;
}

/* SHIYANGYANG
 * Stop the RTOS Kernel scheduler.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_KernelStop(void)
{
    return OS_OK;
}

/* Lock the RTOS Kernel scheduler.
 * \return previous lock state (1 - locked, 0 - not locked, error code if negative).
 */
int32_t OS_KernelLock(void)
{
    return 0;
}

/* Unlock the RTOS Kernel scheduler.
 * \return previous lock state (1 - locked, 0 - not locked, error code if negative).
 */
int32_t OS_KernelUnlock(void)
{
    return 0;
}

/* Restore the RTOS Kernel scheduler lock state.
 * \param[in]     lock          lock state obtained by \ref OS_KernelLock or \ref OS_KernelUnlock.
 * \return new lock state (1 - locked, 0 - not locked, error code if negative).
 */
int32_t OS_KernelRestoreLock(int32_t lock)
{
    return 0;
}

/* Suspend the RTOS Kernel scheduler.
 * \return time in ticks, for how long the system can sleep or power-down.
 */
uint32_t OS_KernelSuspend(void)
{
    return 0;
}

/* Resume the RTOS Kernel scheduler.
 * \param[in]     sleep_ticks   time in ticks for how long the system was in sleep or power-down mode.
 */
void OS_KernelResume(uint32_t sleep_ticks)
{
    printf("this null function\n");
}

/* Get the RTOS kernel tick count.
 * \return RTOS kernel current tick count.
 */
uint32_t OS_KernelGetTickCount(void)
{
    return basic_get_tick();
}

/* Get the RTOS kernel tick frequency.
 * \return frequency of the kernel tick in hertz, i.e. kernel ticks per second.
 */
uint32_t OS_KernelGetTickFreq(void)
{
    return basic_get_freq();
}

/* Get the RTOS kernel system timer count.
 * \return RTOS kernel current system timer count as 32-bit value.
 */
uint32_t OS_KernelGetSysTimerCount(void)
{
    return 0;
}

/* Get the RTOS kernel system timer frequency.
 * \return frequency of the system timer in hertz, i.e. timer ticks per second.
 */
uint32_t OS_KernelGetSysTimerFreq(void)
{
    return 0;
}

/*  ==== Thread Management Functions ==== */

/* Create a thread and add it to Active Threads.
 * \param[in]     func          thread function.
 * \param[in]     argument      pointer that is passed to the thread function as start argument.
 * \param[in]     attr          thread attributes; NULL: default values.
 * \return thread ID for reference by other functions or NULL in case of error.
 */
OS_ThreadId_t OS_ThreadNew(OS_ThreadFunc_t func, void *argument, const OS_ThreadAttr_t *attr)
{
    return (OS_ThreadId_t)0;
}

/* Get name of a thread.
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \return name as null-terminated string.
 */
const char *OS_ThreadGetName(OS_ThreadId_t thread_id)
{
    return NULL;
}

/* Return the thread ID of the current running thread.
 * \return thread ID for reference by other functions or NULL in case of error.
 */
OS_ThreadId_t OS_ThreadGetId(void)
{
    return (OS_ThreadId_t)0;
}

/* Get current thread state of a thread.
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \return current thread state of the specified thread.
 */
OS_ThreadState_t OS_ThreadGetState(OS_ThreadId_t thread_id)
{
    return OS_ThreadInactive;
}

/* Get stack size of a thread.
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \return stack size in bytes.
 */
uint32_t OS_ThreadGetStackSize(OS_ThreadId_t thread_id)
{
    return 0;
}

/* Get available stack space of a thread based on stack watermark recording during execution.
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \return remaining stack space in bytes.
 */
uint32_t OS_ThreadGetStackSpace(OS_ThreadId_t thread_id)
{
    return 0;
}

/* Change priority of a thread.
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \param[in]     priority      new priority value for the thread function.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_ThreadSetPriority(OS_ThreadId_t thread_id, OS_Priority_t priority)
{
    return OS_OK;
}

/* Get current priority of a thread.
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \return current priority value of the specified thread.
 */
OS_Priority_t OS_ThreadGetPriority(OS_ThreadId_t thread_id)
{
    return 0;
}

/* Pass control to next thread that is in state \b READY.
 * \param[in]     core_id     code_id obtained by \ref __get_hartid.It's useful when it's multicore.
 * \return status code that indicates the execution status of the function.
 */

/* OS_Status_t OS_ThreadYield (void); */
OS_Status_t OS_ThreadYield(uint32_t core_id)
{
    return OS_OK;
}

/* Suspend execution of a thread.
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_ThreadSuspend(OS_ThreadId_t thread_id)
{
    return OS_OK;
}

/* Resume execution of a thread.
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_ThreadResume(OS_ThreadId_t thread_id)
{
    return OS_OK;
}

/* Detach a thread (thread storage can be reclaimed when thread terminates).
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_ThreadDetach(OS_ThreadId_t thread_id)
{
    return OS_OK;
}

/* Wait for specified thread to terminate.
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_ThreadJoin(OS_ThreadId_t thread_id)
{
    return OS_OK;
}

/* Terminate execution of current running thread.*/
/* __NO_RETURN void OS_ThreadExit (void); */
void OS_ThreadExit(void)
{
    printf("this null function\n");
}

/* Terminate execution of a thread.
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_ThreadTerminate(OS_ThreadId_t thread_id)
{
    return OS_OK;
}

/* Get number of active threads.
 * \return number of active threads.
 */
uint32_t OS_ThreadGetCount(void)
{
    return 0;
}

/* Enumerate active threads.
 * \param[out]    thread_array  pointer to array for retrieving thread IDs.
 * \param[in]     array_items   maximum number of items in array for retrieving thread IDs.
 * \return number of enumerated threads.
 */
uint32_t OS_ThreadEnumerate(OS_ThreadId_t *thread_array, uint32_t array_items)
{
    return 0;
}

/* SHIYANGYANG
 * Delete a thread (thread storage can be reclaimed when thread terminates).
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_ThreadDelete(OS_ThreadId_t thread_id)
{
    return OS_OK;
}

/* SHIYANGYANG
 * Start a thread (Only RTThread uses it).
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_ThreadStartUp(OS_ThreadId_t thread_id)
{
    return OS_OK;
}

/*  ==== Thread Flags Functions ==== */

/* Set the specified Thread Flags of a thread.
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \param[in]     flags         specifies the flags of the thread that sbasicl be set.
 * \return thread flags after setting or error code if highest bit set.
 */
uint32_t OS_ThreadFlagsSet(OS_ThreadId_t thread_id, uint32_t flags)
{
    return 0;
}

/* Clear the specified Thread Flags of current running thread.
 * \param[in]     flags         specifies the flags of the thread that sbasicl be cleared.
 * \return thread flags before clearing or error code if highest bit set.
 */
uint32_t OS_ThreadFlagsClear(uint32_t flags)
{
    return 0;
}

/* Get the current Thread Flags of current running thread.
 * \return current thread flags.
 */
uint32_t OS_ThreadFlagsGet(void)
{
    return 0;
}

/* Wait for one or more Thread Flags of the current running thread to become signaled.
 * \param[in]     flags         specifies the flags to wait for.
 * \param[in]     options       specifies flags options (OS_FlagsXxxx).
 * \param[in]     timeout       \ref EMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
 * \return thread flags before clearing or error code if highest bit set.
 */
/* uint32_t OS_ThreadFlagsWait (uint32_t flags, uint32_t options, uint32_t timeout); */
uint32_t OS_ThreadFlagsWait(OS_ThreadId_t thread_id, uint32_t flags, uint32_t options, uint32_t timeout)
{
    return 0;
}

/*  ==== Generic Wait Functions ==== */

/* Wait for Timeout (Time Delay).
 * \param[in]     ticks         \ref EMSIS_RTOS_TimeOutValue "time ticks" value
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_DelayTicks(uint32_t ticks)
{
    (void)basic_set_tick(ticks);
    return OS_OK;
}

/* Wait until specified time.
 * \param[in]     ticks         absolute time in ticks
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_DelayTicksUntil(uint32_t ticks)
{
    (void)basic_set_tick_interval(ticks);
    return OS_OK;
}

/*  ==== Timer Management Functions ====  */

/* Create and Initialize a timer.
 * \param[in]     func          function pointer to callback function.
 * \param[in]     type          \ref OS_TimerOnce for one-shot or \ref OS_TimerPeriodic for periodic behavior.
 * \param[in]     argument      argument to the timer callback function.
 * \param[in]     attr          timer attributes; NULL: default values.
 * \return timer ID for reference by other functions or NULL in case of error.
 */
OS_TimerId_t OS_TimerNew(OS_TimerFunc_t func, OS_TimerType_t type, void *argument, const OS_TimerAttr_t *attr)
{
    return 0;
}

/* Get name of a timer.
 * \param[in]     timer_id      timer ID obtained by \ref OS_TimerNew.
 * \return name as null-terminated string.
 */
const char *OS_TimerGetName(OS_TimerId_t timer_id)
{
    return 0;
}

/* Start or restart a timer.
 * \param[in]     timer_id      timer ID obtained by \ref OS_TimerNew.
 * \param[in]     ticks         \ref EMSIS_RTOS_TimeOutValue "time ticks" value of the timer.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_TimerStart(OS_TimerId_t timer_id, uint32_t ticks)
{
    return OS_OK;
}

/* Stop a timer.
 * \param[in]     timer_id      timer ID obtained by \ref OS_TimerNew.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_TimerStop(OS_TimerId_t timer_id)
{
    return OS_OK;
}

/* Check if a timer is running.
 * \param[in]     timer_id      timer ID obtained by \ref OS_TimerNew.
 * \return 0 not running, 1 running.
 */
uint32_t OS_TimerIsRunning(OS_TimerId_t timer_id)
{
    return 0;
}

/* Delete a timer.
 * \param[in]     timer_id      timer ID obtained by \ref OS_TimerNew.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_TimerDelete(OS_TimerId_t timer_id)
{
    return OS_OK;
}

/*  ==== Event Flags Management Functions ==== */

/* Create and Initialize an Event Flags object.
 * \param[in]     attr          event flags attributes; NULL: default values.
 * \return event flags ID for reference by other functions or NULL in case of error.
 */
OS_EventFlagsId_t OS_EventFlagsNew(const OS_EventFlagsAttr_t *attr)
{
    return 0;
}

/* Get name of an Event Flags object.
 * \param[in]     ef_id         event flags ID obtained by \ref OS_EventFlagsNew.
 * \return name as null-terminated string.
 */
const char *OS_EventFlagsGetName(OS_EventFlagsId_t ef_id)
{
    return 0;
}

/* Set the specified Event Flags.
 * \param[in]     ef_id         event flags ID obtained by \ref OS_EventFlagsNew.
 * \param[in]     flags         specifies the flags that sbasicl be set.
 * \return event flags after setting or error code if highest bit set.
 */
uint32_t OS_EventFlagsSet(OS_EventFlagsId_t ef_id, uint32_t flags)
{
    return 0;
}

/* Clear the specified Event Flags.
 * \param[in]     ef_id         event flags ID obtained by \ref OS_EventFlagsNew.
 * \param[in]     flags         specifies the flags that sbasicl be cleared.
 * \return event flags before clearing or error code if highest bit set.
 */
uint32_t OS_EventFlagsClear(OS_EventFlagsId_t ef_id, uint32_t flags)
{
    return 0;
}

/* Get the current Event Flags.
 * \param[in]     ef_id         event flags ID obtained by \ref OS_EventFlagsNew.
 * \return current event flags.
 */
uint32_t OS_EventFlagsGet(OS_EventFlagsId_t ef_id)
{
    return 0;
}

/* Wait for one or more Event Flags to become signaled.
 * \param[in]     ef_id         event flags ID obtained by \ref OS_EventFlagsNew.
 * \param[in]     flags         specifies the flags to wait for.
 * \param[in]     options       specifies flags options (OS_FlagsXxxx).
 * \param[in]     timeout       \ref EMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
 * \return event flags before clearing or error code if highest bit set.
 */
uint32_t OS_EventFlagsWait(OS_EventFlagsId_t ef_id, uint32_t flags, uint32_t options, uint32_t timeout)
{
    return 0;
}

/* Delete an Event Flags object.
 * \param[in]     ef_id         event flags ID obtained by \ref OS_EventFlagsNew.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_EventFlagsDelete(OS_EventFlagsId_t ef_id)
{
    return OS_OK;
}

/*  ==== Mutex Management Functions ==== */

/* Create and Initialize a Mutex object.
 * \param[in]     attr          mutex attributes; NULL: default values.
 * \return mutex ID for reference by other functions or NULL in case of error.
 */
OS_MutexId_t OS_MutexNew(const OS_MutexAttr_t *attr)
{
    (void)attr;
    return (OS_MutexId_t)0;
}

/* Get name of a Mutex object.
 * \param[in]     mutex_id      mutex ID obtained by \ref OS_MutexNew.
 * \return name as null-terminated string.
 */
const char *OS_MutexGetName(OS_MutexId_t mutex_id)
{
    (void)mutex_id;
    return NULL;
}

/* Acquire a Mutex or timeout if it is locked.
 * \param[in]     mutex_id      mutex ID obtained by \ref OS_MutexNew.
 * \param[in]     timeout       \ref EMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_MutexAcquire(OS_MutexId_t mutex_id, uint32_t timeout)
{
    (void)mutex_id;
    (void)timeout;
    return OS_OK;
}

/* Release a Mutex that was acquired by \ref OS_MutexAcquire.
 * \param[in]     mutex_id      mutex ID obtained by \ref OS_MutexNew.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_MutexRelease(OS_MutexId_t mutex_id)
{
    (void)mutex_id;
    return OS_OK;
}

/* Get Thread which owns a Mutex object.
 * \param[in]     mutex_id      mutex ID obtained by \ref OS_MutexNew.
 * \return thread ID of owner thread or NULL when mutex was not acquired.
 */
OS_ThreadId_t OS_MutexGetOwner(OS_MutexId_t mutex_id)
{
    (void)mutex_id;
    return (OS_ThreadId_t)0;
}

/* Delete a Mutex object.
 * \param[in]     mutex_id      mutex ID obtained by \ref OS_MutexNew.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_MutexDelete(OS_MutexId_t mutex_id)
{
    (void)mutex_id;
    return OS_OK;
}

/*  ==== Semaphore Management Functions ====  */

/* Create and Initialize a Semaphore object.
 * \param[in]     max_count     maximum number of available tokens.
 * \param[in]     initial_count initial number of available tokens.
 * \param[in]     attr          semaphore attributes; NULL: default values.
 * \return semaphore ID for reference by other functions or NULL in case of error.
 */
OS_SemaphoreId_t OS_SemaphoreNew(uint32_t max_count, uint32_t initial_count, const OS_SemaphoreAttr_t *attr)
{
    OS_BM_Sema_t *sema = NULL;
    int i;

    for (i = 0; i < SEMA_CNT; i++) {
        if (g_sema_t[i].used == false) {
            sema = &g_sema_t[i];
            break;
        }
    }

    OS_DisableInterrupt();
    sema->max_count = max_count;
    sema->count     = initial_count;
    sema->attr      = (void *)attr;
    sema->used      = true;
    OS_EnableInterrupt();

    return (void *)sema;
}

/* Get name of a Semaphore object.
 * \param[in]     semaphore_id  semaphore ID obtained by \ref OS_SemaphoreNew.
 * \return name as null-terminated string.
 */
const char *OS_SemaphoreGetName(OS_SemaphoreId_t semaphore_id)
{
    OS_BM_Sema_t *sema       = (OS_BM_Sema_t *)semaphore_id;
    OS_SemaphoreAttr_t *attr = (OS_SemaphoreAttr_t *)sema->attr;
    const char *name         = NULL;

    OS_DisableInterrupt();
    name = attr->name;
    OS_DisableInterrupt();

    return name;
}

/* Acquire a Semaphore token or timeout if no tokens are available.
 * \param[in]     semaphore_id  semaphore ID obtained by \ref OS_SemaphoreNew.
 * \param[in]     timeout       \ref EMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_SemaphoreAcquire(OS_SemaphoreId_t semaphore_id, uint32_t timeout)
{
    OS_ASSERT(semaphore_id != NULL);
    OS_BM_Sema_t *sema = (OS_BM_Sema_t *)semaphore_id;
    OS_Status_t ret    = OS_OK;

    if ((timeout == 0U)) {
        /* when the timeout is 0 the wait operation is the equivalent of try_wait,
            meaning that if the semaphore is 0 return immediately with an error code
        */
        if (sema->count == 0U) {
            ret = OS_Error;
        }
    } else {
        /* timeout is not 0 */
        uint32_t timeoutTicks;
        if (timeout == OS_WaitForever) {
            timeoutTicks = OS_WaitForever;
        } else {
            /* Convert timeout from milliseconds to ticks. */
            timeoutTicks = OS_Msec2Tick(timeout);
        }
        uint32_t start = OS_KernelGetTickCount();
        uint32_t end   = (uint32_t)(start + timeoutTicks);
        uint32_t max   = end - start;
        while (sema->count == 0U) {
            uint32_t crt_ticks = OS_KernelGetTickCount();
            uint32_t delta     = crt_ticks - start;
            if ((timeoutTicks != OS_WaitForever) && (delta > max)) {

                /* Timeout occured, stop waiting and return fail code */
                ret = OS_ErrorTimeout;
                break;
            }
        }
    }

    if (ret == OS_OK) {
        OS_DisableInterrupt();
        --(sema->count);
        OS_EnableInterrupt();
    }

    return ret;
}

/* Release a Semaphore token up to the initial maximum count.
 * \param[in]     semaphore_id  semaphore ID obtained by \ref OS_SemaphoreNew.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_SemaphoreRelease(OS_SemaphoreId_t semaphore_id)
{
    OS_ASSERT(semaphore_id != NULL);

    OS_BM_Sema_t *sema = (OS_BM_Sema_t *)semaphore_id;
    OS_Status_t ret    = OS_OK;

    OS_DisableInterrupt();
    if (sema->count != sema->max_count) {
        ++(sema->count);
    } else {
        ret = OS_Error;
    }

    OS_DisableInterrupt();

    return ret;
}

/* Get current Semaphore token count.
 * \param[in]     semaphore_id  semaphore ID obtained by \ref OS_SemaphoreNew.
 * \return number of tokens available.
 */
uint32_t OS_SemaphoreGetCount(OS_SemaphoreId_t semaphore_id)
{
    OS_ASSERT(semaphore_id != NULL);

    OS_BM_Sema_t *sema = (OS_BM_Sema_t *)semaphore_id;
    uint32_t count     = 0;

    OS_DisableInterrupt();
    count = sema->count;
    OS_DisableInterrupt();

    return count;
}

/* Delete a Semaphore object.
 * \param[in]     semaphore_id  semaphore ID obtained by \ref OS_SemaphoreNew.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_SemaphoreDelete(OS_SemaphoreId_t semaphore_id)
{
    OS_ASSERT(semaphore_id != NULL);

    OS_BM_Sema_t *sema = (OS_BM_Sema_t *)semaphore_id;

    sema->used = false;

    return OS_OK;
}

/*  ==== Memory Pool Management Functions ====  */

/* Create and Initialize a Memory Pool object.
 * \param[in]     block_count   maximum number of memory blocks in memory pool.
 * \param[in]     block_size    memory block size in bytes.
 * \param[in]     attr          memory pool attributes; NULL: default values.
 * \return memory pool ID for reference by other functions or NULL in case of error.
 */
OS_MemoryPoolId_t OS_MemoryPoolNew(uint32_t block_count, uint32_t block_size, const OS_MemoryPoolAttr_t *attr)
{
    return (OS_MemoryPoolId_t)0;
}

/* Get name of a Memory Pool object.
 * \param[in]     mp_id         memory pool ID obtained by \ref OS_MemoryPoolNew.
 * \return name as null-terminated string.
 */
const char *OS_MemoryPoolGetName(OS_MemoryPoolId_t mp_id)
{
    return NULL;
}

/* Allocate a memory block from a Memory Pool.
 * \param[in]     mp_id         memory pool ID obtained by \ref OS_MemoryPoolNew.
 * \param[in]     timeout       \ref EMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
 * \return address of the allocated memory block or NULL in case of no memory is available.
 */
void *OS_MemoryPoolAlloc(OS_MemoryPoolId_t mp_id, uint32_t timeout)
{
    return NULL;
}

/* Return an allocated memory block back to a Memory Pool.
 * \param[in]     mp_id         memory pool ID obtained by \ref OS_MemoryPoolNew.
 * \param[in]     block         address of the allocated memory block to be returned to the memory pool.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_MemoryPoolFree(OS_MemoryPoolId_t mp_id, void *block)
{
    return OS_OK;
}

/* Get maximum number of memory blocks in a Memory Pool.
 * \param[in]     mp_id         memory pool ID obtained by \ref OS_MemoryPoolNew.
 * \return maximum number of memory blocks.
 */
uint32_t OS_MemoryPoolGetCapacity(OS_MemoryPoolId_t mp_id)
{
    return 0;
}

/* Get memory block size in a Memory Pool.
 * \param[in]     mp_id         memory pool ID obtained by \ref OS_MemoryPoolNew.
 * \return memory block size in bytes.
 */
uint32_t OS_MemoryPoolGetBlockSize(OS_MemoryPoolId_t mp_id)
{
    return 0;
}

/* Get number of memory blocks used in a Memory Pool.
 * \param[in]     mp_id         memory pool ID obtained by \ref OS_MemoryPoolNew.
 * \return number of memory blocks used.
 */
uint32_t OS_MemoryPoolGetCount(OS_MemoryPoolId_t mp_id)
{
    return 0;
}

/* Get number of memory blocks available in a Memory Pool.
 * \param[in]     mp_id         memory pool ID obtained by \ref OS_MemoryPoolNew.
 * \return number of memory blocks available.
 */
uint32_t OS_MemoryPoolGetSpace(OS_MemoryPoolId_t mp_id)
{
    return 0;
}

/* Delete a Memory Pool object.
 * \param[in]     mp_id         memory pool ID obtained by \ref OS_MemoryPoolNew.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_MemoryPoolDelete(OS_MemoryPoolId_t mp_id)
{
    return OS_OK;
}

/*  ==== Memory Management Functions ==== */

/* Create and Initialize a Memory object.
 * \param[in]     nbytes   the size of malloc memory.
 * \return  allocated memory block address.
 */
void *OS_MemoryMalloc(uint32_t nbytes)
{
    return NULL;
}

/* Get name of a Memory object.
 * \param[in]     ptr      memory block pointer to be released.
 * \return none.
 */
void OS_MemoryFree(void *ptr)
{
    printf("this null function\n");
}

/* Create and Initialize a Memory object.
 * \param[in]     nmemb    the count of malloc memory.
 * \param[in]     nbytes   the size of malloc memory.
 * \return  allocated memory block address.
 */
void *OS_MemoryCalloc(uint32_t nmemb, uint32_t nbytes)
{
    return NULL;
}

/*  ==== Message Queue Management Functions ==== */

/* Create and Initialize a Message Queue object.
 * \param[in]     msg_count     maximum number of messages in queue.
 * \param[in]     msg_size      maximum message size in bytes.
 * \param[in]     attr          message queue attributes; NULL: default values.
 * \return message queue ID for reference by other functions or NULL in case of error.
 */
OS_MessageQueueId_t OS_MessageQueueNew(uint32_t msg_count, uint32_t msg_size, const OS_MessageQueueAttr_t *attr)
{
    return (OS_MessageQueueId_t)0;
}

/* Get name of a Message Queue object.
 * \param[in]     mq_id         message queue ID obtained by \ref OS_MessageQueueNew.
 * \return name as null-terminated string.
 */
const char *OS_MessageQueueGetName(OS_MessageQueueId_t mq_id)
{
    return 0;
}

/* Put a Message into a Queue or timeout if Queue is full.
 * \param[in]     mq_id         message queue ID obtained by \ref OS_MessageQueueNew.
 * \param[in]     msg_ptr       pointer to buffer with message to put into a queue.
 * \param[in]     msg_prio      message priority.
 * \param[in]     timeout       \ref EMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_MessageQueuePut(OS_MessageQueueId_t mq_id, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout)
{
    return OS_OK;
}

/* Get a Message from a Queue or timeout if Queue is empty.
 * \param[in]     mq_id         message queue ID obtained by \ref OS_MessageQueueNew.
 * \param[out]    msg_ptr       pointer to buffer for message to get from a queue.
 * \param[out]    msg_prio      pointer to buffer for message priority or NULL.
 * \param[in]     timeout       \ref EMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_MessageQueueGet(OS_MessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout)
{
    return OS_OK;
}

/* Get maximum number of messages in a Message Queue.
 * \param[in]     mq_id         message queue ID obtained by \ref OS_MessageQueueNew.
 * \return maximum number of messages.
 */
uint32_t OS_MessageQueueGetCapacity(OS_MessageQueueId_t mq_id)
{
    return 0;
}

/* Get maximum message size in a Message Queue.
 * \param[in]     mq_id         message queue ID obtained by \ref OS_MessageQueueNew.
 * \return maximum message size in bytes.
 */
uint32_t OS_MessageQueueGetMsgSize(OS_MessageQueueId_t mq_id)
{
    return 0;
}

/* Get number of queued messages in a Message Queue.
 * \param[in]     mq_id         message queue ID obtained by \ref OS_MessageQueueNew.
 * \return number of queued messages.
 */
uint32_t OS_MessageQueueGetCount(OS_MessageQueueId_t mq_id)
{
    return 0;
}

/* Get number of available slots for messages in a Message Queue.
 * \param[in]     mq_id         message queue ID obtained by \ref OS_MessageQueueNew.
 * \return number of available slots for messages.
 */
uint32_t OS_MessageQueueGetSpace(OS_MessageQueueId_t mq_id)
{
    return 0;
}

/* Reset a Message Queue to initial empty state.
 * \param[in]     mq_id         message queue ID obtained by \ref OS_MessageQueueNew.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_MessageQueueReset(OS_MessageQueueId_t mq_id)
{
    return OS_OK;
}

/* Delete a Message Queue object.
 * \param[in]     mq_id         message queue ID obtained by \ref OS_MessageQueueNew.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_MessageQueueDelete(OS_MessageQueueId_t mq_id)
{
    return OS_OK;
}
