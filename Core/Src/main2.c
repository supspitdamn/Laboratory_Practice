#include "..\Inc\init2.h"
#include "interrupt.h"

volatile uint32_t global_counter = 0;
extern volatile uint32_t last_button_time_pressed;
extern volatile uint32_t last_button_time_unpressed;

int main(void) // Запускать плл блок после всех настроек
{
    // Настройка регистров

    RCC_init();
    GPIO_init();
    Interrupt_init();
    systick_init();

    // Основной код
    int counter = 0;

    while(1)
    {
        if(last_button_time_unpressed - last_button_time_pressed > TIME_DELAY_2SEC)
        {
            counter++;
            if ((GPIOC->IDR & GPIO_IDR_ID6))
            {
                if(counter == 4)
                {
                    counter = 0;
                }
            }   
        }
        
        switch(counter)
        {
            case 0 :SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR0 | GPIO_BSRR_BR7 | GPIO_BSRR_BR14);
            break;
            case 1 :SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS0 | GPIO_BSRR_BR7 | GPIO_BSRR_BR14);
            break;
            case 2 :SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS0 | GPIO_BSRR_BS7 | GPIO_BSRR_BR14);
            break;
            case 3 :SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS0 | GPIO_BSRR_BS7 | GPIO_BSRR_BS14);
            break;
        }
    }
}


