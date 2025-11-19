#include "interrupt.h"

volatile uint32_t last_button1_time_pressed = 0;
volatile uint32_t last_button1_time_unpressed = 0;
volatile uint32_t last_button2_time_pressed = 0;
volatile uint32_t last_button2_time_unpressed = 0;

void EXTI9_5_IRQHandler(void)
{
    // Обработка кнопки 1 (PC6)
    if (READ_BIT(EXTI->PR, EXTI_PR_PR6)) 
    {
        EXTI->PR = EXTI_PR_PR6;  // Сброс флага
    }

    // Обработка кнопки 2 (PC7)
    if (READ_BIT(EXTI->PR, EXTI_PR_PR7)) 
    {
        EXTI->PR = EXTI_PR_PR7;  // Сброс флага
    }
}

void SysTick_Handler(void)
{
    global_counter++;
}

void TIM1_UP_TIM10_IRQHandler(void)
{
    if(READ_BIT(TIM1->SR, TIM_SR_UIF))
    {
        CLEAR_BIT(TIM1->SR, TIM_SR_UIF);
        /*Реализейшн*/

        uint32_t pot_value = READ_POT();
        uint16_t per = 4095/4;
        for(uint8_t i = 0; i < 4; i++)
        {
            if(pot_value >= (i+1)*per)
            {
                states[i] = 1;
            }
            else
            {
                states[i] = 0;
            }
        }
    }
}
