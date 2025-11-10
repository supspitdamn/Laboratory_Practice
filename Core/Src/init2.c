#include "../Inc/init2.h"


void Interrupt_init(void) // По какому-либо сценарию происходит остановка программы по таймеру, вызывая обработчик прерывания
{
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN); // Подали тактирование для прерываний

    SET_BIT(SYSCFG->EXTICR[3], SYSCFG_EXTICR4_EXTI12_PC); // Активировали PC12

    SET_BIT(EXTI->IMR, EXTI_IMR_IM12); // Настроили маску на прерывание

    SET_BIT(EXTI->RTSR, EXTI_RTSR_TR12); // Назначили по фронту интеррапт

    CLEAR_BIT(EXTI->FTSR, EXTI_FTSR_TR12); // Убрали по спаду интеррапт

    NVIC_SetPriority(EXTI15_10_IRQn, NVIC_EncodePriority(__NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(EXTI15_10_IRQn); // Разрешил прерывание (название взял в ассемблерном коде stm32)

}

void RCC_init(void)
{
    MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, 0x80UL);
    CLEAR_REG(RCC->CFGR);
    while (READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RESET);
    CLEAR_BIT(RCC->CR, RCC_CR_PLLON);
    while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) != RESET);
    CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON);
    while (READ_BIT(RCC->CR, RCC_CR_HSERDY) != RESET);
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);

    CLEAR_BIT(RCC->CR, RCC_CR_HSION);

    SET_BIT(RCC->CR, RCC_CR_HSEON); // Включение внешнего источника тактирования
    while (READ_BIT(RCC->CR, RCC_CR_HSERDY) == RESET);
    SET_BIT(RCC->CR, RCC_CR_CSSON); // Включение Clock Security

    // PLL configurator
    CLEAR_REG(RCC->PLLCFGR);
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_HSE);                                                            // Источник тактирования HSE
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLM_2);                                                                // деление тактирования на 4
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLN_3 | RCC_PLLCFGR_PLLN_5 | RCC_PLLCFGR_PLLN_6 | RCC_PLLCFGR_PLLN_8); // число 360 в  bin
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLP_0);                                                                // Деление после умножения на 4 (PLLP). теперь нужно делить на 4. Для этого передать 01
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLQ_0 | RCC_PLLCFGR_PLLQ_1 | RCC_PLLCFGR_PLLQ_2 | RCC_PLLCFGR_PLLQ_3); // Настроили PLLQ (деление после умножения на 15)

    // tact configurator
    // SET_BIT(RCC->CFGR, RCC_CFGR_SW_1);

    /* while (READ_BIT(RCC->CFGR, RCC_CFGR_SWS_1) == RESET); */  // не запустится pll
    SET_BIT(RCC->CFGR, RCC_CFGR_SW_PLL);                         // В качестве системного тактирования выбран PLL
    SET_BIT(RCC->CFGR, RCC_CFGR_HPRE_DIV1);                      // предделитель шины AHB1 настроен на 1 без деления
    SET_BIT(RCC->CFGR, RCC_CFGR_PPRE1_DIV4);                     // предделитель шины AHB1 настроен на 4 ОНА от 45
    SET_BIT(RCC->CFGR, RCC_CFGR_PPRE2_DIV2);                     // предделитель шины APB2 настроен на 2 ОНА от 90
    SET_BIT(RCC->CFGR, RCC_CFGR_MCO1);                           // настройка вывода на MCO1
    CLEAR_BIT(RCC->CFGR, RCC_CFGR_MCO2);                         // Настройка вывода частоты SYSCLOCK на MSO2
    SET_BIT(RCC->CFGR, RCC_CFGR_MCO1PRE_2 | RCC_CFGR_MCO1PRE_1); // Предделитель 2 для вывода на MCO1
    SET_BIT(RCC->CFGR, RCC_CFGR_MCO2PRE_2 | RCC_CFGR_MCO2PRE_1); //

    SET_BIT(FLASH->ACR, FLASH_ACR_LATENCY_5WS); // Утановка 5 циклов ожидания для FLASH памяти
    SET_BIT(RCC->CR, RCC_CR_PLLON);             // Включение PLL
    while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) == RESET);
}

void systick_init(void)
{
    CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);// Если поймали помехи. Отключили счетный регистр
    SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk); // Включили прерывания
    SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk); // Подключили источник тактирования. 1 - без делителя


}