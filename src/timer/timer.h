#ifndef _TIMER_H_
#define _TIMER_H_

#include <common/peripherals.h>
#include <types.h>

#define TIMER_BASE  (PERIPHERALS_BASE + 0x3000)

/* page 172 of BCM2835-ARM-Perriperals.pdf */
typedef struct {
    volatile u32 control_status;
    volatile u32 counter_lo;
    volatile u32 counter_hi;
    volatile u32 compare0;
    volatile u32 compare1;
    volatile u32 compare2;
    volatile u32 compare3;
} mske_system_timer_t;

void usleep(u32 us);

#endif /* _TIMER_H_ */
