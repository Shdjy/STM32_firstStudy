#include "stm32f10x.h"  

void LED_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOC的时钟
															//使用各个外设前必须开启时钟，否则对外设的操作无效
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;					//定义结构体变量
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//GPIO模式，赋值为推挽输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;				//GPIO引脚，赋值为第13号引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//GPIO速度，赋值为50MHz
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将赋值后的构体变量传递给GPIO_Init函数
															//函数内部会自动根据结构体的参数配置相应寄存器
															//实现GPIOC的初始化
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