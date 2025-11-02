/* See LICENSE of license details. */
#if defined ( __GNUC__ )
#include "eswin_sdk_soc.h"
#include <unistd.h>

__WEAK int _isatty(int fd)
{
	return 1;
}
#else
#endif
