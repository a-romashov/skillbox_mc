#include "setup/setup.h"
#include "../lesson4/morse/morse.h"

void enable();
void disable();
void wait(const uint8_t interval);
void setupManager();

volatile uint16_t timer = 0;
volatile uint16_t intervalModifier = 1;
volatile uint8_t app_stage = 0;
volatile uint8_t ext1Triggered = 0;

MorseManager_i manager;

int main() {

    setup();
    setupManager();

    while (1) {
        echoMorse("Hello, world!", 16, &manager);
    }

}

void wait(const uint8_t interval) {
    timer = (uint16_t) interval * intervalModifier;
    while (timer);
}

void setupManager() {

    manager.wait = &wait;

    switch (app_stage) {
        case STAGE_LED1:
        default:
            manager.disable = &disableLed1;
            manager.enable = &enableLed1;
            intervalModifier = INT_SPEED_NORMAL;
            break;

        case STAGE_LED1_FAST:
            manager.disable = &disableLed1;
            manager.enable = &enableLed1;
            intervalModifier = INT_SPEED_FAST;
            break;

        case STAGE_LED2:
            manager.disable = &disableLed2;
            manager.enable = &enableLed2;
            intervalModifier = INT_SPEED_NORMAL;
            break;

        case STAGE_LED_2_FAST:
            manager.disable = &disableLed2;
            manager.enable = &enableLed2;
            intervalModifier = INT_SPEED_FAST;
            break;

    }
}

////HANDLERS/////////////////////////////////////////////////////////

void SysTick_Handler() {
    timer && timer--;

    if (ext1Triggered) {
        ext1Triggered = 0;
        app_stage = (app_stage + 1) % 4;
        setupManager();
    }
}

void EXT_INT1_IRQHandler() {
    /**
     * Как оказалось зажатая кнопка постоянно вызывает эту функцию,
     * что-же, тогда отложим выполнение до следующего тика, ну и выключим лампочку, пока ждем
     */
    manager.disable();
    ext1Triggered = 1;
}