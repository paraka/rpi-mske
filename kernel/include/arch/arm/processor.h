#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_

#include <types.h>

typedef u32 mske_register_t;

typedef struct
{
    mske_register_t r0;
    mske_register_t r1;
    mske_register_t r2;
    mske_register_t r3;
    mske_register_t r4;
    mske_register_t r5;
    mske_register_t r6;
    mske_register_t r7;
    mske_register_t r8;
    mske_register_t r9;
    mske_register_t r10;
    mske_register_t r11;
    mske_register_t r12;
    mske_register_t lr_svc;
    mske_register_t sp_usr;
    mske_register_t lr_usr;
    mske_register_t lr;
    mske_register_t spsr;
} mske_context_t;

void dump_registers(const mske_context_t *ctx);

static inline u32 asm_get_cpsr(void)
{
    mske_register_t reg_cpsr;
    asm volatile (" mrs  %0, cpsr" : "=r" (reg_cpsr) : /* no inputs */  );
    return reg_cpsr;
}

static inline void asm_set_cpsr(mske_register_t reg_cpsr)
{
    asm volatile (" msr  cpsr, %0" : /* no outputs */ : "r" (reg_cpsr));
}

#endif /* _PROCESSOR_H_ */
