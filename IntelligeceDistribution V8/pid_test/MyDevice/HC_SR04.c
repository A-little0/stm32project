#include "HC_SR04.h"

HC_SR04 hc_sr04Left;
HC_SR04 hc_sr04Right;
int16_t distanceleft;
int16_t distanceright;

void HC_SR04_Init()
{

	HAL_GPIO_WritePin(GPIOB,TrigLEFT,GPIO_PIN_RESET);		//输出低电平
	PY_usDelayTest();                                       //微秒级函数1初始化
	PY_usDelayOptimize();									//微秒级函数2初始化
	//Delay_us(15);											//延时15微秒
	hc_sr04Left.time=0;
	hc_sr04Left.time_end=0;
	hc_sr04Left.HC_distance_m=0;
	hc_sr04Left.HC_distance_mm=0;
	
	HAL_GPIO_WritePin(GPIOB,TrigRIGHT,GPIO_PIN_RESET);		//输出低电平
	//Delay_us(15);											//延时15微秒
	hc_sr04Right.time=0;
	hc_sr04Right.time_end=0;
	hc_sr04Right.HC_distance_m=0;
	hc_sr04Right.HC_distance_mm=0;
	
	distanceleft=0;
	distanceright=0;
	
}
void HC_SR04UPdate(HC_SR04* device)							//放在更新中断函数中，用来计时，每10微秒变量time加1
{
	device->time++;
}
int16_t sonar_mmLEFT(void)									    //测距并返回单位为毫米的距离结果
{
	uint32_t Distance,Distance_mm = 0;
	HAL_GPIO_WritePin(GPIOB,TrigLEFT,GPIO_PIN_SET);			//输出高电平
	PY_Delay_us(15);										//延时15微秒
	HAL_GPIO_WritePin(GPIOB,TrigLEFT,GPIO_PIN_RESET);		//输出低电平
	while(HAL_GPIO_ReadPin(GPIOB,EchoLEFT)==0);				//等待低电平结束
	hc_sr04Left.time=0;										//计时清零
	while(HAL_GPIO_ReadPin(GPIOB,EchoLEFT)==1);			//等待高电平结束
	hc_sr04Left.time_end=hc_sr04Left.time;				//记录结束时的时间
	if(hc_sr04Left.time_end/100<38)						//判断是否小于38毫秒，大于38毫秒的就是超时，直接调到下面返回0
	{
		Distance=(hc_sr04Left.time_end*346)/2;			//计算距离，25°C空气中的音速为346m/s
		Distance_mm=Distance/100;						//因为上面的time_end的单位是10微秒，所以要得出单位为毫米的距离结果，还得除以100
	}
	
	hc_sr04Left.HC_distance_mm=Distance_mm;
	return Distance_mm;									//返回测距结果
}

float sonarLEFT(void)										//测距并返回单位为米的距离结果
{
	uint32_t Distance,Distance_mm = 0;
	float Distance_m=0;
	HAL_GPIO_WritePin(GPIOB,TrigLEFT,GPIO_PIN_SET);		//输出高电平
	PY_Delay_us(15);
	HAL_GPIO_WritePin(GPIOB,TrigLEFT,GPIO_PIN_RESET);	//输出低电平
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


int16_t sonar_mmRIGHT(void)									    //测距并返回单位为毫米的距离结果
{
	uint32_t Distance,Distance_mm = 0;
	HAL_GPIO_WritePin(GPIOB,TrigRIGHT,GPIO_PIN_SET);			//输出高电平
	PY_Delay_us(15);										//延时15微秒
	HAL_GPIO_WritePin(GPIOB,TrigRIGHT,GPIO_PIN_RESET);		//输出低电平
	while(HAL_GPIO_ReadPin(GPIOB,EchoRIGHT)==0);				//等待低电平结束
	hc_sr04Right.time=0;										//计时清零
	while(HAL_GPIO_ReadPin(GPIOB,EchoRIGHT)==1);			//等待高电平结束
	hc_sr04Right.time_end=hc_sr04Right.time;				//记录结束时的时间
	if(hc_sr04Right.time_end/100<38)						//判断是否小于38毫秒，大于38毫秒的就是超时，直接调到下面返回0
	{
		Distance=(hc_sr04Right.time_end*346)/2;			//计算距离，25°C空气中的音速为346m/s
		Distance_mm=Distance/100;						//因为上面的time_end的单位是10微秒，所以要得出单位为毫米的距离结果，还得除以100
	}
	
	hc_sr04Right.HC_distance_mm=Distance_mm;
	return Distance_mm;									//返回测距结果
}

float sonarRIGHT(void)										//测距并返回单位为米的距离结果
{
	uint32_t Distance,Distance_mm = 0;
	float Distance_m=0;
	HAL_GPIO_WritePin(GPIOB,TrigRIGHT,GPIO_PIN_SET);		//输出高电平
	PY_Delay_us(15);
	HAL_GPIO_WritePin(GPIOB,TrigRIGHT,GPIO_PIN_RESET);	//输出低电平
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





