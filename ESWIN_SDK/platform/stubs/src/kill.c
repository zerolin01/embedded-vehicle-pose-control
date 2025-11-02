/* See LICENSE of license details. */
#if defined ( __GNUC__ )
#include "eswin_sdk_soc.h"
#include <errno.h>
#undef errno
extern int errno;

__WEAK int _kill(int pid, int sig)
{
	errno = EINVAL;
	return -1;
}
#else
#endif
