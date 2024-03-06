#include "stm32f10x.h" 



void Buzzer_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//����GPIOC��ʱ��
															//ʹ�ø�������ǰ���뿪��ʱ�ӣ����������Ĳ�����Ч
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;					//����ṹ�����
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//GPIOģʽ����ֵΪ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				//GPIO���ţ���ֵΪ��13������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//GPIO�ٶȣ���ֵΪ50MHz
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
}	

void Buzzer_No(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
}

void Buzzer_Off(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
}
