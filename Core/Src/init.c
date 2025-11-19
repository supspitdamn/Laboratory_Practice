#include "../Inc/init.h"
#include "../Inc/interrupt.h"

void GPIO_init(void)
{
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOAEN); // Светодиоды PB4, PA4, PB3, PB5. Потенциометр на PA8

    // PB3 PB4 PB5 - светодиод

    SET_BIT(GPIOB->MODER, GPIO_MODER_MODE3_0 | GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_0);
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT3 | GPIO_OTYPER_OT4 | GPIO_OTYPER_OT5);
    SET_BIT(GPIOB->OSPEEDR, GPIO_OSPEEDR_OSPEED3_Msk | GPIO_OSPEEDR_OSPEED4_Msk | GPIO_OSPEEDR_OSPEED5_Msk);
    CLEAR_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPD3_Msk | GPIO_PUPDR_PUPD4_Msk | GPIO_PUPDR_PUPD5_Msk);
    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR3 | GPIO_BSRR_BR4 | GPIO_BSRR_BR5);

    // PB1 - светодиод

    SET_BIT(GPIOB->MODER, GPIO_MODER_MODE1_0);
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT1);
    SET_BIT(GPIOB->OSPEEDR, GPIO_OSPEEDR_OSPEED1_Msk);
    CLEAR_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPD1_Msk);
    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR1);

    // PA8 - TIM1

    SET_BIT(GPIOA->MODER,  GPIO_MODER_MODE8_1);
    MODIFY_REG(GPIOA->AFR[1], GPIO_AFRH_AFSEL8_Msk, 1 << GPIO_AFRH_AFSEL8_Pos); //AF1
    CLEAR_BIT(GPIOA->OTYPER, GPIO_OTYPER_OT8);
    CLEAR_BIT(GPIOA->PUPDR, GPIO_PUPDR_PUPD8_Msk);
    SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR8);

    // PA0 - ПОТЕНЦИОМЕТР

    MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE0_Msk, 0x3 << GPIO_MODER_MODE0_Pos);
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

/*Что в итоге сказать. Для настройки обычного счетчика мне нужно сделать:
1) Подать тактирование на таймер (для TIM1 это APB2, по документации)
2) Настроить предделитель PSC (Prescaler)
3) Настроить значение переполнения ARR (AUTO RENEW)
4) Обновить счетчик EGR_UG. РАЗРЕШИТЬ ПРЕРЫВАНИЕ ПО ПЕРЕПОЛНЕНИЮ DIER_UIE
5) Запустить счетчик CR1_CEN
6) Добавить обработчик
7) Выдать приоритет (важно когда прерываний несколько)*/

void TIM1_init(void)
{
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN); // Подали тактирование на TIM1

    MODIFY_REG(TIM1->PSC,TIM_PSC_PSC_Msk, 179 << TIM_PSC_PSC_Pos); // предделитель CLK 
    MODIFY_REG(TIM1->ARR, TIM_ARR_ARR_Msk, 999 << TIM_ARR_ARR_Pos); // переполнение 

    SET_BIT(TIM1->CR1, TIM_CR1_ARPE); 

    MODIFY_REG(TIM1->CCMR1, TIM_CCMR1_OC1M_Msk, 6 << TIM_CCMR1_OC1M_Pos); // Режим PWM
    SET_BIT(TIM1->CCMR1, TIM_CCMR1_OC1PE);
    MODIFY_REG(TIM1->CCR1, TIM_CCR1_CCR1_Msk, 500 << TIM_CCR1_CCR1_Pos);

    SET_BIT(TIM1->CCER, TIM_CCER_CC1E);
    SET_BIT(TIM1->BDTR, TIM_BDTR_MOE);


    SET_BIT(TIM1->EGR, TIM_EGR_UG); // Обновляем счетчик

    SET_BIT(TIM1->DIER, TIM_DIER_UIE); // Разрешил прерывание по переполнению

    SET_BIT(TIM1->CR1, TIM_CR1_CEN); // Включил TIM1

    NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn); // Соответствующий обработчик прерывания
    NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0); // Приоритет прерывания 1


}

void ADC_init(void)
{
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC1EN);
    SET_BIT(ADC1->CR2, ADC_CR2_ADON);

    MODIFY_REG(ADC1->SQR3, ADC_SQR3_SQ1_Msk, 0 << ADC_SQR3_SQ1_Pos);
    MODIFY_REG(ADC1->SQR1, ADC_SQR1_L_Msk, 0 << ADC_SQR1_L_Pos);

    for(uint16_t i = 0; i<1000; i++);
}

uint32_t READ_POT(void)
{
    SET_BIT(ADC1->CR2, ADC_CR2_SWSTART);

    while(!READ_BIT(ADC1->SR, ADC_SR_EOC)) __NOP();

    return ADC1->DR; // Возвращает зашимленное значение
}