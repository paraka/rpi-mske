#ifndef _META_H_
#define _META_H_

#include <types.h>

static const char *build_time = __DATE__ " " __TIME__;

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
    metadata->BUILD_DATE = build_time;
    metadata->VERSION = "0.0.0.0.1";
}

#endif // _META_H_
