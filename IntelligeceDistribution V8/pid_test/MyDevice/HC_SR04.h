#ifndef __HC_SR04_H
#define __HC_SR04_H
#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "delay.h"
/****************************************************************************
					            引脚配置

@#define Trig HAL_GPIO_Pin_13		//HC-SR04模块的Trig脚接GPIOB13
Trig:		//设置GPIO口为推挽输出
			//设置GPIO口13
			//设置GPIO口速度50Mhz
@#define Echo HAL_GPIO_Pin_0		//HC-SR04模块的Echo脚接GPIOG0
Echo    	//设置GPIO口为下拉输入模式
			//设置GPIO口6
			//初始化GPIOB
			//输出低电平
****************************************************************************/

#define TrigLEFT GPIO_PIN_14	//HC-SR04模块的Trig脚接GPIOB14
#define EchoLEFT GPIO_PIN_15	//HC-SR04模块的Echo脚接GPIOB15
#define TrigRIGHT GPIO_PIN_12	//HC-SR04模块的Trig脚接GPIOB12
#define EchoRIGHT GPIO_PIN_13	//HC-SR04模块的Echo脚接GPIOB13
typedef struct{
	uint64_t time;			//声明变量，用来计时
	uint64_t time_end;		//声明变量，存储回波信号时间
	int16_t HC_distance_mm;
	int16_t HC_distance_m;
}HC_SR04;
extern HC_SR04 hc_sr04Left;
extern HC_SR04 hc_sr04Right;
extern int16_t distanceleft;
extern int16_t distanceright;
void HC_SR04_Init(void);
void HC_SR04UPdate(HC_SR04* device);	
int16_t sonar_mmLEFT(void);
float sonarLEFT(void);
int16_t sonar_mmRIGHT(void);
float sonarRIGHT(void);
#endif
