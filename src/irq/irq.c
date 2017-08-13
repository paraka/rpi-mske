#include <common/defs.h>
#include <common/io.h>
#include <arch/arm/processor.h>
#include <irq/irq.h>

static mske_interrupt_vector_t mske_irq_vector_table[INTC_IRQ_TOTAL];

/* page 112 */
enum IRQ_Register
{
    IRQ_BASIC_PENDING = (IRQ_BASE + 0x200),
    IRQ_PENDING1      = (IRQ_BASE + 0x204),
    IRQ_PENDING2      = (IRQ_BASE + 0x208),
    IRQ_FIQ_CONTROL   = (IRQ_BASE + 0x20C),
    IRQ_ENABLE1       = (IRQ_BASE + 0x210),
    IRQ_ENABLE2       = (IRQ_BASE + 0x214),
    IRQ_ENABLE_BASIC  = (IRQ_BASE + 0x218),
    IRQ_DISABLE1      = (IRQ_BASE + 0x21C),
    IRQ_DISABLE2      = (IRQ_BASE + 0x220),
    IRQ_DISABLE_BASIC = (IRQ_BASE + 0x224)
};

/**
 *  Enables all IRQ's in the CPU's CPSR register.
 */
static inline void irq_enable() 
{
    asm volatile("mrs     r0,cpsr");      /* Read in the cpsr register */
    asm volatile("bic     r0,r0,#0x80");  /* Clear bit 8, (0x80) -- Causes IRQs to be enabled. */
    asm volatile("msr     cpsr_c, r0");   /* Write it back to the CPSR register */
}

static inline void irq_disable() 
{
    asm volatile("mrs     r0,cpsr");      /* Read in the cpsr register */
    asm volatile("orr     r0,r0,#0x80");  /* Set bit 8, (0x80) -- Causes IRQs to be disabled */
    asm volatile("msr     cpsr_c, r0");   /* Write it back to the CPSR register */
}

#define clz(a) \
({ unsigned long __value, __arg = (a); \
     asm ("clz\t%0, %1": "=r" (__value): "r" (__arg)); \
          __value; })


/* default irq_handler */
static void default_irq_handler(void *param) 
{
    UNUSED(param);
    printk("%s\n", __func__);
}

mske_ret_code_t interrupt_controller_init()
{
    int i = 0;

    for(i=0; i<INTC_IRQ_TOTAL; i++)
    {
        mske_irq_vector_table[i].irq_fn_handler = default_irq_handler;
        mske_irq_vector_table[i].param = (void *)0;
    }

    return MSKE_SUCESS;
}

mske_ret_code_t register_interrupt(enum mske_irq_vector_id nirq, mske_fn_interrupt_handler irq_handler, void *param)
{
    printk("%s: %d (0x%08X)\n", __func__, nirq, &irq_handler);

    irq_disable();
    mske_irq_vector_table[nirq].irq_fn_handler = irq_handler;
    mske_irq_vector_table[nirq].param = param;
    irq_enable();

    return MSKE_SUCESS;
}

mske_ret_code_t enable_irqs(void)
{
    irq_enable();
    
    return MSKE_SUCESS;
}

mske_ret_code_t disable_irqs(void)
{
    irq_disable();

    return MSKE_SUCESS;
}

void enable_irq(enum mske_irq_vector_id vector)
{
    enum IRQ_Register reg = 
            (vector < 32) ? IRQ_ENABLE1
                          : ((vector < 64) ? IRQ_ENABLE2 : IRQ_ENABLE_BASIC);
    
    u32 mask = (1 << (vector % 32)); 
    mmio_write(reg, mask);
}

void disable_irq(enum mske_irq_vector_id vector)
{
    enum IRQ_Register reg = 
            (vector < 32) ? IRQ_DISABLE1
                          : ((vector < 64) ? IRQ_DISABLE2 : IRQ_DISABLE_BASIC);
    
    u32 mask = (1 << (vector % 32)); 
    mmio_write(reg, mask);
}

/**
 *  This is the global IRQ handler
 * It is based on the assembler code found in the Broadcom datasheet.
 **/
void handler_irq(mske_context_t *ctx, u32 num)
{
    register u32 ul_masked_status;
    register u32 nirq;

    UNUSED(ctx);
    UNUSED(num);

    printk("%s: Regs @ %p Num: %d\n", "IRQ", ctx,  num);

    ul_masked_status = mmio_read(IRQ_BASIC_PENDING);

    /* Bits 7 through 0 in IRQ_BASIC_PENDING represent interrupts 64-71 */
    if (ul_masked_status & 0xFF)
    {
        nirq = 64 + 31;
    }
    /* Bit 8 in IRQ_BASIC_PENDING indicates interrupts in IRQ_PENDING1 (interrupts 31-0) */
    else if (ul_masked_status & 0x100)
    {
        ul_masked_status = mmio_read(IRQ_PENDING1);
        nirq = 0 + 31;
    }
    /* Bit 9 in IRQ_BASIC_PENDING indicates interrupts in IRQ_PENDING2 (interrupts 63-32) */
    else if (ul_masked_status & 0x200)
    {
        ul_masked_status = mmio_read(IRQ_PENDING2);
        nirq = 32 + 31;
    }
    else
    {
        /* No interrupt avaiable */
        return;
    }

    /* Keep only least significant bit, in case multiple interrupts have occured */
    ul_masked_status &= -ul_masked_status;

    /* Some magic to determine number of interrupt to serve */
    nirq = nirq - clz(ul_masked_status);

    /* Call interrupt handler */
    mske_irq_vector_table[nirq].irq_fn_handler(mske_irq_vector_table[nirq].param);
}

void handler_fiq(mske_context_t *ctx, u32 num)
{
    UNUSED(num);
    printk("%s: Regs @ %p\n", "FIQ", ctx);
    dump_registers(ctx);
}
