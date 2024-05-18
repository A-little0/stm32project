/**
  ******************************************************************************
  * @file    motor.c
  * @author  chenjiangnan
  * @brief   �����������һЩ����ģʽ.
  *
  @verbatim
  ==============================================================================
  
                     ##### How to use this driver #####
  ==============================================================================
  @endverbatim
  ******************************************************************************
  * @attention
  *	1.��Ҫ������ͷ�ļ���motor.h(���ⲿ�ļ��ṩ���������������Լ���غ궨��)
  *					   tim.h(�ṩhal�ⶨʱ�������йصĿ⺯��
  *	2.���ļ�����������tb6612����l298n֮��ĵ������ģ��(������ת�Լ�pwm����ģʽ)
  ******************************************************************************
  */
#include "motor.h"
#include "tim.h"

int target_speedMA;//motor A��Ŀ���ٶ�
int target_speedMB;//motor B��Ŀ���ٶ�
int target_speedMC;//motor C��Ŀ���ٶ�
int target_speedMD;//motor D��Ŀ���ٶ�
int setPwm_MA;//������motor A��pwmֵ
int setPwm_MB;//������motor B��pwmֵ
int setPwm_MC;//������motor C��pwmֵ
int setPwm_MD;//������motor D��pwmֵ
int realspeed_MA;//motor A��ʵ��ת��(��λʱ�䲶�������)
int realspeed_MB;//motor B��ʵ��ת��(��λʱ�䲶�������)
int realspeed_MC;//motor C��ʵ��ת��(��λʱ�䲶�������)
int realspeed_MD;//motor D��ʵ��ת��(��λʱ�䲶�������)
int target_angle;//���̵�Ŀ��Ƕȣ��������������ϵ��
int watch_outerloopoutput;//����⻷���ֵ
int watch_interloopoutput;//����ڻ����ֵ

extern int buffArr[1];
extern int dataArr[8];
extern int _motorSingal;
extern int openmv_setpwm;
extern int basepwm;

/**
  * @function		void motor_Init(void)
  * @brief          ��������ĳ�ʼ������
  *					��ʼ�����ļ���ȫ�ֱ���
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
  * @brief          ����pwm���������Ժ���				
  *					-->�жϵ������ת�����ŵ�ƽ(�궨��)
  *					-->�����ת
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
  * @brief          �����ٶȱջ����Ժ���				
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
	if(setPwm_MB<0){//����ת�ж�
		setPwm_MB=-setPwm_MB;
		MB_ENA_OUT1_H;
		MB_ENA_OUT2_L;
	}
	if(setPwm_MB>2000){setPwm_MB=2000;}//�޷�
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,setPwm_MB);
	
	//MOTOR D
	MD_ENA_OUT1_L;
	MD_ENA_OUT2_H;
	target_speedMD=30;
	setPwm_MD=pid_SpeedControl_MD(target_speedMD,&motorD,realspeed_MD);
	if(setPwm_MD<0){//����ת�ж�
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
	if(setPwm_MA<0){//����ת�ж�
		setPwm_MA=-setPwm_MA;
		MA_ENA_OUT1_L;
		MA_ENA_OUT2_H;
	}
	if(setPwm_MA>2000){setPwm_MA=2000;}//�޷�
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,setPwm_MA);
	
    //MOTOR C
	MC_ENA_OUT1_H;
	MC_ENA_OUT2_L;
	target_speedMC=30;
	setPwm_MC=pid_SpeedControl_MC(target_speedMC,&motorC,realspeed_MC);
	if(setPwm_MC<0){//����ת�ж�
		MC_ENA_OUT1_L;
		MC_ENA_OUT2_H;
		setPwm_MC=-setPwm_MC;
	}
	if(setPwm_MC>2000){setPwm_MC=2000;}//�޷�
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,setPwm_MC);
}

/**
  * @function		void chassis_turn(void)
  * @brief          ���̽Ƕȴ����ջ����Ժ���				
  *	@brief 				-->����
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
  * @brief          ѭ��ģʽ�µĵ��̿��Ʒ���				
  * @param[in]      int base_speed->����״̬��С��Ѳ�ߵ��ٶ�
  * @param[in]      int  add_speed->ƫ��״̬��С�����ⲹ�����ٶ�
  * @retval         none
  * @attention		ͬ���ֲ���ֵһ��
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
  * @brief          ��ʱ�����Ƶ�����ת����				
  * @param[in]      int base_turnSpeed->С����ת���ٶ�
  * @retval         none
  * @attention		�ڶ�ʱ�����ڵ��øú���
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
  * @brief          ���Ƶ����˶�����				
  * @param[in]      int base_moveSpeed->С�����ٶ�
  * @retval         none
  * @attention		ֻ��ǰ�����ҷ�����˶�
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


