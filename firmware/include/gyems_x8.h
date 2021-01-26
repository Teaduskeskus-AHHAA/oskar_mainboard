#ifndef GYEMS_X8_H
#define GYEMS_X8_H

#include <stdint.h>
#include <include/mcp2515.h>

#define READ_MOTOR_STATUS_1_ERROR_FLAGS 0x9A


typedef struct {
    uint16_t current;
    uint16_t original;
    uint16_t offset;
} encoder_t;

typedef struct {
  uint16_t id;
  uint8_t status;
  encoder_t encoder;
  int64_t multiturn_angle;
  int64_t multiturn_angle_0;
  uint16_t singleturn_angle;
  uint16_t singleturn_angle_0;
  int8_t temperature;
  uint16_t voltage;
  uint8_t error_state;
  int16_t torque_current;
  int32_t speed;
  uint16_t maxSpeed;
  uint8_t spinDirection;
} gyems_x8;

void gyems_x8_request_status(gyems_x8* motor);
void gyems_x8_parse_can(gyems_x8* motor, can_frame_t* frame);

#endif