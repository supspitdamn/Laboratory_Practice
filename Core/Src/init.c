#include "../Inc/init.h"
// Кнопки PB1, PB2, PB3, PC11
// Фонарики A5, A10, A11

void macros_init(void)
{
    // Сделай сама
}

// Для кнопок нужен MODER, OTYPER, IDR

void cmsis_init(void)
{
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN); // Включила тактирование A, B, C

    // Настройка светодиодов (я тренировалась с пользовательскими)

    // Пользовательские

    CLEAR_REG(GPIOB->MODER);
    CLEAR_REG(GPIOB->OTYPER);
    CLEAR_REG(GPIOB->PUPDR); // без пулап и пулдаун
    CLEAR_REG(GPIOC->MODER);

    SET_BIT(GPIOB->MODER, GPIO_MODER_MODE0_0 | GPIO_MODER_MODE7_0 | GPIO_MODER_MODE14_0); // Настроил на выход
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT0 | GPIO_OTYPER_OT7 | GPIO_OTYPER_OT14); // пуш-пулл
    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR0 | GPIO_BSRR_BR7 | GPIO_BSRR_BR14); // предварительно выключила светодиоды

    // на порту А сама

    /**/

    // Кнопки
    
    MODIFY_REG(GPIOB->MODER, GPIO_MODER_MODE1_Msk | GPIO_MODER_MODE2_Msk | GPIO_MODER_MODE3_Msk, 0x0);
    SET_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPD1_0 | GPIO_PUPDR_PUPD2_0 | GPIO_PUPDR_PUPD3_0);

    MODIFY_REG(GPIOC->MODER, GPIO_MODER_MODE11_Msk, 0x0);
    SET_BIT(GPIOC->PUPDR, GPIO_PUPDR_PUPD11_0);
}

void simple_init(void)
{
    // Сделай сама
}

uint32_t pressed_button(GPIO_TypeDef* port, uint32_t mask)
{
    if(!READ_BIT(port->IDR, mask))
    {
        for(int i = 0; i < 5000; i++);
        if(!READ_BIT(port->IDR, mask))
        {
            while(!READ_BIT(port->IDR, mask))
            {
                duration++;
            }
            return 1;
        }
    }
    return 0;
}

void independent_counter()
{
    time++;
}

