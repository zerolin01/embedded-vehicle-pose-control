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
 * Title:        basic_struct.h header file
 *
 * Version 0.0.0
 *    Initial Release
 *---------------------------------------------------------------------------
 */

#ifndef __BASIC_STRUCT_H
#define __BASIC_STRUCT_H

// #include "basic_common.h"
// #include "basic_api.h"
#include "platform.h"
/*--------------------------------------------------------------------------*/
/* Error num */
typedef enum basic_error_e {
	BASICEINVAL	= 1,	/* Invalid parameter */
	BASICEBUSY	= 2,	/* Resource occupied */
	BASICENOSRC	= 3,	/* resource exhaustion */
	BASICENOMEM	= 4,	/* Out of memory */
	BASICEINIT	= 5,	/* Initialzation failed */
	BASICEREGRW	= 6,	/* Register read write failure */
	BASICTIMER	= 7		/* Timer failure */
} basic_error_t;

/*--------------------------------------------------------------------------*/
/* Interrupt */
/* Interrupt processing mode setting */
typedef enum intr_mode_e {
	INTR_MODE_CLINT_DIRECT = 0,	/* Set interrupt processing mode to direct */
	INTR_MODE_CLINT_VECTOR,		/* Set interrupt processing mode to vector */
#if CONFIG_RV_CLIC_PRESENT
	INTR_MODE_CLIC_DIRECT,		/* Set the interrupt processing mode to Clic direct */
	INTR_MODE_CLIC_VECTOR		/* Set interrupt processing mode to Clic vector */
#endif
} inte_mode_t;

/*--------------------------------------------------------------------------*/
/* Lock */
/* The definition of software lock structure supports both non reentrant lock and reentrant lock */
struct __lock {
	volatile uint32_t lock;			/* It is used to define whether it is locked or not. 0 is not locked and 1 is more than one lock */
	volatile int owner;				/* Lock holder ID */
	union {
		volatile uint32_t cnt;		/* It can be used in the state of re-entry lock, indicating the number of re-entry */
		volatile uint32_t mstatus;	/* It can't be used under re-entry lock, indicating the CPU status mstatus when grasping the lock */
	} u;							/* It is used to keep the locked private information */
};

#define container_of(ptr, type, member) \
	({ \
		void *__mptr = (void *) (ptr); \
		((type *) ((uint32_t)__mptr - offsetof(type, member))); \
	})

/*--------------------------------------------------------------------------*/
/* Memory */
/* Free block management structure. */
typedef struct basic_free_block_s {
	struct basic_free_block_s *mnext; /* Points to the next free block */
	uint32_t mlen;					/* This idle fast size */
} basic_free_block_t;

/* Allocated memory management structure */
typedef struct basic_allocated_block_s {
	basic_free_block_t *pPool; /* This allocated block size */
	uint32_t size;			 /* Points to the first address of the memory pool */
} basic_allocated_block_t;

/*--------------------------------------------------------------------------*/
/* Timer */

/* Timer timeout callback function, run in interrupt context, do not call blocking function, execution time as short as possible */
typedef void (*timer_callback)(void *);

typedef enum timer_flags_e {
	FLAG_ONE_SHOT,		/* Single timing. */
	FLAG_PERIODIC		/* Periodic timing. */
} timer_flags_t;

/* Timer structure */
typedef struct basic_timer_s {
	struct basic_timer_s *tnext; /* Next timer */
	timer_callback cb;		   /* Timer timeout callback function */
	void *arg;				   /* Timer timeout callback function parameters */
	uint32_t tick;			   /* The time interval that this timer triggers */
	uint32_t expired;		   /* Timer timeout tick */
	timer_flags_t flags;	   /* Timer type */
} basic_timer_t;

/*--------------------------------------------------------------------------*/
/* PMP */
/* HPM counter statement */
typedef enum basic_hpm_counter_s {
	BASIC_HPM_CYCLE = 0,
	BASIC_HPM_TIME,
	BASIC_HPM_MINSTRET,
	BASIC_HPM_COUNTER3,
	BASIC_HPM_COUNTER4,
	BASIC_HPM_COUNTER5,
	BASIC_HPM_COUNTER6,
	BASIC_HPM_COUNTER7,
	BASIC_HPM_COUNTER8,
	BASIC_HPM_COUNTER9,
	BASIC_HPM_COUNTER10,
	BASIC_HPM_COUNTER11,
	BASIC_HPM_COUNTER12,
	BASIC_HPM_COUNTER13,
	BASIC_HPM_COUNTER14,
	BASIC_HPM_COUNTER15,
	BASIC_HPM_COUNTER16,
	BASIC_HPM_COUNTER17,
	BASIC_HPM_COUNTER18,
	BASIC_HPM_COUNTER19,
	BASIC_HPM_COUNTER20,
	BASIC_HPM_COUNTER21,
	BASIC_HPM_COUNTER22,
	BASIC_HPM_COUNTER23,
	BASIC_HPM_COUNTER24,
	BASIC_HPM_COUNTER25,
	BASIC_HPM_COUNTER26,
	BASIC_HPM_COUNTER27,
	BASIC_HPM_COUNTER28,
	BASIC_HPM_COUNTER29,
	BASIC_HPM_COUNTER30,
	BASIC_HPM_COUNTER31
} basic_hpm_counter_t;

/* Definition of PMP operation permission */
typedef enum basic_pmp_perm_e {
	BASIC_PMP_READ	= 1,	/* Allow read operations */
	BASIC_PMP_WRITE	= 2,	/* Allow write operations */
	BASIC_PMP_EXEC	= 4,	/* Operation allowed */
	BASIC_PMP_LOCK	= 128,	/* PMP lock mode */
} basic_pmp_perm_t;

/* PMP type definition */
typedef enum basic_pmp_type_e {
	BASIC_PMP_OFF,	/* Turn off PMP */
	BASIC_PMP_TOR,	/* Set PMP to tor mode */
	BASIC_PMP_NA4,	/* Set PMP to Na4 mode */
	BASIC_PMP_NAPOT	/* Set PMP to nano mode */
} basic_pmp_type_t;

#endif /* __BASIC_STRUCT_H  */
