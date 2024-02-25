#include "stm32f10x.h"                 
#include "Delay.h"
#include "LightSensor.h"
#include "OLED.h"
#include "Encoder.h"

int16_t Num;

int main(void)
{
	OLED_Init();
	Encoder_Init();
	
	OLED_ShowString(1, 1, "Num:");
	
	while (1)
	{
		Num += Get_Count();
		OLED_ShowSignedNum(2, 1, Num, 5);
	}
}
