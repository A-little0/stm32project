/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "key.h"
#include "MG4005V2.h"
int timer=1;
int key0=0;
int key1=0;
int key2=0;
int keyup=0;

uint16_t encoderValue = 0;


static uint8_t motorId = 1;
static uint32_t ctlValue = 100*10;
static uint32_t txMailBox;

static int32_t angleControl = 30000;

int buffArr[1]={0};
int dataArr[7]={0};

//int32_t target_angle=0;
//int     target_pwm=150;
//int _motorSingal=0;
//int real_distance=200;

int32_t target_angle_x=0;
int     target_angle_y=0;
int _motorSingal=0;
int real_distance=200;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
struct FILE{
	int handle;
};
int fputc(int ch,struct FILE *f)
{
	HAL_UART_Transmit(&huart1,(uint8_t*)&ch,1,0xffff);
	return ch;
}
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
//			key1=1;
//			key_state[key_mandowm]=1;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if(htim->Instance==TIM3)
	{
		timer++;
		//key_mandown
		if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)==0){
			while(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)==0)
			{
				key1=1;
				key_state[key_mandowm]=1;
			}
		}
		else if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)==1){
			key1=0;
		}
		//key_rangeup
		if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)==0){
			while(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)==0)
			{
				key0=1;
				key_state[key_rangeup]=1;
			}
		}
		else if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)==1){
			key0=0;
		}
		//key_rangedowm
		if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)==0){
			while(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)==0)
			{
				key2=1;
				key_state[key_rangedowm]=1;
			}
		}
		else if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)==1){
			key2=0;
		}
		//key_manup
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1){
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1)
			{
			    keyup=1;
				key_state[key_manup]=1;
			}
		}
		else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==0){
			keyup=0;
		}
		
		Key_Start();	
		OLED_ShowNum(220,1,(uint16_t)target_angle_y,16,16);
		OLED_ShowString(1,1,(uint8_t*)"angley:",16);	
		OLED_ShowNum(220,4,(uint16_t)target_angle_x/100,16,16);
		OLED_ShowString(1,4,(uint8_t*)"anglex:",16);
		
	}
	
	if(htim->Instance==TIM4)
	{
		if(_motorSingal==1)
		{
//			__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,target_angle_y);	
//		    AngleCloseControlLoop6(1,target_angle_x,1500);
//			_motorSingal=0;
		}

		
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle) 
{  

	if(UartHandle->Instance==USART1)
	{
		static int state=0;
		if(buffArr[0]==0x2c||state==1)
		{
			state=1;
			static int length=0;
			dataArr[length++]=buffArr[0];
			if(length==7)
			{
				length=0;
				state=0;
				_motorSingal=1;
				
				
				int tempx=dataArr[3]*40;
				//dataArr[3]=0;
				int tempy=dataArr[5]/40;
				//dataArr[5]=0;
				if(dataArr[2]==0)
				{
					tempx=-tempx;
				}
				if(dataArr[4]==1)
				{
					tempy=-tempy;
				}
				static int temp=150;
				temp=temp+tempy;
				
				target_angle_y=temp;
				target_angle_x=tempx;
				if(target_angle_y<=70){target_angle_y=70;}//上限
				else if(target_angle_y>=170){target_angle_y=170;}//下限
				if(dataArr[6]==0){temp=150;target_angle_y=150;}
			}
		}
		HAL_UART_Receive_IT(&huart1,(uint8_t*)buffArr,1);
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_CAN_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_Base_Start_IT(&htim4);
  OLED_Init();
  OLED_Display_On();
  Key_Init();
  OLED_ClearMYself();
  HAL_UART_Receive_IT(&huart1,(uint8_t*)buffArr,1);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
  __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,150);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//	canTxHeader.StdId = DEVICE_STD_ID + motorId;
//	canTxHeader.ExtId = 0x00;
//	canTxHeader.RTR = CAN_RTR_DATA;
//	canTxHeader.IDE = CAN_ID_STD;
//	canTxHeader.DLC = 8;
//	
//	canTxData[0] = 0xA7;
//	canTxData[1] = 0x00;
//	canTxData[2] = 0x00;
//	canTxData[3] = 0x00;
//	canTxData[4] = *((uint8_t *)&angleControl + 0);
//	canTxData[5] = *((uint8_t *)&angleControl + 1);
//	canTxData[6] = *((uint8_t *)&angleControl + 2);
//	canTxData[7] = *((uint8_t *)&angleControl + 3);
//	
//	/* send can command */
//	HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);
	//AngleCloseControlLoop5(1,36000);
	//AngleCloseControlLoop1(1,8900);
	//SpeedCloseControlLoop(motorId,1000);
	//AngleCloseControlLoop6(1,-90000,2000);
//	AngleCloseControlLoop6(1,359999,2000);
////	HAL_Delay(1000);
//	__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,130);
//	HAL_Delay(1000-1);
	__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,150);
	HAL_Delay(1000-1);
//		__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,170);
//	HAL_Delay(1000-1);

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  /* Get RX message */
  if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &canRxHeader, canRxData) != HAL_OK)
  {
    /* Reception Error */
    Error_Handler();
  }


  if ((canRxHeader.StdId == (DEVICE_STD_ID + motorId)) && (canRxHeader.DLC == 8))
  {
		/* get encoder value */
		encoderValue = (canRxData[7] << 8) + canRxData[6];
		
		//LED1_TOGGLE();
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
