#ifndef __HC_SR04_H
#define __HC_SR04_H
#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "delay.h"
/****************************************************************************
					            ��������

@#define Trig HAL_GPIO_Pin_13		//HC-SR04ģ���Trig�Ž�GPIOB13
Trig:		//����GPIO��Ϊ�������
			//����GPIO��13
			//����GPIO���ٶ�50Mhz
@#define Echo HAL_GPIO_Pin_0		//HC-SR04ģ���Echo�Ž�GPIOG0
Echo    	//����GPIO��Ϊ��������ģʽ
			//����GPIO��6
			//��ʼ��GPIOB
			//����͵�ƽ
****************************************************************************/

#define TrigLEFT GPIO_PIN_14	//HC-SR04ģ���Trig�Ž�GPIOB14
#define EchoLEFT GPIO_PIN_15	//HC-SR04ģ���Echo�Ž�GPIOB15
#define TrigRIGHT GPIO_PIN_12	//HC-SR04ģ���Trig�Ž�GPIOB12
#define EchoRIGHT GPIO_PIN_13	//HC-SR04ģ���Echo�Ž�GPIOB13
typedef struct{
	uint64_t time;			//����������������ʱ
	uint64_t time_end;		//�����������洢�ز��ź�ʱ��
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
