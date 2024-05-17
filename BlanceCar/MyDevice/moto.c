#include "moto.h"
//u8 mpu6050_data_flag;  //是否正常读取数据
int bianma1=0;         //编码器1的数据
int bianma2=0;         //编码器2的数据
float pitch,roll,yaw;  //欧拉角
short aacx,aacy,aacz;	 //加速度传感器原始数据
short gyrox,gyroy,gyroz;//陀螺仪原始数据
short mpu6050_temp;		 //温度	
int bianma1_num=0;     //记录1号编码器数值用于测速
int bianma2_num=0;     //记录2号编码器数值用于测速
int speed_num1=0;      //1号电机速度 
int speed_num2=0;      //2号电机速度 
int PID_temp=0;        //PID计算后输出值
int bianma_cnt1=0;     //保存编码器1旋转总数
int bianma_cnt2=0;     //保存编码器2旋转总数
int speed_avr1=0;      //测量速度1   
int	speed_last_avr1=0; //上次测量的速度1
int speed_avr1_num=0;  //速度1计算用中间值
int speed_bga1[10];    //速度1缓存数据包
int speed_cnt1=0;      //速度1滤波数组计数
int speed_avr2=0;      //测量速度2  
int	speed_last_avr2=0; //上次测量的速度2
int speed_avr2_num=0;  //速度2计算用中间值
int speed_bga2[10];    //速度2缓存数据包
int speed_cnt2=0;      //速度2滤波数组计数
int sudu_out=0.0f;     //速度环输出
int zhili_out=0;       //直立环输出
int zhuan_out=0;        //转向换输出
int PWM_OUT1=0;        //最终输出到左电机
int PWM_OUT2=0;        //最终输出到右电机
//******移动速度****************************
int Movement=0;   //100以上  正数前进，复数后退
int turnment=0;   //100以上  正数顺时针，负数逆时针
//****************小车机械零点***************
float Car_zero=1.5f;      
//*************PID参数宏定义*****************
//直立环
float zhili_Kp=-300.0f;  //出现大幅度低频振荡 -500
float zhili_Kd=1.08f;    //出现小幅度高频振荡 1.8
//速度环
float sudu_Kp=15.0f;  
float sudu_Ki=0.075f;    //除以200
//转向环
float zhuan_Kp=20.0f;   //期望小车转向，正反馈
float zhuan_Kd=0.6f;    //抑制小车转向，负反馈
//**************编码器读取***************************
void Encoder_Count_read(void)
{
	bianma1=Encoder_Value(1);
    bianma2=Encoder_Value(3);
}
//************MPU6050相关数据读取********************
void MPU6050_Data_read(void)
{
	pitch=JY901_data.angle.angle[0];
	roll=JY901_data.angle.angle[1];
	yaw=JY901_data.angle.angle[2];
	//得到加速度传感器数据
	aacx=JY901_data.acc.a[0];
	aacy=JY901_data.acc.a[1];
	aacz=JY901_data.acc.a[2];
	//得到陀螺仪数据
	//MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	
	gyroy=JY901_data.w.w[1];	
}
//***********速度读取不带滤波*******************************
void Moto_Speed_Read(int n)
{ 
	//1号电机测速
	if(n==1)
	{
	  speed_num1=Encoder_Value(1); //读取XXms以后的编码器数值
		speed_num1=speed_num1*10;
		Encoder_Count_Clear(1);       //将编码器清零，用于下次计数		
	}
	//2号电机测速
	if(n==2)
	{
	  speed_num2=-(Encoder_Value(3));//读取XXms以后的编码器数值
		speed_num2=speed_num2*10;
		Encoder_Count_Clear(3);      //将编码器清零，用于下次计数
	}	 
}
//****************速度1读取，带滤波****************************
void Speed1_Read(void)
{
	int i=0,j=0;
	int temp=0;
	speed_last_avr1=speed_avr1;         //保存上次计算的结果
    speed_avr1_num=Encoder_Value(1); //读取XXms以后的编码器数值
	bianma_cnt1+=speed_avr1_num;        //记录总里程
	speed_avr1_num*=100;                 //1秒时间编码器对应的数值得出秒速度 
	Encoder_Count_Clear(1);          //将编码器清零，用于下次计数		
	speed_bga1[speed_cnt1]=speed_avr1_num;  //冒泡排序数据封包
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
		for(i=2;i<8;i++)                  //去掉2个最小值，去掉2个最大值      
		{
		  temp+=speed_bga1[i];                     
		}	
		speed_avr1_num=temp/6;    //本次最新计算结果
		//一阶低通滤波
		speed_avr1 = (float)( ((float)0.8* speed_avr1_num) + ((float)0.2)*speed_last_avr1 );
	}		
}
//****************速度2读取，带滤波****************************
void Speed2_Read(void)
{
	int i=0,j=0;
	int temp=0;
	speed_last_avr2=speed_avr2;         //保存上次计算的结果
    speed_avr2_num=-(Encoder_Value(3)); //读取XXms以后的编码器数值
	bianma_cnt2+=speed_avr2_num;        //记录总里程
	speed_avr2_num*=100;                 //1秒时间编码器对应的数值得出秒速度 
	Encoder_Count_Clear(3);          //将编码器清零，用于下次计数		
	speed_bga2[speed_cnt2]=speed_avr2_num;  //冒泡排序数据封包
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
		for(i=2;i<8;i++)                  //去掉2个最小值，去掉2个最大值      
		{
		  temp+=speed_bga2[i];                     
		}	
		speed_avr2_num=temp/6;    //本次最新计算结果
		//一阶低通滤波
		speed_avr2 = (float)( ((float)0.8 * speed_avr2_num) + ((float)0.2)*speed_last_avr2 );
	}	
}
//***********绝对值函数*****************************
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
//***********电机控制*******************************
//pwm: 电机pwm
//n:   电机编号
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
//***********平衡车控制******************************************
//函数功能：控制小车保持直立
//Angle：采集到的实际角度值
//Gyro： 采集到的实际角速度值
int zhili(float Angle,float Gyro)
{  
     float err;
	 int pwm_zhili;
	 err=Car_zero-Angle;    //期望值-实际值，这里期望小车平衡，因此期望值就是机械中值       
	 pwm_zhili=zhili_Kp*err+Gyro*zhili_Kd;//计算平衡控制的电机PWM
	 return pwm_zhili;
}
//*****************************************************************
//函数功能：控制小车速度
//encoder_left： 左轮编码器值
//encoder_right：右轮编码器值   因为程序周期执行，所以这里编码器的值可以理解为速度
int sudu(int encoder_left,int encoder_right)
{  
	  static int pwm_sudu,Encoder_Least,Encoder;
	  static int Encoder_Integral;	
		Encoder_Least =(encoder_left+encoder_right)-Movement;  //获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此次为零） 
		Encoder *= 0.8;		                             //一阶低通滤波器       
		Encoder += Encoder_Least*0.2;	                 //一阶低通滤波器    
  	    Encoder_Integral +=Encoder;                     //积分出位移 积分时间：5ms
		if(Encoder_Integral>8000)  Encoder_Integral=8000;  //积分限幅
		if(Encoder_Integral<-8000)	Encoder_Integral=-8000; //积分限幅	
		pwm_sudu=sudu_Kp*Encoder+sudu_Ki*Encoder_Integral;     //速度PI控制器	
		if((pitch>=80)||(pitch<=-80))  //小车跌倒后清零
		{
		  Encoder_Integral=0;    
		}			
	  return pwm_sudu;
}
//*************************************************************
//函数功能：控制小车转向
//Set_turn：目标旋转角速度
//Gyro_Z:陀螺仪Z轴的角速度
//不是一个严格的PD控制器，为小车的叠加控制
int zhuan(float Set_turn,float Gyro_Z)
{
  int PWM_Out=0; 
	if(Set_turn==0)
	{
	 PWM_Out=zhuan_Kd*Gyro_Z; //没有转向需求，Kd约束小车转向
	}
	if(Set_turn!=0)
	{
	 PWM_Out=zhuan_Kp*Set_turn; //有转向需求，Kp为期望小车转向 
	}
	return PWM_Out;
}
//*********平衡车运动任务**********************************
void Smart_Car_Task(void)
{
	zhili_out=zhili(roll,gyroy);							  //直立环
	sudu_out=sudu(speed_num1,speed_num2);                     //速度环
    zhuan_out=0;//zhuan(turnment,gyroz);                          //转向环，并行关系
	PWM_OUT1=zhili_out+sudu_out+zhuan_out;                    //速度差产生转向效果
	PWM_OUT2=zhili_out+sudu_out-zhuan_out;                    //速度差产生转向效果
	
    if(PWM_OUT1>=2000){PWM_OUT1=2000;}
	else if(PWM_OUT1<=2000){PWM_OUT1=-2000;}
	if(PWM_OUT2>=2000){PWM_OUT2=2000;}
	else if(PWM_OUT2<=-2000){PWM_OUT2=-2000;}

	if((roll>=80)||(roll<=-80))
	{
	    PWM_OUT1=0;
		PWM_OUT2=0;       
	}

	Moto_Ctrl(PWM_OUT1,1); //作用到电机
	Moto_Ctrl(PWM_OUT2,2); //作用到电机
	
}


