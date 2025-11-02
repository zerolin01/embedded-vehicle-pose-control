#include "eswin_sdk_soc.h"
#ifndef CONFIG_CUSTOM_REENT
#include <reent.h>
#include <string.h>

/* As an exception handler may also use the library, it is better to use
 * a different re-entrancy data structure for the exceptions.
 * This data structure is configured here and as part of the exception
 * handler (kitty_exception_handler) temporarily replaces the software's
 * impure data pointer.
 *
 * During initialization, the libraries standard _impure_data and the exception
 * impure data (_exception_impure_data) are initialized. Afterwards,
 * the current value _current_impure_ptr is set to _impure_ptr.
 *
 * At runtime __getreent is called to return the current reentrancy pointer,
 * which is stored in _current_impure_ptr.
 *
 * In the kitty_exception_handler the _current_impure_ptr is set to point to
 * _exception_impure_ptr. After the exception handler returned, it is set back
 * to _impure_ptr.
 */

/* Link in the external impure_data structure */
extern struct _reent *__ATTRIBUTE_IMPURE_PTR__ _impure_ptr;

#if (CONFIG_RV_CORE_NUM > 1)
struct _reent **_kitty_impure_ptr;
struct _reent **_kitty_exception_impure_ptr;
struct _reent **_kitty_current_impure_ptr;
#else
struct _reent *__ATTRIBUTE_IMPURE_PTR__ _kitty_impure_ptr;
/* Create exception impure data structure */
struct _reent _kitty_exception_impure_data = _REENT_INIT(_kitty_exception_impure_data);
/* Link to the exception impure data structure */
struct _reent *__ATTRIBUTE_IMPURE_PTR__ _kitty_exception_impure_ptr = &_kitty_exception_impure_data;
/* Link to the currently used data structure. */
struct _reent *__ATTRIBUTE_IMPURE_PTR__ _kitty_current_impure_ptr;
#endif

/**
 * \brief  Stub function to get reent for current thread
 * \details
 * This stub function will be used by newlib when executing reentrancy functions
 */
struct _reent *__getreent(void)
{
#if (CONFIG_RV_CORE_NUM > 1)
	return _kitty_current_impure_ptr[__get_hartid()];
#else
	return _kitty_current_impure_ptr;
#endif
}

/**
 * \brief  initialize impure data for normal and exception executing enviroment
 * \details
 * for multicore case, treat normal state and exception state of each core
 * as a different thread, so create a impure data for each state of each core
 */
void _kitty_libc_impure_init(void)
{
#if (CONFIG_RV_CORE_NUM > 1)
	uint32_t c;

	_kitty_impure_ptr = _sbrk_r(0, sizeof(struct _reent *) * CONFIG_RV_CORE_NUM);
	_kitty_exception_impure_ptr = _sbrk_r(0, sizeof(struct _reent *) * CONFIG_RV_CORE_NUM);
	_kitty_current_impure_ptr = _sbrk_r(0, sizeof(struct _reent *) * CONFIG_RV_CORE_NUM);

	_kitty_impure_ptr[0] = _impure_ptr;
	_REENT_INIT_PTR(_impure_ptr);
	for (c = 1; c < CONFIG_RV_CORE_NUM; c++) {
		_kitty_impure_ptr[c] = _sbrk_r(0, sizeof(struct _reent));
		_REENT_INIT_PTR(_kitty_impure_ptr[c]);
	}

	for (c = 0; c < CONFIG_RV_CORE_NUM; c++) {
		_kitty_exception_impure_ptr[c] = _sbrk_r(0, sizeof(struct _reent));
		_REENT_INIT_PTR(_kitty_exception_impure_ptr[c]);
	}

	for (c = 0; c < CONFIG_RV_CORE_NUM; c++)
		_kitty_current_impure_ptr[c] = _kitty_impure_ptr[c];
#else
	// Initialize both impure data structures
	_REENT_INIT_PTR(_impure_ptr);
	_REENT_INIT_PTR(_kitty_exception_impure_ptr);
	// Use the standard impure ptr during normal software run
	_kitty_impure_ptr = _impure_ptr;
	// Set current to standard impure pointer
	_kitty_current_impure_ptr = _impure_ptr;
#endif
}

struct _reent *_kitty_libc_getreent(void)
{
#if (CONFIG_RV_CORE_NUM > 1)
	return _kitty_current_impure_ptr[__get_hartid()];
#else
	return _kitty_current_impure_ptr;
#endif
}

void _kitty_libc_setreent(struct _reent *reent)
{
#if (CONFIG_RV_CORE_NUM > 1)
	_kitty_current_impure_ptr[__get_hartid()] = reent;
#else
	_kitty_current_impure_ptr = reent;
#endif
}

struct _reent *_kitty_libc_getreent_by_type(int type)
{
	if (type >= MAX_THREAD)
		return NULL;

#if (CONFIG_RV_CORE_NUM > 1)
	if (type == NORMAL_THREAD)
		return _kitty_impure_ptr[__get_hartid()];
	else if (type == EXCEPTION_THREAD)
		return _kitty_exception_impure_ptr[__get_hartid()];
	else
		return NULL;
#else
	if (type == NORMAL_THREAD)
		return _kitty_impure_ptr;
	else if (type == EXCEPTION_THREAD)
		return _kitty_exception_impure_ptr;
	else
		return NULL;
#endif
}
#endif
