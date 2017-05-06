#include <common/io.h>
#include <arch/arm/processor.h>

enum PROCESSOR_MODE
{
    USR = 0xb10000,
    FIQ = 0xb10001,
    IRQ = 0xb10010,
    SVC = 0xb10011,
    MON = 0xb10110,
    ABT = 0xb10111,
    HYP = 0xb11010,
    UND = 0xb11011,
    SYS = 0xb11111  
};

static const char *processor_mode_to_string(u32 mode)
{
    const char *ret;

    switch(mode)
    {
        case 0xb10000:
            ret = "USR";
            break;
        case 0xb10001:
            ret = "FIQ";
            break;
        case 0xb10010:
            ret = "IRQ";
            break;
        case 0xb10011:
            ret = "SVC";
            break;
        case 0xb10110:
            ret = "MON";
            break;
        case 0xb10111:
            ret = "HYP";
            break;
        case 0xb11010:
            ret = "UND";
            break;
        case 0xb11111:
            ret = "SYS";
            break;
        default:
            ret = "ERROR";
            break;
    }

    return ret;
}

#define BIT(w, i, c) (((w) & (1 << (i))) ? c : '-')

void dump_registers(const mske_context_t *ctx)
{
    printk("r0 : %08x  r1 : %08x   r2 : %08x   r3 : %08x\n",
            ctx->r0, ctx->r1, ctx->r2, ctx->r3);   
    printk("r4 : %08x  r5 : %08x   r6 : %08x   r7 : %08x\n",
            ctx->r4, ctx->r5, ctx->r6, ctx->r7);   
    printk("r8 : %08x  r9 : %08x   r10 : %08x  r11 : %08x\n",
            ctx->r8, ctx->r9, ctx->r10, ctx->r11);   
    printk("r12 : %08x  sp_usr : %08x   lr_usr : %08x  lr : %08x\n",
            ctx->r12, ctx->sp_usr, ctx->lr_usr, ctx->lr);   
    printk("SPSR: %c%c%c%c%c IT[1:0]=%x %c GE[3:0]=%x IT[7:2]=%02x %c%c%c%c%c %s(%02x)\n",
            BIT(ctx->spsr, 31, 'N'),
            BIT(ctx->spsr, 30, 'Z'),
            BIT(ctx->spsr, 29, 'C'),
            BIT(ctx->spsr, 28, 'V'),
            BIT(ctx->spsr, 27, 'Q'),
            (ctx->spsr >> 25) & 0x3,
            BIT(ctx->spsr, 24, 'J'),
            // 23-20 reserved
            (ctx->spsr >> 16) & 0xF,
            (ctx->spsr >> 10) & 0x3F,
            BIT(ctx->spsr,  9, 'E'),
            BIT(ctx->spsr,  8, 'A'),
            BIT(ctx->spsr,  7, 'I'),
            BIT(ctx->spsr,  6, 'F'),
            BIT(ctx->spsr,  5, 'T'),
            processor_mode_to_string(ctx->spsr & 0x1F),
            ctx->spsr & 0x1F);
}
