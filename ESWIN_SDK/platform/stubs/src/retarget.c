
/* dummy lock routines and static locks for single-threaded apps */
#if defined ( __GNUC__ )
#include "eswin_sdk_soc.h"
#ifndef CONFIG_CUSTOM_LOCK
#include <sys/lock.h>
#include <stdio.h>
#include <stdlib.h>

struct __lock __lock___sinit_recursive_mutex __attribute__((__section__(".amo"))) = {0, -1, {0}};
struct __lock __lock___sfp_recursive_mutex __attribute__((__section__(".amo"))) = {0, -1, {0}};
struct __lock __lock___atexit_recursive_mutex __attribute__((__section__(".amo"))) = {0, -1, {0}};
struct __lock __lock___at_quick_exit_mutex __attribute__((__section__(".amo"))) = {0, -1, {0}};
struct __lock __lock___malloc_recursive_mutex __attribute__((__section__(".amo"))) = {0, -1, {0}};
struct __lock __lock___env_recursive_mutex __attribute__((__section__(".amo"))) = {0, -1, {0}};
struct __lock __lock___tz_mutex __attribute__((__section__(".amo"))) = {0, -1, {0}};
struct __lock __lock___dd_hash_mutex __attribute__((__section__(".amo"))) = {0, -1, {0}};
struct __lock __lock___arc4random_mutex __attribute__((__section__(".amo"))) = {0, -1, {0}};
struct __lock __lock___uart_write_mutex __attribute__((__section__(".amo"))) = {0, -1, {0}};
/* struct __lock __lock___apool_mutex __attribute__((__section__(".amo"))) = {0, -1, 0}; */

void __retarget_lock_init(_LOCK_T *lock)
{
	basic_lock_init(lock);
}

void __retarget_lock_init_recursive(_LOCK_T *lock)
{
	basic_lock_init(lock);
}

void __retarget_lock_close(_LOCK_T lock)
{
	basic_lock_close(lock);
}

void __retarget_lock_close_recursive(_LOCK_T lock)
{
	basic_lock_close(lock);
}

void __retarget_lock_acquire(_LOCK_T lock)
{
	basic_lock_acquire(lock);
}

void __retarget_lock_acquire_recursive(_LOCK_T lock)
{
	basic_lock_acquire_recursive(lock);
}

int __retarget_lock_try_acquire(_LOCK_T lock)
{
	return basic_lock_try_acquire(lock);
}

int __retarget_lock_try_acquire_recursive(_LOCK_T lock)
{
	return basic_lock_try_acquire_recursive(lock);
}

void __retarget_lock_release(_LOCK_T lock)
{
	basic_lock_release(lock);
}

void __retarget_lock_release_recursive(_LOCK_T lock)
{
	basic_lock_release_recursive(lock);
}
#endif
#else
#endif
