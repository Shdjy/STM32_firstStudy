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
uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t i, Byte = 0x00;
	MyI2C_W_SDA(1);
	for(i = 0; i < 8; i++)
	{
		MyI2C_W_SCL(1);
		if(MyI2C_R_SDA() == 1)
		{
			Byte |= (0x80 >> i);
		}
		MyI2C_W_SCL(0);
	}
	
	return Byte;
}


//����Ӧ�������ڽ�����һ���ֽ�֮������һ��ʱ�ӷ���һλ���ݣ�
//����0��ʾӦ������1��ʾ��Ӧ��
void MyI2C_SendAck(uint8_t AckBit)
{
	MyI2C_W_SDA(AckBit);					//������Ӧ��λ���ݷŵ�SDA��
	MyI2C_W_SCL(1);							//�ͷ�SCL���ӻ���SCL�ߵ�ƽ�ڼ䣬��ȡӦ��λ
	MyI2C_W_SCL(0);
}

//����Ӧ�������ڷ�����һ���ֽ�֮������һ��ʱ�ӽ���һλ���ݣ��жϴӻ��Ƿ�Ӧ��
//����0��ʾӦ������1��ʾ��Ӧ�������ڽ���֮ǰ����Ҫ�ͷ�SDA��

uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t AckBit;							//����Ӧ��λ����
	MyI2C_W_SDA(1);							//����ǰ��������ȷ���ͷ�SDA��������Ŵӻ������ݷ���
	MyI2C_W_SCL(1);							//�ͷ�SCL����������SCL�ߵ�ƽ�ڼ��ȡSDA
	AckBit = MyI2C_R_SDA();					//��Ӧ��λ�洢��������
	MyI2C_W_SCL(0);							//����SCL����ʼ��һ��ʱ��ģ��
	return AckBit;	
}
