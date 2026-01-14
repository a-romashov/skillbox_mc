#ifndef SKILLBOXMC_SETUP_H
#define SKILLBOXMC_SETUP_H

#ifndef UART_BAUD_RATE
#define UART_BAUD_RATE 115200
#endif

#define BUFFER_SIZE_RX 1
#define BUFFER_SIZE_TX 1 << 8

#include "K1986VE9xI.h"                 // Device header
#include "MDR32FxQI_port.h"             // Milandr::Drivers:PORT
#include "MDR32FxQI_rst_clk.h"          // Milandr::Drivers:RST_CLK
#include "MDR32FxQI_dma.h"              // Milandr::Drivers:DMA
#include "MDR32FxQI_uart.h"             // Milandr::Drivers:UART


void setup(void);

uint8_t *getBufferDmaRx(void);
void setBufferDmaTx(char *message);

#endif
