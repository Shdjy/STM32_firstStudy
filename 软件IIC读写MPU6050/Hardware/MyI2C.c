#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//引脚配置

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


//I2C初始化
void MyI2C_Init(void)
{
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//初始化，引脚初始化为开漏输出
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//设置默认电平，I2C时序起始条件都为高电平
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}

//I2C协议层

//I2C起始,起始条件：SCL高电平期间，SDA从高电平切换到低电平
void MyI2C_Start(void)
{
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}

//终止条件：SCL高电平期间，SDA从低电平切换到高电平
void MyI2C_Stop(void)
{
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}

//发送一个字节：SCL低电平期间，主机将数据位依次放到SDA线上（高位先行），
//然后释放SCL，从机将在SCL高电平期间读取数据位，所以SCL高电平期间SDA不允许有数据变化，
//依次循环上述过程8次，即可发送一个字节
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

//接收一个字节：SCL低电平期间，从机将数据位依次放到SDA线上（高位先行），
//然后释放SCL，主机将在SCL高电平期间读取数据位，所以SCL高电平期间SDA不允许有数据变化，
//依次循环上述过程8次，即可接收一个字节（主机在接收之前，需要释放SDA）
