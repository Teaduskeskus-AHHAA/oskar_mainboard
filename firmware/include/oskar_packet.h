#ifndef OSKAR_PACKET_H
#define OSKAR_PACKET_H
#include <stdint.h>

#define END 0xC0
#define ESC 0xDB
#define ESC_END 0xDC
#define ESC_ESC 0xDD

typedef struct {
  uint8_t length;
  uint8_t command;
  uint8_t data_size;
  uint16_t crc;
  uint8_t data[];

} OskarPacket;

uint8_t* getEscapedData(OskarPacket* packet);
uint8_t* getUnescapedData(uint8_t* escaped_data);
uint8_t checkCRC(uint16_t expected, uint8_t* data, uint8_t data_size);

#endif