#ifndef CAN_RECEIVE_H
#define CAN_RECEIVE_H

#include "main.h"
#include "can.h"
#include "VOFA.h"

#define CAN_CHASSIS_ALL_ID 0x200
#define CAN_2006_M1_ID 0x201
#define CAN_2006_M2_ID 0x202
#define CAN_2006_M3_ID 0x203
#define CAN_2006_M4_ID 0x204

typedef struct
{
    uint16_t ecd;
    int16_t speed_rpm;
    int16_t given_current;
    uint8_t temperate;
    int16_t last_ecd;
	int16_t round;
	int32_t code;//зЊзг
} motor_measure_t;
extern float odometryResetFlag;
extern void CAN_cmd_chassis(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
extern void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
extern const motor_measure_t *get_chassis_motor_measure_point(uint8_t i);
#endif

