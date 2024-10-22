#ifndef CHASSIS_H
#define CHASSIS_H

#include "main.h"
#include "math.h"
#include "pid.h"
#include "can_receive.h"
#include "INS_task.h"


/*电机基础数据*/
#define M2006_MOTOR_SPEED_RATIO 36.0f//减速比
#define M2006_MOTOR_ECD_TO_ANGLE 8192

/*电机数据转换*/
#define ROUND_PI  3.1415926f   //pi
#define DIAMETER_OF_WHEEL 0.127f   //轮子直径，单位m
#define RADIUS_OF_CHASSIS 0.20266f   //底盘半径，单位m
#define M2006_MOTOR_RPM_TO_SPEED ((ROUND_PI*DIAMETER_OF_WHEEL)/(M2006_MOTOR_SPEED_RATIO*60))//(rpm)->(m/s)
#define TO_PI_RATE   (ROUND_PI/180.0f)
#define WZ_TO_SPEED (TO_PI_RATE * RADIUS_OF_CHASSIS)

//单个电机最大速度
#define MAX_WHEEL_SPEED 4.0f 

//速度PID
#define M2006_MOTOR_SPEED_PID_KP 18000.0f
#define M2006_MOTOR_SPEED_PID_KI 300.0f
#define M2006_MOTOR_SPEED_PID_KD 0.0f
#define M2006_MOTOR_SPEED_PID_MAX_OUT 8000.0f
#define M2006_MOTOR_SPEED_PID_MAX_IOUT 1200.0f //积分限幅
//底盘速度pid
#define CHASSIS_SPEED_PID_KP 3.0f
#define CHASSIS_SPEED_PID_KI 0.01f
#define CHASSIS_SPEED_PID_KD 0.0f
#define CHASSIS_SPEED_PID_MAX_OUT 1.0f
#define CHASSIS_SPEED_PID_MAX_IOUT 0.4f
//位置pid
#define CHASSIS_POSITION_PID_KP 12.0f//10.0f
#define CHASSIS_POSITION_PID_KI 0.0f
#define CHASSIS_POSITION_PID_KD 10.0f
#define CHASSIS_POSITION_PID_MAX_OUT 0.5f
#define CHASSIS_POSITION_PID_MAX_IOUT 0.2f
//角度环pid
#define CHASSIS_YAW_PID_KP 12.0f
#define CHASSIS_YAW_PID_KI 0.0f
#define CHASSIS_YAW_PID_KD 80.0f
#define CHASSIS_YAW_PID_MAX_OUT 90.0f
#define CHASSIS_YAW_PID_MAX_IOUT 40.0f

typedef struct
{
	const motor_measure_t *chassis_motor_measure;
	float speed;
	float speed_last;
	float speed_set;
	float distance;
	int16_t give_current;
}chassis_motor_t;

typedef struct
{                
	const float *chassis_INS_angle_degree;//获取陀螺仪解算出的欧拉角指针(角度制)
	
	chassis_motor_t motor_chassis[4];          
  
	pid_type_def motor_speed_pid[4];
	pid_type_def chassis_speed_pid_x;
	pid_type_def chassis_speed_pid_y;
	pid_type_def chassis_speed_pid_z;
	pid_type_def chassis_location_pid_x;
	pid_type_def chassis_location_pid_y;
	pid_type_def chassis_yaw_pid;
	
	float vx_last;
	float vy_last;
	float wz_last;
	float vx;
	float vy;
	float wz;
	float x;
	float y;
	float yaw;
	
	float vx_set;
	float vy_set;
	float wz_set;
	float x_set;
	float y_set;
	float yaw_set;
			
}chassis_move_t;



extern void chassis_vector_to_omni_wheel_speed(const float vx_set, const float vy_set, const float wz_set, float wheel_speed[4]);
extern void chassis_control_loop(chassis_move_t *chassis_move);
extern void chassis_feedback_update(chassis_move_t *chassis_move);
extern void chassis_move_control(float x_set, float y_set, float yaw_set, chassis_move_t *chassis_move_control);
extern void chassis_init(chassis_move_t *chassis_move);
extern void ramp_function_chassis(float *data_follow, float data_target, float k);
#endif

