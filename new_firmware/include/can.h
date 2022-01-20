#ifndef CAN_H
#define CAN_H

#include <stdint.h>

#include "include/candefs.h"

/* Structures */

typedef struct
{
  // SID of the target;
  uint32_t SID;
  // Frame header
  struct
  {
    unsigned int rtr : 1;
    unsigned int len : 4;
  } header;
  // Frame data
  uint8_t data[8];
} CAN_frame_t;

typedef enum
{
  CAN_ERROR_NONE = 0,
  CAN_ERROR_ALLTXBUSY = 1,
  CAN_ERROR_UNKNOWN_FLOW = 2,
  CAN_ERROR_ABORT = 3,
  CAN_ERROR_LOSTARBITATION = 4,
  CAN_ERROR_TXERR = 5
} CAN_error;

uint8_t CAN_init(uint8_t freq_meg, long baudrate);
void CAN_set_mode(uint8_t mode);

#endif  // CAN_H