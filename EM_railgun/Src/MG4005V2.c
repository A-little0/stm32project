#include "MG4005V2.h"
#include "can.h"

/********************************************************************************
--------------------------MG4005V2 COONTROL COMMAND------------------------------
0.��ȡpid��������	 													0x30
1.дpid������RAM���� 													0x31
2.дpid������ROM���� 													0x32
3.��ȡ���ٶ����� 														0x33
4.д����ٶȵ�RAM���� 													0x34
5.��ȡ���ٶ����� 														0x90
6.��ȡ������������ 														0x91
7.д���������ֵ��ROM��Ϊ����������  									0x91
8.д�뵱ǰλ�õ�ROM��Ϊ���������� 										0x19
9.��ȡ��Ȧ�Ƕ����� 														0x92
10.��ȡ��Ȧ�Ƕ����� 														0x94
11.�������Ƕ�������õ����ʼλ�ã�									0x95
12.������״̬1�ͺʹ����־����											0x9A
12.��ȡ��������־���� 													0x9B
13.��ȡ���״̬2����														0x9C
14.��ȡ���״̬3����														0x9D
15.����ر�����															0x80
16.���ֹͣ����															0x81
17.�����������															0x88
18.ת�ؿ�����������														0xA0
19.ת�رջ���������														0xA1
20.�ٶȱջ���������														0xA2
21.λ�ñջ���������1														0xA3
22.λ�ñջ���������2														0xA4
23.λ�ñջ���������3														0xA5
21.λ�ñջ���������4														0xA6
22.λ�ñջ���������5														0xA7
23.λ�ñջ���������6														0xA8
*************************************************************************************/

MGpid motorpid;
MGmotor mgmotor1;



void ReadPIDArgument(uint8_t motorId)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0x30;
		canTxData[1] = 0x00;
		canTxData[2] = 0x00;
		canTxData[3] = 0x00;
		canTxData[4] = 0x00;
		canTxData[5] = 0x00;
		canTxData[6] = 0x00;
		canTxData[7] = 0x00;
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);
		
}
void WritePIDArgumentToRAM(uint8_t motorId,MGpid* mg_pid)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0x31;
		canTxData[1] = 0x00;
		canTxData[2] = mg_pid->anglePid_kp;
		canTxData[3] = mg_pid->anglePid_ki;
		canTxData[4] = mg_pid->speedPid_kp;
		canTxData[5] = mg_pid->speedPid_ki;
		canTxData[6] = mg_pid->iqPid_kp;
		canTxData[7] = mg_pid->iqPid_ki;
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);
		
}
void WritePIDArgumentToROM(uint8_t motorId,MGpid* mg_pid)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0x32;
		canTxData[1] = 0x00;
		canTxData[2] = mg_pid->anglePid_kp;
		canTxData[3] = mg_pid->anglePid_ki;
		canTxData[4] = mg_pid->speedPid_kp;
		canTxData[5] = mg_pid->speedPid_ki;
		canTxData[6] = mg_pid->iqPid_kp;
		canTxData[7] = mg_pid->iqPid_ki;
		
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);
	
}	
void ReadAccleration(uint8_t motorId)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0x33;
		canTxData[1] = 0x00;
		canTxData[2] = 0x00;
		canTxData[3] = 0x00;
		canTxData[4] = 0x00;
		canTxData[5] = 0x00;
		canTxData[6] = 0x00;
		canTxData[7] = 0x00;
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);
	
}
void WriteAcclerationToRAM(uint8_t motorId,MGmotor* mgmotorX)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0x34;
		canTxData[1] = 0x00;
		canTxData[2] = 0x00;
		canTxData[3] = 0x00;
		canTxData[4] = *((uint8_t *)&mgmotorX->Accel + 0);
		canTxData[5] = *((uint8_t *)&mgmotorX->Accel + 1);
		canTxData[6] = *((uint8_t *)&mgmotorX->Accel + 2);
		canTxData[7] = *((uint8_t *)&mgmotorX->Accel + 3);
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);
	
}
void ReadEncoder(uint8_t motorId)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0x90;
		canTxData[1] = 0x00;
		canTxData[2] = 0x00;
		canTxData[3] = 0x00;
		canTxData[4] = 0x00;
		canTxData[5] = 0x00;
		canTxData[6] = 0x00;
		canTxData[7] = 0x00;
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);
	
}
void WriteTargetEncoderToROM(uint8_t motorId,MGmotor* mgmotorX)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0x91;
		canTxData[1] = 0x00;
		canTxData[2] = 0x00;
		canTxData[3] = 0x00;
		canTxData[4] = 0x00;
		canTxData[5] = 0x00;
		canTxData[6] = *((uint8_t *)&mgmotorX->encoderOffset + 0);
		canTxData[7] = *((uint8_t *)&mgmotorX->encoderOffset + 1);
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);
	
}
void WriteNowEncoderToROM(uint8_t motorId)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0x19;
		canTxData[1] = 0x00;
		canTxData[2] = 0x00;
		canTxData[3] = 0x00;
		canTxData[4] = 0x00;
		canTxData[5] = 0x00;
		canTxData[6] = 0x00;
		canTxData[7] = 0x00;
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);
	
}
void ReadMCircle(uint8_t motorId)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0x92;
		canTxData[1] = 0x00;
		canTxData[2] = 0x00;
		canTxData[3] = 0x00;
		canTxData[4] = 0x00;
		canTxData[5] = 0x00;
		canTxData[6] = 0x00;
		canTxData[7] = 0x00;
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);
	
}
void ReadSCircle(uint8_t motorId)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0x94;
		canTxData[1] = 0x00;
		canTxData[2] = 0x00;
		canTxData[3] = 0x00;
		canTxData[4] = 0x00;
		canTxData[5] = 0x00;
		canTxData[6] = 0x00;
		canTxData[7] = 0x00;
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);
	

}
//void ClearMotorAngle(uint8_t motorId)//��δʵ��
//{
//		uint32_t txMailBox;
//		canTxHeader.StdId = DEVICE_STD_ID + motorId;
//		canTxHeader.ExtId = 0x00;
//		canTxHeader.RTR = CAN_RTR_DATA;
//		canTxHeader.IDE = CAN_ID_STD;
//		canTxHeader.DLC = 8;
//		
//		canTxData[0] = 0x95;
//		canTxData[1] = 0x00;
//		canTxData[2] = 0x00;
//		canTxData[3] = 0x00;
//		canTxData[4] = 0x00;
//		canTxData[5] = 0x00;
//		canTxData[6] = 0x00;
//		canTxData[7] = 0x00;
//		
//		/* send can command */
//		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);
//	

//}
//12.������״̬1�ͺʹ����־����		
void ClearErrorMessage(uint8_t motorId)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0x9B;
		canTxData[1] = 0x00;
		canTxData[2] = 0x00;
		canTxData[3] = 0x00;
		canTxData[4] = 0x00;
		canTxData[5] = 0x00;
		canTxData[6] = 0x00;
		canTxData[7] = 0x00;
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);
	

}
void ReadMotorState2(uint8_t motorId)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0x9C;
		canTxData[1] = 0x00;
		canTxData[2] = 0x00;
		canTxData[3] = 0x00;
		canTxData[4] = 0x00;
		canTxData[5] = 0x00;
		canTxData[6] = 0x00;
		canTxData[7] = 0x00;
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);
	
}
void ReadMotorState3(uint8_t motorId)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0x9D;
		canTxData[1] = 0x00;
		canTxData[2] = 0x00;
		canTxData[3] = 0x00;
		canTxData[4] = 0x00;
		canTxData[5] = 0x00;
		canTxData[6] = 0x00;
		canTxData[7] = 0x00;
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);
	
}
void TurnOffMotor(uint8_t motorId)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0x80;
		canTxData[1] = 0x00;
		canTxData[2] = 0x00;
		canTxData[3] = 0x00;
		canTxData[4] = 0x00;
		canTxData[5] = 0x00;
		canTxData[6] = 0x00;
		canTxData[7] = 0x00;
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);
	
}
void MotorStop(uint8_t motorId)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0x81;
		canTxData[1] = 0x00;
		canTxData[2] = 0x00;
		canTxData[3] = 0x00;
		canTxData[4] = 0x00;
		canTxData[5] = 0x00;
		canTxData[6] = 0x00;
		canTxData[7] = 0x00;
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);
	
}
void MotorRun(uint8_t motorId)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0x88;
		canTxData[1] = 0x00;
		canTxData[2] = 0x00;
		canTxData[3] = 0x00;
		canTxData[4] = 0x00;
		canTxData[5] = 0x00;
		canTxData[6] = 0x00;
		canTxData[7] = 0x00;
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);
	
}
void TorqueOpenControlLoop(uint8_t motorId,int16_t powerControl)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0xA0;
		canTxData[1] = 0x00;
		canTxData[2] = 0x00;
		canTxData[3] = 0x00;
		canTxData[4] = *((uint8_t *)&powerControl + 0);
		canTxData[5] = *((uint8_t *)&powerControl + 1);
		canTxData[6] = 0x00;
		canTxData[7] = 0x00;
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);
	
}
void TorqueCloseControlLoop(uint8_t motorId,int16_t iqControl)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0xA1;
		canTxData[1] = 0x00;
		canTxData[2] = 0x00;
		canTxData[3] = 0x00;
		canTxData[4] = *((uint8_t *)&iqControl + 0);
		canTxData[5] = *((uint8_t *)&iqControl + 1);
		canTxData[6] = 0x00;
		canTxData[7] = 0x00;
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);

}
void SpeedCloseControlLoop(uint8_t motorId,int32_t speedControl)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0xA2;
		canTxData[1] = 0x00;
		canTxData[2] = 0x00;
		canTxData[3] = *((uint8_t *)&speedControl + 0);
		canTxData[4] = *((uint8_t *)&speedControl + 1);
		canTxData[5] = *((uint8_t *)&speedControl + 2);
		canTxData[6] = *((uint8_t *)&speedControl + 3);
		canTxData[7] = 0x00;
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);

}
/**
  * @function		AngleCloseControlLoop1
  * @brief          ����(���id,�����ٶ�)
  * @param[in]      motorId: 
  * @param[in]      angleControl: ���λ�ö�Ȧ�Ƕȣ�������Ŀ��Ƕ���ʵ�ʽǶȵĲ�ֵ ����Χ[]
  * @param[in]      �ÿ���ģʽ�£��������� ���ٶ� ����λ��(LK-Motor Tool)��Max Acclerationֵ����
  * @param[in]      �ÿ���ģʽ�£��������� ת��   ����λ��(LK-Motor Tool)��Max Torque Currentֵ����
  * @retval         36000����360��
  * @retval         none
  */
void AngleCloseControlLoop1(uint8_t motorId,int32_t angleControl)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0xA3;
		canTxData[1] = 0x00;
		canTxData[2] = 0x00;
		canTxData[3] = *((uint8_t *)&angleControl + 0);
		canTxData[4] = *((uint8_t *)&angleControl+ 1);
		canTxData[5] = *((uint8_t *)&angleControl + 2);
		canTxData[6] = *((uint8_t *)&angleControl + 3);
		canTxData[7] = 0x00;
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);

}
/**
  * @function		AngleCloseControlLoop2
  * @brief          ����(���id,���ƽǶȣ��ٶ�����)
  * @param[in]      motorId: 
  * @param[in]      angleControl: ���λ�ö�Ȧ�Ƕȣ�������Ŀ��Ƕ���ʵ�ʽǶȵĲ�ֵ ����Χ[]
  * @param[in]      maxSpeed:�������� �ٶ�  ����
  * @param[in]      �ÿ���ģʽ�£��������� �Ƕ�  ����λ��(LK-Motor Tool)��Max Angleֵ����
  * @param[in]      �ÿ���ģʽ�£��������� ���ٶ� ����λ��(LK-Motor Tool)��Max Acclerationֵ����
  * @param[in]      �ÿ���ģʽ�£��������� ת��   ����λ��(LK-Motor Tool)��Max Torque Currentֵ����
  * @retval         36000����360��
  * @retval         none
  */
void AngleCloseControlLoop2(uint8_t motorId,int32_t angleControl,uint16_t maxSpeed)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0xA4;
		canTxData[1] = 0x00;
		canTxData[2] = *((uint8_t *)&maxSpeed + 0);
		canTxData[3] = *((uint8_t *)&maxSpeed + 1);
		canTxData[4] = *((uint8_t *)&angleControl + 0);
		canTxData[5] = *((uint8_t *)&angleControl + 1);
		canTxData[6] = *((uint8_t *)&angleControl + 2);
		canTxData[7] = *((uint8_t *)&angleControl + 3);
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);

}
/**
  * @function		AngleCloseControlLoop3
  * @brief          ����(���id,���ƽǶȣ�ѡ����)
  * @param[in]      motorId: 
  * @param[in]      angleControl: ���λ�õ�Ȧ�Ƕȣ������ɷ��ž��� ����Χ[0,359.99]
  * @param[in]      spinDirection:0x00->˳ʱ�룬0x01->��ʱ��
  * @param[in]      �ÿ���ģʽ�£��������� ���ٶ� ����λ��(LK-Motor Tool)��Max Acclerationֵ����
  * @param[in]      �ÿ���ģʽ�£��������� ת��   ����λ��(LK-Motor Tool)��Max Torque Currentֵ����
  * @retval         none
  */
void AngleCloseControlLoop3(uint8_t motorId,uint16_t angleControl,uint8_t spinDirection)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0xA5;
		canTxData[1] = spinDirection;
		canTxData[2] = 0x00;
		canTxData[3] = 0x00;
		canTxData[4] = *((uint8_t *)&angleControl + 0);
		canTxData[5] = *((uint8_t *)&angleControl + 1);
		canTxData[6] = 0x00;
		canTxData[7] = 0x00;
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);

}
/**
  * @function		AngleCloseControlLoop4
  * @brief          ����(���id,���ƽǶ�,��ת����,����ٶ�)
  * @param[in]      motorId: 
  * @param[in]      angleControl: ���λ�õ�Ȧ�Ƕȣ������ɷ��ž��� ����Χ[0,359.99]
  * @param[in]      spinDirection:0x00->˳ʱ�룬0x01->��ʱ��
  * @param[in]      maxSpeed:�������� �ٶ�  ����
  * @retval         none
  */
void AngleCloseControlLoop4(uint8_t motorId,uint16_t angleControl,uint8_t spinDirection,uint16_t maxSpeed)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0xA6;
		canTxData[1] = spinDirection;
		canTxData[2] = *((uint8_t *)&maxSpeed + 0);
		canTxData[3] = *((uint8_t *)&maxSpeed + 1);
		canTxData[4] = *((uint8_t *)&angleControl + 0);
		canTxData[5] = *((uint8_t *)&angleControl + 1);
		canTxData[6] = 0x00;
		canTxData[7] = 0x00;
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);


}
/**
  * @function		AngleCloseControlLoop5
  * @brief          ����(���id,���ƽǶ�)
  * @param[in]      motorId: 
  * @param[in]      angleControl: ���λ���������ƣ������ɷ��ž��� ����Χ[]
  * @param[in]      �ÿ���ģʽ�£��������� �ٶ�   ����λ��(LK-Motor Tool)��Max Speedֵ����
  * @param[in]      �ÿ���ģʽ�£��������� ���ٶ� ����λ��(LK-Motor Tool)��Max Acclerationֵ����
  * @param[in]      �ÿ���ģʽ�£��������� ת��   ����λ��(LK-Motor Tool)��Max Torque Currentֵ����
  * @retval         none
  */
void AngleCloseControlLoop5(uint8_t motorId,int32_t angleControl)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0xA7;
		canTxData[1] = 0x00;
		canTxData[2] = 0x00;
		canTxData[3] = 0x00;
		canTxData[4] = *((uint8_t *)&angleControl + 0);
		canTxData[5] = *((uint8_t *)&angleControl + 1);
		canTxData[6] = *((uint8_t *)&angleControl + 2);
		canTxData[7] = *((uint8_t *)&angleControl + 3);
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);


}

/**
  * @function		AngleCloseControlLoop6
  * @brief          ����(���id,���ƽǶ�,����ٶ�)
  * @param[in]      motorId: 
  * @param[in]      angleControl: ���λ���������ƣ������ɷ��ž���
  * @param[in]      maxSpeed: ��Χ [0,]
  * @retval         none
  */
void AngleCloseControlLoop6(uint8_t motorId,int32_t angleControl,uint16_t maxSpeed)
{
		uint32_t txMailBox;
		canTxHeader.StdId = DEVICE_STD_ID + motorId;
		canTxHeader.ExtId = 0x00;
		canTxHeader.RTR = CAN_RTR_DATA;
		canTxHeader.IDE = CAN_ID_STD;
		canTxHeader.DLC = 8;
		
		canTxData[0] = 0xA8;
		canTxData[1] = 0x00;
		canTxData[2] = *((uint8_t *)&maxSpeed + 0);
		canTxData[3] = *((uint8_t *)&maxSpeed + 1);
		canTxData[4] = *((uint8_t *)&angleControl + 0);
		canTxData[5] = *((uint8_t *)&angleControl + 1);
		canTxData[6] = *((uint8_t *)&angleControl + 2);
		canTxData[7] = *((uint8_t *)&angleControl + 3);
		
		/* send can command */
		HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &txMailBox);
}
