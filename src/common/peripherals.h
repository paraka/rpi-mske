#ifndef __PERIPHERALS_H_
#define __PERIPHERALS_H_

#include <types.h>

// Raspberry pi peripherals base address
#define PERIPHERALS_BASE    0x20000000

inline void mmio_write(u32 reg, u32 data)
{
    *(volatile u32 *)reg = data;
}
 
inline u32 mmio_read(u32 reg)
{
    return *(volatile u32 *)reg;
}

#endif // __PERIPHERALS_H_
