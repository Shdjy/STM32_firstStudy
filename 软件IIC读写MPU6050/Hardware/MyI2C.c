#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//��������

void MyI2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);
	Delay_us(10);
}

void MyI2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
	Delay_us(10);
}

uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
	Delay_us(10);
	return BitValue;
}


//I2C��ʼ��
void MyI2C_Init(void)
{
	//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//��ʼ�������ų�ʼ��Ϊ��©���
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//����Ĭ�ϵ�ƽ��I2Cʱ����ʼ������Ϊ�ߵ�ƽ
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}

//I2CЭ���

//I2C��ʼ,��ʼ������SCL�ߵ�ƽ�ڼ䣬SDA�Ӹߵ�ƽ�л����͵�ƽ
void MyI2C_Start(void)
{
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}

//��ֹ������SCL�ߵ�ƽ�ڼ䣬SDA�ӵ͵�ƽ�л����ߵ�ƽ
void MyI2C_Stop(void)
{
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}

//����һ���ֽڣ�SCL�͵�ƽ�ڼ䣬����������λ���ηŵ�SDA���ϣ���λ���У���
//Ȼ���ͷ�SCL���ӻ�����SCL�ߵ�ƽ�ڼ��ȡ����λ������SCL�ߵ�ƽ�ڼ�SDA�����������ݱ仯��
//����ѭ����������8�Σ����ɷ���һ���ֽ�
void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for(i = 0; i < 8; i++)
	{
		MyI2C_W_SDA(Byte & (0x80 >> i));
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
	}
}

//����һ���ֽڣ�SCL�͵�ƽ�ڼ䣬�ӻ�������λ���ηŵ�SDA���ϣ���λ���У���
//Ȼ���ͷ�SCL����������SCL�ߵ�ƽ�ڼ��ȡ����λ������SCL�ߵ�ƽ�ڼ�SDA�����������ݱ仯��
//����ѭ����������8�Σ����ɽ���һ���ֽڣ������ڽ���֮ǰ����Ҫ�ͷ�SDA��
