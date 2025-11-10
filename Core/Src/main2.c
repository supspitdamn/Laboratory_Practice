#include "..\Inc\init2.h"
#include "interrupt.h"

volatile uint8_t button_pressed = 0;
volatile uint8_t prev_state = 0;

int main(void) // Запускать плл блок после всех настроек
{
    GPIO_init();

    while(1)
    {
        SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS0 | GPIO_BSRR_BS7 | GPIO_BSRR_BS14);
    }
}

// Код +- рабочий, однако кпока работает раз через раз

