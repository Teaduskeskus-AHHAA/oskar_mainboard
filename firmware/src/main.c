#include <avr/interrupt.h>
#include <avr/io.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include <include/gyems.h>
#include <include/mcp2515.h>
#include <include/oskar_packet.h>
#include <include/oskar_defs.h>
#include <include/spi.h>

#define F_CPU 8000000UL

// Packet recieve defines

#define STATE_IDLE 0
#define STATE_PACKET_STARTED 1
#define STATE_PACKET_STREAMING 2
#define STATE_PACKET_ENDED 3

// Packet recieve global variables
volatile uint8_t recieve_buffer[255];
volatile uint8_t packet_buffer[255];
volatile uint8_t rxn = 0;
volatile uint8_t packet_len = 0;
volatile uint8_t rx_packet_start = 0;

uint8_t state = STATE_IDLE;

// Global variables
can_frame_t recieved_can_frame;
uint8_t motors_ready = 0;

uint8_t motors_zeroing_speed = 200;

// Motor instances
// When adding a motor - declare it here, add configuration to
// configure_motors(), add CAN frame parsing to motors_parse_can_msg(), add
// endpoint search to zero_motors(), add update to motors_update
volatile gyems_motor left_wheel_motor;
volatile gyems_motor right_wheel_motor;
volatile gyems_motor squat_motor;
volatile gyems_motor motor1;

void configure_motors() {
  left_wheel_motor.id = 0x141;
  left_wheel_motor.operating_mode = MOTOR_OPMODE_MULTITURN;
  left_wheel_motor.endpoint_speed = motors_zeroing_speed;
  left_wheel_motor.multiturn_angle_range = 0; // doesn't matter
  left_wheel_motor.endpoints_found = 1;

  right_wheel_motor.id = 0x142;
  right_wheel_motor.operating_mode = MOTOR_OPMODE_MULTITURN;
  right_wheel_motor.endpoint_speed = motors_zeroing_speed;
  right_wheel_motor.multiturn_angle_range = 0; // doesn't matter
  right_wheel_motor.endpoints_found = 1;

  /*motor1.id = 0x141;
  motor1.operating_mode = 1; // Multiturn
  motor1.endpoint_speed = 200;
  motor1.multiturn_angle_range = -5760;
  motor1.endpoints_found = 0;
  motor1.endpoint_1_port = &PORTD;
  motor1.endpoint_2_port = &PORTD;
  motor1.endpoint_1_pin = 7;
  motor1.endpoint_2_pin = 5;
  motor1.angle = 0;*/
}

void motors_parse_can_msg() {
  gyems_motor_parse_can(&left_wheel_motor, &recieved_can_frame);
  gyems_motor_parse_can(&right_wheel_motor, &recieved_can_frame);
}

void zero_motors() {}

void motors_update() {
  if (motors_ready) {
    gyems_motor_request_status(&left_wheel_motor);
  }
}

ISR(USART_RX_vect) {
  while (!(UCSR0A & (1 << RXC0))) {
  };
  uint8_t recv = UDR0;
  recieve_buffer[rxn] = recv;
  if ((state == STATE_IDLE) && (recv == END)) { // Packet start
    rx_packet_start = rxn;
    state = STATE_PACKET_STARTED;
    rxn++;
  }
  if ((state == STATE_PACKET_STARTED) && (recv != END)) { // Lenght byte
    if (recieve_buffer[rxn - 1] == END) {
      packet_len = recv;
      state = STATE_PACKET_STREAMING;
    }
    rxn++;
  }
  if ((state == STATE_PACKET_STREAMING) && (recv != END)) {
    rxn++;
  }
  if ((state == STATE_PACKET_STREAMING) && (recv == END)) {
    rxn = 0;
    state = STATE_PACKET_ENDED;
  }
}

void init_usart() {
  CLKPR = 128;
  CLKPR = 0;
  DDRD |= (1 << PIND1);
  DDRD &= ~(1 << PIND0);
  UCSR0B |= (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
  UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
  UBRR0H = 0;
  UBRR0L = 51;
}

void usart_putc(char send) {
  while ((UCSR0A & (1 << UDRE0)) == 0) {
  };
  UDR0 = send;
}

void usart_puts(const char *send) {
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
    mcp2515_read(0, &recieved_can_frame);
    motors_parse_can_msg();
  } else if (!(PINC & (1 << PINC4))) {
    mcp2515_read(1, &recieved_can_frame);
    motors_parse_can_msg();
  }
}

int main() {

  /* Configure motors */
  configure_motors();

  // *(motor1.endpoint_1_port - 1) &= ~(1 << motor1.endpoint_1_pin);
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
 /* if (!mcp2515_init(16, 1000000)) {
    stall_can_init_error();
  }*/
  // DDRC &= ~(1 << DDC0);

  // PCMSK1 |= (1 << PCINT9) | (1 << PCINT10) | (1 << PCINT11) | (1 << PCINT12)
  // |(1 << PCINT13);
  // PCICR |= (1 << PCIE1);
  /* End Initialize peripheral systems */
  _delay_ms(100);

  sei();

  zero_motors();

  //_delay_ms(500);
  //	usart_puts("OSKAR\r\n");

  //	gyems_motor_request_status(&motor1);
  // gyems_motor_set_speed(&motor1,0);
  // gyems_motor_find_endpoints(&motor1);

  // motors_ready = 1;
  //_delay_ms(1000);
  // gyems_motor_set_multiturn_angle(&motor1, 10, 5760 / 2);

  while (1) {

    if(state == STATE_PACKET_ENDED) {
       
        uint8_t total_packet_lenght = packet_len+5;
        memcpy(&packet_buffer,&recieve_buffer[rx_packet_start],total_packet_lenght+1);
        if(packet_buffer[0] == END && packet_buffer[1] == packet_len && packet_buffer[total_packet_lenght] == END) {
         
          OskarPacket packet;
          packet.command = packet_buffer[2];
          packet.length = packet_len;
          packet.data_size = packet.length - 1;
          memcpy(&packet.data,&packet_buffer[4],packet.length-1);
          packet.crc = ((uint16_t) packet_buffer[total_packet_lenght-1] << 8) | packet_buffer[total_packet_lenght-2];
          if(checkCRC(packet.crc, &packet.data, packet.length-1)) {
            PORTB |= (1 << PORTB0);
          }
          //usart_send(packet_buffer,total_packet_lenght);
        }
        state = STATE_IDLE;
      }

      motors_update();

      // gyems_motor_parse_can(&motor1,recieved_frame);

      //	usart_puts("\r\n");
    }

  return 0;
}
