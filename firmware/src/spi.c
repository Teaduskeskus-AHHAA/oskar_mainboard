#include <avr/io.h>
#include <include/spi.h>

void SPI_init() {
	DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB4) | (1 << DDB5);
	SPCR|=(1<<SPE)|(1<<MSTR)|(1<<SPR0);
	PORTB |= (1 << PINB2);
}

uint8_t SPI_txrx(uint8_t data) {
    PORTB &= ~(1 << PINB2);
	SPDR = data;
	while(!(SPSR & (1<<SPIF) ));
    PORTB |= (1 << PINB2);
	return(SPDR);
}

void SPI_unset_cs() {
    PORTB &= ~(1 << PINB2);
}

void SPI_set_cs(){
    PORTB |= (1 << PINB2);
}

void SPI_send(uint8_t data) {
	SPDR = data;
	while(!(SPSR & (1<<SPIF) ));
}
