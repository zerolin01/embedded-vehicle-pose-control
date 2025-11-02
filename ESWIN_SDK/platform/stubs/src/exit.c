/* See LICENSE of license details. */
#include "eswin_sdk_soc.h"

__WEAK void _exit(int fd)
{
	while (1)
		__WFI();
}
