#include "door.h"
#include "tim.h"
#include "gpio.h"

int LeftDoorTIMAnalysis;
int LeftDoorKEY;
int LeftDoorStatus;
int RightDoorTIMAnalysis;
int RightDoorKEY;
int RightDoorStatus;

void Door_Init()
{
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
	
	LeftDoorTIMAnalysis=0;
	RightDoorTIMAnalysis=0;
	LeftDoorKEY=0;
	RightDoorKEY=0;
	LeftDoorStatus=0;
	RightDoorStatus=0;
	DoorControl(1,1);
}

//void LeftDoor(int ONorOFF)
//{
//	if(ONorOFF==1)//door open
//	{
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,120);
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,120);
//	}
//	else //door off
//	{
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,50);
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,50);
//	}
//}

//void RightDoor(int ONorOFF)
//{
//	if(ONorOFF==1)//door open
//	{
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,50);
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,50);
//	}
//	else //door off
//	{
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,120);
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,120);
//	}
//}
void LeftDoor(int ONorOFF)
{
	if(ONorOFF==0)//door off
	{
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,110);
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,110);
	}
	else //door on
	{
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,155);//140
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,155);
	}
}

void RightDoor(int ONorOFF)
{
	if(ONorOFF==0)//door open
	{
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,120);//120
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,120);
	}
	else //door off
	{
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,75);//90
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,75);
	}
}
void DoorControl(int leftdoorstatus,int rightdoorstatus)
{
	LeftDoor(leftdoorstatus);
	RightDoor(rightdoorstatus);
}

int GetorPutLeftDoorObject(int getorput)
{
	if(getorput==PUT)
	{
		if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_10)==1){
			LeftDoorStatus=2;//无有物体
		}
		else
		{
			LeftDoorStatus=1;
			//开启定时器开关
			LeftDoorKEY=1;
		}
		

		if(LeftDoorTIMAnalysis>=CLOSEWAITTIME)
		{
			LeftDoor(0);
			//LeftDoorTIMAnalysis=0;
			//关闭定时器开关
			LeftDoorKEY=0;
			return 1;//结束
		}
	}
	else if(getorput==GET)
	{
		if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_10)==0){
			LeftDoorStatus=1;//有物体
		}
		else
		{
			LeftDoorStatus=2;
			//开启定时器开关
			LeftDoorKEY=1;
		}
		

		if(LeftDoorTIMAnalysis>=CLOSEWAITTIME)
		{
			LeftDoor(0);
			//LeftDoorTIMAnalysis=0;
			//关闭定时器开关
			LeftDoorKEY=0;
			return 1;//结束
		}
	}
	
	return 0;//未结束
}

int GetorPutRightDoorObject(int getorput)
{
	if(getorput==PUT)
	{
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==1)
		{
			RightDoorStatus=2;

		}
		else{
			//开启定时器开关
			RightDoorStatus=1;
			RightDoorKEY=1;
		}

		if(RightDoorTIMAnalysis>=CLOSEWAITTIME)
		{
			RightDoor(0);//rightdoor close
			//RightDoorTIMAnalysis=0;
			//关闭定时器开关
			RightDoorKEY=0;
			return 1;//结束
		}
	}
	else if(getorput==GET)
	{
	    if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==0)
		{
			RightDoorStatus=1;

		}
		else{
			//开启定时器开关
			RightDoorStatus=2;
			RightDoorKEY=1;
		}
		if(RightDoorTIMAnalysis>=CLOSEWAITTIME)
		{
			RightDoor(0);//rightdoor close
			//RightDoorTIMAnalysis=0;
			//关闭定时器开关
			RightDoorKEY=0;
			return 1;//结束
		}
	
	}
	
	return 0;//未结束
}

void DoorKey_Detection()
{
	if(LeftDoorKEY==1){LeftDoorTIMAnalysis++;}
	if(RightDoorKEY==1){RightDoorTIMAnalysis++;}
}