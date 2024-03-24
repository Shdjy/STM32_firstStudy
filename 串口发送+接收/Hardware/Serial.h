#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

void Serial_Init(void);
void Serial_SendByte(uint8_t byte);
void Serial_SendArry(uint8_t *Arry, uint16_t length);
void Serial_SendString(char* string);
void Serial_SendNum(uint32_t num, int16_t length);
void Serial_Printf(char *format, ...);

uint8_t Serial_GetRxFlag();
uint8_t Serial_GetRxData();

#endif
