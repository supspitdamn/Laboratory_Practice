#include "interrupt.h"

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

