#ifndef __TIMER_H
#define __TIMER_H

void Timer_Init(void);
uint16_t TIM_GetCount(void);

#endif

/*定时器初始化
开启RCC，时钟源选择，时基单元，中断输出配置， NVIC中断分组， NVIC初始化， TIM使能*/