#include "Includes.h"


void Start_Task(void const * argument)
{
	vTaskSuspend(arm_taskHandle);
	vTaskSuspend(total_taskHandle);
	Servo_Init();
	can_filter_init();
	DM_Data_Init();
	DM_PID_Init();
	led_init();
	
  vTaskDelay(1000);
	
	do
	{
		Enable_DM_Motor(&hcan2,DM4340_ID);
		Enable_DM_Motor(&hcan2,DM4310_ID);
		osDelay(1);
	} while(DM4340_Data.state == 0 || DM4310_Data.state == 0);
		
//	Save_Pos_Zero(&hcan2,DM4340_ID);
//	vTaskDelay(10);
//	Save_Pos_Zero(&hcan2,DM4310_ID);
//	vTaskDelay(10);
	
	
	vTaskDelay(20);
	vTaskResume(arm_taskHandle);
	
	vTaskDelay(20);
	vTaskResume(total_taskHandle);
	
	vTaskDelete(start_taskHandle);
	
}
