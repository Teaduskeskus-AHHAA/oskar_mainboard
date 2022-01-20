#include "include/spi.h"
#include "include/pindefs.h"

void SPI_init() {
  SPI_DDR |= (1 << SPI_MOSI_DDR) | (1 << SPI_MISO_DDR) | (1 << SPI_SCK_DDR);
  SPCR |= (1<<SPE) | (1<<MSTR) | (1<<SPR0);

}

void SPI_write(uint8_t data) {
  SPDR = data;
  SPI_wait();
}

uint8_t SPI_read() {
  SPI_wait();
  return SPDR;
}

uint8_t SPI_write_read(uint8_t data) {
  SPI_write(data);
  return SPI_read();
}

void SPI_wait() {
  	while(!(SPSR & (1<<SPIF) ));
}