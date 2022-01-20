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
#include <include/oskar_commands.h>
#include <include/spi.h>


#include <util/crc16.h>


#define F_CPU 20000000UL

// Packet recieve defines

#define STATE_IDLE 0
#define STATE_PACKET_STARTED 1
#define STATE_PACKET_STREAMING 2
#define STATE_PACKET_ENDED 3

// Packet recieve global variables
volatile uint8_t recieve_buffer[128];
volatile uint8_t packet_buffer[128];
volatile uint8_t rxn = 0;
volatile uint8_t packet_len = 0;
volatile uint8_t rx_packet_start = 0;

volatile uint8_t state = STATE_IDLE;

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
  left_wheel_motor.speed =0;

  right_wheel_motor.id = 0x142;
  right_wheel_motor.operating_mode = MOTOR_OPMODE_MULTITURN;
  right_wheel_motor.endpoint_speed = motors_zeroing_speed;
  right_wheel_motor.multiturn_angle_range = 0; // doesn't matter
  right_wheel_motor.endpoints_found = 1;
  right_wheel_motor.speed =0;
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

void zero_motors() {
  motors_ready = 1;
}

void motors_update() {
  if (motors_ready) {
    gyems_motor_request_status(&left_wheel_motor);
    gyems_motor_request_status(&right_wheel_motor);

  }
}
  uint8_t odomdata[8];

void send_odom() {


  odomdata[0] = *(uint8_t *)(&left_wheel_motor.speed);
  odomdata[1] = *((uint8_t *)(&left_wheel_motor.speed) +1);
  odomdata[2] = *((uint8_t *)(&left_wheel_motor.speed) +2);
  odomdata[3] = *((uint8_t *)(&left_wheel_motor.speed) +3);
  odomdata[4] = *(uint8_t *)(&right_wheel_motor.speed);
  odomdata[5] = *((uint8_t *)(&right_wheel_motor.speed)+1);
  odomdata[6] = *((uint8_t *)(&right_wheel_motor.speed)+2);
  odomdata[7] = *((uint8_t *)(&right_wheel_motor.speed)+3);

  uint8_t result[255];
  uint8_t result_i;

  for (uint8_t i = 0; i < 8; i++) {
    if (odomdata[i] == END) {
      result[result_i] = ESC;
      result[result_i + 1] = ESC_END;
      result_i += 2;
    } else if (odomdata[i] == ESC) {
      result[result_i] = ESC;
      result[result_i + 1] = ESC_ESC;
      result_i += 2;
    } else {
      result[result_i] = odomdata[i];
      result_i++;
    }
  }

  uint16_t crc = 0;
  for (uint8_t i = 0; i < result_i; i++) {
    crc = _crc_ccitt_update(crc, result[i]);
  }

  uart_putc(END);
  uart_putc(result_i+1);
  uart_putc(ODOM_COMMAND);
  for(uint8_t i = 0; i < result_i; i++) {
    uart_putc(result[i]);
  }
  uart_putc(crc & 0xFF);
  uart_putc(crc >> 8);
  uart_putc(END);
/*
  uint8_t escapedsize;
  uint8_t* escaped = getEscapedData(odomdata,8, &escapedsize);

  uint16_t crc = 0;
  for (uint8_t i = 0; i < escapedsize; i++) {
    crc = _crc_ccitt_update(crc, escaped[i]);
  }
  uart_putc(END);
  uart_putc(escapedsize+1);
  uart_putc(ODOM_COMMAND);
  for(uint8_t i = 0; i < escapedsize; i++) {
    uart_putc(escaped[i]);
  }
  uart_putc(crc & 0xFF);
  uart_putc(crc >> 8);
  uart_putc(END);*/
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
  if ((state == STATE_PACKET_STARTED) && (recv != END)) { 
    if (recieve_buffer[rxn - 1] == END) { // Lenght byte
      packet_len = recv; 
      state = STATE_PACKET_STREAMING;
    }
    rxn++;
  } else if ((state == STATE_PACKET_STREAMING) && (recv != END)) {
    rxn++;
  } else if ((state == STATE_PACKET_STREAMING) && (recv == END)) {
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
  UBRR0 = 10;
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

void processPacket(OskarPacket *packet) {

  switch(packet->command) {
    case DRIVESPEEDS_COMMAND: ; // This semicolon coaxes the C compiler into allowing a declaration immediately after a case label

      int32_t lspeed = (int32_t)((int32_t)packet->data[3] << 24) | ((int32_t)packet->data[2] << 16) | ((int32_t)packet->data[1] << 8) | (packet->data[0]);
      int32_t rspeed = (int32_t) ((int32_t)packet->data[7] << 24) | ((int32_t)packet->data[6] << 16) | ((int32_t)packet->data[5] << 8)| (packet->data[4]);
      //gyems_motor_set_speed(&left_wheel_motor, lspeed);
     // gyems_motor_set_speed(&right_wheel_motor, rspeed);
      left_wheel_motor.speed = lspeed;
      right_wheel_motor.speed = rspeed;
      break;
    default:
      break;
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
        memcpy(&packet_buffer,&recieve_buffer[rx_packet_start],total_packet_lenght);

        if((packet_buffer[0] == END) && (packet_buffer[1] == packet_len) && (packet_buffer[total_packet_lenght-1] == END)) {

          OskarPacket packet;
          packet.command = packet_buffer[2];

          
          packet.length = packet_len;
          packet.data_size = packet.length - 1;
          memcpy(&packet.data,&packet_buffer[3],packet.length);
          packet.crc = ((uint16_t) packet_buffer[total_packet_lenght-2] << 8) | packet_buffer[total_packet_lenght-3];
          if(checkCRC(packet.crc, &packet.data, packet.length-1)) {
            uint8_t unescaped[255];
            uint8_t unescaped_i = 0;
            uint8_t i = 0;

            while(i < sizeof(packet.data)) {
              PORTB |= (1 << PB0);
              if(packet.data[i] != ESC) {
                unescaped[unescaped_i] = packet.data[i];
                i++;
              } else {
                if (packet.data[i + 1] == ESC_END) {
                  unescaped[unescaped_i] = END;
                } else if (packet.data[i + 1] == ESC_ESC) {
                  unescaped[unescaped_i] = ESC;
                }
                i += 2;
              }
              unescaped_i++;
            }

            memcpy(&packet.data,&unescaped,unescaped_i);
            processPacket(&packet);
                                
  /*        uint8_t *result;
  uint8_t result_i = 0;
  uint8_t i = 0;
  while (i < sizeof(escaped_data) / sizeof(uint8_t)) {
    if (escaped_data[i] != ESC) {
      result[result_i] = escaped_data[i];
      i++;
    } else {
      if (escaped_data[i + 1] == ESC_END) {
        result[result_i] = END;
      } else if (escaped_data[i + 1] == ESC_ESC) {
        result[result_i] = ESC;
      }
      i += 2;
    }
    result_i++;
  }
  return result;*/

          /*uint8_t* clean_data = getUnescapedData(&packet.data, packet.length-1);
            *packet.data = *clean_data;
            processPacket(&packet);*/
            
          }
          //usart_send(packet_buffer,total_packet_lenght);
        }
        
        state = STATE_IDLE;
      }
     send_odom();

  
  //    motors_update();

    
                   PORTB &= ~(1 << PB0);


      // gyems_motor_parse_can(&motor1,recieved_frame);

      //	usart_puts("\r\n");
    }

  return 0;
}
