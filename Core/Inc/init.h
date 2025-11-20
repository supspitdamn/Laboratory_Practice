#ifndef INIT_H
#define INIT_H
#include <stdint.h>
#include "stm32f4xx.h" 

/*Для читаемости кода, проценты*/

#define TWENTY_PERCENT   819    // 4095 * 0.20 = 819
#define FOURTY_PERCENT   1638   // 4095 * 0.40 = 1638  
#define SIXTY_PERCENT    2457   // 4095 * 0.60 = 2457
#define EIGHTY_PERCENT   3276   // 4095 * 0.80 = 3276

/**/

void RCC_init(void);

void TIM1_init(void);

void GPIO_init(void);

void ADC_init(void);

uint16_t READ_POT(void);

#endif
