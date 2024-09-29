#include "Includes.h"



//**************PID参数******************//

	//速度环PI控制。
	float DM4340_speed_kp=0.21300000012f;//0.85f
	float DM4340_speed_ki=0.000106123481f;//0.08f
	float DM4340_speed_kd=0.0f;
	float DM4340_speed_max_out=10.0f;
	float DM4340_speed_max_iout=12.0f;
	float DM4340_speed_PID[3];

	//位置环PD控制。
	float DM4340_pos_kp=0.06f;//22.0f//0.00025f
	float DM4340_pos_ki=0.0f;
	float DM4340_pos_kd=0.20f;//1500.0f//0.049f//最新0.09
	float DM4340_pos_max_out=28.0f;
	float DM4340_pos_max_iout=28.0f;
	float DM4340_pos_PID[3];


	//********************************//
	
	//速度环PI控制。
	float DM4310_speed_kp=0.0500000007f;
	float DM4310_speed_ki=0.00800000038f;
	float DM4310_speed_kd=0.0f;
	float DM4310_speed_max_out=10.0f;
	float DM4310_speed_max_iout=10.0f;
	float DM4310_speed_PID[3];

	//位置环PD控制。
	float DM4310_pos_kp=0.0045f;
	float DM4310_pos_ki=0.0f;
	float DM4310_pos_kd=3.05f;//最新3.05
	float DM4310_pos_max_out=35.0f;
	float DM4310_pos_max_iout=35.0f;
	float DM4310_pos_PID[3];
//**************END******************//


//结构体变量。
DM_Motor_t DM4340_Data,DM4310_Data;//DM4340，DM4310电机的数据。
pid_type_def DM4340_speed_pid, DM4310_speed_pid;//DM4340，DM4310电机的速度环PID数据。
pid_type_def DM4340_pos_pid, DM4310_pos_pid;//DM4340，DM4310电机的位置环PID数据。


float DM4340_LPF_K = 0.25f;		//DM4310的低通滤波LPF(Lowpass filtering)参数。
float DM4310_LPF_K = 0.15f;		//DM4310的低通滤波LPF(Lowpass filtering)参数。

float DM4340_target_speed;//DM4340的目标速度。
float DM4340_target_pos;  //DM4340的目标位置。
float DM4310_target_speed;//DM4310的目标速度。
float DM4310_target_pos;  //DM4310的目标位置。


float arms_js_data[4] = { 0.0f, 0.0f, 0.0f, 0.0f };//姿态解算后得到的数据。
										//arms_js_data[0]：机械臂舵机转过的角度。
										//arms_js_data[1]：DM4310转过的角度。
										//arms_js_data[2]：DM4340转过的角度。
										//arms_js_data[3]：解算是否正确的标志位，1表示解算错误。

float Servo_target_angle;		//机械臂舵机的目标位置。
int DM4310_target_pos_int;  //DM4310的目标位置。(int型。)
int DM4340_target_pos_int;  //DM4340的目标位置。(int型。)

float target_x_follow; 	//目标x坐标的追随值。
float target_y_follow;		//目标y坐标的追随值。
float target_angle_follow;//目标角度的追随值。

float target_x;			//机械臂解算时输入的目标x坐标。
float target_y;			//机械臂解算时输入的目标y坐标。
float target_angle;	//机械臂解算时输入的目标角度。

int DM4310_delta_enc = 0;//在task里面一直记录DM4310编码器的p_int与last_p_int的差值。
int last_DM4310_delta_enc = 0;//上一次的DM4310_delta_enc。

//标志位。（1为是，0为否。）
uint8_t flag_z=0;										//检测程序是否卡在了这里。
uint8_t force_sensing_flag = 0;	//是否进行力控感知。
