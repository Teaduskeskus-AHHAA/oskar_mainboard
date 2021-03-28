#ifndef UART_H
#define UART_H
#include <stdint.h>

void uart_putc(uint8_t send);
void uart_puts(const char *send);

#endif