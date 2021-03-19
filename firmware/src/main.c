#include <avr/interrupt.h>
#include <avr/io.h>
#include <include/gyems.h>
#include <include/mcp2515.h>
#include <include/spi.h>

#define F_CPU 8000000UL
#include <include/oskar_defs.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

can_frame_t can_frame;
uint8_t can_frame_ready = 0;

volatile gyems_motor motor1;
can_frame_t frm;
char buffer[30];

uint8_t motors_ready = 0;

#define KEY_PIN PINC

void init_usart() {
  CLKPR = 128;
  CLKPR = 0;
  DDRD |= (1 << PIND1);
  DDRD &= ~(1 << PIND0);
  UCSR0B |= (1 << TXEN0) |
            (1 << RXEN0); // Turn on the transmission and reception circuitry
  UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01); // Use 8-bit character sizes
  UBRR0H = 0;
  UBRR0L = 51;
}

void usart_putc(char send) {
  // Do nothing for a bit if there is already
  // data waiting in the hardware to be sent
  while ((UCSR0A & (1 << UDRE0)) == 0) {
  };
  UDR0 = send;
}

void usart_puts(const char *send) {
  // Cycle through each character individually
  while (*send) {
    usart_putc(*send++);
  }
}

void stall_can_init_error() {
  cli();
  while (1) {
    PORTB |= (1 << PB0);
    _delay_ms(1000);
    PORTB &= ~(1 << PB0);
    _delay_ms(1000);
  }
}

ISR(PCINT1_vect) {
  if (!(PINC & (1 << PINC5))) {
    mcp2515_read(0, &frm);
    gyems_motor_parse_can(&motor1, &frm);
  } else if (!(PINC & (1 << PINC4))) {
    mcp2515_read(1, &frm);
    gyems_motor_parse_can(&motor1, &frm);
  }
}

uint8_t key_state; // debounced and inverted key state:
                   // bit = 1: key pressed
uint8_t key_press; // key press detect

ISR(TIMER0_COMPA_vect) // every 10ms
{
  static uint8_t ct0 = 0xFF, ct1 = 0xFF; // 8 * 2bit counters
  uint8_t i;

  i = ~KEY_PIN;               // read keys (low active)
  i ^= key_state;             // key changed ?
  ct0 = ~(ct0 & i);           // reset or count ct0
  ct1 = ct0 ^ (ct1 & i);      // reset or count ct1
  i &= ct0 & ct1;             // count until roll over ?
  key_state ^= i;             // then toggle debounced state
  key_press |= key_state & i; // 0->1: key press detect
}

uint8_t get_key_press(uint8_t key_mask) {
  cli();                 // read and clear atomic !
  key_mask &= key_press; // read key(s)
  key_press ^= key_mask; // clear key(s)
  sei();
  return key_mask;
}

int main() {
  TCCR0A = 1 << WGM01;                // T0 Mode 2: CTC
  TCCR0B = 1 << CS02 ^ 1 << CS00;     // divide by 1024
  OCR0A = F_CPU / 1024.0 * 10e-3 - 1; // 10ms
  TIMSK0 = 1 << OCIE0A;
  key_state = ~KEY_PIN; // no action on keypress during reset
  /* Define motors */

  motor1.id = 0x141;
  motor1.operating_mode = 1; // Multiturn
  motor1.endpoint_speed = 200;
  motor1.multiturn_angle_range = -5760;
  motor1.endpoints_found = 0;
  motor1.endpoint_1_port = &PORTD;
  motor1.endpoint_2_port = &PORTD;
  motor1.endpoint_1_pin = 7;
  motor1.endpoint_2_pin = 5;
  motor1.angle = 0;

  /* Start motor-related IO conf */

  *(motor1.endpoint_1_port - 1) &= ~(1 << motor1.endpoint_1_pin);
  //*(motor1.endpoint_2_port-1) &= ~(1 << motor1.endpoint_2_pin);

  /* Start Board IO conf */

  // B0 - CAN Error led
  DDRB |= (1 << DDB0) | CAN_RESET_DDR;
  DDRD |= (1 << DDD6);

  /* End Board IO conf */

  /* Start Initialize peripheral systems */

  // USART
  init_usart();

  // Initialize SPI
  SPI_init();

  PORTB |= CAN_RESET_PIN;
  _delay_ms(100);
  PORTB &= ~CAN_RESET_PIN;
  _delay_ms(100);
  PORTB |= CAN_RESET_PIN;
  // Initialize CAN Controller or enter hard error state if that fails.
  if (!mcp2515_init(16, 1000000)) {
    stall_can_init_error();
  }
  DDRC &= ~(1 << DDC0);

  PCMSK1 |= (1 << PCINT9) | (1 << PCINT10) | (1 << PCINT11) | (1 << PCINT12) |
            (1 << PCINT13);
  PCICR |= (1 << PCIE1);
  /* End Initialize peripheral systems */
  _delay_ms(100);

  sei();
  _delay_ms(500);
  //	usart_puts("OSKAR\r\n");

  //	gyems_motor_request_status(&motor1);
  // gyems_motor_set_speed(&motor1,0);
  gyems_motor_find_endpoints(&motor1);

  motors_ready = 1;
  _delay_ms(1000);
  gyems_motor_set_multiturn_angle(&motor1, 10, 5760 / 2);

  while (1) {
    if (motors_ready) {
      //	gyems_motor_request_status(&motor1);
    }

    if (key_state & (1 << PINC0)) { // 10deg
      if (get_key_press(1 << PINC1)) {
        gyems_motor_set_multiturn_angle(&motor1, 5, motor1.angle - 10);
      }
      if (get_key_press(1 << PINC3)) {
        gyems_motor_set_multiturn_angle(&motor1, 5, motor1.angle + 10);
      }
    } else {
      if (get_key_press(1 << PINC1)) {
        gyems_motor_set_multiturn_angle(&motor1, 10, 0);
      }
      if (key_state & (1 << PINC2)) {
        gyems_motor_set_multiturn_angle(&motor1, 10, 5760 / 2);
      }
      if (get_key_press(1 << PINC3)) {
        gyems_motor_set_multiturn_angle(&motor1, 10, 5760);
      }
    }

    // gyems_motor_parse_can(&motor1,frm);

    //	usart_puts("\r\n");
  }

  return 0;
}
