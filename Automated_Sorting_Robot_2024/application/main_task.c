#include "main_task.h"
#include "DM_Control.h"
#include "Servo.h"
#include "math.h"
#include "stdlib.h"
#include "configs.h"
#include "ins_task.h"
#include "Arm_Task.h"
#include "tim.h"
#include "remote_control.h"

int catch_object_mode = 0;//机械臂抓取动作模式。
int push_ball_mode = 0;//机械臂拨球动作模式。
int look_object_mode = 0;//机械臂看阶梯平台模式。
int disable_arm_flag = 0;//机械臂无力标志位。（1为无力。）



extern uint8_t rxBuffer_main[2];
extern uint8_t mode_openmv, flag_openmv, color, shape,flag_openmv_ball;

void main_task(void const * argument)
{
	HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
	while(!ins_init_flag)
	{
		osDelay(1);
	}
	disable_arm_flag = 0;//当为1时无力。
	
	Servo_Ctrl_claw(0);//机械爪闭合。
	Servo_Ctrl_3(2);//出口半打开。
	Servo_Ctrl_6(0);//甜甜圈塑料桩远离。
	while(1)
	{
		if(rc_ctrl.rc.s[1] == 3)
		{
			disable_arm_flag = 1;
		}
		else
		{
			disable_arm_flag = 0;//当为1时无力。
		}
			
//		mode_openmv = 1;//打开openmv的识别形状颜色模式。
//		mode_openmv = 2;//打开openmv的识别球颜色模式。

//		catch_object(7);//立桩抓球。
		

		//圆盘机拨球。
		if (chassis_arm_comm_flag == 1 || chassis_arm_comm_flag == 2)
		{
			push_ball(1);//圆盘机拨球。
		}
		
		//低平台看和识别抓取。
		if (height_flag == 1)
		{
			if (arm_flag % 2 == 0)
			{
				look_object(1);//低平台看。
			}
			if ((arm_flag % 2 == 1)
				&& (color == 1) && (shape == 2 || shape == 3))//红色方块或矩形。
			{
				begin_look_flag = 0;//结束看。
				catch_object(1);//低平台抓方块。
			}
			else if ((arm_flag % 2 == 1)
					&& (color == 1) && (shape == 1))//红色圆环。
			{
				begin_look_flag = 0;//结束看。
				catch_object(2);//低平台抓甜甜圈。
			}
			else if ((arm_flag % 2 == 1)
					&& (color == 2))//如果是蓝色。
			{
				begin_look_flag = 0;//结束看。
				arm_flag++;
			}				
		}
		//高平台看和识别抓取。
		if (height_flag == 2)
			
		{
			if (arm_flag % 2 == 0)
			{
				look_object(2);//高平台看。
			}
			if ((arm_flag % 2 == 1)
				&& (color == 1) && (shape == 2 || shape == 3))//红色方块或矩形。
			{
				begin_look_flag = 0;//结束看。
				catch_object(3);//高平台抓方块。
			}
			else if ((arm_flag % 2 == 1)
					&& (color == 1) && (shape == 1))//红色圆环。
			{
				begin_look_flag = 0;//结束看。
				catch_object(4);//高平台抓甜甜圈。
			}
			else if ((arm_flag % 2 == 1)
					&& (color == 2))//蓝色。
			{
				begin_look_flag = 0;//结束看。
				arm_flag++;
			}				
		}		
		//中平台看和识别抓取。
		if (height_flag == 3)
		{
			if (arm_flag % 2 == 0)
			{
				look_object(3);//中平台看。
			}
			if ((arm_flag % 2 == 1)
				&& (color == 1) && (shape == 2 || shape == 3))//红色方块或矩形。
			{
				begin_look_flag = 0;//结束看。
				catch_object(5);//中平台抓方块。
			}
			else if ((arm_flag % 2 == 1)
					&& (color == 1) && (shape == 1))//红色圆环。
			{
				begin_look_flag = 0;//结束看。
				catch_object(6);//中平台抓甜甜圈。
			}
			else if ((arm_flag % 2 == 1)
					&& (color == 2))//蓝色。
			{
				begin_look_flag = 0;//结束看。
				arm_flag++;
			}				
		}		
		
		//立桩抓球。
//		if (chassis_arm_comm_flag == 4)
//		{
//			catch_object(7);//立桩抓球。
//		}
		
//		//立桩拨球。
//		if (chassis_arm_comm_flag == 5 || chassis_arm_comm_flag == 6 || chassis_arm_comm_flag == 9)
//		{
//			push_ball(2);//立桩拨球。
//		}
		

		

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
