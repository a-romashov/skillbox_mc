#include "setup.h"
#include "stm32f10x.h"                  // Device header

void initLed(void) {
    //LED PC13
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    GPIOC->CRH &= ~GPIO_CRH_CNF13_0;
    GPIOC->CRH |= GPIO_CRH_MODE13_1;
}

void initTimer1(void) {
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; //enable timer 1
    TIM1->DIER |= TIM_DIER_UIE; // update interupt enable
    TIM1->CR1 |= TIM_CR1_CEN; // enable timer counter

    TIM1->PSC = 7999; // 8M / 8000 = 10 000
    TIM1->ARR = 999; // 10 000 / 1 000 interupts per sec

    NVIC_EnableIRQ(TIM1_UP_IRQn);
    NVIC_SetPriority(TIM1_UP_IRQn, 1);

}

void setup(void) {
    initLed();
    initTimer1();
}

