#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <types.h>
#include <os/task.h>
#include <os/os_status.h>

typedef struct semaphore
{
    u32 spinlock; /* only for MP, not used */
    s32 value; /* semaphore value */
    mske_task_t *queue; /* FIFO queue of waiting tasks */
} mske_semaphore_t;

mske_return_code_t mske_semaphore_wait(mske_semaphore_t *s);

mske_return_code_t mske_semaphore_release(mske_semaphore_t *s);

#endif /* SEMAPHORE_H_ */
