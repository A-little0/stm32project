#ifndef __KEY_H
#define __KEY_H
#include "gpio.h"

#define key_rangedowm 0
#define key_rangeup   1
#define key_mandowm   2
#define key_manup     3

extern int target_range;//目标射程(200-300)
extern int magnification;//倍率

extern int key_state[4];

void Key_Init(void);
void Key_Start(void);
#endif
