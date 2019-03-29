#ifndef __PID_H_
#define __PID_H_

#include "headfile.h"

typedef struct
{
	float setpoint;//�趨ֵ
	float propertiongain;//����ϵ��
	float integrlgain;//����ϵ��
	float derivativegain;//΢��ϵ��
	float lasterror;//ǰһ��ƫ��
	float preerror;//ǰ����ƫ��
	float deadband;//����
	float result;//���ֵ
	float epilson;//ƫ������ֵ
}PID;
static uint16_t BetaGeneration(float error,float epsilon);
void PIDgegulation(PID *vPID,float processValue);
void PID_init(void);
void PID_fun(void);

#endif
