#include "openmv_task.h"
#include "configs.h"
#include "usart.h"

#define RXBUFFER_SIZE 3
uint8_t rxBuffer_main[2],rxBuffer_color[RXBUFFER_SIZE],rxBuffer_shape[RXBUFFER_SIZE];
uint8_t flag_openmv = 0,flag_openmv_ball = 0;
uint8_t rx_index = 0;
uint16_t color = 0, shape = 0;
uint16_t count_openmv = 0;


//mode的值是0，表示停止，不识别；
//mode的值是1，表示同时识别颜色和形状；
//mode的值是2，表示识别小球颜色；
//mode的值是3，表示识别二维码；

//shape的值是0x31，表示圆环；
//shape的值是0x32，表示正方形；
//shape的值是0x33，表示矩形；

//color的值是0x31，表示红色； 49
//color的值是0x32，表示蓝色； 50
//color的值是0x33，表示黄色； 51
//color的值是0x34，表示白色； 52

void openmv_task(void const * argument)
{
		
	while(1)
	{
		HAL_UART_Transmit(&huart1, &mode_openmv, 1, 100);
		osDelay(1);
	}
}

//寻找数组中出现次数最多的数字
uint8_t findMostFrequent(uint8_t *array, uint8_t size1) 
{
    uint8_t maxValue = array[0];
    uint8_t minValue = array[1];
    uint8_t counts[size1];
    uint8_t maxCount = 0;
    uint8_t mostFrequentNumber;

    // 找出数组中的最大值和最小值
    for (uint8_t i = 0; i < size1; i++) 
	{
        if (array[i] > maxValue) 
					{
            maxValue = array[i];
        }
        if (array[i] < minValue) 
					{
            minValue = array[i];
        }
    }

    // 记录每个数字的出现次数
    for (uint8_t i = 0; i < size1; i++) 
		{
        counts[array[i] - minValue]++;
    }

    // 找出出现次数最多的数字
    for (uint8_t i = 0; i < maxValue - minValue + 1; i++) 
		{
        if (counts[i] > maxCount) 
					{
            maxCount = counts[i];
            mostFrequentNumber = i + minValue;
        }
    }

    return mostFrequentNumber;
}



//HAL库回调函数。
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
				if( mode_openmv == 1)
				{
						if(count_openmv != RXBUFFER_SIZE && flag_openmv == 0)
						{
								rxBuffer_color[count_openmv] = rxBuffer_main[0]; 
								rxBuffer_shape[count_openmv] = rxBuffer_main[1]; 
								count_openmv++;
						}
						else if(count_openmv == RXBUFFER_SIZE)
						{
								color = findMostFrequent(rxBuffer_color, RXBUFFER_SIZE) - 48;
								shape = findMostFrequent(rxBuffer_shape, RXBUFFER_SIZE) - 48;
								count_openmv = 0;
								flag_openmv = 1;
						}
						
				}
				if(mode_openmv == 3)
				{
				
				}
				if(mode_openmv == 2)
				{
							color = rxBuffer_main[0] - 48;
				}
        // 重新启动中断接收，准备接收下一批数据
        HAL_UART_Receive_IT(&huart1, rxBuffer_main, sizeof(rxBuffer_main));
    }
}

