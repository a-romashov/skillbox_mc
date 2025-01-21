#include "setup.h"

#define SYS_TICK_FREQ 8000000
#define LED_PORT RST_CLK_PCLK_PORTA
#define BTN_PORT RST_CLK_PCLK_PORTA

void initSysTick(const uint32_t freq) {

    SysTick->LOAD = freq / 1000 - 1;
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
}

void initIrq() {
    NVIC_EnableIRQ(EXT_INT1_IRQn);

    NVIC_SetPriority(EXT_INT1_IRQn, 1);

    NVIC_SetPriority(SysTick_IRQn, 2);
}

void initLed() {

    RST_CLK_PCLKcmd(LED_PORT, ENABLE);

    PORT_InitTypeDef config;
    config.PORT_Pin = PORT_Pin_1 | PORT_Pin_3;
    config.PORT_SPEED = PORT_SPEED_FAST;
    config.PORT_OE = PORT_OE_OUT;
    config.PORT_MODE = PORT_MODE_DIGITAL;
    config.PORT_FUNC = PORT_FUNC_PORT;

    PORT_Init(MDR_PORTA, &config);
}

void initBtn() {


    RST_CLK_PCLKcmd(BTN_PORT, ENABLE);

    PORT_InitTypeDef config;
    config.PORT_Pin = PORT_Pin_0;
    config.PORT_SPEED = PORT_SPEED_FAST;
    config.PORT_OE = PORT_OE_IN;
    config.PORT_MODE = PORT_MODE_DIGITAL;
    config.PORT_FUNC = PORT_FUNC_ALTER;
    config.PORT_PD_SHM = PORT_PD_SHM_ON;
    config.PORT_GFEN = PORT_GFEN_ON;

    PORT_Init(MDR_PORTA, &config);
}


void setup() {

    initSysTick(SYS_TICK_FREQ);
    initIrq();
    initLed();
    initBtn();
}


void enableLed1() {
    PORT_SetBits(MDR_PORTA, PORT_Pin_1);
}

void disableLed1() {
    PORT_ResetBits(MDR_PORTA, PORT_Pin_1);
}

void enableLed2() {
    PORT_SetBits(MDR_PORTA, PORT_Pin_3);
}

void disableLed2() {
    PORT_ResetBits(MDR_PORTA, PORT_Pin_3);
}