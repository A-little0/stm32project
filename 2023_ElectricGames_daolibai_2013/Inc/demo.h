#ifndef __DEMO_H
#define __DEMO_H
#include "tim.h"
#include "pid.h"

#define EAH HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET)
#define EAL HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET)
#define EBH HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET)
#define EBL HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET)


extern int timerSP;
extern int motorangle;//实时电机角度
extern int target_motorangle;//电机目标角度
extern int straightangle;//电位器机械中值
extern int Watch_speedControl;//监测电位器速度环输出
extern int motorOutputPWM;//输出到电机上的pwm值

void DemoInit(void);
void MotoRunTest(void);
void StraightPrepare(void);//先暂时不考虑启摆
void StraightRun(void);

#endif
