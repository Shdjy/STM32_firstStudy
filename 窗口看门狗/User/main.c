#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"

int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	Key_Init();
	
	OLED_ShowString(1, 1, "WWDG TEST!");

	if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET)
	{
		OLED_ShowString(2, 1, "WWDG REST");
		Delay_ms(500);
		OLED_ShowString(2, 1, "         ");
		Delay_ms(200);
		
		RCC_ClearFlag();//清除标志位
	}
	else
	{
		OLED_ShowString(2, 1, "REST");
		Delay_ms(500);
		OLED_ShowString(2, 1, "    ");
		Delay_ms(200);
	}
	
	//窗口看门狗初始化
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);//开启时钟
		/*WWDG初始化*/
	WWDG_SetPrescaler(WWDG_Prescaler_8);			//设置预分频为8
	WWDG_SetWindowValue(0x40 | 21);					//设置窗口值，窗口时间为30ms
	WWDG_Enable(0x40 | 54);							//使能并第一次喂狗，超时时间为50ms
	
	
	while (1)
	{
		Key_GetNum();
		 
		OLED_ShowString(4, 1, "FEED");				//OLED闪烁FEED字符串
		Delay_ms(20);								//喂狗间隔为200+600=800ms
		OLED_ShowString(4, 1, "    ");
		Delay_ms(20);
		
		WWDG_SetCounter(0x40 | 54);					//重装计数器，喂狗
	}
}
