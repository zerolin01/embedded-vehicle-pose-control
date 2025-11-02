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
 * @file osal.h
 * @brief emps osal
 * @author abu (abu@eswincomputing.com)
 * @date 2023-03-24
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __OSAL_H_
#define __OSAL_H_

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#if defined(__GNUC__)
#include <unistd.h>
#else
#endif
#include <string.h>
#if defined(__GNUC__)
#include <unistd.h>
#else
#endif

#include "basic_api.h"
#include "../../../log/include/log.h"
#include "status.h"

#ifdef config_SELECT_FreeRTOS
#include "emsis_os.h"
#endif

#ifdef config_SELECT_Baremetal
#include "baremetal_os.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define OS_ASSERT(x)                                                                   \
    do {                                                                               \
        if (!(x)) {                                                                    \
            printf("ASSERT at %s() in %s, %d\n", __FUNCTION__, __FILE__, __LINE__); \
            while (1)                                                                  \
                ;                                                                      \
        }                                                                              \
    } while (0)

/* ----------------------------------------------------------------------------
   -- Generic macros
   ---------------------------------------------------------------------------- */

/* IO definitions (access restrictions to peripheral registers) */

#ifndef __IO
/** \brief Defines 'read only' permissions */
#ifdef __cplusplus
#define __I volatile
#else
#define __I volatile const
#endif
/** \brief Defines 'write only' permissions */
#define __O volatile
/** \brief Defines 'read / write' permissions */
#define __IO volatile

/* following defines should be used for structure members */
/** \brief Defines 'read only' structure member permissions */
#define __IM volatile const
/** \brief Defines 'write only' structure member permissions */
#define __OM volatile
/** \brief Defines 'read/write' structure member permissions */
#define __IOM volatile
#endif

#ifndef OS_NOP
#define OS_NOP __NOP
#endif

#ifndef OS_WFI
#define OS_WFI __WFI
#endif

/*  ==== Enumerations, structures, defines ==== */

/**
 * @brief OS register type structure.
 */
typedef register_type_t OS_RegisterType_t;

/**
 * @brief Version information structure.
 */
typedef struct {
    uint32_t api;    /*< API version (major.minor.rev: mmnnnrrrr dec).*/
    uint32_t kernel; /*< Kernel version (major.minor.rev: mmnnnrrrr dec).*/
} OS_Version_t;

/* OS kernel state enum.*/
typedef enum {
    OS_KernelInactive  = 0,         /*< Inactive.*/
    OS_KernelReady     = 1,         /*< Ready.*/
    OS_KernelRunning   = 2,         /*< Running.*/
    OS_KernelLocked    = 3,         /*< Locked.*/
    OS_KernelSuspended = 4,         /*< Suspended.*/
    OS_KernelError     = -1,        /*< Error.*/
    OS_KernelReserved  = 0x7FFFFFFF /*< Prevents enum down-size compiler optimization.*/
} OS_KernelState_t;

/* OS thread state enum.*/
typedef enum {
    OS_ThreadInactive   = 0,         /*< Inactive.*/
    OS_ThreadReady      = 1,         /*< Ready.*/
    OS_ThreadRunning    = 2,         /*< Running.*/
    OS_ThreadBlocked    = 3,         /*< Blocked.*/
    OS_ThreadTerminated = 4,         /*< Terminated.*/
    OS_ThreadError      = -1,        /*< Error.*/
    OS_ThreadReserved   = 0x7FFFFFFF /*< Prevents enum down-size compiler optimization.*/
} OS_ThreadState_t;

/* OS Priority type structure.*/
typedef uint32_t OS_Priority_t;

/* Entry point of a thread.*/
typedef void (*OS_ThreadFunc_t)(void *argument);

/* Timer callback function.*/
typedef void (*OS_TimerFunc_t)(void *argument);

/* Timer type enum.*/
typedef enum {
    OS_TimerOnce     = 0, /*< One-shot timer.*/
    OS_TimerPeriodic = 1  /*< Repeating timer.*/
} OS_TimerType_t;

/* Timeout value.*/
#define OS_WaitForever 0xFFFFFFFFU /*< Wait forever timeout value.*/

/* Flags options (\ref OS_ThreadFlagsWait and \ref OS_EventFlagsWait).*/
#define OS_FlagsWaitAny 0x00000000U /*< Wait for any flag (default).*/
#define OS_FlagsWaitAll 0x00000001U /*< Wait for all flags.*/
#define OS_FlagsNoClear 0x00000002U /*< Do not clear flags which have been specified to wait for.*/

/* Flags errors (returned by OS_ThreadFlagsXxxx and OS_EventFlagsXxxx).*/
#define OS_FlagsError          0x80000000U /*< Error indicator.*/
#define OS_FlagsErrorUnknown   0xFFFFFFFFU /*< OS_Error (-1).*/
#define OS_FlagsErrorTimeout   0xFFFFFFFEU /*< OS_ErrorTimeout (-2).*/
#define OS_FlagsErrorResource  0xFFFFFFFDU /*< OS_ErrorResource (-3).*/
#define OS_FlagsErrorParameter 0xFFFFFFFCU /*< OS_ErrorParameter (-4).*/
#define OS_FlagsErrorISR       0xFFFFFFFAU /*< OS_ErrorISR (-6).*/

/* Thread attributes (attr_bits in \ref OS_ThreadAttr_t).*/
#define OS_ThreadDetached 0x00000000U /*< Thread created in detached mode (default)*/
#define OS_ThreadJoinable 0x00000001U /*< Thread created in joinable mode*/

/* Mutex attributes (attr_bits in \ref OS_MutexAttr_t). */
#define OS_MutexRecursive   0x00000001U /*< Recursive mutex.*/
#define OS_MutexPrioInherit 0x00000002U /*< Priority inherit protocol.*/
#define OS_MutexRobust      0x00000008U /*< Robust mutex.*/

/* Mode attribute (mode in \ref XxxxAttr_t)*/
#define OS_ModeFalse 0x00000000U
#define OS_ModeTrue  0x00000001U

/* Status code values returned by EMSIS-RTOS functions enum.*/
typedef enum {
    OS_OK             = 0,  /*< Operation completed successfully.*/
    OS_Error          = -1, /*< Unspecified RTOS error: run-time error but no other error message fits.*/
    OS_ErrorTimeout   = -2, /*< Operation not completed within the timeout period.*/
    OS_ErrorResource  = -3, /*< Resource not available.*/
    OS_ErrorParameter = -4, /*< Parameter error.*/
    OS_ErrorNoMemory =
        -5,                        /*< System is out of memory: it was impOS_sible to allocate or reserve memory for the operation.*/
    OS_ErrorISR       = -6,        /*< Not allowed in ISR context: the function cannot be called from interrupt service routines.*/
    OS_StatusReserved = 0x7FFFFFFF /*< Prevents enum down-size compiler optimization.*/
} OS_Status_t;

/* details Thread ID identifies the thread.*/
typedef void *OS_ThreadId_t;

/* details Timer ID identifies the timer.*/
typedef void *OS_TimerId_t;

/* details Event Flags ID identifies the event flags.*/
typedef void *OS_EventFlagsId_t;

/* details Mutex ID identifies the mutex.*/
typedef void *OS_MutexId_t;

/* details Semaphore ID identifies the semaphore.*/
typedef void *OS_SemaphoreId_t;

/* details Memory Pool ID identifies the memory pool.*/
typedef void *OS_MemoryPoolId_t;

/* details Message Queue ID identifies the message queue.*/
typedef void *OS_MessageQueueId_t;

#ifndef TZ_MODULEID_T
#define TZ_MODULEID_T
/* details Data type that identifies secure software modules called by a process.*/
typedef uint32_t TZ_ModuleId_t;
#endif

/**
 * @brief Attributes structure for thread.
 */
typedef struct {
    const char *name;        /*< name of the thread*/
    uint32_t attr_bits;      /*< attribute bits*/
    void *cb_mem;            /*< memory for control block*/
    uint32_t cb_size;        /*< size of provided memory for control block*/
    void *stack_mem;         /*< memory for stack*/
    uint32_t stack_size;     /*< size of stack*/
    OS_Priority_t priority;  /*< initial thread priority (default: OS_PriorityNormal)*/
    TZ_ModuleId_t tz_module; /*< TrustZone module identifier*/
    uint32_t reserved;       /*< reserved (must be 0)*/
} OS_ThreadAttr_t;

/**
 * @brief Attributes structure for timer.
 */
typedef struct {
    const char *name;   /*< name of the timer*/
    uint32_t attr_bits; /*< attribute bits*/
    void *cb_mem;       /*< memory for control block*/
    uint32_t cb_size;   /*< size of provided memory for control block*/
} OS_TimerAttr_t;

/**
 * @brief Attributes structure for event flags.
 */
typedef struct {
    const char *name;   /*< name of the event flags*/
    uint32_t attr_bits; /*< attribute bits*/
    void *cb_mem;       /*< memory for control block*/
    uint32_t cb_size;   /*< size of provided memory for control block*/
} OS_EventFlagsAttr_t;

/**
 * @brief Attributes structure for mutex.
 */
typedef struct {
    const char *name;   /*< name of the mutex*/
    uint32_t attr_bits; /*< attribute bits*/
    void *cb_mem;       /*< memory for control block*/
    uint32_t cb_size;   /*< size of provided memory for control block*/
} OS_MutexAttr_t;

/**
 * @brief Attributes structure for semaphore.
 */
typedef struct {
    const char *name;   /*< name of the semaphore*/
    uint32_t attr_bits; /*< attribute bits*/
    void *cb_mem;       /*< memory for control block*/
    uint32_t cb_size;   /*< size of provided memory for control block*/
} OS_SemaphoreAttr_t;

/**
 * @brief Attributes structure for memory pool.
 */
typedef struct {
    const char *name;   /*< name of the memory pool*/
    uint32_t attr_bits; /*< attribute bits*/
    void *cb_mem;       /*< memory for control block*/
    uint32_t cb_size;   /*< size of provided memory for control block*/
    void *mp_mem;       /*< memory for data storage*/
    uint32_t mp_size;   /*< size of provided memory for data storage*/
} OS_MemoryPoolAttr_t;

/**
 * @brief Attributes structure for message queue.
 */
typedef struct {
    const char *name;   /*< name of the message queue*/
    uint32_t attr_bits; /*< attribute bits*/
    void *cb_mem;       /*< memory for control block*/
    uint32_t cb_size;   /*< size of provided memory for control block*/
    void *mq_mem;       /*< memory for data storage*/
    uint32_t mq_size;   /*< size of provided memory for data storage*/
} OS_MessageQueueAttr_t;

/*  ==== Kernel Management Functions ==== */

/* Initialize the RTOS Kernel.*/
/* return status code that indicates the execution status of the function.
 *OS_Status_t OS_KernelInitialize(void);
 */

/*  Get RTOS Kernel Information.
 * \param[out]    version       pointer to buffer for retrieving version information.
 * \param[out]    id_buf        pointer to buffer for retrieving kernel identification string.
 * \param[in]     id_size       size of buffer for kernel identification string.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_KernelGetInfo(OS_Version_t *version, char *id_buf, uint32_t id_size);

/* Get the current RTOS Kernel state.
 * \return current RTOS Kernel state.
 */
OS_KernelState_t OS_KernelGetState(void);

/* Start the RTOS Kernel scheduler.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_KernelStart(void);

/* SHIYANGYANG
 * Stop the RTOS Kernel scheduler.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_KernelStop(void);

/* Lock the RTOS Kernel scheduler.
 * \return previous lock state (1 - locked, 0 - not locked, error code if negative).
 */
int32_t OS_KernelLock(void);

/* Unlock the RTOS Kernel scheduler.
 * \return previous lock state (1 - locked, 0 - not locked, error code if negative).
 */
int32_t OS_KernelUnlock(void);

/* Restore the RTOS Kernel scheduler lock state.
 * \param[in]     lock          lock state obtained by \ref OS_KernelLock or \ref OS_KernelUnlock.
 * \return new lock state (1 - locked, 0 - not locked, error code if negative).
 */
int32_t OS_KernelRestoreLock(int32_t lock);

/* Suspend the RTOS Kernel scheduler.
 * \return time in ticks, for how long the system can sleep or power-down.
 */
uint32_t OS_KernelSuspend(void);

/* Resume the RTOS Kernel scheduler.
 * \param[in]     sleep_ticks   time in ticks for how long the system was in sleep or power-down mode.
 */
void OS_KernelResume(uint32_t sleep_ticks);

/* Get the RTOS kernel tick count.
 * \return RTOS kernel current tick count.
 */
uint32_t OS_KernelGetTickCount(void);

/* Get the RTOS kernel tick frequency.
 * \return frequency of the kernel tick in hertz, i.e. kernel ticks per second.
 */
uint32_t OS_KernelGetTickFreq(void);

/* Get the RTOS kernel system timer count.
 * \return RTOS kernel current system timer count as 32-bit value.
 */
uint32_t OS_KernelGetSysTimerCount(void);

/* Get the RTOS kernel system timer frequency.
 * \return frequency of the system timer in hertz, i.e. timer ticks per second.
 */
uint32_t OS_KernelGetSysTimerFreq(void);

/**
 * @brief OS_Msec2Tick
 * @details
 * 		The millisecond is converted to tick.
 * @param
 * 		millisecond - how many millisecond need to be converted.
 * @retval
 * 		return ticks.
 * */
uint64_t OS_Msec2Tick(uint32_t milliseconds);

/**
 * @brief OS_TimerCountReset
 * @details
 * 		reset the counter to zero.
 * @retval
 * 		return void.
 * */
void OS_TimerCountReset(void);

/*  ==== Thread Management Functions ==== */

/* Create a thread and add it to Active Threads.
 * \param[in]     func          thread function.
 * \param[in]     argument      pointer that is passed to the thread function as start argument.
 * \param[in]     attr          thread attributes; NULL: default values.
 * \return thread ID for reference by other functions or NULL in case of error.
 */
OS_ThreadId_t OS_ThreadNew(OS_ThreadFunc_t func, void *argument, const OS_ThreadAttr_t *attr);

/* Get name of a thread.
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \return name as null-terminated string.
 */
const char *OS_ThreadGetName(OS_ThreadId_t thread_id);

/* Return the thread ID of the current running thread.
 * \return thread ID for reference by other functions or NULL in case of error.
 */
OS_ThreadId_t OS_ThreadGetId(void);

/* Get current thread state of a thread.
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \return current thread state of the specified thread.
 */
OS_ThreadState_t OS_ThreadGetState(OS_ThreadId_t thread_id);

/* Get stack size of a thread.
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \return stack size in bytes.
 */
uint32_t OS_ThreadGetStackSize(OS_ThreadId_t thread_id);

/* Get available stack space of a thread based on stack watermark recording during execution.
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \return remaining stack space in bytes.
 */
uint32_t OS_ThreadGetStackSpace(OS_ThreadId_t thread_id);

/* Change priority of a thread.
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \param[in]     priority      new priority value for the thread function.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_ThreadSetPriority(OS_ThreadId_t thread_id, OS_Priority_t priority);

/* Get current priority of a thread.
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \return current priority value of the specified thread.
 */
OS_Priority_t OS_ThreadGetPriority(OS_ThreadId_t thread_id);

/* Pass control to next thread that is in state \b READY.
 * \param[in]     core_id     code_id obtained by \ref __get_hartid.It's useful when it's multicore.
 * \return status code that indicates the execution status of the function.
 */

/* OS_Status_t OS_ThreadYield (void); */
OS_Status_t OS_ThreadYield(uint32_t core_id);

/* Suspend execution of a thread.
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_ThreadSuspend(OS_ThreadId_t thread_id);

/* Resume execution of a thread.
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_ThreadResume(OS_ThreadId_t thread_id);

/* Detach a thread (thread storage can be reclaimed when thread terminates).
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_ThreadDetach(OS_ThreadId_t thread_id);

/* Wait for specified thread to terminate.
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_ThreadJoin(OS_ThreadId_t thread_id);

/* Terminate execution of current running thread.*/
/* __NO_RETURN void OS_ThreadExit (void); */
void OS_ThreadExit(void);

/* Terminate execution of a thread.
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_ThreadTerminate(OS_ThreadId_t thread_id);

/* Get number of active threads.
 * \return number of active threads.
 */
uint32_t OS_ThreadGetCount(void);

/* Enumerate active threads.
 * \param[out]    thread_array  pointer to array for retrieving thread IDs.
 * \param[in]     array_items   maximum number of items in array for retrieving thread IDs.
 * \return number of enumerated threads.
 */
uint32_t OS_ThreadEnumerate(OS_ThreadId_t *thread_array, uint32_t array_items);

/* SHIYANGYANG
 * Delete a thread (thread storage can be reclaimed when thread terminates).
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_ThreadDelete(OS_ThreadId_t thread_id);

/* SHIYANGYANG
 * Start a thread (Only RTThread uses it).
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_ThreadStartUp(OS_ThreadId_t thread_id);

/*  ==== Thread Flags Functions ==== */

/* Set the specified Thread Flags of a thread.
 * \param[in]     thread_id     thread ID obtained by \ref OS_ThreadNew or \ref OS_ThreadGetId.
 * \param[in]     flags         specifies the flags of the thread that sbasicl be set.
 * \return thread flags after setting or error code if highest bit set.
 */
uint32_t OS_ThreadFlagsSet(OS_ThreadId_t thread_id, uint32_t flags);

/* Clear the specified Thread Flags of current running thread.
 * \param[in]     flags         specifies the flags of the thread that sbasicl be cleared.
 * \return thread flags before clearing or error code if highest bit set.
 */
uint32_t OS_ThreadFlagsClear(uint32_t flags);

/* Get the current Thread Flags of current running thread.
 * \return current thread flags.
 */
uint32_t OS_ThreadFlagsGet(void);

/* Wait for one or more Thread Flags of the current running thread to become signaled.
 * \param[in]     flags         specifies the flags to wait for.
 * \param[in]     options       specifies flags options (OS_FlagsXxxx).
 * \param[in]     timeout       \ref EMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
 * \return thread flags before clearing or error code if highest bit set.
 */
/* uint32_t OS_ThreadFlagsWait (uint32_t flags, uint32_t options, uint32_t timeout); */
uint32_t OS_ThreadFlagsWait(OS_ThreadId_t thread_id, uint32_t flags, uint32_t options, uint32_t timeout);

/*  ==== Generic Wait Functions ==== */

/* Wait for Timeout (Time Delay).
 * \param[in]     ticks         \ref EMSIS_RTOS_TimeOutValue "time ticks" value
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_DelayTicks(uint32_t ticks);

/* Wait until specified time.
 * \param[in]     ticks         absolute time in ticks
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_DelayTicksUntil(uint32_t ticks);

/**
 * @brief OS_DelayS
 * @details
 * 		Wait for a specified time.The accuracy is seconds.
 * @param
 * 		seconds - how many seconds need to be wait.
 * @retval
 * 		status code that indicates the execution status of the function.
 * */
OS_Status_t OS_DelayS(uint32_t seconds);

/**
 * @brief OS_DelayMs
 * @details
 * 		Wait for a specified time.The accuracy is milliseconds.
 * @param
 * 		milliseconds - how many milliseconds need to be wait.
 * @retval
 * 		status code that indicates the execution status of the function.
 * */
OS_Status_t OS_DelayMs(uint32_t milliseconds);

/**
 * @brief OS_DelayUs
 * @details
 * 		Wait for a specified time.The accuracy is microseconds.
 * @param
 * 		microseconds - how many microseconds need to be wait.
 * @retval
 * 		status code that indicates the execution status of the function.
 * */
OS_Status_t OS_DelayUs(uint32_t microseconds);

/*  ==== Timer Management Functions ====  */

/* Create and Initialize a timer.
 * \param[in]     func          function pointer to callback function.
 * \param[in]     type          \ref OS_TimerOnce for one-shot or \ref OS_TimerPeriodic for periodic behavior.
 * \param[in]     argument      argument to the timer callback function.
 * \param[in]     attr          timer attributes; NULL: default values.
 * \return timer ID for reference by other functions or NULL in case of error.
 */
OS_TimerId_t OS_TimerNew(OS_TimerFunc_t func, OS_TimerType_t type, void *argument, const OS_TimerAttr_t *attr);

/* Get name of a timer.
 * \param[in]     timer_id      timer ID obtained by \ref OS_TimerNew.
 * \return name as null-terminated string.
 */
const char *OS_TimerGetName(OS_TimerId_t timer_id);

/* Start or restart a timer.
 * \param[in]     timer_id      timer ID obtained by \ref OS_TimerNew.
 * \param[in]     ticks         \ref EMSIS_RTOS_TimeOutValue "time ticks" value of the timer.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_TimerStart(OS_TimerId_t timer_id, uint32_t ticks);

/* Stop a timer.
 * \param[in]     timer_id      timer ID obtained by \ref OS_TimerNew.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_TimerStop(OS_TimerId_t timer_id);

/* Check if a timer is running.
 * \param[in]     timer_id      timer ID obtained by \ref OS_TimerNew.
 * \return 0 not running, 1 running.
 */
uint32_t OS_TimerIsRunning(OS_TimerId_t timer_id);

/* Delete a timer.
 * \param[in]     timer_id      timer ID obtained by \ref OS_TimerNew.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_TimerDelete(OS_TimerId_t timer_id);

/*  ==== Event Flags Management Functions ==== */

/* Create and Initialize an Event Flags object.
 * \param[in]     attr          event flags attributes; NULL: default values.
 * \return event flags ID for reference by other functions or NULL in case of error.
 */
OS_EventFlagsId_t OS_EventFlagsNew(const OS_EventFlagsAttr_t *attr);

/* Get name of an Event Flags object.
 * \param[in]     ef_id         event flags ID obtained by \ref OS_EventFlagsNew.
 * \return name as null-terminated string.
 */
const char *OS_EventFlagsGetName(OS_EventFlagsId_t ef_id);

/* Set the specified Event Flags.
 * \param[in]     ef_id         event flags ID obtained by \ref OS_EventFlagsNew.
 * \param[in]     flags         specifies the flags that sbasicl be set.
 * \return event flags after setting or error code if highest bit set.
 */
uint32_t OS_EventFlagsSet(OS_EventFlagsId_t ef_id, uint32_t flags);

/* Clear the specified Event Flags.
 * \param[in]     ef_id         event flags ID obtained by \ref OS_EventFlagsNew.
 * \param[in]     flags         specifies the flags that sbasicl be cleared.
 * \return event flags before clearing or error code if highest bit set.
 */
uint32_t OS_EventFlagsClear(OS_EventFlagsId_t ef_id, uint32_t flags);

/* Get the current Event Flags.
 * \param[in]     ef_id         event flags ID obtained by \ref OS_EventFlagsNew.
 * \return current event flags.
 */
uint32_t OS_EventFlagsGet(OS_EventFlagsId_t ef_id);

/* Wait for one or more Event Flags to become signaled.
 * \param[in]     ef_id         event flags ID obtained by \ref OS_EventFlagsNew.
 * \param[in]     flags         specifies the flags to wait for.
 * \param[in]     options       specifies flags options (OS_FlagsXxxx).
 * \param[in]     timeout       \ref EMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
 * \return event flags before clearing or error code if highest bit set.
 */
uint32_t OS_EventFlagsWait(OS_EventFlagsId_t ef_id, uint32_t flags, uint32_t options, uint32_t timeout);

/* Delete an Event Flags object.
 * \param[in]     ef_id         event flags ID obtained by \ref OS_EventFlagsNew.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_EventFlagsDelete(OS_EventFlagsId_t ef_id);

/*  ==== Mutex Management Functions ==== */

/* Create and Initialize a Mutex object.
 * \param[in]     attr          mutex attributes; NULL: default values.
 * \return mutex ID for reference by other functions or NULL in case of error.
 */
OS_MutexId_t OS_MutexNew(const OS_MutexAttr_t *attr);

/* Get name of a Mutex object.
 * \param[in]     mutex_id      mutex ID obtained by \ref OS_MutexNew.
 * \return name as null-terminated string.
 */
const char *OS_MutexGetName(OS_MutexId_t mutex_id);

/* Acquire a Mutex or timeout if it is locked.
 * \param[in]     mutex_id      mutex ID obtained by \ref OS_MutexNew.
 * \param[in]     timeout       \ref EMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_MutexAcquire(OS_MutexId_t mutex_id, uint32_t timeout);

/* Release a Mutex that was acquired by \ref OS_MutexAcquire.
 * \param[in]     mutex_id      mutex ID obtained by \ref OS_MutexNew.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_MutexRelease(OS_MutexId_t mutex_id);

/* Get Thread which owns a Mutex object.
 * \param[in]     mutex_id      mutex ID obtained by \ref OS_MutexNew.
 * \return thread ID of owner thread or NULL when mutex was not acquired.
 */
OS_ThreadId_t OS_MutexGetOwner(OS_MutexId_t mutex_id);

/* Delete a Mutex object.
 * \param[in]     mutex_id      mutex ID obtained by \ref OS_MutexNew.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_MutexDelete(OS_MutexId_t mutex_id);

/*  ==== Semaphore Management Functions ====  */

/* Create and Initialize a Semaphore object.
 * \param[in]     max_count     maximum number of available tokens.
 * \param[in]     initial_count initial number of available tokens.
 * \param[in]     attr          semaphore attributes; NULL: default values.
 * \return semaphore ID for reference by other functions or NULL in case of error.
 */
OS_SemaphoreId_t OS_SemaphoreNew(uint32_t max_count, uint32_t initial_count, const OS_SemaphoreAttr_t *attr);

/* Get name of a Semaphore object.
 * \param[in]     semaphore_id  semaphore ID obtained by \ref OS_SemaphoreNew.
 * \return name as null-terminated string.
 */
const char *OS_SemaphoreGetName(OS_SemaphoreId_t semaphore_id);

/* Acquire a Semaphore token or timeout if no tokens are available.
 * \param[in]     semaphore_id  semaphore ID obtained by \ref OS_SemaphoreNew.
 * \param[in]     timeout       \ref EMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_SemaphoreAcquire(OS_SemaphoreId_t semaphore_id, uint32_t timeout);

/* Release a Semaphore token up to the initial maximum count.
 * \param[in]     semaphore_id  semaphore ID obtained by \ref OS_SemaphoreNew.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_SemaphoreRelease(OS_SemaphoreId_t semaphore_id);

/* Get current Semaphore token count.
 * \param[in]     semaphore_id  semaphore ID obtained by \ref OS_SemaphoreNew.
 * \return number of tokens available.
 */
uint32_t OS_SemaphoreGetCount(OS_SemaphoreId_t semaphore_id);

/* Delete a Semaphore object.
 * \param[in]     semaphore_id  semaphore ID obtained by \ref OS_SemaphoreNew.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_SemaphoreDelete(OS_SemaphoreId_t semaphore_id);

/*  ==== Memory Pool Management Functions ====  */

/* Create and Initialize a Memory Pool object.
 * \param[in]     block_count   maximum number of memory blocks in memory pool.
 * \param[in]     block_size    memory block size in bytes.
 * \param[in]     attr          memory pool attributes; NULL: default values.
 * \return memory pool ID for reference by other functions or NULL in case of error.
 */
OS_MemoryPoolId_t OS_MemoryPoolNew(uint32_t block_count, uint32_t block_size, const OS_MemoryPoolAttr_t *attr);

/* Get name of a Memory Pool object.
 * \param[in]     mp_id         memory pool ID obtained by \ref OS_MemoryPoolNew.
 * \return name as null-terminated string.
 */
const char *OS_MemoryPoolGetName(OS_MemoryPoolId_t mp_id);

/* Allocate a memory block from a Memory Pool.
 * \param[in]     mp_id         memory pool ID obtained by \ref OS_MemoryPoolNew.
 * \param[in]     timeout       \ref EMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
 * \return address of the allocated memory block or NULL in case of no memory is available.
 */
void *OS_MemoryPoolAlloc(OS_MemoryPoolId_t mp_id, uint32_t timeout);

/* Return an allocated memory block back to a Memory Pool.
 * \param[in]     mp_id         memory pool ID obtained by \ref OS_MemoryPoolNew.
 * \param[in]     block         address of the allocated memory block to be returned to the memory pool.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_MemoryPoolFree(OS_MemoryPoolId_t mp_id, void *block);

/* Get maximum number of memory blocks in a Memory Pool.
 * \param[in]     mp_id         memory pool ID obtained by \ref OS_MemoryPoolNew.
 * \return maximum number of memory blocks.
 */
uint32_t OS_MemoryPoolGetCapacity(OS_MemoryPoolId_t mp_id);

/* Get memory block size in a Memory Pool.
 * \param[in]     mp_id         memory pool ID obtained by \ref OS_MemoryPoolNew.
 * \return memory block size in bytes.
 */
uint32_t OS_MemoryPoolGetBlockSize(OS_MemoryPoolId_t mp_id);

/* Get number of memory blocks used in a Memory Pool.
 * \param[in]     mp_id         memory pool ID obtained by \ref OS_MemoryPoolNew.
 * \return number of memory blocks used.
 */
uint32_t OS_MemoryPoolGetCount(OS_MemoryPoolId_t mp_id);

/* Get number of memory blocks available in a Memory Pool.
 * \param[in]     mp_id         memory pool ID obtained by \ref OS_MemoryPoolNew.
 * \return number of memory blocks available.
 */
uint32_t OS_MemoryPoolGetSpace(OS_MemoryPoolId_t mp_id);

/* Delete a Memory Pool object.
 * \param[in]     mp_id         memory pool ID obtained by \ref OS_MemoryPoolNew.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_MemoryPoolDelete(OS_MemoryPoolId_t mp_id);

/*  ==== Memory Management Functions ==== */

/* Create and Initialize a Memory object.
 * \param[in]     nbytes   the size of malloc memory.
 * \return  allocated memory block address.
 */
void *OS_MemoryMalloc(uint32_t nbytes);

/* Get name of a Memory object.
 * \param[in]     ptr      memory block pointer to be released.
 * \return none.
 */
void OS_MemoryFree(void *ptr);

/* Create and Initialize a Memory object.
 * \param[in]     nmemb    the count of malloc memory.
 * \param[in]     nbytes   the size of malloc memory.
 * \return  allocated memory block address.
 */
void *OS_MemoryCalloc(uint32_t nmemb, uint32_t nbytes);

/*  ==== Message Queue Management Functions ==== */

/* Create and Initialize a Message Queue object.
 * \param[in]     msg_count     maximum number of messages in queue.
 * \param[in]     msg_size      maximum message size in bytes.
 * \param[in]     attr          message queue attributes; NULL: default values.
 * \return message queue ID for reference by other functions or NULL in case of error.
 */
OS_MessageQueueId_t OS_MessageQueueNew(uint32_t msg_count, uint32_t msg_size, const OS_MessageQueueAttr_t *attr);

/* Get name of a Message Queue object.
 * \param[in]     mq_id         message queue ID obtained by \ref OS_MessageQueueNew.
 * \return name as null-terminated string.
 */
const char *OS_MessageQueueGetName(OS_MessageQueueId_t mq_id);

/* Put a Message into a Queue or timeout if Queue is full.
 * \param[in]     mq_id         message queue ID obtained by \ref OS_MessageQueueNew.
 * \param[in]     msg_ptr       pointer to buffer with message to put into a queue.
 * \param[in]     msg_prio      message priority.
 * \param[in]     timeout       \ref EMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_MessageQueuePut(OS_MessageQueueId_t mq_id, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout);

/* Get a Message from a Queue or timeout if Queue is empty.
 * \param[in]     mq_id         message queue ID obtained by \ref OS_MessageQueueNew.
 * \param[out]    msg_ptr       pointer to buffer for message to get from a queue.
 * \param[out]    msg_prio      pointer to buffer for message priority or NULL.
 * \param[in]     timeout       \ref EMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_MessageQueueGet(OS_MessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout);

/* Get maximum number of messages in a Message Queue.
 * \param[in]     mq_id         message queue ID obtained by \ref OS_MessageQueueNew.
 * \return maximum number of messages.
 */
uint32_t OS_MessageQueueGetCapacity(OS_MessageQueueId_t mq_id);

/* Get maximum message size in a Message Queue.
 * \param[in]     mq_id         message queue ID obtained by \ref OS_MessageQueueNew.
 * \return maximum message size in bytes.
 */
uint32_t OS_MessageQueueGetMsgSize(OS_MessageQueueId_t mq_id);

/* Get number of queued messages in a Message Queue.
 * \param[in]     mq_id         message queue ID obtained by \ref OS_MessageQueueNew.
 * \return number of queued messages.
 */
uint32_t OS_MessageQueueGetCount(OS_MessageQueueId_t mq_id);

/* Get number of available slots for messages in a Message Queue.
 * \param[in]     mq_id         message queue ID obtained by \ref OS_MessageQueueNew.
 * \return number of available slots for messages.
 */
uint32_t OS_MessageQueueGetSpace(OS_MessageQueueId_t mq_id);

/* Reset a Message Queue to initial empty state.
 * \param[in]     mq_id         message queue ID obtained by \ref OS_MessageQueueNew.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_MessageQueueReset(OS_MessageQueueId_t mq_id);

/* Delete a Message Queue object.
 * \param[in]     mq_id         message queue ID obtained by \ref OS_MessageQueueNew.
 * \return status code that indicates the execution status of the function.
 */
OS_Status_t OS_MessageQueueDelete(OS_MessageQueueId_t mq_id);

/*  ==== IRQ Management Functions ==== */

/**
 * @brief OS_RequestIrq
 * @details Interrupt processing mode is set, and the specific SOC supports different interrupt modes.
 * @param
 * 		mode- For interrupt mode, see data structure enum intr_mode.
 * @retval
 * 		0- Set successfully.
 *   	<0- Setting failed, error code returned.
 **/
int OS_IntrMode(inte_mode_t mode);

/**
 * @brief OS_RequestIrq
 * @details Register interrupt handler for interrupt with IRQN.
 * @param irq- Interrupt number.
 * @param handler- Processing function.
 * @param type- sturct register_type_t config irq level/model/priority
 * @retval
 * 		Returns the original interrupt handler, and returns NULL
 * 		to indicate that the interrupt handler has not been registered before.
 **/
irq_handler OS_RequestIrq(IRQn_Type irq, irq_handler handler, OS_RegisterType_t *type);

/**
 * @brief OS_UnRequestIrq
 * @details
 * 		If the interrupt processing function with IRQ is cancelled, the corresponding
 *   	interrupt processing function will be set as the default processing function,
 * @param irq- Interrupt number.
 * @retval
 * 		Original interrupt processing function.
 **/
irq_handler OS_UnRequestIrq(IRQn_Type irq);


/**
 * @brief Used to set the interrupt level and interrupt priority.
 *
 *  There are five ways to classify interrupt levels and interrupt priorities.
 *  the default type is 0(31 interrupt level and 32 interrupt priority)
 *
 * @param  irqMode
 *
 * 0:  31 interrupt level, 32 interrupt priority default.
 * 1:  2  interrupt level, 16 interrupt priority
 * 2:  4  interrupt level, 8  interrupt priority
 * 3:  8  interrupt level, 4  interrupt priority
 * 4:  16  interrupt level, 2  interrupt priority
 *
 * @return ** void
 */
void OS_SetLevelProperty(irq_mode_t irqMode);

/**
 * @brief OS_SetIntProperty
 *
 * @param  irq        Interrupt number.
 * @param  trig_mode  CLIC Trigger Enum for different Trigger Type
 * @param  lvl        Interrupt level
 * @param  priority   interrupt priority level
 * @return ** void
 */
void OS_SetIntProperty(IRQn_Type irq, CLIC_TRIGGER_Type trig_mode, irq_level_t lvl, irq_priority_t priority);

/**
 * @brief OS_EnableInterrupt
 * @details
 * 		Enable global interrupt.
 * @retval
 * 		0- Global interrupt enabled successfully.
 *      <0- Failed to enable global interrupt, error code returned.
 **/
int OS_EnableInterrupt(void);

/**
 * @brief OS_DisableInterrupt
 * @details
 * 		Disable global interrupt.
 * @retval
 * 		0- Global interrupt disable successfully.
 *      <0- Failed to disable global interrupt, error code returned.
 **/
int OS_DisableInterrupt(void);

/**
 * @brief OS_EnableIrq
 * @details
 * 		Interrupt number corresponding to enable interrupt number IRQN.
 * @param
 * 		irq- Interrupt number.
 * @retval
 * 		0- Enable interrupt IRQN succeeded.
 *   	<0- Enable interrupt IRQN failed, return error code.
 **/
int OS_EnableIrq(IRQn_Type irq);

/**
 * @brief OS_DisableIrq
 * @details
 * 		Interrupt number corresponding to disable interrupt number IRQN.
 * @param
 * 		irq- Interrupt number.
 * @retval
 * 		0- Disable interrupt IRQN succeeded.
 *   	<0- Disable interrupt IRQN failed, return error code.
 **/
int OS_DisableIrq(IRQn_Type irq);

/*  ==== PMP Management Functions ==== */

/**
 * @brief OS_PMU_Add
 * @details
 * 		 Add access control
 * @param
 * 		addr- First address of address space to be restricted.
 *   	num- The number of the PMPxCFg hand PMPADDRx.
 *  	size- The size of the address space that needs to be limited. The size is 2^(size+3) byte.
 *         this parameters is only available when the mode is PMP_NAPOT is valid.
 *   	perm- Permission setting.
 *   	addr- PMP type
 * @retval
 * 		0-16- Returns the access control restriction handle.
 *   	ERROR- Error code.
 **/
int OS_PMU_Add(void *addr, uint32_t num, uint32_t size, basic_pmp_perm_t perm, basic_pmp_type_t type);

/**
 * @brief OS_PMU_Remove
 * @details
 * 		 Remove access control
 * @param
 * 		id- The ID returned when adding access control.
 * @retval
 * 		0- Access control removed successfully.
 *   	ERRNO:Error code.
 * */
int OS_PMU_Remove(int id);

/*  ==== IO Functions ==== */

/**
 * @brief OS_IoRead8
 * @details
 * 		IO Read 8 bits of data.
 * @param
 * 		addr- addr to be read
 * @retval
 * 		Returns the read data
 * */
uint8_t OS_IoRead8(volatile uint8_t *addr);

/**
 * @brief OS_IoRead16
 * @details
 * 		IO Read 16 bits of data.
 * @param
 * 		addr- addr to be read
 * @retval
 * 		Returns the read data
 * */
uint16_t OS_IoRead16(volatile uint16_t *addr);

/**
 * @brief OS_IoRead32
 * @details
 * 		IO Read 32 bits of data.
 * @param
 * 		addr- addr to be read
 * @retval
 * 		Returns the read data
 * */
uint32_t OS_IoRead32(volatile uint32_t *addr);

/**
 * @brief OS_IoRead64
 * @details
 * 		IO Read 64 bits of data.
 * @param
 * 		addr- addr to be read
 * @retval
 * 		Returns the read data
 * */
uint64_t OS_IoRead64(volatile uint64_t *addr);
/**
 * @brief OS_IoWrite8
 * @details
 * 		IO Write 8 bits of data.
 * @param
 * 		addr- addr to be write
 * */
void OS_IoWrite8(volatile uint8_t *addr, uint8_t data);

/**
 * @brief OS_IoWrite16
 * @details
 * 		IO Write 16 bits of data.
 * @param
 * 		addr- addr to be write
 * */
void OS_IoWrite16(volatile uint16_t *addr, uint16_t data);

/**
 * @brief OS_IoWrite32
 * @details
 * 		IO Write 32 bits of data.
 * @param
 * 		addr- addr to be write
 * */
void OS_IoWrite32(volatile uint32_t *addr, uint32_t data);

/**
 * @brief OS_IoWrite64
 * @details
 * 		IO Write 64 bits of data.
 * @param
 * 		addr- addr to be write
 * */
void OS_IoWrite64(volatile uint64_t *addr, uint64_t data);

/**
 * @brief OS_IoSetBit8
 * @details
 * 		io set bit of 8 bit.
 * @param
 * 		addr- addr to be write
 * */
void OS_IoSetBit8(volatile uint8_t *addr, uint8_t mask);

/**
 * @brief OS_IoSetBit16
 * @details
 * 		io set bit of 16 bit.
 * @param
 * 		addr- addr to be write
 * */
void OS_IoSetBit16(volatile uint16_t *addr, uint16_t mask);

/**
 * @brief OS_IoSetBit32
 * @details
 * 		io set bit of 32 bit.
 * @param
 * 		addr- addr to be write
 * */
void OS_IoSetBit32(volatile uint32_t *addr, uint32_t mask);

/**
 * @brief OS_IoSetBit64
 * @details
 * 		io set bit of 64 bit.
 * @param
 * 		addr- addr to be write
 * */
void OS_IoSetBit64(volatile uint64_t *addr, uint64_t mask);

/**
 * @brief OS_IoClearBit8
 * @details
 * 		io clear bit of 8 bit.
 * @param
 * 		addr- addr to be write
 * */
void OS_IoClearBit8(volatile uint8_t *addr, uint8_t mask);

/**
 * @brief OS_IoClearBit16
 * @details
 * 		io clear bit of 16 bit.
 * @param
 * 		addr- addr to be write
 * */
void OS_IoClearBit16(volatile uint16_t *addr, uint16_t mask);

/**
 * @brief OS_IoClearBit32
 * @details
 * 		io clear bit of 32 bit.
 * @param
 * 		addr- addr to be write
 * */
void OS_IoClearBit32(volatile uint32_t *addr, uint32_t mask);

/**
 * @brief OS_IoClearBit64
 * @details
 * 		io clear bit of 64 bit.
 * @param
 * 		addr- addr to be write
 * */
void OS_IoClearBit64(volatile uint64_t *addr, uint64_t mask);

/**
 * @brief OS_IoRMW8
 * @details
 * 		8 bit clear bits and set with new value
 * @param
 * 		addr- addr to be write
 * @note It is user's responsability to make sure that value has only "mask" bits set - (value&~mask)==0
 *
 * */
void OS_IoRMW8(volatile uint8_t *addr, uint8_t mask, uint8_t value);

/**
 * @brief OS_IoRMW16
 * @details
 * 		16 bit clear bits and set with new value
 * @param
 * 		addr- addr to be write
 * @note It is user's responsability to make sure that value has only "mask" bits set - (value&~mask)==0
 *
 * */
void OS_IoRMW16(volatile uint16_t *addr, uint16_t mask, uint16_t value);

/**
 * @brief OS_IoRMW32
 * @details
 * 		32 bit clear bits and set with new value
 * @param
 * 		addr- addr to be write
 * @note It is user's responsability to make sure that value has only "mask" bits set - (value&~mask)==0
 *
 * */
void OS_IoRMW32(volatile uint32_t *addr, uint32_t mask, uint32_t value);

/**
 * @brief OS_IoRMW64
 * @details
 * 		64 bit clear bits and set with new value
 * @param
 * 		addr- addr to be write
 * @note It is user's responsability to make sure that value has only "mask" bits set - (value&~mask)==0
 *
 * */
void OS_IoRMW64(volatile uint64_t *addr, uint64_t mask, uint64_t value);

/*  ==== Cache Management Functions ==== */

/**
 * @brief OS_InvalidCache
 * @details
 * 		An invalidate operation deallocates the copy of the cache block.
 * @param
 * 		addr The address if cache block.
 * @retval
 * 		null
 * */
void OS_InvalidCache(void *addr);

/**
 * @brief OS_FlushCache
 * @details
 * 		An flush operation deallocates the copy of the cache block.
 * @param
 * 		addr The address if cache block.
 * @retval
 * 		null
 * */
void OS_FlushCache(void *addr);

#ifdef __cplusplus
}
#endif

#endif /* OSAL_H_ */
