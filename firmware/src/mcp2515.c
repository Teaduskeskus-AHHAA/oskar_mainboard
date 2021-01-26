#include <include/mcp2515.h>
#include <include/spi.h>

uint8_t mcp2515_verify_register(uint8_t reg, uint8_t expected) {
	uint8_t reading = mcp2515_read_register(reg);
	return (reading == expected);
}

uint8_t mcp2515_check_bit(uint8_t reg, uint8_t bitmask) {
    return mcp2515_read_register(reg) & bitmask;
}

void mcp2515_set_register(uint8_t reg, uint8_t data) {
    SPI_unset_cs();
    SPI_send(WRITE);
    SPI_send(reg);
    SPI_send(data);
    SPI_set_cs();
}

void mcp2515_set_mode(uint8_t mode) {
    SPI_unset_cs();
    SPI_send(BIT_MODIFY);
    SPI_send(CANCTRL);
    SPI_send(REQOP_MASK);
    SPI_send((mode << REQOP_OFFSET));
    SPI_set_cs();    
}

uint8_t mcp2515_init(uint8_t freq_MHz, long baud) {
    uint8_t cnf1, cnf2, cnf3;
    mcp2515_set_mode(REQOP_CONFIG);
	while (!mcp2515_check_bit(CANSTAT,0x80)) {}
    switch (freq_MHz)
    {
    case 16:
        switch (baud)
        {
        case 1000000:
            cnf1 = CNF1_16MHZ_1MBPS;
            cnf2 = CNF2_16MHZ_1MBPS;
            cnf3 = CNF3_16MHZ_1MBPS;
            break;
        
        default:
            return 0;
        }
        break;
    
    default:
        return 0;
    }
    mcp2515_set_register(CNF1,cnf1);
	mcp2515_set_register(CNF2,cnf2);
	mcp2515_set_register(CNF3,cnf3);
    uint8_t result = mcp2515_verify_register(CNF1,cnf1) && mcp2515_verify_register(CNF2,cnf2) && mcp2515_verify_register(CNF3,cnf3);
    mcp2515_set_register(CANINTE,(1 << RX0IE) | (1 << RX1IE));
    mcp2515_set_register(BFPCTRL,(1 << B0BFM) | (1 << B1BFM) | (1 << B0BFE) | (1 << B1BFE));
    mcp2515_set_mode(REQOP_NORMAL);
    while(!mcp2515_verify_register(CANSTAT,0x00)){};

    return result;
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

void mcp2515_read_registers(uint8_t start, uint8_t target[], uint8_t len) {
    SPI_unset_cs();
    SPI_send(READ);
    SPI_send(start);
    for(int i = 0; i < len; i++) {
        SPI_send(0x00);
        target[i] = SPDR;
    }
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

CAN_error mcp2515_send(can_frame_t* frame) {
    // Check if buffers are busy to prevent messing up any frames that have not yet been successfully sent
    uint8_t txbuffer_full[3];
    txbuffer_full[0] = mcp2515_check_bit(TXB0CTRL,0x08);
    txbuffer_full[1] = mcp2515_check_bit(TXB1CTRL,0x08);
    txbuffer_full[2] = mcp2515_check_bit(TXB2CTRL,0x08);

    int selected_buffer = -1;

    for(int i = 0; i <3; i++) {
        if(!txbuffer_full[i]) {
            selected_buffer = i;
            break;
        }
    }
    if(selected_buffer == -1) {
        return CAN_ERROR_ALLTXBUSY;
    }
    
    uint8_t abort, lost,txerr;

    switch (selected_buffer)
    {
    case 0:
        mcp2515_load_message(TXB0ADDR, frame);
        mcp2515_request_to_send(TXB0ADDR);
        abort = mcp2515_check_bit(TXB0ADDR,ABTF);
        lost = mcp2515_check_bit(TXB0ADDR, MLOA);
        txerr = mcp2515_check_bit(TXB0ADDR, TXERR);
        break;
    case 1:
        mcp2515_load_message(TXB1ADDR, frame);
        mcp2515_request_to_send(TXB1ADDR);
        abort = mcp2515_check_bit(TXB1ADDR,ABTF);
        lost = mcp2515_check_bit(TXB1ADDR, MLOA);
        txerr = mcp2515_check_bit(TXB1ADDR, TXERR);
        break;  
    case 2:
        mcp2515_load_message(TXB2ADDR, frame);
        mcp2515_request_to_send(TXB2ADDR);
        abort = mcp2515_check_bit(TXB2ADDR,ABTF);
        lost = mcp2515_check_bit(TXB2ADDR, MLOA);
        txerr = mcp2515_check_bit(TXB2ADDR, TXERR);
        break;
    default:
        return CAN_ERROR_UNKNOWN_FLOW;
    }

    if(abort) {
        return CAN_ERROR_ABORT;
    } 
    if(lost) {
        return CAN_ERROR_LOSTARBITATION;
    }
    if(txerr) {
        return CAN_ERROR_TXERR;
    }

    return CAN_ERROR_NONE;
}


void mcp2515_read(uint8_t buffer, can_frame_t* frame) {
    uint8_t metadata[5];
    if(buffer == 0) {
        mcp2515_read_registers(RXB0SIDH, metadata, 5);
        mcp2515_read_registers(RXB0D0,frame->data,8);
        SPI_unset_cs();
        SPI_send(BIT_MODIFY);
        SPI_send(CANINTF);
        SPI_send(RX0IF);
        SPI_send(!RX0IF);
        SPI_set_cs();
    } else if (buffer == 1) {
        mcp2515_read_registers(RXB1SIDH, metadata, 5);
        mcp2515_read_registers(RXB1D0,frame->data,8);
        SPI_unset_cs();
        SPI_send(BIT_MODIFY);
        SPI_send(CANINTF);
        SPI_send(RX1IF);
        SPI_send(!RX1IF);
        SPI_set_cs();
    }
    frame->SID = (metadata[0] << 3) | (metadata[1] >> 5);
    
    frame->header.len = metadata[4];
}