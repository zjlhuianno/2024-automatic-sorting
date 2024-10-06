#include "DM_Control.h"
#include "Arm_Posture_Calculate.h"
#include "math.h"
#include "stdlib.h"
#include "configs.h"

/**
 * @brief  uintתfloat
 * @param  x_int   ���ֵ
 * @param  x_min   ��Сֵ
 * @param  bits    λ��
 */
float uint_to_float(int x_int, float x_min, float x_max, int bits)
{
  float span = x_max - x_min;
  float offset = x_min;
  return ((float)x_int)*span/((float)((1<<bits)-1)) + offset;
}

/**
 * @brief  floatתuint
 * @param  x_int   ���ֵ
 * @param  x_min   ��Сֵ
 * @param  bits    λ��
 */
int float_to_uint(float x, float x_min, float x_max, int bits)
{ 
  float span = x_max - x_min;
  float offset = x_min;
  return (int) ((x-offset)*((float)((1<<bits)-1))/span);
}


//�Ƕ������λ�õĻ��㡣
int DM4340_angle_to_enc_p_int(float angle)
{
	return (int)( DM4340_MAX_P_INT * (angle/360.0f) );
}

float DM4340_enc_p_int_to_angle(int enc)
{
	return ((float)enc*360.0f)/DM4340_MAX_P_INT;
}

int DM4310_angle_to_enc_p_int(float angle)
{
	int enc_p_int = (int)( DM4340_MAX_P_INT * (angle/360.0f) );
	//return DM4310_MAX_P_INT - enc_p_int;
	return enc_p_int;
}

float DM4310_enc_p_int_to_angle(int enc)
{
	float angle = ((float)enc*360.0f)/DM4310_MAX_P_INT;
	//return 360 - angle;//��Ϊ4310��Ҫ��ת��
	return angle;
}

//������ת�ɽǶ��ơ�
float radian_to_degree(float radian)
{
	return radian * 57.3f;
}

//����������
float Inferior_arc_treatment(float ref,float set,float max)
{
    if(ref-set>max/2){
        return ref-max;
    }
    else if(ref-set<-max/2)
    {
        return ref+max;
    }
    else{
        return ref;
    }
}

//DM4340��DM4310��PID��ʼ����
void DM_PID_Init(void)
{
	//DM4340�ٶȻ���
	DM4340_speed_PID[0]=DM4340_speed_kp;
	DM4340_speed_PID[1]=DM4340_speed_ki;
	DM4340_speed_PID[2]=DM4340_speed_kd;
	PID_init(&DM4340_speed_pid,PID_POSITION_ARM,DM4340_speed_PID,DM4340_speed_max_out,DM4340_speed_max_iout);

	//DM4340λ�û���
	DM4340_pos_PID[0]=DM4340_pos_kp;
	DM4340_pos_PID[1]=DM4340_pos_ki;
	DM4340_pos_PID[2]=DM4340_pos_kd;
	PID_init(&DM4340_pos_pid,PID_POSITION_ARM,DM4340_pos_PID,DM4340_pos_max_out,DM4340_pos_max_iout);

	//DM4310�ٶȻ���
	DM4310_speed_PID[0]=DM4310_speed_kp;
	DM4310_speed_PID[1]=DM4310_speed_ki;
	DM4310_speed_PID[2]=DM4310_speed_kd;
	PID_init(&DM4310_speed_pid,PID_POSITION_ARM,DM4310_speed_PID,DM4310_speed_max_out,DM4310_speed_max_iout);
	
	//DM4310λ�û���
	DM4310_pos_PID[0]=DM4310_pos_kp;
	DM4310_pos_PID[1]=DM4310_pos_ki;
	DM4310_pos_PID[2]=DM4310_pos_kd;
	PID_init(&DM4310_pos_pid,PID_POSITION_ARM,DM4310_pos_PID,DM4310_pos_max_out,DM4310_pos_max_iout);
	
}

void DM_Data_Init(void)
{
	DM4310_Data.state=0;
	DM4310_Data.p_int=0;
	DM4310_Data.v_int=0;
	DM4310_Data.t_int=0;
	DM4310_Data.pos=0.0f;
	DM4310_Data.vel=0.0f;
	DM4310_Data.torq=0.0f;
	DM4310_Data.last_p_int=0;
	DM4310_Data.last_vel=0.0f;
	DM4310_Data.target_torq=0.0f;
	
	DM4340_Data.state=0;
	DM4340_Data.p_int=0;
	DM4340_Data.v_int=0;
	DM4340_Data.t_int=0;
	DM4340_Data.pos=0.0f;
	DM4340_Data.vel=0.0f;
	DM4340_Data.torq=0.0f;
	DM4340_Data.last_p_int=0;
	DM4340_Data.last_vel=0.0f;
	DM4340_Data.target_torq=0.0f;
}

/**
 * @brief  MITģʽ���¿���֡
 * @param  hcan   CAN�ľ��
 * @param  ID     ����֡��ID
 * @param  _pos   λ�ø���
 * @param  _vel   �ٶȸ���
 * @param  _KP    λ�ñ���ϵ��
 * @param  _KD    λ��΢��ϵ��
 * @param  _torq  ת�ظ���ֵ
 */
void MIT_Ctrl_DM_Motor(CAN_HandleTypeDef* hcan,uint16_t ID, float _pos, float _vel,float _KP, float _KD, float _torq)
{
	uint8_t Tx_Data[8];
	uint32_t Can_Tx_Mail_Box;
	CAN_TxHeaderTypeDef Tx_Msg;
	uint16_t pos_tmp,vel_tmp,kp_tmp,kd_tmp,tor_tmp;
	
	Tx_Msg.StdId=ID;
	Tx_Msg.IDE=CAN_ID_STD;
	Tx_Msg.RTR=CAN_RTR_DATA;
	Tx_Msg.DLC=0x08; 
	
	pos_tmp = float_to_uint(_pos, P_MIN, P_MAX, 16);
	vel_tmp = float_to_uint(_vel, V_MIN, V_MAX, 12);
	kp_tmp = float_to_uint(_KP, KP_MIN, KP_MAX, 12);
	kd_tmp = float_to_uint(_KD, KD_MIN, KD_MAX, 12);
	tor_tmp = float_to_uint(_torq, T_MIN, T_MAX, 12);
	
	Tx_Data[0] = (pos_tmp >> 8);
	Tx_Data[1] = pos_tmp;
	Tx_Data[2] = (vel_tmp >> 4);
	Tx_Data[3] = ((vel_tmp&0xF)<<4)|(kp_tmp>>8);
	Tx_Data[4] = kp_tmp;
	Tx_Data[5] = (kd_tmp >> 4);
	Tx_Data[6] = ((kd_tmp&0xF)<<4)|(tor_tmp>>8);
	Tx_Data[7] = tor_tmp;
	
	HAL_CAN_AddTxMessage(hcan, &Tx_Msg, Tx_Data, &Can_Tx_Mail_Box);	
}

/**
 * @brief  λ���ٶ�ģʽ���¿���֡
 * @param  hcan   CAN�ľ��
 * @param  ID     ����֡��ID
 * @param  _pos   λ�ø���
 * @param  _vel   �ٶȸ���
 */
void PosSpeed_CtrlMotor(CAN_HandleTypeDef* hcan, uint16_t ID, float _pos, float _vel)
{
	uint8_t Tx_Data[8];
	CAN_TxHeaderTypeDef Tx_Msg;
	uint32_t Can_Tx_Mail_Box;
	uint8_t *pbuf,*vbuf;
	pbuf=(uint8_t*)&_pos;
	vbuf=(uint8_t*)&_vel;

	Tx_Msg.StdId = ID;
	Tx_Msg.IDE = CAN_ID_STD;
	Tx_Msg.RTR = CAN_RTR_DATA;
	Tx_Msg.DLC = 0x08;

	Tx_Data[0] = *pbuf;;
	Tx_Data[1] = *(pbuf+1);
	Tx_Data[2] = *(pbuf+2);
	Tx_Data[3] = *(pbuf+3);
	Tx_Data[4] = *vbuf;
	Tx_Data[5] = *(vbuf+1);
	Tx_Data[6] = *(vbuf+2);
	Tx_Data[7] = *(vbuf+3);

	//�ҵ��յķ������䣬�����ݷ��ͳ�ȥ
	HAL_CAN_AddTxMessage(hcan, &Tx_Msg, Tx_Data, &Can_Tx_Mail_Box);
}

/**
 * @brief  �������
 * @param  hcan   CAN�ľ��
 * @param  ID     ����֡��ID      
 * @param  data   ����֡
 */
void Enable_DM_Motor(CAN_HandleTypeDef* hcan,uint8_t Motor_ID)
{
	uint8_t Tx_Data[8];
	uint32_t send_mail_box;
	CAN_TxHeaderTypeDef Tx_Msg;
	
	Tx_Msg.StdId=0x000+Motor_ID;//MITģʽ��Ϊ0x000+ID��

	Tx_Msg.IDE=CAN_ID_STD;		  
	Tx_Msg.RTR=CAN_RTR_DATA;		  
	Tx_Msg.DLC=0x08;
	
	Tx_Data[0] = 0xff;
	Tx_Data[1] = 0xff;
	Tx_Data[2] = 0xff;
	Tx_Data[3] = 0xff;
	Tx_Data[4] = 0xff;
	Tx_Data[5] = 0xff;
	Tx_Data[6] = 0xff;
	Tx_Data[7] = 0xfc;

  HAL_CAN_AddTxMessage(&hcan2, &Tx_Msg, Tx_Data, &send_mail_box);
}

/**
 * @brief  �������
 * @param  hcan   CAN�ľ��
 * @param  ID     ����֡��ID      
 * @param  data   ����֡
 */
void Disable_DM_Motor(CAN_HandleTypeDef* hcan,uint8_t Motor_ID)
{
	uint8_t Tx_Data[8];
	uint32_t send_mail_box;
	CAN_TxHeaderTypeDef Tx_Msg;
	
	Tx_Msg.StdId=0x000+Motor_ID;//MITģʽ��Ϊ0x000+ID��

	Tx_Msg.IDE=CAN_ID_STD;		  
	Tx_Msg.RTR=CAN_RTR_DATA;		  
	Tx_Msg.DLC=0x08;
	
	Tx_Data[0] = 0xff;
	Tx_Data[1] = 0xff;
	Tx_Data[2] = 0xff;
	Tx_Data[3] = 0xff;
	Tx_Data[4] = 0xff;
	Tx_Data[5] = 0xff;
	Tx_Data[6] = 0xff;
	Tx_Data[7] = 0xfd;

  HAL_CAN_AddTxMessage(&hcan2, &Tx_Msg, Tx_Data, &send_mail_box);
}

/**
************************************************************************
* @brief:      	save_pos_zero: ����λ����㺯��
* @param[in]:   hcan:     ָ��CAN_HandleTypeDef�ṹ��ָ��
* @param[in]:   Motor_ID: ���ID��ָ��Ŀ����
* @retval:     	void
* @details:    	ͨ��CAN�������ض�������ͱ���λ����������
************************************************************************
**/
void Save_Pos_Zero(CAN_HandleTypeDef* hcan, uint16_t Motor_ID)
{
	uint8_t Tx_Data[8];
	uint32_t send_mail_box;
	CAN_TxHeaderTypeDef Tx_Msg;
	
	Tx_Msg.StdId=0x000+Motor_ID;//MITģʽ��Ϊ0x000+ID��
	Tx_Msg.IDE=CAN_ID_STD;		  
	Tx_Msg.RTR=CAN_RTR_DATA;		  
	Tx_Msg.DLC=0x08;
	
	Tx_Data[0] = 0xFF;
	Tx_Data[1] = 0xFF;
	Tx_Data[2] = 0xFF;
	Tx_Data[3] = 0xFF;
	Tx_Data[4] = 0xFF;
	Tx_Data[5] = 0xFF;
	Tx_Data[6] = 0xFF;
	Tx_Data[7] = 0xFE;
	
	HAL_CAN_AddTxMessage(&hcan2, &Tx_Msg, Tx_Data, &send_mail_box);
}


void Get_DM4340_Data(DM_Motor_t *DM4340_Data, uint8_t* Rx_Data)
{
	DM4340_Data->state = (Rx_Data[0] & 0x10) >> 4;
	
	DM4340_Data->p_int=(Rx_Data[1]<<8 | Rx_Data[2]);
	DM4340_Data->v_int=(Rx_Data[3]<<4 | Rx_Data[4]>>4);
	DM4340_Data->t_int=(((Rx_Data[4]&0xF)<<8)|(Rx_Data[5]));
	
	DM4340_Data->p_int-=DM4340_ZERO_POS_OFFSET;//��ȥһ��ƫ�á�

	//���ݴ�����p_int������0��DM4340_MAX_P_INT֮�䡣
	if(DM4340_Data->p_int<0)
	{
		DM4340_Data->p_int+=DM4340_MAX_P_INT;
	}
	else
	{
		DM4340_Data->p_int%=DM4340_MAX_P_INT;
	}
	
	
	DM4340_Data->pos=uint_to_float(DM4340_Data->p_int,P_MIN,P_MAX,16);
	DM4340_Data->vel=uint_to_float(DM4340_Data->v_int,V_MIN,V_MAX,12);
	DM4340_Data->torq=uint_to_float(DM4340_Data->t_int,T_MIN,T_MAX,12);

	DM4340_Data->last_p_int = DM4340_Data->p_int;
	
	//��ͨ�˲�LPF(Lowpass filtering)��
	DM4340_Data->last_vel = DM4340_Data->vel;
	DM4340_Data->vel = DM4340_Data->vel * DM4340_LPF_K + DM4340_Data->last_vel * (1 - DM4340_LPF_K);

}

void Get_DM4310_Data(DM_Motor_t *DM4310_Data, uint8_t* Rx_Data)
{
	DM4310_Data->state = (Rx_Data[0] & 0x10) >> 4;
	
	DM4310_Data->p_int=((Rx_Data[1]<<8)|Rx_Data[2]);
	DM4310_Data->v_int=(Rx_Data[3]<<4)|(Rx_Data[4]>>4);
	DM4310_Data->t_int=((Rx_Data[4]&0xF)<<8)|Rx_Data[5];
	
	DM4310_Data->p_int-=DM4310_ZERO_POS_OFFSET;//��ȥһ��ƫ�á�
	
	//���ݴ�������4310��Ҫ��ת������Ҫ�����´���
	DM4310_Data->p_int=DM4310_MAX_P_INT-DM4310_Data->p_int;
	
	//���ݴ�����p_int������0��DM4310_MAX_P_INT֮�䡣
	if(DM4310_Data->p_int<0)
	{
		DM4310_Data->p_int+=DM4310_MAX_P_INT;
	}
	else
	{
		DM4310_Data->p_int%=DM4310_MAX_P_INT;
	}
	
	DM4310_Data->pos = uint_to_float(DM4310_Data->p_int, P_MIN, P_MAX, 16);
	DM4310_Data->vel = uint_to_float(DM4310_Data->v_int, V_MIN, V_MAX, 12);
	DM4310_Data->torq = uint_to_float(DM4310_Data->t_int, T_MIN, T_MAX, 12);
	
	DM4310_Data->last_p_int = DM4310_Data->p_int;	
	
	//��ͨ�˲�LPF(Lowpass filtering)��
	DM4310_Data->last_vel = DM4310_Data->vel;
	DM4310_Data->vel = DM4310_Data->vel * DM4310_LPF_K + DM4310_Data->last_vel * (1 - DM4310_LPF_K);
	
}

//�ٶȵ���PID���ƣ��������ٶȻ�ʱʹ�á�
void DM_PID_Speed_Ctrl(uint8_t ID, float DM_target_speed)
{
	if (ID == DM4340_ID)
	{
		DM4340_Data.target_torq=PID_calc(&DM4340_speed_pid,DM4340_Data.vel,DM_target_speed);
		MIT_Ctrl_DM_Motor(&hcan2,DM4340_ID,0,0,0,0,DM4340_Data.target_torq);	
	}
	else if(ID == DM4310_ID)
	{
		DM4310_Data.target_torq=PID_calc(&DM4310_speed_pid,DM4310_Data.vel,DM_target_speed);
		MIT_Ctrl_DM_Motor(&hcan2,DM4310_ID,0,0,0,0,DM4310_Data.target_torq);	
	}
}


//λ���ٶ�˫��PID���ơ�
void DM_PID_Pos_Speed_Ctrl(uint8_t ID, float DM_target_pos_int)
{
	if (ID == DM4340_ID)
	{
																									//����������
		DM4340_target_speed=PID_calc(&DM4340_pos_pid,Inferior_arc_treatment(DM4340_Data.p_int, DM_target_pos_int, DM4340_MAX_P_INT),DM_target_pos_int);
		DM4340_Data.target_torq=PID_calc(&DM4340_speed_pid,DM4340_Data.vel,DM4340_target_speed);
		
		MIT_Ctrl_DM_Motor(&hcan2,DM4340_ID,0,0,0,0,DM4340_Data.target_torq);
	}
	else if (ID == DM4310_ID)
	{
																									//����������		
		DM4310_target_speed=PID_calc(&DM4310_pos_pid,Inferior_arc_treatment(DM4310_Data.p_int, DM_target_pos_int, DM4310_MAX_P_INT),DM_target_pos_int);
		DM4310_target_speed = -DM4310_target_speed;//��ΪDM4310Ҫ��ת��
		DM4310_Data.target_torq=PID_calc(&DM4310_speed_pid,DM4310_Data.vel,DM4310_target_speed);
		
		MIT_Ctrl_DM_Motor(&hcan2,DM4310_ID,0,0,0,0,DM4310_Data.target_torq);		
	}
}

//��е��б�º�����ʹ�û�е����������֮���νӵ�������
//k�������ķ��ȣ����task�����osDelay���Լ����ÿ��data_follow����data_target�ƽ����١�
void ramp_function(float *data_follow,float data_target,float k)
{
	if(data_target!=*data_follow)
	{
		if(fabs(data_target-*data_follow)<k/10.0f)
		{
			*data_follow=data_target;
		}
		else
		{
			*data_follow += k * (data_target - *data_follow>0 ? 1.0f : -1.0f);
		}
	}
}


void Arm_Ctrl(float target_x, float target_y, float target_angle)
{
	//б�¿��ơ�
	//��k==0.01,osDelay(1)������£�ÿ1ms��target_x_follow�ƽ�target_x�Ĵ�СΪ0.01����1s����10��
	ramp_function(&target_x_follow, target_x, 0.02);
	ramp_function(&target_y_follow, target_y, 0.02);
	ramp_function(&target_angle_follow, target_angle, 0.02);
	
	//��е�۽��㡣
	arms_js(arms_js_data, target_x_follow, target_y_follow, target_angle_follow, 16.085f, 17.514f, 16.454f);
	
	//��������ֵ��
	if (arms_js_data[3] == 0)//����û�г����⡣
	{
		Servo_target_angle = arms_js_data[0];
		DM4310_target_pos_int = DM4310_angle_to_enc_p_int(arms_js_data[1]);
		DM4340_target_pos_int = DM4340_angle_to_enc_p_int(arms_js_data[2]);
	}
	else if (arms_js_data[3] == 1)//�����������,���û�е�ۻص��̶�λ�á�
	{
		Servo_target_angle = 35.660469f;
		DM4310_target_pos_int =  DM4310_angle_to_enc_p_int(242.605469);
		DM4340_target_pos_int =  DM4340_angle_to_enc_p_int(61.055054);
	}
	
	//�ؽڶ��������ơ�
//	Servo_Ctrl_arm(Servo_target_angle);
	
//	if (Get_KeyNum() == 0)
//	{
//		//ʹ������ߡ�
//		MIT_Ctrl_DM_Motor(&hcan2,DM4310_ID,0,0,0,0,0);
//		MIT_Ctrl_DM_Motor(&hcan2,DM4340_ID,0,0,0,0,0);
//	}
	DM_PID_Pos_Speed_Ctrl(DM4310_ID, DM4310_target_pos_int);
	DM_PID_Pos_Speed_Ctrl(DM4340_ID, DM4340_target_pos_int);		

}
