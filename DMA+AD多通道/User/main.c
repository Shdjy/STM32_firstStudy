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
		OLED_ShowNum(1, 5, AD_Value[0], 4);		//��ʾת�������0������
		OLED_ShowNum(2, 5, AD_Value[1], 4);		//��ʾת�������1������
		OLED_ShowNum(3, 5, AD_Value[2], 4);		//��ʾת�������2������
		OLED_ShowNum(4, 5, AD_Value[3], 4);		//��ʾת�������3������
		
		Delay_ms(100);							//��ʱ100ms���ֶ�����һЩת���ļ��ʱ��
	}
}
