#include "pid.h"
#include "key.h"
#include "timer.h"


struct _pid  pid;

void PID_init(void)  //PID��ʼ��
{
	pid.actualspeed = 0.0; //ʵ��ֵ
	pid.setspeed  = 0.0;   //�趨ֵ
	pid.err  = 0.0;        //���
	pid.err_last  = 0.0;   //�ϴ����
	pid.integral = 0.0; //����
	pid.voltage = 0.0;   //����ִ�����ı���
	pid.Kp = 2.0; //����ʱ��10
	pid.Ki = 0.1;
	pid.Kd = 0.1;
	pid.umax = 400;   //��������ֵ(δ���㣩
	pid.umin = 0;     //�������Сֵ
}





float PID_realize(void)
{
	float t;
	pid.setspeed = setspeed; //����������趨ֵ
  pid.actualspeed = backspeed; //���������ص��ٶ�ֵ
	pid.err = pid.setspeed - pid.actualspeed;
	
	if((pid.err < 20)&&(pid.err > -20))    //���ַ��루ֵδ���㣩
	{
		pid.integral = pid.integral + pid.err;
	}
	
	pid.voltage = pid.Kp*pid.err + pid.Ki*pid.integral + pid.Kd*(pid.err-pid.err_last);
	pid.err_last = pid.err;
	t=pid.voltage;
	
	if(pid.actualspeed>pid.umax)			pid.actualspeed=pid.umax;  //���������ȡֵ��Χ
	else if(pid.actualspeed<pid.umin)	pid.actualspeed=-pid.actualspeed;

	return t;  //����PWM����ռ�ձ�
}

