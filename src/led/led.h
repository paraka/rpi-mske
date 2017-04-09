#ifndef __LED_H_
#define __LED_H_

#if !defined(__cplusplus)
#include <stdbool.h>
#endif

void init_led(void);
void set_led_state(bool state);

#endif // __LED_H_
