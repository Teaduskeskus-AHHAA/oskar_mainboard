#include "include/can.h"
#include "include/spi.h"

void CAN_set_mode(uint8_t mode) {
  // TODO: UNSET CS
  SPI_write(BIT_MODIFY);
  SPI_write(CANCTRL);
  SPI_write(REQOP_MASK);
  SPI_write((mode << REQOP_OFFSET));
  //TODO: SET CS
}

uint8_t CAN_init(uint8_t freq_meg, long baudrate) {
  uint8_t cnf1, cnf2, cnf3;
  CAN_set_mode(REQOP_CONFIG);
}