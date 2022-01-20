#ifndef CAN_DEFS_H
#define CAN_DEFS_H

/* 16 MHz 1Mbps */
#define CNF1_16MHZ_1MBPS 0x00
#define CNF2_16MHZ_1MBPS 0x91
#define CNF3_16MHZ_1MBPS 0x01

// CANCTRL.REQOP bits to define operational modes
#define REQOP_NORMAL 0x00
#define REQOP_SLEEP 0x01
#define REQOP_LOOPBACK 0x02
#define REQOP_LISTEN 0x03
#define REQOP_CONFIG 0x04
#define REQOP_OFFSET 5
#define REQOP_MASK 0xE0

#define CLKEN 0x04

// LOAD_TX_BUFFER offests to point to the buffers
#define TXB2ADDR 4
#define TXB1ADDR 2
#define TXB0ADDR 0

// Things in DLC registers
#define RTR 6

// Interrupt stuff

#define RX1IE 0x01
#define RX0IE 0x02

#define B1BFS 5
#define B0BFS 4
#define B1BFE 3
#define B0BFE 2
#define B1BFM 1
#define B0BFM 0

/* -------------------    Instructions    ------------------- */

#define RESET 0xC0
#define READ 0x03
#define WRITE 0x02
#define READ_RX_BUFFER 0x90
#define LOAD_TX_BUFFER 0x40
#define READ_STATUS 0xA0
#define RTS 0x80
#define RX_STATUS 0xB0
#define BIT_MODIFY 0x05

/* -------------------     Registers     ------------------- */

// Transmit buffer 0 control
#define TXB0CTRL 0x30

// Transmit buffer 0 ID
#define TXB0SIDH 0x31
#define TXB0SIDL 0x32

// Transmit buffer 0 E(xtended)ID
#define TXB0EIDH 0x33
#define TXB0EIDL 0x34

// Transmit buffer 0 DLC (data length)
#define TXB0DLC 0x35

// Transmit buffer 0 data bytes
#define TXB0D0 0x36
#define TXB0D1 0x37
#define TXB0D2 0x38
#define TXB0D3 0x39
#define TXB0D4 0x3A
#define TXB0D5 0x3B
#define TXB0D6 0x3C
#define TXB0D7 0x3D

// Transmit buffer 1 Control
#define TXB1CTRL 0x40

// Transmit buffer 1 ID
#define TXB1SIDH 0x41
#define TXB1SIDL 0x42

// Transmit buffer 1 E(xtended)ID
#define TXB1EIDH 0x43
#define TXB1EIDL 0x44

// Transmit buffer 0 DLC (data length)
#define TXB1DLC 0x45

// Transmit buffer 0 data bytes
#define TXB1D0 0x46
#define TXB1D1 0x47
#define TXB1D2 0x48
#define TXB1D3 0x49
#define TXB1D4 0x4A
#define TXB1D5 0x4B
#define TXB1D6 0x4C
#define TXB1D7 0x4D

// Transmit buffer 2 Control
#define TXB2CTRL 0x50

// Transmit buffer 2 ID
#define TXB2SIDH 0x51
#define TXB2SIDL 0x52

// Transmit buffer 2 E(xtended)ID
#define TXB2EIDH 0x53
#define TXB2EIDL 0x54

// Transmit buffer 0 DLC (data length)
#define TXB2DLC 0x55

// Transmit buffer 0 data bytes
#define TXB2D0 0x56
#define TXB2D1 0x57
#define TXB2D2 0x58
#define TXB2D3 0x59
#define TXB2D4 0x5A
#define TXB2D5 0x5B
#define TXB2D6 0x5C
#define TXB2D7 0x5D

// Recieve buffer 0 control
#define RXB0CTRL 0x60

// Recieve buffer 0 ID
#define RXB0SIDH 0x61
#define RXB0SIDL 0x62

// Recieve buffer 0 E(xtended)ID
#define RXB0EIDH 0x63
#define RXB0EIDL 0x64

// Recieve buffer 0 DLC (data length)
#define RXB0DLC 0x65

// Recieve buffer 0 data bytes
#define RXB0D0 0x66
#define RXB0D1 0x67
#define RXB0D2 0x68
#define RXB0D3 0x69
#define RXB0D4 0x6A
#define RXB0D5 0x6B
#define RXB0D6 0x6C
#define RXB0D7 0x6D

// Recieve buffer 1 control
#define RXB1CTRL 0x70

// Recieve buffer 1 ID
#define RXB1SIDH 0x71
#define RXB1SIDL 0x72

// Recieve buffer 1 E(xtended)ID
#define RXB1EIDH 0x73
#define RXB1EIDL 0x74

// Recieve buffer 1 DLC (data length)
#define RXB1DLC 0x75

// Recieve buffer 1 data bytes
#define RXB1D0 0x76
#define RXB1D1 0x77
#define RXB1D2 0x78
#define RXB1D3 0x79
#define RXB1D4 0x7A
#define RXB1D5 0x7B
#define RXB1D6 0x7C
#define RXB1D7 0x7D

// Recieve acceptance filter 0 ID and EID
#define RXF0SIDH 0x00
#define RXF0SIDL 0x01
#define RXF0EIDH 0x02
#define RXF0EIDL 0x03

// Recieve acceptance filter 1 ID and EID
#define RXF1SIDH 0x04
#define RXF1SIDL 0x05
#define RXF1EIDH 0x06
#define RXF1EIDL 0x07

// Recieve acceptance filter 2 ID and EID
#define RXF2SIDH 0x08
#define RXF2SIDL 0x09
#define RXF2EIDH 0x0A
#define RXF2EIDL 0x0B

// Recieve acceptance filter 3 ID and EID
#define RXF3SIDH 0x10
#define RXF3SIDL 0x11
#define RXF3EIDH 0x12
#define RXF3EIDL 0x13

// Recieve acceptance filter 4 ID and EID
#define RXF4SIDH 0x14
#define RXF4SIDL 0x15
#define RXF4EIDH 0x16
#define RXF4EIDL 0x17

// Recieve acceptance filter 5 ID and EID
#define RXF5SIDH 0x18
#define RXF5SIDL 0x19
#define RXF5EIDH 0x1A
#define RXF5EIDL 0x1B

// Recieve acceptance mask 0 ID and EID
#define RXM0SIDH 0x20
#define RXM0SIDL 0x21
#define RXM0EIDH 0x22
#define RXM0EIDL 0x23

// RXnBF Pin control - control for RX Buffer full pins
#define BFPCTRL 0x0C

// TXnRTS Pin control - control for TX Buffer request to send pins
#define TXRTSCTRL 0x0D

// CAN status
#define CANSTAT 0x0E

// Can control
#define CANCTRL 0x0F

// Transmit error count
#define TEC 0x1C

// Reciefe error flag
#define REC 0x1D

// Bit timing configuration
#define CNF3 0x28
#define CNF2 0x29
#define CNF1 0x2A

// CAN Interrupt enable & interrupt flag
#define CANINTE 0x2B
#define CANINTF 0x2C

#define RX0IF 0x01
#define RX1IF 0x02

// Error flag
#define EFLG 0x2D

#define ABTF 0x40
#define MLOA 0x20
#define TXERR 0x10

#endif