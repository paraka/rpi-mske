#include <stddef.h>
#include <common/io.h>
#include <os/queue.h>

mske_task_t *get_task_from_list(mske_task_t **list)
{
    mske_task_t *aux = *list;
    *list = (*list)->next;
    return aux;
}

void put_task_in_list(mske_task_t **list, mske_task_t *task)
{
    if (*list == NULL)
    {
        *list = task;
        (*list)->next = NULL;
        return;
    }

    mske_task_t *aux = *list;

    while (aux->next != NULL)
        aux = aux->next;

    aux->next = task;
    aux->next->next = NULL;
}

void print_list(mske_task_t *task)
{
    mske_task_t *p = task;
    
    printk("Process list: \n");
    while(p != NULL)
    {
        printk("[ %d ] ", p->pid);
        p = p->next;
    }
    printk("\n");
}
