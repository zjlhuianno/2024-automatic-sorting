#include "Includes.h"

//float m_x = 38.0f;
//float m_y = 7.0f;
//float m_z = 40.0f;
//float servo_arm_angle = 5.0f;

int i = 0;
int ii = 0;

extern int arm_catch_flag;
void Total_Task(void const * argument)
{
	
	osDelay(1500); 
	while(1)
	{
//			target_x = 10.0f;
//			target_y = 30.5f;
//			target_angle = 40;
//			Servo_Ctrl_arm(40.0f);
//			arm_catch_flag=1;
		
		
		//catch_object(0);
		/*
		//��ƽ̨ץ�ŷ��顣
		if (i == 0)//��е���쵽���崦��צ�ſ���
		{
			target_x = 38.0f;
			target_y = 25.5f;
			target_angle = 40.0f;
			Servo_Ctrl_arm(10);
			arm_catch_flag=1;
			
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.2 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.2)
			{
				ii++;
				if (ii == 3000)
				{
					i = 1;
					ii=0;
				}
			}
			
		}
		else if (i == 1)//��е���쵽��ƽ̨��צȡ���顣
		{
			target_x = 38.0f;
			target_y = 7.0f;
			target_angle = 40;
			Servo_Ctrl_arm(5.0f);
			osDelay(1000);
			arm_catch_flag=2;
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.2 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.2)
			{
				ii++;
				if (ii == 5)
				{
					i = 2;
					ii=0;
				}
			}		
			
		}
		else if (i==2)//��е���쵽��ƽ̨�Ľϸߴ���צ�Ա���ץ�������̬��Ϊ���Ƿ�ֹצ�ŷ���ʱ�����굽����ƽ̨��
		{
			target_x = 43.0f;
			target_y = 18.9f;
			target_angle = 40;
			Servo_Ctrl_arm(10.0f);
			osDelay(1000);
			arm_catch_flag=2;
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.2 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.2)
			{
				ii++;
				if (ii == 5)
				{
					i = 3;
					ii=0;
				}
			}		
						
		}
		
		else if (i==3)//��е����������ֿ⣬צ�ŷ��顣
		{
			target_x = 27.0f;
			target_y = 10.5f;
			target_angle = 40;
			Servo_Ctrl_arm(5);
			osDelay(2000);
			arm_catch_flag=1;			
		}
		*/
		
		//��ƽ̨ץ������Ȧ��
		/*
		if (i == 0)//��е���쵽���崦��צ�ſ���
		{
			target_x = 38.0f;
			target_y = 25.5f;
			target_angle = 40.0f;
			Servo_Ctrl_arm(10);
			arm_catch_flag=1;//צ�ſ���
			
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.2 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.2)
			{
				ii++;
				if (ii == 3000)
				{
					i = 1;
					ii=0;
				}
			}
			
		}
		else if (i == 1)//��е���쵽��ƽ̨��צȡ����Ȧ��
		{
			target_x = 38.0f;
			target_y = 7.0f;
			target_angle = 40;
			Servo_Ctrl_arm(5.0f);
			osDelay(1000);
			arm_catch_flag=3;//צȡ����Ȧ��
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.2 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.2)
			{
				ii++;
				if (ii == 5)
				{
					i = 2;
					ii=0;
				}
			}		
			
		}
		else if (i==2)//��е���쵽��ƽ̨�Ľϸߴ���צ�Ա���ץ�������̬��Ϊ���Ƿ�ֹצ�ŷ���ʱ����Ȧ��굽����ƽ̨��
		{
			target_x = 43.0f;
			target_y = 18.9f;
			target_angle = 40;
			Servo_Ctrl_arm(10.0f);
			osDelay(1000);
			arm_catch_flag=3;//צȡ����Ȧ��
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.2 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.2)
			{
				ii++;
				if (ii == 5)
				{
					i = 3;
					ii=0;
				}
			}		
						
		}
		
		else if (i==3)//��е����������ֿ⣬צ������Ȧ��
		{
			target_x = 27.0f;
			target_y = 10.5f;
			target_angle = 40;
			Servo_Ctrl_arm(5);
			osDelay(2000);
			arm_catch_flag=1;	//צ�ſ���		
		}
		*/
		
		
		//��ƽ̨ץ������Ȧ��
		if (i == 0)//��е���쵽�߻��崦��צ�պϡ�
		{
			target_x = 38.0f;
			target_y = 30.0f;
			target_angle = 30;
			Servo_Ctrl_arm(5.0f);
			osDelay(1000);
			arm_catch_flag=0;//צ�պϡ�
			
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.2 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.2)
			{
				ii++;
				if (ii == 3)
				{
					i = 1;
					ii=0;
				}
			}
			
		}
		else if (i == 1)//��е���쵽��ƽ̨��צȡ����Ȧ��
		{
			arm_catch_flag=1;//צ�ſ���
			
			target_x = 38.0f;
			target_y = 25.5f;
			target_angle = 20;
			Servo_Ctrl_arm(10.0f);
			osDelay(1000);
			arm_catch_flag=3;//צȡ����Ȧ��
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.2 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.2)
			{
				ii++;
				if (ii == 5)
				{
					i = 2;
					ii=0;
				}
			}		
			
		}
		else if (i==2)//��е���쵽��ƽ̨�Ľϸߴ���צ�Ա���ץ����Ȧ����̬��Ϊ���Ƿ�ֹצ������Ȧʱ����Ȧ��굽����ƽ̨��
		{
			target_x = 38.0f;
			target_y = 30.0f;
			target_angle = 30;
			Servo_Ctrl_arm(5.0f);
			osDelay(1000);
			arm_catch_flag=3;//צȡ����Ȧ��
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.2 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.2)
			{
				ii++;
				if (ii == 5)
				{
					i = 3;
					ii=0;
				}
			}		
						
		}
		
		else if (i==3)//��е���쵽�ͻ�������צ�Ա���ץ����Ȧ����̬��Ϊ���Ƿ�ֹצ������Ȧʱ����Ȧ��굽����ƽ̨��
		{
			target_x = 10.0f;
			target_y = 30.5f;
			target_angle = 40;
			Servo_Ctrl_arm(40.0f);
			osDelay(1000);
			arm_catch_flag=3;//צȡ����Ȧ��
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.2 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.2)
			{
				ii++;
				if (ii == 5)
				{
					i = 4;
					ii=0;
				}
			}		
						
		}		
		
		else if (i==4)//��е����������ֿ⣬צ�ŷ��顣
		{
			target_x = 27.0f;
			target_y = 10.5f;
			target_angle = 40;
			Servo_Ctrl_arm(5);
			osDelay(2000);
			arm_catch_flag=1;			
		}		
		
		

		
		//���λ�ñ궨��
//		Servo_Ctrl_claw(2);
//		Servo_Ctrl_3(0);
//		Servo_Ctrl_6(0);
		
		osDelay(1);
	}
}

void catch_object(uint8_t flag)
{
//	int force_sensing_cnt = 0;

	if (flag == 0)
	{
//		force_sensing_flag = 1;//�������ظ�֪��
		
//		target_x = m_x;//15.0f;
//		target_y = m_y;//20.5f;
//		target_angle = m_z;//100.0f;
		
		
		
		
		
		
		
		
		
		
//		//���ظ�֪��
//		while(force_sensing_flag)
//		{
//			target_y-=0.03f;
//			if(target_y<130)
//			{
//				break;
//			}
//			if(abs(DM4310_delta_enc-last_DM4310_delta_enc)<5)
//			{
//				if(DM4310_delta_enc<25)
//				{
//					force_sensing_cnt++;
//					if(force_sensing_cnt==15)
//					{
//						target_y+=10;
//						force_sensing_flag=0;
//					}
//				}
//				else
//				{
//					force_sensing_cnt=0;
//				}
//			}
//			osDelay(1);			
//		}//���ظ�֪������


		
		
		
		
		
	//flag == 0������
	}
	
	
}


//�����ǲ��Գ���

		//���Դ�task��ѭ�������������С�
//		flag_z=1;
//		aRGB_led_show(0xFFFF0000);
//		osDelay(500);
//		aRGB_led_show(0x7F123456);
//		osDelay(500);

