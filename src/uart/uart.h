#ifndef __UART_H_
#define __UART_H_

#include <types.h>

void init_uart(void);
void uart_putc(u8 c);
void write(const char *buf, size_t len);
u8 uart_getc(void);

#endif // __UART_H_
