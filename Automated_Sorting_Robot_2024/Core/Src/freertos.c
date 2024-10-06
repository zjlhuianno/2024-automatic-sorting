/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId INS_TASKHandle;
osThreadId CHASSIS_TaskHandle;
osThreadId Grayscale_TaskHandle;
osThreadId main_TaskHandle;
osThreadId arm_taskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void INS_Task(void const * argument);
void Chassis_task(void const * argument);
void Graysacle_task(void const * argument);
void main_task(void const * argument);
void Arm_Task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of INS_TASK */
  osThreadDef(INS_TASK, INS_Task, osPriorityHigh, 0, 512);
  INS_TASKHandle = osThreadCreate(osThread(INS_TASK), NULL);

  /* definition and creation of CHASSIS_Task */
  osThreadDef(CHASSIS_Task, Chassis_task, osPriorityHigh, 0, 512);
  CHASSIS_TaskHandle = osThreadCreate(osThread(CHASSIS_Task), NULL);

  /* definition and creation of Grayscale_Task */
  osThreadDef(Grayscale_Task, Graysacle_task, osPriorityNormal, 0, 128);
  Grayscale_TaskHandle = osThreadCreate(osThread(Grayscale_Task), NULL);

  /* definition and creation of main_Task */
  osThreadDef(main_Task, main_task, osPriorityHigh, 0, 128);
  main_TaskHandle = osThreadCreate(osThread(main_Task), NULL);

  /* definition and creation of arm_task */
  osThreadDef(arm_task, Arm_Task, osPriorityAboveNormal, 0, 512);
  arm_taskHandle = osThreadCreate(osThread(arm_task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_INS_Task */
/**
  * @brief  Function implementing the INS_TASK thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_INS_Task */
__weak void INS_Task(void const * argument)
{
  /* USER CODE BEGIN INS_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END INS_Task */
}

/* USER CODE BEGIN Header_Chassis_task */
/**
* @brief Function implementing the CHASSIS_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Chassis_task */
__weak void Chassis_task(void const * argument)
{
  /* USER CODE BEGIN Chassis_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Chassis_task */
}

/* USER CODE BEGIN Header_Graysacle_task */
/**
* @brief Function implementing the Grayscale_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Graysacle_task */
__weak void Graysacle_task(void const * argument)
{
  /* USER CODE BEGIN Graysacle_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Graysacle_task */
}

/* USER CODE BEGIN Header_main_task */
/**
* @brief Function implementing the main_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_main_task */
__weak void main_task(void const * argument)
{
  /* USER CODE BEGIN main_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END main_task */
}

/* USER CODE BEGIN Header_Arm_Task */
/**
* @brief Function implementing the arm_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Arm_Task */
__weak void Arm_Task(void const * argument)
{
  /* USER CODE BEGIN Arm_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Arm_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
