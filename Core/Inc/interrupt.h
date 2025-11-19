#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "init.h"
#include "stm32f4xx.h"  

extern volatile uint32_t global_counter;
extern volatile uint8_t states[4];

// Прототипы функций
void Interrupt_init(void);
void EXTI9_5_IRQHandler(void);
void SysTick_Handler(void);
void TIM1_UP_TIM10_IRQHandler(void);


#endif