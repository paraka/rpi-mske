#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stddef.h>
#include <types.h>
#include <init/meta.h>
#include <init/atags.h>
#include <common/defs.h>
#include <common/peripherals.h>
#include <common/delay.h>
#include <common/panic.h>
#include <common/io.h>
#include <gpio/gpio.h>
#include <led/led.h>
#include <uart/uart.h>
#include <timer/timer.h>

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

static void blink(u8 ntimes, u32 us_delay)
{
    volatile int i = 0;

    for (i=0; i<ntimes; i++)
    {
        /* turn off */
        set_led_state(true);
        usleep(us_delay);

        /* turn on */
        set_led_state(false);
        usleep(us_delay);
    }

    /* blink finally always turn off led */
    set_led_state(true);
}

static void welcome_message()
{
    struct meta info;

    get_build_info(&info);

    printk("**** Welcome to rpi-mske kernel ****\n");
    printk("Written by: %s\n", info.AUTHOR);
    printk("Email: <%s>\n", info.EMAIL);
    printk("Version: %s \n", info.VERSION);
    printk("Build time: %s\n", info.BUILD_DATE);
    printk("************************************\n\n");
}

void kernel_main(u32 r0, u32 r1, u32 atags)
{
    UNUSED(r0);
    UNUSED(r1);
    UNUSED(atags);

    int uart_dev_id;

    init_led();
    blink(3, 1000000);

    device_init();
    
    if ((uart_dev_id = uart_register()) < 0)
    {
        kernel_panic();
    }

    welcome_message();
    resume_atags();

    u8 buffer;
    while (true)
    {
        device_read(uart_dev_id, &buffer, 1);
        device_write(uart_dev_id, &buffer, 1);
    }
}
