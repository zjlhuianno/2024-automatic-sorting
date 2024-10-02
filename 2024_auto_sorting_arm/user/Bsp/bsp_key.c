#include "bsp_key.h"
#include "cmsis_os.h"
#include "main.h"
#include "stm32f4xx_hal.h"

uint8_t Get_KeyNum(void)//读取按键的返回值。
{
	uint8_t KeyNum = 0;
	
	//读取PA0端口的按键状态，若被按下，则函数返回1。
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1)//如果KEY被按下。
	{
		osDelay(20);//按下后，按键消抖。
//		while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1);//若PA1一直处于被按下的状态，则一直困在这个死循环里面，直到松手。
//		osDelay(20);//松手后，按键消抖。
		KeyNum = 1;//按键松手后，再考虑按键应该有的动作。
	}
	
	return KeyNum;
}
