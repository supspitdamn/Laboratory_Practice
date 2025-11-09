#include "..\Inc\init2.h"
#include "interrupt.h"

volatile uint8_t button_pressed = 0;
volatile uint8_t prev_state = 0;

int main(void) // Запускать плл блок после всех настроек
{
    RCC_init();

    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN);

    MODIFY_REG(GPIOC->MODER, GPIO_MODER_MODER12_Msk, 0x00); // Input mode
    MODIFY_REG(GPIOC->PUPDR, GPIO_PUPDR_PUPDR12, GPIO_PUPDR_PUPDR12_0); // Pull-up

    SET_BIT(GPIOC->MODER, GPIO_MODER_MODER9_1);           // Настраиваем пин на альтернативный режим
    SET_BIT(GPIOC->OSPEEDR, GPIO_OSPEEDR_OSPEED9_Msk);    // Настраиваем пин на максимальную скорость работы
    MODIFY_REG(GPIOC->AFR[1], GPIO_AFRH_AFSEL9_Msk, 0x0); // Выбираем тип альтернативной функции – Выход MCO2

    MODIFY_REG(GPIOB->MODER, GPIO_MODER_MODER0_Msk, GPIO_MODER_MODER0_0);

    MODIFY_REG(GPIOB->OSPEEDR, GPIO_OSPEEDR_OSPEED0_Msk, GPIO_OSPEEDR_OSPEED0_1);

    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT_0);

    MODIFY_REG(GPIOB->MODER, GPIO_MODER_MODER7_Msk, GPIO_MODER_MODER7_0);

    MODIFY_REG(GPIOB->OSPEEDR, GPIO_OSPEEDR_OSPEED7_Msk, GPIO_OSPEEDR_OSPEED7_1);

    Interrupt_init();

    int counter_ = 0;

    while(1)
    {
        if(button_pressed && prev_state == 0)
        {
            counter_++;
            prev_state = 1;
            button_pressed = 0;
            
            // Управление светодиодами
            switch(counter_)
            {
                case 1:
                    // Включить PB0, выключить PB7
                    GPIOB->BSRR = GPIO_BSRR_BS_0 | GPIO_BSRR_BR_7;
                    break;
                    
                case 2:
                    // Выключить PB0, включить PB7
                    GPIOB->BSRR = GPIO_BSRR_BR_0 | GPIO_BSRR_BS_7;
                    break;
                    
                case 3:
                    // Выключить оба светодиода и сбросить счетчик
                    GPIOB->BSRR = GPIO_BSRR_BR_0 | GPIO_BSRR_BR_7;
                    counter_ = 0;
                    break;
            }
        }
        
        // Сброс prev_state когда кнопка отпущена
        if(GPIOC->IDR & GPIO_IDR_ID12)
        {
            prev_state = 0;
        }
        
    }
}

// Код +- рабочий, однако кпока работает раз через раз

