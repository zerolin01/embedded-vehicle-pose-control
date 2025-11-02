/* See LICENSE of license details. */
#include "eswin_sdk_soc.h"
#if defined ( __GNUC__ )
#include <sys/stat.h>
#include <errno.h>

#undef errno
extern int errno;

__WEAK int _wait_r(struct _reent *ptr, int *status)
{
	errno = ECHILD;
	return -1;
}
#else
#endif
