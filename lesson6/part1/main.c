#include "stm32f10x.h"                  // Device header
#include "setup/setup.h"
#include "../blinker/blinker.h"

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
        wait(8);
    }
    blinkerDestroy(blinker);
}

void wait(uint16_t period) {
    counter = period;
    while (counter);
}

void SysTick_Handler() {
    counter && counter--;
}
//
