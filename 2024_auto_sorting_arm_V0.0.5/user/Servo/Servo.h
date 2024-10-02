#ifndef __SERVO_H__
#define __SERVO_H__

#include "main.h"

float Servo_arm_angle_to_pwm(int16_t angle);

void Servo_Ctrl_arm(int16_t angle_for_servo_arm);
void Servo_Ctrl_claw(uint8_t ctrl_flag);
void Servo_Ctrl_3(uint8_t ctrl_flag);
void Servo_Ctrl_5(uint8_t ctrl_flag);
void Servo_Ctrl_6(uint8_t ctrl_flag);
void Servo_Init(void);
void Servo_Ctrl(uint8_t servo_id, int16_t angle);

#endif
