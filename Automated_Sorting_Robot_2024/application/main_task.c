#include "main_task.h"
#include "DM_Control.h"
#include "Servo.h"
#include "math.h"
#include "stdlib.h"
#include "configs.h"
#include "ins_task.h"
#include "Arm_Task.h"
#include "tim.h"


int catch_object_mode = 0;//��е��ץȡ����ģʽ��
int push_ball_mode = 0;//��е�۲�����ģʽ��

void main_task(void const * argument)
{
	while(!ins_init_flag)
			osDelay(1);
	
	openmv_ok_flag=1;
	while(1)
	{
//		pile_approach_flag = 1;//����Ȧ����׮������
//			set_arm_pos_param(38.0f, 20.0f, 0.0f, 10.0f);//Բ�̻�ƽ̨��
//			pos_stable_function(0, 0);//�������һ֡����צ�պϡ�
//		catch_object(7);
		push_ball(1);
		osDelay(1);
	}
}








//�����ǲ��Գ���

		//������е�۶����
		//Servo_Ctrl_arm(30.0f);
		//__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,2500);

		/*
		//��׮ץ����
		
		if (pos_frame_cnt == 0)//��е���쵽�߻��崦��צ�պϡ�
		{
			target_x = 38.0f;
			target_y = 30.0f;
			target_angle = 30;
			Servo_Ctrl_arm(5.0f);
			osDelay(1000);
			arm_catch_flag=0;//צ�պϡ�
			
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.1 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.1)
			{
				pos_stable_cnt++;
				if (pos_stable_cnt == 10)
				{
					pos_frame_cnt = 1;
					pos_stable_cnt=0;
				}
			}
			
		}
		
		else if (pos_frame_cnt == 1)//��е���쵽��׮��ʶ��openmv��
		{
			target_x = 42.0f;
			target_y = 21.5f;
			target_angle = 10;
			Servo_Ctrl_arm(20.0f);
			arm_catch_flag=0;//צ�պϡ�
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.1 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.1)
			{
				pos_stable_cnt++;
				if (pos_stable_cnt == 10)
				{
					pos_frame_cnt = 2;
					pos_stable_cnt=0;
				}
			}		
			
		}		
		
		else if (pos_frame_cnt == 2)//��е���쵽��ƽ̨��צȡ��
		{
			arm_catch_flag=1;//צ�ſ���
			
			target_x = 40.0f;
			target_y = 21.5f;
			target_angle = 20;
			Servo_Ctrl_arm(10.0f);
			osDelay(1500);
			arm_catch_flag=3;//צȡ��
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.1 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.1)
			{
				pos_stable_cnt++;
				if (pos_stable_cnt == 10)
				{
					pos_frame_cnt = 3;
					pos_stable_cnt=0;
				}
			}		
			
		}
		
		else if (pos_frame_cnt==3)//��е���쵽�ͻ�������צ����
		{
			target_x = 10.0f;
			target_y = 30.5f;
			target_angle = 40;
			Servo_Ctrl_arm(40.0f);
			osDelay(2000);
			arm_catch_flag=1;//צ����
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.2 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.2)
			{
				pos_stable_cnt++;
				if (pos_stable_cnt == 2)
				{
					pos_frame_cnt = 4;
					pos_stable_cnt=0;
				}
			}		
						
		}		
		*/
		
		
		
		//���λ�ñ궨��
//		Servo_Ctrl_claw(2);
//		Servo_Ctrl_3(0);
//		Servo_Ctrl_6(0);


		/*
		if (arm_move_mode == 1)//��ƽ̨ץ�ŷ��顣
		{
			if (pos_frame_cnt == 0)//��е���쵽�߻��崦��צ�ſ���
			{
				set_arm_pos_param(38.0f, 30.0f, 30.0f, 5.0f);
				pos_stable_function(0, 1);//�������һ֡����צ�ſ���
				
			}
			else if (pos_frame_cnt == 1)//��е���쵽��ƽ̨��צȡ���顣
			{
				//arm_catch_flag=2;//צȡ���顣
				set_arm_pos_param(38.0f, 6.0f, 40.0f, 5.0f);
				pos_stable_function(0, 2);//�������һ֡����צȡ���顣	
			}
			else if (pos_frame_cnt==2)//��е���쵽��ƽ̨�Ľϸߴ���צ�Ա���ץ�������̬��Ϊ���Ƿ�ֹצ�ŷ���ʱ�����굽����ƽ̨��
			{
				set_arm_pos_param(43.0f, 18.9f, 40.0f, 10.0f);
				pos_stable_function(0, 2);//�������һ֡����צȡ���顣
							
			}
			else if (pos_frame_cnt==3)//��е����������ֿ���Ϸ���צ�ŷ��顣
			{
				set_arm_pos_param(20.0f, 25.5f, 40.0f, 5.0f);
				pos_stable_function(1, 1);//�����һ֡����צ�ſ���
			}			
		}

		else if (arm_move_mode == 2)//��׮ץ����
		{
			if (pos_frame_cnt == 0)//��е���쵽�߻��崦��צ�պϡ�
			{
				target_x = 38.0f;
				target_y = 30.0f;
				target_angle = 30;
				Servo_Ctrl_arm(5.0f);
				osDelay(1000);
				
				
				if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.1 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.1)
				{
					pos_stable_cnt++;
					if (pos_stable_cnt == 10)
					{
						arm_catch_flag=0;//צ�պϡ�
						pos_frame_cnt = 1;
						pos_stable_cnt=0;
					}
				}
				
			}
			
			else if (pos_frame_cnt == 1)//��е���쵽��׮��ʶ��openmv��
			{
				target_x = 42.0f;
				target_y = 21.5f;
				target_angle = 10;
				Servo_Ctrl_arm(20.0f);
				
				if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.1 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.1)
				{
					pos_stable_cnt++;
					if (pos_stable_cnt == 10)
					{
						arm_catch_flag=0;//צ�պϡ�
						pos_frame_cnt = 2;
						pos_stable_cnt=0;
					}
				}		
				
			}		
			
			else if (pos_frame_cnt == 2)//��е���쵽��ƽ̨��צȡ��
			{
				arm_catch_flag=1;//צ�ſ���
				
				target_x = 40.0f;
				target_y = 21.5f;
				target_angle = 20;
				Servo_Ctrl_arm(10.0f);
				osDelay(1500);
				
				if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.1 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.1)
				{
					pos_stable_cnt++;
					if (pos_stable_cnt == 10)
					{
						arm_catch_flag=3;//צȡ��
						pos_frame_cnt = 3;
						pos_stable_cnt=0;
					}
				}		
				
			}
			
			else if (pos_frame_cnt==3)//��е���쵽�ͻ�������צ����
			{
				target_x = 10.0f;
				target_y = 30.5f;
				target_angle = 40;
				Servo_Ctrl_arm(40.0f);
				osDelay(2000);
				
				if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.2 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.2)
				{
					pos_stable_cnt++;
					if (pos_stable_cnt == 2)
					{
						arm_catch_flag=1;//צ����
						pos_frame_cnt = 0;
						pos_stable_cnt=0;
						arm_move_mode =0;	
					}
				}		
							
			}				
		}

		else if (arm_move_mode == 3)
		{
			//Բ�̻�����
			
			if (pos_frame_cnt == 0)//��е���쵽Բ�̻��ض��ĸ߻��崦��צ�պϡ�
			{
				target_x = 38.0f;
				target_y = 28.5f;
				target_angle = 20;
				Servo_Ctrl_arm(10.0f);
				osDelay(1000);
				arm_catch_flag=0;//צ�պϡ�
				
				if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.1 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.1)
				{
					pos_stable_cnt++;
					if (pos_stable_cnt == 10)
					{
						pos_frame_cnt = 1;
						pos_stable_cnt=0;
					}
				}
				
			}
			else if (pos_frame_cnt == 1)//��е���쵽Բ�̻���צ����
			{
				claw_freq_cnt++;
				target_x = 41.0f;
				target_y = 21.5f;
				target_angle = 20;
				Servo_Ctrl_arm(10.0f);
				osDelay(500);
				
				if (claw_freq_cnt %= 1500)
				{
					arm_catch_flag=4;//צ����
					osDelay(500);
					arm_catch_flag=0;//צ�պϡ�
					
					claw_freq_cnt=0;
				}			
				if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.1 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.1)
				{
					pos_stable_cnt++;
					if (pos_stable_cnt == 20)
					{
						pos_frame_cnt = 2;
						pos_stable_cnt=0;
					}
				}		
				
			}
			
			else if (pos_frame_cnt==2)//��е�������ͻ���������������
			{
				target_x = 10.0f;
				target_y = 30.5f;
				target_angle = 40;
				Servo_Ctrl_arm(40.0f);
				osDelay(1000);
				arm_catch_flag=0;//צ�պϡ�
				if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.2 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.2)
				{
					pos_stable_cnt++;
					if (pos_stable_cnt == 5)
					{
						pos_frame_cnt = 3;
						pos_stable_cnt=0;
						arm_move_mode =0;
					}
				}					
							
			}		
		}
		*/


		//���Դ�task��ѭ�������������С�
//		flag_z=1;
//		aRGB_led_show(0xFFFF0000);
//		osDelay(500);
//		aRGB_led_show(0x7F123456);
//		osDelay(500);




