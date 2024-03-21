#include "stm32f10x.h"  

uint16_t AD_Value[4];

void AD_Init(void)
{
	//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		//����DMA1��ʱ��
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//����adcʱ��
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//����ͨ������
	/*������ͨ������*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);	//����������1��λ�ã�����Ϊͨ��0
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);	//����������2��λ�ã�����Ϊͨ��1
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);	//����������3��λ�ã�����Ϊͨ��2
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);	//����������4��λ�ã�����Ϊͨ��3
	
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_NbrOfChannel = 4;
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;
	ADC_Init(ADC1, &ADC_InitStruct);
	
	//DMA��ʼ��
	DMA_InitTypeDef DMA_InitStructure;											//����ṹ�����
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;				//�������ַ�������β�AddrA
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//�������ݿ�ȣ�ѡ����֣���Ӧ16Ϊ��ADC���ݼĴ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//�����ַ������ѡ��ʧ�ܣ�ʼ����ADC���ݼĴ���ΪԴ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;					//�洢������ַ���������ADת�������ȫ������AD_Value
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//�洢�����ݿ�ȣ�ѡ����֣���Դ���ݿ�ȶ�Ӧ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//�洢����ַ������ѡ��ʹ�ܣ�ÿ��ת�˺������Ƶ���һ��λ��
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;							//���ݴ��䷽��ѡ�������赽�洢����ADC���ݼĴ���ת������
	DMA_InitStructure.DMA_BufferSize = 4;										//ת�˵����ݴ�С��ת�˴���������ADCͨ����һ��
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//ģʽ��ѡ��ѭ��ģʽ����ADC������ת��һ��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								//�洢�����洢����ѡ��ʧ�ܣ�������ADC���败��ת�˵��洢��
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;						//���ȼ���ѡ���е�
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);								//���ṹ���������DMA_Init������DMA1��ͨ��1
	
	/*DMA��ADCʹ��*/
	DMA_Cmd(DMA1_Channel1, ENABLE);							//DMA1��ͨ��1ʹ��
	ADC_DMACmd(ADC1, ENABLE);								//ADC1����DMA1���ź�ʹ��
	ADC_Cmd(ADC1, ENABLE);									//ADC1ʹ��
	
	//У׼
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) == SET);
	
	/*ADC����*/
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//�������ADC��ʼ����������ADC��������ת��ģʽ���ʴ���һ�κ�ADC�Ϳ���һֱ�������ϵع���
}

