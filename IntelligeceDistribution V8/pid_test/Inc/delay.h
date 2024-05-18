#ifndef   __DELAY_H
#define   __DELAY_H

#include "main.h"

void PY_usDelayTest(void);
void PY_Delay_us_t(uint32_t Delay);

void PY_usDelayOptimize(void);
void PY_Delay_us(uint32_t Delay);

void user_delay_us(uint16_t us);
void user_delay_ms(uint16_t ms);
void nop_delay_us(uint16_t us);
void nop_delay_ms(uint16_t ms);
#endif
