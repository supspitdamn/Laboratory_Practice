#include "Interrupt.h"

void TIM1_UP_TIM10_IRQHandler(void)
{
        TIM1->SR &= ~TIM_SR_UIF;
}

