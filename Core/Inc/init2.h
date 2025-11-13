#ifndef INIT2_H

#include "stm32f4xx.h"
#include "stm32f429xx.h"
#include "stdio.h"

#define TIME_DELAY_2SEC   2000 // при частоте тактирования 1000 Гц
#define TIME_DELAY_50MS   50

#define FREQ_0_3Hz 1660
#define FREQ_1Hz 500
#define FREQ_1_4Hz 357

#define FREQ_0_4Hz 1250
#define FREQ_1_2Hz 416
#define FREQ_1_7Hz 294

#define FREQ_0_5Hz 1000
#define FREQ_1_6Hz 313
#define FREQ_1_9Hz 263

#define FREQ_0_3Hz    1667    // 0.30 Гц → 3.33 сек/цикл
#define FREQ_0_5Hz    1000    // 0.50 Гц → 2.00 сек/цикл
#define FREQ_0_8Hz     625    // 0.80 Гц → 1.25 сек/цикл

#define FREQ_1_0Hz     500    // 1.00 Гц → 1.00 сек/цикл
#define FREQ_1_3Hz     385    // 1.30 Гц → 0.77 сек/цикл
#define FREQ_1_6Hz     312    // 1.60 Гц → 0.625 сек/цикл

#define FREQ_2_0Hz     250    // 2.00 Гц → 0.50 сек/цикл
#define FREQ_2_5Hz     200    // 2.50 Гц → 0.40 сек/цикл
#define FREQ_3_3Hz     152    // 3.30 Гц → 0.30 сек/цикл


void GPIO_init(void);

void RCC_init(void);

void Interrupt_init(void);

void systick_init(void);

#endif