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


void GPIO_init(void);

void RCC_init(void);

void Interrupt_init(void);

void systick_init(void);

#endif