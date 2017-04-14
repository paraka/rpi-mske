#include <stdarg.h>
#include <common/io.h>

/**
 *  Kernel printf function requiring minimal infrastructure.
 */
void printk(const char *fmt, ...)
{
  va_list  ap;       /* points to each unnamed argument in turn */

  va_start(ap, fmt); /* make ap point to 1st unnamed arg */
  vprintk(fmt, ap);
  va_end(ap);        /* clean up when done */
}
