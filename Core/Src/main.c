#include "init.h"           
#include "interrupt.h"     

int main(void)
{
    RCC_init();
    GPIO_init();
    TIM1_init();
    ADC_init();

    while (1)
    {
        uint16_t PWM = READ_POT();
        uint16_t percentage = (PWM * 100)/4096;

        if(percentage < TWENTY_PERCENT || percentage > EIGHTY_PERCENT)
        {
            SET_BIT(GPIOD->BSRR, GPIO_BSRR_BR4 | GPIO_BSRR_BR5 | GPIO_BSRR_BR6 | GPIO_BSRR_BR7);
        }
        else
        {
            if(percentage >= TWENTY_PERCENT)
            {
                SET_BIT(GPIOD->BSRR, GPIO_BSRR_BS4);
            }
            else
            {
                SET_BIT(GPIOD->BSRR, GPIO_BSRR_BR4);
            }

            if(percentage >= FOURTY_PERCENT)
            {
                SET_BIT(GPIOD->BSRR, GPIO_BSRR_BS5);
            }
            else
            {
                SET_BIT(GPIOD->BSRR, GPIO_BSRR_BR5);
            }

            if(percentage >= SIXTY_PERCENT)
            {
                SET_BIT(GPIOD->BSRR, GPIO_BSRR_BS6);
            }
            else
            {
                SET_BIT(GPIOD->BSRR, GPIO_BSRR_BR6);
            }

            if(percentage >= EIGHTY_PERCENT)
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