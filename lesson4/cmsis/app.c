#include "stm32f10x.h"                  // Device header
#include "../morse/morse.h"


void enable();
void disable();
void wait(uint8_t interval);
void setup();



int main(void) {

    MorsePrinter_i printer;
    printer.enable = &enable;
    printer.disable = &disable;
    printer.wait = &wait;
    
    setup();
        
    while(1) {
        echoMorse("Hello, world!", 16, &printer);
        wait(10);
    }
}

void setup() {
    //LED PC13
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    GPIOC->CRH &= ~GPIO_CRH_CNF13_0;
    GPIOC->CRH |= GPIO_CRH_MODE13_1;
}

void enable() {
    GPIOC->ODR &= ~GPIO_ODR_ODR13;
}

void disable() {
    GPIOC->ODR |= GPIO_ODR_ODR13;
}

void wait(uint8_t interval) {
    for (int i = 0; i < 300000 * interval;i++);
}


