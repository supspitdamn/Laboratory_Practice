#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "init2.h"
#include "stm32f4xx.h"  
extern volatile uint32_t last_button1_time_pressed;
extern volatile uint32_t last_button1_time_unpressed;
extern volatile uint32_t last_button2_time_pressed;
extern volatile uint32_t last_button2_time_unpressed;

extern volatile uint32_t global_counter;
extern volatile uint8_t short_press2;
extern volatile uint8_t long_press2;
extern volatile uint8_t short_press1;
extern volatile uint8_t long_press1;
extern volatile uint32_t frequences[6];
extern volatile uint32_t last_toggle[6];
extern volatile uint8_t states[6];
extern volatile int8_t selected_led;
extern volatile uint8_t counter_light;

// Прототипы функций
void Interrupt_init(void);
void EXTI9_5_IRQHandler(void);
void SysTick_Handler(void);
void wait(uint32_t time);


#endif