#include "interrupt.h"

void EXTI9_5_IRQHandler(void)
{
    if (READ_BIT(EXTI->PR, EXTI_PR_PR6)) 
    {
        EXTI->PR = EXTI_PR_PR6;

        if(!(GPIOC->IDR & GPIO_IDR_ID6)) // кнопка нажата
        {
            last_button_time_pressed = global_counter; 
        }
        else // кнопка отпущена
        {
            last_button_time_unpressed = global_counter;
            if(last_button_time_unpressed - last_button_time_pressed > TIME_DELAY_50MS)
            {
                if(last_button_time_unpressed - last_button_time_pressed > TIME_DELAY_2SEC)
                {
                    button = !button;
                }
            }
            last_button_time_unpressed = 0;
            last_button_time_pressed = 0; 
        }
    }
}

void SysTick_Handler(void)
{
    global_counter++;
}

