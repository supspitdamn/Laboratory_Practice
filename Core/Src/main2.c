#include "..\Inc\init2.h"
#include "interrupt.h"

volatile uint32_t global_counter = 0;
volatile uint32_t last_button_time_pressed = 0;
volatile uint32_t last_button_time_unpressed = 0;
volatile uint16_t MCO2_freq = 0;
volatile uint16_t SysClock_freq = 0;
volatile uint8_t state = 0;
volatile uint8_t button = 0;

int main(void) // Запускать плл блок после всех настроек
{
    // Настройка регистров

    RCC_init();

    GPIO_init();
    Interrupt_init();
    systick_init();

    // Основной код
    int counter = 0;
    uint8_t long_press = 0;
    uint8_t short_press = 0;

    while(1)
    {
        state = button;
        // Управление светодиодами
        if (state) 
        {
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS7); // Включить PB7
        }
        else 
        {
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR7); // Выключить PB7
        }
    }
}


