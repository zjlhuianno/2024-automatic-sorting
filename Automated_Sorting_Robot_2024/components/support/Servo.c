#include "Servo.h"
#include "configs.h"
#include "tim.h"


//ID == 1����е�۵������Ķ����							λ�ã�tim1, TIM_CHANNEL_1��
//ID == 2����е�ۼ�צ�Ķ����  							λ�ã�tim1, TIM_CHANNEL_2��
//ID == 3���ֿ��·����ڵĶ����  						λ�ã�tim1, TIM_CHANNEL_3��
//ID == 4���ֿ��Ϸ����ڵĶ����  						λ�ã�tim1, TIM_CHANNEL_4��
//ID == 5���ֿ��Ϸ������Ķ���� 						λ�ã�tim8, TIM_CHANNEL_1��
//ID == 6���ֿ�����������Ȧ�Ķ����  			λ�ã�tim8, TIM_CHANNEL_2��

void Servo_Init(void)
{
	HAL_TIM_Base_Start(&htim1);//����TIM1��
	HAL_TIM_Base_Start(&htim8);//����TIM8��
	
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //����PWMͨ����IDΪ1�����ǿ��ƻ�е�۵������Ķ����
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);	//����PWMͨ����IDΪ2�����ǿ��ƻ�е�ۼ�צ�Ķ����
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3); //����PWMͨ����IDΪ3�����ǿ��Ʋֿ��·����ڵĶ����
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);	//����PWMͨ����IDΪ4�����ǿ��Ʋֿ��Ϸ����ڵĶ���� 
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1); //����PWMͨ����IDΪ5�����ǿ��Ʋֿ��Ϸ������Ķ���� 	
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);	//����PWMͨ����IDΪ6�����ǿ��Ʋֿ�����������Ȧ�Ķ����	
	
}

//��Ŀ��Ƕ�ת��Ϊ��е�۵������Ķ����pwmֵ��
float Servo_arm_angle_to_pwm(int16_t angle)
{
	//angleΪ�� = ��B'CD��
	float pwm = 0.0f;
	pwm = 700 + 2000 * (angle / 180.0f);
	if (pwm < 500)
	{
		pwm = 500;
	}
	if (pwm > 1800)//1800 100�����ҡ�
	{
		pwm = 1800;
	}	
	return pwm;
}

//���ƻ�е�۵������Ķ����
void Servo_Ctrl_arm(int16_t angle_for_servo_arm)
{
	//pwm=700ʱ����������е��ˮƽ��
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,Servo_arm_angle_to_pwm(angle_for_servo_arm));
}

//���ƻ�е�ۼ�צ�Ķ����
//ctrl_flag��	0����еצ�պϣ�pwm = 2400��
//						1����еצ�ſ���pwm = 2000��
//						2����еצץȡ���飬pwm = 2350��
//						3����еצץȡ����Ȧ��pwm = 2200��
//						4����еצ����pwm = 1900��
void Servo_Ctrl_claw(uint8_t ctrl_flag)
{
	switch (ctrl_flag)
	{
		case 0:
		{
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,2400);
			break;			
		}
		case 1:
		{
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,2000);
			break;			
		}
		case 2:
		{
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,2350);
			break;			
		}		
		case 3:
		{
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,2200);
			break;			
		}	
		case 4:
		{
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,1900);
			break;			
		}		
		default:
			break;
	}
}

//���Ʋֿ��·����ڵĶ����
//ctrl_flag��	0�����ڱպϣ�pwm = 1650��
//						1�����ڴ򿪣�pwm = 850��
//						2�����ڰ�򿪣�pwm = 1500��
void Servo_Ctrl_3(uint8_t ctrl_flag)
{
	switch (ctrl_flag)
	{
		case 0:
		{
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 1650);
			break;
		}
		case 1:
		{
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 850);
			break;
		}
		case 2:
		{
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 1500);
			break;
		}		
		default:
			break;		
	}
}

//���Ʋֿ��Ϸ����ڵĶ����
//ctrl_flag��	0������pwm = 650��
//						1���ӷ��飬pwm = 1200��
void Servo_Ctrl_4(uint8_t ctrl_flag)
{
	switch (ctrl_flag)
	{
		case 0:
		{
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 650);
			break;
		}
		case 1:
		{
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 1200);
			break;
		}		
			
		default:
			break;		
	}
}

//���Ʋֿ��Ϸ������Ķ����
//ctrl_flag��	0������������pwm = 1800��
//						1������������pwm = 1100��
void Servo_Ctrl_5(uint8_t ctrl_flag)
{
	switch (ctrl_flag)
	{
		case 0:
		{
			__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 1800);
			break;
		}
		case 1:
		{
			__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 2200);
			break;
		}
		default:
			break;		
	}
}

//���Ʋֿ�����������Ȧ�Ķ����
//ctrl_flag��	0�����ɲֿ����ֿ⣬pwm = 500��
//						1�����ɲ�Զ��ֿ⣬pwm = 1500��
void Servo_Ctrl_6(uint8_t ctrl_flag)
{
	switch (ctrl_flag)
	{
		case 0:
		{
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_2,500);
			break;
		}
		case 1:
		{
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_2,1500);
			break;
		}		
		default:
			break;		
	}
}
	
////�������ж�����ܺ�����
//void Servo_Ctrl_All(uint8_t servo_id, int16_t angle_for_1, uint8_t ctrl_flag)
//{
//	switch (servo_id)
//	{
//		case 1:
//		{
//			Servo_Ctrl_1(angle_for_1);//���ƻ�е�۵������Ķ����
//			break;
//		}
//		case 2:
//		{
//			Servo_Ctrl_2(ctrl_flag);//���ƻ�е�ۼ�צ�Ķ����
//			break;
//		}	
//		case 3:
//		{
//			Servo_Ctrl_3(ctrl_flag);//���Ʋֿ��·����ڵĶ����
//			break;
//		}	
//		case 4:
//		{
//			Servo_Ctrl_4(ctrl_flag);//���Ʋֿ��Ϸ����ڵĶ����
//			break;
//		}	
//		case 5:
//		{
//			Servo_Ctrl_5(ctrl_flag);//���Ʋֿ��Ϸ������Ķ����
//			break;
//		}	
//		case 6:
//		{
//			Servo_Ctrl_6(ctrl_flag);//���Ʋֿ�����������Ȧ�Ķ����
//			break;
//		}			
//		default:
//			break;
//	}

//}
