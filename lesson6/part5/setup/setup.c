#include "setup.h"
#include "stm32f10x.h"                  // Device header

void initLed(void) {
    //LED PA8
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRH = GPIO_CRH_CNF8_1 | GPIO_CRH_MODE8_1;
}

void initTimer1(void) {
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; //enable timer 1

    TIM1->CCMR1 = TIM_CCMR1_OC1PE | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0;
    TIM1->CCER = TIM_CCER_CC1E;
    TIM1->BDTR = TIM_BDTR_MOE;

    TIM1->CR1 |= TIM_CR1_CEN; // enable timer counter

    TIM1->PSC = 7999;
    TIM1->ARR = 99;
    TIM1->CCR1 = 100;
}

void initTimer2(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; //enable timer 2

    TIM2->DIER = TIM_DIER_UIE;

    TIM2->CR1 |= TIM_CR1_CEN; // enable timer counter

    TIM2->PSC = 7999;
    TIM2->ARR = 99;
    TIM2->CCR1 = 100;


    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 0);

}

void setup(void) {
    initLed();
    initTimer1();
    initTimer2();
}

