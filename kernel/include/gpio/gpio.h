#ifndef __GPIO_H_
#define __GPIO_H_

#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <types.h>
#include <common/peripherals.h>

/* relative to PERIPhERALS_BASE */
#define GPIO_BASE   (PERIPHERALS_BASE + 0x200000)

/* FSEL9 -> bits 29-27 */
/* 000 = GPIO Pin 9 is an input
 * 001 = GPIO Pin 9 is an output
 * 100 = GPIO Pin 9 takes alternate function 0
 * 101 = GPIO Pin 9 takes alternate function 1
 * 110 = GPIO Pin 9 takes alternate function 2
 * 111 = GPIO Pin 9 takes alternate function 3
 * 011 = GPIO Pin 9 takes alternate function 4
 * 010 = GPIO Pin 9 takes alternate function 5
 */
enum GPIO_Funcs 
{ 
    INPUT   = 0x0, 
    OUTPUT  = 0x1,
    FN0     = 0x4,
    FN1     = 0x5,
    FN2     = 0x6,
    FN3     = 0x7,
    FN4     = 0x3,
    FN5     = 0x2
};

void set_gpio_function(u32 pin, enum GPIO_Funcs fn); 
void set_gpio_state(u32 pin, bool state);

enum PullUpDown 
{ 
    OFF     = 0x0, 
    DOWN    = 0x1,
    UP      = 0x2
};

void set_gpio_pull_up_down(u32 pin, enum PullUpDown fn);

#endif // __GPIO_H_
