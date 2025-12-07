#include "setup.h"
#include "stm32f10x.h"                  // Device header

void initLed(void) {
    //LED PA8
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRH = GPIO_CRH_CNF8_1 | GPIO_CRH_MODE8_1;
}

void initBtn(void) {


    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    //BTN PA0
    GPIOA->CRL = GPIO_CRL_CNF0_1;
    GPIOA->ODR |= GPIO_ODR_ODR0;

    AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI0_PA;

    //EXTI->RTSR |= EXTI_RTSR_TR0;
    EXTI->FTSR |= EXTI_FTSR_TR0;
    EXTI->IMR |= EXTI_IMR_MR0;

    NVIC_EnableIRQ(EXTI0_IRQn);
}

void initTimer1(void) {
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; //enable timer 1

    TIM1->CCMR1 = TIM_CCMR1_OC1PE | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0;
    TIM1->CCER = TIM_CCER_CC1E;
    TIM1->BDTR = TIM_BDTR_MOE;
    TIM1->DIER = TIM_DIER_UIE;

    TIM1->CR1 |= TIM_CR1_CEN; // enable timer counter

    TIM1->PSC = 7999;
    TIM1->ARR = 99;
    TIM1->CCR1 = 100;


    NVIC_EnableIRQ(TIM1_UP_IRQn);

}

void setup(void) {
    initLed();
    initBtn();
    initTimer1();

    NVIC_SetPriority(TIM1_UP_IRQn, 1);
    NVIC_SetPriority(EXTI0_IRQn, 0);
}

