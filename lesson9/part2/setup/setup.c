#include "setup.h"


#define SYS_TICK_FREQ 8000000

void setupUserLed() {

    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTA, ENABLE);

    PORT_InitTypeDef led;
    led.PORT_Pin = PORT_Pin_1;
    led.PORT_SPEED = PORT_SPEED_FAST;
    led.PORT_OE = PORT_OE_OUT;
    led.PORT_MODE = PORT_MODE_DIGITAL;

    PORT_Init(MDR_PORTA, &led);

}

/**
 * Настройка Tx,Rx пинов
 * Для UART1 это пины 5(Tx), 6(Rx) на порту B
 * Включаем тактирование порта, и конфигурируем пины Tx, Rx
 */
void setupRxTxUart1() {

    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB, ENABLE);


    PORT_InitTypeDef initRX;
    PORT_StructInit(&initRX);
    initRX.PORT_Pin = PORT_Pin_6;
    initRX.PORT_OE = PORT_OE_IN;
    initRX.PORT_MODE = PORT_MODE_DIGITAL;
    initRX.PORT_FUNC = PORT_FUNC_ALTER;
    initRX.PORT_SPEED = PORT_SPEED_FAST;


    PORT_InitTypeDef initTX;
    PORT_StructInit(&initTX);
    initTX.PORT_Pin = PORT_Pin_5;
    initTX.PORT_OE = PORT_OE_OUT;
    initTX.PORT_MODE = PORT_MODE_DIGITAL;
    initTX.PORT_FUNC = PORT_FUNC_ALTER;
    initTX.PORT_SPEED = PORT_SPEED_FAST;

    PORT_Init(MDR_PORTB, &initRX);
    PORT_Init(MDR_PORTB, &initTX);
}

/**
 * Конфигурация прерываний UART1
 */
void setupUart1IT() {

    UART_ITConfig(MDR_UART1, UART_IT_RX, ENABLE);

    NVIC_EnableIRQ(UART1_IRQn);
    NVIC_SetPriority(UART1_IRQn, 1);
    NVIC_SetPriority(SysTick_IRQn, 2);

}


/**
 * Настройка UART1
 * Включаем тактирование UART1
 * Конфигурируем, конфигурируем пины, затем включаем UART
 * Настраиваем прерывания
 */
void setupUART1() {

    RST_CLK_PCLKcmd(RST_CLK_PCLK_UART1, ENABLE);//тактирование UART1

    UART_BRGInit(MDR_UART1, UART_HCLKdiv1);

    UART_InitTypeDef initializer;
    initializer.UART_BaudRate = UART_BAUD_RATE;
    initializer.UART_FIFOMode = UART_FIFO_OFF;
    initializer.UART_HardwareFlowControl = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;
    initializer.UART_Parity = UART_Parity_No;
    initializer.UART_StopBits = UART_StopBits1;
    initializer.UART_WordLength = UART_WordLength8b;
    while (UART_Init(MDR_UART1, &initializer) != SUCCESS);


    setupRxTxUart1();
    setupUart1IT(); //прерывания


    UART_Cmd(MDR_UART1, ENABLE); // Включение UART1

}


void initSysTick() {

    SysTick->LOAD = SYS_TICK_FREQ / 1000 - 1;
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
}

void setup(void) {
    initSysTick();
    setupUART1();
    setupUserLed();
}
