#include "pid.h"


struct PID motorLeft;
struct PID motorRight;
struct STRAIGHTPID carBody;
struct PID newcar;

void Pid_Init(void)
{
	motorLeft.kp=2;
	motorLeft.ki=0;
	motorLeft.kd=0;
	motorLeft.error=0;
	motorLeft.last_error=0;
	
	motorRight.kp=2;
	motorRight.ki=0;
	motorRight.kd=0;
	motorRight.error=0;
	motorRight.last_error=0;
	

	carBody.angle_to_anglespeed.kp=-76;//-75;
	carBody.angle_to_anglespeed.ki=0;
	carBody.angle_to_anglespeed.kd=-200;//-200;
	carBody.angle_to_anglespeed.error=0;
	carBody.angle_to_anglespeed.last_error=0;
	
	
	carBody.anglespeed_to_speed.kp=0;
	carBody.anglespeed_to_speed.ki=0;
	carBody.anglespeed_to_speed.kd=0;
	carBody.anglespeed_to_speed.error=0;
	carBody.anglespeed_to_speed.last_error=0;
	
	newcar.kp=-85;
	newcar.ki=0;
	newcar.kd=-3;
	newcar.error=0;
	newcar.last_error=0;
	
	
}

int pid_SpeedControl_MLeft(int targetspeed,struct PID* motorX,int realspeed)
{
	motorX->error=targetspeed-realspeed;
	
	float P_path=(motorX->error)*motorX->kp;
	static float I_path=0;
	I_path+=(motorX->error)*motorX->ki;
	float D_path=(motorX->error-motorX->last_error)*motorX->kd;
	motorX->last_error=motorX->error;
	
	float outputvalue=P_path+I_path+D_path;
	if(targetspeed>2000){targetspeed=2000;}
	else if(targetspeed<-2000){targetspeed=-2000;}
	
	return (int)outputvalue;
}
int pid_SpeedControl_MRight(int targetspeed,struct PID* motorX,int realspeed)
{
	motorX->error=targetspeed-realspeed;
	
	float P_path=(motorX->error)*motorX->kp;
	static float I_path=0;
	I_path+=(motorX->error)*motorX->ki;
	float D_path=(motorX->error-motorX->last_error)*motorX->kd;
	motorX->last_error=motorX->error;
	
	float outputvalue=P_path+I_path+D_path;
	if(targetspeed>2000){targetspeed=2000;}
	else if(targetspeed<-2000){targetspeed=-2000;}
	
	return (int)outputvalue;
}

int pid_StraightAngleOuterloopControl(float targetangle,struct STRAIGHTPID* carbody,float realangle)
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

int pid_StraightAngleInnerloopControl(float targetanglespeed,struct STRAIGHTPID* carbody,float realanglespeed)
{
	carbody->anglespeed_to_speed.error=targetanglespeed-realanglespeed;
	
	float P_path=(carbody->anglespeed_to_speed.error)*carbody->anglespeed_to_speed.kp;
	static float I_path=0;
	I_path+=(carbody->anglespeed_to_speed.error)*carbody->anglespeed_to_speed.ki;
	float D_path=(carbody->anglespeed_to_speed.error-carbody->anglespeed_to_speed.last_error)*carbody->anglespeed_to_speed.kd;
	carbody->anglespeed_to_speed.last_error=carbody->anglespeed_to_speed.error;

	int speed_output=P_path+I_path+D_path;
	return speed_output;

}

int PDStraightAngleControl(float targetangle,struct PID* ncar,User_USART* jy901)
{
	ncar->error=targetangle-jy901->angle.angle[1];
	float P_path=(ncar->error)*ncar->kp;
	float D_path=(jy901->w.w[1])*ncar->kd;
	
	int speed_output=P_path+D_path;
	return speed_output;

}
