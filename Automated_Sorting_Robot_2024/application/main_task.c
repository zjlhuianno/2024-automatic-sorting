#include "main_task.h"
#include "DM_Control.h"
#include "Servo.h"
#include "math.h"
#include "stdlib.h"
#include "configs.h"
#include "ins_task.h"
#include "Arm_Task.h"
#include "tim.h"


int catch_object_mode = 0;//机械臂抓取动作模式。
int push_ball_mode = 0;//机械臂拨球动作模式。
int look_object_mode = 0;//机械臂看阶梯平台模式。
int disable_arm_flag = 0;//机械臂无力标志位。（1为无力。）

void main_task(void const * argument)
{
	while(!ins_init_flag)
	{
		osDelay(1);
	}
			
	disable_arm_flag = 0;//1为机械臂无力。
	while(1)
	{
//		mode_openmv = 1;//打开openmv的识别形状颜色模式。
//		mode_openmv = 2;//打开openmv的识别球颜色模式。
//		look_object_mode = 1;//低平台看。
		catch_object_mode = 1;//低平台抓取。
//		push_ball_mode = 1;//圆盘机拨球。
		
//		set_arm_pos_param_ture(38.0f, 20.0f, 0.0f);
		
		
//机械臂动作标志位判断。
		FUCK_YOU();
		
		//push_ball_mode = 1;//圆盘机拨球。
		
//		arm_flag = 1;
//		if(arm_flag == 1)
//		{
//			//disable_arm_flag = 0;
//			push_ball(1);
//			
//		}
		
		
		if(chassis_flag == 2)
		{
			osDelay(1000);
			chassis_flag = 3;
		}
		if(chassis_flag == 3)
		{
			osDelay(2000);
			chassis_flag = 4;
		}
		
		if(chassis_flag == 6)//g
		{
			osDelay(2000);
			chassis_flag = 7;
		}
		if(chassis_flag == 7)
		{
			osDelay(2000);
			chassis_flag = 8;
		}
		if(chassis_flag == 8)
		{
			osDelay(3000);
			chassis_flag = 9;
		}
		if(chassis_flag == 9)
		{
			osDelay(3000);
			chassis_flag = 10;
		}
		if(chassis_flag == 10)
		{
			osDelay(2000);
			chassis_flag = 11;
		}
		if(chassis_flag == 11)
		{
			osDelay(2000);
			chassis_flag = 12;
		}
		if(chassis_flag == 15)
		{
			osDelay(2000);
			chassis_flag = 16;
		}
		if(chassis_flag == 16)
		{
			osDelay(2000);
			chassis_flag = 17;
		}
		if(chassis_flag == 17)
		{
			osDelay(2000);
			chassis_flag = 18;
		}
		
//******************************************


//		if(arm_flag == 2)
//		{
//			osDelay(5000);
//			arm_flag = 3;
//		}



		//look_object(1);//高平台爪甜甜圈。
		
//			set_arm_pos_param(40.0f, 20.0f, 0.0f, 0.0f);//立桩平台。
//			pos_stable_function(0, 0);//不是最后一帧，且爪闭合。


//******************************************

//		chassis_flag = 1;
////		if(arm_flag == 1)
//			push_ball(1);
//		osDelay(5000);
//		no_ball_push_flag_openmv = 1;
//		osDelay(2000);
//		chassis_flag = 4;


		
		osDelay(1);
	}

}






//以下是测试程序。

//		Servo_Ctrl_arm(0.0f);
//		pile_approach_flag = 1;//甜甜圈塑料桩靠近。
//			set_arm_pos_param(38.0f, 20.0f, 0.0f, 10.0f);//圆盘机平台。
//			pos_stable_function(0, 0);//不是最后一帧，且爪闭合。
//		catch_object(2);
//		push_ball(1);

		//矫正机械臂舵机。
		//Servo_Ctrl_arm(30.0f);
		//__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,2500);

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


		/*
		if (arm_move_mode == 1)//低平台抓放方块。
		{
			if (pos_frame_cnt == 0)//机械臂伸到高缓冲处，爪张开。
			{
				set_arm_pos_param(38.0f, 30.0f, 30.0f, 5.0f);
				pos_stable_function(0, 1);//不是最后一帧，且爪张开。
				
			}
			else if (pos_frame_cnt == 1)//机械臂伸到低平台，爪取方块。
			{
				//arm_catch_flag=2;//爪取方块。
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
		*/


		//测试此task的循环正在正常运行。
//		flag_z=1;
//		aRGB_led_show(0xFFFF0000);
//		osDelay(500);
//		aRGB_led_show(0x7F123456);
//		osDelay(500);




