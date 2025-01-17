#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
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
        echoMorse("Hello, World!", 16, &manager);
    }
}

//LED PC13
void setup() {
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    GPIO_InitTypeDef initConfig;
    initConfig.GPIO_Mode = GPIO_Mode_Out_PP;
    initConfig.GPIO_Speed = GPIO_Speed_2MHz;
    initConfig.GPIO_Pin = GPIO_Pin_13;
    
    GPIO_Init(GPIOC, &initConfig);
    
}

void enable() {
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}
    
void disable() {
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
}
    
void wait(const uint8_t interval) {
    for (volatile int i = 0; i < (2 << 19) * interval; i++);
}
