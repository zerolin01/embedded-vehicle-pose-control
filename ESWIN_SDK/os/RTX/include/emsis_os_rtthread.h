#ifndef __EMSIS_OS_RTTHREAD_H__
#define __EMSIS_OS_RTTHREAD_H__

/* Kernel includes. */
#include "rtthread.h"
#include "rthw.h"

#define CONFIG_STACK_CONFIG(data) (data)

typedef rt_uint8_t rtos_stacktype_t;

typedef struct rt_thread rtos_thread_t;
typedef struct rt_timer rtos_timer_t;
typedef struct rt_evevt rtos_event_t;
typedef struct rt_mutex rtos_mutex_t;
typedef struct rt_semaphore rtos_semaphore_t;
typedef struct rt_messagequeue rtos_messagequeue_t;
typedef struct rt_mempool rtos_mempool_t;

#endif /* __EMSIS_OS_FREERTOS_H__ */
