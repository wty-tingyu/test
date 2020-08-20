#ifndef _KEY_H_
#define _KEY_H_
#include "sys.h"

float keycontrol(void);           
void KEY_init(void);
uint8_t KEY_scan(uint8_t);
void EXTIX_Init(void);
void control(void);

extern int setspeed;
extern u8 mood;
#define KEY0   GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)  //读取按键0
#define KEY1   GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)  //读取按键1
#define WK_UP  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)  //读取按键WK_UP

#define IN2  PAout(6)// PB5   
#define IN1  PAout(5)// PA5   	

#define KEY0_PRES 	1	//KEY0按下
#define KEY1_PRES	  2	//KEY1按下
#define WKUP_PRES   3	//WK_UP按下

#endif

