#include <avr/io.h>
#include <avr/interrupt.h>
#include <include/spi.h>
#include <include/mcp2515.h>

can_frame_t can_frame;
uint8_t can_frame_ready = 0;

void stall_can_init_error() {
	cli();
	while(1) {

	}
}

int main() {

	/* Start Initialize peripheral systems */

	// Initialize SPI
	SPI_init();

	// Initialize CAN Controller or enter hard error state if that fails.
	if(!mcp2515_init(16,1000000)) {
		stall_can_init_error();
	}

	/* End Initialize peripheral systems */

	while(1) {
		if(can_frame_ready) {
			mcp2515_load_message(TXB0ADDR,&can_frame);
			mcp2515_request_to_send(TXB0ADDR);
			can_frame_ready = 0;
		}
	}
	
	return 0;
}