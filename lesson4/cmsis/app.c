#include "stm32f10x.h"                  // Device header
#include "../morse/morse.h"

void setup();
void enable();
void disable();
void wait(uint8_t interval);



int main(void) {

    MorseManager_i manager;
    manager.disable = &disable;
    manager.enable = &enable;
    manager.wait = &wait;
    
    setup();
        
    while(1) {
        echoMorse("Hello, World!", 16, &manager);
    }
}

void setup() {
    //LED PC13
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    GPIOC->CRH &= ~GPIO_CRH_CNF13_0;
    GPIOC->CRH |= GPIO_CRH_MODE13_1;
}

void enable() {
    GPIOC->ODR &= ~(GPIO_ODR_ODR13);
}

void disable() {
    GPIOC->ODR |= GPIO_ODR_ODR13;
}

void wait(const uint8_t interval) {
    for (volatile int i = 0; i < (2 << 19) * interval;i++);
}


