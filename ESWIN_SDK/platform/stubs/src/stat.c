/* See LICENSE of license details. */
#if defined ( __GNUC__ )
#include "eswin_sdk_soc.h"
#include <sys/stat.h>

__WEAK int _stat(char *file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}
#else
#endif
