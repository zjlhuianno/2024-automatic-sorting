#include "chassis_task.h"

chassis_move_t chassis_move;
float mz = 0;
float mx = 0;
float my = 0;
extern uint8_t push_ball_mode;
extern uint16_t color;
extern int pos_frame_cnt;
int color_time = 0;
int ballout_time = 0;
void Chassis_task(void const * argument)
{
	
	chassis_init(&chassis_move);
	TimerHandle_t xTimer = xTimerCreate("MyTimer", pdMS_TO_TICKS(10), pdTRUE, (void *)0, vTimerCallback);
	HAL_UART_Receive_IT(&huart6,USART6_Receive_buf,sizeof(USART6_Receive_buf));
	while(!ins_init_flag)//等待陀螺仪初始化
		osDelay(1);
	
	xTimerStart(xTimer, 0);		
	chassis_work_flag = 1;
	work_flag=0;	
	mode_openmv = 2;
//	height_flag=1;
	while(1)
	{
		
		chassis_feedback_update(&chassis_move);
		
		
		
		chassis_work();
		//speed_set(0,0.2,-32.5);
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
		//0
		//2
		if(work_flag==0) if(location_set(3.9f,0.59f,0))   							work_flag=1;
		if(work_flag==1) if(location_set(0.7,0,0)||target_cross_flag)				work_flag=2,target_cross_flag=0,chassis_odometry_reset(&chassis_move);else speed_set(0.2,0,0);else;
		if(work_flag==2) if(location_set(0,0,89.5f)) 								work_flag=3;
		if(work_flag==3) if(location_set(0.2f,0,89.5f)||gray_stop_flag) 			work_flag=4,gray_stop_flag=0,chassis_odometry_reset(&chassis_move);else speed_set(0.2,0,0);else;
		if(work_flag==4) if(location_set(0,0,89.5f))								work_flag=0,chassis_work_flag=0,chassis_arm_comm_flag=1;
	}//
	/*阶梯平台以及避障*/
	if(chassis_work_flag == 2)
	{
		//0
		//5
		//7
		mode_openmv = 1;
		if(work_flag==0) if(location_set(-0.116f,0,89.0f)||target_cross_flag) 		work_flag=1,target_cross_flag=0,chassis_odometry_reset(&chassis_move);
		if(work_flag==1) if(location_set(0.01,0,0.0f)) 								work_flag=2;
		if(work_flag==2) if(location_set(0,-0.8f,0)||lidar_distance<350) 			work_flag=3,chassis_odometry_reset(&chassis_move);
		if(work_flag==3) if(location_set(-1.23f,0,0)) 								work_flag=4;
		if(work_flag==4) if(location_set(0,0,-89.0f)) 								work_flag=5;
		
		//标定
		if(work_flag==5) if(location_set(1.5,0,-88.0f)||target_cross_flag)			work_flag=6,target_cross_flag=0,chassis_odometry_reset(&chassis_move);
		if(work_flag==6) if(location_set(0,-0.2,-88.0f)) 							work_flag=7;
		if(work_flag==7) if(location_set(1.0f,0,-88.0f)||gray_stop_flag) 			work_flag=8,gray_stop_flag=0,height_flag=1,chassis_odometry_reset(&chassis_move);else speed_set(0.2,0,0);else;
		
		if(work_flag==8) if(chassis_move.y<-0.186f) 								work_flag=9,height_flag=2,begin_look_flag=0,arm_flag=0,chassis_odometry_reset(&chassis_move);else if(color==1) speed_set(0,0,0);else if(begin_look_flag==1) speed_set(0,-0.05,0);else speed_set(0,0,0);else;
		if(work_flag==9) if(chassis_move.y<-0.42f)									work_flag=10,height_flag=3,begin_look_flag=0,arm_flag=0,chassis_odometry_reset(&chassis_move);else if(color==1) speed_set(0,0,0);else if(begin_look_flag==1) speed_set(0.001,-0.05,0);else speed_set(0,0,0);else;
		if(work_flag==10)if(chassis_move.y<-0.22f)									work_flag=11,height_flag=2,begin_look_flag=0,arm_flag=0,chassis_odometry_reset(&chassis_move);else if(color==1) speed_set(0,0,0);else if(begin_look_flag==1) speed_set(0.001,-0.05,0);else speed_set(0,0,0);else;
		
		if(work_flag==11)if(location_set(0,0.85,-88.0f))							work_flag=0,chassis_work_flag=4;

	}
	/*立桩*/
	if(chassis_work_flag == 3)
	{
		mode_openmv = 2;
		if(work_flag==0) if(location_set(-0.5,0,-88.0f))							work_flag=1;
		if(work_flag==1) if(location_set(0,0,-180.0f)) 								work_flag=2;
		if(work_flag==2) if(chassis_move.y<-0.4f)									work_flag=3,chassis_arm_comm_flag=4,chassis_odometry_reset(&chassis_move);else speed_set(0.01,-0.2,0);else;
		if(work_flag==3) if(chassis_arm_comm_flag==6)								work_flag=4;else speed_set(0,0,0);else;	
		if(work_flag==4) if(chassis_move.y>2.11f) 									work_flag=0,chassis_arm_comm_flag=9,chassis_work_flag=4,chassis_odometry_reset(&chassis_move);else if(color==1) speed_set(0,0,0);else speed_set(0,0.2,-34.5);else;
	}
	/*放球*/
	if(chassis_work_flag == 4)
	{
		if(work_flag==0) if(location_set(-0.5,0.28,-87.0f))							work_flag=1;
		if(work_flag==1) if(location_set(0,0,-263.0f)) 								work_flag=2;
//		if(work_flag==0) if(chassis_move.x<-0.2)									work_flag=1,chassis_odometry_reset(&chassis_move);else speed_set(-0.2,0,0);else;
//		if(work_flag==1) if(location_set(0,0,-180.0f))								work_flag=2;
//		if(work_flag==2) if(location_set(0,-0.4,-180.0f))							work_flag=3;
//		if(work_flag==3) if(location_set(0,0,-260.0f))								work_flag=4;
		if(work_flag==2) if(location_set(0.83f,0,-263.0f))							work_flag=3;
		if(work_flag==3) if(location_set(0,0.585f,-263.0f))							work_flag=4;
		if(work_flag==4) if(chassis_move.x>0.5f||gray_stop_flag)					work_flag=0,ball_out_flag=1,chassis_work_flag=0,gray_stop_flag=0,chassis_odometry_reset(&chassis_move);else speed_set(0.2,0,0);else;

	}
	/*回出发点*/
	if(chassis_work_flag == 5)
	{
		if(work_flag==0) if(location_set(0,2.33f,-263.0f))						work_flag=1;
		if(work_flag==1) if(location_set(-0.7f,0,-263.0f))					work_flag=0,chassis_work_flag=0;
	}
}
//0.01秒定时器回调
void vTimerCallback(TimerHandle_t xTimer)
{
	
	if(push_ball_mode == 1 && pos_frame_cnt == 2 )
	{
		color_time++;
		if(color_time > 3000)
			chassis_arm_comm_flag = 2;
	}
	if(color == 1 || color == 3)
		color_time=0;
	
	if(ball_out_flag == 1)
	{
		ballout_time++;
	}
	if(ballout_time>500)
		chassis_work_flag = 5,ballout_time = 0,ball_out_flag=0;
		
	//chassis_move.yaw_speed = (chassis_move.yaw - chassis_move.yaw_last) / 0.01f;
}
