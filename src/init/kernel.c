#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stddef.h>
#include <types.h>
#include <init/meta.h>
#include <init/atags.h>
#include <common/defs.h>
#include <common/peripherals.h>
#include <common/delay.h>
#include <common/panic.h>
#include <common/io.h>
#include <gpio/gpio.h>
#include <irq/irq.h>
#include <led/led.h>
#include <uart/uart.h>
#include <timer/timer.h>
#include <timer/arm_timer.h>
#include <os/task.h>

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

#define MAX_EXEC_TIMES  10

static void arm_timer_irq_handler(void *param)
{
    UNUSED(param);

    static int count = 0;
    static bool turned = false;

    /*
     * Even we clear the interrupt bit in the timer
     * it will be exec again the interrupt until pending
     * bit for the interrupt will be clear in source
     */
    arm_timer_clear_irq();

    if (turned)
    {
        /* turn off */
        set_led_state(true);
        turned = false;
    }
    else
    {
        /* turn on */
        set_led_state(false);
        turned = true;
    }


    /*
     * increment count when it reaches 10 clear
     * the interrup pending bit for IRQ_ARM_TIMER
     */
    if (++count == MAX_EXEC_TIMES)
        disable_irq(IRQ_ARM_TIMER);
}

static void blink(u8 ntimes, u32 us_delay)
{
    volatile int i = 0;

    for (i=0; i<ntimes; i++)
    {
        /* turn off */
        set_led_state(true);
        usleep(us_delay);

        /* turn on */
        set_led_state(false);
        usleep(us_delay);
    }

    /* blink finally always turn off led */
    set_led_state(true);
}

static void welcome_message()
{
    struct meta info;

    get_build_info(&info);

    printk("**** Welcome to rpi-mske kernel ****\n");
    printk("Written by: %s\n", info.AUTHOR);
    printk("Email: <%s>\n", info.EMAIL);
    printk("Version: %s \n", info.VERSION);
    printk("Build time: %s\n", info.BUILD_DATE);
    printk("************************************\n\n");
}

static void register_all_devices()
{
    mske_ret_code_t ret;

    if ((ret = uart_register()) != MSKE_SUCESS)
    {
        kernel_panic();
    }
}

void copy_vector_table(void)
{
    extern volatile u32 vectors_start, vectors_end;
    volatile u32 *v_src = &vectors_start;
    volatile u32 *v_dst = (u32 *)0;

    while (v_src < &vectors_end)
        *v_dst++ = *v_src++;
}

void clear_bss(void)
{
    extern volatile u32 __bss_start, __bss_end;
    volatile u32 *bss = &__bss_start;
    volatile u32 *bss_end = &__bss_end;

    while (bss < bss_end)
        *bss++ = 0;
}

void kernel_main(u32 r0, u32 r1, u32 atags)
{
    UNUSED(r0);
    UNUSED(r1);
    UNUSED(atags);

    init_led();
    blink(3, 1000000);

    interrupt_controller_init();

    device_init();
    
    register_all_devices();

    welcome_message();

    resume_atags();

    enable_irq(IRQ_ARM_TIMER);

    register_interrupt(IRQ_ARM_TIMER, arm_timer_irq_handler, NULL);

    /* test timer interrupt */
    setup_arm_timer();

    enable_irqs();

    init_dummy_processes();

    while(1)
    {
        usleep(3000000);
        context_switch();
    }
}
