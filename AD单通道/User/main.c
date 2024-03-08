#include "stm32f10x.h"                  // Device header
#include "AD.h"
#include "OLED.h"

int main(void)
{
	OLED_Init();
	AD_Init();
	
	OLED_ShowString(1, 1, "ADValue:");
	
	while (1)
	{
		OLED_ShowNum(2, 1, AD_GetValue(), 5);
	}
}
