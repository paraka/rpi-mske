#include <led/led.h>
#include <timer/timer.h>
#include <common/panic.h>

void kernel_panic()
{
    /* TODO: Dump registers etc ... */

    while (true)
    {
        /* turn off */
        set_led_state(true);
        usleep(250000);

        /* turn on */
        set_led_state(false);
        usleep(250000);
    }
}
