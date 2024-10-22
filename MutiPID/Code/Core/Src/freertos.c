/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "printf.h"
#include "PID_bsp.h"
#include "can_bsp.h"
#include "RM3508.h"
#include "filter.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern int32_t RM3508_Spd[8];
float filter_out[2];
float aim_pos=0;
PID fuzzy_result;

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
/* Definitions for motorTask */
osThreadId_t motorTaskHandle;
const osThreadAttr_t motorTask_attributes = {
  .name = "motorTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for PID_calulate */
osThreadId_t PID_calulateHandle;
const osThreadAttr_t PID_calulate_attributes = {
  .name = "PID_calulate",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for filter_printf */
osThreadId_t filter_printfHandle;
const osThreadAttr_t filter_printf_attributes = {
  .name = "filter_printf",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartmotorTask(void *argument);
void StartPID_calulate(void *argument);
void Startprintf(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

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
  /* creation of motorTask */
  motorTaskHandle = osThreadNew(StartmotorTask, NULL, &motorTask_attributes);

  /* creation of PID_calulate */
  PID_calulateHandle = osThreadNew(StartPID_calulate, NULL, &PID_calulate_attributes);

  /* creation of filter_printf */
  filter_printfHandle = osThreadNew(Startprintf, NULL, &filter_printf_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartmotorTask */
/**
  * @brief  Function implementing the motorTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartmotorTask */
void StartmotorTask(void *argument)
{
  /* USER CODE BEGIN StartmotorTask */
  /* Infinite loop */
  for(;;)
  {
   RM3508_Set_I(fuzzy_result.PID_Out, 1);
   osDelay(10);
  }
  /* USER CODE END StartmotorTask */
}

/* USER CODE BEGIN Header_StartPID_calulate */
/**
* @brief Function implementing the PID_calulate thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartPID_calulate */
void StartPID_calulate(void *argument)
{
  /* USER CODE BEGIN StartPID_calulate */
    
  /* Infinite loop */
  for(;;)
  {
  pidcalulate(&pos_pid,RM3508_Pos[0],aim_pos*8192.f*19.f/360);
  fuzzy_result=fuzzy_judge(fuzzy_spd_pid,pos_pid);
  pidcalulate(&fuzzy_result,RM3508_Spd[0],pos_pid.PID_Out);
    osDelay(10);
  }
  /* USER CODE END StartPID_calulate */
}

/* USER CODE BEGIN Header_Startprintf */
/**
* @brief Function implementing the printf thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Startprintf */
void Startprintf(void *argument)
{
  /* USER CODE BEGIN Startprintf */
  /* Infinite loop */
  for(;;)
  {
    filter_out[0]=filter0FnManual((float)RM3508_Pos[0]/8192.f/19.f*360);
    filter_out[1]=filter1FnManual((float)pos_pid.PID_Out/100.f);
     
    printf("%f,%f,%f,%d\n",filter_out[0], filter_out[1],aim_pos,state_of_result);
    osDelay(1);
  }
  /* USER CODE END Startprintf */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

