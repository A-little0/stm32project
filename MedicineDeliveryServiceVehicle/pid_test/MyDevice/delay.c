#include "delay.h"

/******************************************************************

@�ȶ���������ȫ�ֱ���usDelayBase��4��������
@��main��������whileѭ��֮ǰ��ִ�� PY_usDelayTest(); �� PY_usDelayOptimize();
@����Ҫ����us��ʱ��ʱ��ִ��PY_Delay_us(x)�� PY_Delay_us_t(x); ����xΪҪ��ʱ��΢������
��ִ��PY_Delay_us(12) ;Ϊ��ʱ12΢�룻PY_Delay_us(1020) ; Ϊ��ʱ1����20΢�롣

*******************************************************************/

__IO float usDelayBase;
void PY_usDelayTest(void)
{
  __IO uint32_t firstms, secondms;
  __IO uint32_t counter = 0;

  firstms = HAL_GetTick()+1;
  secondms = firstms+1;

  while(uwTick!=firstms) ;

  while(uwTick!=secondms) counter++;

  usDelayBase = ((float)counter)/1000;
}

void PY_Delay_us_t(uint32_t Delay)
{
  __IO uint32_t delayReg;
  __IO uint32_t usNum = (uint32_t)(Delay*usDelayBase);

  delayReg = 0;
  while(delayReg!=usNum) delayReg++;
}


void PY_usDelayOptimize(void)
{
  __IO uint32_t firstms, secondms;
  __IO float coe = 1.0;

  firstms = HAL_GetTick();
  PY_Delay_us_t(1000000) ;
  secondms = HAL_GetTick();

  coe = ((float)1000)/(secondms-firstms);
  usDelayBase = coe*usDelayBase;
}

void PY_Delay_us(uint32_t Delay)
{
  __IO uint32_t delayReg;

  __IO uint32_t msNum = Delay/1000;
  __IO uint32_t usNum = (uint32_t)((Delay%1000)*usDelayBase);

  if(msNum>0) HAL_Delay(msNum);

  delayReg = 0;
  while(delayReg!=usNum) delayReg++;
}

void MYDelay_ms(uint32_t delay)
{
	for(uint32_t i=0;i<delay;i++)
	{
		PY_Delay_us_t(500);
		PY_Delay_us_t(500);
	}
}

void delay_ms(uint16_t time)
{
	uint16_t i=0;
	while(time--)
	{
		i=12000;
		while(i--);
	}
}
