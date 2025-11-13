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

        if(!(GPIOC->IDR & GPIO_IDR_ID6)) // кнопка нажата
        {
            last_button1_time_pressed = global_counter; 
        }
        else // кнопка отпущена
        {
            last_button1_time_unpressed = global_counter;
            uint32_t press_duration = last_button1_time_unpressed - last_button1_time_pressed;
            
            if(press_duration > TIME_DELAY_50MS)
            {
                if(press_duration > TIME_DELAY_2SEC)
                {
                    long_press1 = 1;
                }
                else
                {
                    short_press1 = 1;
                }
            }
            // Сброс временных меток
            last_button1_time_unpressed = 0;
            last_button1_time_pressed = 0; 
        }
    }

    // Обработка кнопки 2 (PC7)
    if (READ_BIT(EXTI->PR, EXTI_PR_PR7)) 
    {
        EXTI->PR = EXTI_PR_PR7;  // Сброс флага

        if(!(GPIOC->IDR & GPIO_IDR_ID7)) // кнопка нажата
        {
            last_button2_time_pressed = global_counter; 
        }
        else // кнопка отпущена
        {
            last_button2_time_unpressed = global_counter;
            uint32_t press_duration = last_button2_time_unpressed - last_button2_time_pressed;
            
            if(press_duration > TIME_DELAY_50MS)
            {
                if(press_duration > TIME_DELAY_2SEC)
                {
                    long_press2 = 1;
                }
                else
                {
                    short_press2 = 1;
                }
            }
            // Сброс временных меток
            last_button2_time_unpressed = 0;
            last_button2_time_pressed = 0; 
        }
    }
}

void SysTick_Handler(void)
{
    global_counter++;

    for (int i = 0; i < 6; i++)
    {

        if (i == selected_led) continue;

        if (global_counter - last_toggle[i] >= frequences[i])
        {
            states[i] = !states[i];           // инвертируем состояние
            last_toggle[i] = global_counter;  // обновляем время
        }
    }
}

void wait(uint32_t time)
{
    uint32_t start_time = global_counter;
    uint32_t delay_time = time;
    
    while((global_counter - start_time) < delay_time)
    {
        __NOP();
    }
}