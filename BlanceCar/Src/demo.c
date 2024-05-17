#include "demo.h"
#include "main.h"
#include "tim.h"
#include "pid.h"

float x;
float y;//直立环以y轴为基准
float z;//转向环以z轴为基准
float a;//直立环角加速度以y轴为基准
int realspeedMleft;//左电机速度
int realspeedMright;//右电机速度

int Watch_angle_output;
int Watch_anglespeed_output;

int setPwm_Mleft;
int setPwm_Mright;

int target_test;
void Demo_Init()
{
	x=0;
	y=0;//直立环以y轴为基准
	z=0;//转向环以z轴为基准
	realspeedMleft=0;//左电机速度
	realspeedMright=0;//右电机速度
	
	target_test=100;
}

void Motor_test()
{	
	//右电机
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,1);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,0);
	//左电机	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,0);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,1);
		
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,500);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,500);
}

void CarStraight()
{
	Watch_anglespeed_output=PDStraightAngleControl(0,&newcar,&JY901_data);
//	Watch_angle_output=pid_StraightAngleOuterloopControl(0,&carBody,y);
//	Watch_anglespeed_output=pid_StraightAngleInnerloopControl(Watch_angle_output,&carBody,a);
	
	setPwm_Mleft=pid_SpeedControl_MLeft(Watch_anglespeed_output,&motorLeft,realspeedMleft);
	setPwm_Mright=pid_SpeedControl_MRight(Watch_anglespeed_output,&motorRight,realspeedMright);
	
	if(setPwm_Mleft>0)
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,0);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,1);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,setPwm_Mleft);
	}else
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,1);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,0);
		setPwm_Mleft=-setPwm_Mleft;
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,setPwm_Mleft);
	}
	
	if(setPwm_Mright>0)
	{
		//右电机
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,1);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,0);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,setPwm_Mright);

	}else
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,0);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,1);
		setPwm_Mright=-setPwm_Mright;
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,setPwm_Mright);
	}
	
}