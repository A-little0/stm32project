#include "motor.h"
#include "tim.h"

int target_speedMA;
int target_speedMB;
int target_speedMC;
int target_speedMD;
int setPwm_MA;
int setPwm_MB;
int setPwm_MC;
int setPwm_MD;
extern int realspeed_MA;
extern int realspeed_MB;
extern int realspeed_MC;
extern int realspeed_MD;
int target_angle;
int watch_outerloopoutput;//目标角速度
int watch_interloopoutput;//目标速度

extern int buffArr[1];
extern int dataArr[8];
extern int _motorSingal;
extern int openmv_setpwm;
extern int basepwm;

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
}

void chassis_run_test(void)
{
	//MOTOR B
	MB_ENA_OUT1_L;
	MB_ENA_OUT2_H;
    __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,2000);
	//MOTOR D
	MD_ENA_OUT1_H;
	MD_ENA_OUT2_L;
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

void chassis_run()
{
	//MOTOR B
	MB_ENA_OUT1_L;
	MB_ENA_OUT2_H;
	target_speedMB=50;
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
	target_speedMD=50;
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
	target_speedMA=50;
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
	target_speedMC=50;
	setPwm_MC=pid_SpeedControl_MC(target_speedMC,&motorC,realspeed_MC);
	if(setPwm_MC<0)
	{
		setPwm_MC=-setPwm_MC;
	}
	if(setPwm_MC>2000){setPwm_MC=2000;}
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,setPwm_MC);
}
void chassis_turn_test()
{
	//MOTOR B
	MB_ENA_OUT1_L;
	MB_ENA_OUT2_H;
	target_speedMB=100;
	setPwm_MB=pid_SpeedControl_MB(target_speedMB,&motorB,realspeed_MB);
	if(setPwm_MB<0){
	setPwm_MB=-setPwm_MB;
		MB_ENA_OUT1_H;
		MB_ENA_OUT2_L;
	}
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,setPwm_MB);
//	//MOTOR D
//	MD_ENA_OUT1_H;
//	MD_ENA_OUT2_L;
//	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,2000);
//	//MOTOR A
//	MA_ENA_OUT1_L;
//	MA_ENA_OUT2_H;
//	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,2000);
//	//MOTOR C
//	MC_ENA_OUT1_L;
//	MC_ENA_OUT2_H;
//	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,2000);
}
void chassis_turn()
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
	MD_ENA_OUT1_H;
	MD_ENA_OUT2_L;
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
	MD_ENA_OUT1_H;
	MD_ENA_OUT2_L;
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