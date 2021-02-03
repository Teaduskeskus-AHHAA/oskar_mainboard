#include <include/gyems.h>
#include <avr/io.h>

void gyems_motor_request_status(gyems_motor* motor) {
  // Needs check for recieve buffer empty conditions
  can_frame_t frm;
  // Motor status 1
  frm.SID = motor->id;
  frm.header.len = 8;
  frm.header.rtr = 0;
  frm.data[0] = READ_MOTOR_STATUS_1_ERROR_FLAGS;
  mcp2515_send(&frm);

  frm.data[0] = READ_MOTOR_STATUS_2;
  mcp2515_send(&frm);

}

void gyems_motor_get_multiturn_angle(gyems_motor* motor) {
  can_frame_t frm;
  frm.SID = motor->id;
  frm.header.len = 8;
  frm.header.rtr = 0;
  frm.data[0] = READ_MULTITURN_ANGLE;
  mcp2515_send(&frm);
}

void gyems_motor_parse_can(gyems_motor* motor, can_frame_t* frame) {

  if(frame->SID == motor->id) {
    switch (frame->data[0])
    {
      case READ_MOTOR_STATUS_1_ERROR_FLAGS:
        motor->temperature = frame->data[1];
        motor->voltage = frame->data[3] | (frame->data[4] << 8);
        motor->error_state = frame->data[7];
        break;
      case READ_MOTOR_STATUS_2:
        motor->temperature = frame->data[1];
        motor->torque_current = frame->data[2] | (frame->data[3] << 8);
        motor->speed = frame->data[4] | (frame->data[5] << 8);
        motor->encoder.current = frame->data[6] | (frame->data[7] << 8);
        break;
      case READ_MULTITURN_ANGLE:
        if(motor->endpoints_valid == 0) {
          motor->multiturn_angle_0 =  (int64_t) frame->data[1] | ((int64_t) frame->data[2] << 8) | ((int64_t) frame->data[3] << 16) | ((int64_t) frame->data[4]  << 24) | ((int64_t) frame->data[5] << 32) | 
          ((int64_t) frame->data[6] << 40) | ((int64_t) frame->data[7] << 48); 
          motor->endpoints_valid = 1;
          break;
        } else {
          motor->multiturn_angle = (int64_t) frame->data[1] | ((int64_t) frame->data[2] << 8) | ((int64_t) frame->data[3] << 16) | ((int64_t) frame->data[4]  << 24) | ((int64_t) frame->data[5] << 32) | 
        ((int64_t) frame->data[6] << 40) | ((int64_t) frame->data[7] << 48); 
        break;
        }
	break;
    default:
      break;
    }
  }
}

void gyems_motor_set_speed(gyems_motor* motor, int32_t speed) {
  speed = speed*100;
  can_frame_t frm;
  // Motor status 1
  frm.SID = motor->id;
  frm.header.len = 8;
  frm.header.rtr = 0;
  frm.data[0] = SET_SPEED;
  frm.data[4] = *(uint8_t*)(&speed);
  frm.data[5] = *((uint8_t*)(&speed)+1);
  frm.data[6] = *((uint8_t*)(&speed)+2);
  frm.data[7] = *((uint8_t*)(&speed)+3);

  mcp2515_send(&frm);
}

void gyems_motor_find_endpoints(gyems_motor* motor) {
  motor->endpoints_valid = 0;
  gyems_motor_set_speed(motor, motor->endpoint_speed);
  while((*(motor->endpoint_1_port - 2)) & (1 << motor->endpoint_1_pin)) {}
  gyems_motor_set_speed(motor,0);
  gyems_motor_get_multiturn_angle(motor);

}

void gyems_motor_parse_switches(gyems_motor* motor, uint8_t switchport) {
  if(! (switchport & (1 << motor->endpoint_1_pin))) {
	if(motor->endpoints_valid == 0) {
		if(motor->endpoints_found == 0) {
			motor->endpoints_found = 1;
			gyems_motor_find_endpoints(motor);
		}
	}
  }
}

void gyems_motor_set_multiturn_angle(gyems_motor* motor, uint16_t speed, int32_t angle) {
  while(motor->multiturn_angle_0 == 0) {};
  speed = speed*100;
  if(motor->multiturn_angle_range > 0) {
    // Uus angle peab olema _0+angle
    int32_t workAngle = motor->multiturn_angle_0+(angle*600);

    if(workAngle <= (motor->multiturn_angle_0+(motor->multiturn_angle_range*600))) {
      can_frame_t frm;
        // Motor status 1
        frm.SID = motor->id;
        frm.header.len = 8;
        frm.header.rtr = 0;
        frm.data[0] = SET_MULTITURN_ANGLE;
        frm.data[2] = *(uint8_t*)(&speed);
        frm.data[3] = *((uint8_t*)(&speed)+1);
        frm.data[4] = *((uint8_t*)(&workAngle));
        frm.data[5] = *((uint8_t*)(&workAngle)+1);
        frm.data[6] = *((uint8_t*)(&workAngle)+2);
        frm.data[7] = *((uint8_t*)(&workAngle)+3);
        mcp2515_send(&frm);

    }
  } else if(motor->multiturn_angle_range < 0) {
    int32_t workAngle = motor->multiturn_angle_0-(angle*600);
    if(workAngle <= (motor->multiturn_angle_0-(motor->multiturn_angle_range*600))) {
        can_frame_t frm;
        // Motor status 1
        frm.SID = motor->id;
        frm.header.len = 8;
        frm.header.rtr = 0;
        frm.data[0] = SET_MULTITURN_ANGLE;
        frm.data[2] = *(uint8_t*)(&speed);
        frm.data[3] = *((uint8_t*)(&speed)+1);
        frm.data[4] = *((uint8_t*)(&workAngle));
        frm.data[5] = *((uint8_t*)(&workAngle)+1);
        frm.data[6] = *((uint8_t*)(&workAngle)+2);
        frm.data[7] = *((uint8_t*)(&workAngle)+3);
        mcp2515_send(&frm);
    }
  }
}
