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
#include "i2c.h"
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
#include "oled.h"
#include "u8g2.h"
#include "stm32_u8g2.h"
#include "encode.h"
#include "chassis.h"
#include "door.h"
#include "HC_SR04.h"
#include "obstacle_avoidance.h"
#include "syn6288.h"

struct FILE
{
	int handle;
};

int fputc(int ch,FILE *f)
{
	HAL_UART_Transmit(&huart1,(uint8_t*)&ch,1,0xffff);
	return ch;
}


u8g2_t u8g2;
int tim7dieorlive=0;
int pwmtest=0;

int my_test_ok=0;
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
  
void testDrawProcess(u8g2_t *u8g2)
{
	for(int i=10;i<=80;i=i+2)
	{
		u8g2_ClearBuffer(u8g2); 
			
		char buff[20];
		sprintf(buff,"%d%%",(int)(i/80.0*100));
		
		u8g2_SetFont(u8g2,u8g2_font_ncenB12_tf);
		u8g2_DrawStr(u8g2,16,32,"STM32 U8g2");//字符显示
		
		u8g2_SetFont(u8g2,u8g2_font_ncenB08_tf);
		u8g2_DrawStr(u8g2,100,49,buff);//当前进度显示
		
		u8g2_DrawRBox(u8g2,16,40,i,10,4);//圆角填充框矩形框
		u8g2_DrawRFrame(u8g2,16,40,80,10,4);//圆角矩形
		
		u8g2_SendBuffer(u8g2);
	}
	HAL_Delay(500);
}

void DrawStatus(u8g2_t *u8g2)
{
	u8g2_ClearBuffer(u8g2); 
	u8g2_SetFont(u8g2,u8g2_font_ncenB12_tf);
	u8g2_DrawStr(u8g2,16,32,"STM32 U8g2");//字符显示
	
	float realangle=JY901_data.angle.angle[2]-TheBeginAngle;
	static float lastangle=0;
	if(lastangle>179.9999 && realangle<0){realangle+=360;}
	else if(lastangle<-179.899 && realangle>0){realangle+=-360;}
	char buff[4]={0};
	if(realangle>0)
	{
		buff[0]='0'+(int)realangle/100;
		buff[1]='0'+((int)(realangle)%100)/10;
		buff[2]='0'+(int)(realangle)%10;
		u8g2_SetFont(u8g2,u8g2_font_ncenB12_tf);
		u8g2_DrawStr(u8g2,16,50,"+");
		u8g2_SetFont(u8g2,u8g2_font_ncenB12_tf);
		u8g2_DrawStr(u8g2,32,50,buff);
	}else
	{
		buff[0]='0'+(int)-(realangle)/100;
		buff[1]='0'+((int)-(realangle)%100)/10;
		buff[2]='0'+(int)-(realangle)%10;
		u8g2_SetFont(u8g2,u8g2_font_ncenB12_tf);
		u8g2_DrawStr(u8g2,16,50,"-");
		u8g2_SetFont(u8g2,u8g2_font_ncenB12_tf);
		u8g2_DrawStr(u8g2,32,50,buff);
	}
	u8g2_SendBuffer(u8g2);
	lastangle=realangle;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM6)
	{
		realspeed_MA=Read_MX_Encoder(1);
		realspeed_MB=Read_MX_Encoder(4);
		realspeed_MC=Read_MX_Encoder(5);
		realspeed_MD=Read_MX_Encoder(8);
//		Chassis_Status_Analysis(1);
//		chassis_run_test();
		DoorKey_Detection();
//		XVector=0;
//		YVector=0;
//		chassis_task(XVector,YVector,RotVector,RotateK);
//		if(GetorPutRightDoorObject(PUT)==1){RightDoor(0);}
//		if(GetorPutLeftDoorObject(PUT)==1) {RightDoor(0);}
		Demo(); 
	}
//	if(htim->Instance==TIM7)
//	{
//		HC_SR04UPdate(&hc_sr04Left);
//		HC_SR04UPdate(&hc_sr04Right);
//		//HAL_Delay(100);
//		tim7dieorlive++;
//	}					

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle) 
{  

	if(UartHandle->Instance==USART1)
	{
		//NumOpenMV_Recieve();

	}
    if(UartHandle->Instance==USART3)
	{
 		OpenMV_Recieve();
	}
	if(UartHandle->Instance==USART2)
	{
		JY901_data.Rx_len=33;
		JY901_Recieve(&JY901_data);
		JY901_Process(&JY901_data);
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
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM6_Init();
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
  HAL_TIM_Base_Start_IT(&htim6);
  //HAL_TIM_Base_Start_IT(&htim7);
  pid_Init();
  motor_Init();
  Encode_Init();
  OpenMV_Init();
  Door_Init();
  JY901_Init();
  User_USART_Init(&JY901_data);
  JY901_Recieve(&JY901_data);
  HC_SR04_Init();
  OpenMV_Init();
  Chassis_task_Init();
  u8g2Init(&u8g2);  		 //显示器调用初始化函数
  u8g2_ClearDisplay(&u8g2);  //清除屏幕缓冲区
//  syn6288_Init();
//  SYN_FrameInfo(2, (uint8_t*)"[v16][m1][t5] 请装入物料，并关闭仓门");
HAL_UART_Transmit_IT(&huart1,VoiceBox_Began,sizeof(VoiceBox_Began));

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	//distanceleft=sonar_mmLEFT();
	//distanceright=sonar_mmRIGHT();
	//testDrawProcess(&u8g2);
	DrawStatus(&u8g2);
	//chassis_run_test();
//	static int myreset=0;
//    if((JY901_data.angle.angle[0]+JY901_data.angle.angle[1]+JY901_data.angle.angle[2])==0&&myreset==0)
//	{
//	    __set_FAULTMASK(1);
//		HAL_NVIC_SystemReset();
//	     myreset=1;
//	}
//	if(Sound_broadcasting_signal==1)
//	{
//		SYN_FrameInfo(2, (uint8_t*)"[v16][m1][t5] 到达1号收货点，请取出货物并关闭仓门");
//		Sound_broadcasting_signal=0;
//	}
//	else if(Sound_broadcasting_signal==2)
//	{
//		SYN_FrameInfo(2, (uint8_t*)"[v16][m1][t5] 到达2号收货点，请取出货物并关闭仓门");
//		Sound_broadcasting_signal=0;
//	}
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
