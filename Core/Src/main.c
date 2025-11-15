#include "..\Inc\init.h"
#include "stdio.h"

volatile uint32_t duration = 0;

volatile int32_t frequences[3] = {50000, 200000, 1000000};
volatile uint8_t states[3] = {0, 0, 0};
volatile uint32_t LED_freq[3] = {50000, 50000, 50000};
volatile uint32_t last_time[3] = {0, 0, 0};

volatile uint32_t time = 0;

int main(void)
{
    cmsis_init();

    uint8_t counter = 0;
    int8_t LED_for_change = -1;
    int8_t x = -1;
    uint8_t appendix = 0;

    while(1)
    {
        if(pressed_button(GPIOB, GPIO_IDR_ID1)) // Кнопка PB1
        {
            counter++;
            if(counter == 4)
            {
                counter = 0;
            }
        }

        if(pressed_button(GPIOC, GPIO_IDR_ID11)) // Кнопка PC11
        {
            if(duration > SHORTDURATION) // Выключение всех светодиодов по короткому жамканью
            {
                counter = 0;
            }
            else if(duration > LONGDURATION) // Сдвиги светодиодов по долгому жамканью
            {

            }
            duration = 0;
        }

        if(pressed_button(GPIOB, GPIO_IDR_ID2)) // Кнопка PB2
        {
            x++;
            if(x > 2)
            {
                x = 0;
            }
            duration = 0;
        }

        if(pressed_button(GPIOB, GPIO_IDR_ID3)) // Кнопка PB3
        {
            if(duration > SHORTDURATION)
            {
                LED_for_change++;
                if(LED_for_change == 4)
                {
                    LED_for_change = -1;
                }
            }
            duration = 0;
            x = 0;
        }

        if(counter == 0)
        {
            time = 0;
            for(uint8_t i = 0; i < 3; i++)
            {
                states[i] = 0;
            }
        }

        if(LED_for_change > -1) 
        {
            LED_freq[LED_for_change] = frequences[x];
        }

        // Переключение диодов

        independent_counter();

        for(uint32_t i = 0; i < counter; i++)
        {
            if(i == LED_for_change)
            {
                states[i] = 1;
            }
            else
            {
                if(LED_freq[i] <= (time - last_time[i])) 
                {
                    states[i] = !states[i];
                    last_time[i] = time;
                }
             }
        }

        // Работа с диодами
        if(states[0])
        {
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS0);
        }
        else
        {
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR0);
        }

        if(states[1])
        {
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS7);
        }
        else
        {
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR7);
        }

        if(states[2])
        {
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS14);
        }
        else
        {
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR14);
        }
    }
}

