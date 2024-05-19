#ifndef __DEMO_H
#define __DEMO_H
#include "tim.h"
#include "pid.h"

#define EAH HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET)
#define EAL HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET)
#define EBH HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET)
#define EBL HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET)


extern int timerSP;
extern int motorangle;//ʵʱ����Ƕ�
extern int target_motorangle;//���Ŀ��Ƕ�
extern int straightangle;//��λ����е��ֵ
extern int Watch_speedControl;//����λ���ٶȻ����
extern int motorOutputPWM;//���������ϵ�pwmֵ

void DemoInit(void);
void MotoRunTest(void);
void StraightPrepare(void);//����ʱ����������
void StraightRun(void);

#endif
