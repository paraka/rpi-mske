#include <common/io.h>

void putc(char c)
{
    (*BSP_output_char)(c);
}
