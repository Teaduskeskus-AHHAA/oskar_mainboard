#include <include/gyems_x8.h>
#include <avr/io.h>

void gyems_x8_request_status(gyems_x8* motor) {
    // Needs check for recieve buffer empty conditions
    can_frame_t frm;
    // Motor status 1
    frm.SID = motor->id;
    frm.header.len = 8;
    frm.header.rtr = 0;
    frm.data[0] = READ_MOTOR_STATUS_1_ERROR_FLAGS;
    frm.data[1] = 0x00;
    frm.data[2] = 0x00;
    frm.data[3] = 0x00;
    frm.data[4] = 0x00;
    frm.data[5] = 0x00;
    frm.data[6] = 0x00;
    frm.data[7] = 0x00;

    mcp2515_send(&frm);
}

void gyems_x8_parse_can(gyems_x8* motor, can_frame_t* frame) {

  if(frame->SID == motor->id) {
    switch (frame->data[0])
    {
    case READ_MOTOR_STATUS_1_ERROR_FLAGS:
      motor->temperature = frame->data[1];
      motor->voltage = frame->data[3] + (frame->data[4] << 8);
      break;
    default:
      break;
    }
  }
}