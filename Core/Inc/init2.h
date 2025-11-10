#ifndef INIT2_H

#include "stm32f4xx.h"
#include "stm32f429xx.h"
#include "stdio.h"

#define TIME_DELAY_2SEC   2000 // при частоте тактирования 1000 Гц
#define TIME_DELAY_50MS   50

void GPIO_init(void);

void RCC_init(void);

void Interrupt_init(void);

void systick_init(void);

#endif