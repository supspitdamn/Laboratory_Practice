#include "../Inc/init2.h"

void GPIO_init(void)
{
    // Активировал порты ГПИОБ ГПИОС
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);

    //

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

    SET_BIT(SYSCFG->EXTICR[1], SYSCFG_EXTICR2_EXTI6_PC); // Активировали PC12

    SET_BIT(EXTI->IMR, EXTI_IMR_IM6); // Настроили маску на прерывание

    SET_BIT(EXTI->RTSR, EXTI_RTSR_TR6); // Назначили по фронту интеррапт

    CLEAR_BIT(EXTI->FTSR, EXTI_FTSR_TR6); // Убрали по спаду интеррапт

    NVIC_SetPriority(EXTI9_5_IRQn, NVIC_EncodePriority(__NVIC_GetPriorityGrouping(), 0, 0));

    NVIC_EnableIRQ(EXTI9_5_IRQn); // Разрешил прерывание (название взял в ассемблерном коде stm32)
}

void RCC_init(void) // Тактирование
{
    SET_BIT(RCC->CR, RCC_CR_HSEON);
    while(!(RCC->CR & RCC_CR_HSERDY));

    CLEAR_REG(RCC->PLLCFGR);
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_Msk, RCC_PLLCFGR_PLLSRC_HSE);

    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN_Msk, 180Ul << RCC_PLLCFGR_PLLN_Pos); // N = 180. VCO_output = 360 МГц
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM_Msk, 4Ul << RCC_PLLCFGR_PLLM_Pos); // М = 4. VCO_input = 2 МГц
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLP_Msk, 2UL << RCC_PLLCFGR_PLLP_Pos); // P = 2/ PLL = 180 МГц

    SET_BIT(RCC->CR, RCC_CR_PLLON); // Включил PLL
    while(!(RCC->CR & RCC_CR_PLLRDY)); // Подождал включения PLL

    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_PLL); // Установил PLL как тактирование
    while((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_PLL); // Ждем переключения мультиплексора
    
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE_Msk, 4 << RCC_CFGR_HPRE_Pos); // Прдедделитель 
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2_Msk, 5 << RCC_CFGR_PPRE2_Pos); // Предделитель

    CLEAR_BIT(RCC->CR, RCC_CR_HSION); // Выключил внутренний источник тактирования

    
}

void systick_init(void) // Прерывания таймера
{
    CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);// Если поймали помехи. Отключили счетный регистр
    SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk); // Включили прерывания
    SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk); // Подключили источник тактирования. 1 - без делителя
    
    // SET_BIT(SysTick->CTRL, SysTick_CTRL_COUNTFLAG_Msk); для лохов

    MODIFY_REG(SysTick->LOAD, SysTick_LOAD_RELOAD_Msk, (180000-1) << SysTick_LOAD_RELOAD_Pos); // Регистр, маска удаления, маска установочная
    MODIFY_REG(SysTick->VAL, SysTick_VAL_CURRENT_Msk, (180000-1) << SysTick_VAL_CURRENT_Pos);
    
    SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk); // Включил прерывание

}