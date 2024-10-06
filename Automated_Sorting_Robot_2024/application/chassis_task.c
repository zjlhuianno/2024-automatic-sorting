#include "chassis_task.h"


chassis_move_t chassis_move;

float m_x = 0.0f;
float m_y = 0.0f;
float m_z = 0.0f;

void Chassis_task(void const * argument)
{
while(!ins_init_flag)//等待陀螺仪初始化
		osDelay(1);
	
	chassis_init(&chassis_move);
	
	
	
	while(1)
	{

		chassis_feedback_update(&chassis_move);
		
		//单个轮子轮子速度pid调节
		/*
		PID_calc(&(chassis_move.motor_speed_pid[0]), chassis_move.motor_chassis[0].speed, m_x);
		chassis_move.motor_chassis[0].give_current = (int16_t)(chassis_move.motor_speed_pid[0].out);
		CAN_cmd_chassis(chassis_move.motor_chassis[0].give_current, 
						0,	
                        0, 
						0);
		VOFA(2, chassis_move.motor_chassis[0].speed, m_x);
		*/
		
		//方向检测，前正，左正，逆正
		/*
		chassis_vector_to_omni_wheel_speed(m_x, m_y, m_z, wheel_speed);
		for (i = 0; i < 4; i++)
		{
			
			PID_calc(&(chassis_move.motor_speed_pid[i]), chassis_move.motor_chassis[i].speed, wheel_speed[i]);
			chassis_move.motor_chassis[i].give_current = (int16_t)(chassis_move.motor_speed_pid[i].out);
		}
		VOFA(2, chassis_move.vy, m_y);
		*/
		
		//底盘速度环&&角度环&&转向环
		/*
		chassis_move.vx_set = PID_calc(&(chassis_move.chassis_speed_pid_x), chassis_move.vx, m_x);
		chassis_move.vy_set = PID_calc(&(chassis_move.chassis_speed_pid_y), chassis_move.vy, m_y);
		chassis_move.wz_set = PID_calc(&(chassis_move.chassis_yaw_pid), chassis_move.yaw, m_z);
		*/
		
		/*
		for (i = 0; i < 4; i++)
		{
			chassis_move.motor_chassis[i].speed_set = wheel_speed[i];
			PID_calc(&(chassis_move.motor_speed_pid[i]), chassis_move.motor_chassis[i].speed, chassis_move.motor_chassis[i].speed_set);
			chassis_move.motor_chassis[i].give_current = (int16_t)(chassis_move.motor_speed_pid[i].out);
		}
		*/

		//底盘位置环
		//chassis_move_control(m_x, m_y, m_z, &chassis_move);
		
		//chassis_control_loop(&chassis_move);
//		CAN_cmd_chassis(chassis_move.motor_chassis[0].give_current, 
//						chassis_move.motor_chassis[1].give_current,	
//                        chassis_move.motor_chassis[2].give_current, 
//						chassis_move.motor_chassis[3].give_current);

				
		osDelay(1);
	}
}

