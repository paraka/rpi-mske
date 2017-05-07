#include <timer/arm_timer.h>

/** 16-bit counters - 1 : 23-bit counter */
#define ARMTIMER_CTRL_23BIT         ( 1 << 1 )

#define ARMTIMER_CTRL_PRESCALE_1    ( 0 << 2 )
#define ARMTIMER_CTRL_PRESCALE_16   ( 1 << 2 )
#define ARMTIMER_CTRL_PRESCALE_256  ( 2 << 2 )

/** Timer interrupt disabled - 1 : Timer interrupt enabled */
#define ARMTIMER_CTRL_INT_ENABLE    ( 1 << 5 )
#define ARMTIMER_CTRL_INT_DISABLE   ( 0 << 5 )

/** Timer disabled - 1 : Timer enabled */
#define ARMTIMER_CTRL_ENABLE        ( 1 << 7 )
#define ARMTIMER_CTRL_DISABLE       ( 0 << 7 )

enum ARM_TIMER_REGS
{
    LOAD                    = (ARM_TIMER_BASE + 0x0),
    VALUE                   = (ARM_TIMER_BASE + 0x4),
    CONTROL                 = (ARM_TIMER_BASE + 0x8),
    IRQ_CLEAR               = (ARM_TIMER_BASE + 0xC),
    RAW_IRQ                 = (ARM_TIMER_BASE + 0x10),
    MASKED_IRQ              = (ARM_TIMER_BASE + 0x14),
    RELOAD                  = (ARM_TIMER_BASE + 0x18),
    PRE_DIVIDER             = (ARM_TIMER_BASE + 0x1C),
    FREE_RUNNING_COUNTER    = (ARM_TIMER_BASE + 0x20)  
};

/* Setup the system timer interrupt */
/* Timer frequency = Clk/256 * 0x400 */
void setup_arm_timer()
{
    mmio_write(LOAD, 0x400);
    mmio_write(CONTROL, (ARMTIMER_CTRL_23BIT 
            | ARMTIMER_CTRL_ENABLE | ARMTIMER_CTRL_INT_ENABLE | ARMTIMER_CTRL_PRESCALE_256));
}

void arm_timer_clear_irq()
{
    mmio_write(IRQ_CLEAR, 1);
}
