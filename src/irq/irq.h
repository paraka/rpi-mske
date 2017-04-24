#ifndef _IRQ_H_
#define _IRQ_H_

#include <common/peripherals.h>

#define IRQ_BASE    (PERIPHERALS_BASE + 0xB000)

void enable_irqs(void);
void disable_irqs(void);

/* pág 113 */
enum IRQ_VECTOR
{
    IRQ_ARM_TIMER       = 0,
    IRQ_ARM_MAILBOX     = 1,
    IRQ_ARM_DOORBELL0   = 2,
    IRQ_ARM_DOORBELL1   = 3,
    IRQ_ARM_GPU0_HALTED = 4,
    IRQ_ARM_GPU1_HALTED = 5,
    IRQ_ILLEGAL_ACCESS1 = 6,
    IRQ_ILLEGAL_ACCESS0 = 7,
    IRQ_AUX             = 29,
    IRQ_I2C_SPI_SLV     = 43,    
    IRQ_PWA0            = 45,
    IRQ_PWA1            = 46,
    IRQ_SMI             = 48,
    IRQ_GPIO0           = 49,
    IRQ_GPIO1           = 50,
    IRQ_GPIO2           = 51,
    IRQ_GPIO3           = 52,
    IRQ_I2C             = 53,
    IRQ_SPI             = 54,
    IRQ_PCM             = 55,
    IRQ_UART            = 57,
};

void enable_irq(enum IRQ_VECTOR vector);
void disable_irq(enum IRQ_VECTOR vector);

#endif /* _IRQ_H_ */
