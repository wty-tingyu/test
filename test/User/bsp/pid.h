#ifndef _PID_H_
#define _PID_H_

struct _pid {
	float setspeed;     //设定值
	float actualspeed;  //实际值
	float err;          //当前误差
	float err_last;     //上一次误差
	float Kp,Ki,Kd;
	float voltage;      //控制执行器的变量
	float integral;     //误差积分值
	float umax, umin;
};

void PID_init(void);
float PID_realize(void);


#endif // _PID_H_
