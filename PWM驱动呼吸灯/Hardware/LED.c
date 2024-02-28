#include "stm32f10x.h"  

void LED_Init(void)
{
	/*����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//����GPIOC��ʱ��
															//ʹ�ø�������ǰ���뿪��ʱ�ӣ����������Ĳ�����Ч
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;					//����ṹ�����
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//GPIOģʽ����ֵΪ�������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;				//GPIO���ţ���ֵΪ��13������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//GPIO�ٶȣ���ֵΪ50MHz
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//����ֵ��Ĺ���������ݸ�GPIO_Init����
															//�����ڲ����Զ����ݽṹ��Ĳ���������Ӧ�Ĵ���
															//ʵ��GPIOC�ĳ�ʼ��
	GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1);
}

void LED0_No(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
}

void LED0_Off(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
}

void LED1_No(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

void LED1_Off(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
}


void LED_Turn(void)
{
	if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1 | GPIO_Pin_0) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_0);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_0);
	}
}