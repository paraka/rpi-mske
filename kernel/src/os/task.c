#include <stddef.h>
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

void context_switch(void)
{
    extern void tswitch(void); /* defined in boot.s */
    disable_irqs();
    tswitch();
    enable_irqs();
}

u32 dummy_task_entry_point(void *arg)
{
    u8 c;
    UNUSED(arg);

    printk("%s: task %d was resumed\n", __func__, running->pid);

    while(1)
    {
        printk("%s: current task %d -> action [s|f|x] (s: context switch | f: fork | x: exit ) : \n", __func__, running->pid);
        uart_read(uart_dev_id, &c, 1);
        switch(c)
        {
            case 's': context_switch();                         break;
            case 'f': kfork((u32)dummy_task_entry_point, 1);    break;
            case 'x': kexit();                                  break;
        }
    }

    return 0;
}

int kfork(u32 func, int priority)
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

void kexit(void)
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
