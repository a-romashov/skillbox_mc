#ifndef __SETUP_HEADER_
#define __SETUP_HEADER_

#ifndef USART_BAUD_RATE
#define USART_BAUD_RATE 115200
#endif



#include "stm32f10x.h"
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART

void setup(void);

#endif
