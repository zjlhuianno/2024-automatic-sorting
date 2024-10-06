#include "DM_Control.h"
#include "Arm_Posture_Calculate.h"
#include "math.h"
#include "stdlib.h"
#include "configs.h"

/**
 * @brief  uint转float
 * @param  x_int   最大值
 * @param  x_min   最小值
 * @param  bits    位数
 */
float uint_to_float(int x_int, float x_min, float x_max, int bits)
{
  float span = x_max - x_min;
  float offset = x_min;
  return ((float)x_int)*span/((float)((1<<bits)-1)) + offset;
}

/**
 * @brief  float转uint
 * @param  x_int   最大值
 * @param  x_min   最小值
 * @param  bits    位数
 */
int float_to_uint(float x, float x_min, float x_max, int bits)
{ 
  float span = x_max - x_min;
  float offset = x_min;
  return (int) ((x-offset)*((float)((1<<bits)-1))/span);
}


//角度与编码位置的换算。
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
	//return 360 - angle;//因为4310需要反转。
	return angle;
}

//弧度制转成角度制。
float radian_to_degree(float radian)
{
	return radian * 57.3f;
}

//零点溢出处理。
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

//DM4340，DM4310的PID初始化。
void DM_PID_Init(void)
{
	//DM4340速度环。
	DM4340_speed_PID[0]=DM4340_speed_kp;
	DM4340_speed_PID[1]=DM4340_speed_ki;
	DM4340_speed_PID[2]=DM4340_speed_kd;
	PID_init(&DM4340_speed_pid,PID_POSITION_ARM,DM4340_speed_PID,DM4340_speed_max_out,DM4340_speed_max_iout);

	//DM4340位置环。
	DM4340_pos_PID[0]=DM4340_pos_kp;
	DM4340_pos_PID[1]=DM4340_pos_ki;
	DM4340_pos_PID[2]=DM4340_pos_kd;
	PID_init(&DM4340_pos_pid,PID_POSITION_ARM,DM4340_pos_PID,DM4340_pos_max_out,DM4340_pos_max_iout);

	//DM4310速度环。
	DM4310_speed_PID[0]=DM4310_speed_kp;
	DM4310_speed_PID[1]=DM4310_speed_ki;
	DM4310_speed_PID[2]=DM4310_speed_kd;
	PID_init(&DM4310_speed_pid,PID_POSITION_ARM,DM4310_speed_PID,DM4310_speed_max_out,DM4310_speed_max_iout);
	
	//DM4310位置环。
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
 * @brief  MIT模式控下控制帧
 * @param  hcan   CAN的句柄
 * @param  ID     数据帧的ID
 * @param  _pos   位置给定
 * @param  _vel   速度给定
 * @param  _KP    位置比例系数
 * @param  _KD    位置微分系数
 * @param  _torq  转矩给定值
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
 * @brief  位置速度模式控下控制帧
 * @param  hcan   CAN的句柄
 * @param  ID     数据帧的ID
 * @param  _pos   位置给定
 * @param  _vel   速度给定
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

	//找到空的发送邮箱，把数据发送出去
	HAL_CAN_AddTxMessage(hcan, &Tx_Msg, Tx_Data, &Can_Tx_Mail_Box);
}

/**
 * @brief  电机命令
 * @param  hcan   CAN的句柄
 * @param  ID     数据帧的ID      
 * @param  data   命令帧
 */
void Enable_DM_Motor(CAN_HandleTypeDef* hcan,uint8_t Motor_ID)
{
	uint8_t Tx_Data[8];
	uint32_t send_mail_box;
	CAN_TxHeaderTypeDef Tx_Msg;
	
	Tx_Msg.StdId=0x000+Motor_ID;//MIT模式下为0x000+ID。

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
 * @brief  电机命令
 * @param  hcan   CAN的句柄
 * @param  ID     数据帧的ID      
 * @param  data   命令帧
 */
void Disable_DM_Motor(CAN_HandleTypeDef* hcan,uint8_t Motor_ID)
{
	uint8_t Tx_Data[8];
	uint32_t send_mail_box;
	CAN_TxHeaderTypeDef Tx_Msg;
	
	Tx_Msg.StdId=0x000+Motor_ID;//MIT模式下为0x000+ID。

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
* @brief:      	save_pos_zero: 保存位置零点函数
* @param[in]:   hcan:     指向CAN_HandleTypeDef结构的指针
* @param[in]:   Motor_ID: 电机ID，指定目标电机
* @retval:     	void
* @details:    	通过CAN总线向特定电机发送保存位置零点的命令
************************************************************************
**/
void Save_Pos_Zero(CAN_HandleTypeDef* hcan, uint16_t Motor_ID)
{
	uint8_t Tx_Data[8];
	uint32_t send_mail_box;
	CAN_TxHeaderTypeDef Tx_Msg;
	
	Tx_Msg.StdId=0x000+Motor_ID;//MIT模式下为0x000+ID。
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
	
	DM4340_Data->p_int-=DM4340_ZERO_POS_OFFSET;//减去一个偏置。

	//数据处理，把p_int限制在0到DM4340_MAX_P_INT之间。
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
	
	//低通滤波LPF(Lowpass filtering)。
	DM4340_Data->last_vel = DM4340_Data->vel;
	DM4340_Data->vel = DM4340_Data->vel * DM4340_LPF_K + DM4340_Data->last_vel * (1 - DM4340_LPF_K);

}

void Get_DM4310_Data(DM_Motor_t *DM4310_Data, uint8_t* Rx_Data)
{
	DM4310_Data->state = (Rx_Data[0] & 0x10) >> 4;
	
	DM4310_Data->p_int=((Rx_Data[1]<<8)|Rx_Data[2]);
	DM4310_Data->v_int=(Rx_Data[3]<<4)|(Rx_Data[4]>>4);
	DM4310_Data->t_int=((Rx_Data[4]&0xF)<<8)|Rx_Data[5];
	
	DM4310_Data->p_int-=DM4310_ZERO_POS_OFFSET;//减去一个偏置。
	
	//数据处理，由于4310需要反转，所以要做以下处理。
	DM4310_Data->p_int=DM4310_MAX_P_INT-DM4310_Data->p_int;
	
	//数据处理，把p_int限制在0到DM4310_MAX_P_INT之间。
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
	
	//低通滤波LPF(Lowpass filtering)。
	DM4310_Data->last_vel = DM4310_Data->vel;
	DM4310_Data->vel = DM4310_Data->vel * DM4310_LPF_K + DM4310_Data->last_vel * (1 - DM4310_LPF_K);
	
}

//速度单环PID控制，仅供调速度环时使用。
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


//位置速度双环PID控制。
void DM_PID_Pos_Speed_Ctrl(uint8_t ID, float DM_target_pos_int)
{
	if (ID == DM4340_ID)
	{
																									//零点溢出处理。
		DM4340_target_speed=PID_calc(&DM4340_pos_pid,Inferior_arc_treatment(DM4340_Data.p_int, DM_target_pos_int, DM4340_MAX_P_INT),DM_target_pos_int);
		DM4340_Data.target_torq=PID_calc(&DM4340_speed_pid,DM4340_Data.vel,DM4340_target_speed);
		
		MIT_Ctrl_DM_Motor(&hcan2,DM4340_ID,0,0,0,0,DM4340_Data.target_torq);
	}
	else if (ID == DM4310_ID)
	{
																									//零点溢出处理。		
		DM4310_target_speed=PID_calc(&DM4310_pos_pid,Inferior_arc_treatment(DM4310_Data.p_int, DM_target_pos_int, DM4310_MAX_P_INT),DM_target_pos_int);
		DM4310_target_speed = -DM4310_target_speed;//因为DM4310要反转。
		DM4310_Data.target_torq=PID_calc(&DM4310_speed_pid,DM4310_Data.vel,DM4310_target_speed);
		
		MIT_Ctrl_DM_Motor(&hcan2,DM4310_ID,0,0,0,0,DM4310_Data.target_torq);		
	}
}

//机械臂斜坡函数，使得机械臂两个动作之间衔接得流畅。
//k：步进的幅度，结合task里面的osDelay可以计算出每秒data_follow会向data_target逼近多少。
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
	//斜坡控制。
	//在k==0.01,osDelay(1)的情况下，每1ms，target_x_follow逼近target_x的大小为0.01，即1s步进10。
	ramp_function(&target_x_follow, target_x, 0.02);
	ramp_function(&target_y_follow, target_y, 0.02);
	ramp_function(&target_angle_follow, target_angle, 0.02);
	
	//机械臂解算。
	arms_js(arms_js_data, target_x_follow, target_y_follow, target_angle_follow, 16.085f, 17.514f, 16.454f);
	
	//结算结果赋值。
	if (arms_js_data[3] == 0)//解算没有出问题。
	{
		Servo_target_angle = arms_js_data[0];
		DM4310_target_pos_int = DM4310_angle_to_enc_p_int(arms_js_data[1]);
		DM4340_target_pos_int = DM4340_angle_to_enc_p_int(arms_js_data[2]);
	}
	else if (arms_js_data[3] == 1)//解算出现问题,则让机械臂回到固定位置。
	{
		Servo_target_angle = 35.660469f;
		DM4310_target_pos_int =  DM4310_angle_to_enc_p_int(242.605469);
		DM4340_target_pos_int =  DM4340_angle_to_enc_p_int(61.055054);
	}
	
	//关节舵机电机控制。
//	Servo_Ctrl_arm(Servo_target_angle);
	
//	if (Get_KeyNum() == 0)
//	{
//		//使电机力竭。
//		MIT_Ctrl_DM_Motor(&hcan2,DM4310_ID,0,0,0,0,0);
//		MIT_Ctrl_DM_Motor(&hcan2,DM4340_ID,0,0,0,0,0);
//	}
	DM_PID_Pos_Speed_Ctrl(DM4310_ID, DM4310_target_pos_int);
	DM_PID_Pos_Speed_Ctrl(DM4340_ID, DM4340_target_pos_int);		

}
