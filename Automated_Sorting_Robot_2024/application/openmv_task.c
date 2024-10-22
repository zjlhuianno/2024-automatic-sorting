#include "openmv_task.h"
#include "configs.h"
#include "usart.h"

#define RXBUFFER_SIZE 3
uint8_t rxBuffer_main[2],rxBuffer_color[RXBUFFER_SIZE],rxBuffer_shape[RXBUFFER_SIZE];
uint8_t flag_openmv = 0,flag_openmv_ball = 0;
uint8_t rx_index = 0;
uint16_t color = 0, shape = 0;
uint16_t count_openmv = 0;


//mode��ֵ��0����ʾֹͣ����ʶ��
//mode��ֵ��1����ʾͬʱʶ����ɫ����״��
//mode��ֵ��2����ʾʶ��С����ɫ��
//mode��ֵ��3����ʾʶ���ά�룻

//shape��ֵ��0x31����ʾԲ����
//shape��ֵ��0x32����ʾ�����Σ�
//shape��ֵ��0x33����ʾ���Σ�

//color��ֵ��0x31����ʾ��ɫ�� 49
//color��ֵ��0x32����ʾ��ɫ�� 50
//color��ֵ��0x33����ʾ��ɫ�� 51
//color��ֵ��0x34����ʾ��ɫ�� 52

void openmv_task(void const * argument)
{
		
	while(1)
	{
		HAL_UART_Transmit(&huart1, &mode_openmv, 1, 100);
		osDelay(1);
	}
}

//Ѱ�������г��ִ�����������
uint8_t findMostFrequent(uint8_t *array, uint8_t size1) 
{
    uint8_t maxValue = array[0];
    uint8_t minValue = array[1];
    uint8_t counts[size1];
    uint8_t maxCount = 0;
    uint8_t mostFrequentNumber;

    // �ҳ������е����ֵ����Сֵ
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

    // ��¼ÿ�����ֵĳ��ִ���
    for (uint8_t i = 0; i < size1; i++) 
		{
        counts[array[i] - minValue]++;
    }

    // �ҳ����ִ�����������
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



//HAL��ص�������
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
        // ���������жϽ��գ�׼��������һ������
        HAL_UART_Receive_IT(&huart1, rxBuffer_main, sizeof(rxBuffer_main));
    }
}

