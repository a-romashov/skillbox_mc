//
// Created by Андрей on 30.11.2025.
//

#ifndef SKILLBOXMC_BLINKER_H
#define SKILLBOXMC_BLINKER_H

#include "inttypes.h"

typedef struct {
    uint16_t pin;
    volatile uint32_t *ODR;
} Blinker;

Blinker *makeBlinker(volatile uint32_t *ODR, uint16_t pin);

void blinkerEnable(Blinker *blinker);

void blinkerDisable(Blinker *blinker);

void blinkerToggle(Blinker *blinker);

void blinkerDestroy(Blinker *blinker);

#endif //SKILLBOXMC_BLINKER_H
