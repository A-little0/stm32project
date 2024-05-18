#ifndef __MG4005_H
#define __MG4005_H

#include "main.h"
struct MGPID
{
	uint8_t anglePid_kp;
	uint8_t anglePid_ki;
	uint8_t speedPid_kp;
	uint8_t speedPid_ki;
	uint8_t iqPid_kp;
	uint8_t iqPid_ki;
};
typedef struct MGPID  MGpid;

struct MGARGUMENT
{
	//int16_t powerControl;//�������
	int16_t  iq;//ת�ص���ֵ
	int16_t  speed;//�ٶ�
	uint32_t Accel;//���ٶ�
	uint16_t encoder;//������λ��
	uint16_t encoderRaw;//��������ʼλ��
	uint16_t encoderOffset;//��������ƫ
	
	int64_t  motorAngle;//�����Ȧ�Ƕ�
	uint32_t circleAngle;//�����Ȧ�Ƕ�
	
	int8_t 	 temperature;//����¶�
	uint16_t voltage;//�����ѹ
	uint8_t  errorState;//����״̬�ֽ�
	
	int16_t iA;//A���������
	int16_t iB;//B���������
	int16_t iC;//C���������
	
};
typedef struct MGARGUMENT MGmotor;


extern MGpid motorpid;
extern MGmotor mgmotor1;

void ReadPIDArgument(uint8_t motorId);
void WritePIDArgumentToRAM(uint8_t motorId,MGpid* mg_pid);
void WritePIDArgumentToROM(uint8_t motorId,MGpid* mg_pid);
void ReadAccleration(uint8_t motorId);
void WriteAcclerationToRAM(uint8_t motorId,MGmotor* mgmotorX);
void ReadEncoder(uint8_t motorId);
void WriteTargetEncoderToROM(uint8_t motorId,MGmotor* mgmotorX);
void WriteNowEncoderToROM(uint8_t motorId);
void ReadMCircle(uint8_t motorId);
void ReadSCircle(uint8_t motorId);
void ClearMotorAngle(uint8_t motorId);
//
void ClearErrorMessage(uint8_t motorId);
void ReadMotorState2(uint8_t motorId);
void ReadMotorState3(uint8_t motorId);
void TurnOffMotor(uint8_t motorId);
void MotorStop(uint8_t motorId);
void MotorRun(uint8_t motorId);
void TorqueOpenControlLoop(uint8_t motorId,int16_t powerControl);
void TorqueCloseControlLoop(uint8_t motorId,int16_t iqControl);
void SpeedCloseControlLoop(uint8_t motorId,int32_t speedControl);
void AngleCloseControlLoop1(uint8_t motorId,int32_t angleControl);
void AngleCloseControlLoop2(uint8_t motorId,int32_t angleControl,uint16_t maxSpeed);
void AngleCloseControlLoop3(uint8_t motorId,uint16_t angleControl,uint8_t spinDirection);
void AngleCloseControlLoop4(uint8_t motorId,uint16_t angleControl,uint8_t spinDirection,uint16_t maxSpeed);
void AngleCloseControlLoop5(uint8_t motorId,int32_t angleControl);
void AngleCloseControlLoop6(uint8_t motorId,int32_t angleControl,uint16_t maxSpeed);

#endif
