#include "init.h"           
#include "interrupt.h"     

#define eps 100

int main(void)
{
    RCC_init();
    TIM1_init();
    GPIO_init();
    ADC_init();

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    GPIOB->MODER |= GPIO_MODER_MODE0_0;
    GPIOB->BSRR |= GPIO_BSRR_BS0;

while (1)
{
    uint16_t PWM = READ_POT();
    
    if(PWM < TWENTY_PERCENT || PWM > EIGHTY_PERCENT + eps)
    {
        SET_BIT(GPIOD->BSRR, GPIO_BSRR_BR4 | GPIO_BSRR_BR5 | GPIO_BSRR_BR6 | GPIO_BSRR_BR7);
    }
    else
    {
        if (PWM >= TWENTY_PERCENT)
        {
            SET_BIT(GPIOD->BSRR, GPIO_BSRR_BS4);
        }
        else
        {
            SET_BIT(GPIOD->BSRR, GPIO_BSRR_BR4);
        }


        if (PWM >= FOURTY_PERCENT)
        {
            SET_BIT(GPIOD->BSRR, GPIO_BSRR_BS5);
        }
        else
        {
            SET_BIT(GPIOD->BSRR, GPIO_BSRR_BR5);
        }

        if (PWM >= SIXTY_PERCENT)
        {
            SET_BIT(GPIOD->BSRR, GPIO_BSRR_BS6);
        }
        else
        {
            SET_BIT(GPIOD->BSRR, GPIO_BSRR_BR6);
        }

        if (PWM >= EIGHTY_PERCENT)
        {
            SET_BIT(GPIOD->BSRR, GPIO_BSRR_BS7);
        }
        else
        {
            SET_BIT(GPIOD->BSRR, GPIO_BSRR_BR7);
        }
    }
}
}