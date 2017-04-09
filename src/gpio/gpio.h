#ifndef __GPIO_H_
#define __GPIO_H_

#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <types.h>
#include <common/peripherals.h>

/* relative to PERIPhERALS_BASE */
#define GPIO_BASE   0x200000

enum GPIO_Funcs { INPUT, OUTPUT };

void set_gpio_function(u32 pin, enum GPIO_Funcs fn); 
void set_gpio_state(u32 pin, bool state);

enum PullUpDown { OFF, UP, DOWN };

void set_gpio_pull_up_down(u32 pin, enum PullUpDown fn);

#endif // __GPIO_H_
