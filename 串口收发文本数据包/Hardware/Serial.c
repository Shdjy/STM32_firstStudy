#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>


char Serial_RxString[50];
uint8_t Serial_RxFlag;

void Serial_Init(void)
{
	/*����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//����GPIOA��ʱ��
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//���Ϳ�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//���տ�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						
	
	
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 9600;//������
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Ӳ������������ʹ��
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//����ģʽ
	USART_InitStruct.USART_Parity = USART_Parity_No;//��żУ�飬����Ҫ
	USART_InitStruct.USART_StopBits = USART_StopBits_1;	//ֹͣλ��ѡ��1λ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;		//�ֳ���ѡ��8λ
	
	USART_Init(USART1, &USART_InitStruct);
	
	//�ж�����
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	/*NVIC�жϷ���*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//����NVICΪ����2
	
	/*NVIC����*/
	NVIC_InitTypeDef NVIC_InitStructure;					//����ṹ�����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//ѡ������NVIC��USART1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//ָ��NVIC��·����ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//ָ��NVIC��·����Ӧ���ȼ�Ϊ1
	NVIC_Init(&NVIC_InitStructure);						
	
	USART_Cmd(USART1, ENABLE);//ʹ��
}

void Serial_SendByte(uint8_t byte)
{
	USART_SendData(USART1, byte);//��������
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//�ȴ��������
	/*�´�д�����ݼĴ������Զ����������ɱ�־λ���ʴ�ѭ�������������־λ*/
}

void Serial_SendArry(uint8_t *Arry, uint16_t length)
{
	uint16_t i;
	for(i = 0; i < length; i++)
	{
		Serial_SendByte(Arry[i]);
	}
}

void Serial_SendString(char* string)
{
	uint16_t i;
	for(i = 0; string[i] != '\0'; i++)
	{
		Serial_SendByte(string[i]);
	}
}

uint32_t Serial_Pow(uint16_t x, uint16_t y)
{
	uint32_t res = 1;
	while(y--)
	{
		res *= x;
	}
	return res;
}

void Serial_SendNum(uint32_t num, int16_t length)
{
	uint16_t i;
	for(i = 0; i < length; i++)
	{
		Serial_SendByte(num / Serial_Pow(10, length - i -1) % 10 + '0');
	}
}

/**
  * ��    ����ʹ��printf��Ҫ�ض���ĵײ㺯��
  * ��    ��������ԭʼ��ʽ���ɣ�����䶯
  * �� �� ֵ������ԭʼ��ʽ���ɣ�����䶯
  */
int fputc(int ch, FILE *f)
{
	
	Serial_SendByte(ch);			//��printf�ĵײ��ض����Լ��ķ����ֽں���
	return ch;
}

/**
  * ��    �����Լ���װ��prinf����
  * ��    ����format ��ʽ���ַ���
  * ��    ����... �ɱ�Ĳ����б�
  * �� �� ֵ����
  */
void Serial_Printf(char *format, ...)
{
	char String[100];				//�����ַ�����
	va_list arg;					//����ɱ�����б��������͵ı���arg
	va_start(arg, format);			//��format��ʼ�����ղ����б�arg����
	vsprintf(String, format, arg);	//ʹ��vsprintf��ӡ��ʽ���ַ����Ͳ����б��ַ�������
	va_end(arg);					//��������arg
	Serial_SendString(String);		//���ڷ����ַ����飨�ַ�����
}

uint8_t Serial_GetRxFlag()
{
	if(Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}


/**
  * ��    ����USART1�жϺ���
  * ��    ������
  * �� �� ֵ����
  * ע������˺���Ϊ�жϺ�����������ã��жϴ������Զ�ִ��
  *           ������ΪԤ����ָ�����ƣ����Դ������ļ�����
  *           ��ȷ����������ȷ���������κβ��죬�����жϺ��������ܽ���
  */
void USART1_IRQHandler(void)
{
	static uint8_t RxSatate = 0;
	static uint8_t RxNum = 0;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)		//�ж��Ƿ���USART1�Ľ����¼��������ж�
	{
		uint8_t Serial_RxData = USART_ReceiveData(USART1);				//��ȡ���ݼĴ���������ڽ��յ����ݱ���
		switch(RxSatate)
		{
			case 0:
				if(Serial_RxData == '@' && Serial_RxFlag == 0)
				{
					RxSatate = 1;
					RxNum = 0;
					memset(Serial_RxString, 0, sizeof(Serial_RxString));
				}
				break;
			case 1:
				if(Serial_RxData == '\r')
				{
					RxSatate = 2;
				}
				else
				{
					Serial_RxString[RxNum] = Serial_RxData;
					RxNum++;
				}
				break;
			case 2:
				if(Serial_RxData == '\n')
				{
					RxSatate = 0;
					Serial_RxString[RxNum] = '\0';
					Serial_RxFlag = 1;	
				}
				break;
			}
			
		}
											
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);			//���USART1��RXNE��־λ
																//��ȡ���ݼĴ������Զ�����˱�־λ
																//����Ѿ���ȡ�����ݼĴ�����Ҳ���Բ�ִ�д˴���
}