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
		
		Data_sum_gray1 = 0;//十字灰度
		for(i_gray = 0; i_gray < 8; i_gray++)
			Data_sum_gray1 += Data_gray[1][i_gray];
		
		/*十字路口计数，在圆盘机十字停下*/
		if(chassis_work_flag == 1 && Data_sum_gray1 > 1) cross_flag = 1;
		if(chassis_work_flag == 1 && cross_flag == 1 && data_gray[1] == 0x00) cross_cnt++,cross_flag = 0;
		if(chassis_work_flag == 1 && cross_cnt == 2)
			if(Data_sum_gray1 > 3)
				if(Data_gray[1][0] || Data_gray[1][7])
					target_cross_flag = 1;
				
		/*前灰度检测停止*/
		if(chassis_work_flag == 1 && work_flag == 3)
			if(data_gray[0] == 0xFF)
				gray_stop_flag = 1;
			
		/*退后至十字处*/
		if(chassis_work_flag == 2 && work_flag == 0)
			if(Data_sum_gray1 > 4)
				target_cross_flag = 1;
		
		/*阶梯平台十字*/
		if(chassis_work_flag == 2 && work_flag == 5)
			if(Data_sum_gray1 > 4)
				target_cross_flag = 1;
		if(chassis_work_flag == 2 && work_flag == 7)
			if(data_gray[0] == 0xFF)
				gray_stop_flag = 1;
		/*仓库白线*/
			if(chassis_work_flag == 4 && work_flag == 4)
				if(data_gray[0] == 0xFF)
					gray_stop_flag = 1;
		//！！！！由于是蓝光款，所以蓝方场地，cnt要到3！！！！！！
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

