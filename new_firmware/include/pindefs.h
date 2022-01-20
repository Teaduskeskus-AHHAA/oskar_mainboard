#ifndef PINDEFS_H
#define PINDEFS_H
#include <avr/io.h>

#define UART_DDR DDRD
#define UART_RX_DDR DDD0
#define UART_TX_DDR DDD1

#define SPI_DDR DDRB
#define SPI_MISO_DDR DDB4
#define SPI_MOSI_DDR DDB3
#define SPI_SCK_DDR DDB5


#endif  // PINDEFS_H