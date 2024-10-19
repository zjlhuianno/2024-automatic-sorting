#include "chassis_task.h"

chassis_move_t chassis_move;

float m_x = 0.0f;
float m_y = 0.0f;
float m_z = 0.0f;
float t_x = 0.0f;
float t_y = 0.0f;
float t_z = 0.0f;

uint8_t num = 0;

void Chassis_task(void const * argument)
{

	
	chassis_init(&chassis_move);
	
	while(!ins_init_flag)//等待陀螺仪初始化
		osDelay(1);
	
	while(1)
	{

		chassis_feedback_update(&chassis_move);
		
		
		
		//底盘位置环
//		if(chassis_stop_flag == 1)
//		{
//			//odometryResetFlag = 1;
////			m_x = 0.0f;
////			m_y = 0.0f;
////			m_z = 0.0f;
//			chassis_stop_flag = 0;
//			
//		}
//		if(chassis_flag == 0)
//		{
//			//chassis_move_control(0.585f + 0.73f + 2.8f +m_x, 0.585f + m_y, m_z, &chassis_move);
//			m_x = 0.585f + 0.73f + 2.8f;
//			m_y = 0.585f;
//			m_z = 0;
//		}

//		if(chassis_flag == 1)
//		{
//			m_z = 90.0f;
//			if(chassis_move.yaw >= 90.0f)
//			{
//				chassis_flag = 2;
//				chassis_stop_flag =1;
//			} 
//		}
//		//圆盘机，到白线停止，发标志位
//		if(chassis_flag == 2)
//		{
//			m_x = 0.15f;
//		}
//		
//		//后退，到0x00，
//		if(chassis_flag == 3)
//		{
//			m_x = -0.15f;
//		}
//		//180度
//		if(chassis_flag == 4)
//		{
//			m_z = 180.0f; 
//			if(chassis_move.yaw > 180.0f)
//				chassis_flag = 5;
//		}
//		if(chassis_flag == 5)
//		{
//			m_y = 1.2f;//直接标y轴到十字处
//			if(avoidance_flag == 1)
//			{
//				chassis_flag = 6;
//			}
//			
//		}
//		//走到阶梯平台的十字处
//		if(chassis_flag == 6)
//		{
//			m_y = 1.2f;//also
//			m_x = 1.2f;
//			
//		}
//		//转向
//		if(chassis_flag == 7)
//		{
//			m_z = 270.0f;
//			if(chassis_move.yaw > 270.0f)
//			{
//				chassis_stop_flag = 1;
//				chassis_flag = 8;
//			}
//		}
//		//走到阶梯平台白线处
//		if(chassis_flag == 8)
//		{
//		
//		}
//		//暂定第一趟不识别白块，第二趟识别方块，后识别二维码
//		if(chassis_flag == 9)
//		{
//			
//		}
//		//后退到十字处
//		if(chassis_flag == 10)
//		{
//			
//		}
//		//1.后退到立桩左侧右转
//		//2.直接放球
//		if(chassis_flag == 11)
//		{
//			
//		}
		chassis_move_control(m_x, m_y, m_z, &chassis_move);
		
//		if(chassis_stop_flag == 1)
//		{
//			odometryResetFlag = 1;
//			m_x = 0.0f;
//			m_y = 0.0f;
//			chassis_move.vx_set = PID_calc(&(chassis_move.chassis_speed_pid_x), chassis_move.vx, 0);
//			chassis_move.vy_set = PID_calc(&(chassis_move.chassis_speed_pid_y), chassis_move.vy, 0);
//			//chassis_move.wz_set = PID_calc(&(chassis_move.chassis_speed_pid_z), chassis_move.wz, 0);
//			chassis_stop_flag = 0;
//		}

		chassis_control_loop(&chassis_move);
		
		CAN_cmd_chassis(chassis_move.motor_chassis[0].give_current, 
										chassis_move.motor_chassis[1].give_current,	
                    chassis_move.motor_chassis[2].give_current, 
										chassis_move.motor_chassis[3].give_current);
				
		osDelay(1);
	}
}

