/**
  ******************************************************************************
  * @file    motor.c
  * @author  chenjiangnan
  * @brief   电机的驱动和一些控制模式.
  *
  @verbatim
  ==============================================================================
  
                     ##### How to use this driver #####
  ==============================================================================
  @endverbatim
  ******************************************************************************
  * @attention
  *	1.需要包含的头文件：motor.h(给外部文件提供变量，函数声明以及相关宏定义)
  *					   tim.h(提供hal库定时器操作有关的库函数
  *	2.本文件适用于驱动tb6612或者l298n之类的电机驱动模块(带正反转以及pwm驱动模式)
  ******************************************************************************
  */
#include "motor.h"
#include "tim.h"

int target_speedMA;//motor A的目标速度
int target_speedMB;//motor B的目标速度
int target_speedMC;//motor C的目标速度
int target_speedMD;//motor D的目标速度
int setPwm_MA;//作用于motor A的pwm值
int setPwm_MB;//作用于motor B的pwm值
int setPwm_MC;//作用于motor C的pwm值
int setPwm_MD;//作用于motor D的pwm值
int realspeed_MA;//motor A的实际转速(单位时间捕获的脉冲)
int realspeed_MB;//motor B的实际转速(单位时间捕获的脉冲)
int realspeed_MC;//motor C的实际转速(单位时间捕获的脉冲)
int realspeed_MD;//motor D的实际转速(单位时间捕获的脉冲)
int target_angle;//底盘的目标角度（相对于世界坐标系）
int watch_outerloopoutput;//检测外环输出值
int watch_interloopoutput;//检测内环输出值

extern int buffArr[1];
extern int dataArr[8];
extern int _motorSingal;
extern int openmv_setpwm;
extern int basepwm;

/**
  * @function		void motor_Init(void)
  * @brief          电机驱动的初始化函数
  *					初始化该文件的全局变量
  * @param[in]      none
  * @retval         none
  */
void motor_Init(void)
{
	target_speedMA=0;
	target_speedMB=0;
	target_speedMC=0;
	target_speedMD=0; 
	
	setPwm_MA=0;
	setPwm_MB=0;
	setPwm_MC=0;
	setPwm_MD=0;
	
	target_angle=0;
	
	watch_outerloopoutput=0;
	watch_interloopoutput=0;
}

/**
  * @function		void chassis_run_test(void)
  * @brief          底盘pwm纯开环测试函数				
  *					-->判断电机正反转的引脚电平(宏定义)
  *					-->电机满转
  * @param[in]      none
  * @retval         none
  */
void chassis_run_test(void)
{
	//MOTOR B
	MB_ENA_OUT1_L;
	MB_ENA_OUT2_H;
    __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,2000);
	//MOTOR D
	MD_ENA_OUT1_L;
	MD_ENA_OUT2_H;
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,2000);
	//MOTOR A
	MA_ENA_OUT1_H;
	MA_ENA_OUT2_L;
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,2000);
	//MOTOR C
	MC_ENA_OUT1_H;
	MC_ENA_OUT2_L;
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,2000);
}

/**
  * @function		void chassis_run(void)
  * @brief          底盘速度闭环测试函数				
  * @param[in]      none
  * @retval         none
  */
void chassis_run(void)
{
	//MOTOR B
	MB_ENA_OUT1_L;
	MB_ENA_OUT2_H;
	target_speedMB=30;
	setPwm_MB=pid_SpeedControl_MB(target_speedMB,&motorB,realspeed_MB);
	if(setPwm_MB<0){//正反转判断
		setPwm_MB=-setPwm_MB;
		MB_ENA_OUT1_H;
		MB_ENA_OUT2_L;
	}
	if(setPwm_MB>2000){setPwm_MB=2000;}//限幅
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,setPwm_MB);
	
	//MOTOR D
	MD_ENA_OUT1_L;
	MD_ENA_OUT2_H;
	target_speedMD=30;
	setPwm_MD=pid_SpeedControl_MD(target_speedMD,&motorD,realspeed_MD);
	if(setPwm_MD<0){//正反转判断
		setPwm_MD=-setPwm_MD;
		MD_ENA_OUT1_L;
		MD_ENA_OUT2_H;
	}
	if(setPwm_MD>2000){setPwm_MD=2000;}
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,setPwm_MD);
	
    //MOTOR A
	MA_ENA_OUT1_H;
	MA_ENA_OUT2_L;
	target_speedMA=30;
	setPwm_MA=pid_SpeedControl_MA(target_speedMA,&motorA,realspeed_MA);
	if(setPwm_MA<0){//正反转判断
		setPwm_MA=-setPwm_MA;
		MA_ENA_OUT1_L;
		MA_ENA_OUT2_H;
	}
	if(setPwm_MA>2000){setPwm_MA=2000;}//限幅
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,setPwm_MA);
	
    //MOTOR C
	MC_ENA_OUT1_H;
	MC_ENA_OUT2_L;
	target_speedMC=30;
	setPwm_MC=pid_SpeedControl_MC(target_speedMC,&motorC,realspeed_MC);
	if(setPwm_MC<0){//正反转判断
		MC_ENA_OUT1_L;
		MC_ENA_OUT2_H;
		setPwm_MC=-setPwm_MC;
	}
	if(setPwm_MC>2000){setPwm_MC=2000;}//限幅
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,setPwm_MC);
}

/**
  * @function		void chassis_turn(void)
  * @brief          底盘角度串级闭环测试函数				
  *	@brief 				-->调参
  * @param[in]      none
  * @retval         none
  */
void chassis_turn(void)
{
	int outerloopOutput=pid_TurnAngleOuterloopControl(target_angle,&carBody,JY901_data.angle.angle[2]);
	int innerloopOutput=pid_TurnAngleInnerloopControl(outerloopOutput,&carBody,JY901_data.acc.a[2]);
	watch_outerloopoutput=outerloopOutput;
	watch_interloopoutput=innerloopOutput;
	
	//MOTOR B
	MB_ENA_OUT1_L;
	MB_ENA_OUT2_H;
	target_speedMB=innerloopOutput;
	setPwm_MB=pid_SpeedControl_MB(target_speedMB,&motorB,realspeed_MB);
	if(setPwm_MB<0){
	setPwm_MB=-setPwm_MB;
		MB_ENA_OUT1_H;
		MB_ENA_OUT2_L;
	}
	if(setPwm_MB>2000){setPwm_MB=2000;}
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,setPwm_MB);
	
	//MOTOR D
	MD_ENA_OUT1_L;
	MD_ENA_OUT2_H;
	target_speedMD=innerloopOutput;
	setPwm_MD=pid_SpeedControl_MD(target_speedMD,&motorD,realspeed_MD);
	if(setPwm_MD<0){
		setPwm_MD=-setPwm_MD;
		MD_ENA_OUT1_L;
		MD_ENA_OUT2_H;
	}
	if(setPwm_MD>2000){setPwm_MD=2000;}
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,setPwm_MD);
	
    //MOTOR A
	MA_ENA_OUT1_H;
	MA_ENA_OUT2_L;
	target_speedMA=-innerloopOutput;
	setPwm_MA=pid_SpeedControl_MA(target_speedMA,&motorA,realspeed_MA);
	if(setPwm_MA<0){
		setPwm_MA=-setPwm_MA;
		MA_ENA_OUT1_L;
		MA_ENA_OUT2_H;
	}
	if(setPwm_MA>2000){setPwm_MA=2000;}
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,setPwm_MA);//target_speedMA
	
    //MOTOR C
	MC_ENA_OUT1_H;
	MC_ENA_OUT2_L;
	target_speedMC=-innerloopOutput;
	setPwm_MC=pid_SpeedControl_MC(target_speedMC,&motorC,realspeed_MC);
	if(setPwm_MC<0)
	{
		MC_ENA_OUT1_L;
		MC_ENA_OUT2_H;
		setPwm_MC=-setPwm_MC;
	}
	if(setPwm_MC>=2000){setPwm_MC=2000;}
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,setPwm_MC);
	
}

/**
  * @function		void ChassisLine_Openmv(int base_speed,int add_speed)
  * @brief          循迹模式下的底盘控制方案				
  * @param[in]      int base_speed->理想状态下小车巡线的速度
  * @param[in]      int  add_speed->偏离状态下小车额外补偿的速度
  * @retval         none
  * @attention		同测轮补偿值一致
  */
void ChassisLine_Openmv(int base_speed,int add_speed)
{
    //MOTOR B
	MB_ENA_OUT1_L;
	MB_ENA_OUT2_H;
	target_speedMB=base_speed-add_speed;
	setPwm_MB=pid_SpeedControl_MB(target_speedMB,&motorB,realspeed_MB);
	if(setPwm_MB<0){
	setPwm_MB=-setPwm_MB;
		MB_ENA_OUT1_H;
		MB_ENA_OUT2_L;
	}
	if(setPwm_MB>2000){setPwm_MB=2000;}
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,setPwm_MB);
	
	//MOTOR D
	MD_ENA_OUT1_L;
	MD_ENA_OUT2_H;
	target_speedMD=base_speed-add_speed;
	setPwm_MD=pid_SpeedControl_MD(target_speedMD,&motorD,realspeed_MD);
	if(setPwm_MD<0){
		setPwm_MD=-setPwm_MD;
		MD_ENA_OUT1_L;
		MD_ENA_OUT2_H;
	}
	if(setPwm_MD>2000){setPwm_MD=2000;}
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,setPwm_MD);
	
    //MOTOR A
	MA_ENA_OUT1_H;
	MA_ENA_OUT2_L;
	target_speedMA=base_speed+add_speed;
	setPwm_MA=pid_SpeedControl_MA(target_speedMA,&motorA,realspeed_MA);
	if(setPwm_MA<0){
		setPwm_MA=-setPwm_MA;
		MA_ENA_OUT1_L;
		MA_ENA_OUT2_H;
	}
	if(setPwm_MA>2000){setPwm_MA=2000;}
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,setPwm_MA);//target_speedMA
	
    //MOTOR C
	MC_ENA_OUT1_H;
	MC_ENA_OUT2_L;
	target_speedMC=base_speed+add_speed;
	setPwm_MC=pid_SpeedControl_MC(target_speedMC,&motorC,realspeed_MC);
	if(setPwm_MC<0)
	{
		MC_ENA_OUT1_L;
		MC_ENA_OUT2_H;
		setPwm_MC=-setPwm_MC;
	}
	if(setPwm_MC>=2000){setPwm_MC=2000;}
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,setPwm_MC);
	
}

/**
  * @function		void TurnChassis_Tim(int base_turnSpeed)
  * @brief          定时器控制底盘旋转方案				
  * @param[in]      int base_turnSpeed->小车旋转的速度
  * @retval         none
  * @attention		在定时区域内调用该函数
  */
void TurnChassis_Tim(int base_turnSpeed)
{
	    //MOTOR B
	MB_ENA_OUT1_L;
	MB_ENA_OUT2_H;
	target_speedMB=base_turnSpeed;
	setPwm_MB=pid_SpeedControl_MB(target_speedMB,&motorB,realspeed_MB);
	if(setPwm_MB<0){
	    setPwm_MB=-setPwm_MB;
		MB_ENA_OUT1_H;
		MB_ENA_OUT2_L;
	}
	if(setPwm_MB>2000){setPwm_MB=2000;}
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,setPwm_MB);
	
	//MOTOR D
	MD_ENA_OUT1_H;
	MD_ENA_OUT2_L;
	target_speedMD=base_turnSpeed;
	setPwm_MD=pid_SpeedControl_MD(target_speedMD,&motorD,realspeed_MD);
	if(setPwm_MD<0){
		setPwm_MD=-setPwm_MD;
		MD_ENA_OUT1_L;
		MD_ENA_OUT2_H;
	}
	if(setPwm_MD>2000){setPwm_MD=2000;}
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,setPwm_MD);
	
    //MOTOR A
	MA_ENA_OUT1_H;
	MA_ENA_OUT2_L;
	target_speedMA=-base_turnSpeed;
	setPwm_MA=pid_SpeedControl_MA(target_speedMA,&motorA,realspeed_MA);
	if(setPwm_MA<0){
		setPwm_MA=-setPwm_MA;
		MA_ENA_OUT1_L;
		MA_ENA_OUT2_H;
	}
	if(setPwm_MA>2000){setPwm_MA=2000;}
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,setPwm_MA);//target_speedMA
	
    //MOTOR C
	MC_ENA_OUT1_H;
	MC_ENA_OUT2_L;
	target_speedMC=-base_turnSpeed;
	setPwm_MC=pid_SpeedControl_MC(target_speedMC,&motorC,realspeed_MC);
	if(setPwm_MC<0)
	{
		MC_ENA_OUT1_L;
		MC_ENA_OUT2_H;
		setPwm_MC=-setPwm_MC;
	}
	if(setPwm_MC>=2000){setPwm_MC=2000;}
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,setPwm_MC);
}

/**
  * @function		void Chassis_Move(int base_moveSpeed)
  * @brief          控制底盘运动方案				
  * @param[in]      int base_moveSpeed->小车的速度
  * @retval         none
  * @attention		只有前后左右方向的运动
  */
void Chassis_Move(int base_moveSpeed)
{
	
	//MOTOR B
	MB_ENA_OUT1_L;
	MB_ENA_OUT2_H;
	target_speedMB=-base_moveSpeed;
	setPwm_MB=pid_SpeedControl_MB(target_speedMB,&motorB,realspeed_MB);
	if(setPwm_MB<0){
	setPwm_MB=-setPwm_MB;
		MB_ENA_OUT1_H;
		MB_ENA_OUT2_L;
	}
	if(setPwm_MB>2000){setPwm_MB=2000;}
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,setPwm_MB);
	
	//MOTOR D
	MD_ENA_OUT1_L;
	MD_ENA_OUT2_H;
	target_speedMD=base_moveSpeed;
	setPwm_MD=pid_SpeedControl_MD(target_speedMD,&motorD,realspeed_MD);
	if(setPwm_MD<0){
	setPwm_MD=-setPwm_MD;
	MD_ENA_OUT1_L;
	MD_ENA_OUT2_H;
	}
	if(setPwm_MD>2000){setPwm_MD=2000;}
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,setPwm_MD);
	
    //MOTOR A
	MA_ENA_OUT1_H;
	MA_ENA_OUT2_L;
	target_speedMA=base_moveSpeed;
	setPwm_MA=pid_SpeedControl_MA(target_speedMA,&motorA,realspeed_MA);
	if(setPwm_MA<0){
	setPwm_MA=-setPwm_MA;
	MA_ENA_OUT1_L;
	MA_ENA_OUT2_H;
	}
	if(setPwm_MA>2000){setPwm_MA=2000;}
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,setPwm_MA);
	
    //MOTOR C
	MC_ENA_OUT1_H;
	MC_ENA_OUT2_L;
	target_speedMC=-base_moveSpeed;
	setPwm_MC=pid_SpeedControl_MC(target_speedMC,&motorC,realspeed_MC);
	if(setPwm_MC<0)
	{
		setPwm_MC=-setPwm_MC;
	}
	if(setPwm_MC>2000){setPwm_MC=2000;}
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,setPwm_MC);
}


