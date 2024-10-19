#include "main.h"
#include "cmsis_os.h"
#include "Servo.h"
#include "DM_Control.h"
#include "bsp_can.h"
#include "ins_task.h"
#include "usart.h"
#include "configs.h"
#include "Arm_Task.h"
#include "math.h"
#include "stdlib.h"


int arm_init_flag = 0;//机械臂初始化标志位。

int pos_frame_cnt = 0;//位置帧计步器。
int pos_stable_cnt = 0;//姿态稳定计时器。
int claw_freq_cnt = 0;//爪频率计时器。

extern int catch_object_mode;//机械臂抓取动作模式。
extern int push_ball_mode;//机械臂拨球动作模式。
extern uint8_t rxBuffer_main[2];

int cnt_zzz = 0;

void Arm_Task(void const * argument)
{
	while(!ins_init_flag)
		osDelay(1);
	if (arm_init_flag == 0)
	{
		Servo_Init();
		DM_Data_Init();
		DM_PID_Init();
//		HAL_UART_Receive_IT(&huart1, rxBuffer_main, sizeof(rxBuffer_main));
		
		vTaskDelay(1000);
		
		do
		{
			Enable_DM_Motor(&hcan2,DM4340_ID);
			Enable_DM_Motor(&hcan2,DM4310_ID);
			osDelay(1);
		} while(DM4340_Data.state == 0 || DM4310_Data.state == 0);
		
		arm_init_flag=1;
	}
	
	while(1)
	{	
//		Arm_Ctrl(target_x,target_y, target_angle);
		//HAL_UART_Transmit_IT(&huart1,&mode,1);	
		
		
		if (arm_catch_flag == 0)//机械爪闭合。
		{
			Servo_Ctrl_claw(0);
		}		
		if (arm_catch_flag == 1)//机械爪张开。
		{
			Servo_Ctrl_claw(1);
		}
		if (arm_catch_flag == 2)//机械爪抓取方块和甜甜圈。
		{
			Servo_Ctrl_claw(2);
		}
		if (arm_catch_flag == 3)//机械爪抓取甜甜圈。
		{
			Servo_Ctrl_claw(3);
		}
//		if (arm_catch_flag == 4)//机械爪拨球一次。
//		{
////			Servo_Ctrl_claw(4);
//			Servo_Ctrl_claw(1);//机械爪张开。
//			osDelay(500);
//			Servo_Ctrl_claw(0);//机械爪闭合。
//			osDelay(500);
//		}		
		
		if (pile_approach_flag == 0)//甜甜圈塑料桩远离。
		{
			Servo_Ctrl_6(0);
		}			
		if (pile_approach_flag == 1)//甜甜圈塑料桩靠近。
		{
			Servo_Ctrl_6(1);
		}	
		
		if (shunt_ball_flag == 0)//将球分流至己方轨道。
		{
			Servo_Ctrl_5(0);
		}
		if (shunt_ball_flag == 1)//将球分流至中立轨道。
		{
			Servo_Ctrl_5(1);
		}		
		
		
//		DM4310_delta_enc = DM4310_Data.p_int - DM4310_Data.last_p_int;
//		last_DM4310_delta_enc = DM4310_delta_enc;
		
		osDelay(1);
	}
}

void push_ball(uint8_t mode)
{
	//	int force_sensing_cnt = 0;//力控感知计时器。
	
	if (mode == 1)//圆盘机拨球。
	{
//		force_sensing_flag = 1;//启动力控感知。
	
		pile_approach_flag = 0;//甜甜圈塑料桩远离。
			
		if (pos_frame_cnt == 0)//机械臂伸到高缓冲处，爪闭合。
		{
			set_arm_pos_param(38.0f, 30.0f, 30.0f, 5.0f);//高缓冲处。
			pos_stable_function(0, 0);//不是最后一帧，且爪闭合。			
		}
		else if (pos_frame_cnt == 1)//机械臂伸到圆盘机平台，爪按需拨球。
		{
			set_arm_pos_param(38.0f, 20.0f, 0.0f, 10.0f);//圆盘机平台。
			pos_stable_function(0, 0);//不是最后一帧，且爪闭合。
		}
		else if (pos_frame_cnt==2)//机械臂保持圆盘机平台位置，并按需拨球。
		{
			set_arm_pos_param(38.0f, 21.0f, 0.0f, 0.0f);//圆盘机平台。
			//pos_stable_function(0, 1);//不是最后一帧，且爪拨球一次。
			
			if (should_push_ball_flag_openmv == 1)//该标志位的初始化值需要在加上视觉后修改为0。
			{
				//爪拨球一次。
				arm_catch_flag = 1;
				osDelay(500);
				arm_catch_flag = 0;
				osDelay(500);				
				//should_push_ball_flag_openmv = 0;//加上视觉后须打开。
				cnt_zzz++;
				if (cnt_zzz == 5)
				{
					should_push_ball_flag_openmv = 0;
					no_ball_push_flag_openmv = 1;
					cnt_zzz=0;
				}
				
			}
			if (ball_color_flag_openmv == 0)//如果是己方球（红球或蓝球），则将球分流至己方轨道。
			{
				shunt_ball_flag = 0;
			}
			if (ball_color_flag_openmv == 1)//如果是中立球（黄球），则将球分流至中立轨道。
			{
				shunt_ball_flag = 1;
			}
			if (no_ball_push_flag_openmv == 1)//如果识别到无球可拨，则结束此动作帧。
			{
				pos_frame_cnt++;
				no_ball_push_flag_openmv = 0;
				should_push_ball_flag_openmv = 1;//加上视觉后须删除。
			}
			
		}		
		else if (pos_frame_cnt==3)//机械臂伸到高缓冲处3，结束拨球。
		{
			set_arm_pos_param(20.0f, 26.0f, 0.0f, 35.0f);//高缓冲处3。
			pos_stable_function(1, 0);//是最后一帧，且爪闭合。
		}
	}//mode == 1结束。	
	
		
		
		
		
		
	
			//力控感知。
//		while(force_sensing_flag)
//		{
//			target_y-=0.03f;
//			if(target_y<5)
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
//						target_y+=1;
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
	
}

void catch_object(uint8_t mode)
{
//	int force_sensing_cnt = 0;//力控感知计时器。

	if (mode == 1)//低平台夹方块。
	{
//		force_sensing_flag = 1;//启动力控感知。
	
		pile_approach_flag = 0;//甜甜圈塑料桩远离。
			
		if (pos_frame_cnt == 0)//机械臂伸到高缓冲处，爪闭合。
		{
			set_arm_pos_param(38.0f, 30.0f, 30.0f, 5.0f);//高缓冲处。
			
			if (openmv_ok_flag == 0)//openmv未识别到数据。
			{
				arm_catch_flag=0;	//爪闭合。
			}
			else if (openmv_ok_flag == 1)//openmv识别到了数据。
			{
				pos_stable_function(0, 1);//不是最后一帧，且爪张开。
			}				
		}
		else if (pos_frame_cnt == 1)//机械臂伸到低平台，爪取方块。
		{
			set_arm_pos_param(38.0f, 18.0f, 40.0f, 5.0f);//低平台。
			pos_stable_function(0, 2);//不是最后一帧，且爪取方块。	
		}
		else if (pos_frame_cnt==2)//机械臂伸到低平台的较高处，爪仍保持抓方块的姿态，为的是防止爪放方块时方块会搓到阶梯平台。
		{
			set_arm_pos_param(43.0f, 20.0f, 40.0f, 10.0f);//低平台的较高处。
			pos_stable_function(0, 2);//不是最后一帧，且爪取方块。
						
		}
		else if (pos_frame_cnt==3)//机械臂缩到方块仓库的上方，爪放方块。
		{
			set_arm_pos_param(20.0f, 30.0f, 40.0f, 5.0f);//方块仓库的上方。
			pos_stable_function(1, 1);//是最后一帧，且爪张开。
		}			
	}//mode == 1结束。
	
		
	if (mode == 2)//低平台夹甜甜圈。
	{
//		force_sensing_flag = 1;//启动力控感知。
	
		pile_approach_flag = 1;//甜甜圈塑料桩靠近。
	
		if (pos_frame_cnt == 0)//机械臂伸到高缓冲处，爪闭合。
		{
			set_arm_pos_param(38.0f, 30.0f, 30.0f, 5.0f);//高缓冲处。
			
			if (openmv_ok_flag == 0)//openmv未识别到数据。
			{
				arm_catch_flag=0;	//爪闭合。
			}
			else if (openmv_ok_flag == 1)//openmv识别到了数据。
			{
				pos_stable_function(0, 1);//不是最后一帧，且爪张开。
			}
		}
		else if (pos_frame_cnt == 1)//机械臂伸到低平台，爪取甜甜圈。
		{
			set_arm_pos_param(38.0f, 17.5f, 40.0f, 5.0f);//低平台。
			pos_stable_function(0, 2);//不是最后一帧，且爪取甜甜圈。	
		}
		else if (pos_frame_cnt==2)//机械臂伸到低平台的较高处，爪仍保持抓甜甜圈的姿态，为的是防止爪放方块时方块会搓到阶梯平台。
		{
			set_arm_pos_param(43.0f, 20.0f, 40.0f, 10.0f);//低平台的较高处。
			pos_stable_function(0, 2);//不是最后一帧，且爪取甜甜圈。
		}
		else if (pos_frame_cnt==3)//机械臂伸到高缓冲处，爪仍保持抓甜甜圈的姿态，为的是防止爪放方块时方块会搓到阶梯平台。
		{
			set_arm_pos_param(38.0f, 30.0f, 30.0f, 5.0f);//高缓冲处。
			//set_arm_pos_param(20.0f, 35.0f, 8.0f, 55.0f);
			pos_stable_function(0, 2);//不是最后一帧，且爪取甜甜圈。
		}			
		else if (pos_frame_cnt==4)//机械臂伸到高缓冲处2，爪仍保持抓甜甜圈的姿态，为的是防止爪放方块时方块会搓到阶梯平台。
		{
			set_arm_pos_param(20.0f, 35.0f, 8.0f, 55.0f);//高缓冲处2。
			pos_stable_function(0, 2);//不是最后一帧，且爪取甜甜圈。
		}					
		else if (pos_frame_cnt==5)//机械臂缩到甜甜圈仓库的上方，爪放甜甜圈。
		{
			set_arm_pos_param(15.0f, 16.0f, 0.0f, 30.0f);//甜甜圈仓库的上方。
			pos_stable_function(1, 1);//是最后一帧，且爪张开。
		}			
	}//mode == 2结束。		
		
	if (mode == 3)//高平台夹方块。
	{
//		force_sensing_flag = 1;//启动力控感知。
	
		pile_approach_flag = 0;//甜甜圈塑料桩远离。
			
		if (pos_frame_cnt == 0)//机械臂伸到高缓冲处，爪闭合。
		{
			set_arm_pos_param(38.0f, 30.0f, 30.0f, 5.0f);//高缓冲处。
			
			if (openmv_ok_flag == 0)//openmv未识别到数据。
			{
				arm_catch_flag=0;	//爪闭合。
			}
			else if (openmv_ok_flag == 1)//openmv识别到了数据。
			{
				pos_stable_function(0, 1);//不是最后一帧，且爪张开。
			}
		}
		else if (pos_frame_cnt == 1)//机械臂伸到高平台，爪取方块。
		{
			set_arm_pos_param(37.5f, 27.5f, 25.600061f, 10.0f);//高平台。
			pos_stable_function(0, 2);//不是最后一帧，且爪取方块。	
		}
		else if (pos_frame_cnt==2)//机械臂伸到高缓冲处2，爪仍保持抓方块的姿态，为的是防止爪放方块时方块会搓到阶梯平台。
		{
			set_arm_pos_param(20.0f, 35.0f, 8.0f, 55.0f);//高缓冲处2。
			pos_stable_function(1, 1);//是最后一帧，且爪张开。
		}
	}//mode == 3结束。

	if (mode == 4)//高平台夹甜甜圈。
	{
//		force_sensing_flag = 1;//启动力控感知。
	
		pile_approach_flag = 1;//甜甜圈塑料桩靠近。
			
		if (pos_frame_cnt == 0)//机械臂伸到高缓冲处，爪闭合。
		{
			set_arm_pos_param(38.0f, 30.0f, 30.0f, 5.0f);//高缓冲处。
			
			if (openmv_ok_flag == 0)//openmv未识别到数据。
			{
				arm_catch_flag=0;	//爪闭合。
			}
			else if (openmv_ok_flag == 1)//openmv识别到了数据。
			{
				pos_stable_function(0, 1);//不是最后一帧，且爪张开。
			}
		}
		else if (pos_frame_cnt == 1)//机械臂伸到高平台，爪取甜甜圈。
		{
			set_arm_pos_param(37.5f, 27.5f, 25.600061f, 10.0f);//高平台。
			pos_stable_function(0, 2);//不是最后一帧，且爪取甜甜圈。	
		}
		else if (pos_frame_cnt==2)//机械臂伸到高缓冲处，爪仍保持抓甜甜圈的姿态，为的是防止爪放方块时方块会搓到阶梯平台。
		{
			set_arm_pos_param(38.0f, 30.0f, 30.0f, 5.0f);//高缓冲处。
			pos_stable_function(0, 2);//不是最后一帧，且爪取甜甜圈。
		}			
		else if (pos_frame_cnt==3)//机械臂伸到高缓冲处2，爪仍保持抓甜甜圈的姿态，为的是防止爪放方块时方块会搓到阶梯平台。
		{
			set_arm_pos_param(20.0f, 35.0f, 8.0f, 55.0f);//高缓冲处2。
			pos_stable_function(0, 2);//不是最后一帧，且爪取甜甜圈。
		}					
		else if (pos_frame_cnt==4)//机械臂缩到甜甜圈仓库的上方，爪放甜甜圈。
		{
			set_arm_pos_param(15.0f, 16.0f, 0.0f, 30.0f);//甜甜圈仓库的上方。
			pos_stable_function(1, 1);//是最后一帧，且爪张开。
		}			
	}//mode == 4结束。		
	
		
	if (mode == 5)//中平台夹方块。
	{
//		force_sensing_flag = 1;//启动力控感知。
	
		pile_approach_flag = 0;//甜甜圈塑料桩远离。
			
		if (pos_frame_cnt == 0)//机械臂伸到高缓冲处，爪闭合。
		{
			set_arm_pos_param(38.0f, 30.0f, 30.0f, 5.0f);//高缓冲处。
			
			if (openmv_ok_flag == 0)//openmv未识别到数据。
			{
				arm_catch_flag=0;	//爪闭合。
			}
			else if (openmv_ok_flag == 1)//openmv识别到了数据。
			{
				pos_stable_function(0, 1);//不是最后一帧，且爪张开。
			}				
		}
		else if (pos_frame_cnt == 1)//机械臂伸到中平台，爪取方块。
		{
			set_arm_pos_param(38.5f, 17.5f, 0.0f, 5.0f);//中平台。
			pos_stable_function(0, 2);//不是最后一帧，且爪取方块。	
		}
		else if (pos_frame_cnt==2)//机械臂伸到中平台的较高处，爪仍保持抓方块的姿态，为的是防止爪放方块时方块会搓到阶梯平台。
		{
			set_arm_pos_param(42.0f, 22.0f, 6.499996f, 10.0f);//中平台的较高处。
			pos_stable_function(0, 2);//不是最后一帧，且爪取方块。
		}
		else if (pos_frame_cnt==3)//机械臂缩到方块仓库的上方，爪放方块。
		{
			set_arm_pos_param(20.0f, 30.0f, 40.0f, 5.0f);//方块仓库的上方。
			pos_stable_function(1, 1);//是最后一帧，且爪张开。
		}			
	}//mode == 5结束。
		

	if (mode == 6)//中平台夹甜甜圈。
	{
//		force_sensing_flag = 1;//启动力控感知。
	
		pile_approach_flag = 1;//甜甜圈塑料桩靠近。
	
		if (pos_frame_cnt == 0)//机械臂伸到高缓冲处，爪闭合。
		{
			set_arm_pos_param(38.0f, 30.0f, 30.0f, 5.0f);//高缓冲处。
			
			if (openmv_ok_flag == 0)//openmv未识别到数据。
			{
				arm_catch_flag=0;	//爪闭合。
			}
			else if (openmv_ok_flag == 1)//openmv识别到了数据。
			{
				pos_stable_function(0, 1);//不是最后一帧，且爪张开。
			}
		}
		else if (pos_frame_cnt == 1)//机械臂伸到中平台，爪取甜甜圈。
		{
			set_arm_pos_param(38.5f, 17.5f, 0.0f, 5.0f);//中平台。
			pos_stable_function(0, 2);//不是最后一帧，且爪取甜甜圈。	
		}
		else if (pos_frame_cnt==2)//机械臂伸到中平台的较高处，爪仍保持抓甜甜圈的姿态，为的是防止爪放方块时方块会搓到阶梯平台。
		{
			set_arm_pos_param(42.0f, 22.0f, 6.499996f, 10.0f);//中平台的较高处。
			pos_stable_function(0, 2);//不是最后一帧，且爪取甜甜圈。
		}			
		else if (pos_frame_cnt==3)//机械臂伸到高缓冲处3，爪仍保持抓甜甜圈的姿态，为的是防止爪放方块时方块会搓到阶梯平台。
		{
			set_arm_pos_param(20.0f, 26.0f, 0.0f, 35.0f);//高缓冲处3。
			pos_stable_function(0, 2);//不是最后一帧，且爪取甜甜圈。
		}					
		else if (pos_frame_cnt==4)//机械臂缩到甜甜圈仓库的上方，爪放甜甜圈。
		{
			set_arm_pos_param(15.0f, 16.0f, 0.0f, 30.0f);//甜甜圈仓库的上方。
			pos_stable_function(1, 1);//是最后一帧，且爪张开。
		}			
	}//mode == 6结束。				


	if (mode == 7)//把甜甜圈从机器桩夹到木桩。
	{
//		force_sensing_flag = 1;//启动力控感知。
	
		pile_approach_flag = 1;//甜甜圈塑料桩靠近。
	
		if (pos_frame_cnt == 0)//机械臂伸到甜甜圈仓库的上上方，爪张开。
		{
			set_arm_pos_param(15.0f, 30.0f, 0.0f, 55.0f);//甜甜圈仓库的上上方。
			pos_stable_function(0, 1);//不是最后一帧，且爪张开。
		}
		else if (pos_frame_cnt == 1)//机械臂伸到中平台，爪取甜甜圈。
		{
			set_arm_pos_param(15.0f, 13.0f, 0.0f, 30.0f);//甜甜圈仓库的上方。
			pos_stable_function(0, 2);//不是最后一帧，且爪取甜甜圈。	
		}
		else if (pos_frame_cnt==2)//机械臂伸到甜甜圈仓库的上上方，爪仍保持抓甜甜圈的姿态，为的是防止爪放甜甜圈时会干涉到塑料桩。
		{
			set_arm_pos_param(15.0f, 30.0f, 0.0f, 55.0f);//甜甜圈仓库的上上方。
			pos_stable_function(0, 2);//不是最后一帧，且爪取甜甜圈。
		}			
		else if (pos_frame_cnt==3)//机械臂伸到木桩上方，爪仍保持抓甜甜圈的姿态，为的是防止爪放方块时方块会搓到阶梯平台。
		{
			set_arm_pos_param(32.0f, 34.0f, 21.100044f, 40.0f);//木桩上方。
			pos_stable_function(1, 1);//是最后一帧，且爪张开。
		}					
	}//mode == 7结束。						
			
		//力控感知。
//		while(force_sensing_flag)
//		{
//			target_y-=0.03f;
//			if(target_y<5)
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
//						target_y+=1;
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

		
		if (pos_stable_cnt == 20)//姿态稳定一段时间以后，才允许执行下一帧。
		{
			pos_stable_cnt=0;
			arm_catch_flag=claw_mode;//直到机械臂姿态稳定，才改变夹爪的动作。
			osDelay(1000);//改变夹爪动作一段时间后，保证夹爪能稳定夹到物体后，再执行下一步动作。
			
			if (is_last_pos_frame == 0)//如果不是该动作模组的最后一帧，则pos_frame_cnt++，执行下一步动作。
			{
				pos_frame_cnt++;
			}
			else if (is_last_pos_frame == 1)//如果是该动作模组的最后一帧，则pos_frame_cnt, catch_object_mode, push_ball_mode重置。
			{
				pos_frame_cnt = 0;
				catch_object_mode =0;
				push_ball_mode = 0;
			}
		}
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


