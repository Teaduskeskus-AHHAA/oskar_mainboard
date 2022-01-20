#include "include/sbus.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void SBUS_init() { // TODO: Rewrite on the UART that the real board will use
  rx_pointer = 0;
  UBRR0 = 4;
  UCSR0B = (1 << RXEN0) | (1 << RXCIE0) | (1 << TXEN0);
  UCSR0C |= (1 << USBS0) || (1 << UPM01);
}

ISR(USART_RX_vect) {
  uint8_t data = UDR0;
  rx_buffer[rx_pointer++] = data;
  if(rx_pointer == SBUS_PACKAGE_SIZE) {
    rx_pointer = 0;
    sbus_read();
  }
}

uint16_t get_value(uint16_t channel_data){
    static uint16_t max_value = 0;
    static uint16_t min_value = 0xFFFF;
    if(channel_data>max_value) max_value = channel_data;
    if(channel_data<min_value) min_value = channel_data;

    return (uint16_t )(256.0 * (channel_data-min_value) / (max_value-min_value));

}

void decode_sbus(uint8_t *sbusData) {
    channels[0] = ((sbusData[1] | sbusData[2] << 8) & 0x07FF);
    channels[1] = ((sbusData[2] >> 3 | sbusData[3] << 5) & 0x07FF);
    channels[2] = ((sbusData[3] >> 6 | sbusData[4] << 2 | sbusData[5] << 10) & 0x07FF);
    channels[3] = ((sbusData[5] >> 1 | sbusData[6] << 7) & 0x07FF);
    channels[4] = ((sbusData[6] >> 4 | sbusData[7] << 4) & 0x07FF);
    channels[5] = ((sbusData[7] >> 7 | sbusData[8] << 1 | sbusData[9] << 9) & 0x07FF);
    channels[6] = ((sbusData[9] >> 2 | sbusData[10] << 6) & 0x07FF);
    channels[7] = ((sbusData[10] >> 5 | sbusData[11] << 3) & 0x07FF); // & the other 8 + 2 channels if you need them
    channels[8] = ((sbusData[12] | sbusData[13] << 8) & 0x07FF);
    channels[9] = ((sbusData[13] >> 3 | sbusData[14] << 5) & 0x07FF);
    channels[10] = ((sbusData[14] >> 6 | sbusData[15] << 2 | sbusData[16] << 10) & 0x07FF);
    channels[11] = ((sbusData[16] >> 1 | sbusData[17] << 7) & 0x07FF);
    channels[12] = ((sbusData[17] >> 4 | sbusData[18] << 4) & 0x07FF);
    channels[13] = ((sbusData[18] >> 7 | sbusData[19] << 1 | sbusData[20] << 9) & 0x07FF);
    channels[14] = ((sbusData[20] >> 2 | sbusData[21] << 6) & 0x07FF);
    channels[15] = ((sbusData[21] >> 5 | sbusData[22] << 3) & 0x07FF);
}

void sbus_read() {

  if (rx_buffer[0] == 0x0F && rx_buffer[24] == 0x00) {
    decode_sbus(rx_buffer);
  }

}