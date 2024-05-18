#ifndef __JY901_H
#define __JY901_H

#include "main.h"
#include "string.h"
#define RXBUFFER_LEN 33		//����9�����ݣ�һ��99λ
extern float ANGLE_ARR[3];

#define SAVE 			0x00
#define CALSW 		0x01
#define RSW 			0x02
#define RRATE			0x03
#define BAUD 			0x04
#define AXOFFSET	0x05
#define AYOFFSET	0x06
#define AZOFFSET	0x07
#define GXOFFSET	0x08
#define GYOFFSET	0x09
#define GZOFFSET	0x0a
#define HXOFFSET	0x0b
#define HYOFFSET	0x0c
#define HZOFFSET	0x0d
#define D0MODE		0x0e
#define D1MODE		0x0f
#define D2MODE		0x10
#define D3MODE		0x11
#define D0PWMH		0x12
#define D1PWMH		0x13
#define D2PWMH		0x14
#define D3PWMH		0x15
#define D0PWMT		0x16
#define D1PWMT		0x17
#define D2PWMT		0x18
#define D3PWMT		0x19
#define IICADDR		0x1a
#define LEDOFF 		0x1b
#define GPSBAUD		0x1c

#define YYMM				0x30
#define DDHH				0x31
#define MMSS				0x32
#define MS					0x33
#define AX					0x34
#define AY					0x35
#define AZ					0x36
#define GX					0x37
#define GY					0x38
#define GZ					0x39
#define HX					0x3a
#define HY					0x3b
#define HZ					0x3c			
#define Roll				0x3d
#define Pitch				0x3e
#define Yaw					0x3f
#define TEMP				0x40
#define D0Status		0x41
#define D1Status		0x42
#define D2Status		0x43
#define D3Status		0x44
#define PressureL		0x45
#define PressureH		0x46
#define HeightL			0x47
#define HeightH			0x48
#define LonL				0x49
#define LonH				0x4a
#define LatL				0x4b
#define LatH				0x4c
#define GPSHeight   0x4d
#define GPSYAW      0x4e
#define GPSVL				0x4f
#define GPSVH				0x50
#define q0          0x51
#define q1          0x52
#define q2          0x53
#define q3          0x54
      
#define DIO_MODE_AIN 0
#define DIO_MODE_DIN 1
#define DIO_MODE_DOH 2
#define DIO_MODE_DOL 3
#define DIO_MODE_DOPWM 4
#define DIO_MODE_GPS 5		

struct STime
{
	unsigned char ucYear;
	unsigned char ucMonth;
	unsigned char ucDay;
	unsigned char ucHour;
	unsigned char ucMinute;
	unsigned char ucSecond;
	unsigned short usMiliSecond;
};
struct SAcc
{
	short a[3];
	short T;
};
struct SGyro
{
	short w[3];
	short T;
};
struct SAngle
{
	short Angle[3];
	short T;
};
struct SMag
{
	short h[3];
	short T;
};

struct SDStatus
{
	short sDStatus[4];
};

struct SPress
{
	long lPressure;
	long lAltitude;
};

struct SLonLat
{
	long lLon;
	long lLat;
};

struct SGPSV
{
	short sGPSHeight;
	short sGPSYaw;
	long lGPSVelocity;
};
struct SQ
{ short q[4];
};
 
//�ض���ṹ���JY901���ݽṹ��
typedef struct
{
	float angle[3];
}Angle;

typedef struct
{
	float a[3];
}Acc;

typedef struct
{
	float w[3];
}SGyro;


typedef struct//��Ԫ��
{ float q[4];
}SQ;

typedef struct//�ų����
{
	float h[3];
}SMag;

typedef struct//��ѹ�߶�
{
	float lPressure;
	float lAltitude;
}SPress;

typedef struct//��γ��
{
	float lLon;
	float lLat;
}SLonLat;


typedef struct User_USART
{
		uint8_t Rx_flag;											
		uint8_t Rx_len;												
		uint8_t frame_head;					//֡ͷ
		uint8_t RxBuffer[RXBUFFER_LEN];		//���ջ���
		Angle angle;						//�Ƕ�
		Acc acc;								//���ٶ�
		SGyro w;								//���ٶ�
		SMag h;									//�ų�
		SPress lPressure;   	  //��ѹ
		SPress lAltitude;     	//�߶�
		SLonLat lLon;						//����
		SLonLat lLat;						//ά��
		SQ q; 									//��Ԫ��
}User_USART;

//���սṹ���ʼ��
void JY901_Init(void);
void JY901_Recieve(User_USART* JY901_data);
void User_USART_Init(User_USART *Data);
void JY901_Process(User_USART* JY901_data);
extern User_USART JY901_data;
#endif

