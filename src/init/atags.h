#ifndef _ATAGS_H_
#define _ATAGS_H_
 
#include <types.h>

enum Model
{
    RPI,
    RPI_B_PLUS,
    RPI_2,
};

typedef struct Atag Atag;
extern Atag *atags;
extern enum Model model;
extern const char *model_name;
extern u32 mem_total;
extern u32 initrd_start;
extern u32 initrd_size;

void resume_atags(void);

#endif /* _ATAGS_H_ */
