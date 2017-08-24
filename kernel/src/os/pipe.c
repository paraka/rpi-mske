#include <stddef.h>
#include <os/task.h>
#include <os/events.h>
#include <os/pipe.h>

#define NUM_PIPES   128

static mske_pipe_t pipes[NUM_PIPES]; /* pipes in the system */

static u32 pipe_index = 0;

mske_pipe_t *mske_create_pipe(void)
{
    if (pipe_index >= NUM_PIPES)
        return NULL;

    mske_pipe_t *ret = &pipes[pipe_index];

    ret->status = FREE;    
    ret->head = ret->tail = 0;
    ret->data = 0;
    ret->room = PIPE_SIZE;
    ret->nreaders = ret->nwriters = 0;
    pipe_index++;

    return ret;
}

s32 mske_write_pipe(mske_pipe_t *pipe, void *buf, u32 bytes)
{
    int r = 0;
    char *buffer = (char *)buf;

    if (bytes <= 0) return MSKE_OS_OK;

    if (!pipe) return -MSKE_OS_INVALID;
    
    while(bytes)
    {
        if (!pipe->nreaders) /* no more readers */
        {
            //mske_exit_task();
            return MSKE_OS_OK;
        }

        while(pipe->room)
        {
            pipe->buf[pipe->head++] = *buffer++; /* write a byte */
            pipe->head %= PIPE_SIZE;
            pipe->data++; pipe->room--; r++; bytes--;
            if (bytes == 0) break;
        }
        
        wake_up(WAIT_FOR_DATA_PIPE); /* wake up readers if any */
        if (bytes == 0)
            return r; /* finished writting bytes */
        /* still has data but pipe has no room */
        sleep(WAIT_FOR_ROOM_PIPE);
    }

    return MSKE_OS_OK;
}

s32 mske_read_pipe(mske_pipe_t *pipe, void *buf, u32 bytes)
{
    int r = 0;
    char *buffer = (char *)buf;

    if (bytes <= 0) return MSKE_OS_OK;

    if (!pipe) return -MSKE_OS_INVALID;

    while(bytes)
    {
        while(pipe->data)
        {
            *buffer++ = pipe->buf[pipe->tail++];
            pipe->tail %= PIPE_SIZE; 
            pipe->data--; pipe->room++; r++; bytes--;
            if (bytes == 0) break;
        }
        /* has read some data */
        if (r) 
        {
            wake_up(WAIT_FOR_ROOM_PIPE); /* wake up writters */
            return r;
        }
        /* pipe has no data */
        if (pipe->nwriters)
        {
            wake_up(WAIT_FOR_ROOM_PIPE);
            sleep(WAIT_FOR_DATA_PIPE);
            continue;
        }
        /* pipe has no writter and no data */
        return MSKE_OS_OK;
    }

    return MSKE_OS_OK;
}
