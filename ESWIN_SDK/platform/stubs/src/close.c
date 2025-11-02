/* See LICENSE of license details. */
#if defined ( __GNUC__ )
#include "eswin_sdk_soc.h"
#include <errno.h>

#undef errno
extern int errno;

__WEAK int _close(int fd)
{
	errno = EBADF;
	return -1;
}
#else
#endif