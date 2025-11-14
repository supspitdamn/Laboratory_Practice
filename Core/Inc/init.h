#ifndef INIT_H

#include "stm32f4xx.h"
#include "stm32f429xx.h"
#include "stdio.h"

extern volatile uint32_t duration;

#define LONGDURATION  5000000
#define MEDIUMDURATION 2500000
#define SHORTDURATION 1000000

/**/

#define LOWFREQ 250000
#define MEDIUMFREQ 500000
#define HUGEFREQ 1000000

/**/

extern volatile uint32_t duration;

void macros_init(void);
void cmsis_init(void);
void simple_init(void);
uint32_t pressed_button(GPIO_TypeDef* port, uint32_t mask);

#endif