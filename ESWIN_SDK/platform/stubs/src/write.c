/* See LICENSE of license details. */
#if defined ( __GNUC__ )
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

#include <eswin_sdk_soc.h>

// static struct __lock __lock___uart_write_mutex __attribute__((__section__(".amo"))) = {0, -1, {0}};

__WEAK void kitty_output(const char ch)
{
	basic_usart_write((uint8_t *)&ch, 1);
}

__WEAK ssize_t _write(int fd, const void *ptr, size_t len)
{
	if (!isatty(fd))
		return -1;

	//basic_lock_acquire(&__lock___uart_write_mutex);
	const uint8_t *writebuf = (const uint8_t *)ptr;

	for (size_t i = 0; i < len; i++) {
		if (writebuf[i] == '\n')
			kitty_output('\r');
		kitty_output(writebuf[i]);
	}
	//basic_lock_release(&__lock___uart_write_mutex);

	return len;
}
#else
#include <eswin_sdk_soc.h>

// static struct __lock __lock___uart_write_mutex __attribute__((__section__(".amo"))) = {0, -1, {0}};

__WEAK void kitty_output(const char ch)
{
    basic_usart_write((uint8_t *)&ch, 1);
}

__WEAK size_t __write(int handle, const unsigned char *buf, size_t bufSize)
{
    if (-1 == handle) {
        return -1;
    }

    if (NULL == buf) {
        return 0;
    }

    if (1 != handle && 2 != handle) {
        return -1;
    }

    //basic_lock_acquire(&__lock___uart_write_mutex);

    for (size_t i = 0; i < bufSize; i++) {
        if (buf[i] == '\n')
            kitty_output('\r');
        kitty_output(buf[i]);
    }
    //basic_lock_release(&__lock___uart_write_mutex);

    return bufSize;
}
#endif
