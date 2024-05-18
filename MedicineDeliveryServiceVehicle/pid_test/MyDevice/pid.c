#include "pid.h"


struct PID motorA;
struct PID motorB;
struct PID motorC;
struct PID motorD;
struct TURNPID carBody;

void pid_Init(void)
{
	motorA.kp=100;
	motorA.ki=5;
	motorA.kd=0;
	motorA.error=0;
	motorA.last_error=0;
	
	motorB.kp=100;
	motorB.ki=5;
	motorB.kd=0;
	motorB.error=0;
	motorB.last_error=0;
	
	motorC.kp=100;
	motorC.ki=5;
	motorC.kd=0;
	motorC.error=0;
	motorC.last_error=0;
	
	motorD.kp=100;
	motorD.ki=5;
	motorD.kd=0;
	motorD.error=0;
	motorD.last_error=0;
	
	
	carBody.angle_to_anglespeed.kp=2;
	carBody.angle_to_anglespeed.ki=0;
	carBody.angle_to_anglespeed.kd=8;
	carBody.angle_to_anglespeed.error=0;
	carBody.angle_to_anglespeed.last_error=0;
	
	
	carBody.anglespeed_to_speed.kp=1.1;
	carBody.anglespeed_to_speed.ki=0;
	carBody.anglespeed_to_speed.kd=0;
	carBody.anglespeed_to_speed.error=0;
	carBody.anglespeed_to_speed.last_error=0;
	
	
}

int pid_SpeedControl_MA(int targetspeed,struct PID* motorX,int realspeed)
{
	motorX->error=targetspeed-realspeed;
	
	float P_path=(motorX->error)*motorX->kp;
	static float I_path=0;
	I_path+=(motorX->error)*motorX->ki;
	float D_path=(motorX->error-motorX->last_error)*motorX->kd;
	motorX->last_error=motorX->error;
	
	float outputvalue=P_path+I_path+D_path;
	
	return (int)outputvalue;
}
int pid_SpeedControl_MB(int targetspeed,struct PID* motorX,int realspeed)
{
	motorX->error=targetspeed-realspeed;
	
	float P_path=(motorX->error)*motorX->kp;
	static float I_path=0;
	I_path+=(motorX->error)*motorX->ki;
	float D_path=(motorX->error-motorX->last_error)*motorX->kd;
	motorX->last_error=motorX->error;
	
	float outputvalue=P_path+I_path+D_path;
	
	return (int)outputvalue;
}
int pid_SpeedControl_MC(int targetspeed,struct PID* motorX,int realspeed)
{
	motorX->error=targetspeed-realspeed;
	
	float P_path=(motorX->error)*motorX->kp;
	static float I_path=0;
	I_path+=(motorX->error)*motorX->ki;
	float D_path=(motorX->error-motorX->last_error)*motorX->kd;
	motorX->last_error=motorX->error;
	
	float outputvalue=P_path+I_path+D_path;
	
	return (int)outputvalue;
}
int pid_SpeedControl_MD(int targetspeed,struct PID* motorX,int realspeed)
{
	motorX->error=targetspeed-realspeed;
	
	float P_path=(motorX->error)*motorX->kp;
	static float I_path=0;
	I_path+=(motorX->error)*motorX->ki;
	float D_path=(motorX->error-motorX->last_error)*motorX->kd;
	motorX->last_error=motorX->error;
	
	float outputvalue=P_path+I_path+D_path;
	
	return (int)outputvalue;
}
int pid_TurnAngleOuterloopControl(float targetangle,struct TURNPID* carbody,float realangle)
{
	
	carbody->angle_to_anglespeed.error=targetangle-realangle;
	
	float P_path=(carbody->angle_to_anglespeed.error)*carbody->angle_to_anglespeed.kp;
	static float I_path=0;
	I_path+=(carbody->angle_to_anglespeed.error)*carbody->angle_to_anglespeed.ki;
	float D_path=(carbody->angle_to_anglespeed.error-carbody->angle_to_anglespeed.last_error)*carbody->angle_to_anglespeed.kd;
	carbody->angle_to_anglespeed.last_error=carbody->angle_to_anglespeed.error;
	
	
	int anglespeed_output=P_path+I_path+D_path;
	return anglespeed_output;
}

int pid_TurnAngleInnerloopControl(float targetanglespeed,struct TURNPID* carbody,float realanglespeed)
{
	carbody->anglespeed_to_speed.error=targetanglespeed-realanglespeed;
	
	float P_path=(carbody->anglespeed_to_speed.error)*carbody->anglespeed_to_speed.kp;
	static float I_path=0;
	I_path=(carbody->anglespeed_to_speed.error)*carbody->anglespeed_to_speed.ki;
	float D_path=(carbody->anglespeed_to_speed.error-carbody->anglespeed_to_speed.last_error)*carbody->anglespeed_to_speed.kd;

	int speed_output=P_path+I_path+D_path;
	return speed_output;

}
