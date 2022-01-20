#ifndef SBUS_H
#define SBUS_H
#include <avr/io.h>

#define SBUS_PACKAGE_SIZE 25

uint8_t rx_buffer[SBUS_PACKAGE_SIZE];
uint8_t rx_pointer;
uint16_t channels[16];
uint16_t get_value(uint16_t channel_data);

void SBUS_init();
void sbus_read();
void decode_sbus(uint8_t *sbusData);

#endif