/**
  ******************************************************************************
  * File Name          : SPI.h
  * Description        : This file provides code for the configuration
  *                      of the SPI instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __spi_H
#define __spi_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "platform.h"

/* USER CODE END Includes */

extern SPI_HandleTypeDef hspi2;

/* USER CODE BEGIN Private defines */
#define SpiSelect()     HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET)
#define SpiDeselect()   HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET)

/* USER CODE END Private defines */

void MX_SPI2_Init(void);

/* USER CODE BEGIN Prototypes */
void SpiInit(SPI_HandleTypeDef *hspi);
uint8_t SpiTxRx(const uint8_t *txData, uint8_t *rxData, uint8_t length);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ spi_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
