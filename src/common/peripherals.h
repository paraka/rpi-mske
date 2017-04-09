#ifndef __PERIPHERALS_H_
#define __PERIPHERALS_H_

#include <types.h>

// Raspberry pi peripherals base address
#define PERIPHERALS_BASE    0x20000000

inline volatile u32 * get_register_address(u32 offset)
{
    return (volatile u32 *)(PERIPHERALS_BASE + offset);
}

#endif // __PERIPHERALS_H_
