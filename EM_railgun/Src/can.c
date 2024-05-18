/**
  ******************************************************************************
  * File Name          : CAN.c
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

/* Includes ------------------------------------------------------------------*/
#include "can.h"

/* USER CODE BEGIN 0 */
CAN_HandleTypeDef hcan;
CAN_TxHeaderTypeDef canTxHeader;
CAN_RxHeaderTypeDef canRxHeader;
uint8_t canTxData[8];
uint8_t canRxData[8];
/* USER CODE END 0 */

CAN_HandleTypeDef hcan;

/* CAN init function */
void MX_CAN_Init(void)
{

  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 2;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_9TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_8TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
	/* config can filter1 */
  	CAN_FilterTypeDef sFilterConfig;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;	// Identifier mask mode
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = (DEVICE_STD_ID) << 5;	// id
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0xFC00;	// id mask	// 0xFC00
	sFilterConfig.FilterMaskIdLow = 0x0006;
	sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.FilterBank = 0;
	if(HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}	

	/* config can filter2 */
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;	// Identifier mask mode
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = (DEVICE_STD_BOARDCAST_ID) << 5;	// id
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0xFFE0;	// id mask	// 0xFFE0
	sFilterConfig.FilterMaskIdLow = 0x0006;
	sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.FilterBank = 14;
	if(HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}	
	
  /*##-3- Start the CAN peripheral ###########################################*/
  if (HAL_CAN_Start(&hcan) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }

	__HAL_CAN_ENABLE_IT(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);	// enable FIFO 0 message pending interrupt
//	__HAL_CAN_ENABLE_IT(&hcan, CAN_IT_TX_MAILBOX_EMPTY);	// enable Transmit mailbox empty interrupt
}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN GPIO Configuration    
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */
	CAN_SHDN_DISABLE();	// can shutdown pin
  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();
  
    /**CAN GPIO Configuration    
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
//	/* config can filter1 */
//	CAN_FilterTypeDef sFilterConfig;
//	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;	// Identifier mask mode
//	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
//	sFilterConfig.FilterIdHigh = (DEVICE_STD_ID) << 5;	// id
//	sFilterConfig.FilterIdLow = 0x0000;
//	sFilterConfig.FilterMaskIdHigh = 0xFC00;	// id mask	// 0xFC00
//	sFilterConfig.FilterMaskIdLow = 0x0006;
//	sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
//	sFilterConfig.FilterActivation = ENABLE;
//	sFilterConfig.FilterBank = 0;
//	if(HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
//	{
//		/* Initialization Error */
//		Error_Handler();
//	}	

//	/* config can filter2 */
//	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;	// Identifier mask mode
//	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
//	sFilterConfig.FilterIdHigh = (DEVICE_STD_BOARDCAST_ID) << 5;	// id
//	sFilterConfig.FilterIdLow = 0x0000;
//	sFilterConfig.FilterMaskIdHigh = 0xFFE0;	// id mask	// 0xFFE0
//	sFilterConfig.FilterMaskIdLow = 0x0006;
//	sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
//	sFilterConfig.FilterActivation = ENABLE;
//	sFilterConfig.FilterBank = 14;
//	if(HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
//	{
//		/* Initialization Error */
//		Error_Handler();
//	}	
//	
//  /*##-3- Start the CAN peripheral ###########################################*/
//  if (HAL_CAN_Start(&hcan) != HAL_OK)
//  {
//    /* Start Error */
//    Error_Handler();
//  }

//	__HAL_CAN_ENABLE_IT(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);	// enable FIFO 0 message pending interrupt
////	__HAL_CAN_ENABLE_IT(&hcan, CAN_IT_TX_MAILBOX_EMPTY);	// enable Transmit mailbox empty interrupt
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
