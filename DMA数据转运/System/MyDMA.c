#include "stm32f10x.h"  


uint16_t DMA_size;

void MyDMA_Init(uint32_t AddrA, uint32_t AddrB, uint16_t Size)
{
	DMA_size = Size;
	//开启时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	
	
	
	DMA_InitTypeDef DMA_InitStruct;
	DMA_InitStruct.DMA_PeripheralBaseAddr = AddrA;//外设基地址
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte ;//外设数据宽度
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable ;//是否自增
	DMA_InitStruct.DMA_BufferSize = Size;//转运数据大小
	DMA_InitStruct.DMA_MemoryBaseAddr = AddrB;//存储器基地址
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte ;//存储器数据宽度
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;//存储器到存储器，软件触发
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;//数据传输方向，选择外设到存储器
	
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;//正常模式
	
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;//优先级，中等
	
	DMA_Init(DMA1_Channel1, &DMA_InitStruct);
	
	//DMA_Cmd(DMA1_Channel1, ENABLE);
	
}

void MyDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1, DMA_size);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);	//等待DMA工作完成
	DMA_ClearFlag(DMA1_FLAG_TC1);						//清除工作完成标志位
	
}