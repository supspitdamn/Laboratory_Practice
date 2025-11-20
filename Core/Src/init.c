#include "init.h"

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

void TIM1_init(void)
{
    // Включение тактирования
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN);
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
    
    // Настройка таймера
    TIM1->PSC = 94;                    // Предделитель: 95MHz / 95 = 1MHz
    TIM1->ARR = 999;                   // Период: 1000 тактов
    TIM1->CCR1 = 500;                  // Скважность 50% (500/1000)
    
    TIM1->EGR = TIM_EGR_UG;
    
    // Настройка вывода PA8 (TIM1_CH1)
    MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODER8_Msk, 2 << GPIO_MODER_MODER8_Pos);    
    MODIFY_REG(GPIOA->AFR[1], GPIO_AFRH_AFSEL8_Msk, 1 << GPIO_AFRH_AFSEL8_Pos);     
    
    // Настройка канала ШИМ
    MODIFY_REG(TIM1->CCMR1, TIM_CCMR1_OC1M_Msk, 6 << TIM_CCMR1_OC1M_Pos);  // ПВМКА
    SET_BIT(TIM1->CCMR1, TIM_CCMR1_OC1PE);          
    SET_BIT(TIM1->CCER, TIM_CCER_CC1E);            
    SET_BIT(TIM1->BDTR, TIM_BDTR_MOE);              
    
    SET_BIT(TIM1->CR1, TIM_CR1_ARPE);              
    
    TIM1->CR1 |= TIM_CR1_CEN;
    
    NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
}

void GPIO_init(void)
{
    /* Светодиоды: PD4, PD5, PD6, PD7 */
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);
    SET_BIT(GPIOD->MODER, GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0 | GPIO_MODER_MODE7_0);
    SET_BIT(GPIOD->BSRR, GPIO_BSRR_BR4 | GPIO_BSRR_BR5 | GPIO_BSRR_BR6 | GPIO_BSRR_BR7);    

    /* Вывод потенциометра: PA0*/
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
    MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODER0_Msk, 3 << GPIO_MODER_MODER0_Pos);
}

void ADC_init(void)
{
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC1EN);
    ADC1->CR2 = 0;
    SET_BIT(ADC1->CR2, ADC_CR2_ADON);     // Включить АЦП
    SET_BIT(ADC1->CR2, ADC_CR2_CONT);     // Непрерывный режим
    ADC1->SQR3 = 0;                       // Канал 0 в 1-м преобразовании
    SET_BIT(ADC1->SMPR2, ADC_SMPR2_SMP0_0 | ADC_SMPR2_SMP0_1 | ADC_SMPR2_SMP0_2);
    SET_BIT(ADC1->CR2, ADC_CR2_SWSTART);
}

uint16_t READ_POT(void)
{
    while(!(ADC1->SR & ADC_SR_EOC)) __NOP();
    return ADC1->DR;
}