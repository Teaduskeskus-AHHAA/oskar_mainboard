#include <avr/io.h>
#include <avr/interrupt.h>
#include <include/spi.h>
#include <include/mcp2515.h>
#include <include/gyems.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <stdlib.h>
#include <include/oskar_defs.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

can_frame_t can_frame;
uint8_t can_frame_ready = 0;

gyems_motor motor1;
can_frame_t frm;
uint8_t motors_ready = 0;

void init_usart() {
    CLKPR = 128;
    CLKPR = 0;
    DDRD |= (1 << PIND1);
    DDRD &= ~(1 << PIND0);
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0);   // Turn on the transmission and reception circuitry
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01); // Use 8-bit character sizes
    UBRR0H = 0;
    UBRR0L=51;
}

void usart_putc (char send) {
    // Do nothing for a bit if there is already
    // data waiting in the hardware to be sent
    while ((UCSR0A & (1 << UDRE0)) == 0) {};
    UDR0 = send;
}

void usart_puts (const char *send) {
    // Cycle through each character individually
    while (*send) {
        usart_putc(*send++);
    }
}



void stall_can_init_error() {
	cli();
	while(1) {
		PORTB |= (1 << PB0);
		_delay_ms(1000);
		PORTB &= ~(1 << PB0);
		_delay_ms(1000);
	}
}

ISR(PCINT1_vect) {
	if(!(PINC & (1 << PINC5))) {
		mcp2515_read(0, &frm);
	} else if(!(PINC & (1 << PINC4))) {
		mcp2515_read(1,&frm);
	}
	gyems_motor_parse_can(&motor1,&frm);
}


int main() {

	/* Define motors */

	motor1.id = 0x141;
	motor1.operating_mode = 1; //Multiturn
	motor1.endpoint_speed = 200;
	motor1.multiturn_angle_range = -5670;
	motor1.endpoints_found = 0;
	motor1.endpoint_1_port = &PORTD;
	motor1.endpoint_2_port = &PORTD;
	motor1.endpoint_1_pin = 7;
	motor1.endpoint_2_pin = 5;

	/* Start motor-related IO conf */

	*(motor1.endpoint_1_port-1) &= ~(1 << motor1.endpoint_1_pin);
	//*(motor1.endpoint_2_port-1) &= ~(1 << motor1.endpoint_2_pin);

	/* Start Board IO conf */

	//B0 - CAN Error led
	DDRB |= (1 << DDB0) | CAN_RESET_DDR;
	DDRD |= (1 << DDD6);

	/* End Board IO conf */


	/* Start Initialize peripheral systems */

	//USART
	init_usart();

	// Initialize SPI
	SPI_init();

	PORTB |= CAN_RESET_PIN;
	_delay_ms(100);
	PORTB &= ~CAN_RESET_PIN;
	_delay_ms(100);
	PORTB |= CAN_RESET_PIN;
	// Initialize CAN Controller or enter hard error state if that fails.
	if(!mcp2515_init(16,1000000)) {
		stall_can_init_error();
	}

	PCMSK1 |= (1 << PCINT12) | (1 << PCINT13);
	PCICR |= (1 << PCIE1);
	/* End Initialize peripheral systems */
	_delay_ms(100);

	sei();
	_delay_ms(500);
	usart_puts("OSKAR\r\n");
	char buffer[30];

//	gyems_motor_request_status(&motor1);
	//gyems_motor_set_speed(&motor1,0);
  gyems_motor_find_endpoints(&motor1);

	motors_ready = 1;
	_delay_ms(1000);
	  gyems_motor_set_multiturn_angle(&motor1,10,5760/2);

	while(1) {
		if(motors_ready) {
		//	gyems_motor_request_status(&motor1);
		}

		//gyems_motor_parse_can(&motor1,frm);
		ltoa(motor1.multiturn_angle_0,buffer,10);
		//sprintf(buffer,"%i",motor1.torque_current*3300.0/2048.0);
		usart_puts(buffer);
		usart_puts("\r\n");
	}
	
	return 0;
}	
