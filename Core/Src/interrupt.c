#include "interrupt.h"

void EXTI9_5_IRQHandler(void)
{
    if (READ_BIT(EXTI->PR, EXTI_PR_PR6)) 
    {
        SET_BIT(EXTI->PR, EXTI_PR_PR6);
        if(!(GPIOC->IDR & GPIO_IDR_ID6)) // На отпускание
        {
            last_button_time_pressed = global_counter; 
        }
        else
        {
            last_button_time_unpressed = global_counter;
        }
    }
}

void SysTick_Handler(void)
{
    global_counter++;
}

