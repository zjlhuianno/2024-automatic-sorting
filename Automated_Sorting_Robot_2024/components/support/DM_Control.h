#ifndef __DMPOWER__
#define __DMPOWER__

#include "main.h"

#define DM4340_ID  0x01   //1电机ID
#define DM4310_ID  0x02   //2电机ID
#define P_MIN   -12.5  //位置最小值
#define P_MAX   12.5   //位置最大值
#define V_MIN   -45    //速度最小值
#define V_MAX   45     //速度最大值
#define KP_MIN  0      //Kp最小值
#define KP_MAX  500    //Kp最大值
#define KD_MIN  0      //Kd最小值
#define KD_MAX  5      //Kd最大值
#define T_MIN   -18    //转矩最大值
#define T_MAX   18     //转矩最小值

#define DM4340_MAX_P_INT 	16383	//DM4340编码器转过一圈以后的最大编码值。//16383//32767
#define DM4310_MAX_P_INT 	16383	//DM4340编码器转过一圈以后的最大编码值。//16383//32767

#define DM4340_ZERO_POS_OFFSET		2785	//DM4340的编码器值p_int的零位置偏置，每次拆开后重装须更改。
#define DM4310_ZERO_POS_OFFSET		2848-5696	//DM4310的编码器值p_int的零位置偏置，每次拆开后重装须更改。

//电机结构体
typedef struct
{
	int state;//电机的状态位，即手册中的ERR位。
	/*
	ERR位解释如下：
	0——失能；
	1——使能；
	8——超压；
	9——欠压；
	A——过电流；
	B——MOS 过温；
	C——电机线圈过温；
	D——通讯丢失；
	E——过载；
	*/
	
  int p_int;//int类型的位置。
  int v_int;//int类型的速度。
  int t_int;//int类型的转矩。
	
  float pos;//float类型的位置。
  float vel;//float类型的速度。
  float torq;//float类型的转矩。
	
	int last_p_int;//上一次位置(int型)。
	float last_vel;//上一次速度。
	
	float target_torq;//目标转矩。
	
} DM_Motor_t;

int DM4340_angle_to_enc_p_int(float angle);
float DM4340_enc_p_int_to_angle(int enc);
int DM4310_angle_to_enc_p_int(float angle);
float DM4310_enc_p_int_to_angle(int enc);
float radian_to_degree(float radian);
float Inferior_arc_treatment(float ref,float set,float max);

void DM_PID_Init(void);
void DM_Data_Init(void);
void Enable_DM_Motor(CAN_HandleTypeDef* hcan,uint8_t ID);
void Disable_DM_Motor(CAN_HandleTypeDef* hcan,uint8_t Motor_ID);

void MIT_Ctrl_DM_Motor(CAN_HandleTypeDef* hcan,uint16_t ID, float _pos, float _vel,float _KP, float _KD, float _torq);
void PosSpeed_CtrlMotor(CAN_HandleTypeDef* hcan, uint16_t ID, float _pos, float _vel);
void Save_Pos_Zero(CAN_HandleTypeDef* hcan, uint16_t Motor_ID);
void Get_DM4340_Data(DM_Motor_t *DM4340_Data, uint8_t* Rx_Data);
void Get_DM4310_Data(DM_Motor_t *DM4310_Data, uint8_t* Rx_Data);
void DM_PID_Speed_Ctrl(uint8_t ID, float DM_target_speed);
void DM_PID_Pos_Speed_Ctrl(uint8_t ID, float DM_target_pos);
void ramp_function(float *data_follow,float data_target,float k);
void Arm_Ctrl(float target_x, float target_y, float target_angle);

#endif

