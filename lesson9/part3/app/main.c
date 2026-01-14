#include "setup.h"

void loop(void);

void uartStringSend(char *str);

uint8_t strCmp(const char *expect, const char *actual);


#define OUTPUT_BUFFER_SIZE 32

char buffer[OUTPUT_BUFFER_SIZE];
uint8_t size = 0;
uint8_t finish = 0;

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
    }
}

void waitUartReady(void) {
    while(DMA_GetFlagStatus(DMA_Channel_UART1_TX, DMA_FLAG_CHNL_ENA) == SET);
}

void uartStringSend(char *str) {

     enableLed(); //на передачу включаем лампочку
     setBufferDmaTx(str);
     waitUartReady();
     disableLed(); //после передачи выключаем лампочку
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


void SysTick_Handler(void) {}

void DMA_IRQHandler(void) {

    // ну кто так делает(флаг то инвертирован)
    if (DMA_GetFlagStatus(DMA_Channel_UART1_RX, DMA_FLAG_DMA_ERR) == RESET) {

        DMA_ClearError();


    } else if (DMA_GetFlagStatus(DMA_Channel_UART1_RX, DMA_FLAG_CHNL_ENA) == RESET) {

        uint8_t toCopyBytes = 0;
        if (size + BUFFER_SIZE_RX + 1 >= OUTPUT_BUFFER_SIZE) {
            toCopyBytes = OUTPUT_BUFFER_SIZE - size - 1;
        } else {
            toCopyBytes = BUFFER_SIZE_RX;
        }
        uint8_t *dma_buffer = getBufferDmaRx();

        for (uint8_t i = 0; i < toCopyBytes; i++, size++) {
            buffer[size] = dma_buffer[i];

            // проверяем на наличие терминального символа
            if (buffer[size] == '\0') {
                finish = 1;
                break;
            }
        }

        // ну либо если буфер забит, то это конец
        if (size + 1 == OUTPUT_BUFFER_SIZE) {
            finish = 1;
            buffer[size] = '\0';
        }

        DMA_ChannelReloadCycle(DMA_Channel_UART1_RX, DMA_CTRL_DATA_PRIMARY, BUFFER_SIZE_RX, DMA_Mode_Basic);
        DMA_Cmd(DMA_Channel_UART1_RX, ENABLE);
        
    }  else if (DMA_GetFlagStatus(DMA_Channel_UART1_TX, DMA_FLAG_CHNL_ENA) == RESET) {
        
        UART_DMACmd(MDR_UART1, UART_DMA_TXE, DISABLE);
    }

}
