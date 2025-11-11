#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "init2.h"
#include "stm32f4xx.h"  // Добавьте этот include

// Объявление внешних переменных (без инициализации)
extern volatile uint32_t last_button_time_pressed;
extern volatile uint32_t last_button_time_unpressed;
extern volatile uint32_t global_counter;

// Прототипы функций
void Interrupt_init(void);
void EXTI15_10_IRQHandler(void);
void SysTick_Handler(void);


#endif