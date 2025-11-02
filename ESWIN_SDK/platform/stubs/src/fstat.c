/* See LICENSE of license details. */
#if defined ( __GNUC__ )
#include "eswin_sdk_soc.h"
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#undef errno
extern int errno;

__WEAK int _fstat(int file, struct stat *st)
{
	if ((file == STDOUT_FILENO) || (file == STDERR_FILENO)) {
		st->st_mode = S_IFCHR;
		return 0;
	} else {
		errno = EBADF;
		return -1;
	}
}
#else
#endif
