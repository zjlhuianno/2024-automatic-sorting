#include "Includes.h"

//float m_x = 38.0f;
//float m_y = 7.0f;
//float m_z = 40.0f;
//float servo_arm_angle = 5.0f;

int i = 0;
int ii = 0;
int iii = 0;//用于控制爪的频率。

extern int arm_catch_flag;
void Total_Task(void const * argument)
{
	
	osDelay(1500); 
	while(1)
	{
//			target_x = 20.0f;
//			target_y = 23.5f;
//			target_angle = 40;
//			Servo_Ctrl_arm(5);
//			arm_catch_flag=1;
		
		
		
		//catch_object(0);
		
		//低平台抓放方块。
		
		if (i == 0)//机械臂伸到高缓冲处，爪张开。
		{
			target_x = 38.0f;
			target_y = 30.0f;
			target_angle = 30.0f;
			Servo_Ctrl_arm(5);
			osDelay(1000); 
			
			
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.1 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.1)
			{
				ii++;
				if (ii == 10)
				{
					arm_catch_flag=1;//爪张开。
					i = 1;
					ii=0;
				}
			}
			
		}
		else if (i == 1)//机械臂伸到低平台，爪取方块。
		{
			target_x = 38.0f;
			target_y = 6.0f;
			target_angle = 40;
			Servo_Ctrl_arm(5.0f);
			osDelay(1000);
			
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.1 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.1)
			{
				ii++;
				if (ii == 3)
				{
					arm_catch_flag=2;//爪取方块。
					i = 2;
					ii=0;
				}
			}		
			
		}
		else if (i==2)//机械臂伸到低平台的较高处，爪仍保持抓方块的姿态，为的是防止爪放方块时方块会搓到阶梯平台。
		{
			target_x = 43.0f;
			target_y = 18.9f;
			target_angle = 40;
			Servo_Ctrl_arm(10.0f);
			osDelay(1000);
			
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.1 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.1)
			{
				ii++;
				if (ii == 3)
				{
					arm_catch_flag=2;//爪取方块。
					i = 3;
					ii=0;
				}
			}		
						
		}
		
		else if (i==3)//机械臂缩到方块仓库的上方，爪放方块。
		{
			target_x = 20.0f;
			target_y = 25.5f;
			target_angle = 40;
			Servo_Ctrl_arm(5);
			osDelay(2000);
			arm_catch_flag=1;			
		}
		
		
		//低平台抓放甜甜圈。
		/*
		if (i == 0)//机械臂伸到缓冲处，爪张开。
		{
			target_x = 38.0f;
			target_y = 25.5f;
			target_angle = 40.0f;
			Servo_Ctrl_arm(10);
			arm_catch_flag=1;//爪张开。
			
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
		else if (i == 1)//机械臂伸到低平台，爪取甜甜圈。
		{
			target_x = 38.0f;
			target_y = 7.0f;
			target_angle = 40;
			Servo_Ctrl_arm(5.0f);
			osDelay(1000);
			arm_catch_flag=3;//爪取甜甜圈。
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
		else if (i==2)//机械臂伸到低平台的较高处，爪仍保持抓方块的姿态，为的是防止爪放方块时甜甜圈会搓到阶梯平台。
		{
			target_x = 43.0f;
			target_y = 18.9f;
			target_angle = 40;
			Servo_Ctrl_arm(10.0f);
			osDelay(1000);
			arm_catch_flag=3;//爪取甜甜圈。
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
		
		else if (i==3)//机械臂缩到方块仓库，爪放甜甜圈。
		{
			target_x = 27.0f;
			target_y = 10.5f;
			target_angle = 40;
			Servo_Ctrl_arm(5);
			osDelay(2000);
			arm_catch_flag=1;	//爪张开。		
		}
		*/
		
		
		//高平台抓放甜甜圈。
		/*
		if (i == 0)//机械臂伸到高缓冲处，爪闭合。
		{
			target_x = 38.0f;
			target_y = 30.0f;
			target_angle = 30;
			Servo_Ctrl_arm(5.0f);
			osDelay(1000);
			arm_catch_flag=0;//爪闭合。
			
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
		else if (i == 1)//机械臂伸到高平台，爪取甜甜圈。
		{
			arm_catch_flag=1;//爪张开。
			
			target_x = 38.0f;
			target_y = 25.5f;
			target_angle = 20;
			Servo_Ctrl_arm(10.0f);
			osDelay(1000);
			arm_catch_flag=3;//爪取甜甜圈。
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
		else if (i==2)//机械臂伸到高平台的较高处，爪仍保持抓甜甜圈的姿态，为的是防止爪放甜甜圈时甜甜圈会搓到阶梯平台。
		{
			target_x = 38.0f;
			target_y = 30.0f;
			target_angle = 30;
			Servo_Ctrl_arm(5.0f);
			osDelay(1000);
			arm_catch_flag=3;//爪取甜甜圈。
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
		
		else if (i==3)//机械臂伸到低缓冲区，爪仍保持抓甜甜圈的姿态，为的是防止爪放甜甜圈时甜甜圈会搓到阶梯平台。
		{
			target_x = 10.0f;
			target_y = 30.5f;
			target_angle = 40;
			Servo_Ctrl_arm(40.0f);
			osDelay(1000);
			arm_catch_flag=3;//爪取甜甜圈。
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
		
		else if (i==4)//机械臂缩到方块仓库，爪放方块。
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
		//立桩抓放球。
		
		if (i == 0)//机械臂伸到高缓冲处，爪闭合。
		{
			target_x = 38.0f;
			target_y = 30.0f;
			target_angle = 30;
			Servo_Ctrl_arm(5.0f);
			osDelay(1000);
			arm_catch_flag=0;//爪闭合。
			
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.1 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.1)
			{
				ii++;
				if (ii == 10)
				{
					i = 1;
					ii=0;
				}
			}
			
		}
		
		else if (i == 1)//机械臂伸到立桩，识别openmv。
		{
			target_x = 42.0f;
			target_y = 21.5f;
			target_angle = 10;
			Servo_Ctrl_arm(20.0f);
			arm_catch_flag=0;//爪闭合。
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.1 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.1)
			{
				ii++;
				if (ii == 10)
				{
					i = 2;
					ii=0;
				}
			}		
			
		}		
		
		else if (i == 2)//机械臂伸到高平台，爪取球。
		{
			arm_catch_flag=1;//爪张开。
			
			target_x = 40.0f;
			target_y = 21.5f;
			target_angle = 20;
			Servo_Ctrl_arm(10.0f);
			osDelay(1500);
			arm_catch_flag=3;//爪取球。
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.1 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.1)
			{
				ii++;
				if (ii == 10)
				{
					i = 3;
					ii=0;
				}
			}		
			
		}
		
		else if (i==3)//机械臂伸到低缓冲区，爪放球。
		{
			target_x = 10.0f;
			target_y = 30.5f;
			target_angle = 40;
			Servo_Ctrl_arm(40.0f);
			osDelay(2000);
			arm_catch_flag=1;//爪放球。
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.2 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.2)
			{
				ii++;
				if (ii == 2)
				{
					i = 4;
					ii=0;
				}
			}		
						
		}		
		*/
		
		
		//圆盘机拨球。
		/*
		if (i == 0)//机械臂伸到圆盘机特定的高缓冲处，爪闭合。
		{
			target_x = 38.0f;
			target_y = 28.5f;
			target_angle = 20;
			Servo_Ctrl_arm(10.0f);
			osDelay(1000);
			arm_catch_flag=0;//爪闭合。
			
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.1 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.1)
			{
				ii++;
				if (ii == 10)
				{
					i = 1;
					ii=0;
				}
			}
			
		}
		else if (i == 1)//机械臂伸到圆盘机，爪拨球。
		{
			iii++;
			target_x = 41.0f;
			target_y = 21.5f;
			target_angle = 20;
			Servo_Ctrl_arm(10.0f);
			osDelay(500);
			
			if (iii %= 1500)
			{
				arm_catch_flag=4;//爪拨球。
				osDelay(500);
				arm_catch_flag=0;//爪闭合。
				
				iii=0;
			}			
			if (abs(arms_js_data[1] == DM4310_enc_p_int_to_angle(DM4310_Data.p_int)) < 0.1 && abs(arms_js_data[2] == DM4340_enc_p_int_to_angle(DM4340_Data.p_int)) < 0.1)
			{
				ii++;
				if (ii == 20)
				{
					i = 2;
					ii=0;
				}
			}		
			
		}
		
		else if (i==2)//机械臂缩到低缓冲区，结束拨球。
		{
			target_x = 10.0f;
			target_y = 30.5f;
			target_angle = 40;
			Servo_Ctrl_arm(40.0f);
			osDelay(1000);
			arm_catch_flag=0;//爪闭合。
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
		*/
		
		
		//舵机位置标定。
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
//		force_sensing_flag = 1;//启动力控感知。
		
//		target_x = m_x;//15.0f;
//		target_y = m_y;//20.5f;
//		target_angle = m_z;//100.0f;
		
		
		
		
		
		
		
		
		
		
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

