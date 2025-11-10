#include "interrupt.h"

uint8_t last_state, FLAG_DELAY;
uint32_t delay_count;

void EXTI15_10_IRQHandler(void)
{
    if (READ_BIT(EXTI->PR, EXTI_PR_PR12)) 
    {
        // // Задержка для антидребезга
        for(volatile int i = 0; i < 100; i++);
        
        // Проверяем что кнопка действительно нажата
        if(!(GPIOC->IDR & GPIO_IDR_ID12))
        {
            button_pressed = 1; // Устанавливаем флаг для основного цикла
        }
        
        SET_BIT(EXTI->PR, EXTI_PR_PR12);
    }
}

void SysTick_Handler(void)
{
    global_counter++;
    delay_count++;
}

void delay(uint32_t del)
{
    FLAG_DELAY = 1;
    while(del<=delay_count)
    {

    }
    delay_count = 0;
    FLAG_DELAY = 0;
}

