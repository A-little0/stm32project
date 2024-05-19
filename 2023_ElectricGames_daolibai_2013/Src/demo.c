#include "demo.h"

int timerSP;
int straightangle;//��λ����е��ֵ
int angle;//ʵʱ��λ���Ƕ�
int motorangle;//ʵʱ����Ƕ�
int target_motorangle;//���Ŀ��Ƕ�
int Watch_angleControl;//����λ���ǶȻ����
int Watch_speedControl;//����λ���ٶȻ����
int Watch_ADDangleControl;//�����λ�û����
int motorOutputPWM;//���������ϵ�pwmֵ
extern uint32_t ADC_value;
extern float ADC_volt;
extern int realspeed_MA;

void DemoInit()
{
	timerSP=0;
	straightangle=0;//��λ����е��ֵҪ����Ϊ��
	angle=0;//ʵʱ��λ���Ƕ�
	Watch_angleControl=0;
	Watch_speedControl=0;
	
	target_motorangle=100;//���Ŀ��Ƕ�
	motorangle=0;//ʵʱ����Ƕ�
}

void MotoRunTest()
{
	EAH;
	EBL;//��ʱ��
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
	//Watch_ADDangleControl=pid_AddationControl(target_motorangle,&motorA_angle,motorangle);//λ�û�
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
