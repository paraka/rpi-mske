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

#endif /* _PROCESSOR_H_ */
