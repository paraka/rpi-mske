#include <stddef.h>
#include <arch/arm/cpuopts.h>
#include <common/defs.h>
#include <irq/irq.h>
#include <timer/timer.h>
#include <common/io.h>
#include <uart/uart.h>
#include <os/task.h>
#include <os/queue.h>

#define NUMBER_OF_REGISTERS 15

const u32 TASK_SIZE = sizeof(mske_task_t);

mske_task_t proc[NUMBER_OF_PROCESS];
mske_task_t *running;
mske_task_t *task_free_list, *task_ready_queue;

extern void tswitch(void); /* defined in boot.s */

void context_switch(void)
{
    disable_irqs();
    tswitch();
    enable_irqs();
}

int mske_create_task(u32 func, int priority)
{
    int i;
    mske_task_t *p = get_task_from_list(&task_free_list);
    if (p == NULL)
    {
        printk("No more processes\n");
        return -1;
    }

    p->status = READY;
    p->priority = priority;
    for (i = 1; i < NUMBER_OF_REGISTERS; i++)
        p->kstack[TASK_STACK_SIZE - i] = 0; /* all saved regs to zero */

    p->kstack[TASK_STACK_SIZE - 1] = func; /* resume execution address */
    p->ksp = &(p->kstack[TASK_STACK_SIZE - 14]); /* saved ksp */
    put_task_in_list(&task_ready_queue, p); /* insert p in ready queue */ 
    printk("task %d forked a new proc %d\n", running->pid, p->pid);
    printk("Ready queue: "); print_list(task_ready_queue); 

    return p->pid;
}

void mske_exit_task(void)
{
    printk("task %d exit\n", running->pid);
    running->status = DORMANT;
    put_task_in_list(&task_free_list, running);
    context_switch(); /* yield cpu */
}

void kernel_process_init(void)
{
    int i;
    mske_task_t *p;

    for (i = 0; i < NUMBER_OF_PROCESS; i++)
    {
        p = &proc[i];
        p->pid = i;
        p->event = 0x00000000;
        p->status = DORMANT;
        p->next = p + 1; /* point to next task */
    }

    proc[NUMBER_OF_PROCESS - 1].next = NULL;
    task_free_list = &proc[0]; /* all processes in free list */
    task_ready_queue = NULL;

    /* get P0 as initial running process */
    p = get_task_from_list(&task_free_list);
    p->priority = 1;
    running = p;
    print_list(task_free_list);
}

void scheduler(void)
{
    if (running->status == READY)
        put_task_in_list(&task_ready_queue, running);

    running = get_task_from_list(&task_ready_queue);
}

void sleep(u32 event)
{
    u32 cpsr = read_cpsr();
    disable_irqs();

    running->event |= event;
    running->status = DORMANT;

    tswitch();
    
    enable_irqs();
    write_cpsr(cpsr);
}

void wake_up(u32 event)
{
    int i = 0;
    mske_task_t *p;

    u32 cpsr = read_cpsr();
    disable_irqs();

    for (i = 0; i < NUMBER_OF_PROCESS; i++)
    {
        p = &proc[i];

        if (p->status == DORMANT && p->event & event)
        {
            p->status = READY;
            put_task_in_list(&task_ready_queue, p);
            context_switch();
        }
    }
    
    enable_irqs();
    write_cpsr(cpsr);
}
