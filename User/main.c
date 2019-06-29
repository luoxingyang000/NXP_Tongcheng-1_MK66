#include "headfile.h"
#include "isr.h"

uint8 i;
uint16 i2cn = 400;
unsigned int sysTickCnt[10],sysTH[10];	//SysTick伪线程时基同步 & 线程附加参数
uint16 vdata1,vdata2;	//电机速度反馈
uint16 vlspeed,vrspeed,vdir;	//左右电机占空比与行进方向
uint8 adcdat[4];	//电磁读数
uint16 sonicValH,sonicValL,sonicVal;	//GYUS42超声波读数


int main(void)
{
	get_clk();//上电后获取各个频率信息  				
	sysInit();	//系统外设初始化
	EnableInterrupts;	

	while(1)
	{	
		//线程0——SysTick心跳指示PA17，30ms/1S
		if(sysTickCnt[0]>=(sysTH[0]-5) && sysTickCnt[0]<=(sysTH[0]+5))
		{
			sysTickCnt[0]=0;//
			if(sysTH[0]==30) {sysTH[0]=970; gpio_set(A17,1);}	//闪烁
			else {sysTH[0]=30; gpio_set(A17,0);}	//熄灭
		}
		
		//线程1——I2C读取GYUS42超声波测距
		if(sysTickCnt[1]>=(sysTH[1]-5) && sysTickCnt[1]<=(sysTH[1]+5))
		{
			sysTickCnt[1]=0;//
			if(sysTH[1]==60)	//读取1次距离值
			{
				sysTH[1]=100;
				sonicVal = i2c_sonic_get();
			}
			else{	//启动一次测量
				sysTH[1]=60;	//waitFor60ms
				i2c_sonic_trig();
			}
		}
		
		
		
		adcdat[0] = adc_once(ADC1_SE10,ADC_8bit);
		adcdat[1] = adc_once(ADC1_SE11,ADC_8bit);
		adcdat[2] = adc_once(ADC1_SE12,ADC_8bit);
		adcdat[3] = adc_once(ADC1_SE13,ADC_8bit);	//ADC电磁赛道数据采集，范围0-255
		
		OLED_Print_Num1(0, 0, vdata1);
		OLED_Print_Num1(48, 0, vdata2);	//OLED显示编码器采集速度值
		OLED_Print_Num1(0, 2, adcdat[0]);
		OLED_Print_Num1(48, 2, adcdat[1]);
		OLED_Print_Num1(0, 3, adcdat[2]);
		OLED_Print_Num1(48, 3, adcdat[3]);	//OLED显示ADC电磁赛道值
		OLED_Print_Num1(0, 5, sonicVal);	//OLED显示距离
		
		PID_fun();
		
		vchange(999, 999, 1);

	}
}

void vchange(uint16 vleft, uint16 vright, uint16 dir)	//双电机方向与占空比控制99.9&精度
{
	if(vleft>999) {vleft=999;}
	if(vright>999) {vright=999;}
	
	if(dir==0)	//反转
	{
		//LR电机A通道使能
		ftm_pwm_duty(ftm0,ftm_ch0,vleft);
		ftm_pwm_duty(ftm0,ftm_ch1,1);
		ftm_pwm_duty(ftm0,ftm_ch2,vright);
		ftm_pwm_duty(ftm0,ftm_ch3,1);
	}else
	{
		//LR电机B通道使能
		ftm_pwm_duty(ftm0,ftm_ch0,1);
		ftm_pwm_duty(ftm0,ftm_ch1,vleft);
		ftm_pwm_duty(ftm0,ftm_ch2,1);
		ftm_pwm_duty(ftm0,ftm_ch3,vright);
	}
}

void sysInit()
{
	//OLED初始化
	OLED_Init();
	OLED_P8x16Str(28,0,"ULuoK66");
	OLED_P6x8Str(2,3,"LQ MK66FX Suite");
	OLED_P6x8Str(2,4,"Onboard OLED");
	systick_delay_ms(500);
	OLED_P6x8Str(96,6,"OK!");
	systick_delay_ms(1000);
	OLED_Fill(0);
	
	//ADC初始化
	adc_init(ADC1_SE10);	//B4
	adc_init(ADC1_SE11);	//B5
	adc_init(ADC1_SE12);	//B6
	adc_init(ADC1_SE13);	//B7
	adc_init(ADC0_SE8);	//B0输入电压检测
	
	//FTM初始化
	ftm_pwm_init(ftm0,ftm_ch0,15000,0);	//C1-L电机A相
	ftm_pwm_init(ftm0,ftm_ch1,15000,0);	//C2-L电机B相
	ftm_pwm_init(ftm0,ftm_ch2,15000,0);	//C3-R电机A相
	ftm_pwm_init(ftm0,ftm_ch3,15000,0);	//C4-R电机B相
	ftm_quad_init(ftm1);//初始化ftm1为正交解码
	ftm_quad_init(ftm2);//初始化ftm2为正交解码
	ftm_pwm_init(ftm3,ftm_ch6,15000,0);	//C10舵机回馈信号
	
	//Cortex-M4 SysTick时基初始化 
	SysTick->LOAD=(unsigned long)180000;	//设定初始值1ms@180MHz
	SysTick->VAL=0;	//清空计数器
	SysTick->CTRL=SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk;	//来源内核时钟/TICKINT/计时使能
	NVIC_SetPriority(SysTick_IRQn,1);
	for(i=0;i<(sizeof(sysTickCnt)/sizeof(sysTickCnt[0]));i++)
	{sysTickCnt[i]=0;}	//复位线程计时
	
	sysTH[0]=30; sysTH[1]=500;	//线程参数初始化
	
	//GPIO初始化
	gpio_init(A17,GPO,1);
	gpio_init(C6,GPI,0);	//电容充电完成信号
	gpio_init(D15,GPO,0);	//电容充电完成指示(熄灭有效)
	
	//I2C初始化
	i2c_init(i2c1, 400*1000);	//Init
	
	//UART串口初始化
}
