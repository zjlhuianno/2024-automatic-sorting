#include "Servo.h"
#include "configs.h"
#include "tim.h"


//ID == 1：机械臂第三级的舵机。							位置：tim1, TIM_CHANNEL_1。
//ID == 2：机械臂夹爪的舵机。  							位置：tim1, TIM_CHANNEL_2。
//ID == 3：仓库下方出口的舵机。  						位置：tim1, TIM_CHANNEL_3。
//ID == 4：仓库上方进口的舵机。  						位置：tim1, TIM_CHANNEL_4。
//ID == 5：仓库上方分流的舵机。 						位置：tim8, TIM_CHANNEL_1。
//ID == 6：仓库侧面放置甜甜圈的舵机。  			位置：tim8, TIM_CHANNEL_2。

void Servo_Init(void)
{
	HAL_TIM_Base_Start(&htim1);//开启TIM1。
	HAL_TIM_Base_Start(&htim8);//开启TIM8。
	
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //开启PWM通道，ID为1，这是控制机械臂第三级的舵机。
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);	//开启PWM通道，ID为2，这是控制机械臂夹爪的舵机。
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3); //开启PWM通道，ID为3，这是控制仓库下方出口的舵机。
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);	//开启PWM通道，ID为4，这是控制仓库上方进口的舵机。 
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1); //开启PWM通道，ID为5，这是控制仓库上方分流的舵机。 	
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);	//开启PWM通道，ID为6，这是控制仓库侧面放置甜甜圈的舵机。	
	
}

//将目标角度转换为机械臂第三级的舵机的pwm值。
float Servo_arm_angle_to_pwm(int16_t angle)
{
	//angle为γ = ∠B'CD。
	float pwm = 0.0f;
	pwm = 700 + 2000 * (angle / 180.0f);
	if (pwm < 500)
	{
		pwm = 500;
	}
	if (pwm > 1800)//1800 100度左右。
	{
		pwm = 1800;
	}	
	return pwm;
}

//控制机械臂第三级的舵机。
void Servo_Ctrl_arm(int16_t angle_for_servo_arm)
{
	//pwm=700时，第三级机械臂水平。
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,Servo_arm_angle_to_pwm(angle_for_servo_arm));
}

//控制机械臂夹爪的舵机。
//ctrl_flag：	0：机械爪闭合，pwm = 2400。
//						1：机械爪张开，pwm = 2000。
//						2：机械爪抓取方块，pwm = 2350。
//						3：机械爪抓取甜甜圈，pwm = 2200。
//						4：机械爪拨球，pwm = 1900。
void Servo_Ctrl_claw(uint8_t ctrl_flag)
{
	switch (ctrl_flag)
	{
		case 0:
		{
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,2400);
			break;			
		}
		case 1:
		{
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,2000);
			break;			
		}
		case 2:
		{
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,2350);
			break;			
		}		
		case 3:
		{
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,2200);
			break;			
		}	
		case 4:
		{
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,1900);
			break;			
		}		
		default:
			break;
	}
}

//控制仓库下方出口的舵机。
//ctrl_flag：	0：出口闭合，pwm = 1650。
//						1：出口打开，pwm = 850。
//						2：出口半打开，pwm = 1500。
void Servo_Ctrl_3(uint8_t ctrl_flag)
{
	switch (ctrl_flag)
	{
		case 0:
		{
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 1650);
			break;
		}
		case 1:
		{
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 850);
			break;
		}
		case 2:
		{
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 1500);
			break;
		}		
		default:
			break;		
	}
}

//控制仓库上方进口的舵机。
//ctrl_flag：	0：接球，pwm = 650。
//						1：接方块，pwm = 1200。
void Servo_Ctrl_4(uint8_t ctrl_flag)
{
	switch (ctrl_flag)
	{
		case 0:
		{
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 650);
			break;
		}
		case 1:
		{
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 1200);
			break;
		}		
			
		default:
			break;		
	}
}

//控制仓库上方分流的舵机。
//ctrl_flag：	0：分流至蓝球，pwm = 1800。
//						1：分流至黄球，pwm = 1100。
void Servo_Ctrl_5(uint8_t ctrl_flag)
{
	switch (ctrl_flag)
	{
		case 0:
		{
			__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 1800);
			break;
		}
		case 1:
		{
			__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 2200);
			break;
		}
		default:
			break;		
	}
}

//控制仓库侧面放置甜甜圈的舵机。
//ctrl_flag：	0：收纳仓靠近仓库，pwm = 500。
//						1：收纳仓远离仓库，pwm = 1500。
void Servo_Ctrl_6(uint8_t ctrl_flag)
{
	switch (ctrl_flag)
	{
		case 0:
		{
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_2,500);
			break;
		}
		case 1:
		{
			__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_2,1500);
			break;
		}		
		default:
			break;		
	}
}
	
////控制所有舵机的总函数。
//void Servo_Ctrl_All(uint8_t servo_id, int16_t angle_for_1, uint8_t ctrl_flag)
//{
//	switch (servo_id)
//	{
//		case 1:
//		{
//			Servo_Ctrl_1(angle_for_1);//控制机械臂第三级的舵机。
//			break;
//		}
//		case 2:
//		{
//			Servo_Ctrl_2(ctrl_flag);//控制机械臂夹爪的舵机。
//			break;
//		}	
//		case 3:
//		{
//			Servo_Ctrl_3(ctrl_flag);//控制仓库下方出口的舵机。
//			break;
//		}	
//		case 4:
//		{
//			Servo_Ctrl_4(ctrl_flag);//控制仓库上方进口的舵机。
//			break;
//		}	
//		case 5:
//		{
//			Servo_Ctrl_5(ctrl_flag);//控制仓库上方分流的舵机。
//			break;
//		}	
//		case 6:
//		{
//			Servo_Ctrl_6(ctrl_flag);//控制仓库侧面放置甜甜圈的舵机。
//			break;
//		}			
//		default:
//			break;
//	}

//}
