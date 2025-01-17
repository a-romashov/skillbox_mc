#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "stm32f4xx_hal_conf.h"         // Keil::Device:STM32Cube Framework:Classic


#include "../morse/morse.h"

void setup();
void enable();
void disable();
void wait(const uint8_t interval);

int main(void) {

    setup();
    
    MorseManager_i manager;
    manager.disable = &disable;
    manager.enable = &enable;
    manager.wait = &wait;
    
    while(1) {
        echoMorse("Hello, world!", 16, &manager);
    }
}
//LED PC13
void setup() {
    
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    GPIO_InitTypeDef config;
    config.Pin = GPIO_PIN_13;
    config.Mode = GPIO_MODE_OUTPUT_PP;
    config.Speed = GPIO_SPEED_FREQ_HIGH;
    config.Pull = GPIO_NOPULL;
    
    HAL_GPIO_Init(GPIOC, &config);
}

void enable(){
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}

void disable() {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}

void wait(const uint8_t interval) {
    for (volatile int i = 0; i < (2 << 17) * interval; i++);
}
