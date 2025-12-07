#include "stm32f10x.h"                  // Device header

#include "setup/setup.h"

#define STEP 20

volatile int duty = 0;
volatile int sign = 1;
volatile uint8_t btnState = 0;

int main(void) {
    setup();
    while (1);
}

void EXTI0_IRQHandler(void) {
    if (EXTI->PR & EXTI_PR_PR0) {
        EXTI->PR |= EXTI_PR_PR0;

    }
    btnState = 1;
}


void TIM1_UP_IRQHandler(void) {
    if (TIM1->SR & TIM_SR_UIF) {
        TIM1->SR &= ~TIM_SR_UIF;

        if (btnState) {
            btnState = 0;
            duty = (uint8_t)(duty + STEP * sign);
            if (duty >= 100 || duty <= 0) {
                sign *= -1;
            }
        }

        TIM1->CCR1 = duty;
    }
}