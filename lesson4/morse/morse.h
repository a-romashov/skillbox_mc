#ifndef MORSE_H
#define MORSE_H

#include "stdint.h"

typedef struct {
    void (*enable)(void);

    void (*disable)(void);

    void (*wait)(uint8_t interval);
} MorseManager_i;

void echoMorse(const char *message, uint8_t messageSizeLimit, const MorseManager_i *manager);

#endif