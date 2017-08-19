#include <common/io.h>

#define LF  '\n'
#define CR  '\r'

void putc(char c)
{
    if (c == LF)
        (*BSP_output_char)(CR);

    (*BSP_output_char)(c);
}
