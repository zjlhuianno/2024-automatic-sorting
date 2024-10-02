#include "VOFA.h"

int fputc(int ch,FILE *f)
{
	HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,HAL_MAX_DELAY);
	return ch;
}
/*int fgetc(FILE *f)
{
	uint8_t ch;
	HAL_UART_Receive(&huart1,(uint8_t *)&ch,1,HAL_MAX_DELAY);
	return ch;
}*/

void VOFA(int num_args, ...)
{
    va_list args;
    va_start(args, num_args);
    uint8_t tail[4] = {0x00, 0x00, 0x80, 0x7F};
    uint8_t Data[4];
	
    for (int i=0; i<num_args;i++) 
	{
        float val =(float)va_arg(args,double);  
        *(float*)Data = val;					
        HAL_UART_Transmit(&huart1,Data,4,0xffff);
    }
    HAL_UART_Transmit(&huart1, tail, 4, 0xffff);
    va_end(args);
}

void VOFA_justfloat(float A)
{
	uint8_t tail[4] = {0x00, 0x00, 0x80, 0x7F};
    uint8_t Data1[4];
	*(float*)Data1=A;

	HAL_UART_Transmit(&huart1,Data1,4,0xffff);
	HAL_UART_Transmit(&huart1,tail,4,0xffff);
}
