/**
  ******************************************************************************
  * File Name          : CAN.h
  * Description        : This file provides code for the configuration
  *                      of the CAN instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
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
#ifndef __can_H
#define __can_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#define DEVICE_STD_ID						(0x140)//
#define DEVICE_STD_BOARDCAST_ID	(0x280)//
	 
#define CAN_SHDN_Pin					GPIO_PIN_7	// shutdown
#define CAN_SHDN_GPIO_Port		GPIOB
#define CAN_SHDN_ENABLE()			HAL_GPIO_WritePin(CAN_SHDN_GPIO_Port, CAN_SHDN_Pin, GPIO_PIN_SET)
#define CAN_SHDN_DISABLE()		HAL_GPIO_WritePin(CAN_SHDN_GPIO_Port, CAN_SHDN_Pin, GPIO_PIN_RESET)
/* USER CODE END Includes */

extern CAN_HandleTypeDef hcan;

/* USER CODE BEGIN Private defines */
extern CAN_HandleTypeDef hcan;
extern CAN_TxHeaderTypeDef canTxHeader;
extern CAN_RxHeaderTypeDef canRxHeader;
extern uint8_t canTxData[];
extern uint8_t canRxData[];
/* USER CODE END Private defines */

void MX_CAN_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ can_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
