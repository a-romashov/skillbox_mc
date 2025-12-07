#include "setup/setup.h"
#include "../blinker/blinker.h"
#include "stm32f10x.h"

void loop(void);

void wait(const uint16_t period);

volatile uint16_t counter = 0;

int main(void) {

    setup();
    loop();
    return 0;
}


void loop(void) {

    Blinker *blinker = makeBlinker(&GPIOC->ODR, GPIO_ODR_ODR13);
    while (1) {
        blinkerToggle(blinker);
        wait(10);
    }
    blinkerDestroy(blinker);
}

void wait(uint16_t period) {
    counter = period;
    while (counter);
}


void TIM1_UP_IRQHandler(void) {
    if (TIM1->SR & TIM_SR_UIF) {
        counter && counter--;
        TIM1->SR &= ~TIM_SR_UIF;    // reset update interrupt flag
    }
}