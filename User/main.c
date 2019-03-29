#include "headfile.h"


int main(void)
{
    get_clk();//上电后获取各个频率信息  				
	 
		OLED_Init();//OLED初始化，SCL-PTC10，SDA-PTC11
		OLED_Test();	//OLED自检
	
		EnableInterrupts;	
	
	while(1)
	{			
		
	}
	
}
