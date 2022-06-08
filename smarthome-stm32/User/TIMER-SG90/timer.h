#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x.h"

/************�߼���ʱ��TIM�������壬ֻ��TIM2,3,4,5************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������ʹ�ø߼����ƶ�ʱ��TIM3

#define            GENERAL_TIM                   TIM3
#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK               RCC_APB1Periph_TIM3
// PWM �źŵ�Ƶ�� F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define            GENERAL_TIM_PERIOD            (20000-1)
#define            GENERAL_TIM_PSC               (72-1)


#define            BASIC_TIM_IRQ                 TIM3_IRQn
#define            BASIC_TIM_IRQHandler          TIM3_IRQHandler


// TIM3 ����Ƚ�ͨ��
#define            GENERAL_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM_CH1_PORT          GPIOA
#define            GENERAL_TIM_CH1_PIN           GPIO_Pin_6
#define            GENERAL_TIM_CH1_PULSE         0

void GENERAL_TIM_Init(void);


#endif





