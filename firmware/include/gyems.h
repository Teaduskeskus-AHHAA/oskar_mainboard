#ifndef GYEMS_X8_H
#define GYEMS_X8_H

#include <stdint.h>
#include <include/mcp2515.h>

#define READ_ACCELERATION 0x33
#define READ_ENCODER 0x90
#define READ_MULTITURN_ANGLE 0x92
#define SET_SPEED 0xA2
#define SET_MULTITURN_ANGLE 0xA4
#define READ_MOTOR_STATUS_1_ERROR_FLAGS 0x9A
#define READ_MOTOR_STATUS_2 0x9C


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
  int64_t multiturn_angle_range;
  uint16_t singleturn_angle;
  uint16_t singleturn_angle_0;
  uint16_t singleturn_angle_range;
  int8_t temperature;
  uint16_t voltage;
  uint8_t error_state;
  int32_t torque_current;
  int32_t speed;
  uint16_t maxSpeed;
  uint8_t spinDirection;
  int32_t endpoint_speed;
  int32_t endpoints_found;
  uint8_t endpoint_1_pin;
  uint8_t endpoint_2_pin;
  uint8_t endpoints_valid;
  volatile uint8_t * endpoint_1_port;
  volatile uint8_t * endpoint_2_port;
  uint8_t operating_mode;
} gyems_motor;

void gyems_motor_request_status(gyems_motor* motor);
void gyems_motor_parse_can(gyems_motor* motor, can_frame_t* frame);
void gyems_motor_set_speed(gyems_motor* motor, int32_t speed);
void gyems_motor_find_endpoints(gyems_motor* motor);
void gyems_motor_get_multiturn_angle(gyems_motor* motor);
void gyems_motor_parse_switches(gyems_motor* motor, uint8_t switchport);
void gyems_motor_set_multiturn_angle(gyems_motor* motor, uint16_t speed, int32_t angle);
#endif
