#include "moto.h"
//u8 mpu6050_data_flag;  //�Ƿ�������ȡ����
int bianma1=0;         //������1������
int bianma2=0;         //������2������
float pitch,roll,yaw;  //ŷ����
short aacx,aacy,aacz;	 //���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;//������ԭʼ����
short mpu6050_temp;		 //�¶�	
int bianma1_num=0;     //��¼1�ű�������ֵ���ڲ���
int bianma2_num=0;     //��¼2�ű�������ֵ���ڲ���
int speed_num1=0;      //1�ŵ���ٶ� 
int speed_num2=0;      //2�ŵ���ٶ� 
int PID_temp=0;        //PID��������ֵ
int bianma_cnt1=0;     //���������1��ת����
int bianma_cnt2=0;     //���������2��ת����
int speed_avr1=0;      //�����ٶ�1   
int	speed_last_avr1=0; //�ϴβ������ٶ�1
int speed_avr1_num=0;  //�ٶ�1�������м�ֵ
int speed_bga1[10];    //�ٶ�1�������ݰ�
int speed_cnt1=0;      //�ٶ�1�˲��������
int speed_avr2=0;      //�����ٶ�2  
int	speed_last_avr2=0; //�ϴβ������ٶ�2
int speed_avr2_num=0;  //�ٶ�2�������м�ֵ
int speed_bga2[10];    //�ٶ�2�������ݰ�
int speed_cnt2=0;      //�ٶ�2�˲��������
int sudu_out=0.0f;     //�ٶȻ����
int zhili_out=0;       //ֱ�������
int zhuan_out=0;        //ת�����
int PWM_OUT1=0;        //�������������
int PWM_OUT2=0;        //����������ҵ��
//******�ƶ��ٶ�****************************
int Movement=0;   //100����  ����ǰ������������
int turnment=0;   //100����  ����˳ʱ�룬������ʱ��
//****************С����е���***************
float Car_zero=1.5f;      
//*************PID�����궨��*****************
//ֱ����
float zhili_Kp=-300.0f;  //���ִ���ȵ�Ƶ�� -500
float zhili_Kd=1.08f;    //����С���ȸ�Ƶ�� 1.8
//�ٶȻ�
float sudu_Kp=15.0f;  
float sudu_Ki=0.075f;    //����200
//ת��
float zhuan_Kp=20.0f;   //����С��ת��������
float zhuan_Kd=0.6f;    //����С��ת�򣬸�����
//**************��������ȡ***************************
void Encoder_Count_read(void)
{
	bianma1=Encoder_Value(1);
    bianma2=Encoder_Value(3);
}
//************MPU6050������ݶ�ȡ********************
void MPU6050_Data_read(void)
{
	pitch=JY901_data.angle.angle[0];
	roll=JY901_data.angle.angle[1];
	yaw=JY901_data.angle.angle[2];
	//�õ����ٶȴ���������
	aacx=JY901_data.acc.a[0];
	aacy=JY901_data.acc.a[1];
	aacz=JY901_data.acc.a[2];
	//�õ�����������
	//MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	
	gyroy=JY901_data.w.w[1];	
}
//***********�ٶȶ�ȡ�����˲�*******************************
void Moto_Speed_Read(int n)
{ 
	//1�ŵ������
	if(n==1)
	{
	  speed_num1=Encoder_Value(1); //��ȡXXms�Ժ�ı�������ֵ
		speed_num1=speed_num1*10;
		Encoder_Count_Clear(1);       //�����������㣬�����´μ���		
	}
	//2�ŵ������
	if(n==2)
	{
	  speed_num2=-(Encoder_Value(3));//��ȡXXms�Ժ�ı�������ֵ
		speed_num2=speed_num2*10;
		Encoder_Count_Clear(3);      //�����������㣬�����´μ���
	}	 
}
//****************�ٶ�1��ȡ�����˲�****************************
void Speed1_Read(void)
{
	int i=0,j=0;
	int temp=0;
	speed_last_avr1=speed_avr1;         //�����ϴμ���Ľ��
    speed_avr1_num=Encoder_Value(1); //��ȡXXms�Ժ�ı�������ֵ
	bianma_cnt1+=speed_avr1_num;        //��¼�����
	speed_avr1_num*=100;                 //1��ʱ���������Ӧ����ֵ�ó����ٶ� 
	Encoder_Count_Clear(1);          //�����������㣬�����´μ���		
	speed_bga1[speed_cnt1]=speed_avr1_num;  //ð���������ݷ��
	speed_cnt1++;
	if(speed_cnt1>=10)
	{
	  speed_cnt1=0;
		for(i=10;i>=1;i--)                      
		{
			for(j=0;j<(i - 1);j++) 
			{
				if(speed_bga1[j] > speed_bga1[j+1])   
				{ 
					temp=speed_bga1[j];                
					speed_bga1[j]=speed_bga1[j+1];
					speed_bga1[j+1]=temp;
				}
			}
		}	
		temp=0;	
		for(i=2;i<8;i++)                  //ȥ��2����Сֵ��ȥ��2�����ֵ      
		{
		  temp+=speed_bga1[i];                     
		}	
		speed_avr1_num=temp/6;    //�������¼�����
		//һ�׵�ͨ�˲�
		speed_avr1 = (float)( ((float)0.8* speed_avr1_num) + ((float)0.2)*speed_last_avr1 );
	}		
}
//****************�ٶ�2��ȡ�����˲�****************************
void Speed2_Read(void)
{
	int i=0,j=0;
	int temp=0;
	speed_last_avr2=speed_avr2;         //�����ϴμ���Ľ��
    speed_avr2_num=-(Encoder_Value(3)); //��ȡXXms�Ժ�ı�������ֵ
	bianma_cnt2+=speed_avr2_num;        //��¼�����
	speed_avr2_num*=100;                 //1��ʱ���������Ӧ����ֵ�ó����ٶ� 
	Encoder_Count_Clear(3);          //�����������㣬�����´μ���		
	speed_bga2[speed_cnt2]=speed_avr2_num;  //ð���������ݷ��
	speed_cnt2++;
	if(speed_cnt2>=10)
	{
	  speed_cnt2=0;
		for(i=10;i>=1;i--)                      
		{
			for(j=0;j<(i - 1);j++) 
			{
				if(speed_bga2[j] > speed_bga2[j+1])   
				{ 
					temp=speed_bga2[j];                
					speed_bga2[j]=speed_bga2[j+1];
					speed_bga2[j+1]=temp;
				}
			}
		}	
		temp=0;	
		for(i=2;i<8;i++)                  //ȥ��2����Сֵ��ȥ��2�����ֵ      
		{
		  temp+=speed_bga2[i];                     
		}	
		speed_avr2_num=temp/6;    //�������¼�����
		//һ�׵�ͨ�˲�
		speed_avr2 = (float)( ((float)0.8 * speed_avr2_num) + ((float)0.2)*speed_last_avr2 );
	}	
}
//***********����ֵ����*****************************
int my_abs(int n)
{
  if(n<0)
	{
	  n=(-n);
	}
	if(n>=0)
	{
	 n=n;
	}
	return n;
}
//***********�������*******************************
//pwm: ���pwm
//n:   ������
void Moto_Ctrl(int pwm,int n)
{
	//int pwm_num=0;
	//pwm_num=my_abs(pwm);
	if(n==1)
	{
		if(pwm>=0)
		{
		   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,0);
		   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,1);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,pwm);
		}
		else if(pwm<0)	
		{
		   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,1);
		   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,-pwm);
		}			 
	}	
	if(n==2)
	{
	    if(pwm>=0)
		{
		    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,1);
		    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,pwm);
		}
		else if(pwm<0)
		{
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,0);
		    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,1);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,-pwm);
		}
	}				   
}
//***********ƽ�⳵����******************************************
//�������ܣ�����С������ֱ��
//Angle���ɼ�����ʵ�ʽǶ�ֵ
//Gyro�� �ɼ�����ʵ�ʽ��ٶ�ֵ
int zhili(float Angle,float Gyro)
{  
     float err;
	 int pwm_zhili;
	 err=Car_zero-Angle;    //����ֵ-ʵ��ֵ����������С��ƽ�⣬�������ֵ���ǻ�е��ֵ       
	 pwm_zhili=zhili_Kp*err+Gyro*zhili_Kd;//����ƽ����Ƶĵ��PWM
	 return pwm_zhili;
}
//*****************************************************************
//�������ܣ�����С���ٶ�
//encoder_left�� ���ֱ�����ֵ
//encoder_right�����ֱ�����ֵ   ��Ϊ��������ִ�У����������������ֵ�������Ϊ�ٶ�
int sudu(int encoder_left,int encoder_right)
{  
	  static int pwm_sudu,Encoder_Least,Encoder;
	  static int Encoder_Integral;	
		Encoder_Least =(encoder_left+encoder_right)-Movement;  //��ȡ�����ٶ�ƫ��==�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩 
		Encoder *= 0.8;		                             //һ�׵�ͨ�˲���       
		Encoder += Encoder_Least*0.2;	                 //һ�׵�ͨ�˲���    
  	    Encoder_Integral +=Encoder;                     //���ֳ�λ�� ����ʱ�䣺5ms
		if(Encoder_Integral>8000)  Encoder_Integral=8000;  //�����޷�
		if(Encoder_Integral<-8000)	Encoder_Integral=-8000; //�����޷�	
		pwm_sudu=sudu_Kp*Encoder+sudu_Ki*Encoder_Integral;     //�ٶ�PI������	
		if((pitch>=80)||(pitch<=-80))  //С������������
		{
		  Encoder_Integral=0;    
		}			
	  return pwm_sudu;
}
//*************************************************************
//�������ܣ�����С��ת��
//Set_turn��Ŀ����ת���ٶ�
//Gyro_Z:������Z��Ľ��ٶ�
//����һ���ϸ��PD��������ΪС���ĵ��ӿ���
int zhuan(float Set_turn,float Gyro_Z)
{
  int PWM_Out=0; 
	if(Set_turn==0)
	{
	 PWM_Out=zhuan_Kd*Gyro_Z; //û��ת������KdԼ��С��ת��
	}
	if(Set_turn!=0)
	{
	 PWM_Out=zhuan_Kp*Set_turn; //��ת������KpΪ����С��ת�� 
	}
	return PWM_Out;
}
//*********ƽ�⳵�˶�����**********************************
void Smart_Car_Task(void)
{
	zhili_out=zhili(roll,gyroy);							  //ֱ����
	sudu_out=sudu(speed_num1,speed_num2);                     //�ٶȻ�
    zhuan_out=0;//zhuan(turnment,gyroz);                          //ת�򻷣����й�ϵ
	PWM_OUT1=zhili_out+sudu_out+zhuan_out;                    //�ٶȲ����ת��Ч��
	PWM_OUT2=zhili_out+sudu_out-zhuan_out;                    //�ٶȲ����ת��Ч��
	
    if(PWM_OUT1>=2000){PWM_OUT1=2000;}
	else if(PWM_OUT1<=2000){PWM_OUT1=-2000;}
	if(PWM_OUT2>=2000){PWM_OUT2=2000;}
	else if(PWM_OUT2<=-2000){PWM_OUT2=-2000;}

	if((roll>=80)||(roll<=-80))
	{
	    PWM_OUT1=0;
		PWM_OUT2=0;       
	}

	Moto_Ctrl(PWM_OUT1,1); //���õ����
	Moto_Ctrl(PWM_OUT2,2); //���õ����
	
}


