#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"
#include "Motor.h"

int8_t speed;
uint16_t KeyNum;

int main(void)
{
	Motor_Init();
	OLED_Init();
	Key_Init();
	
	OLED_ShowString(1, 1, "speed");
	Motor_SetSpeed(0);
	while (1)
	{
		KeyNum = Key_GetNum();
		if(KeyNum == 1)
		{
			speed += 20;
			if(speed > 100)
			{
				speed = 0;
			}
			Motor_SetSpeed(speed);
		}
		if(KeyNum == 2)
		{
			Motor_SetSpeed(0);
		}
		
		OLED_ShowNum(2, 1, speed, 3);
	}
}
