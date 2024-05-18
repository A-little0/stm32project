#ifndef   __DELAY_H
#define   __DELAY_H

#include "main.h"

void PY_usDelayTest(void);
void PY_Delay_us_t(uint32_t Delay);

void PY_usDelayOptimize(void);
void PY_Delay_us(uint32_t Delay);
void MYDelay_ms(uint32_t delay);
void delay_ms(uint16_t time);
#endif
