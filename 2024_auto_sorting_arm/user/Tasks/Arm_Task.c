#include "Includes.h"

int arm_catch_flag = 0;

void Arm_Task(void const * argument)
{
	osDelay(3000); 
	while(1)
	{	
		
		Arm_Ctrl(target_x,target_y, target_angle);
		
		if (arm_catch_flag == 0)//��еצ�պ�
		{
			Servo_Ctrl_claw(0);
		}		
		if (arm_catch_flag == 1)//��еצ�ſ�
		{
			Servo_Ctrl_claw(1);
		}
		if (arm_catch_flag == 2)//��еצץȡ����
		{
			Servo_Ctrl_claw(2);
		}
		if (arm_catch_flag == 3)//��еצץȡ����Ȧ
		{
			Servo_Ctrl_claw(3);
		}
		if (arm_catch_flag == 4)//��еצ����
		{
			Servo_Ctrl_claw(4);
		}		
		
//		DM4310_delta_enc = DM4310_Data.p_int - DM4310_Data.last_p_int;
//		last_DM4310_delta_enc = DM4310_delta_enc;
		
		osDelay(1);
	}
}







//�����ǲ��Գ���

//float target_speed = 18.0f;
//float target_angle_4340 = 103.946274;//145.0��ʱ��4340�ؽ���ֱ��//55.0��ʱ��4340�ؽ�ˮƽ��
//float target_angle_4310 = 229.227997;//33.0��ʱ��4310�ؽ�ˮƽ��
//float now_angle_4310 = 0.0f;
//float now_angle_4340 = 0.0f;
//float pwm = 700;//700����ؽ�ֱ����
//int i = 0;

		//����DM4310�ٶȻ���
//		VOFA(2, target_speed, DM4310_Data.vel);
//		DM_PID_Speed_Ctrl(DM4310_ID, target_speed);

		
		//����DM4310λ�û���
//		i++;
//		if (i == 3000)
//		{
//			i = 0;
//			target_angle += 10.0f;
//		}
//		if (target_angle >= 180.0f)
//		{
//			target_angle = 30.0f;
//		}
//		
//		now_angle_4310 = DM4310_enc_p_int_to_angle(DM4310_Data.p_int);
//		VOFA(2, target_angle_4310, now_angle_4310);
//		DM_PID_Pos_Speed_Ctrl(DM4310_ID, DM4310_angle_to_enc_p_int(target_angle_4310));
//		osDelay(1000);
		
		//����DM4340�ٶȻ���
//		VOFA(2, target_speed, DM4340_Data.vel);
//		DM_PID_Speed_Ctrl(DM4340_ID, target_speed);
						
		//����DM4340λ�û���
//		i++;
//		if (i == 3000)
//		{
//			i = 0;

//			target_angle += 10.0f;
//		}
//		if (target_angle >= 180.0f)
//		{
//			target_angle = 30.0f;
//		}
//		

//		now_angle_4340 = DM4340_enc_p_int_to_angle(DM4340_Data.p_int);
//		VOFA(2, target_angle_4340, now_angle_4340);
//		DM_PID_Pos_Speed_Ctrl(DM4340_ID, DM4340_angle_to_enc_p_int(target_angle_4340));


		//ʹ������ߡ�
//		MIT_Ctrl_DM_Motor(&hcan2,DM4310_ID,0,0,0,0,0);
//		osDelay(10);
//		MIT_Ctrl_DM_Motor(&hcan2,DM4340_ID,0,0,0,0,0);

//		DM_PID_Pos_Speed_Ctrl(DM4310_ID, DM4310_angle_to_enc_p_int(60.0f));
//		DM_PID_Pos_Speed_Ctrl(DM4-340_ID, DM4340_angle_to_enc_p_int(60.0f));

//		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, Servo_1_angle_to_pwm(0.0f));

