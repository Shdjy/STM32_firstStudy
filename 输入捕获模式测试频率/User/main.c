#include "stm32f10x.h"                  // Device header
#include "IC.h"
#include "OLED.h"
#include "PWM.h"

uint16_t i;

int main(void)
{
	PWM_Init();
	IC_Init();
	OLED_Init();
	
	OLED_ShowString(1, 1, "Freq:00000Hz");
	
	i = 720 - 1;
	PWM_SetPrescaler(i); 	           //PWMÆµÂÊFreq = 72M / (PSC + 1) / 100
	i = 50;
	PWM_SetCompare1(i);                 //PWMÕ¼¿Õ±ÈDuty = CCR / 100
	
	while (1)
	{
		OLED_ShowNum(1, 6, IC_GetFreq(), 5);
			
	}
}
