#ifndef __DELAY_H_
#define __DELAY_H_

#include <types.h>

static inline void delay(u32 count)
{
        asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
                 : "=r"(count): [count]"0"(count) : "cc");
}

#endif // __DELAY_H_
