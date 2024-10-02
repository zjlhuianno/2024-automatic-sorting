#include "Includes.h"

//float m_x = 38.0f;
//float m_y = 7.0f;
//float m_z = 40.0f;
//float servo_arm_angle = 5.0f;

int pos_frame_cnt = 0;//λ��֡��ʱ����
int pos_stable_cnt = 0;//��̬�ȶ���ʱ����
int claw_freq_cnt = 0;//צƵ�ʼ�ʱ����

int arm_move_mode = 1;//��е�۶���ģʽ��

extern int arm_catch_flag;//��צ�����̬��־λ��

void Total_Task(void const * argument)
{
	
	while(1)
	{
//			target_x = 20.0f;
//			target_y = 23.5f;
//			target_angle = 40;
//			Servo_Ctrl_arm(5);
//			arm_catch_flag=1;
		
		
		
		//catch_object(0);
		if (arm_move_mode == 1)//��ƽ̨ץ�ŷ��顣
		{
			if (pos_frame_cnt == 0)//��е���쵽�߻��崦��צ�ſ���
			{
				set_arm_pos_param(38.0f, 30.0f, 30.0f, 5.0f);
				pos_stable_function(0, 1);//�������һ֡����צ�ſ���
			}
			else if (pos_frame_cnt == 1)//��е���쵽��ƽ̨��צȡ���顣
			{
				arm_catch_flag=2;//צȡ���顣
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
		
		
		//��ƽ̨ץ������Ȧ��
		/*
		if (pos_frame_cnt == 0)//��е���쵽���崦��צ�ſ���
		{
			target_x = 38.0f;
			target_y = 25.5f;
			target_angle = 40.0f;
			Servo_Ctrl_arm(10);
			arm_catch_flag=1;//צ�ſ���
			
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.2 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.2)
			{
				pos_stable_cnt++;
				if (pos_stable_cnt == 3000)
				{
					pos_frame_cnt = 1;
					pos_stable_cnt=0;
				}
			}
			
		}
		else if (pos_frame_cnt == 1)//��е���쵽��ƽ̨��צȡ����Ȧ��
		{
			target_x = 38.0f;
			target_y = 7.0f;
			target_angle = 40;
			Servo_Ctrl_arm(5.0f);
			osDelay(1000);
			arm_catch_flag=3;//צȡ����Ȧ��
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.2 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.2)
			{
				pos_stable_cnt++;
				if (pos_stable_cnt == 5)
				{
					pos_frame_cnt = 2;
					pos_stable_cnt=0;
				}
			}		
			
		}
		else if (pos_frame_cnt==2)//��е���쵽��ƽ̨�Ľϸߴ���צ�Ա���ץ�������̬��Ϊ���Ƿ�ֹצ�ŷ���ʱ����Ȧ��굽����ƽ̨��
		{
			target_x = 43.0f;
			target_y = 18.9f;
			target_angle = 40;
			Servo_Ctrl_arm(10.0f);
			osDelay(1000);
			arm_catch_flag=3;//צȡ����Ȧ��
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.2 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.2)
			{
				pos_stable_cnt++;
				if (pos_stable_cnt == 5)
				{
					pos_frame_cnt = 3;
					pos_stable_cnt=0;
				}
			}		
						
		}
		
		else if (pos_frame_cnt==3)//��е����������ֿ⣬צ������Ȧ��
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
		/*
		if (pos_frame_cnt == 0)//��е���쵽�߻��崦��צ�պϡ�
		{
			target_x = 38.0f;
			target_y = 30.0f;
			target_angle = 30;
			Servo_Ctrl_arm(5.0f);
			osDelay(1000);
			arm_catch_flag=0;//צ�պϡ�
			
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.2 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.2)
			{
				pos_stable_cnt++;
				if (pos_stable_cnt == 3)
				{
					pos_frame_cnt = 1;
					pos_stable_cnt=0;
				}
			}
			
		}
		else if (pos_frame_cnt == 1)//��е���쵽��ƽ̨��צȡ����Ȧ��
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
				pos_stable_cnt++;
				if (pos_stable_cnt == 5)
				{
					pos_frame_cnt = 2;
					pos_stable_cnt=0;
				}
			}		
			
		}
		else if (pos_frame_cnt==2)//��е���쵽��ƽ̨�Ľϸߴ���צ�Ա���ץ����Ȧ����̬��Ϊ���Ƿ�ֹצ������Ȧʱ����Ȧ��굽����ƽ̨��
		{
			target_x = 38.0f;
			target_y = 30.0f;
			target_angle = 30;
			Servo_Ctrl_arm(5.0f);
			osDelay(1000);
			arm_catch_flag=3;//צȡ����Ȧ��
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.2 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.2)
			{
				pos_stable_cnt++;
				if (pos_stable_cnt == 5)
				{
					pos_frame_cnt = 3;
					pos_stable_cnt=0;
				}
			}		
						
		}
		
		else if (pos_frame_cnt==3)//��е���쵽�ͻ�������צ�Ա���ץ����Ȧ����̬��Ϊ���Ƿ�ֹצ������Ȧʱ����Ȧ��굽����ƽ̨��
		{
			target_x = 10.0f;
			target_y = 30.5f;
			target_angle = 40;
			Servo_Ctrl_arm(40.0f);
			osDelay(1000);
			arm_catch_flag=3;//צȡ����Ȧ��
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.2 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.2)
			{
				pos_stable_cnt++;
				if (pos_stable_cnt == 5)
				{
					pos_frame_cnt = 4;
					pos_stable_cnt=0;
				}
			}		
						
		}		
		
		else if (pos_frame_cnt==4)//��е����������ֿ⣬צ�ŷ��顣
		{
			target_x = 27.0f;
			target_y = 10.5f;
			target_angle = 40;
			Servo_Ctrl_arm(5);
			osDelay(2000);
			arm_catch_flag=1;			
		}		
		*/
		
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

void set_arm_pos_param(float target_x_param, float target_y_param, float target_angle_param, float true_target_angle_param)
{
	target_x = target_x_param;
	target_y = target_y_param;
	target_angle = target_angle_param;
	Servo_Ctrl_arm(true_target_angle_param);
}


void pos_stable_function(uint8_t is_last_pos_frame, uint8_t claw_mode)
{
	osDelay(100);
	
	if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.1 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.1)
	{
		
		pos_stable_cnt++; //ÿ��һ��ʱ�䣬���������������Ԥ�ⷶΧ�ڣ�pos_stable_cnt++��
											//ע�⣺�����һ��ʱ�䲢�����ɱ������ڵ�osDelay(100)����������ģ����ܵ�task��ͨѶƵ�ʵ�Ӱ�졣
		
		if (pos_stable_cnt == 100)//��̬�ȶ�һ��ʱ���Ժ󣬲�����ִ����һ֡��
		{
			pos_stable_cnt=0;
			
			arm_catch_flag=claw_mode;//ֱ����е����̬�ȶ����Ÿı��צ�Ķ�����
			
			if (is_last_pos_frame == 0)//������Ǹö���ģ������һ֡����pos_frame_cnt++��ִ����һ��������
			{
				pos_frame_cnt++;
			}
			else if (is_last_pos_frame == 1)//����Ǹö���ģ������һ֡����pos_frame_cnt��arm_move_mode���á�
			{
				pos_frame_cnt = 0;
				arm_move_mode =0;
			}
		}
	}	
}

//�����ǲ��Գ���

		//���Դ�task��ѭ�������������С�
//		flag_z=1;
//		aRGB_led_show(0xFFFF0000);
//		osDelay(500);
//		aRGB_led_show(0x7F123456);
//		osDelay(500);

