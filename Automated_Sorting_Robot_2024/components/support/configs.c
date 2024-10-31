#include "configs.h"

uint8_t chassis_work_flag = 0;
uint8_t work_flag = 0;
uint8_t odometryResetFlag = 0;
uint8_t ins_init_flag = 0;
uint8_t cross_flag = 0;
uint8_t target_cross_flag = 0;
uint8_t gray_stop_flag = 0;


uint8_t openmv_look_flag = 0;

/*
机械臂
chassis_arm_comm_flag ==
0 收起状态
1 对准圆盘机

2 阶梯平台
2 对准立桩中心白球
3 对准立桩边缘
4 对准台阶物品
5 对准收物处
*/

//底盘与机械臂交互。
uint8_t chassis_arm_comm_flag = 0;//底盘与机械臂交流标志位。
uint8_t height_flag = 0;


/*
拨夹爪
servo_claw_flag == 
0 闭合，给值或失能，给值有点担心烧坏
1 开
2 夹
*/
uint8_t servo_claw_flag = 0;

/*
分类舵机
servo_sort_flag ==
0 加分球
1 黄球
*/
uint8_t servo_sort_flag = 0;

/*
圆环舵机
servo_circle_flag == 
0 收紧
1 打开
*/
uint8_t servo_circle_flag =0;

/*
放球舵机
servo_release_flag = 
0 收起
1 放下
*/
uint8_t servo_release_flag = 0;

int arm_flag_ok=0;

uint8_t mode_openmv = 0;
//以下都是openmv的相关变量。
//***********************************************************//


//以下都是机械臂的相关变量。
//***********************************************************//
float offset_4310_angle = 0.0f;

//**************PID参数******************//

	//速度环PI控制。
	float DM4340_speed_kp=0.21300000012f;
	float DM4340_speed_ki=0.000106123481f;
	float DM4340_speed_kd=0.0f;
	float DM4340_speed_max_out=10.0f;
	float DM4340_speed_max_iout=12.0f;
	float DM4340_speed_PID[3];

	//位置环PD控制。
	float DM4340_pos_kp=0.20f;//0.25f
	float DM4340_pos_ki=0.0f;
	float DM4340_pos_kd=0.20f;//0.20f
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
	float DM4310_pos_kp=0.10f;//0.30f
	float DM4310_pos_ki=0.0f;
	float DM4310_pos_kd=13.0f;//13.0f
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

uint8_t flag_z=0;				//检测程序是否卡在了这里。
uint8_t force_sensing_flag = 0;	//是否进行力控感知。
int begin_look_flag = 0;//开始看动作的标志位。
uint8_t arm_flag = 0;//控制机械臂阶梯平台动作衔接的标志位。
uint8_t arm_flag_last = 0;//控制机械臂阶梯平台动作衔接的标志位。
uint8_t just_once_flag = 0;//保证机械臂放回甜甜圈时只设置一次舵机的延时。（因为只有这个动作组需要舵机设置延时。）

//直接控制舵机的标志位。
uint8_t arm_catch_flag=99;//夹爪舵机姿态标志位。
uint8_t pile_approach_flag = 99;//甜甜圈塑料桩标志位。
uint8_t shunt_ball_flag = 99;//分流轨道标志位。
uint8_t ball_in_flag = 99;//进口接方块或接球。
uint8_t ball_out_flag = 99;//出口。

//机械臂与openmv有关的标志位。
uint8_t openmv_ok_flag = 0;//openmv是否识别到了数据。
uint8_t should_push_ball_flag_openmv = 1;//是否要拨球。
uint8_t ball_color_flag_openmv = 0;//识别到的球的颜色。（0为己方球，1为中立球。）
uint8_t no_ball_push_flag_openmv = 0;//是否识别到无球可拨。



