#ifndef _PID_H_
#define _PID_H_

struct _pid {
	float setspeed;     //�趨ֵ
	float actualspeed;  //ʵ��ֵ
	float err;          //��ǰ���
	float err_last;     //��һ�����
	float Kp,Ki,Kd;
	float voltage;      //����ִ�����ı���
	float integral;     //������ֵ
	float umax, umin;
};

void PID_init(void);
float PID_realize(void);


#endif // _PID_H_
