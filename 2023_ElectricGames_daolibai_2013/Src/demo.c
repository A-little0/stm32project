#include "demo.h"

int timerSP;
int straightangle;//电位器机械中值
int angle;//实时电位器角度
int motorangle;//实时电机角度
int target_motorangle;//电机目标角度
int Watch_angleControl;//监测电位器角度环输出
int Watch_speedControl;//监测电位器速度环输出
int Watch_ADDangleControl;//监测电机位置环输出
int motorOutputPWM;//输出到电机上的pwm值
extern uint32_t ADC_value;
extern float ADC_volt;
extern int realspeed_MA;

void DemoInit()
{
	timerSP=0;
	straightangle=0;//电位器机械中值要设置为零
	angle=0;//实时电位器角度
	Watch_angleControl=0;
	Watch_speedControl=0;
	
	target_motorangle=100;//电机目标角度
	motorangle=0;//实时电机角度
}

void MotoRunTest()
{
	EAH;
	EBL;//逆时针
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,1000);
}

void StraightPrepare()
{
	timerSP++;
	if(timerSP<=5)
	{
		 EAH;
	     EBL;
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,1500);
	}
	else if(timerSP<=10&&timerSP>5)
	{
		 EAL;
	     EBH;
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,1500);
		timerSP=0;
	}
}
void StraightRun(void)
{
	//Watch_ADDangleControl=pid_AddationControl(target_motorangle,&motorA_angle,motorangle);//位置环
	//straightangle=Watch_ADDangleControl;
	Watch_angleControl=pid_AngleOuterloopControl(straightangle,&WWD_35D4,ADC_volt-2.5);
	Watch_speedControl=pid_SpeedControl(Watch_angleControl,&motorA,realspeed_MA);
	//target_motorangle=Watch_angleControl;
//    if(realspeed_MA>=1083)
//	{
//		TIM3->CNT=0;
//		motorangle=0;
//	}
//	else if(realspeed_MA<0)
//	{
//		motorangle=1083+realspeed_MA;
//		if(realspeed_MA<-1083)
//		{
//			TIM3->CNT=0;
//		}
//	}
//	else
//	{
//		motorangle=realspeed_MA;
//	}

	motorOutputPWM=Watch_angleControl+Watch_speedControl;
	
	if(motorOutputPWM>2000){motorOutputPWM=2000;}
	else if(motorOutputPWM<-2000){motorOutputPWM=-2000;}
	if(motorOutputPWM>0){
		 EAH;
	     EBL;
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,motorOutputPWM);
	}
	else{
		 EAL;
	     EBH;
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,-motorOutputPWM);
	}
	
}
