#include <common/delay.h>
#include <gpio/gpio.h>

/* page 90 of BCM2835-ARM-Peripherals.pdf */
/* These are relative offset to GPIO_BASE address */
enum
{
    /* function selector */
    GPIO_FSEL0      = (GPIO_BASE + 0x00),
    GPIO_FSEL1      = (GPIO_BASE + 0x04),
    GPIO_FSEL2      = (GPIO_BASE + 0x08),
    GPIO_FSEL3      = (GPIO_BASE + 0x0C),
    GPIO_FSEL4      = (GPIO_BASE + 0x10),
    GPIO_FSEL5      = (GPIO_BASE + 0x14),

    /* set and clear pin output */
    GPIO_SET0       = (GPIO_BASE + 0x1C),
    GPIO_SET1       = (GPIO_BASE + 0x20),
    GPIO_CLR0       = (GPIO_BASE + 0x28),
    GPIO_CLR1       = (GPIO_BASE + 0x2C),

    /* pulldown control registers */
    GPIO_PUD        = (GPIO_BASE + 0x94), /* control all pins */
    GPIO_PUDCLK0    = (GPIO_BASE + 0x98), /* enable clock 0 */
    GPIO_PUDCLK1    = (GPIO_BASE + 0x9C), /* enable clock 1 */
};

void set_gpio_function(u32 pin, enum GPIO_Funcs fn)
{
    u32 reg = pin / 10;
    u32 shift = (pin % 10) * 3;
    u32 mask = ~(7 << shift);
    volatile u32 *addr = &((volatile u32 *)GPIO_FSEL0)[reg];
    u32 old_value = mmio_read(*addr);
    u32 new_value = (old_value & mask) | (fn << shift);
    mmio_write(*addr, new_value);
}

void set_gpio_state(u32 pin, bool state)
{
    u32 reg = pin / 32;
    
    ((volatile u32 *)(state ? GPIO_SET0 : GPIO_CLR0))[reg] = (1 << (pin % 32));
}

/* 
 * The GPIO Pull-up/down Clock Registers control the actuation of internal pull-downs on
 * the respective GPIO pins. These registers must be used in conjunction with the GPPUD
 * register to effect GPIO Pull-up/down changes. The following sequence of events is
 * required:
 *  1. Write to GPPUD to set the required control signal (i.e. Pull-up or Pull-Down or neither
 *     to remove the current Pull-up/down)
 *  2. Wait 150 cycles – this provides the required set-up time for the control signal
 *  3. Write to GPPUDCLK0/1 to clock the control signal into the GPIO pads you wish to
 *     modify – NOTE only the pads which receive a clock will be modified, all others will
 *     retain their previous state.
 *  4. Wait 150 cycles – this provides the required hold time for the control signal
 *  5. Write to GPPUD to remove the control signal
 *  6. Write to GPPUDCLK0/1 to remove the clock
*/
void set_gpio_pull_up_down(u32 pin, enum PullUpDown fn)
{
    mmio_write(GPIO_PUD, fn);
    delay(150);

    volatile u32 *pud_addr = ((pin < 32) ? (volatile u32 *)GPIO_PUDCLK0 : (volatile u32 *)GPIO_PUDCLK1);

    mmio_write(*pud_addr, (1 << (pin % 32)));
    delay(150);

    /* clear action and remove clock */
    mmio_write(GPIO_PUD, OFF);
    mmio_write(*pud_addr, 0x00000000);
}
