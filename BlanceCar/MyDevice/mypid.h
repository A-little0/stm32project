#ifndef _pid_H
#define _pid_H
#include "main.h"
//***************PID��������********************************************
//����λ�û�PID������غ� 
#define  L_KP      2.5f             // P���� 
#define  L_KI      0.0f             // I���� 
#define  L_KD      0.2f             // D���� 

//�����ٶȻ�PID������غ� 
#define  S_KP      0.03f              // P���� 
#define  S_KI      0.02f             // I���� 
#define  S_KD      0.01f             // D���� 

//���������PID������غ� 
#define  C_KP      8.00f             // P���� 
#define  C_KI      4.00f             // I���� 
#define  C_KD      1.00f             // D���� 
#define  SMAPLSE_PID_SPEED  50       // �������� ��λms 
//PID�ṹ��
typedef struct
{
     float  SetPoint;            //�û�Ŀ��ֵ 
     float  ActualValue;         //PID��������ֵ 
     float  SumError;            //����ۼ� 
     float  Proportion;          //�������� P 
     float  Integral;            //���ֳ��� I 
     float  Derivative;          //΢�ֳ��� D 
     float  Error;               //Error[1] 
     float  LastError;           //Error[-1] 
     float  PrevError;           //Error[-2] 
} PID_TypeDef;

extern PID_TypeDef  g_location_pid;  //λ�û�PID�����ṹ�� 
extern PID_TypeDef  g_speed_pid;     //�ٶȻ�PID�����ṹ�� 
extern PID_TypeDef  g_current_pid;   //������PID�����ṹ�� 

//****************************************************************************************

void pid_init(void);                 //pid��ʼ��
int32_t increment_pid_ctrl(PID_TypeDef *PID,float Feedback_value,int lose_flag,float lose_n);
int32_t location_pid_ctrl(PID_TypeDef *PID,float Feedback_value,int lose_flag,float lose_n);
void integral_limit( PID_TypeDef *PID , float max_limit, float min_limit );     //�����޷� 

#endif

