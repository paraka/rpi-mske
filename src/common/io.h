#ifndef _IO_H_
#define _IO_H_

/**
 * This type defines the prototype for the BSP provided method to
 * print a character. It is assumed to be polled.
 */
typedef void    (*BSP_output_char_function_type)    (char c);

/**
 * This variable points to the BSP provided method to output
 * a character for the purposes of debug output.
 */
extern  BSP_output_char_function_type       BSP_output_char;

#include <stdarg.h>

extern void vprintk(const char *fmt, va_list ap);

extern void printk(const char *fmt, ...);

extern void putc(char c);

#endif /* _IO_H_ */
