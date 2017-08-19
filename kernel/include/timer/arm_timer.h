#ifndef _ARM_TIMER_H_
#define _ARM_TIMER_H_

#include <common/peripherals.h>

#define ARM_TIMER_BASE  (PERIPHERALS_BASE + 0xB400)

void setup_arm_timer();
void arm_timer_clear_irq();

#endif /* _ARM_TIMER_H_ */
