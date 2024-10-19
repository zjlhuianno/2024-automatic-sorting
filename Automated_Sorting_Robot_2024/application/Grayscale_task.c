#include "Grayscale_task.h"
//前面0，中间1
//黑零白一
int ret_receive = 1;
int ret_transmit = 1;
uint8_t i2c_cmd = 0xDD;
uint8_t i_gray=0;
uint8_t data_gray[2];
uint8_t Data_gray[2][8];
uint8_t Grayscale_ADDR[2] = { 0x4D, 0x4C };
uint8_t Grayscale_num = 0;
uint8_t cross_cnt = 0;
uint8_t cnt = 0;
float Delta_y;

void Grayscale_task(void const * argument)
{
	
	while(!ins_init_flag)
		osDelay(1);
	
	while(1)
	{
		Grayscale_num = 0;grayscale_GetData(Grayscale_num);
		Grayscale_num = 1;grayscale_GetData(Grayscale_num);

//		if(chassis_flag == 2 && data_gray[0] == 0xFF)
//		{
//			chassis_stop_flag = 1;
//		}
//		if(chassis_flag == 3 && data_gray[1] == 0xFF)
//		{
//			chassis_stop_flag = 1;
//			chassis_flag = 4;
//		}
//		if(chassis_flag == 6 && data_gray[1] == 0xFF)
//		{
//			chassis_flag = 7;
//			chassis_stop_flag = 1;
//		}
		
		
		//不用
//		if(chassis_flag == 3)
//		{
//			if(data_gray[0] == 0x00)
//				cnt = 1;
//			if(cnt == 1)
//			{
//				if(data_gray[0] == 0xFF)
//				{
//					chassis_flag = 3;
//				}
//			}
//		}
		//下面的灰度加一个灰度巡线,调y轴值
//		if(Data_gray[1][2] == 0 || Data_gray[1][3] == 0 || Data_gray[1][4] == 0 || Data_gray[1][5] == 0)
//			cross_flag = 1;
//		if(cross_flag == 1)
//		{
//			if(data_gray[1] == 0x00)
//			{
//				cross_flag = 0;
//				cross_cnt ++;
//			}
//			if(cross_cnt == 3 && data_gray[1] == 0xFF)
//			{
//				chassis_stop_flag = 1;
//				chassis_flag = 1;
//				cross_cnt ++;
//			}
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
