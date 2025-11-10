#include "..\Inc\init.h"
#include "stdio.h"

// Кнопка 6 - включение светодиодов
// Кнопка 7 - смена режима работы

volatile uint32_t duration = 0;

int main(void)
{
    cmsis_init();
    int counter = 0;
    int mass[3] = {1000, 5000, 10000};
    while(1)
    {
        if(pressed_button(GPIOB, GPIO_IDR_ID3)) // Переключение светодиодов
        {
            if(duration>LONGDURATION)
            {
                SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR0 | GPIO_BSRR_BR7 | GPIO_BSRR_BR14);
            }
            else if(duration>MEDIUMDURATION)
            {
                SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS0);
            }
            else if(duration>SHORTDURATION)
            {
                SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS7);
            }
            else if(duration<=SHORTDURATION)
            {
                SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS14);
            }
            duration = 0;
        }

        if(pressed_button(GPIOB, GPIO_IDR_ID1)) // Цикличное переключение светодиодов
        {
            counter++;
            if(counter == 4)
            {
                counter = 0;
            }
            duration = 0;
        }

        switch(counter)
        {
            case 0 : SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR0 | GPIO_BSRR_BR7 | GPIO_BSRR_BR14);
            break;
            case 1 : SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS0);
            break;
            case 2 : SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS7);
            break;
            case 3 : SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS14);
            break;
        }

        if(pressed_button(GPIOB, GPIO_IDR_ID2)) // Задаем частоту мерцания
        {

        }
    }
}

