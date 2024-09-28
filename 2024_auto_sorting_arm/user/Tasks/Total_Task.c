#include "Includes.h"

float m_x = 38.0f;
float m_y = 7.0f;
float m_z = 40.0f;
float servo_arm_angle = 5.0f;

int i = 1;
void Total_Task(void const * argument)
{
	
	while(1)
	{

		//catch_object(0);
		if (i == 1)
		{
			target_x = 38.0f;
			target_y = 25.5f;
			target_angle = 40.0f;
			Servo_Ctrl_arm(10);
			osDelay(3000);
			i = 0;
		}

		
		target_x = m_x;//15.0f;
		target_y = m_y;//20.5f;
		target_angle = m_z;//100.0f;
		Servo_Ctrl_arm(servo_arm_angle);
		
		//舵机位置标定。
//		Servo_Ctrl_claw(2);
//		Servo_Ctrl_3(0);
//		Servo_Ctrl_6(0);
		
		osDelay(1);
	}
}

void catch_object(uint8_t flag)
{
	int force_sensing_cnt = 0;

	if (flag == 0)
	{
//		force_sensing_flag = 1;//启动力控感知。
		
		target_x = m_x;//15.0f;
		target_y = m_y;//20.5f;
		target_angle = m_z;//100.0f;
		
		
		
		
		
		
		
		
		
		
//		//力控感知。
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
//		}//力控感知结束。


		
		
		
		
		
	//flag == 0结束。
	}
	
	
}


//以下是测试程序。

		//测试此task的循环正在正常运行。
//		flag_z=1;
//		aRGB_led_show(0xFFFF0000);
//		osDelay(500);
//		aRGB_led_show(0x7F123456);
//		osDelay(500);

