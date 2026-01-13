#include "setup.h"

void setupUserLed() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef userLed;
    userLed.GPIO_Mode = GPIO_Mode_Out_OD;
    userLed.GPIO_Speed = GPIO_Speed_2MHz;
    userLed.GPIO_Pin = GPIO_Pin_13;
    
    GPIO_Init(GPIOC, &userLed);
}

/**
 * Настройка Tx,Rx пинов
 * Для USART1 это пины 9(Tx), 10(Rx) на порту А по умолчанию
 * Включаем тактирование порта, и конфигурируем пины Tx, Rx
 */
void setupRxTxUsart1() {
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    
    GPIO_InitTypeDef initTX;
    initTX.GPIO_Mode = GPIO_Mode_AF_PP;
    initTX.GPIO_Speed = GPIO_Speed_2MHz;
    initTX.GPIO_Pin = GPIO_Pin_9;
    
    GPIO_InitTypeDef initRX;
    initRX.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    initRX.GPIO_Speed = GPIO_Speed_2MHz;
    initRX.GPIO_Pin = GPIO_Pin_10;
    
    
    GPIO_Init(GPIOA, &initRX);
    GPIO_Init(GPIOA, &initTX);
    
}

/**
 * Конфигурация прерываний USART1
 */
void setupUsart1IT() {
    
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
   
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_SetPriority(USART1_IRQn, 1);
    
}


/**
 * Настройка USART1
 * Включаем тактирование USART1
 * Конфигурируем, конфигурируем пины, затем включаем USART
 * Настраиваем прерывания
 */
void setupUSART1() {

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //тактирование USART1
    
    
    USART_InitTypeDef initializer;
    initializer.USART_BaudRate = USART_BAUD_RATE;
    initializer.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    initializer.USART_WordLength = USART_WordLength_8b;
    initializer.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    initializer.USART_Parity = USART_Parity_No;
    initializer.USART_StopBits = USART_StopBits_1;
    
    USART_Init(USART1, &initializer); //Конфигурация USART1
    setupRxTxUsart1();
    
    
    USART_Cmd(USART1, ENABLE); // Включение USART1
    
    setupUsart1IT(); //прерывания
}

void setup(void) {
    setupUSART1();
    setupUserLed();
}
