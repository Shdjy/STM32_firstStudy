#ifndef __TIMER_H
#define __TIMER_H

void Timer_Init(void);
uint16_t TIM_GetCount(void);

#endif

/*��ʱ����ʼ��
����RCC��ʱ��Դѡ��ʱ����Ԫ���ж�������ã� NVIC�жϷ��飬 NVIC��ʼ���� TIMʹ��*/