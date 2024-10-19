#include "chassis.h"


float data_follow[3];
float ramp_k = 0.00065;
/*
   0/-\1
    | | 
   3\-/2
*/
/*
底盘速度解算
平动单位为(m)，左正右负，前正后负
转动单位为(度)，逆时针为正
*/
void chassis_vector_to_omni_wheel_speed(const float vx_set, const float vy_set, const float wz_set, float wheel_speed[4])
{
	wheel_speed[0] = ( - ( - vx_set + vy_set) * 0.70710678f - wz_set * WZ_TO_SPEED);
	wheel_speed[1] = ( - ( + vx_set + vy_set) * 0.70710678f - wz_set * WZ_TO_SPEED);
	wheel_speed[2] = ( - ( + vx_set - vy_set) * 0.70710678f - wz_set * WZ_TO_SPEED);
	wheel_speed[3] = ( - ( - vx_set - vy_set) * 0.70710678f - wz_set * WZ_TO_SPEED);
}

//控制循环，根据控制设定值，计算电机电流值，进行控制
void chassis_control_loop(chassis_move_t *chassis_move_control_loop)
{
	
    float max_speed = 0.0f;//用于存储四个轮子中最大速度的值
	float speed_rate = 0.0f;//比例因子，用于缩放速度以限制最大速度
    float temp = 0.0f;
	float wheel_speed[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    uint8_t i = 0;
	
    chassis_vector_to_omni_wheel_speed(chassis_move_control_loop->vx_set,
                                       chassis_move_control_loop->vy_set, 
									   chassis_move_control_loop->wz_set,
									   wheel_speed);
	
    //计算轮子控制最大速度，并限制其最大速度
	for (i = 0; i < 4; i++)
    {
        chassis_move_control_loop->motor_chassis[i].speed_set = wheel_speed[i];
        temp = fabs(chassis_move_control_loop->motor_chassis[i].speed_set);
        if (max_speed < temp)
            max_speed = temp;
    }
    if (max_speed > MAX_WHEEL_SPEED)
    {
        speed_rate = MAX_WHEEL_SPEED / max_speed;
        for (i = 0; i < 4; i++)
            chassis_move_control_loop->motor_chassis[i].speed_set *= speed_rate;
    }
	
    //计算pid
    for (i = 0; i < 4; i++)
        PID_calc(&chassis_move_control_loop->motor_speed_pid[i], chassis_move_control_loop->motor_chassis[i].speed, chassis_move_control_loop->motor_chassis[i].speed_set);
	//赋值电流值
    for (i = 0; i < 4; i++)
        chassis_move_control_loop->motor_chassis[i].give_current = (int16_t)(chassis_move_control_loop->motor_speed_pid[i].out);
	
}

//更新底盘数据
void chassis_feedback_update(chassis_move_t *chassis_move_update)
{
	if (chassis_move_update == NULL)
    {
        return;
    }
	
    uint8_t i = 0;
    for (i = 0; i < 4; i++)
	{
		chassis_move_update->motor_chassis[i].speed_last = chassis_move_update->motor_chassis[i].speed;
		chassis_move_update->motor_chassis[i].speed = M2006_MOTOR_RPM_TO_SPEED * chassis_move_update->motor_chassis[i].chassis_motor_measure->speed_rpm*0.15f
												    + 0.85f * chassis_move_update->motor_chassis[i].speed_last;
		chassis_move_update->motor_chassis[i].distance = chassis_move_update->motor_chassis[i].chassis_motor_measure->code 
														/(M2006_MOTOR_ECD_TO_ANGLE * M2006_MOTOR_SPEED_RATIO) 
														* ROUND_PI * DIAMETER_OF_WHEEL;
	}

	
	chassis_move_update->vx_last = chassis_move_update->vx;
	chassis_move_update->vx = ((- chassis_move_update->motor_chassis[1].speed 
								+ chassis_move_update->motor_chassis[0].speed
							    - chassis_move_update->motor_chassis[2].speed
							    + chassis_move_update->motor_chassis[3].speed)
							    / 4.0f / 0.70710678f) * 0.15f + chassis_move_update->vx_last * 0.85f;
	chassis_move_update->vy_last = chassis_move_update->vy;
	chassis_move_update->vy = ((- chassis_move_update->motor_chassis[0].speed 
								+ chassis_move_update->motor_chassis[2].speed
								- chassis_move_update->motor_chassis[1].speed
							    + chassis_move_update->motor_chassis[3].speed)
							    / 4.0f / 0.70710678f) * 0.15f + chassis_move_update->vy_last * 0.85f;
	chassis_move_update->wz = -(+ chassis_move_update->motor_chassis[1].speed 
								+ chassis_move_update->motor_chassis[0].speed 
								+ chassis_move_update->motor_chassis[3].speed 
								+ chassis_move_update->motor_chassis[2].speed)
							    / WZ_TO_SPEED / 4;


	chassis_move_update->x = (+ chassis_move_update->motor_chassis[0].distance
							  - chassis_move_update->motor_chassis[1].distance
							  - chassis_move_update->motor_chassis[2].distance
							  + chassis_move_update->motor_chassis[3].distance)
							  / 4.0f / 0.70710678f;
	chassis_move_update->y = (- chassis_move_update->motor_chassis[0].distance
							  - chassis_move_update->motor_chassis[1].distance
							  + chassis_move_update->motor_chassis[2].distance
							  + chassis_move_update->motor_chassis[3].distance)
							  / 4.0f / 0.70710678f;	

	
	chassis_move_update->yaw = *(chassis_move_update->chassis_INS_angle_degree + INS_YAW_ADDRESS_OFFSET);
	if(chassis_move_update->yaw < -50)
		chassis_move_update->yaw += 360;
}

//底盘移动控制
void chassis_move_control(float x_set, float y_set, float yaw_set, chassis_move_t *chassis_move_control)
{
	//加入斜坡函数实现缓起
	ramp_function(data_follow, x_set, ramp_k);
	ramp_function(data_follow + 1, y_set, ramp_k);
	ramp_function(data_follow + 1 + 1, yaw_set, ramp_k*1000);
	
	chassis_move_control->x_set = data_follow[0];
	chassis_move_control->y_set = data_follow[1];
	chassis_move_control->yaw_set = data_follow[2];

	chassis_move_control->vx_set = PID_calc(&chassis_move_control->chassis_location_pid_x, chassis_move_control->x, chassis_move_control->x_set);
	chassis_move_control->vy_set = PID_calc(&chassis_move_control->chassis_location_pid_y, chassis_move_control->y, chassis_move_control->y_set);
	chassis_move_control->wz_set = PID_calc(&chassis_move_control->chassis_yaw_pid, chassis_move_control->yaw, chassis_move_control->yaw_set);
}

void chassis_init(chassis_move_t *chassis_move_init)
{
	if (chassis_move_init == NULL)
    {
        return;
    }
	
    //底盘速度环pid值
    const static float motor_speed_pid[3] = { M2006_MOTOR_SPEED_PID_KP, M2006_MOTOR_SPEED_PID_KI, M2006_MOTOR_SPEED_PID_KD };
    const static float chassis_speed_pid[3] = { CHASSIS_SPEED_PID_KP, CHASSIS_SPEED_PID_KI, CHASSIS_SPEED_PID_KD };
	const static float chassis_location_pid[3] = { CHASSIS_POSITION_PID_KP, CHASSIS_POSITION_PID_KI, CHASSIS_POSITION_PID_KD };
	const static float chassis_yaw_pid[3] = { CHASSIS_YAW_PID_KP, CHASSIS_YAW_PID_KI, CHASSIS_YAW_PID_KD };
	uint8_t i;

    //获取陀螺仪姿态角指针
    chassis_move_init->chassis_INS_angle_degree = get_INS_angle_deg_point();
	
    //获取底盘电机数据指针，初始化PID 
    for (i = 0; i < 4; i++)
    {
		chassis_move_init->x = 0;
		chassis_move_init->y = 0;
        chassis_move_init->motor_chassis[i].chassis_motor_measure = get_chassis_motor_measure_point(i);
		PID_init(&chassis_move_init->motor_speed_pid[i], PID_POSITION, motor_speed_pid, M2006_MOTOR_SPEED_PID_MAX_OUT, M2006_MOTOR_SPEED_PID_MAX_IOUT);
    }
	
	PID_init(&chassis_move_init->chassis_speed_pid_x, PID_POSITION, chassis_speed_pid,CHASSIS_SPEED_PID_MAX_OUT ,CHASSIS_SPEED_PID_MAX_IOUT);
	PID_init(&chassis_move_init->chassis_speed_pid_y, PID_POSITION, chassis_speed_pid,CHASSIS_SPEED_PID_MAX_OUT ,CHASSIS_SPEED_PID_MAX_IOUT);
	PID_init(&chassis_move_init->chassis_speed_pid_z, PID_POSITION, chassis_speed_pid,CHASSIS_SPEED_PID_MAX_OUT ,CHASSIS_SPEED_PID_MAX_IOUT);

	PID_init(&chassis_move_init->chassis_location_pid_x, PID_POSITION, chassis_location_pid,CHASSIS_POSITION_PID_MAX_OUT ,CHASSIS_POSITION_PID_MAX_IOUT);
	PID_init(&chassis_move_init->chassis_location_pid_y, PID_POSITION, chassis_location_pid,CHASSIS_POSITION_PID_MAX_OUT ,CHASSIS_POSITION_PID_MAX_IOUT);

	PID_init(&chassis_move_init->chassis_yaw_pid, PID_POSITION, chassis_yaw_pid,CHASSIS_YAW_PID_MAX_OUT ,CHASSIS_YAW_PID_MAX_IOUT);
	
    //更新一下数据
    chassis_feedback_update(chassis_move_init);
}

//斜坡函数
//k：步进的幅度，结合task里面的osDelay可以计算出每秒data_follow会向data_target逼近多少
void ramp_function(float *data_follow, float data_target, float k)
{
	if(data_target != *data_follow)
	{
		if(fabs(data_target - *data_follow) < k / 10.0f)
		{
			*data_follow = data_target;
		}
		else
		{
			*data_follow += k * (data_target - *data_follow> 0 ? 1.0f : - 1.0f);
		}
	}
}
