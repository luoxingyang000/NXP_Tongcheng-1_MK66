 #include "headfile.h"


int main(void)
{
    get_clk();//上电后获取各个频率信息  				
	 
		OLED_Init();//OLED初始化
	OLED_P8x16Str(28,0,"ULuoK66");
	OLED_P6x8Str(2,3,"LQ MK66FX Suite");
	OLED_P6x8Str(2,4,"Onboard OLED");
	OLED_P6x8Str(96,6,"OK!");
	
		EnableInterrupts;	
	
	while(1)
	{			
		
	}
	
}
