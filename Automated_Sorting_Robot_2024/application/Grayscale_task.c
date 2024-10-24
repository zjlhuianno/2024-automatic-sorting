#include "Grayscale_task.h"
//前面0，中间1
//黑零白一
int ret_receive = 1;
int ret_transmit = 1;
uint8_t i2c_cmd = 0xDD;
uint8_t i_gray = 0;
uint8_t data_gray[2];
uint8_t Data_gray[2][8];
uint8_t Data_sum_gray1;
uint8_t Grayscale_ADDR[2] = { 0x4D, 0x4C };
uint8_t Grayscale_num = 0;
uint8_t cross_cnt = 0;


void Grayscale_task(void const * argument)
{
	
	while(!ins_init_flag)
		osDelay(1);
	
	while(1)
	{
		Grayscale_num = 0;grayscale_GetData(Grayscale_num);
		Grayscale_num = 1;grayscale_GetData(Grayscale_num);
		Data_sum_gray1 = 0;
		for(i_gray = 0; i_gray < 8; i_gray++)
			Data_sum_gray1 += Data_gray[1][i_gray];
		
		if(chassis_flag == 1 && Data_sum_gray1 > 1)
		{
			cross_flag = 1;
			
		}
		if(chassis_flag == 1 && cross_flag == 1 && data_gray[1] == 0x00)
		{
			cross_cnt++;
			cross_flag = 0;
		}
		if(chassis_flag == 1 && cross_cnt == 2)
		{
			if(Data_sum_gray1 > 1)
			{
				if(Data_gray[1][0] || Data_gray[1][7])
				{
					//chassis_stop_flag = 1;
					chassis_flag = 2;
				}
			}
		}
		if(chassis_flag == 4 && data_gray[0] == 0xFF)
		{
			chassis_flag = 0;
			arm_flag = 1;
			//chassis_stop_flag = 1;
			//arm_flag = 1;
		}
		if(chassis_flag == 5 && Data_sum_gray1 > 4)
		{
			chassis_stop_flag = 1;
			chassis_flag = 6;
		}
		if(chassis_flag == 9 && Data_sum_gray1 > 4)
		{
			chassis_stop_flag = 1;
		}
		if(chassis_flag == 10 && data_gray[0] == 0xFF)
		{
			chassis_flag = 0;
		}

		if(chassis_flag == 14 && Data_sum_gray1 > 4)
		{
			chassis_stop_flag = 1;
			chassis_flag = 16;
		}
		//！！！！由于是蓝光款，所以蓝方场地，cnt要到3！！！！！！
//		if(chassis_flag == 11)
//		{
//			chassis_flag = 0;
//			
//		}
		osDelay(1);
	}
	
}

void grayscale_GetData(uint8_t num)
{
	while(ret_transmit != HAL_OK)
	{
		ret_transmit = HAL_I2C_Master_Transmit(&hi2c2, Grayscale_ADDR[num] << 1, &i2c_cmd, 1, 100);
	}
	ret_transmit = 1;
//	ret_transmit = HAL_I2C_Master_Transmit(&hi2c2, Grayscale_ADDR[num] << 1, &i2c_cmd, 1, 100);
//	if(ret_transmit != HAL_OK)
//	{
//		ret_transmit = HAL_I2C_Master_Transmit(&hi2c2, Grayscale_ADDR[num] << 1, &i2c_cmd, 1, 100);
//	}
	while(ret_receive != HAL_OK)
	{
		ret_receive = HAL_I2C_Master_Receive(&hi2c2,((Grayscale_ADDR[num] << 1)|0x01) , data_gray+num, 1,100);
	}
	ret_receive = 1;
	for(i_gray=0;i_gray<8;i_gray++)
	{
		Data_gray[num][i_gray]=(data_gray[num] >> i_gray) & 0x01;
	}
}

