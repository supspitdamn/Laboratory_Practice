#include "../Inc/init2.h"
#include "../Inc/interrupt.h"
extern volatile uint16_t MCO2_freq;
extern volatile uint16_t SysClock_freq;

void GPIO_init(void)
{
    // Активировал порты ГПИОБ ГПИОС
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);


    // НАСТРОЙКА MCO2 (PC9) - ИСПРАВЛЕНО
    MODIFY_REG(GPIOC->MODER, GPIO_MODER_MODER9_Msk, 2UL << GPIO_MODER_MODER9_Pos);   // Alternate function
    MODIFY_REG(GPIOC->OSPEEDR, GPIO_OSPEEDR_OSPEED9_Msk, 3UL << GPIO_OSPEEDR_OSPEED9_Pos); // High speed
    MODIFY_REG(GPIOC->AFR[1], GPIO_AFRH_AFSEL9_Msk, 0UL << GPIO_AFRH_AFSEL9_Pos);   // AF0 ???

    SET_BIT(GPIOB->MODER, GPIO_MODER_MODE0_0 | GPIO_MODER_MODE7_0 | GPIO_MODER_MODE14_0
         | GPIO_MODER_MODE8_0 | GPIO_MODER_MODE9_0 | GPIO_MODER_MODE10_0); // Светодиоды на выход

    CLEAR_BIT(GPIOC->MODER, GPIO_MODER_MODE6 | GPIO_MODER_MODE7); // Кнопки по входу

    //

    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR0 | GPIO_BSRR_BR7 | GPIO_BSRR_BR14 | GPIO_BSRR_BR8 | GPIO_BSRR_BR9 | GPIO_BSRR_BR10); // Выключил светодиоды
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT0_Msk); // светодиоды пуш-пул
    SET_BIT(GPIOC->PUPDR, GPIO_PUPDR_PUPD6_0 | GPIO_PUPDR_PUPD7_0); // Кнопки пул-ап

}

void Interrupt_init(void) // По какому-либо сценарию происходит остановка программы по таймеру, вызывая обработчик прерывания
{
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN); // Подали тактирование для прерываний. 90 Мгц

    SET_BIT(SYSCFG->EXTICR[1], SYSCFG_EXTICR2_EXTI6_PC); // Активировали PC6

    SET_BIT(EXTI->IMR, EXTI_IMR_IM6); // Настроили маску на прерывание

    SET_BIT(EXTI->RTSR, EXTI_RTSR_TR6); // Назначили по фронту интеррапт

    SET_BIT(EXTI->FTSR, EXTI_FTSR_TR6); // Назначили по спаду интеррапт

    SET_BIT(SYSCFG->EXTICR[1], SYSCFG_EXTICR2_EXTI7_PC); // Активировали PC6

    SET_BIT(EXTI->IMR, EXTI_IMR_IM7); // Настроили маску на прерывание

    SET_BIT(EXTI->RTSR, EXTI_RTSR_TR7); // Назначили по фронту интеррапт

    SET_BIT(EXTI->FTSR, EXTI_FTSR_TR7); // Назначили по спаду интеррапт

    NVIC_SetPriority(EXTI9_5_IRQn, NVIC_EncodePriority(__NVIC_GetPriorityGrouping(), 0, 0));

    NVIC_EnableIRQ(EXTI9_5_IRQn); // Разрешил прерывание (название взял в ассемблерном коде stm32)
}

void systick_init(void) // Прерывания таймера
{
    CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);// Если поймали помехи. Отключили счетный регистр
    SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk); // Включили прерывания
    SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk); // Подключили источник тактирования. 1 - без делителя

    MODIFY_REG(SysTick->LOAD, SysTick_LOAD_RELOAD_Msk, (180000-1) << SysTick_LOAD_RELOAD_Pos); // Регистр, маска удаления, маска установочная
    MODIFY_REG(SysTick->VAL, SysTick_VAL_CURRENT_Msk, (0) << SysTick_VAL_CURRENT_Pos);
    
    SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk); // Включил прерывание
}

void RCC_init(void) // Тактирование майселф
{
    // Подготовка к настройке - чистим регистры
    MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, 0x80UL);
    CLEAR_REG(RCC->CFGR); // Выбрали HSI
    while((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_HSI); // Ждем включения HSI
    CLEAR_BIT(RCC->CR, RCC_CR_PLLON); // Отчищаем предделители PLL
    while ((READ_BIT(RCC->CR, RCC_CR_PLLRDY))); // Ждем отключения PLL
    CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON); // Отключаем HSE и CSS
    while ((READ_BIT(RCC->CR, RCC_CR_HSERDY))); // Ждем отключения HSE

    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP); // Без пайпаса
    SET_BIT(RCC->CR, RCC_CR_HSEON); // Включаем HSE
    while(!(RCC->CR & RCC_CR_HSERDY)); // Ждем включения
    SET_BIT(RCC->CR, RCC_CR_CSSON);

    CLEAR_REG(RCC->PLLCFGR);

    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_HSE);
    
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM_Msk, 8UL << RCC_PLLCFGR_PLLM_Pos); // Какое пишем, такое и будет
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN_Msk, 360UL << RCC_PLLCFGR_PLLN_Pos); // Какое пишем, такое и будет
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLP_Msk, 0UL << RCC_PLLCFGR_PLLP_Pos); // 0 - 2, 1 - 4, 2 - 6, 3 - 8

    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_5WS);

    SET_BIT(RCC->CR, RCC_CR_PLLON); // Включил PLL
    while((RCC->CR & RCC_CR_PLLRDY) == RESET); // Подождал включения PLL

    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_PLL); // Установил PLL как тактирование

    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE_Msk, RCC_CFGR_HPRE_DIV1); // Прдедделитель 
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2_Msk, RCC_CFGR_PPRE2_DIV2); // Предделитель APB2 - 90 МГц
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1_Msk, RCC_CFGR_PPRE1_DIV4); // Предделитель APB1 - 45 МГц

    MODIFY_REG(RCC->CFGR, RCC_CFGR_MCO2PRE_Msk, 0UL << RCC_CFGR_MCO2PRE_Pos); // Предделитель MCО2 = 0 - 0, 4 - 2, 5 - 3, 6 - 4, 3 - 5
    CLEAR_BIT(RCC->CFGR, RCC_CFGR_MCO2_Msk); // MCO2 от SysClock
}
