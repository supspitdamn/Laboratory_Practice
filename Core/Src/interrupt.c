#include "interrupt.h"

uint8_t last_state, FLAG_DELAY;
uint32_t delay_count;

void EXTI9_5_IRQHandler(void)
{
    if (READ_BIT(EXTI->PR, EXTI_PR_PR6)) 
    {
        SET_BIT(EXTI->PR, EXTI_PR_PR6);
        last_button_time = global_counter; 
        if((GPIOC->IDR & GPIO_IDR_ID6)) // На отпускание
        {
            button_pressed = 1;
        }
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

