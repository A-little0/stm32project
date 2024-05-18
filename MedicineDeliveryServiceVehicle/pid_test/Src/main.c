/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "pid.h"
#include "motor.h"
#include "jy901.h"
#include "openmv.h"
#include "delay.h"
int realspeed_MA;
int realspeed_MB;
int realspeed_MC;
int realspeed_MD;
unsigned int pwm_MA;

struct FILE
{
	int handle;
};

int fputc(int ch,FILE *f)
{
	 HAL_UART_Transmit(&huart1,(uint8_t*)&ch,1,0xffff);
	return ch;
}

float x=0;
float y=0;
float z=0;
float a=0;

int basepwm=30;
int _StraightSingal=2;
int _TurnSingal=2;
int testusart2[1];
int testusart3[1];
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
 int Read_MA_Encoder(uint8_t TIMX)
{
	int Encoder_MA_TIM;
	switch(TIMX)
	{
		case 1:
			Encoder_MA_TIM=(short)TIM1->CNT;
			TIM1->CNT=0;
			break;
		case 4:
			Encoder_MA_TIM=(short)TIM4->CNT;
			TIM4->CNT=0;
			break;
		case 5:
			Encoder_MA_TIM=(short)TIM5->CNT;
			TIM5->CNT=0;
			break;
		case 8:
			Encoder_MA_TIM=(short)TIM8->CNT;
			TIM8->CNT=0;                                                                 
			break;
		default:
			Encoder_MA_TIM=0;
		
	
	}
	return Encoder_MA_TIM;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	realspeed_MA=Read_MA_Encoder(1);
	realspeed_MB=Read_MA_Encoder(4);
	realspeed_MC=Read_MA_Encoder(5);
	realspeed_MD=Read_MA_Encoder(8);
	
	//_TurnSingal=LineOpenMV_TurnSingal();
//	if(_TurnSingal==1){_StraightSingal=2;}
	if(_StraightSingal==1&&_TurnSingal==2)
	{
		openmv_setpwm=LineOpenMV_GetPWM();
		if(LineOpenMV_StopSingal()==1)
		{
			basepwm=0;
			openmv_setpwm=0;
		}
		else
		{
			basepwm=30;
		}
	ChassisLine_Openmv(basepwm,openmv_setpwm);
	}
	else if(_TurnSingal==1 )//&& _StraightSingal==2
	{
		//_StraightSingal=2;
		if(openmv_turnway>0)
		{
			TurnChassis_Tim(30);
		}
		else if(openmv_turnway<0)
		{
			TurnChassis_Tim(-30);
		}
//		delay_ms(1000);
//		delay_ms(1000);
		//MYDelay_ms(380);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,0);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);
		//MYDelay_ms(380);
		_TurnSingal=2;
	    //_StraightSingal=1;
		//MYDelay_ms(380);
	}
	else
	{
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,0);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);
	}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle) 
{  

	if(UartHandle->Instance==USART3)
	{
		NumOpenMV_Recieve();

	}
    if(UartHandle->Instance==USART1)
	{
		LineOpenMV_Recieve();
	}
	if(UartHandle->Instance==USART2)
	{
//		JY901_data.Rx_len=33;
//		JY901_Process(&JY901_data);
//		HAL_UART_Receive_IT(&huart3,JY901_data.RxBuffer,RXBUFFER_LEN);
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
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM8_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
HAL_TIM_Encoder_Start(&htim1,TIM_CHANNEL_ALL);
HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_ALL);
HAL_TIM_Encoder_Start(&htim5,TIM_CHANNEL_ALL);
HAL_TIM_Encoder_Start(&htim8,TIM_CHANNEL_ALL);
HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);


//开启定时器中断服务函数
HAL_TIM_Base_Start_IT(&htim3);

//初始化pid
pid_Init();
motor_Init();
User_USART_Init(&JY901_data);
HAL_UART_Receive_IT(&huart2,JY901_data.RxBuffer,RXBUFFER_LEN);
MB_ENA_OUT1_H;
MB_ENA_OUT2_H;
__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);
LineOpenMV_Init();
NumOpenMV_Init();
PY_usDelayTest();
PY_usDelayOptimize();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

//	  OutData[1]=2E(&htim2,TIM_CHANNEL_1,500);
//	  OutData[0]=1000;000;
//	  OutData[2]=3000;
//	  OutData[3]=4000;
	  //OutPut_Data();
	  x=JY901_data.angle.angle[0];
	  y=JY901_data.angle.angle[1];
	  z=JY901_data.angle.angle[2];
	  a=JY901_data.acc.a[2];
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
