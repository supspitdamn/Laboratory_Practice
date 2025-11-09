#include "../Inc/init.h"

// Всем хай)

void macros_init(void)
{
    SETBIT(RCC_TACT, GPIO_B_RCC_ON); // Включение тактирования ГПИОБ

    SETBIT(MODERB_, MODERB0_ON);
    SETBIT(MODERB_, MODERB7_ON); // Включение тактирования для портов PB0, PB7, PB14
    SETBIT(MODERB_, MODERB14_ON);

    SETBIT(OTYPER_, OTYPER_ON); // режим пуш-пулл для портов PB0, PB7, PB14

    SETBIT(OSPEEDR_, OSPEEDRB_0_ON);
    SETBIT(OSPEEDR_, OSPEEDRB_7_ON); // Настройка скорости для портов PB0, PB7, PB14 (средняя)
    SETBIT(OSPEEDR_, OSPEEDRB_14_ON);
    
    SETBIT(PUPDR_, PUPDRB_0_ON);
    SETBIT(PUPDR_, PUPDRB_7_ON); // Настройка без пулл-ап и пулл-даун для портов PB0, PB7, PB14
    SETBIT(PUPDR_, PUPDRB_14_ON);

    SETBIT(BSRR_, BSRRB_0_OFF);
    SETBIT(BSRR_, BSRRB_7_OFF); // 0 на пинах PB0, PB7, PB14
    SETBIT(BSRR_, BSRRB_14_OFF);


    SETBIT(RCC_TACT, GPIO_C_RCC_ON);

    SETBIT(MODERC_, MODERC6_ON);

    SETBIT(OSPEEDR_, OSPEEDRC6_ON);

    SETBIT(PUPDRC_, PUPDRC6_ON);

    SETBIT(IDRC_, IDRC6_);

    SETBIT(MODERC_, MODERC7_ON);

    SETBIT(OSPEEDR_, OSPEEDRC7_ON);

    SETBIT(PUPDRC_, PUPDRC7_ON);

    SETBIT(IDRC_, IDRC7_);

}

// Для кнопок нужен MODER, OTYPER, IDR

void cmsis_init(void)
{
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN); // Тактирование ГПИОБ
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN); // Тактирование ГПИОС

    /******************************************************/

    SET_BIT(GPIOB->MODER, GPIO_MODER_MODE0_0);
    SET_BIT(GPIOB->MODER, GPIO_MODER_MODE7_0); // Порты В на выход
    SET_BIT(GPIOB->MODER, GPIO_MODER_MODE14_0);

    CLEAR_BIT(GPIOC->MODER, GPIO_MODER_MODE6_0 | GPIO_MODER_MODE6_1); // Порты С на вход
    CLEAR_BIT(GPIOC->MODER, GPIO_MODER_MODE7_0 | GPIO_MODER_MODE7_1); 

    //

    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT_0);
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT_7); // Порты пуш-пул
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT_14);

    //

    SET_BIT(GPIOB->OSPEEDR, GPIO_OSPEEDR_OSPEED0_0);
    SET_BIT(GPIOB->OSPEEDR, GPIO_OSPEEDR_OSPEED7_0); // Средняя скорость
    SET_BIT(GPIOB->OSPEEDR, GPIO_OSPEEDR_OSPEED14_0);

    //

    CLEAR_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPDR0_0);
    CLEAR_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPDR7_0); // Без пулап пулдаун
    CLEAR_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPDR14_0); 

    SET_BIT(GPIOC->PUPDR, GPIO_PUPDR_PUPDR6_1);
    CLEAR_BIT(GPIOC->PUPDR, GPIO_PUPDR_PUPDR6_0);
    SET_BIT(GPIOC->PUPDR, GPIO_PUPDR_PUPDR7_1);
    CLEAR_BIT(GPIOC->PUPDR, GPIO_PUPDR_PUPDR7_0);

    //

    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR0);
    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR7); // Выключение светодиодов
    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR14);

    //
}

void simple_init(void)
{
    *(uint32_t*)(0x40023800UL + 0x30) |= 0x02UL;   

    *(uint32_t*)(0x40020400UL + 0x00) |= 0x1UL;
    *(uint32_t*)(0x40020400UL + 0x00) |= 0x4000UL;
    *(uint32_t*)(0x40020400UL + 0x00) |= 0x10000000UL;

    *(uint32_t*)(0x40020400UL + 0x04) |= 0x0000000000000000UL;

    *(uint32_t*)(0x40020400 + 0x08) |= 0x01UL;
    *(uint32_t*)(0x40020400 + 0x08) |= 0x4000UL;
    *(uint32_t*)(0x40020400 + 0x08) |= 0x10000000UL;

    *(uint32_t*)(0x40020400 + 0x0C) |= 0x00UL;
    *(uint32_t*)(0x40020400 + 0x0C) |= 0x00UL;
    *(uint32_t*)(0x40020400 + 0x0C) |= 0x00UL;
    
    *(uint32_t*)(0x40020400UL + 0x18) |= 0x01UL;
    *(uint32_t*)(0x40020400UL + 0x18) |= 0x080UL;
    *(uint32_t*)(0x40020400UL + 0x18) |= 0x04000UL;

    // Кнопки

    *(uint32_t*)(0x40023800UL + 0x30) |= 0x04UL;

    *(uint32_t*)(0x40020800UL + 0x00) |= (0x00000000);

    *(uint32_t*)(0x40020800UL + 0x0C) |= (0x1000);

    *(uint32_t*)(0x40020800UL + 0x00) |= (0x00000000);

    *(uint32_t*)(0x40020800UL + 0x0C) |= 0x4000;
}

uint8_t pressed_button(uint32_t mask)
{
    if(!(IDRC_ & mask))
    {
        for(int i = 0; i < 5000; i++)
        {
        }
        if(!(IDRC_ & mask))
        {
                while(!(IDRC_ & mask))
                {
                    printf("Yes");
                }
                return 1;
        }
    }
    return 0;
}

uint32_t blick_LED_mode(uint32_t mode)
{
    uint32_t output = 0;
    switch(mode)
    {
        case 0: output = 10000;
            break;
        case 1: output = 100000;
            break;
        case 2: output = 1000000;
            break;
        case 3: output = 10000000;
            break;
    }
    return output;
}

void glow_LED(uint8_t LED1, uint8_t LED2, uint8_t LED3, uint8_t mode)
{
    for(int i = 0; i < blick_LED_mode(mode); i++)
    {
        if(!(IDRC_ & IDRC6_) && !(IDRC_ & IDRC7_))
        {
            break;
        }
        if(LED1)
        {
            SET_BIT(BSRR_, BSRRB_0_ON);
        }
        else
        {
            SET_BIT(BSRR_, BSRRB_0_OFF);
        }

        if(LED2)
        {
            SET_BIT(BSRR_, BSRRB_7_ON);
        }
        else
        {
            SET_BIT(BSRR_, BSRRB_7_OFF);
        }

        if(LED3)
        {
            SET_BIT(BSRR_, BSRRB_14_ON);
        }
        else
        {
            SET_BIT(BSRR_, BSRRB_14_OFF);
        }
    }
    for(int i = 0; i < blick_LED_mode(mode); i++)
    {

            if(!(IDRC_ & IDRC6_) && !(IDRC_ & IDRC7_))
            {
                break;
            }

            SET_BIT(BSRR_, BSRRB_0_OFF);

            SET_BIT(BSRR_, BSRRB_7_OFF);

            SET_BIT(BSRR_, BSRRB_14_OFF);
    }
}

