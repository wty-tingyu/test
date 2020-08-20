#include "pid.h"
#include "key.h"
#include "timer.h"


struct _pid  pid;

void PID_init(void)  //PID初始化
{
	pid.actualspeed = 0.0; //实际值
	pid.setspeed  = 0.0;   //设定值
	pid.err  = 0.0;        //误差
	pid.err_last  = 0.0;   //上次误差
	pid.integral = 0.0; //积分
	pid.voltage = 0.0;   //控制执行器的变量
	pid.Kp = 2.0; //计算时除10
	pid.Ki = 0.1;
	pid.Kd = 0.1;
	pid.umax = 400;   //输出的最大值(未计算）
	pid.umin = 0;     //输出的最小值
}





float PID_realize(void)
{
	float t;
	pid.setspeed = setspeed; //按键输入的设定值
  pid.actualspeed = backspeed; //编码器返回的速度值
	pid.err = pid.setspeed - pid.actualspeed;
	
	if((pid.err < 20)&&(pid.err > -20))    //积分分离（值未计算）
	{
		pid.integral = pid.integral + pid.err;
	}
	
	pid.voltage = pid.Kp*pid.err + pid.Ki*pid.integral + pid.Kd*(pid.err-pid.err_last);
	pid.err_last = pid.err;
	t=pid.voltage;
	
	if(pid.actualspeed>pid.umax)			pid.actualspeed=pid.umax;  //输出变量的取值范围
	else if(pid.actualspeed<pid.umin)	pid.actualspeed=-pid.actualspeed;

	return t;  //传给PWM调节占空比
}

