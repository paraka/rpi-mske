#include <common/io.h>
#include <common/peripherals.h>
#include <arch/arm/processor.h>
#include <exceptions/exceptions.h>

extern u32 exception_vector[];

static const char *exception[] = {
    "Reset",
    "Undefined instruction",
    "Supervisor Call",
    "Prefetch Abort",
    "Data Abort",
    "Hypervisor",
    "IRQ",
    "FIQ"
};

static void set_vbar(u32 *base_address)
{
    asm volatile ("mcr p15, 0, %[base_address], c12, c0, 0"
                    :: [base_address] "r" (base_address));
}

void init_exceptions(void)
{
    set_vbar(exception_vector);
}

void handler_reset(mske_context_t *ctx, u32 num)
{
    printk("%s: Regs @ %p\n", exception[num], ctx);
    dump_registers(ctx);
}

void handler_undef(mske_context_t *ctx, u32 num)
{
    printk("%s: Regs @ %p\n", exception[num], ctx);
    dump_registers(ctx);
    printk("%s @ %08X = %08X\n", exception[num], ctx->lr, mmio_read(ctx->lr));
    /* skip instruction */
    ctx->lr += 4;
}

void handler_svc(mske_context_t *ctx, u32 num)
{
    printk("%s: Regs @ %p\n", exception[num], ctx);
    dump_registers(ctx);
}

void handler_prefetch_abort(mske_context_t *ctx, u32 num)
{
    printk("%s: Regs @ %p\n", exception[num], ctx);
    dump_registers(ctx);
}

void handler_data_abort(mske_context_t *ctx, u32 num)
{
    printk("%s: Regs @ %p\n", exception[num], ctx);
    dump_registers(ctx);
}

void handler_hypervisor_trap(mske_context_t *ctx, u32 num)
{
    printk("%s: Regs @ %p\n", exception[num], ctx);
    dump_registers(ctx);
}
