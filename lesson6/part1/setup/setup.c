//
// Created by Андрей on 30.11.2025.
//

#include "setup.h"
#include "stm32f10x.h"                  // Device header
#include "RTE_Device.h"

#define SYS_TICK_FREQ RTE_HSI - 1

void initLed(void);

void initSysTick(void);

void initLed(void) {
    //LED PC13
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    GPIOC->CRH &= ~GPIO_CRH_CNF13_0;
    GPIOC->CRH |= GPIO_CRH_MODE13_1;
}

void initSysTick(void) {
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
    SysTick->LOAD = SYS_TICK_FREQ;
}

void setup(void) {
    initLed();
    initSysTick();
}

