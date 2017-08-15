#ifndef TASK_H_
#define TASK_H_

#include <types.h>

#define TASK_STACK_SIZE     1024

/* just dummy number of processes */
#define NUMBER_OF_PROCESS   5

typedef enum
{
    DORMANT,
    READY,
    RUNNING,
    WAITING,
} mske_task_status_t;

typedef struct task_struct
{
    struct task_struct *next; /* next task_struct structure */
    u32 *ksp; /* saved stack pointer of process when is not running */
    mske_task_status_t status; /* status of the process */
    u32 pid; /* process identifier */
    u32 priority; /* process priority */
    u32 kstack[TASK_STACK_SIZE]; /* execution stack of the process */
} mske_task_t;

extern const u32 TASK_SIZE; /* size of the struct to be able to set SVC stack correctly */
extern mske_task_t *running; /* current running task */
extern mske_task_t proc[NUMBER_OF_PROCESS]; /* this are the processes in the system */

void init_dummy_processes(void);
void context_switch(void);
void scheduler(void);

#endif /* TASK_H_ */
