#include "include/oskar_packet.h"
#include <util/crc16.h>
#include <include/uart.h>
/*
uint8_t *getEscapedData(OskarPacket *packet, uint8_t *resultsize) {
  uint8_t *result;
  uint8_t result_i = 0;

  for (uint8_t i = 0; i < packet->data_size; i++) {
    if (packet->data[i] == END) {
      result[result_i] = ESC;
      result[result_i + 1] = ESC_END;
      result_i += 2;
    } else if (packet->data[i] == ESC) {
      result[result_i] = ESC;
      result[result_i + 1] = ESC_ESC;
      result_i += 2;
    } else {
      result[result_i] = packet->data[i];
      result_i++;
    }
  }
  resultsize = result_i;
  return result;
}*/

uint8_t *getEscapedData(uint8_t* data, uint8_t datasize, uint8_t *resultsize) {
  uint8_t *result;
  uint8_t result_i = 0;

  for (uint8_t i = 0; i < datasize; i++) {
    if (data[i] == END) {
      result[result_i] = ESC;
      result[result_i + 1] = ESC_END;
      result_i += 2;
    } else if (data[i] == ESC) {
      result[result_i] = ESC;
      result[result_i + 1] = ESC_ESC;
      result_i += 2;
    } else {
      result[result_i] = data[i];
      result_i++;
    }
  }
  *resultsize = result_i;
  return result;
}


uint8_t *getUnescapedData(uint8_t *escaped_data, uint8_t data_size) {
  uint8_t *result;
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
  return result;
};

uint8_t checkCRC(uint16_t expected, uint8_t *data, uint8_t data_size) {
  uint16_t crc = 0;
  for (uint8_t i = 0; i < data_size; i++) {
    crc = _crc_ccitt_update(crc, data[i]);
  }

  return crc == expected;
};

void sendPacket(OskarPacket *packet) {
/*  uint8_t* escaped = getEscapedData(packet);
  uint16_t crc = 0;
  for (uint8_t i = 0; i < packet->data_size_escaped; i++) {
    crc = _crc_ccitt_update(crc, escaped[i]);
  }
  uart_putc(END);
  uart_putc(packet->data_size_escaped+1);
  uart_putc(packet->command);
  for(uint8_t i = 0; i < packet->data_size_escaped; i++) {
    uart_putc(escaped[i]);
  }
  uart_putc(crc & 0xFF);
  uart_putc(crc >> 8);
  uart_putc(END);*/
}