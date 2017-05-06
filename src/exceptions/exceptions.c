#include <common/io.h>
#include <common/peripherals.h>
#include <arch/arm/processor.h>

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
