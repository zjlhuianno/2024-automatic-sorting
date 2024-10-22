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
	chassis_flag = 0;
	while(1)
	{

		chassis_feedback_update(&chassis_move);
		
	
//		if(chassis_flag == 0)
//		{
//			chassis_stop_flag = 1;
//		}
//		if(chassis_flag == 1)
//		{
//			m_x = 0.555f + 0.73f + 2.92f;
//			m_y = 0.555f;
//			m_z = 0;
//		}
//		if(chassis_flag == 2)
//		{
//			chassis_stop_flag = 1;
//			m_z = 0.0f;
//			if(chassis_move.yaw >= 90)
//			{
//				chassis_flag = 3;
//				//arm_flag = 1;
//			}
//		}
//		if(chassis_flag == 3)
//		{
//			m_z = 90.0f;
//		}
//		if(chassis_flag == 4)
//		{
//			m_x += 0.15f;
//		}
//		
//		if(chassis_flag == 5)
//		{
//			m_x = -0.15f;
//		}
//		if(chassis_flag == 6)
//		{
//			m_z = 0.0f;//0.0f;
//			
//		}
//		if(chassis_flag == 7)
//		{
//			m_y = -1.2f;
//		}
//		if(chassis_flag == 8)
//		{
//			m_x = -1.2f;
//		}
//		
//		if(chassis_flag == 9)
//		{
//			m_z = -90.0f;
//			
//		}
//		if(chassis_flag == 10)
//		{
//			m_x = 0.33f;
//			m_y = -0.1f;
//		}
		if(chassis_flag == 11)
		{
			m_x = 0.15f;//这些灰度检测停止的位置，要标定差不多，防止最后灰度寄导致卡死
		}
		if(chassis_flag == 12)
		{
			m_x = 0.0f;//待标定
		}
		//立桩
		
//		m_x = t_x;
//		m_y = t_y;
//		m_z = t_z;
		chassis_move_control(m_x, m_y, m_z, &chassis_move);
		
		//立桩//180 / (ROUND_PI * 320)
//		chassis_move.vy_set = 0.5f;
//		chassis_move.wz_set = 180 / (ROUND_PI * 320) * 0.5f;
		if(chassis_flag == 13)
		{
			chassis_move.vy_set = PID_calc(&(chassis_move.chassis_speed_pid_y), chassis_move.vy, -0.2f);
			chassis_move.wz_set = PID_calc(&(chassis_move.chassis_speed_pid_z), chassis_move.wz, 180 / (ROUND_PI * 0.35) * 0.2f);
//			if(ins_beyond_flag == 0 && chassis_move.yaw > 180.0f)
//			{
//				ins_beyond_flag = 1;
//			}
//			if(ins_beyond_flag == 1 && fabs(chassis_move.yaw) <= 1.0f)
//			{
//				chassis_flag = 0;
//			}
		}
		if(chassis_stop_flag == 1)
		{
			odometryResetFlag = 1;
			m_x = 0.0f;
			m_y = 0.0f;
			chassis_move.vx_set = PID_calc(&(chassis_move.chassis_speed_pid_x), chassis_move.vx, 0);
			chassis_move.vy_set = PID_calc(&(chassis_move.chassis_speed_pid_y), chassis_move.vy, 0);
			//chassis_move.wz_set = PID_calc(&(chassis_move.chassis_speed_pid_z), chassis_move.wz, 0);
			chassis_stop_flag = 0;
		}

		chassis_control_loop(&chassis_move);
		
		CAN_cmd_chassis(chassis_move.motor_chassis[0].give_current, 
						chassis_move.motor_chassis[1].give_current,	
						chassis_move.motor_chassis[2].give_current, 
						chassis_move.motor_chassis[3].give_current);
				
		osDelay(1);
	}

}
