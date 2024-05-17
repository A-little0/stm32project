#include "demo.h"
#include "main.h"
#include "tim.h"
#include "pid.h"

float x;
float y;//ֱ������y��Ϊ��׼
float z;//ת����z��Ϊ��׼
float a;//ֱ�����Ǽ��ٶ���y��Ϊ��׼
int realspeedMleft;//�����ٶ�
int realspeedMright;//�ҵ���ٶ�

int Watch_angle_output;
int Watch_anglespeed_output;

int setPwm_Mleft;
int setPwm_Mright;

int target_test;
void Demo_Init()
{
	x=0;
	y=0;//ֱ������y��Ϊ��׼
	z=0;//ת����z��Ϊ��׼
	realspeedMleft=0;//�����ٶ�
	realspeedMright=0;//�ҵ���ٶ�
	
	target_test=100;
}

void Motor_test()
{	
	//�ҵ��
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,1);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,0);
	//����	
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
		//�ҵ��
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