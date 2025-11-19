#ifndef INIT_H

/*Настроить таймер в режиме энкодера.
При зависимости от положения ручки потенциометра включается
определенное число светодиодов.
20% - 1 светодиод,
40% - 2 светодиода,
60% - 3 светодиода,
80% - 4 светодиода.
При других положениях светодиоды не горят. */

#include "stm32f4xx.h"
#include "stm32f429xx.h"
#include "stdio.h"

void GPIO_init(void);

void RCC_init(void);

void Interrupt_init(void);

void systick_init(void);

void TIM1_init(void);

void ADC_init(void);

uint32_t READ_POT(void);

#endif