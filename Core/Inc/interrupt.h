#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "init.h"
#include "stm32f4xx.h"  

extern volatile uint8_t states[4];

void TIM1_UP_TIM10_IRQHandler(void);

#endif