#ifndef _IRQ_H_
#define _IRQ_H_

#include <common/peripherals.h>
#include <common/device.h>

#define IRQ_BASE    (PERIPHERALS_BASE + 0xB000)

#define IRQ_BASIC_BASE_ID   64

#define INTC_IRQ_TOTAL      (64 + 8)

/* page 113 */
enum mske_irq_vector_id
{
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
    IRQ_ARM_TIMER       = (IRQ_BASIC_BASE_ID + 0),  /* 64 */
    IRQ_ARM_MAILBOX     = (IRQ_BASIC_BASE_ID + 1),  /* 65 */
    IRQ_ARM_DOORBELL0   = (IRQ_BASIC_BASE_ID + 2),  /* 66 */
    IRQ_ARM_DOORBELL1   = (IRQ_BASIC_BASE_ID + 3),  /* 67 */
    IRQ_ARM_GPU0_HALTED = (IRQ_BASIC_BASE_ID + 4),  /* 68 */
    IRQ_ARM_GPU1_HALTED = (IRQ_BASIC_BASE_ID + 5),  /* 69 */
    IRQ_ILLEGAL_ACCESS1 = (IRQ_BASIC_BASE_ID + 6),  /* 70 */
    IRQ_ILLEGAL_ACCESS0 = (IRQ_BASIC_BASE_ID + 7)   /* 71 */
};

typedef void (*mske_fn_interrupt_handler)(enum mske_irq_vector_id nirq, void *param);

typedef struct
{
    mske_fn_interrupt_handler irq_fn_handler; /* function that handler this interrupt */
    void *param; /* parameters passed to the IRQ handler */
} mske_interrupt_vector_t;

mske_ret_code_t interrupt_controller_init();
mske_ret_code_t register_interrupt(enum mske_irq_vector_id nirq, mske_fn_interrupt_handler irq_handler, void *param);
void enable_irq(enum mske_irq_vector_id vector);
void disable_irq(enum mske_irq_vector_id vector);
mske_ret_code_t enable_irqs(void);
mske_ret_code_t disable_irqs(void);
void irq_handler(void);

#endif /* _IRQ_H_ */
