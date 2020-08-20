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


	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	LEDinit();			     //LED�˿ڳ�ʼ��
	KEY_init();
	EXTIX_Init();
	PID_init();
	control();

	PWM_init(899,0);	 //����Ƶ��PWMƵ��=72000000/900=80Khz					 
  while(1)
  {
		pwmval = PID_realize(); //ʵ�������ռ�ձ�
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

 