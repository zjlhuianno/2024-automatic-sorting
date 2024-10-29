#include "main.h"
#include "bsp_can.h"
#include "can_receive.h"
#include "cmsis_os.h"
#include "chassis.h"
#include "VOFA.h"
#include "INS_task.h"
#include "configs.h"
#include "Grayscale_task.h"
#include "lidar.h"
#include "arm_task.h"
#include "openmv_task.h"

void vTimerCallback(TimerHandle_t xTimer);
void chassis_work(void);
void speed_set(float vx_set, float vy_set, float wz_set);
uint8_t location_set(float x_set, float y_set, float yaw_set);
void remote_control(void);
void Chassis_task(void const * argument);
