/* See LICENSE of license details. */
#if defined ( __GNUC__ )
#include <errno.h>
#include <sys/time.h>
#include "eswin_sdk_soc.h"

__WEAK int _gettimeofday(struct timeval *tp, void *tzp)
{
	uint64_t cycles = get_cycles();

	tp->tv_sec = cycles / SystemCoreClock;
	tp->tv_usec = (cycles % SystemCoreClock) * 1000000 / SystemCoreClock;
	return 0;
}
#else
#endif
