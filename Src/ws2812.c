#include "ws2812.h"
#include "tim.h"

void PWM_DMA_code0(void)
{
	uint32_t send_buff[1]={32};//18
	HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_1,(uint32_t*)send_buff,1);
}
void PWM_DMA_code1(void)
{
	uint32_t send_buff[1]={64};//53
	HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_1,(uint32_t*)send_buff,1);
}
void PWM_DMA_set_rgb(uint8_t r,uint8_t g,uint8_t b)
{

	for(uint8_t i=1;i<=8;i++)
	{
		if((g>>(8-i))&1)
		{
			PWM_DMA_code1();
		}
		else
		{
			PWM_DMA_code0();
		}
	}
	for(uint8_t i=1;i<=8;i++)
	{
		if((r>>(8-i))&1)
		{
			PWM_DMA_code1();
		}
		else
		{
			PWM_DMA_code0();
		}
	}
	for(uint8_t i=1;i<=8;i++)
	{
		if((b>>(8-i))&1)
		{
			PWM_DMA_code1();
		}
		else
		{
			PWM_DMA_code0();
		}
	}
}




uint8_t LED_Data[MAX_LED][4];

void Set_LED (int LEDnum, int Red, int Green, int Blue)
{
	LED_Data[LEDnum][0] = LEDnum;
	LED_Data[LEDnum][1] = Green;
	LED_Data[LEDnum][2] = Red;
	LED_Data[LEDnum][3] = Blue;
}

#define PI 3.14159265

//void Set_Brightness (int brightness)  // 0-45
//{
//#if USE_BRIGHTNESS

//	if (brightness > 45) brightness = 45;
//	for (int i=0; i<MAX_LED; i++)
//	{
//		LED_Mod[i][0] = LED_Data[i][0];
//		for (int j=1; j<4; j++)
//		{
//			float angle = 90-brightness;  // in degrees
//			angle = angle*PI / 180;  // in rad
//			LED_Mod[i][j] = (LED_Data[i][j])/(tan(angle));
//		}
//	}

//#endif

//}


int datasentflag=0;
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM4)
	{
		HAL_TIM_PWM_Stop_DMA(&htim4, TIM_CHANNEL_1);
	    datasentflag = 1;
	}
}

uint32_t pwmData[24];
void WS2812_Send (uint32_t color)
{
	for (int i=23; i>=0; i--)
	{
		if (color&(1<<i))
		{
			pwmData[i] = 64;
		}
		else pwmData[i] = 32;
	}

	HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_1, (uint32_t *)pwmData, 24);
	while (!datasentflag){};
	datasentflag = 0;
}


uint32_t pwmData2[(24*MAX_LED)+50];
void WS2812_Send2 (void)
{
	uint32_t indx=0;
	uint32_t color;


	for (int i= 0; i<MAX_LED; i++)
	{
		color = ((LED_Data[i][1]<<16) | (LED_Data[i][2]<<8) | (LED_Data[i][3]));
		for (int i=23; i>=0; i--)
		{
			if (color&(1<<i))
			{
				pwmData[indx] = 60;  // 2/3 of 90
			}

			else pwmData[indx] = 30;  // 1/3 of 90

			indx++;
		}

	}

	for (int i=0; i<50; i++)
	{
		pwmData2[indx] = 0;
		indx++;
	}

	HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_1, (uint32_t *)pwmData2, indx);
	while (!datasentflag){};
	datasentflag = 0;
}
void WS2812_RESET(void)
{
	for(int i=0;i<40;i++)
	{
		WS2812_Send(0x000000);
	}
}
