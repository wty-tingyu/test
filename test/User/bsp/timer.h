#ifndef _TIM3_H_
#define _TIM3_H_
#include "stm32f10x.h"

extern u16 backspeed;
void PWM_init(uint16_t arr,uint16_t psc);
int Read_Encoder(void);
#endif //_TIM3_H_

