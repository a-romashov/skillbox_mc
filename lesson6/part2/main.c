#include "setup/setup.h"
#include "../blinker/blinker.h"
#include "stm32f10x.h"

void loop(void);

void wait(const uint16_t period);

volatile uint16_t counter = 0;
Blinker *blinker = (void *) 0;

int main(void) {

    setup();

    blinker = makeBlinker(&GPIOC->ODR, GPIO_ODR_ODR13);

    loop();
    return 0;
}


void loop(void) {
    while (1);
    blinkerDestroy(blinker);
}


void TIM1_UP_IRQHandler(void) {
    if (TIM1->SR & TIM_SR_UIF) {
        if (blinker) {
            blinkerToggle(blinker);
        }
        TIM1->SR &= ~TIM_SR_UIF;    // reset update interrupt flag
    }
}