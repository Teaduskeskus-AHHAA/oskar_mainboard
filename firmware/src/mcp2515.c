#include <include/mcp2515.h>
#include <include/spi.h>

void mcp2515_set_mode(uint8_t mode) {
    SPI_unset_cs();
    SPI_send(BIT_MODIFY);
    SPI_send(CANCTRL);
    SPI_send(REQOP_MASK);
    SPI_send((mode << REQOP_OFFSET));
    SPI_set_cs();    
}

void mcp2515_enable_clkout() {
    SPI_unset_cs();
    SPI_send(BIT_MODIFY);
    SPI_send(CANCTRL);
    SPI_send(CLKEN);
    SPI_send(CLKEN);
    SPI_set_cs();
}

void mcp2515_disable_clkout() {
    SPI_unset_cs();
    SPI_send(BIT_MODIFY);
    SPI_send(CANCTRL);
    SPI_send(CLKEN);
    SPI_send(!CLKEN);
    SPI_set_cs();
}

void mcp2515_set_register(uint8_t reg, uint8_t data) {
    SPI_unset_cs();
    SPI_send(WRITE);
    SPI_send(reg);
    SPI_send(data);
    SPI_set_cs();
}

uint8_t mcp2515_read_register(uint8_t reg) {
    SPI_unset_cs();
    SPI_send(READ);
    SPI_send(reg);
    SPI_send(0x00);
    SPI_set_cs();
    return SPDR;
}

void mcp2515_load_message(uint8_t buffer, can_frame_t* frame) {
    SPI_unset_cs();
    SPI_send(LOAD_TX_BUFFER | buffer);
    SPI_send((frame->SID >> 3));
    SPI_send((frame->SID << 5));
    SPI_send(0);
    SPI_send(0);
    
    uint8_t length = frame->header.len & 0x0F;
    if (frame->header.rtr) {
        SPI_send((1<<RTR) | length);
    }
    else {
      SPI_send(length);       
	  uint8_t i;
      for (i=0; i < length; i++)
      {
        SPI_send(frame->data[i]);
      }
   }

    SPI_set_cs();

}

void mcp2515_request_to_send(uint8_t buffer) {
    uint8_t address = (buffer == 0) ? 1 : buffer;
    SPI_unset_cs();
    SPI_send(RTS | address);
    SPI_set_cs();
}

void mcp2515_abort_send(uint8_t buffer) {
    SPI_unset_cs();
    SPI_send(BIT_MODIFY);
    SPI_send(buffer);
    SPI_send(0x08);
    SPI_send(!0x08);
    SPI_set_cs();
}

uint8_t mcp2515_verify_register(uint8_t reg, uint8_t expected) {
	uint8_t reading = mcp2515_read_register(reg);
	return (reading == expected);
}
