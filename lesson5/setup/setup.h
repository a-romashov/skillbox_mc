#ifndef _F401_SETUP_SETUP_H
#define _F401_SETUP_SETUP_H
//---------------------------
#include "K1986VE9xI.h"                 // Device header
#include "MDR32FxQI_port.h"             // Milandr::Drivers:PORT
#include "MDR32FxQI_rst_clk.h"          // Milandr::Drivers:RST_CLK

typedef enum {
    STAGE_LED1 = 0,
    STAGE_LED1_FAST = 1,
    STAGE_LED2 = 2,
    STAGE_LED_2_FAST = 3,
} APP_STAGE;

typedef enum {
    INT_SPEED_NORMAL = 500,
    INT_SPEED_FAST = 150,
} intervalSpeed;

void setup();

void enableLed1();

void disableLed1();

void enableLed2();

void disableLed2();

//---------------------------
#endif