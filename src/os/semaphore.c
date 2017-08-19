#include <irq/irq.h>
#include <os/queue.h>
#include <os/semaphore.h>

static mske_return_code_t block(mske_semaphore_t *s)
{
    running->status = WAITING;
    put_task_in_list(&s->queue, running);
    context_switch();
    return MSKE_OS_OK;
}

static mske_return_code_t signal(mske_semaphore_t *s)
{
    mske_task_t *p = get_task_from_list(&s->queue);
    p->status = READY;
    put_task_in_list(&task_ready_queue, p);
    return MSKE_OS_OK;
}

mske_return_code_t mske_semaphore_wait(mske_semaphore_t *s)
{
    mske_return_code_t ret = MSKE_OS_OK;

    disable_irqs();
    s->value--;
    if (s->value < 0)
        ret = block(s);
    enable_irqs();

    return ret;
}

mske_return_code_t mske_semaphore_release(mske_semaphore_t *s)
{
    mske_return_code_t ret = MSKE_OS_OK;

    disable_irqs();
    s->value++;
    if (s->value >= 0)
        signal(s);
    enable_irqs();

    return ret;
}
