#ifndef INIT_H
#define INIT_H
#include <stdint.h>
#include "stm32f4xx.h" 

/*Для читаемости кода, проценты*/

#define TWENTY_PERCENT 20
#define FOURTY_PERCENT 40
#define SIXTY_PERCENT 60
#define EIGHTY_PERCENT 80

/**/

void RCC_init(void);

void TIM1_init(void);

void GPIO_init(void);

void ADC_init(void);

uint16_t READ_POT(void);

#endif
