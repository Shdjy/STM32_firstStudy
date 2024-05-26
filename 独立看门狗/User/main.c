#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"

int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	Key_Init();
	
	OLED_ShowString(1, 1, "IWDG TEST!");

	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET)
	{
		OLED_ShowString(2, 1, "IWDG REST");
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
	
	//独立看门狗初始化
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//写使能
	IWDG_SetPrescaler(IWDG_Prescaler_16);//16分频
	IWDG_SetReload(2499);//设置重装值
	IWDG_ReloadCounter();//重装技术去，喂狗
	IWDG_Enable();//使能
	
	while (1)
	{
		Key_GetNum();
		
		IWDG_ReloadCounter();
		
		OLED_ShowString(4, 1, "FEED");				//OLED闪烁FEED字符串
		Delay_ms(200);								//喂狗间隔为200+600=800ms
		OLED_ShowString(4, 1, "    ");
		Delay_ms(600);
	}
}
