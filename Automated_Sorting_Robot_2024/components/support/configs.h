#include "struct_typedef.h"
#include "cmsis_os.h"
#include "PID.h"
#include "DM_Control.h"

extern uint8_t chassis_flag;
extern uint8_t arm_flag;
extern uint8_t servo_claw_flag;
extern uint8_t servo_sort_flag;
extern uint8_t servo_circle_flag;
extern uint8_t servo_release_flag;
extern int arm_flag_ok;




//以下都是机械臂的相关变量。
//***********************************************************//



//库变量的申明。
extern CAN_HandleTypeDef hcan2;
extern osThreadId arm_taskHandle;
extern osThreadId start_taskHandle;
extern osThreadId total_taskHandle;


//自己变量的申明。
extern float DM4340_speed_kp;
extern float DM4340_speed_ki;
extern float DM4340_speed_kd;
extern float DM4340_speed_max_out;
extern float DM4340_speed_max_iout;
extern float DM4340_speed_PID[3];
extern float DM4340_pos_kp;
extern float DM4340_pos_ki;
extern float DM4340_pos_kd;
extern float DM4340_pos_max_out;
extern float DM4340_pos_max_iout;
extern float DM4340_pos_PID[3];
extern float DM4310_speed_kp;
extern float DM4310_speed_ki;
extern float DM4310_speed_kd;
extern float DM4310_speed_max_out;
extern float DM4310_speed_max_iout;
extern float DM4310_speed_PID[3];
extern float DM4310_pos_kp;
extern float DM4310_pos_ki;
extern float DM4310_pos_kd;
extern float DM4310_pos_max_out;
extern float DM4310_pos_max_iout;
extern float DM4310_pos_PID[3];
extern DM_Motor_t DM4340_Data,DM4310_Data;
extern pid_type_def DM4340_speed_pid, DM4310_speed_pid;
extern pid_type_def DM4340_pos_pid, DM4310_pos_pid;
extern float DM4340_LPF_K;
extern float DM4310_LPF_K;
extern float DM4340_target_speed;
extern float DM4340_target_pos;
extern float DM4310_target_speed;
extern float DM4310_target_pos;
extern float arms_js_data[4];
extern float Servo_target_angle;
extern int DM4310_target_pos_int;
extern int DM4340_target_pos_int;
extern float target_x_follow;
extern float target_y_follow;
extern float target_angle_follow;
extern float target_x;
extern float target_y;
extern float target_angle;
extern int DM4310_delta_enc;
extern int last_DM4310_delta_enc;
extern uint8_t flag_z;
extern uint8_t force_sensing_flag;	
extern int arm_catch_flag;
