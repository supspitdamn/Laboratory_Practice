#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "init2.h"
#include "stm32f4xx.h"  // Добавьте этот include

// Объявление внешних переменных (без инициализации)
extern volatile uint8_t button_pressed;

// Прототипы функций
void Interrupt_init(void);
void EXTI15_10_IRQHandler(void);

#endif