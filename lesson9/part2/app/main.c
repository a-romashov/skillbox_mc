#include "setup.h"

void loop(void);

void uartStringSend(char *str);

uint8_t strCmp(const char *expect, const char *actual);

char buffer[255];
uint8_t size = 0;
uint8_t finish = 0;
volatile uint8_t timer = 0;

int main(void) {
    setup();
    loop();
    return 0;
}


void enableLed() {
    PORT_SetBits(MDR_PORTA, PORT_Pin_1);
}

void disableLed() {
    PORT_ResetBits(MDR_PORTA, PORT_Pin_1);
}

void awaitFinish() {
    if (timer) {
        while (timer);
        finish = 1;
        buffer[size] = '\0';
    }
}

void loop(void) {
    disableLed();
    while (1) {
        if (finish) {
            finish = 0;
            size = 0;
            if (strCmp("hello", buffer) || strCmp("goodbye", buffer)) {
                uartStringSend(buffer);
            } else {
                uartStringSend("u wrote: ");
                uartStringSend(buffer);
            }
            uartStringSend("\n");
        }
        awaitFinish();
    }
}

void waitUartReady(void) {
    while (UART_GetFlagStatus(MDR_UART1, UART_FLAG_BUSY) == SET);
}

void uartStringSend(char *str) {
    for (uint8_t i = 0; str[i] != '\0'; i++) {
        waitUartReady();
        enableLed(); //на передачу включаем лампочку

        UART_SendData(MDR_UART1, str[i]);

        disableLed(); //после передачи выключаем лампочку
    }
}

void UART1_IRQHandler(void) {

    if (UART_GetITStatus(MDR_UART1, UART_IT_RX)) {
        buffer[size++] = UART_ReceiveData(MDR_UART1);
        timer = 4;
    }

}

void SysTick_Handler(void) {
    timer && timer--;
}

uint8_t strCmp(const char *expect, const char *actual) {
    for (uint8_t i = 0; i < 255; i++) {
        if (expect[i] == '\0') {
            return expect[i] == actual[i];
        } else if (expect[i] != actual[i]) {
            return 0;
        }
    }
    return 1;
}