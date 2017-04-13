#include <led/led.h>
#include <common/delay.h>
#include <common/panic.h>

void kernel_panic()
{
    /* TODO: Dump registers etc ... */

    while (true)
    {
        set_led_state(true);
        delay(0x10000);
        set_led_state(false);
        delay(0x10000);
    }
}
