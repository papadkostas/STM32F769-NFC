/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "demo.h"
#include "platform.h"
#include "st_errno.h"
#include "stm32f7xx.h"
#include "stm32f769i_discovery.h"
#include "lvgl.h"
#include "lv_nfc.h"
#include "sysmon.h"
#include "../../Middlewares/hal_stm_lvgl/tft/tft.h"
#include "../../Middlewares/hal_stm_lvgl/touchpad/touchpad.h"
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
/* Definitions for lv_task */
osThreadId_t lv_taskHandle;
const osThreadAttr_t lv_task_attributes = {
  .name = "lv_task",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 8196 * 4
};
/* Definitions for nfc_task */
osThreadId_t nfc_taskHandle;
const osThreadAttr_t nfc_task_attributes = {
  .name = "nfc_task",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 1024 * 4
};
/* Definitions for lv_mutex */
osMutexId_t lv_mutexHandle;
const osMutexAttr_t lv_mutex_attributes = {
  .name = "lv_mutex"
};
/* Definitions for nfc_mutex */
osMutexId_t nfc_mutexHandle;
const osMutexAttr_t nfc_mutex_attributes = {
  .name = "nfc_mutex"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void lv_task_start(void *argument);
void nfc_task_start(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationIdleHook(void);
void vApplicationTickHook(void);
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);
void vApplicationMallocFailedHook(void);

/* USER CODE BEGIN 2 */
__weak void vApplicationIdleHook( void )
{
   /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
   task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()). If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
}
/* USER CODE END 2 */

/* USER CODE BEGIN 3 */
__weak void vApplicationTickHook( void )
{
	/* This function will be called by each tick interrupt if
	 * configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
	 * added here, but the tick hook is called from an interrupt context, so
	 * code must not attempt to block, and only the interrupt safe FreeRTOS API
	 * functions can be used (those that end in FromISR()). */
}

void HAL_Delay(volatile uint32_t millis)
{
	/* replace HAL library blocking delay function
	 * with FreeRTOS thread aware equivalent */
	vTaskDelay(millis);
}

/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
__weak void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
	  while(1){
		  HAL_Delay(500);
		  BSP_LED_Toggle(LED_RED);
	  }
}
/* USER CODE END 4 */

/* USER CODE BEGIN 5 */
__weak void vApplicationMallocFailedHook(void)
{
   /* vApplicationMallocFailedHook() will only be called if
   configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
   function that will get called if a call to pvPortMalloc() fails.
   pvPortMalloc() is called internally by the kernel whenever a task, queue,
   timer or semaphore is created. It is also called by various parts of the
   demo application. If heap_1.c or heap_2.c are used, then the size of the
   heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   to query the size of free heap space that remains (although it does not
   provide information on how the remaining heap might be fragmented). */
	  while(1){
		  HAL_Delay(1000);
		  BSP_LED_Toggle(LED_RED);
	  }
}
/* USER CODE END 5 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of lv_mutex */
  lv_mutexHandle = osMutexNew(&lv_mutex_attributes);

  /* creation of nfc_mutex */
  nfc_mutexHandle = osMutexNew(&nfc_mutex_attributes);

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
  /* creation of lv_task */
  lv_taskHandle = osThreadNew(lv_task_start, NULL, &lv_task_attributes);

  /* creation of nfc_task */
  nfc_taskHandle = osThreadNew(nfc_task_start, NULL, &nfc_task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_lv_task_start */
/**
* @brief Function implementing the lv_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_lv_task_start */
void lv_task_start(void *argument)
{
  /* USER CODE BEGIN lv_task_start */
  lv_init();
  tft_init();
  touchpad_init();
  nfc_create();

  /* Infinite loop */
  for(;;)
  {
	  lv_task_handler();
	  BSP_LED_Toggle(LED_RED);
	  osDelay(5);
  }
  /* USER CODE END lv_task_start */
}

/* USER CODE BEGIN Header_nfc_task_start */
/**
* @brief Function implementing the nfc_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_nfc_task_start */
void nfc_task_start(void *argument)
{
  /* USER CODE BEGIN nfc_task_start */
  /* Initialize RFAL */
  demoIni();
  /* Infinite loop */
  for(;;)
  {
	  demoCycle();
	  osDelay(10);
	  BSP_LED_Toggle(LED_GREEN);
  }
  /* USER CODE END nfc_task_start */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
