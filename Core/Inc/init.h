#ifndef INIT_H
#define INIT_H

#include "stm32f4xx.h"

void Clock_Init_HSE_PLL_168MHz(void);

void GPIO_Init(void);
void ADC1_Init(void);
void TIM1_Encoder_Init(void);

uint16_t ADC_Read(void);

float Get_Pot_Percent(void);
void Update_LEDs(float perc);

#endif
