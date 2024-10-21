#ifndef __ARM_TASK_H__
#define __ARM_TASK_H__

#include "main.h"

void Arm_Task(void const * argument);



//机械臂相关控制函数的封装。
void look_object(uint8_t mode);
void catch_object(uint8_t mode);
void push_ball(uint8_t mode);
void set_arm_pos_param(float target_x_param, float target_y_param, float target_angle_param, float true_target_angle_param);
void pos_stable_function(uint8_t is_last_pos_frame, uint8_t arm_catch_mode);
void look_stable_function(uint8_t is_last_pos_frame, uint8_t claw_mode);

#endif

