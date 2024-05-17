#include "mypid.h"
PID_TypeDef  g_location_pid;    //λ�û�PID�����ṹ�� 
PID_TypeDef  g_speed_pid;       // �ٶȻ�PID�����ṹ�� 
PID_TypeDef  g_current_pid;     // ������PID�����ṹ�� 

//******************PID��ʼ������*************************************
void pid_init(void)
{
    //��ʼ��λ�û�PID���� 
    g_location_pid.SetPoint = 0;      //Ŀ��ֵ 
    g_location_pid.ActualValue = 0.0;   //PID��������ֵ
    g_location_pid.SumError = 0.0;      //����ֵ
    g_location_pid.Error = 0.0;         //Error[1]
    g_location_pid.LastError = 0.0;     //Error[-1]
    g_location_pid.PrevError = 0.0;     //Error[-2]
    g_location_pid.Proportion = L_KP;   //�������� Proportional Const 
    g_location_pid.Integral = L_KI;     //���ֳ��� Integral Const 
    g_location_pid.Derivative = L_KD;   //΢�ֳ��� Derivative Const 

    //��ʼ���ٶȻ�PID���� 
    g_speed_pid.SetPoint = 1000;         //Ŀ��ֵ 
    g_speed_pid.ActualValue = 0.0;      //PID��������ֵ
    g_speed_pid.SumError = 0.0;         //����ֵ 
    g_speed_pid.Error = 0.0;            //Error[1]
    g_speed_pid.LastError = 0.0;        //Error[-1] 
    g_speed_pid.PrevError = 0.0;        //Error[-2] 
    g_speed_pid.Proportion = S_KP;      //�������� Proportional Const 
    g_speed_pid.Integral = S_KI;        //���ֳ��� Integral Const 
    g_speed_pid.Derivative = S_KD;      //΢�ֳ��� Derivative Const 

    //��ʼ��������PID���� 
    g_current_pid.SetPoint = 0.0;       //Ŀ��ֵ 
    g_current_pid.ActualValue = 0.0;    //PID��������ֵ
    g_current_pid.SumError = 0.0;       //����ֵ
    g_current_pid.Error = 0.0;          //Error[1]
    g_current_pid.LastError = 0.0;      //Error[-1]
    g_current_pid.PrevError = 0.0;      //Error[-2]
    g_current_pid.Proportion = C_KP;    //�������� Proportional Const 
    g_current_pid.Integral = C_KI;      //���ֳ��� Integral Const 
    g_current_pid.Derivative = C_KD;    //΢�ֳ��� Derivative Const 
}
//***************����ʽPID���ƺ���************************
//*PID��PID�ṹ�������ַ
//Feedback_value����ǰʵ��ֵ
//lose_flag���ɺ��Ե����
//lose_n:�ɺ������ֵ
//����ֵ��PID��������ֵ
int32_t increment_pid_ctrl(PID_TypeDef *PID,float Feedback_value,int lose_flag,float lose_n)
{   
	  if(lose_flag==0)
		{
		   PID->Error = (float)(PID->SetPoint - Feedback_value);                   //����ƫ��   
		}
    if(lose_flag==1)
		{
		  PID->Error = (float)(PID->SetPoint - Feedback_value);
      if((PID->Error<=lose_n)&&(lose_n>=-lose_n))
			{
			 PID->Error =0;
			}        				
		}
    PID->ActualValue += (PID->Proportion * (PID->Error - PID->LastError))                          //�������� 
                        + (PID->Integral * PID->Error)                                             //���ֻ��� 
                        + (PID->Derivative * (PID->Error - 2 * PID->LastError + PID->PrevError));  //΢�ֻ��� 
    
    PID->PrevError = PID->LastError;                                        //�洢ƫ������´μ��� 
    PID->LastError = PID->Error;
    return ((int32_t)(PID->ActualValue));                                   //���ؼ�����������ֵ 
}
//***************λ��ʽPID���ƺ���************************
//*PID��PID�ṹ�������ַ
//Feedback_value����ǰʵ��ֵ
//lose_flag���ɺ��Ե����
//lose_n:�ɺ������ֵ
//����ֵ��PID��������ֵ
int32_t location_pid_ctrl(PID_TypeDef *PID,float Feedback_value,int lose_flag,float lose_n)
{
	  if(lose_flag==0)
		{
		   PID->Error = (float)(PID->SetPoint - Feedback_value);                   //����ƫ��   
		}
    if(lose_flag==1)
		{
		  PID->Error = (float)(PID->SetPoint - Feedback_value);
      if((PID->Error<=lose_n)&&(lose_n>=-lose_n))
			{
			  PID->Error =0;
				PID->SumError=0;
			}        				
		}
    PID->SumError += PID->Error;
    PID->ActualValue = (PID->Proportion * PID->Error)                       //�������� 
                       + (PID->Integral * PID->SumError)                    //���ֻ��� 
                       + (PID->Derivative * (PID->Error - PID->LastError)); //΢�ֻ��� 
    PID->LastError = PID->Error;    
    return ((int32_t)(PID->ActualValue));                                   //���ؼ�����������ֵ   
}
//*************�����޷�*********************************************
//*PID��PID�ṹ�������ַ
//max_limit�����ֵ
//min_limit����Сֵ


void integral_limit( PID_TypeDef *PID , float max_limit, float min_limit )
{
    if (PID->SumError >= max_limit)                           //�����޷� 
    {
        PID->SumError = max_limit;                            //���ƻ��� 
    }
    else if (PID->SumError <= min_limit)                      //�����޷� 
    {
        PID->SumError = min_limit;
    }
}


