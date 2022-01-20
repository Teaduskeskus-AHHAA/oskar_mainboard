#ifndef UART_H
#define UART_H

void UART_writechar(char c);
void UART_writestr(const char *str);
void UART_init();

#endif  // UART_H