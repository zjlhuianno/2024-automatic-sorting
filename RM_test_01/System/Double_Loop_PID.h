#ifndef __DOUBLE_LOOP_PID_H__
#define __DOUBLE_LOOP_PID_H__

#include <stdio.h>

//宏定义PID1参数。
#define Kp_position1 1.6//0.07f
#define Ki_position1 0.1//0.1f
#define Kd_position1 1.0//0.9f

//宏定义PID2参数。
#define Kp_position2 0.38//0.08f
#define Ki_position2 0.15f//0.5f//0.2f
#define Kd_position2 0.5//0.85f

//创建PID数值结构体。
typedef struct _PID
{
	float Kp;//比例系数。
	float Ki;//积分系数。
	float Kd;//微分系数。
	
	float error;//本次误差。
	float lastError;//上一次的误差。
	
	float integral;//积分，即误差的累计。
	float maxIntegral;//为了限制积分值，避免误差过大，而设置的积分上下限。
	
	float output;//PID输出。
	float maxOutput;//为了限制输出值，避免误差过大，而设置的输出上下限。
} PIDStruct;

void PID_Init(void);
float PID_Calculate(PIDStruct *pid, float setpoint, float actual_value);

#endif
