/* See LICENSE of license details. */
#include "eswin_sdk_soc.h"
#include <stdint.h>
#include <stddef.h>
#if defined ( __GNUC__ )
#include <unistd.h>
#else
#endif
char *_heap_end;

extern char _heap_start[];

/* Determine whether the heap MEM is initialized. */
static int heap_mm_init;

__WEAK void *_sbrk(ptrdiff_t incr)
{
	if (heap_mm_init == 0) {
		basic_pool_init(_heap_start, _heap_end - _heap_start);
		heap_mm_init = 1;
	}

	return basic_malloc(_heap_start, incr);
}
