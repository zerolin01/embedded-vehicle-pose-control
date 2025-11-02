#include "eswin_sdk_soc.h"
#include "e320-internals.h"
#if defined ( __GNUC__ )
#include <reent.h>
#else
#endif

#if (CONFIG_RV_CORE_NUM > 1)
e320_exception_handler_table_t *_e320_exception_handler_table;
#else
e320_exception_handler_table_t _e320_exception_handler_table;
#endif

static void _e320_exception_default_handler(unsigned long *sp)
{
    __asm__ __volatile__("ebreak");
    while (1);
}

/* !!! Add ecall handler here !!! */
// extern void reg_store(uint32_t *address, uint32_t *regs);
// extern void reg_restore(uint32_t *address);
__WEAK void reg_store(unsigned long *regs) {};
__WEAK void reg_restore(void) {};

/* for test save the register in array */
static void ecall_from_machine(unsigned long regs[32])
{
    /* save register to corresponding address */
    reg_store(regs);

    for (int i = 0; i < 10; i++)
        asm volatile("nop");

    /* restore register from corresponding address */
    reg_restore();
}

static void ecall_from_load_fault(unsigned long regs[32])
{
    rv_csr_t mepc = __RV_CSR_READ(CSR_MEPC);
    if ((mepc & 0x03) == 0x03) {
        __RV_CSR_WRITE(CSR_MEPC, mepc + 4);
    } else {
        __RV_CSR_WRITE(CSR_MEPC, mepc + 2);
    }
}

static void ecall_from_store_fault(unsigned long regs[32])
{
    rv_csr_t mepc = __RV_CSR_READ(CSR_MEPC);
    if ((mepc & 0x03) == 0x03) {
        __RV_CSR_WRITE(CSR_MEPC, mepc + 4);
    } else {
        __RV_CSR_WRITE(CSR_MEPC, mepc + 2);
    }
}

void _e320_exception_init(void)
{
#if (CONFIG_RV_CORE_NUM > 1)
    uint32_t c;

    for (c = 0; c < CONFIG_RV_CORE_NUM; c++) {
        uint32_t num;

        for (num = 0; num < SYSTEM_MAX_EXCEPTION_NUM; num++)
            _e320_exception_handler_table[c][num] = _e320_exception_default_handler;
    }
#else
    uint32_t num;

    for (num = 0; num < SYSTEM_MAX_EXCEPTION_NUM; num++)
        _e320_exception_handler_table[num] = _e320_exception_default_handler;
    /* !!! register ecall handler !!! */
    register_exception_handler(LdFault_EXCn, ecall_from_load_fault);
    register_exception_handler(StAccessFault_EXCn, ecall_from_store_fault);
    register_exception_handler(MmodeEcall_EXCn, ecall_from_machine);
#endif
}

int register_exception_handler(EXCn_Type EXCn, exception_handler_fptr handler)
{
#if (CONFIG_RV_CORE_NUM > 1)
    _e320_exception_handler_table[__get_hartid()][IRQn] = handler;
#else
    _e320_exception_handler_table[EXCn] = handler;
#endif

    return 0;
}

void _e320_exception_call(unsigned long mcause, unsigned long sp)
{
    uint32_t excn = (uint32_t)(mcause & 0x00000fff);
#if defined ( __GNUC__ )
    __attribute__((unused)) struct _reent *local;

    SAVE_REENT_STRUCT(local);
#else
#endif
#if (CONFIG_RV_CORE_NUM > 1)
    _e320_exception_handler_table[__get_hartid()][excn](sp);
#else
    _e320_exception_handler_table[excn](&sp);
#endif
    RESTORE_REENT_STRUCT(local);
}
