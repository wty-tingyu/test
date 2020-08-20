#ifndef _LED_H_
#define _LED_H_
#include "sys.h"

void LEDinit(void);

#define LED0 PBout(5)// PB5
#define LED1 PEout(5)// PE5	

#endif 
