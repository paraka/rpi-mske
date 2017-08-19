#include <timer/timer.h>

static mske_system_timer_t *system_timer = (mske_system_timer_t *)TIMER_BASE;

void usleep(u32 us)
{
    volatile u32 ts = system_timer->counter_lo;

    while((system_timer->counter_lo - ts) < us) { }
}
