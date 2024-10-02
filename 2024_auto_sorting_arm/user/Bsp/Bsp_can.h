#ifndef __BSP_CAN_H__
#define __BSP_CAN_H__
#include "can.h"

extern void can_filter_init(void);
void CAN_motor_Send(int16_t data1);

#endif
