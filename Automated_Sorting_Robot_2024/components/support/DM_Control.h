#ifndef __DMPOWER__
#define __DMPOWER__

#include "main.h"

#define DM4340_ID  0x01   //1���ID
#define DM4310_ID  0x02   //2���ID
#define P_MIN   -12.5  //λ����Сֵ
#define P_MAX   12.5   //λ�����ֵ
#define V_MIN   -45    //�ٶ���Сֵ
#define V_MAX   45     //�ٶ����ֵ
#define KP_MIN  0      //Kp��Сֵ
#define KP_MAX  500    //Kp���ֵ
#define KD_MIN  0      //Kd��Сֵ
#define KD_MAX  5      //Kd���ֵ
#define T_MIN   -18    //ת�����ֵ
#define T_MAX   18     //ת����Сֵ

#define DM4340_MAX_P_INT 	16383	//DM4340������ת��һȦ�Ժ��������ֵ��//16383//32767
#define DM4310_MAX_P_INT 	16383	//DM4340������ת��һȦ�Ժ��������ֵ��//16383//32767

#define DM4340_ZERO_POS_OFFSET		2785	//DM4340�ı�����ֵp_int����λ��ƫ�ã�ÿ�β𿪺���װ����ġ�
#define DM4310_ZERO_POS_OFFSET		2848-5696	//DM4310�ı�����ֵp_int����λ��ƫ�ã�ÿ�β𿪺���װ����ġ�

//����ṹ��
typedef struct
{
	int state;//�����״̬λ�����ֲ��е�ERRλ��
	/*
	ERRλ�������£�
	0����ʧ�ܣ�
	1����ʹ�ܣ�
	8������ѹ��
	9����Ƿѹ��
	A������������
	B����MOS ���£�
	C���������Ȧ���£�
	D����ͨѶ��ʧ��
	E�������أ�
	*/
	
  int p_int;//int���͵�λ�á�
  int v_int;//int���͵��ٶȡ�
  int t_int;//int���͵�ת�ء�
	
  float pos;//float���͵�λ�á�
  float vel;//float���͵��ٶȡ�
  float torq;//float���͵�ת�ء�
	
	int last_p_int;//��һ��λ��(int��)��
	float last_vel;//��һ���ٶȡ�
	
	float target_torq;//Ŀ��ת�ء�
	
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

