#include <include/uart.h>
#include <avr/io.h>

void uart_putc(uint8_t send) {
  while ((UCSR0A & (1 << UDRE0)) == 0) {};
  UDR0 = send;
}

void uart_puts(const char *send) {
  while (*send) {
    uart_putc(*send++);
  }
}