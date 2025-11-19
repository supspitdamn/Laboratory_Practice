#include "init.h"

/* --------------- Настройка тактирования: HSE 8 МГц → PLL → 168 МГц --------------- */
void Clock_Init_HSE_PLL_168MHz(void)
{
    /* Включаем тактирование интерфейса питания */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    /* Устанавливаем масштаб напряжения регулятора в режим 1 (необходимо для 168 МГц) */
    PWR->CR |= PWR_CR_VOS;

    /* Включаем внешний кварцевый генератор HSE */
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY)); // Ждем, пока HSE стабилизируется

    /* Настраиваем Flash: включаем кэш и выставляем 5 тактов задержки (5 WS) */
    FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN |
                 FLASH_ACR_LATENCY_5WS;

    /* Настройка делителей шины */
    RCC->CFGR = RCC_CFGR_HPRE_DIV1 |   // AHB = SYSCLK / 1
                RCC_CFGR_PPRE1_DIV4 |  // APB1 = AHB / 4
                RCC_CFGR_PPRE2_DIV2;   // APB2 = AHB / 2

    /* Настройка PLL: PLLM=8, PLLN=336, PLLP=2, PLLQ=7 */
    RCC->PLLCFGR = (8 << RCC_PLLCFGR_PLLM_Pos) |
                   (336 << RCC_PLLCFGR_PLLN_Pos) |
                   (0 << RCC_PLLCFGR_PLLP_Pos) |      // PLLP=2
                   RCC_PLLCFGR_PLLSRC_HSE |           // источник PLL = HSE
                   (7 << RCC_PLLCFGR_PLLQ_Pos);      // PLLQ=7 для USB, SDIO и RNG

    /* Включаем PLL */
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY)); // Ждем готовности PLL

    /* Переключаем системный такт на PLL */
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

    /* Выключаем внутренний RC-генератор HSI (опционально) */
    RCC->CR &= ~RCC_CR_HSION;

    /* Обновляем глобальную переменную SystemCoreClock */
    SystemCoreClock = 168000000;
}

void GPIO_Init(void)
{
    /* ---- Светодиоды: PD4, PD5, PD6, PD7 ---- */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; // Включаем тактирование порта D

    /* Сбрасываем режимы выводов PD4–PD7 */
    GPIOD->MODER &= ~(
        (3 << (4*2)) |
        (3 << (5*2)) |
        (3 << (6*2)) |
        (3 << (7*2))
    );

    /* Устанавливаем режим "выход" для PD4–PD7 */
    GPIOD->MODER |=  (
        (1 << (4*2)) |
        (1 << (5*2)) |
        (1 << (6*2)) |
        (1 << (7*2))
    );

    /* ---- Пин для АЦП PA0 ---- */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Включаем тактирование порта A
    GPIOA->MODER |= (3 << 0);           // Устанавливаем режим "аналоговый" для PA0
}

void ADC1_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    __DSB(); // Добавьте барьер памяти
    
    ADC1->SQR3 = 0; // Канал 0
    ADC1->SQR1 = 0; // 1 преобразование в последовательности
    
    // Увеличьте время выборки для надежности
    ADC1->SMPR2 |= (0b111 << ADC_SMPR2_SMP0_Pos); // 480 циклов вместо 28
    
    ADC1->CR2 |= ADC_CR2_ADON;
    
    // Задержка для стабилизации ADC
    for(volatile uint32_t i = 0; i < 10000; i++);
}


uint16_t ADC_Read(void)
{
    ADC1->CR2 |= ADC_CR2_SWSTART;        // Запуск однократного преобразования
    while (!(ADC1->SR & ADC_SR_EOC)) __NOP(); // Ждем завершения преобразования
    return ADC1->DR;                     // Возвращаем результат (12 бит)
}

void TIM1_Encoder_Init(void)
{
    // PA8 (CH1) и PA9 (CH2) – выводы энкодера
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Тактирование порта A

    /* Настройка режима альтернативной функции для PA8 и PA9 */
    GPIOA->MODER &= ~(3 << (8*2));
    GPIOA->MODER &= ~(3 << (9*2));
    GPIOA->MODER |=  (2 << (8*2));    // Альтернативная функция
    GPIOA->MODER |=  (2 << (9*2));

    /* Настройка альтернативной функции AF1 для TIM1 */
    GPIOA->AFR[1] |= (1 << (0*4));    // PA8 -> AF1
    GPIOA->AFR[1] |= (1 << (1*4));    // PA9 -> AF1

    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // Тактирование TIM1

    /* Режим энкодера: mode 3 (TIM1 counts on both edges of CH1 and CH2) */
    TIM1->SMCR = TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;

    /* Настройка каналов как вход */
    TIM1->CCMR1 |= (1 << 0) | (1 << 8);

    /* Полярность сигналов (не инвертируем) */
    TIM1->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);

    /* Включение основного выхода таймера (MOE) */
    TIM1->BDTR |= TIM_BDTR_MOE;

    TIM1->CR1 |= TIM_CR1_CEN; // Включение таймера
}
/* Возвращает процент положения ручки: 0.0 … 1.0 */
float Get_Pot_Percent(void)
{
    uint16_t adc = ADC_Read();           // Считываем значение с АЦП
    return (float)adc / 4095.0f;        // Нормализуем к диапазону 0.0 – 1.0
}


/* Управляет диодами по диапазонам */
void Update_LEDs(float perc)
{
    // Сначала выключаем все диоды (записываем в BSRR 1 << n в старший байт для сброса)
    GPIOD->BSRR = (1 << 4) << 16;
    GPIOD->BSRR = (1 << 5) << 16;
    GPIOD->BSRR = (1 << 6) << 16;
    GPIOD->BSRR = (1 << 7) << 16;

    // Включаем диоды в зависимости от диапазона значения потенциометра
    if (perc >= 0.80f && perc <= 0.85f) {
        GPIOD->BSRR = (1 << 4);
        GPIOD->BSRR = (1 << 5);
        GPIOD->BSRR = (1 << 6);
        GPIOD->BSRR = (1 << 7);
    }
    else if (perc >= 0.60f && perc < 0.80f) {
        GPIOD->BSRR = (1 << 4);
        GPIOD->BSRR = (1 << 5);
        GPIOD->BSRR = (1 << 6);
    }
    else if (perc >= 0.40f && perc < 0.60f) {
        GPIOD->BSRR = (1 << 4);
        GPIOD->BSRR = (1 << 5);
    }
    else if (perc >= 0.20f && perc < 0.40f) {
        GPIOD->BSRR = (1 << 4);
    }
}