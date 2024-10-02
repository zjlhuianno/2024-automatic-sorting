#ifndef __TOTAL_TASK_H__
#define __TOTAL_TASK_H__

#include "main.h"

void Total_Task(void const * argument);

void catch_object(uint8_t flag);
void set_arm_pos_param(float target_x_param, float target_y_param, float target_angle_param, float true_target_angle_param);
void pos_stable_function(uint8_t is_last_pos_frame, uint8_t arm_catch_mode);

#endif
