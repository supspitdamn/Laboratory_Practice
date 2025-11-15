#include "..\Inc\init.h"
#include "stdio.h"

// Кнопка 6 - включение светодиодов
// Кнопка 7 - смена режима работы

volatile uint32_t duration = 0;
volatile uint32_t flag1 = 0;
volatile uint32_t flag2 = 0;
volatile uint32_t flag3 = 0;

int main(void)
{
    cmsis_init();

    int8_t counter = -1;
    int8_t LED_for_change = -1;
    int16_t frequences[3] = {100000, 500000, 1000000};
    uint8_t states[3] = {0, 0, 0};
    uint16_t LED_freq[3] = {0, 0, 0};
    int8_t x = -1;

    while(1)
    {
        if(pressed_button(GPIOB, GPIO_IDR_ID1)) // Кнопка PB1
        {
            if(duration > SHORTDURATION) // Переключение светодиодов цикличное
            {
                counter++;
                if(counter > 2)
                {
                    counter = 0;
                }

                for(uint8_t i = 0; i < counter; i++)
                {
                    states[i] = 1;
                }
            }
            duration = 0;
        }

        if(pressed_button(GPIOC, GPIO_IDR_ID11)) // Кнопка PC11
        {
            if(duration > SHORTDURATION) // Выключение всех светодиодов по короткому жамканью
            {
                for(uint8_t i = 0; i < 3; i++)
                {
                    states[i] = 0;
                }
            }
            else if(duration > LONGDURATION) // Сдвиги светодиодов по долгому жамканью
            {

            }
            duration = 0;
        }

        if(counter == 0) // Выключение всех светодиодов при по завершении цикла переключений
        {
            for(uint8_t i = 0; i < 3; i++)
            {
                states[i] = 0;
            }
        }

        if(pressed_button(GPIOB, GPIO_IDR_ID2)) // Кнопка PB2
        {
            x++;
            if(x > 2)
            {
                x = 0;
            }
            if(x >= 0 && LED_for_change >= 0)
            {
                if(duration > SHORTDURATION)
                {
                    LED_freq[LED_for_change] = frequences[x];
                }
            }
            duration = 0;
        }

        if(pressed_button(GPIOB, GPIO_IDR_ID3)) // Кнопка PB3
        {
            if(duration > SHORTDURATION)
            {
                LED_for_change++;
                if(LED_for_change > 2)
                {
                    LED_for_change = -1;
                }
            }
            duration = 0;
        }
    }
}

