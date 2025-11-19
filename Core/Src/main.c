#include "..\Inc\init.h"
#include "interrupt.h"

volatile uint32_t global_counter = 0;

volatile uint8_t states[4] = {0, 0, 0, 0};
 
int main(void)
{
    // RCC_init();

    GPIO_init();
    // TIM1_init();
    // ADC_init();
    SET_BIT(GPIOB->MODER, GPIO_MODER_MODE7_0);
    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS7);
    // SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS4);


    while (1)
    {
        // if(states[0])
        // {
        //     SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS4);
        // }
        // else
        // {
        //     SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR4);
        // }
        // if(states[1])
        // {
        //     SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS1);
        // }
        // else
        // {
        //     SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR1);
        // }
        // if(states[2])
        // {
        //     SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS3);
        // }
        // else
        // {
        //     SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR3);
        // }
        // if(states[3])
        // {
        //     SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS5);
        // }
        // else
        // {
        //     SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR5);
        // }
    }
}