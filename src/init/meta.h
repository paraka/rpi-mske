#ifndef _META_H_
#define _META_H_

#include <types.h>

struct meta 
{
    const char *AUTHOR;
    const char *EMAIL;
    const char *KERNEL_NAME;
    const char *KERNEL_NAME_CODE;
    const char *KERNEL_REPO;
    const char *BUILD_DATE;
    const char *VERSION;
};

static inline void get_build_info(struct meta *metadata) 
{
    metadata->AUTHOR = "Sergio Paracuellos";
    metadata->EMAIL = "sergio.paracuellos@gmail.com";
    metadata->KERNEL_NAME = "MSKE";
    metadata->KERNEL_NAME_CODE = "MSKE";
    metadata->KERNEL_REPO = "https://github.com/paraka/rpi-mske";
    /* TODO: get time stamp */
    metadata->BUILD_DATE = "2017-07-17 07:27:55 AM";
    metadata->VERSION = "0.0.0.0.1";
}

#endif // _META_H_
