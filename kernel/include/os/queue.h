#ifndef QUEUE_H_
#define QUEUE_H_

#include <os/task.h>

mske_task_t *get_task_from_list(mske_task_t **list);
void put_task_in_list(mske_task_t **list, mske_task_t *task);
void print_list(mske_task_t *task);

#endif /* QUEUE_H_ */
