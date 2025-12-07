#include "stm32f10x.h"                  // Device header

#include "setup/setup.h"

volatile int duty = 0;
volatile int sign = 1;

int main(void) {
    setup();
    while (1);
}

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;

        duty = (uint8_t)(duty + sign);
        if (duty >= 100 || duty <= 0) {
            sign *= -1;
        }

        TIM1->CCR1 = duty;
    }
}