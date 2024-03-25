#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "Key.h"

int main(void)
{
	/*模块初始化*/
	OLED_Init();		
	Key_Init();
	Serial_Init();
	
	OLED_ShowString(1, 1, "RxData:");
	OLED_ShowString(3, 1, "TxData:");
	
	TxData[0] = 0x01;
	TxData[1] = 0x01;
	TxData[2] = 0x01;
	TxData[3] = 0x01;
	
	Serial_SendPacket();

	
	while (1)
	{
		if(Serial_GetRxFlag() == 1)
		{
			OLED_ShowHexNum(2, 1, RxPacket[0], 2);
			OLED_ShowHexNum(2, 4, RxPacket[0], 2);
			OLED_ShowHexNum(2, 7, RxPacket[0], 2);
			OLED_ShowHexNum(2, 10, RxPacket[0], 2);
		}
	}
}
