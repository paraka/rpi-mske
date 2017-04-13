#include <common/peripherals.h>
#include <gpio/gpio.h>
#include "uart.h"

#define UART0_BASE   (GPIO_BASE + 0x1000)

#define TX_GPIO_PIN     14
#define RCV_GPIO_PIN    15

#define ALL_INTERRUPTS  0x7FF

enum
{
    UART0_DR     = (UART0_BASE + 0x00),
    UART0_RSRECR = (UART0_BASE + 0x04),
    UART0_FR     = (UART0_BASE + 0x18),
    UART0_ILPR   = (UART0_BASE + 0x20),
    UART0_IBRD   = (UART0_BASE + 0x24),
    UART0_FBRD   = (UART0_BASE + 0x28),
    UART0_LCRH   = (UART0_BASE + 0x2C),
    UART0_CR     = (UART0_BASE + 0x30),
    UART0_IFLS   = (UART0_BASE + 0x34),
    UART0_IMSC   = (UART0_BASE + 0x38),
    UART0_RIS    = (UART0_BASE + 0x3C),
    UART0_MIS    = (UART0_BASE + 0x40),
    UART0_ICR    = (UART0_BASE + 0x44),
    UART0_DMACR  = (UART0_BASE + 0x48),
    UART0_ITCR   = (UART0_BASE + 0x80),
    UART0_ITIP   = (UART0_BASE + 0x84),
    UART0_ITOP   = (UART0_BASE + 0x88),
    UART0_TDR    = (UART0_BASE + 0x8C),

    /* Flag Register (depends on LCRH.FEN) */
    FR_TXFE = 1 << 7, // Transmit FIFO empty
    FR_RXFF = 1 << 6, // Receive FIFO full
    FR_TXFF = 1 << 5, // Transmit FIFO full
    FR_RXFE = 1 << 4, // Receive FIFO empty
    FR_BUSY = 1 << 3, // BUSY transmitting data
    FR_CTS = 1 << 0, // Clear To Send
};

struct dev_operations device_operations = {
    uart_read,
    uart_write,
    uart_init,
    uart_destroy,
};

mske_ret_code_t uart_init(void)
{
    /* disable uart0 */
    mmio_write(UART0_CR, 0x00000000);

    /* wait for end of transmission or reception(?) */
    while(mmio_read(UART0_FR) & FR_BUSY) { }

    /* flush transmit fifo */
    mmio_write(UART0_FR, mmio_read(UART0_FR) & ~(1 << 4));

    /* disable pull down */
    set_gpio_pull_up_down(TX_GPIO_PIN, OFF);
    set_gpio_pull_up_down(RCV_GPIO_PIN, OFF);

    /* select function for tx and rcv pins */
    set_gpio_function(TX_GPIO_PIN, FN0);
    set_gpio_function(RCV_GPIO_PIN, FN0);

    /* clear  interrupts */
    mmio_write(UART0_ICR, ALL_INTERRUPTS);
    
    // Set integer & fractional part of baud rate.
    // Divider = UART_CLOCK/(16 * Baud)
    // Fraction part register = (Fractional part * 64) + 0.5
    // UART_CLOCK = 3000000; Baud = 115200.
                 
    // Divider = 3000000 / (16 * 115200) = 1.627 = ~1.
    // Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
    mmio_write(UART0_IBRD, 1);
    mmio_write(UART0_FBRD, 40);

    /* Enable FIFO & 8 bit data transmission (1 stop bit, no parity) */
    mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

    /* mask interrupts */
    mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
                               (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

    /* Enable UART0, receive & transfer part of UART */
    mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));

    return MSKE_SUCESS;
}

static void uart_putc(u8 c)
{
    while(mmio_read(UART0_FR) & FR_TXFF) { }

    mmio_write(UART0_DR, c);
}

static u8 uart_getc(void)
{
    while(mmio_read(UART0_FR) & FR_RXFE) { }

    return mmio_read(UART0_DR);
}

mske_ret_code_t uart_write(u8 dev_id, void *buffer, size_t nbytes)
{
    dev_id = dev_id;

    const u8 *buf = (const u8 *)buffer;

    for (size_t i = 0; i<nbytes; i++)
        uart_putc(buf[i]);

    return nbytes;
}

mske_ret_code_t uart_read(u8 dev_id, void *buffer, size_t nbytes)
{
    dev_id = dev_id;

    u8 *buf = (u8 *)buffer;

    while (nbytes--)
        *buf++ = uart_getc();

    return nbytes;
}

mske_ret_code_t uart_destroy(u8 dev_id)
{
    dev_id = dev_id;

    return MSKE_SUCESS;
}
