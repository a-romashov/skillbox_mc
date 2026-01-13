#ifndef SKILLBOXMC_SETUP_H
#define SKILLBOXMC_SETUP_H

#ifndef UART_BAUD_RATE
#define UART_BAUD_RATE 115200
#endif

#include "K1986VE9xI.h"                 // Device header
#include "MDR32FxQI_port.h"             // Milandr::Drivers:PORT
#include "MDR32FxQI_rst_clk.h"          // Milandr::Drivers:RST_CLK
#include "MDR32FxQI_uart.h"             // Milandr::Drivers:UART

void setup(void);

#endif
