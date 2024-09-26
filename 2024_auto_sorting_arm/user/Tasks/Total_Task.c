#include "Includes.h"

void Total_Task(void const * argument)
{
	while(1)
	{
		target_x = 10.0f;
		target_y = 15.5f;
		target_angle = 0.0f;
		osDelay(1000);
		target_x = 22.0f;
		target_y = 20.5f;
		target_angle = 90.0f;
		
		
		osDelay(1);
	}
}
