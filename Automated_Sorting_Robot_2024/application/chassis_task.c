#include "chassis_task.h"

chassis_move_t chassis_move;
float mz = 0;
float mx = 0;
float my = 0;
void Chassis_task(void const * argument)
{
	
	chassis_init(&chassis_move);
	TimerHandle_t xTimer = xTimerCreate("MyTimer", pdMS_TO_TICKS(10), pdTRUE, (void *)0, vTimerCallback);
	HAL_UART_Receive_IT(&huart6,USART6_Receive_buf,sizeof(USART6_Receive_buf));
	while(!ins_init_flag)//等待陀螺仪初始化
		osDelay(1);
	
	xTimerStart(xTimer, 0);		
	chassis_work_flag = 0;
	while(1)
	{
		
		chassis_feedback_update(&chassis_move);
		
		
		
		
		//chassis_work();
		//speed_set(mx,my,mz);
		remote_control();
		chassis_control_loop(&chassis_move);
		
		CAN_cmd_chassis(chassis_move.motor_chassis[0].give_current, 
						chassis_move.motor_chassis[1].give_current,	
						chassis_move.motor_chassis[2].give_current, 
						chassis_move.motor_chassis[3].give_current);			
		osDelay(1);
	}
}
	

uint8_t location_set(float x_set, float y_set, float yaw_set)
{
	chassis_control(x_set, y_set, yaw_set, LOCATION_MODE, &chassis_move);
	
	float dx = fabs(x_set - chassis_move.x);
	float dy = fabs(y_set - chassis_move.y);
	float dz = fabs(yaw_set - chassis_move.yaw);
	
	if(dx < 0.01f && dy < 0.01f && dz < 0.1f)
	{
		chassis_odometry_reset(&chassis_move);
		
		return 1;
	}
	
	return 0;
}
void speed_set(float vx_set, float vy_set, float wz_set)
{
	chassis_control(vx_set, vy_set, wz_set, SPEED_MODE, &chassis_move);
}
void remote_control(void)
{
	chassis_control(0,0,0,RC_MODE,&chassis_move);
}
void chassis_work(void)
{
	/*待机*/
	if(chassis_work_flag == 0)
		speed_set(0,0,0);
	/*圆盘机*/
	if(chassis_work_flag == 1)
	{
		if(work_flag==0) if(location_set(0.58f+3.55f,0.58f,0)||target_cross_flag)   work_flag=1,target_cross_flag=0; 
		if(work_flag==1) if(location_set(0,0,90.0f)) 								work_flag=2;
		if(work_flag==2) if(location_set(0.2,-0.02,90.0f)||gray_stop_flag) 			work_flag=0,chassis_work_flag=0,gray_stop_flag=0;
		
	}
	/*阶梯平台以及避障*/
	if(chassis_work_flag == 2)
	{
		if(work_flag==0) if(location_set(-0.15f,0,90.0f)||target_cross_flag) 		work_flag=1,target_cross_flag=0;
		if(work_flag==1) if(location_set(0,0,0.0f)) 								work_flag=2;
		if(work_flag==2) if(location_set(0,-1.2f,0)||lidar_distance<350) 			work_flag=3,chassis_odometry_reset(&chassis_move);
		if(work_flag==3) if(location_set(-1.25f,0,0)) 								work_flag=4;
		if(work_flag==4) if(location_set(0,0,-90.0f)) 								work_flag=5;
		if(work_flag==5) if(location_set(1,-0.02,-90.0f)||target_cross_flag)		work_flag=6,target_cross_flag=0;
		if(work_flag==6) if(location_set(0,-0.2,-90.0f)) 							work_flag=7;
		if(work_flag==7) if(location_set(0.5f,0,-90.0f)||gray_stop_flag) chassis_work_flag=0;
		//if(work_flag==6) if(location_set()) work_flag=7;chassis_work_flag = 0;
		//if(work_flag==7) if()
		/*阶梯平台*/
	}
	/*立桩*/
	if(chassis_work_flag == 3)
	{
		if(work_flag==0) ;
	}
	/*放球*/
	
	/*回出发点*/
}
//0.01秒定时器回调
void vTimerCallback(TimerHandle_t xTimer)
{
	chassis_move.yaw_speed = (chassis_move.yaw - chassis_move.yaw_last) / 0.01f;
}
