#include <common/defs.h>
#include <irq/irq.h>
#include <timer/timer.h>
#include <common/io.h>
#include <os/task.h>

const u32 TASK_SIZE = sizeof(mske_task_t);
mske_task_t proc[NUMBER_OF_PROCESS];

#define NUMBER_OF_REGISTERS 15

mske_task_t *running;

void context_switch(void)
{
    extern void tswitch(void); /* defined in boot.s */
    disable_irqs();
    tswitch();
    enable_irqs();
}

static u32 dummy_task_entry_point(void *arg)
{
    UNUSED(arg);

    printk("%s: task %d was resumed\n", __func__, running->pid);

    while(1)
    {
        printk("%s: infinite loop for %d\n", __func__, running->pid);
        usleep(3000000);
        context_switch();
        printk("%s: context switch done\n", __func__);
    }

    return 0;
}

void init_dummy_processes(void)
{
    int i, j;
    mske_task_t *p;

    for (i = 0; i < NUMBER_OF_PROCESS; i++)
    {
        p = &proc[i];
        p->pid = i;
        p->status = READY;
        p->priority = 1;
        /* init kstack and saved ksp */
        for (j = 1; j < NUMBER_OF_REGISTERS; j++)
            p->kstack[TASK_STACK_SIZE - j] = 0; /* all saved regs = 0 */
        p->kstack[TASK_STACK_SIZE - 1] = (u32)dummy_task_entry_point;
        p->ksp = &(p->kstack[TASK_STACK_SIZE - 14]); /* saved ksp */
        p->next = p + 1; /* point to next task */
    }
    
    /* suppose we have a circular list of processes. make it possible */
    proc[NUMBER_OF_PROCESS - 1].next = &proc[0];
    running = &proc[0];
}

void scheduler(void)
{
    printk("task %u in scheduler\n", running->pid);
    running = running->next;
    printk("next running task %d\n", running->pid);
}
