#include <common/delay.h>
#include "gpio.h"

/* page 90 of BCM2835-ARM-Peripherals.pdf */
/* These are relative offset to GPIO_BASE address */
enum
{
    /* function selector */
    GPIO_FSEL0      = 0x00,
    GPIO_FSEL1      = 0x04,
    GPIO_FSEL2      = 0x08,
    GPIO_FSEL3      = 0x0C,
    GPIO_FSEL4      = 0x10,
    GPIO_FSEL5      = 0x14,

    /* set and clear pin output */
    GPIO_SET0       = 0x1C,
    GPIO_SET1       = 0x20,
    GPIO_CLR0       = 0x28,
    GPIO_CLR1       = 0x2C,

    /* pulldown control registers */
    GPIO_PUD        = 0x94, /* control all pins */
    GPIO_PUDCLK0    = 0x98, /* enable clock 0 */
    GPIO_PUDCLK1    = 0x9C, /* enable clock 1 */
};

static inline volatile u32 * get_gpio_address(u32 offset)
{
    return get_register_address(GPIO_BASE + offset);
}

void set_gpio_function(u32 pin, enum GPIO_Funcs fn)
{
    volatile u32 *fsel = &get_gpio_address(GPIO_FSEL0)[pin / 10];
    u32 shift = (pin % 10) * 3;
    u32 mask = ~(7 << shift);
    *fsel = (*fsel & mask) | (fn << shift);
}

void set_gpio_state(u32 pin, bool state)
{
    /* set or clear */
    get_gpio_address(state ? GPIO_SET0 
            : GPIO_CLR0)[pin / 32] = 1 << (pin % 32);
}

void set_gpio_pull_up_down(u32 pin, enum PullUpDown fn)
{
    volatile u32 *pud = get_gpio_address(GPIO_PUD);
    *pud = fn;
    delay(150);

    volatile u32 *clock = get_gpio_address( (pin < 32) ? GPIO_PUDCLK0 : GPIO_PUDCLK1);
    *clock = (1 << (pin % 32));
    delay(150);

    /* clear action and remove clock */
    *pud = OFF;
    *clock = 0;
}
