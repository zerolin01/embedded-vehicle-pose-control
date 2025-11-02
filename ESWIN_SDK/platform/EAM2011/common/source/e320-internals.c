#include <stddef.h>
#include <stdint.h>
#if defined(__GNUC__)
#include <reent.h>
#else
#endif
#include "e320-internals.h"
#include "eswin_sdk_soc.h"

#include <stddef.h>
#include <stdint.h>

#if (CONFIG_RV_CLIC_NUM == 1)
static CLIC_Type clicInst         = {{CLIC_BASE(0)}};
static MSIP_Type msipInst         = {{MSIP_BASE(0)}};
static SysTimer_Type systimerInst = {{SYSTIMER_BASE(0)}};
#elif (CONFIG_RV_CLIC_NUM == 2)
static CLIC_Type clicInst         = {{CLIC_BASE(0), CLIC_BASE(1)}};
static MSIP_Type msipInst         = {{MSIP_BASE(0), MSIP_BASE(1)}};
static SysTimer_Type systimerInst = {{SYSTIMER_BASE(0), SYSTIMER_BASE(1)}};
#else
#error "Not supported CLIC NUM"
#endif

CLIC_Type *clic         = &clicInst;
MSIP_Type *msip         = &msipInst;
SysTimer_Type *systimer = &systimerInst;

/*
 *
 * for libc printf
 */

static void _core_clic_init(void)
{
}

static void _core_systimer_init(void)
{
}

void _core_init(void)
{
#ifdef CONFIG_RV_CLIC_PRESENT
    _core_clic_init();
#endif
#ifdef CONFIG_RV_CLIC_PRESENT
    _core_systimer_init();
#endif
}

#if (CONFIG_RV_CORE_NUM > 1)
uint8_t **_e320_stack_core;
#endif
uint8_t *_e320_stack_top;
uint8_t *_e320_stack_bottom;
size_t _e320_stack_size = 0;

__WEAK void _e320_board_init_single(void)
{
}

__WEAK void _e320_soc_init_single(void)
{
}

__WEAK void _e320_board_init_multicore(void)
{
}

__WEAK void _e320_soc_init_multicore(void)
{
}

void _e320_init_pre(void)
{
#if (CONFIG_RV_CORE_NUM > 1)
    uint32_t c;

    // Initialize stacks
    _e320_stack_core    = _sbrk_r(0, sizeof(uint8_t *) * CONFIG_RV_CORE_NUM);
    _e320_stack_core[0] = _e320_stack_top;

    for (c = 1; c < CONFIG_RV_CORE_NUM; c++)
        _e320_stack_core[c] = _e320_stack_core[c - 1] - _e320_stack_size;

    size_t exc_size = sizeof(void *) * CONFIG_RV_CORE_NUM * SYSTEM_MAX_EXCEPTION_NUM;

    _e320_exception_handler_table = _sbrk_r(0, exc_size);

    size_t int_size     = sizeof(void *) * CONFIG_RV_CORE_NUM * SOC_INT_MAX;
    size_t intdata_size = sizeof(void *) * CONFIG_RV_CORE_NUM * SOC_INT_MAX;

    _kitty_interrupt_handler_table          = _sbrk_r(0, int_size);
    _kitty_interrupt_handler_data_ptr_table = _sbrk_r(0, intdata_size);

#endif
    /* init atomic lock pool */
    /* apool_init(); */
    basic_apool_init();
}

void _e320_init_single(void)
{
    /* TODO: Add your own initialization code here, called before main */
    /* __ICACHE_PRESENT and __DCACHE_PRESENT are defined in e320.h */
#ifdef CONFIG_RV_ICACHE_PRESENT
    EnableICache();
#endif
#ifdef CONFIG_RV_DCACHE_PRESENT
    EnableDCache();
#endif

    /* Modules reset */
    // writel((uint32_t)(CRG_BASE + 0XB0), (uint32_t)0x00);
    // writel((uint32_t)(CRG_BASE + 0XB0), (uint32_t)0xFFFFFFFF);
    // writel((uint32_t)(CRG_BASE + 0XB4), (uint32_t)0x00);
    // writel((uint32_t)(CRG_BASE + 0XB4), (uint32_t)0xFFFFFFFF);

    /* TODO release reset */
    // OS_IoWrite32((void *)0x4008014C, 0x1);
    // OS_IoWrite32((void *)0x40080230, 0xffffffff);
    // OS_IoSetBit32((void *)0x40080234, 0xf);
    // TODO
    // SystemCoreClock = e320_get_cpu_freq();

#if 0 // TODO
	/* Initialize and configure clocks
	*     -    see clock manager component for details
	*/
	CLOCK_SYS_Init(g_pstClockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
					g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
	CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);
#endif

    /* Initialize exception default handlers */
    _e320_exception_init();
    /* _e320_interrupt_init(); */
    basic_intr_init();

    __enable_external_irq();

    /* enable globe int */
    basic_intr_enable(true);

    /* Give Board chance to do single init */
    _e320_board_init_single();
}

static void _e320_clic_init(void)
{
    CLIC_SetMth(0);

    /* Config interrupt level and interrupt priority 1 interrupt level and 32 interrupt priority */
    CLIC_SetCfgNlbits(0);
}

void _e320_init_multicore(void)
{

    _e320_clic_init();

    /* Give Board chance to do multicore init */
    _e320_board_init_multicore();
}

/**
 * \brief _init function called in __libc_init_array()
 * \details
 * This `__libc_init_array()` function is called during startup code,
 * user need to implement this function, otherwise when link it will
 * error init.c:(.text.__libc_init_array+0x26): undefined reference to `_init'
 * \note
 * Please use \ref _premain_init function now
 */
void _init(void)
{
    /* Don't put any code here, please use _premain_init now */
}

/**
 * \brief _fini function called in __libc_fini_array()
 * \details
 * This `__libc_fini_array()` function is called when exit main.
 * user need to implement this function, otherwise when link it will
 * error fini.c:(.text.__libc_fini_array+0x28): undefined reference to `_fini'
 * \note
 * Please use \ref _postmain_fini function now
 */
void _fini(void)
{
    /* Don't put any code here, please use _postmain_fini now */
}
