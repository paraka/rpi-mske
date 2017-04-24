#include <common/defs.h>
#include <irq/irq.h>

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

void enable_irqs(void)
{
}

void disable_irqs(void)
{
}

void enable_irq(enum IRQ_VECTOR vector)
{
    UNUSED(vector);
}

void disable_irq(enum IRQ_VECTOR vector)
{
    UNUSED(vector);
}
