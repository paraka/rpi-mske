#ifndef __UART_H_
#define __UART_H_

#include <common/device.h>
#include <types.h>

int uart_register(void);

mske_ret_code_t uart_init(void);
mske_ret_code_t uart_read(u8 dev_id, void *buffer, size_t nbytes);
mske_ret_code_t uart_write(u8 dev_id, void *buffer, size_t nbytes);
mske_ret_code_t uart_destroy(u8 dev_id);

#endif // __UART_H_
