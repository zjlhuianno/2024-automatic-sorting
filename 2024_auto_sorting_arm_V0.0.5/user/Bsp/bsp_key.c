#include "bsp_key.h"
#include "cmsis_os.h"
#include "main.h"
#include "stm32f4xx_hal.h"

uint8_t Get_KeyNum(void)//��ȡ�����ķ���ֵ��
{
	uint8_t KeyNum = 0;
	
	//��ȡPA0�˿ڵİ���״̬���������£���������1��
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1)//���KEY�����¡�
	{
		osDelay(20);//���º󣬰���������
//		while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1);//��PA1һֱ���ڱ����µ�״̬����һֱ���������ѭ�����棬ֱ�����֡�
//		osDelay(20);//���ֺ󣬰���������
		KeyNum = 1;//�������ֺ��ٿ��ǰ���Ӧ���еĶ�����
	}
	
	return KeyNum;
}
