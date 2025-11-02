/* See LICENSE of license details. */
#if defined ( __GNUC__ )
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include "eswin_sdk_soc.h"

// #define UART_AUTO_ECHO
//
__WEAK ssize_t _read(int fd, void *ptr, size_t len)
{
	if (fd != STDIN_FILENO)
		return -1;

	uint8_t *readbuf = (uint8_t *)ptr;

	basic_usart_read(readbuf, 1);
#ifdef UART_AUTO_ECHO
	basic_usart_write(readbuf, 1);
#endif
	return 1;
}
#else

#include "eswin_sdk_soc.h"

// #define UART_AUTO_ECHO

__WEAK size_t __read(int handle, unsigned char *buf, size_t bufLen)
{
        if (handle != 0) {
            return -1;
        }

        if (NULL == buf) {
            return 0;
        }

	basic_usart_read(buf, bufLen);
#ifdef UART_AUTO_ECHO
	basic_usart_write(buf, bufLen);
#endif
	return 1;
}
#endif
