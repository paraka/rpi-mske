#ifndef _CPUOPS_H_
#define _CPUOPS_H_

#include <arch/arm/barrier.h>
#include <types.h>

/* Read Vector Base Address Register */
static inline u32 read_vbar()
{
    u32 vbar;

    asm volatile("mrc p15, 0, %[vbar], c12, c0, 0 @ Read VBAR\n\t"
                  : [vbar] "=r" (vbar));
    return vbar;
}

/* Write Vector Base Address Register */
static inline void write_vbar(u32 *vbar)
{
    asm volatile("mcr p15, 0, %[vbar], c12, c0, 0 @ Write VBAR\n\t"
                 : : [vbar] "r" (vbar));
    dsb();
}

#endif /* _CPUOPS_H_ */
