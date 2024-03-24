#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	OLED_ShowString(1, 1, "RxData:");
	Serial_Init();
	
	while (1)
	{
		if(Serial_GetRxFlag() == 1)
		{
			OLED_ShowHexNum(1, 8, Serial_GetRxData(), 2);
			Serial_SendString("YES");
		}
	}
}
