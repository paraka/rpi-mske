#include <gpio/gpio.h>
#include "led.h"

#define GPIO_STATUS_LED_PIN  16

void init_led(void)
{
    /* disable pull up down for pin 16 */
    set_gpio_pull_up_down(GPIO_STATUS_LED_PIN, OFF);

    /* configure pin 16 for an OUTPUT */
    set_gpio_function(GPIO_STATUS_LED_PIN, OUTPUT);
}

void set_led_state(bool state)
{
    set_gpio_state(GPIO_STATUS_LED_PIN, state);
}
