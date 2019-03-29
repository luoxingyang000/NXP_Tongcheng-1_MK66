<<<<<<< HEAD
 #include "headfile.h"
=======
#include "headfile.h"
>>>>>>> c5d0ae82c83cdf50d6176d8ce485a6e790ada189


int main(void)
{
    get_clk();//上电后获取各个频率信息  				
	 
<<<<<<< HEAD
		OLED_Init();//OLED初始化
	OLED_P8x16Str(28,0,"ULuoK66");
	OLED_P6x8Str(2,3,"LQ MK66FX Suite");
	OLED_P6x8Str(2,4,"Onboard OLED");
	OLED_P6x8Str(96,6,"OK!");
=======
		OLED_Init();//OLED初始化，SCL-PTC10，SDA-PTC11
		OLED_Test();	//OLED自检
>>>>>>> c5d0ae82c83cdf50d6176d8ce485a6e790ada189
	
		EnableInterrupts;	
	
	while(1)
	{			
		
	}
	
}
