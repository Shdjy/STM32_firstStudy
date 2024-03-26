#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"

int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	Serial_Init();
	LED_Init();
	OLED_ShowString(1, 1, "RxData:");
	OLED_ShowString(3, 1, "TxData:");
	
	while (1)
	{
		if(Serial_GetRxFlag() == 1)
		{
			if(strcmp(Serial_RxString, "LED_ON") == 0)
			{
				Serial_SendString("LED_OPEN\r\n");
				OLED_ShowString(2, 1, "LED_ON");
				OLED_ShowString(4, 1, "LED_OPEN");
				LED1_ON();
			}
			else if(strcmp(Serial_RxString, "LED_OFF") == 0)
			{
				Serial_SendString("LED_CLOSE\r\n");
				OLED_ShowString(2, 1, "LED_OFF");
				OLED_ShowString(4, 1, "LED_CLOSE");
				LED1_OFF();
			}
			else
			{
				Serial_SendString("ERROR_COMMAND\r\n");
				OLED_ShowString(2, 1, "ERROR_COMMAND");
			}
		}
	}
}
