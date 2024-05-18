/**
  ******************************************************************************
  * @file    chassis.c
  * @author  chenjiangnan
  * @brief   麦克拉姆轮底盘的控制方案
  *
  @verbatim
  ==============================================================================
  
                     ##### How to use this driver #####
  ==============================================================================
  @endverbatim
  ******************************************************************************
  * @attention
  *	1.需要包含的头文件：chassis.h(给外部文件提供变量，函数声明以及相关宏定义)
  *					   motor.h(电机相关变量)
  *       			   tim.h(提供hal库定时器操作有关的库函数)
  *					   encode.h(获取电机的脉冲)
  *					   openmv.h（控制信号）
  *	2.本文件适用于麦轮底盘或者全向轮底盘
  ******************************************************************************
*/
#include "chassis.h"
#include "motor.h"
#include "tim.h"
#include "openmv.h"
#include "encode.h"

int XVector;
int YVector;
int RotVector;
int RotateK;
float TheBeginAngle;
int32_t encode_average;

/**
  * @function		void Chassis_task_Init(void)
  * @brief          全局变量初始化		
  * @brief          获取初始底盘角度值	  
  * @param[in]      none
  * @retval         none
  */
void Chassis_task_Init(void)
{
	XVector=0;
	YVector=0;
	RotVector=0;
	RotateK=1;
	encode_average=0;
	TheBeginAngle=JY901_data.angle.angle[2];
}

/**
  * @function		void Chassis_Status_Analysis(int leftfirstorrightfirst)
  * @brief          控制小车先左在右，或者先右在左			  
  * @param[in]      int leftfirstorrightfirst -> 输入1先到左边收货点，输入2先到右边收货点
  * @retval         none
  */
void Chassis_Status_Analysis(int leftfirstorrightfirst)
{
	static int key=0;
	if(chassis_status==1){key=1;}
	if(key==1)
	{
		if(leftfirstorrightfirst==1)
		{			
			//前进13200个编码器值
			static int status_first=0;
			if(status_first==0&&encode_average<=13000)
			{encode_average=(encodeMA+encodeMB+encodeMC+encodeMD)/4;chassis_status=1;}
			else if(encode_average>13000 && chassis_status==1){
				chassis_status=2;
				if(status_first==0){Encode_Init();}
				status_first=1;
				encode_average=0;
			}
			//向左平移6600个编码器值
			static int status_second=0;
			if(encode_average<=6600&&status_first==1&&status_second==0)
			{
			encode_average=(-encodeMA+encodeMB+encodeMC-encodeMD)/4;
			chassis_status=2;
			}
			else if(status_first==1 && encode_average>6600 && chassis_status==2){
				chassis_status=3;
				if(status_second==0){Encode_Init();}
				status_second=1;
				encode_average=0;
			}
			//向右平移13200个编码器
			static int status_third=0;
			if(status_second==1&&status_third==0&&encode_average<=13200)
			{encode_average=(encodeMA-encodeMB-encodeMC+encodeMD)/4;chassis_status=3;}
			else if(status_second==1&&status_first==1&&status_third==0&&encode_average>13200){
					chassis_status=5;
					if(status_third==0){Encode_Init();}
					status_third=1;
					encode_average=0;
				}
			
		}
		else if(leftfirstorrightfirst==2)
		{
		
		}
	}
}
void Chassis_Status_AnalysisTEST(int leftfirstorrightfirst)
{
	static int key=0;
	if(chassis_status==1){key=1;}
	if(key==1)
	{
		if(leftfirstorrightfirst==1)
		{			
			//前进13200个编码器值
			static int status_first=0;
			if(status_first==0&&encode_average<=13000)
			{encode_average=(encodeMA+encodeMB+encodeMC+encodeMD)/4;chassis_status=1;}
			else if(encode_average>13000 && chassis_status==1){
				chassis_status=2;
				if(status_first==0){Encode_Init();}
				status_first=1;
				encode_average=0;
			}
			//向左平移6600个编码器值
			static int status_second=0;
			if(encode_average<=6600&&status_first==1&&status_second==0)
			{
			encode_average=(-encodeMA+encodeMB+encodeMC-encodeMD)/4;
			chassis_status=2;
			}
			else if(status_first==1 && encode_average>6600 && chassis_status==2){
				chassis_status=3;
				if(status_second==0){Encode_Init();}
				status_second=1;
				encode_average=0;
			}
			//向右平移13200个编码器
			static int status_third=0;
			if(status_second==1&&status_third==0&&encode_average<=13200)
			{encode_average=(encodeMA-encodeMB-encodeMC+encodeMD)/4;chassis_status=3;}
			else if(status_second==1&&status_first==1&&status_third==0&&encode_average>13200){
					chassis_status=5;
					if(status_third==0){Encode_Init();}
					status_third=1;
					encode_average=0;
				}
			
		}
		else if(leftfirstorrightfirst==2)
		{
		
		}
	}
}


/**
  * @function		void chassis_task(int x_vector,int y_vector,int rotate_vector,int rotateK)
  * @brief         	底盘运动学解算			  
  * @param[in]      int x_vector x方向速度的大小(相对于底盘坐标系)
  * @param[in]		int y_vector y方向速度的大小(相对于底盘坐标系)
  * @param[in]      int rotate_vector 底盘旋转角速度(相对于底盘坐标系)
  * @param[in]		int rotateK 底盘轮距长+宽
  * @retval         none
  * @attention		底盘中心为原点指向车头的方向为x的正方向，以底盘中心为原点向右垂直于x轴的方向为y的正反向
					/\ x
					||
					||
					||
					  ————> y
  * @attention		底盘逆时针旋转为正方向，逆时针旋转角度增加
  */
void chassis_task(int x_vector,int y_vector,int rotate_vector,int rotateK)
{
	float realangle=JY901_data.angle.angle[2]-TheBeginAngle;
	static float lastangle=0;
	if(lastangle>178.5 && realangle<0){realangle+=360;}
	else if(lastangle<-178.899 && realangle>0){realangle+=-360;}
    int outerloopOutput=pid_TurnAngleOuterloopControl(target_angle,&carBody,realangle);
	int innerloopOutput=pid_TurnAngleInnerloopControl(outerloopOutput,&carBody,JY901_data.acc.a[2]);
	watch_outerloopoutput=outerloopOutput;
	watch_interloopoutput=innerloopOutput;
	rotate_vector=watch_interloopoutput;
	lastangle=realangle;
	
	target_speedMA=x_vector-y_vector-rotate_vector*rotateK;
	target_speedMC=x_vector+y_vector-rotate_vector*rotateK;
	target_speedMB=x_vector+y_vector+rotate_vector*rotateK;
	target_speedMD=x_vector-y_vector+rotate_vector*rotateK;
	
	//MOTOR B
	setPwm_MB=pid_SpeedControl_MB(target_speedMB,&motorB,realspeed_MB);
	if(setPwm_MB>=0)
	{
		MB_ENA_OUT1_L;
		MB_ENA_OUT2_H;
		if(setPwm_MB>2000){setPwm_MB=2000;}
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,setPwm_MB);
	}
	else if(setPwm_MB<0){
		MB_ENA_OUT1_H;
		MB_ENA_OUT2_L;
		if(-setPwm_MB>2000){setPwm_MB=-2000;}
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,-setPwm_MB);
	}
	
	//MOTOR D
	setPwm_MD=pid_SpeedControl_MD(target_speedMD,&motorD,realspeed_MD);
	if(setPwm_MD>=0)
	{
		MD_ENA_OUT1_L;
		MD_ENA_OUT2_H;
		if(setPwm_MD>2000){setPwm_MD=2000;}
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,setPwm_MD);
	}
	if(setPwm_MD<0){
		MD_ENA_OUT1_H;
		MD_ENA_OUT2_L;
		if(-setPwm_MD>2000){setPwm_MD=-2000;}
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,-setPwm_MD);
	}
	
    //MOTOR A
	setPwm_MA=pid_SpeedControl_MA(target_speedMA,&motorA,realspeed_MA);
	if(setPwm_MA>=0)
	{
		MA_ENA_OUT1_H;
		MA_ENA_OUT2_L;
		if(setPwm_MA>2000){setPwm_MA = 2000;}
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,setPwm_MA);
	}
	else if(setPwm_MA<0)
	{
		MA_ENA_OUT1_L;
		MA_ENA_OUT2_H;
		if(-setPwm_MA>2000){setPwm_MA = -2000;}
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,-setPwm_MA);
	}

    //MOTOR C
	setPwm_MC=pid_SpeedControl_MC(target_speedMC,&motorC,realspeed_MC);
	if(setPwm_MC>=0)
	{
		MC_ENA_OUT1_H;
		MC_ENA_OUT2_L;
		if(setPwm_MC>=2000){setPwm_MC = 2000;}
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,setPwm_MC);
	}
	if(setPwm_MC<0)
	{
		MC_ENA_OUT1_L;
		MC_ENA_OUT2_H;
		if(-setPwm_MC>=2000){setPwm_MC = -2000;}
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,-setPwm_MC);
	}
}