#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "timer.h"
#include "pid.h"
u16 pwmval = 0;

 int main(void)
 {	


	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	LEDinit();			     //LED端口初始化
	KEY_init();
	EXTIX_Init();
	PID_init();
	control();

	PWM_init(899,0);	 //不分频。PWM频率=72000000/900=80Khz					 
  while(1)
  {
		pwmval = PID_realize(); //实际输出的占空比
	  LCD_show();
		if(setspeed>0)
		{
		IN1 = 0;
		IN2 = 1;
	  TIM_SetCompare2(TIM3,pwmval);	//PB5 
    }
		if(setspeed<0)
		{
			IN1 = 1;
			IN2 = 0;
			TIM_SetCompare1(TIM3,pwmval);	//PB5 
		}
		if(setspeed==0)
		{
			IN1 = 0;
			IN2 = 0;
		}
	}
 }

 