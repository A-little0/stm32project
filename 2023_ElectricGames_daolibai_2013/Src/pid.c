#include "pid.h"


struct PID motorA;//代指编码器电机
struct PID WWD_35D4;//代指电位器
struct PID motorA_angle;//代指编码器电机角度环pid
float d;

void pid_Init(void)
{
	motorA.kp=5;
	motorA.ki=0;
	motorA.kd=0;
	motorA.error=0;
	motorA.last_error=0;
	
	WWD_35D4.kp=137;//24
	WWD_35D4.ki=0;//3.7
	WWD_35D4.kd=11;
	WWD_35D4.error=0;
	WWD_35D4.last_error=0;
	
	motorA_angle.kp=0;
	motorA_angle.ki=0;
	motorA_angle.kd=0;
	motorA_angle.error=0;
	motorA_angle.last_error=0;
	
	d=0;
}

int pid_SpeedControl(int targetspeed,struct PID* motorX,int realspeed)
{
	motorX->error=targetspeed-realspeed;
	
	float P_path=(motorX->error)*motorX->kp;
	static float I_path=0; 
	I_path+=motorX->error*motorX->ki;
	float D_path=(motorX->error-motorX->last_error)*motorX->kd;
	motorX->last_error=motorX->error;
	
	float outputvalue=P_path+I_path+D_path;
	if(outputvalue>2000){outputvalue=2000;}
	else if(outputvalue<-2000){outputvalue=-2000;}
	
	return (int)outputvalue;
}

int pid_AngleOuterloopControl(float targetangle,struct PID* WWD,float realangle)
{
	WWD->error=targetangle-realangle;
	
	float P_path=(WWD->error)*WWD->kp;
	static float I_path=0;
	I_path+=(WWD->error)*WWD->ki;
	float D_path=(WWD->error-WWD->last_error)*WWD->kd;
	d=D_path;
	WWD->last_error=WWD->error;
	
	float angleTospeedOutputvalue=P_path+I_path+D_path;
	return angleTospeedOutputvalue;
}

int pid_AddationControl(int targetangle,struct PID* motorX_Angle,int realangle)
{
	
	motorX_Angle->error=targetangle-realangle;
	
	float P_path=(motorX_Angle->error)*motorX_Angle->kp;
	static float I_path=0;
	I_path+=(motorX_Angle->error)*motorX_Angle->ki;
	float D_path=(motorX_Angle->error-motorX_Angle->last_error)*motorX_Angle->kd;
	motorX_Angle->last_error=motorX_Angle->error;
	
	float addoutput=P_path+I_path+D_path;
	return addoutput;
}

