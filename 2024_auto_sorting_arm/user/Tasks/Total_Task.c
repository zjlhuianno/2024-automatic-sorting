#include "Includes.h"

//float m_x = 38.0f;
//float m_y = 7.0f;
//float m_z = 40.0f;
//float servo_arm_angle = 5.0f;

int pos_frame_cnt = 0;//位置帧计时器。
int pos_stable_cnt = 0;//姿态稳定计时器。
int claw_freq_cnt = 0;//爪频率计时器。

int arm_move_mode = 1;//机械臂动作模式。

extern int arm_catch_flag;//夹爪舵机姿态标志位。

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
		if (arm_move_mode == 1)//低平台抓放方块。
		{
			if (pos_frame_cnt == 0)//机械臂伸到高缓冲处，爪张开。
			{
				set_arm_pos_param(38.0f, 30.0f, 30.0f, 5.0f);
				pos_stable_function(0, 1);//不是最后一帧，且爪张开。
			}
			else if (pos_frame_cnt == 1)//机械臂伸到低平台，爪取方块。
			{
				arm_catch_flag=2;//爪取方块。
				set_arm_pos_param(38.0f, 6.0f, 40.0f, 5.0f);
				pos_stable_function(0, 2);//不是最后一帧，且爪取方块。	
			}
			else if (pos_frame_cnt==2)//机械臂伸到低平台的较高处，爪仍保持抓方块的姿态，为的是防止爪放方块时方块会搓到阶梯平台。
			{
				set_arm_pos_param(43.0f, 18.9f, 40.0f, 10.0f);
				pos_stable_function(0, 2);//不是最后一帧，且爪取方块。
							
			}
			else if (pos_frame_cnt==3)//机械臂缩到方块仓库的上方，爪放方块。
			{
				set_arm_pos_param(20.0f, 25.5f, 40.0f, 5.0f);
				pos_stable_function(1, 1);//是最后一帧，且爪张开。
			}			
		}

		else if (arm_move_mode == 2)//立桩抓放球。
		{
			if (pos_frame_cnt == 0)//机械臂伸到高缓冲处，爪闭合。
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
						arm_catch_flag=0;//爪闭合。
						pos_frame_cnt = 1;
						pos_stable_cnt=0;
					}
				}
				
			}
			
			else if (pos_frame_cnt == 1)//机械臂伸到立桩，识别openmv。
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
						arm_catch_flag=0;//爪闭合。
						pos_frame_cnt = 2;
						pos_stable_cnt=0;
					}
				}		
				
			}		
			
			else if (pos_frame_cnt == 2)//机械臂伸到高平台，爪取球。
			{
				arm_catch_flag=1;//爪张开。
				
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
						arm_catch_flag=3;//爪取球。
						pos_frame_cnt = 3;
						pos_stable_cnt=0;
					}
				}		
				
			}
			
			else if (pos_frame_cnt==3)//机械臂伸到低缓冲区，爪放球。
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
						arm_catch_flag=1;//爪放球。
						pos_frame_cnt = 0;
						pos_stable_cnt=0;
						arm_move_mode =0;	
					}
				}		
							
			}				
		}

		else if (arm_move_mode == 3)
		{
			//圆盘机拨球。
			
			if (pos_frame_cnt == 0)//机械臂伸到圆盘机特定的高缓冲处，爪闭合。
			{
				target_x = 38.0f;
				target_y = 28.5f;
				target_angle = 20;
				Servo_Ctrl_arm(10.0f);
				osDelay(1000);
				arm_catch_flag=0;//爪闭合。
				
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
			else if (pos_frame_cnt == 1)//机械臂伸到圆盘机，爪拨球。
			{
				claw_freq_cnt++;
				target_x = 41.0f;
				target_y = 21.5f;
				target_angle = 20;
				Servo_Ctrl_arm(10.0f);
				osDelay(500);
				
				if (claw_freq_cnt %= 1500)
				{
					arm_catch_flag=4;//爪拨球。
					osDelay(500);
					arm_catch_flag=0;//爪闭合。
					
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
			
			else if (pos_frame_cnt==2)//机械臂缩到低缓冲区，结束拨球。
			{
				target_x = 10.0f;
				target_y = 30.5f;
				target_angle = 40;
				Servo_Ctrl_arm(40.0f);
				osDelay(1000);
				arm_catch_flag=0;//爪闭合。
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
		
		
		//低平台抓放甜甜圈。
		/*
		if (pos_frame_cnt == 0)//机械臂伸到缓冲处，爪张开。
		{
			target_x = 38.0f;
			target_y = 25.5f;
			target_angle = 40.0f;
			Servo_Ctrl_arm(10);
			arm_catch_flag=1;//爪张开。
			
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
		else if (pos_frame_cnt == 1)//机械臂伸到低平台，爪取甜甜圈。
		{
			target_x = 38.0f;
			target_y = 7.0f;
			target_angle = 40;
			Servo_Ctrl_arm(5.0f);
			osDelay(1000);
			arm_catch_flag=3;//爪取甜甜圈。
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
		else if (pos_frame_cnt==2)//机械臂伸到低平台的较高处，爪仍保持抓方块的姿态，为的是防止爪放方块时甜甜圈会搓到阶梯平台。
		{
			target_x = 43.0f;
			target_y = 18.9f;
			target_angle = 40;
			Servo_Ctrl_arm(10.0f);
			osDelay(1000);
			arm_catch_flag=3;//爪取甜甜圈。
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
		
		else if (pos_frame_cnt==3)//机械臂缩到方块仓库，爪放甜甜圈。
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
		if (pos_frame_cnt == 0)//机械臂伸到高缓冲处，爪闭合。
		{
			target_x = 38.0f;
			target_y = 30.0f;
			target_angle = 30;
			Servo_Ctrl_arm(5.0f);
			osDelay(1000);
			arm_catch_flag=0;//爪闭合。
			
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
		else if (pos_frame_cnt == 1)//机械臂伸到高平台，爪取甜甜圈。
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
				pos_stable_cnt++;
				if (pos_stable_cnt == 5)
				{
					pos_frame_cnt = 2;
					pos_stable_cnt=0;
				}
			}		
			
		}
		else if (pos_frame_cnt==2)//机械臂伸到高平台的较高处，爪仍保持抓甜甜圈的姿态，为的是防止爪放甜甜圈时甜甜圈会搓到阶梯平台。
		{
			target_x = 38.0f;
			target_y = 30.0f;
			target_angle = 30;
			Servo_Ctrl_arm(5.0f);
			osDelay(1000);
			arm_catch_flag=3;//爪取甜甜圈。
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
		
		else if (pos_frame_cnt==3)//机械臂伸到低缓冲区，爪仍保持抓甜甜圈的姿态，为的是防止爪放甜甜圈时甜甜圈会搓到阶梯平台。
		{
			target_x = 10.0f;
			target_y = 30.5f;
			target_angle = 40;
			Servo_Ctrl_arm(40.0f);
			osDelay(1000);
			arm_catch_flag=3;//爪取甜甜圈。
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
		
		else if (pos_frame_cnt==4)//机械臂缩到方块仓库，爪放方块。
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
		
		if (pos_frame_cnt == 0)//机械臂伸到高缓冲处，爪闭合。
		{
			target_x = 38.0f;
			target_y = 30.0f;
			target_angle = 30;
			Servo_Ctrl_arm(5.0f);
			osDelay(1000);
			arm_catch_flag=0;//爪闭合。
			
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
		
		else if (pos_frame_cnt == 1)//机械臂伸到立桩，识别openmv。
		{
			target_x = 42.0f;
			target_y = 21.5f;
			target_angle = 10;
			Servo_Ctrl_arm(20.0f);
			arm_catch_flag=0;//爪闭合。
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
		
		else if (pos_frame_cnt == 2)//机械臂伸到高平台，爪取球。
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
				pos_stable_cnt++;
				if (pos_stable_cnt == 10)
				{
					pos_frame_cnt = 3;
					pos_stable_cnt=0;
				}
			}		
			
		}
		
		else if (pos_frame_cnt==3)//机械臂伸到低缓冲区，爪放球。
		{
			target_x = 10.0f;
			target_y = 30.5f;
			target_angle = 40;
			Servo_Ctrl_arm(40.0f);
			osDelay(2000);
			arm_catch_flag=1;//爪放球。
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
		
		pos_stable_cnt++; //每隔一段时间，如果编码器数据在预测范围内，pos_stable_cnt++。
											//注意：这里的一段时间并不是由本函数内的osDelay(100)单方面决定的，还受到task的通讯频率的影响。
		
		if (pos_stable_cnt == 100)//姿态稳定一段时间以后，才允许执行下一帧。
		{
			pos_stable_cnt=0;
			
			arm_catch_flag=claw_mode;//直到机械臂姿态稳定，才改变夹爪的动作。
			
			if (is_last_pos_frame == 0)//如果不是该动作模组的最后一帧，则pos_frame_cnt++，执行下一步动作。
			{
				pos_frame_cnt++;
			}
			else if (is_last_pos_frame == 1)//如果是该动作模组的最后一帧，则pos_frame_cnt与arm_move_mode重置。
			{
				pos_frame_cnt = 0;
				arm_move_mode =0;
			}
		}
	}	
}

//以下是测试程序。

		//测试此task的循环正在正常运行。
//		flag_z=1;
//		aRGB_led_show(0xFFFF0000);
//		osDelay(500);
//		aRGB_led_show(0x7F123456);
//		osDelay(500);

