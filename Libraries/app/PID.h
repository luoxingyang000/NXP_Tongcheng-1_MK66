#ifndef __PID_H_
#define __PID_H_

#include "headfile.h"

typedef struct
{
	float setpoint;//设定值
	float propertiongain;//比例系数
	float integrlgain;//积分系数
	float derivativegain;//微分系数
	float lasterror;//前一拍偏差
	float preerror;//前两拍偏差
	float deadband;//死区
	float result;//输出值
	float epilson;//偏差检测阈值
}PID;
static uint16_t BetaGeneration(float error,float epsilon);
void PIDgegulation(PID *vPID,float processValue);
void PID_init(void);
void PID_fun(void);

#endif
