#include "setup.h"

void loop(void);
void usartStringSend(char *str);
uint8_t strCmp(const char *expect, const char* actual);

char buffer[255];
uint8_t size = 0;
uint8_t finish = 0;

int main(void) {
    setup();
    loop();
    return 0;
}

void enableLed() {
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void disableLed() {
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void loop(void) {
    disableLed();

    while(1) {
        if (finish) {
            finish = 0;
            size = 0;
            if (strCmp("hello", buffer) || strCmp("goodbye", buffer)) {
                usartStringSend(buffer);
            } else {
                usartStringSend("u wrote: ");
                usartStringSend(buffer);
            }
            usartStringSend("\n");
        }
    }
}


void waitUsartTxReady(void) {
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}
void waitUsartTransferFinish(void) {
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

void usartStringSend(char *str) {
    for (uint8_t i = 0; str[i] != '\0'; i++) {
        waitUsartTxReady();
        enableLed(); //на передачу включаем лампочку

        USART_SendData(USART1, str[i]);

        waitUsartTransferFinish();
        disableLed(); //после передачи выключаем лампочку
    }
}

void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_RXNE)) {
        buffer[size++] = USART_ReceiveData(USART1);
    } else if (USART_GetITStatus(USART1, USART_IT_IDLE)) {
        buffer[size] = '\0'; //force end string
        finish = 1;
        USART_ReceiveData(USART1);//clear data register
    }

}

uint8_t strCmp(const char *expect, const char* actual) {
    for (uint8_t i = 0; i < 255; i++) {
        if (expect[i] == '\0'){
            return expect[i] == actual[i];
        } else if (expect[i] != actual[i]) {
            return 0;
        }
    }
    return 1;
}