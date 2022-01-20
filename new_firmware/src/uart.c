#include "include/uart.h"
#include <avr/io.h>
#include "include/pindefs.h"

void UART_writechar(char c) {
  while ((UCSR0A & (1 << UDRE0)) == 0) {}
  UDR0 = c;
}

void UART_writestr(const char *str) {
  while(*str) {
    UART_writechar(*str++);
  }
}

void UART_init() {
  UART_DDR |= (1 << UART_TX_DDR);
  UART_DDR &= ~(1 << UART_RX_DDR);
}