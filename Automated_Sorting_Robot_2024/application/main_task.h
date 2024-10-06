#include "main.h"
#include "cmsis_os.h"
#include "configs.h"


void main_task(void const * argument);




//main_task中机械臂相关控制函数的封装。
void catch_object(uint8_t mode);
void set_arm_pos_param(float target_x_param, float target_y_param, float target_angle_param, float true_target_angle_param);
void pos_stable_function(uint8_t is_last_pos_frame, uint8_t arm_catch_mode);
