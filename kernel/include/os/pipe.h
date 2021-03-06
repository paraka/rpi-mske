#ifndef PIPE_H_
#define PIPE_H_

#include <types.h>
#include <os/os_status.h>

#define PIPE_SIZE   128

typedef enum
{
    FREE,
    BUSY,
} mske_pipe_status_t;

typedef struct
{
    u8  buf[PIPE_SIZE]; /* circular data buffer */
    s32 head, tail; /* circular buffer index */
    s32 data, room; /* number of data and room in pipe */
    u32 nreaders, nwriters; /* number of readers and writters in the pipe */
    mske_pipe_status_t status; /* pipe status */
}mske_pipe_t;

mske_pipe_t *mske_create_pipe(void);
s32 mske_write_pipe(mske_pipe_t *pipe, void *buf, u32 bytes);
s32 mske_read_pipe(mske_pipe_t *pipe, void *buf, u32 bytes);

#endif /* PIPE_H_ */
