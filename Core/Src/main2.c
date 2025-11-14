#include "..\Inc\init2.h"
#include "interrupt.h"

volatile uint32_t global_counter = 0;

volatile uint8_t short_press1 = 0, long_press1 = 0;
volatile uint8_t short_press2 = 0, long_press2 = 0;

volatile uint32_t frequences[6] = {
    FREQ_0_3Hz, FREQ_0_3Hz, FREQ_0_3Hz, FREQ_0_3Hz, FREQ_0_3Hz, FREQ_0_3Hz
};

volatile uint8_t states[6] = {0};

volatile uint32_t last_toggle[6] = {0};

const uint32_t freq_table[3][3] = {
    { FREQ_0_3Hz, FREQ_1Hz,   FREQ_1_4Hz },
    { FREQ_0_4Hz, FREQ_1_2Hz, FREQ_1_7Hz },  
    { FREQ_0_5Hz, FREQ_1_6Hz, FREQ_1_9Hz }   
};

// Пины светодиодов: PB8, PB9, PB10, PB7, PB14, PB0
const uint8_t led_pins[6] = {8, 9, 10, 7, 14, 0};

volatile int8_t selected_led = -1;

volatile uint32_t tolerance = 0;

volatile uint16_t pressing_long_1 = 0;
volatile uint16_t pressing_long_2 = 0;

volatile uint16_t pressing_short_1 = 0;
volatile uint16_t pressing_short_2 = 0;

volatile uint8_t counter_light = 0;
int main(void)
{
    RCC_init();
    GPIO_init();
    Interrupt_init();
    systick_init();

    uint8_t x = 0, y = 0;

    while (1)
    {
        // PC6
        if (long_press1)
        {
            counter_light = (counter_light + 1) % 7;
            long_press1 = 0;
            pressing_long_1++;
        }
        else if (short_press1)
        {
            y = (y + 1) % 3;
            short_press1 = 0;
            tolerance = global_counter;
            pressing_short_1++;
        }

        // PC7
        if (long_press2)
        {
            if (selected_led == -1)
                selected_led = 0;
            else if (selected_led < 5)
                selected_led++;
            else
                selected_led = -1;

            long_press2 = 0;

            x = 0;
            y = 0;

            tolerance = global_counter;
            pressing_long_2++;
        }
        else if (short_press2)
        {
            x = (x + 1) % 3;
            short_press2 = 0;
            tolerance = global_counter;
            pressing_short_2++;
        }

        if(global_counter - tolerance > TIME_DELAY_5SEC)
        {
            selected_led = -1;
            x = 0;
            y = 0;
        }

        if(selected_led >= 0)
        {
            frequences[selected_led] = freq_table[x][y];
        }

        for (int i = 0; i < 6; i++)
        {
            if (i == selected_led)
                states[i] = 1;  
        }

        
        for (int i = 0; i < 6; i++)
        {
            if (i == selected_led)
            {
                // ВЫбранный светодиод горит
                GPIOB->BSRR = (1 << led_pins[i]);
            }
            else if (counter_light > 0 && i < counter_light && states[i])
            {
                // Остальные мерцают
                GPIOB->BSRR = (1 << led_pins[i]);
            }
            else
            {
                // светодиод не горит
                GPIOB->BSRR = (1 << (led_pins[i] + 16));
            }
        }
    }
}