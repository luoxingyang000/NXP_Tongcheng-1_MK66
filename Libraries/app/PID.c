#include "PID.h"
#include "math.h"
//---------------------------------------------------------------------------------------------------------------------------������ȫ��䣡������������
static uint16_t BetaGeneration(float error,float epsilon)
{
	uint16_t beta = 0;
	if(fabs(error) <= epsilon)
	{
		beta = 1;
	}
	return beta;
}

void PIDgegulation(PID *vPID,float processValue)//PID������
{
	float thisError;
	float increment;
	float pError,dError,iError;
	uint16_t beta;
	
	thisError = vPID->setpoint-processValue;//�õ�ƫ��ֵ
	pError = thisError-vPID->lasterror;
	iError = thisError;
	dError = thisError-2*(vPID->lasterror)+vPID->preerror;

	beta = BetaGeneration(thisError,vPID->epilson);
	if(beta > 0)
	{
		increment= vPID->propertiongain*pError+vPID->derivativegain*dError;//��������
	}
	else
	{
		increment= vPID->propertiongain*pError+vPID->integrlgain*iError+vPID->derivativegain*dError;//��������
	}
	vPID->preerror=vPID->lasterror;//���ƫ������´μ���
	vPID->lasterror= thisError;
	vPID->result+=increment;
	if(vPID->result > 1000)vPID->result = 500;
	if(vPID->result < -1000)vPID->result = -500;
}
//-----------------------------------------------------------------------------------------------------------------------------
PID dianji_1;
PID dianji_2;
extern uint32_t date_1,date_2 ;

void PID_init()//��ʼ�����
{
//---------------------------------------------���1�ɰ���Сβ��	
		ftm_pwm_init(ftm0,ftm_ch0,15000,0);
		ftm_pwm_init(ftm0,ftm_ch1,15000,0);
//---------------------------------------------���2����¥�϶�������	
		ftm_pwm_init(ftm0,ftm_ch2,15000,0);
		ftm_pwm_init(ftm0,ftm_ch3,15000,0);
//---------------------------------------------PID������ĬĬ�Թ�	
		dianji_1.propertiongain = 1;//P
		dianji_1.integrlgain = 1;//I
		dianji_1.propertiongain = 1;//D
	
		dianji_2.propertiongain = 1;//P
		dianji_2.integrlgain = 1;//I
		dianji_2.propertiongain = 1;//D
	
		dianji_1.setpoint = 300;//����ֵ
		dianji_2.setpoint = 300;
	
		dianji_1.epilson = 300;//΢��������ֵ
		dianji_2.epilson = 300;//
}

void PID_fun()//���Ƶ��
{
		PIDgegulation(&dianji_1,date_1/1024/30);
		PIDgegulation(&dianji_2,date_2/1024/30);
//********************************************************	
		if(dianji_1.result < 0)															 
		{																									   
			ftm_pwm_duty(ftm0,ftm_ch0,fabs(dianji_1.result));
			ftm_pwm_duty(ftm0,ftm_ch1,0);
		}
		else
		{
			ftm_pwm_duty(ftm0,ftm_ch0,0);
			ftm_pwm_duty(ftm0,ftm_ch1,dianji_1.result);			
		}
//*********************************************************		
		if(dianji_2.result < 0)															 
		{																									   
			ftm_pwm_duty(ftm0,ftm_ch2,fabs(dianji_2.result));
			ftm_pwm_duty(ftm0,ftm_ch3,0);
		}
		else
		{
			ftm_pwm_duty(ftm0,ftm_ch2,0);
			ftm_pwm_duty(ftm0,ftm_ch3,dianji_2.result);			
		}
}


