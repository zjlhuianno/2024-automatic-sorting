#include "Double_Loop_PID.h"

PIDStruct PID_position1;
PIDStruct PID_position2;

//PID初始化。
void PID_Init(void)
{
	//位置环1。
	PID_position1.Kp = Kp_position1;
	PID_position1.Ki = Ki_position1;
	PID_position1.Kd = Kd_position1;
	
	PID_position1.error = 0.0f;
	PID_position1.lastError = 0.0f;
	
	PID_position1.integral = 0.0f;
	PID_position1.maxIntegral = 80.0f;
	
	PID_position1.output = 0.0f;
	PID_position1.maxOutput = 1000;
	
	//位置环2。
	PID_position2.Kp = Kp_position2;
	PID_position2.Ki = Ki_position2;
	PID_position2.Kd = Kd_position2;
	
	PID_position2.error = 0.0f;
	PID_position2.lastError = 0.0f;
	
	PID_position2.integral = 0.0f;
	PID_position2.maxIntegral = 80.0f;
	
	PID_position2.output = 0.0f;
	PID_position2.maxOutput = 1000;
}

//进行一次PID计算。
float PID_Calculate(PIDStruct *pid, float setpoint, float actual_value)
{
	//PID死区。
//	if ((pid -> error) < 0.5 && (pid -> error) > -0.5) { (pid -> error) = 0; }
	
	(pid -> error) = setpoint - actual_value;							//计算本次误差。
	(pid -> integral) += (pid -> error);									//累积误差。
  float error_gap = (pid -> error) - (pid -> lastError);//计算前后两次误差的差值。
	
	//积分限幅。
	if 			((pid -> Ki) * (pid -> integral) < -(pid -> maxIntegral)) { (pid -> integral) = -(pid -> maxIntegral) / (pid -> Ki); }
	else if ((pid -> Ki) * (pid -> integral) >  (pid -> maxIntegral)) { (pid -> integral) =  (pid -> maxIntegral) / (pid -> Ki); } 
	
	//加权求和。
	(pid -> output) = (pid -> Kp) * (pid -> error) + (pid -> Ki) * (pid -> integral) + (pid -> Kd) * error_gap;
	
	//输出限幅。
	if 			((pid -> output) < -(pid -> maxOutput))	{ (pid -> output) = -(pid -> maxOutput); }
	else if ((pid -> output) >  (pid -> maxOutput))	{ (pid -> output) =  (pid 	-> maxOutput); }
	
	(pid -> lastError) = (pid -> error);//更新上一次的误差。
	
	return (pid -> output);
}
