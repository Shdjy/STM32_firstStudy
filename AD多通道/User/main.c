#include "stm32f10x.h"                  // Device header
#include "AD.h"
#include "OLED.h"

int main(void)
{
	OLED_Init();
	AD_Init();
	
	OLED_ShowString(1, 1, "AD1:");
	OLED_ShowString(2, 1, "AD2:");
	OLED_ShowString(3, 1, "AD3:");
	OLED_ShowString(4, 1, "AD4:");
	
	while (1)
	{
		OLED_ShowNum(1, 5, AD_GetValue(ADC_Channel_0), 5);
		OLED_ShowNum(2, 5, AD_GetValue(ADC_Channel_1), 5);
		OLED_ShowNum(3, 5, AD_GetValue(ADC_Channel_2), 5);
		OLED_ShowNum(4, 5, AD_GetValue(ADC_Channel_3), 5);
	}
}
