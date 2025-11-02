/*
 * Copyright (c) 2013-2017 ARM Limited. All rights reserved.
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
 * $Date:        10. January 2017
 * $Revision:    V1.2
 *
 * Project:      EMSIS-RTOS API V1
 * Title:        emsis_os_v1.c V1 module file
 *---------------------------------------------------------------------------
 */

#include "emsis_os.h"

#define MEM_ALIGN(size, align) (((size) + (align) - 1) & ~((align) - 1))

extern __WEAK void _exit(int fd);

typedef struct os_memPool {
	void *start_address;
	uint32_t size;

	uint32_t block_size;
	uint8_t *block_list;

	uint32_t block_total_count;
	uint32_t block_free_count;

	List_t suspend_thread;
} os_memPool_t;

//  ==== Kernel Management Functions ====
osStatus_t osKernelInitialize(void)
{
	// TODO To be adapted.
	return osOK;
}

osStatus_t osKernelGetInfo(osVersion_t *version, char *id_buf, uint32_t id_size)
{
	// TODO To be adapted.
	return osOK;
}

osKernelState_t osKernelGetState(void)
{
	BaseType_t kernelReturn;

	kernelReturn = xTaskGetSchedulerState();

	switch (kernelReturn) {
		case taskSCHEDULER_NOT_STARTED:
			return osKernelInactive;
		case taskSCHEDULER_RUNNING:
			return osKernelRunning;
		case taskSCHEDULER_SUSPENDED:
			return osKernelSuspended;
		default:
			return osKernelError;
	}
}

osStatus_t osKernelStart(void)
{
	return osOK;
}

osStatus_t osKernelStop(void)
{
	vTaskEndScheduler();
	return osOK;
}

int32_t osKernelLock(void)
{
	taskENTER_CRITICAL();
	return 0;
}

int32_t osKernelUnlock(void)
{
	// TODO To be adapted.
	taskEXIT_CRITICAL();
	return 0;
}

int32_t osKernelRestoreLock(int32_t lock)
{
	taskEXIT_CRITICAL();
	return 0;
}

uint32_t osKernelSuspend(void)
{
	vTaskSuspendAll();
	return 0;
}

void osKernelResume(uint32_t sleep_ticks)
{
	xTaskResumeAll();
}

uint32_t osKernelGetTickCount(void)
{
	return xTaskGetTickCount();
}

uint32_t osKernelGetTickFreq(void)
{
	// TODO To be adapted.
	return basic_get_freq();
}

uint32_t osKernelGetSysTimerCount(void)
{
	// TODO To be adapted.
	return 0;
}

uint32_t osKernelGetSysTimerFreq(void)
{
	// TODO To be adapted.
	return 0;
}

//  ==== Thread Management Functions ====
osThreadId_t osThreadNew(osThreadFunc_t func, void *argument, const osThreadAttr_t *attr)
{
	TaskHandle_t threadHandle = NULL;
	BaseType_t threadRval;

	if (NULL == attr->stack_mem) {
#if (configSUPPORT_DYNAMIC_ALLOCATION == 1)
		threadRval = xTaskCreate((TaskFunction_t)func,
								 (char *)attr->name,
								 (configSTACK_DEPTH_TYPE)attr->stack_size,
								 (void *)argument,
								 (UBaseType_t)attr->priority,
								 (TaskHandle_t *)&threadHandle);
		if (pdPASS == threadRval)
			return threadHandle;
		else
			return NULL;
#endif
	} else if (NULL != attr->stack_mem) {
#if (configSUPPORT_STATIC_ALLOCATION == 1)
		threadHandle = xTaskCreateStatic((TaskFunction_t)func,
										 (char *)attr->name,
										 (uint32_t)attr->stack_size,
										 (void *)argument,
										 (UBaseType_t)attr->priority,
										 (StackType_t *)attr->stack_mem,
										 (StaticTask_t *)attr->cb_mem);
		return threadHandle;
#endif
	}
	return NULL;
}

/* SHIYANGYANG
 * Delete a thread (thread storage can be reclaimed when thread terminates).
 * \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
 * \return status code that indicates the execution status of the function.
 */
osStatus_t osThreadDelete(osThreadId_t thread_id)
{
	vTaskDelete(thread_id);
	return osOK;
}

const char *osThreadGetName(osThreadId_t thread_id)
{
	char *threadName;

	threadName = pcTaskGetName((TaskHandle_t) thread_id);
	return threadName;
}

osThreadId_t osThreadGetId(void)
{
#if ((INCLUDE_xTaskGetCurrentTaskHandle == 1) || (configUSE_MUTEXES == 1))
	return xTaskGetCurrentTaskHandle();
#else
	return NULL;
#endif /* ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) */
}

osThreadState_t osThreadGetState(osThreadId_t thread_id)
{
#if ((INCLUDE_eTaskGetState == 1) || (configUSE_TRACE_FACILITY == 1) || (INCLUDE_xTaskAbortDelay == 1))
	eTaskState threadState;

	threadState = eTaskGetState((TaskHandle_t) thread_id);
	switch (threadState) {
		case eRunning:
			return osThreadRunning;
		case eReady:
			return osThreadReady;
		case eBlocked:
			return osThreadBlocked;
		case eSuspended:
			return osThreadInactive;
		case eDeleted:
			return osThreadTerminated;
		default:
			return osThreadError;
	}
#else
	return osThreadError;
#endif /* INCLUDE_eTaskGetState */
}

uint32_t osThreadGetStackSize(osThreadId_t thread_id)
{
	// TODO To be adapted.
	return 0;
}

uint32_t osThreadGetStackSpace(osThreadId_t thread_id)
{
	// TODO To be adapted.
	return 0;
}

osStatus_t osThreadSetPriority(osThreadId_t thread_id, osPriority_t priority)
{
#if (INCLUDE_vTaskPrioritySet == 1)
	vTaskPrioritySet((TaskHandle_t) thread_id, (UBaseType_t) priority);
	return osOK;
#else
	return osErrorTimeout;
#endif /* INCLUDE_vTaskPrioritySet */
}

osPriority_t osThreadGetPriority(osThreadId_t thread_id)
{
#if (INCLUDE_uxTaskPriorityGet == 1)
	UBaseType_t threadprioritry;

	threadprioritry = uxTaskPriorityGet((TaskHandle_t)thread_id);
	return threadprioritry;
#else
	return osPriorityError;
#endif /* INCLUDE_uxTaskPriorityGet */
}

/* osStatus_t osThreadYield (void) */
osStatus_t osThreadYield(uint32_t core_id)
{
	taskYIELD();
	return osOK;
}

osStatus_t osThreadSuspend(osThreadId_t thread_id)
{
#if (INCLUDE_vTaskSuspend == 1)
	vTaskSuspend((TaskHandle_t)thread_id);
	return osOK;
#else
	return osErrorTimeout;
#endif /* INCLUDE_vTaskSuspend */
}

osStatus_t osThreadResume(osThreadId_t thread_id)
{
#if (INCLUDE_vTaskSuspend == 1)
	vTaskResume((TaskHandle_t)thread_id);
	return osOK;
#else
	return osErrorTimeout;
#endif /* INCLUDE_vTaskSuspend */
}

osStatus_t osThreadDetach(osThreadId_t thread_id)
{
#if (INCLUDE_vTaskDelete == 1)
	vTaskDelete((TaskHandle_t)thread_id);
	return osOK;
#else
	return osErrorTimeout;
#endif /* INCLUDE_vTaskDelete */
}

osStatus_t osThreadJoin(osThreadId_t thread_id)
{
	// TODO To be adapted.
	return osErrorTimeout;
}

void osThreadExit(void)
{
	// TODO To be adapted.
}

osStatus_t osThreadTerminate(osThreadId_t thread_id)
{
	// TODO To be adapted.
	return osErrorTimeout;
}

uint32_t osThreadGetCount(void)
{
	return uxTaskGetNumberOfTasks();
}

uint32_t osThreadEnumerate(osThreadId_t *thread_array, uint32_t array_items)
{
#if ((configUSE_TRACE_FACILITY == 1) && (configUSE_STATS_FORMATTING_FUNCTIONS > 0) && (configSUPPORT_DYNAMIC_ALLOCATION == 1))
	vTaskList((char *)thread_array);
	return 0;
#else
	return 0;
#endif /* ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
}

osStatus_t osThreadStartUp(osThreadId_t thread_id)
{
	if (NULL == thread_id)
		return osErrorParameter;

	static int flagNum;

	if (0 == flagNum) {
		flagNum++;
		vTaskStartScheduler();
	}
	return osOK;
}

//  ==== Thread Flags Functions ====
uint32_t osThreadFlagsSet(osThreadId_t thread_id, uint32_t flags)
{
#if (configUSE_TASK_NOTIFICATIONS == 1)
	BaseType_t taskNotifyBool;

	xTaskNotifyFromISR((TaskHandle_t)thread_id,
							(uint32_t)flags,
							(eNotifyAction)eSetValueWithOverwrite,
							(BaseType_t *)&taskNotifyBool);
	return flags;
#else
	return osFlagsErrorResource;
#endif /* configUSE_TASK_NOTIFICATIONS */
}

uint32_t osThreadFlagsClear(uint32_t flags)
{
#if (configUSE_TASK_NOTIFICATIONS == 1)
	TaskHandle_t threadFlagsHandle = osThreadGetId();

	return ulTaskNotifyValueClear((TaskHandle_t)threadFlagsHandle, (uint32_t)flags);
#else
	return osFlagsErrorResource;
#endif /* configUSE_TASK_NOTIFICATIONS */
}

uint32_t osThreadFlagsGet(void)
{
	/* #if( configUSE_TASK_NOTIFICATIONS == 1 )
	 *     TCB_t *pxTCB;
	 *     uint32_t ulReturn;
	 *     TaskHandle_t threadFlagsHandle = osThreadGetId();
	 *     pxTCB = prvGetTCBFromHandle(threadFlagsHandle);
	 *     ulReturn = pxTCB->ulNotifiedValue;
	 *     return ulReturn;
	 * #else
	 *     return osFlagsErrorResource;
	 * #endif [> configUSE_TASK_NOTIFICATIONS <]
	 */
	return 0;
}

uint32_t osThreadFlagsWait(osThreadId_t thread_id, uint32_t flags, uint32_t options, uint32_t timeout)
/* uint32_t osThreadFlagsWait (uint32_t flags, uint32_t options, uint32_t timeout) */
{
#if (configUSE_TASK_NOTIFICATIONS == 1)
	BaseType_t threadFlagsReturn;
	uint32_t threadFlagsValue;

	if (osWaitForever == timeout)
		timeout = (uint32_t)portMAX_DELAY;

	if ((osFlagsNoClear | osFlagsWaitAll) == options) {
		threadFlagsReturn = xTaskNotifyWait((uint32_t)0x00,
											(uint32_t)0x00,
											(uint32_t *)&threadFlagsValue,
											timeout);
		if (pdTRUE == threadFlagsReturn)
			return threadFlagsValue;
		else
			return osFlagsErrorResource;
	} else if (osFlagsWaitAll == options) {
		threadFlagsReturn = xTaskNotifyWait((uint32_t)0x00,
											(uint32_t)0xffffffffUL,
											(uint32_t *)&threadFlagsValue,
											timeout);
		if (pdTRUE == threadFlagsReturn)
			return threadFlagsValue;
		else
			return osFlagsErrorResource;
	} else {
		return osFlagsErrorResource;
	}
#else
	return osFlagsErrorResource;
#endif /* configUSE_TASK_NOTIFICATIONS */
}

//  ==== Generic Wait Functions ====
osStatus_t osDelay(uint32_t ticks)
{
#if (INCLUDE_vTaskDelay == 1)
	vTaskDelay((TickType_t)ticks);
	return osOK;
#else
	return osErrorISR;
#endif /* INCLUDE_vTaskDelay */
}

osStatus_t osDelayUntil(uint32_t ticks)
{
#if (INCLUDE_vTaskDelayUntil == 1)
	TickType_t lasttick = xTaskGetTickCount();

	vTaskDelayUntil((TickType_t *)&lasttick, (TickType_t)ticks);
	return osOK;
#else
	return osErrorParameter;
#endif /* INCLUDE_vTaskDelayUntil */
}

//  ==== Timer Management Functions ====
osTimerId_t osTimerNew(osTimerFunc_t func, osTimerType_t type, void *argument, const osTimerAttr_t *attr)
{
	TimerHandle_t timerHandle = NULL;

	if (osTimerOnce == type)
		type = (osTimerType_t)pdFALSE;
	else if (osTimerPeriodic == type)
		type = (osTimerType_t)pdTRUE;

	if (NULL == attr->cb_mem) {
#if ((configSUPPORT_DYNAMIC_ALLOCATION == 1) && (configUSE_TIMERS == 1))
		timerHandle = xTimerCreate((char *)attr->name,
									(TickType_t)attr->attr_bits,
									(UBaseType_t)type,
									(void *)argument,
									(TimerCallbackFunction_t)func);
		return timerHandle;
#endif /* configSUPPORT_DYNAMIC_ALLOCATION */
	} else if (NULL != attr->cb_mem) {
#if ((configSUPPORT_STATIC_ALLOCATION == 1) && (configUSE_TIMERS == 1))
		timerHandle = xTimerCreateStatic((char *)attr->name,
										 (TickType_t)attr->attr_bits,
										 (UBaseType_t)type,
										 (void *)argument,
										 (TimerCallbackFunction_t)func,
										 (StaticTimer_t *)attr->cb_mem);
		return timerHandle;
#endif /* configSUPPORT_STATIC_ALLOCATION */
	}
	return NULL;
}

const char *osTimerGetName(osTimerId_t timer_id)
{
#if (configUSE_TIMERS == 1)
	return pcTimerGetName((TimerHandle_t)timer_id);
#else
	return NULL;
#endif /* configUSE_TIMERS */
}

osStatus_t osTimerStart(osTimerId_t timer_id, uint32_t ticks)
{
#if (configUSE_TIMERS == 1)
	if (pdPASS == xTimerStart((TimerHandle_t)timer_id,
								(TickType_t)ticks))
		return osOK;
	else
		return osErrorTimeout;
#else
	return osError;
#endif /* configUSE_TIMERS */
}

osStatus_t osTimerStop(osTimerId_t timer_id)
{
#if (configUSE_TIMERS == 1)
	if (pdPASS == xTimerStop((TimerHandle_t)timer_id,
								(TickType_t)0))
		return osOK;
	else
		return osErrorTimeout;
#else
	return osErrorTimeout;
#endif /* configUSE_TIMERS */
}

uint32_t osTimerIsRunning(osTimerId_t timer_id)
{
#if (configUSE_TIMERS == 1)
	if (pdFALSE != xTimerIsTimerActive((TimerHandle_t)timer_id))
		return 1;
	else
		return 0;
#else
	return 0;
#endif /* configUSE_TIMERS */
}

osStatus_t osTimerDelete(osTimerId_t timer_id)
{
#if (configUSE_TIMERS == 1)
	if (pdPASS == xTimerDelete((TimerHandle_t)timer_id,
									(TickType_t)0))
		return osOK;
	else
		return osErrorTimeout;
#else
	return osErrorTimeout;
#endif /* configUSE_TIMERS */
}

//  ==== Event Flags Management Functions ====
osEventFlagsId_t osEventFlagsNew(const osEventFlagsAttr_t *attr)
{
	EventGroupHandle_t eventHandle = NULL;

	if (NULL == attr->cb_mem) {
#if (configSUPPORT_DYNAMIC_ALLOCATION == 1)
		eventHandle = xEventGroupCreate();
		return eventHandle;
#else
		return NULL;
#endif /* configSUPPORT_DYNAMIC_ALLOCATION */
	} else if (NULL != attr->cb_mem) {
#if (configSUPPORT_STATIC_ALLOCATION == 1)
		eventHandle = xEventGroupCreateStatic((StaticEventGroup_t *)attr->cb_mem);
		return eventHandle;
#else
		return NULL;
#endif /* configSUPPORT_STATIC_ALLOCATION */
	}
	return NULL;
}

const char *osEventFlagsGetName(osEventFlagsId_t ef_id)
{
	// TODO To be adapted.
	return NULL;
}

uint32_t osEventFlagsSet(osEventFlagsId_t ef_id, uint32_t flags)
{
#if ((configUSE_TRACE_FACILITY == 1) && (INCLUDE_xTimerPendFunctionCall == 1) && (configUSE_TIMERS == 1))
	/* BaseType_t eventFlagsValue; */
	BaseType_t eventFlagsReturn;

	/* eventFlagsReturn = xEventGroupSetBitsFromISR((EventGroupHandle_t)ef_id,
	 *                                             (EventBits_t) flags,
	 *                                             (BaseType_t*)eventFlagsValue);
	 */
	eventFlagsReturn = xEventGroupSetBits((EventGroupHandle_t)ef_id,
												(EventBits_t)flags);

	if (pdPASS == eventFlagsReturn)
		return osEventFlagsGet(ef_id);
	else
		return osFlagsErrorResource;
#else
	return osFlagsErrorResource;
#endif /* configUSE_TRACE_FACILITY */
}

uint32_t osEventFlagsClear(osEventFlagsId_t ef_id, uint32_t flags)
{
#if ((configUSE_TRACE_FACILITY == 1) && (INCLUDE_xTimerPendFunctionCall == 1) && (configUSE_TIMERS == 1))
	BaseType_t eventFlagsReturn;

	/* eventFlagsReturn = xEventGroupClearBitsFromISR((EventGroupHandle_t)ef_id,
	 *                                             (EventBits_t) flags);
	 */
	eventFlagsReturn = xEventGroupClearBits((EventGroupHandle_t)ef_id,
												(EventBits_t)flags);

	if (pdPASS == eventFlagsReturn)
		return osEventFlagsGet(ef_id);
	else
		return osFlagsErrorResource;
#else
	return osFlagsErrorResource;
#endif /* configUSE_TRACE_FACILITY */
}

uint32_t osEventFlagsGet(osEventFlagsId_t ef_id)
{
	/* return xEventGroupGetBitsFromISR((EventGroupHandle_t)ef_id); */
	return xEventGroupGetBits((EventGroupHandle_t)ef_id);
}

uint32_t osEventFlagsWait(osEventFlagsId_t ef_id, uint32_t flags, uint32_t options, uint32_t timeout)
{
	EventBits_t eventWaitValue;
	BaseType_t clearValue = -1;
	BaseType_t waitValue = -1;

	if (osFlagsNoClear == options) {
		clearValue = pdFALSE;
		waitValue = pdTRUE;
	} else if (osFlagsWaitAny == options) {
		clearValue = pdTRUE;
		waitValue = pdFALSE;
	} else if (osFlagsWaitAll == options) {
		clearValue = pdTRUE;
		waitValue = pdTRUE;
	} else if ((osFlagsWaitAll | osFlagsNoClear) == options) {
		clearValue = pdFALSE;
		waitValue = pdTRUE;
	} else if ((osFlagsWaitAny | osFlagsNoClear) == options) {
		clearValue = pdFALSE;
		waitValue = pdFALSE;
	}

	if (osWaitForever == timeout)
		timeout = (uint32_t)portMAX_DELAY;

	eventWaitValue = xEventGroupWaitBits((EventGroupHandle_t)ef_id,
										(EventBits_t)flags,
										(BaseType_t)clearValue,
										(BaseType_t)waitValue,
										(TickType_t)timeout);
	return eventWaitValue;
}

osStatus_t osEventFlagsDelete(osEventFlagsId_t ef_id)
{
	vEventGroupDelete((EventGroupHandle_t)ef_id);
	return osOK;
}

//  ==== Mutex Management Functions ====
osMutexId_t osMutexNew(const osMutexAttr_t *attr)
{
	SemaphoreHandle_t mutexHandle = NULL;

	if (NULL == attr->cb_mem) {
#if ((configUSE_MUTEXES == 1) && (configSUPPORT_DYNAMIC_ALLOCATION == 1))
		mutexHandle = xSemaphoreCreateMutex();
		return mutexHandle;
#endif /* configUSE_MUTEXES */
	} else if (NULL != attr->cb_mem) {
#if ((configUSE_MUTEXES == 1) && (configSUPPORT_STATIC_ALLOCATION == 1))
		mutexHandle = xSemaphoreCreateMutexStatic((StaticQueue_t *)attr->cb_mem);
		return mutexHandle;
#endif /* configUSE_MUTEXES */
	}
	return NULL;
}

const char *osMutexGetName(osMutexId_t mutex_id)
{
	// TODO To be adapted.
	return NULL;
}

osStatus_t osMutexAcquire(osMutexId_t mutex_id, uint32_t timeout)
{
	if (osWaitForever == timeout)
		timeout = (uint32_t)portMAX_DELAY;

	if (pdTRUE == xSemaphoreTake((SemaphoreHandle_t)mutex_id, (TickType_t)timeout))
		return osOK;
	else
		return osErrorTimeout;
}

osStatus_t osMutexRelease(osMutexId_t mutex_id)
{
	if (pdPASS == xSemaphoreGive((SemaphoreHandle_t)mutex_id))
		return osOK;
	else
		return osErrorTimeout;
}

osThreadId_t osMutexGetOwner(osMutexId_t mutex_id)
{
	// TODO To be adapted.
	return NULL;
}

osStatus_t osMutexDelete(osMutexId_t mutex_id)
{
	vSemaphoreDelete((SemaphoreHandle_t)mutex_id);
	return osOK;
}

//  ==== Semaphore Management Functions ====
osSemaphoreId_t osSemaphoreNew(uint32_t max_count, uint32_t initial_count, const osSemaphoreAttr_t *attr)
{
	SemaphoreHandle_t semaphoreHandle = NULL;

	if (NULL == attr->cb_mem) {
#if ((configUSE_COUNTING_SEMAPHORES == 1) && (configSUPPORT_DYNAMIC_ALLOCATION == 1))
		semaphoreHandle = xSemaphoreCreateCounting((UBaseType_t)max_count,
												(UBaseType_t)initial_count);
		return semaphoreHandle;
#endif /* ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
	} else if (NULL != attr->cb_mem) {
#if ((configUSE_COUNTING_SEMAPHORES == 1) && (configSUPPORT_STATIC_ALLOCATION == 1))
		semaphoreHandle = xSemaphoreCreateCountingStatic((UBaseType_t)max_count,
														(UBaseType_t)initial_count,
														(StaticSemaphore_t *)attr->cb_mem);
		return semaphoreHandle;
#endif /* ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 1 ) ) */
	}
	return NULL;
}

const char *osSemaphoreGetName(osSemaphoreId_t semaphore_id)
{
	// TODO To be adapted.
	return NULL;
}

osStatus_t osSemaphoreAcquire(osSemaphoreId_t semaphore_id, uint32_t timeout)
{
	BaseType_t pxSemTaskWoken;

	if (0 == timeout) {
		if (pdPASS == xSemaphoreTakeFromISR((SemaphoreHandle_t)semaphore_id, (BaseType_t *)&pxSemTaskWoken))
			return osOK;
		else
			return osErrorTimeout;
	} else {
		if (osWaitForever == timeout)
			timeout = (uint32_t)portMAX_DELAY;

		if (pdPASS == xSemaphoreTake((SemaphoreHandle_t)semaphore_id, (TickType_t)timeout))
			return osOK;
		else
			return osErrorTimeout;
	}
}

osStatus_t osSemaphoreRelease(osSemaphoreId_t semaphore_id)
{
	BaseType_t pxSemTaskWoken;

	if (pdTRUE == xSemaphoreGiveFromISR((SemaphoreHandle_t)semaphore_id, (BaseType_t *)&pxSemTaskWoken))
		return osOK;
	else
		return osErrorTimeout;
}

uint32_t osSemaphoreGetCount(osSemaphoreId_t semaphore_id)
{
	return uxSemaphoreGetCount((SemaphoreHandle_t)semaphore_id);
}

osStatus_t osSemaphoreDelete(osSemaphoreId_t semaphore_id)
{
	vSemaphoreDelete((SemaphoreHandle_t)semaphore_id);
	return osOK;
}

//  ==== Memory Pool Management Functions ====
osMemoryPoolId_t osMemoryPoolNew(uint32_t block_count, uint32_t block_size, const osMemoryPoolAttr_t *attr)
{
	uint8_t *block_ptr;
	os_memPool_t *mp;

	register uint32_t offset;

	/* parameter check */
	if ((0 == block_count) && (0 == block_size))
		return NULL;

	/* allocate object */
	mp = pvPortMalloc(sizeof(os_memPool_t));
	/* allocate object failed */
	if (NULL == mp)
		return NULL;

	/* initialize memory pool */
	block_size = MEM_ALIGN(block_size, 4);
	mp->block_size = block_size;
	mp->size = (block_size + sizeof(uint8_t *)) * block_count;

	/* allocate memory */
	mp->start_address = pvPortMalloc((block_size + sizeof(uint8_t *)) * block_count);
	if (NULL == mp->start_address) {
		/* no memory */
		vPortFree(mp);
		return NULL;
	}

	mp->block_total_count = block_count;
	mp->block_free_count = mp->block_total_count;

	/* initialize suspended thread list */
	vListInitialise(&(mp->suspend_thread));

	/* initialize free block list */
	block_ptr = (uint8_t *)mp->start_address;
	for (offset = 0; offset < mp->block_total_count; offset++)
		*(uint8_t **)(block_ptr + offset * (block_size + sizeof(uint8_t *))) = block_ptr + (offset + 1) * (block_size + sizeof(uint8_t *));

	*(uint8_t **)(block_ptr + (offset - 1) * (block_size + sizeof(uint8_t *))) = NULL;

	mp->block_list = block_ptr;

	return mp;
}

const char *osMemoryPoolGetName(osMemoryPoolId_t mp_id)
{
	// TODO To be adapted.
	return NULL;
}

void *osMemoryPoolAlloc(osMemoryPoolId_t mp_id, uint32_t timeout)
{
	uint8_t *block_ptr;
	os_memPool_t *mp;
	BaseType_t xEntryTimeSet = pdFALSE;
	TimeOut_t xTimeOut;

	mp = mp_id;

	/* parameter check */
	if (mp == NULL)
		return NULL;

	if (osWaitForever == timeout)
		timeout = (uint32_t)portMAX_DELAY;

	/* disable interrupt */
	portDISABLE_INTERRUPTS();

	for (;;) {
		if (mp->block_free_count == 0) {
			/* memory block is unavailable. */
			if ((timeout == 0) && (mp->block_free_count == 0)) {
				/* enable interrupt */
				portENABLE_INTERRUPTS();

				return NULL;
			} else if ((xEntryTimeSet == pdFALSE) && (mp->block_free_count == 0)) {
				vTaskInternalSetTimeOutState(&xTimeOut);
				xEntryTimeSet = pdTRUE;
			} else {
				mtCOVERAGE_TEST_MARKER();
			}

			portENABLE_INTERRUPTS();

			vTaskSuspendAll();

			if (xTaskCheckForTimeOut(&xTimeOut, (TickType_t *)&timeout) == pdFALSE) {
				if (mp->block_free_count == 0) {
					vTaskPlaceOnEventList(&(mp->suspend_thread), timeout);

					if (xTaskResumeAll() == pdFALSE)
						portYIELD_WITHIN_API();
				} else
					(void) xTaskResumeAll();
			} else {
				(void) xTaskResumeAll();
				return NULL;
			}
		}

		/* memory block is available. decrease the free block counter */
		mp->block_free_count--;

		/* get block from block list */
		block_ptr = mp->block_list;
		if (NULL == mp->block_list)
			return NULL;

		/* Setup the next free node. */
		mp->block_list = *(uint8_t **)block_ptr;

		/* point to memory pool */
		*(uint8_t **)block_ptr = (uint8_t *)mp;

		/* enable interrupt */
		portENABLE_INTERRUPTS();

		return (uint8_t *)(block_ptr + sizeof(uint8_t *));
	}
}

osStatus_t osMemoryPoolFree(osMemoryPoolId_t mp_id, void *block)
{
	uint8_t **block_ptr;
	os_memPool_t *mp;

	/* parameter check */
	if (block == NULL)
		return osErrorParameter;
	for (;;) {
		/* get the control block of pool which the block belongs to */
		block_ptr = (uint8_t **)((uint8_t *)block - sizeof(uint8_t *));
		mp = (os_memPool_t *)*block_ptr;

		/* disable interrupt */
		portDISABLE_INTERRUPTS();

		/* increase the free block count */
		mp->block_free_count++;

		/* link the block into the block list */
		*block_ptr = mp->block_list;
		mp->block_list = (uint8_t *)block_ptr;

		if (listLIST_IS_EMPTY(&(mp->suspend_thread)) == pdFALSE) {
			/* if( xCoRoutineRemoveFromEventList( &( mp->suspend_thread ) ) != pdFALSE ) */
			if (xTaskRemoveFromEventList(&(mp->suspend_thread)) != pdFALSE) {
				portYIELD_WITHIN_API();
			} else {
				mtCOVERAGE_TEST_MARKER();
			}
		} else {
			mtCOVERAGE_TEST_MARKER();
		}

		portENABLE_INTERRUPTS();
		return osOK;
	}
}

uint32_t osMemoryPoolGetCapacity(osMemoryPoolId_t mp_id)
{
	// TODO To be adapted.
	return 0;
}

uint32_t osMemoryPoolGetBlockSize(osMemoryPoolId_t mp_id)
{
	// TODO To be adapted.
	return 0;
}

uint32_t osMemoryPoolGetCount(osMemoryPoolId_t mp_id)
{
	// TODO To be adapted.
	return 0;
}

uint32_t osMemoryPoolGetSpace(osMemoryPoolId_t mp_id)
{
	// TODO To be adapted.
	return 0;
}

osStatus_t osMemoryPoolDelete(osMemoryPoolId_t mp_id)
{
	os_memPool_t *mp;

	mp = mp_id;

	/* parameter check */
	if (NULL == mp)
		return osErrorParameter;

	/* wake up all suspended threads */
	while (listLIST_IS_EMPTY(&(mp->suspend_thread)) == pdFALSE) {
		/* disable interrupt */
		portDISABLE_INTERRUPTS();

		/* get next suspend thread */
		xCoRoutineRemoveFromEventList(&(mp->suspend_thread));

		/* enable interrupt */
		portENABLE_INTERRUPTS();
	}

	/* release allocated room */
	vPortFree(mp->start_address);
	vPortFree(mp);

	return osOK;
}

//  ==== Memory Management Functions ====
void *osMemoryMalloc(uint32_t nbytes)
{
	return pvPortMalloc((size_t)nbytes);
}

void osMemoryFree(void *ptr)
{
	vPortFree(ptr);
}

void *osMemoryCalloc(uint32_t nmemb, uint32_t nbytes)
{
	void *pvReturn;

	extern void *memset(void *s, int ch, size_t n);
	pvReturn = pvPortMalloc(nmemb * nbytes);
	if (pvReturn)
		memset(pvReturn, 0, nmemb * nbytes);
	return pvReturn;
}

//  ==== Message Queue Management Functions ====
osMessageQueueId_t osMessageQueueNew(uint32_t msg_count, uint32_t msg_size, const osMessageQueueAttr_t *attr)
{
	QueueHandle_t queueHandle = NULL;

	if (NULL == attr->cb_mem) {
#if (configSUPPORT_DYNAMIC_ALLOCATION == 1)
		queueHandle = xQueueCreate((UBaseType_t)msg_count,
										(UBaseType_t)msg_size);
		return queueHandle;
#endif /* configSUPPORT_DYNAMIC_ALLOCATION */
	} else if (NULL != attr->cb_mem) {
#if (configSUPPORT_STATIC_ALLOCATION == 1)
		queueHandle = xQueueCreateStatic((UBaseType_t)msg_count,
											(UBaseType_t)msg_size,
											(uint8_t *)attr->mq_mem,
											(StaticQueue_t *)attr->cb_mem);

		return queueHandle;
#endif /* configSUPPORT_STATIC_ALLOCATION */
	}
	return NULL;
}

const char *osMessageQueueGetName(osMessageQueueId_t mq_id)
{
#if (configQUEUE_REGISTRY_SIZE > 0)
	return pcQueueGetName((QueueHandle_t)mq_id);
#else
	return NULL;
#endif /* configQUEUE_REGISTRY_SIZE */
}

osStatus_t osMessageQueuePut(osMessageQueueId_t mq_id, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout)
{
	BaseType_t pxMessageTaskWoken;

	if (0 != msg_prio)
		return osErrorParameter;

	if (0 == timeout) {
		if (pdTRUE == xQueueSendFromISR((QueueHandle_t)mq_id,
										(void *)msg_ptr,
										(BaseType_t *)&pxMessageTaskWoken))
			return osOK;
		else
			return osErrorTimeout;
	} else {
		if (osWaitForever == timeout)
			timeout = (uint32_t)portMAX_DELAY;

		if (pdTRUE == xQueueSend((QueueHandle_t)mq_id,
								(void *)msg_ptr,
								(TickType_t)timeout))
			return osOK;
		else
			return osErrorTimeout;
	}
}

osStatus_t osMessageQueueGet(osMessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout)
{
	BaseType_t pxMessageTaskWoken;

	if (0 != msg_prio)
		return osErrorParameter;

	if (0 == timeout) {
		if (pdTRUE == xQueueReceiveFromISR((QueueHandle_t)mq_id,
											(void *)msg_ptr,
											(BaseType_t *)&pxMessageTaskWoken))
			return osOK;
		else
			return osErrorTimeout;
	} else {
		if (osWaitForever == timeout)
			timeout = (uint32_t)portMAX_DELAY;

		if (pdTRUE == xQueueReceive((QueueHandle_t)mq_id,
									(void *)msg_ptr,
									(TickType_t)timeout))
			return osOK;
		else
			return osErrorTimeout;
	}
}

uint32_t osMessageQueueGetCapacity(osMessageQueueId_t mq_id)
{
	// TODO To be adapted.
	return 0;
}

uint32_t osMessageQueueGetMsgSize(osMessageQueueId_t mq_id)
{
	// TODO To be adapted.
	return 0;
}

uint32_t osMessageQueueGetCount(osMessageQueueId_t mq_id)
{
	return uxQueueMessagesWaiting((QueueHandle_t)mq_id);
}

uint32_t osMessageQueueGetSpace(osMessageQueueId_t mq_id)
{
	return uxQueueSpacesAvailable((QueueHandle_t)mq_id);
}

osStatus_t osMessageQueueReset(osMessageQueueId_t mq_id)
{
	if (pdPASS == xQueueReset((QueueHandle_t)mq_id))
		return osOK;
	else
		return osErrorTimeout;
}

osStatus_t osMessageQueueDelete(osMessageQueueId_t mq_id)
{
	vQueueDelete((QueueHandle_t)mq_id);
	return osOK;
}

__WEAK void vApplicationMallocFailedHook(void)
{
	/* vApplicationMallocFailedHook() will only be called if
	 * configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	 * function that will get called if a call to pvPortMalloc() fails.
	 * pvPortMalloc() is called internally by the kernel whenever a task, queue,
	 * timer or semaphore is created.  It is also called by various parts of the
	 * demo application.  If heap_1.c or heap_2.c are used, then the size of the
	 * heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	 * FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	 * to query the size of free heap space that remains (although it does not
	 * provide information on how the remaining heap might be fragmented).
	 */
	taskDISABLE_INTERRUPTS();

	_exit(1);
}
/*-----------------------------------------------------------*/

__WEAK void vApplicationIdleHook(void)
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	 * to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	 * task.  It is essential that code added to this hook function never attempts
	 * to block in any way (for example, call xQueueReceive() with a block time
	 * specified, or call vTaskDelay()).  If the application makes use of the
	 * vTaskDelete() API function (as this demo application does) then it is also
	 * important that vApplicationIdleHook() is permitted to return to its calling
	 * function, because it is the responsibility of the idle task to clean up
	 * memory allocated by the kernel to any task that has since been deleted.
	 */
}
/*-----------------------------------------------------------*/

__WEAK void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
	(void) pcTaskName;
	(void) pxTask;

	/* Run time stack overflow checking is performed if
	 * configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.This hook
	 * function is called if a stack overflow is detected.
	 */
	taskDISABLE_INTERRUPTS();

	printf("ERROR Stack overflow on func: \r\n");
	printf("%s\r\n", pcTaskName);

	_exit(1);
}
/*-----------------------------------------------------------*/

__WEAK void vApplicationTickHook(void)
{
	/* The tests in the full demo expect some interaction with interrupts. */
}
/*-----------------------------------------------------------*/

__WEAK void vAssertCalled(void)
{
	taskDISABLE_INTERRUPTS();

	_exit(1);
}
