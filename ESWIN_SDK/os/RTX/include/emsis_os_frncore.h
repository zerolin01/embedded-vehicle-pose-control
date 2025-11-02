#ifndef __EMSIS_OS_FRNCORE_H__
#define __EMSIS_OS_FRNCORE__H__

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "croutine.h"

#define CONFIG_STACK_CONFIG(data) (data / 4)

typedef StackType_t rtos_stacktype_t;

typedef StaticTask_t rtos_thread_t;
typedef StaticTimer_t rtos_timer_t;
typedef StaticEventGroup_t rtos_event_t;
typedef StaticQueue_t rtos_messagequeue_t;
typedef StaticQueue_t rtos_mutex_t;
typedef StaticSemaphore_t rtos_semaphore_t;

void vApplicationMallocFailedHook(void);
void vApplicationIdleHook(void);
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName);
void vApplicationTickHook(void);
void vAssertCalled(void);

#endif /* __EMSIS_OS_FRNCORE_H__ */
