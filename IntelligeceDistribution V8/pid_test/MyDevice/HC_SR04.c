#include "HC_SR04.h"

HC_SR04 hc_sr04Left;
HC_SR04 hc_sr04Right;
int16_t distanceleft;
int16_t distanceright;

void HC_SR04_Init()
{

	HAL_GPIO_WritePin(GPIOB,TrigLEFT,GPIO_PIN_RESET);		//����͵�ƽ
	PY_usDelayTest();                                       //΢�뼶����1��ʼ��
	PY_usDelayOptimize();									//΢�뼶����2��ʼ��
	//Delay_us(15);											//��ʱ15΢��
	hc_sr04Left.time=0;
	hc_sr04Left.time_end=0;
	hc_sr04Left.HC_distance_m=0;
	hc_sr04Left.HC_distance_mm=0;
	
	HAL_GPIO_WritePin(GPIOB,TrigRIGHT,GPIO_PIN_RESET);		//����͵�ƽ
	//Delay_us(15);											//��ʱ15΢��
	hc_sr04Right.time=0;
	hc_sr04Right.time_end=0;
	hc_sr04Right.HC_distance_m=0;
	hc_sr04Right.HC_distance_mm=0;
	
	distanceleft=0;
	distanceright=0;
	
}
void HC_SR04UPdate(HC_SR04* device)							//���ڸ����жϺ����У�������ʱ��ÿ10΢�����time��1
{
	device->time++;
}
int16_t sonar_mmLEFT(void)									    //��ಢ���ص�λΪ���׵ľ�����
{
	uint32_t Distance,Distance_mm = 0;
	HAL_GPIO_WritePin(GPIOB,TrigLEFT,GPIO_PIN_SET);			//����ߵ�ƽ
	PY_Delay_us(15);										//��ʱ15΢��
	HAL_GPIO_WritePin(GPIOB,TrigLEFT,GPIO_PIN_RESET);		//����͵�ƽ
	while(HAL_GPIO_ReadPin(GPIOB,EchoLEFT)==0);				//�ȴ��͵�ƽ����
	hc_sr04Left.time=0;										//��ʱ����
	while(HAL_GPIO_ReadPin(GPIOB,EchoLEFT)==1);			//�ȴ��ߵ�ƽ����
	hc_sr04Left.time_end=hc_sr04Left.time;				//��¼����ʱ��ʱ��
	if(hc_sr04Left.time_end/100<38)						//�ж��Ƿ�С��38���룬����38����ľ��ǳ�ʱ��ֱ�ӵ������淵��0
	{
		Distance=(hc_sr04Left.time_end*346)/2;			//������룬25��C�����е�����Ϊ346m/s
		Distance_mm=Distance/100;						//��Ϊ�����time_end�ĵ�λ��10΢�룬����Ҫ�ó���λΪ���׵ľ����������ó���100
	}
	
	hc_sr04Left.HC_distance_mm=Distance_mm;
	return Distance_mm;									//���ز����
}

float sonarLEFT(void)										//��ಢ���ص�λΪ�׵ľ�����
{
	uint32_t Distance,Distance_mm = 0;
	float Distance_m=0;
	HAL_GPIO_WritePin(GPIOB,TrigLEFT,GPIO_PIN_SET);		//����ߵ�ƽ
	PY_Delay_us(15);
	HAL_GPIO_WritePin(GPIOB,TrigLEFT,GPIO_PIN_RESET);	//����͵�ƽ
	while(HAL_GPIO_ReadPin(GPIOB,EchoLEFT)==0);	
	hc_sr04Left.time=0;
	while(HAL_GPIO_ReadPin(GPIOB,EchoLEFT)==1);
	hc_sr04Left.time_end=hc_sr04Left.time;
	if(hc_sr04Left.time_end/100<38)
	{
		Distance=(hc_sr04Left.time_end*346)/2;
		Distance_mm=Distance/100;
		Distance_m=Distance_mm/1000;
	}
	hc_sr04Left.HC_distance_m=Distance_m;
	return Distance_m;
}


int16_t sonar_mmRIGHT(void)									    //��ಢ���ص�λΪ���׵ľ�����
{
	uint32_t Distance,Distance_mm = 0;
	HAL_GPIO_WritePin(GPIOB,TrigRIGHT,GPIO_PIN_SET);			//����ߵ�ƽ
	PY_Delay_us(15);										//��ʱ15΢��
	HAL_GPIO_WritePin(GPIOB,TrigRIGHT,GPIO_PIN_RESET);		//����͵�ƽ
	while(HAL_GPIO_ReadPin(GPIOB,EchoRIGHT)==0);				//�ȴ��͵�ƽ����
	hc_sr04Right.time=0;										//��ʱ����
	while(HAL_GPIO_ReadPin(GPIOB,EchoRIGHT)==1);			//�ȴ��ߵ�ƽ����
	hc_sr04Right.time_end=hc_sr04Right.time;				//��¼����ʱ��ʱ��
	if(hc_sr04Right.time_end/100<38)						//�ж��Ƿ�С��38���룬����38����ľ��ǳ�ʱ��ֱ�ӵ������淵��0
	{
		Distance=(hc_sr04Right.time_end*346)/2;			//������룬25��C�����е�����Ϊ346m/s
		Distance_mm=Distance/100;						//��Ϊ�����time_end�ĵ�λ��10΢�룬����Ҫ�ó���λΪ���׵ľ����������ó���100
	}
	
	hc_sr04Right.HC_distance_mm=Distance_mm;
	return Distance_mm;									//���ز����
}

float sonarRIGHT(void)										//��ಢ���ص�λΪ�׵ľ�����
{
	uint32_t Distance,Distance_mm = 0;
	float Distance_m=0;
	HAL_GPIO_WritePin(GPIOB,TrigRIGHT,GPIO_PIN_SET);		//����ߵ�ƽ
	PY_Delay_us(15);
	HAL_GPIO_WritePin(GPIOB,TrigRIGHT,GPIO_PIN_RESET);	//����͵�ƽ
	while(HAL_GPIO_ReadPin(GPIOB,EchoRIGHT)==0);	
	hc_sr04Right.time=0;
	while(HAL_GPIO_ReadPin(GPIOB,EchoRIGHT)==1);
	hc_sr04Right.time_end=hc_sr04Right.time;
	if(hc_sr04Right.time_end/100<38)
	{
		Distance=(hc_sr04Right.time_end*346)/2;
		Distance_mm=Distance/100;
		Distance_m=Distance_mm/1000;
	}
	hc_sr04Right.HC_distance_m=Distance_m;
	return Distance_m;
}





