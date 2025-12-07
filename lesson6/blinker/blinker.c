//
// Created by Андрей on 30.11.2025.
//
#include "blinker.h"
#include "stdlib.h"

Blinker *makeBlinker(volatile uint32_t *ODR, uint16_t pin) {
    Blinker *result = (Blinker *) malloc(sizeof(Blinker));
    result->ODR = ODR;
    result->pin = pin;
    return result;
}

void blinkerEnable(Blinker *blinker) {
    *blinker->ODR &= ~(blinker->pin);
}

void blinkerDisable(Blinker *blinker) {
    *blinker->ODR |= blinker->pin;
}

void blinkerToggle(Blinker *blinker) {
    *blinker->ODR & blinker->pin
    ? blinkerEnable(blinker)
    : blinkerDisable(blinker);
}

void blinkerDestroy(Blinker *blinker) {
    free(blinker);
}
