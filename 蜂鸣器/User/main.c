#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"

int main(void)
{
	LED_Init();
	Key_Init();
	Buzzer_Init();
	LightSensor_Init();
	
	while (1)
	{
		switch(Key_GetNum())
		{
			case 1:
				LED_Turn();
			break;
			case 2:
				LED1_Off();
				LED0_Off();
				break;
			default:
				
				break;
		}
		
		if(LightSensor_GetState() == 1)
		{
			Buzzer_No();
		}
		else
		{
			Buzzer_Off();
		}
	}
}
