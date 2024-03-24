#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

void Serial_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//开启GPIOA的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//将PA1和PA2引脚初始化为推挽输出
	
	
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 9600;//波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件控制流，不使用
	USART_InitStruct.USART_Mode = USART_Mode_Tx;//发送模式
	USART_InitStruct.USART_Parity = USART_Parity_No;//奇偶校验，不需要
	USART_InitStruct.USART_StopBits = USART_StopBits_1;	//停止位，选择1位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;		//字长，选择8位
	
	USART_Init(USART1, &USART_InitStruct);
	
	USART_Cmd(USART1, ENABLE);//使能
}

void Serial_SendByte(uint8_t byte)
{
	USART_SendData(USART1, byte);//发送数据
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//等待发送完成
	/*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/
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
  * 函    数：使用printf需要重定向的底层函数
  * 参    数：保持原始格式即可，无需变动
  * 返 回 值：保持原始格式即可，无需变动
  */
int fputc(int ch, FILE *f)
{
	
	Serial_SendByte(ch);			//将printf的底层重定向到自己的发送字节函数
	return ch;
}

/**
  * 函    数：自己封装的prinf函数
  * 参    数：format 格式化字符串
  * 参    数：... 可变的参数列表
  * 返 回 值：无
  */
void Serial_Printf(char *format, ...)
{
	char String[100];				//定义字符数组
	va_list arg;					//定义可变参数列表数据类型的变量arg
	va_start(arg, format);			//从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);	//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);					//结束变量arg
	Serial_SendString(String);		//串口发送字符数组（字符串）
}
