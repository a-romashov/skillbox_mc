#include "setup.h"


#define SYS_TICK_FREQ 8000000


uint8_t buffer_dma_rx[BUFFER_SIZE_RX];

uint8_t buffer_dma_tx[BUFFER_SIZE_TX];


void setupUart1DmaTx(uint8_t count);

uint8_t *getBufferDmaRx(void) {
    return buffer_dma_rx;
}

void setBufferDmaTx(char *message) {
   
    int count = 0;
    while(message[count] != '\0' && count < BUFFER_SIZE_TX) {
        buffer_dma_tx[count] = (uint8_t) message[count];
        count++;
    }

    // Хотелось бы конечно заюзать DMA_ChannelReloadCycle, но что-то не пошло,
    // хотя параметр цикла имеется, есть подозрение, что не меняется конечный адрес памяти
    // видимо так делать нельзя, поэтому инициализируем все по новой
    setupUart1DmaTx(count);
    UART_DMACmd(MDR_UART1, UART_DMA_TXE, ENABLE);
}

void initSysTick(void) {

    SysTick->LOAD = SYS_TICK_FREQ / 1000 - 1;
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
}

void setupUserLed(void) {

    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTA, ENABLE);

    PORT_InitTypeDef led;
    led.PORT_Pin = PORT_Pin_1;
    led.PORT_SPEED = PORT_SPEED_FAST;
    led.PORT_OE = PORT_OE_OUT;
    led.PORT_MODE = PORT_MODE_DIGITAL;

    PORT_Init(MDR_PORTA, &led);

}


/**
 * Конфигурация прерываний UART1
 */
void setupIT(void) {

    NVIC_SetPriority(DMA_IRQn, 1);
    NVIC_SetPriority(SysTick_IRQn, 2);
    NVIC_EnableIRQ(DMA_IRQn);
    NVIC_ClearPendingIRQ(DMA_IRQn);
}


/**
 * Настройка UART1
 * Включаем тактирование UART1
 * Конфигурируем,
 * конфигурируем пины
 */
void setupUART1(void) {

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



    /**
     * Настройка Tx,Rx пинов
     * Для UART1 это пины 5(Tx), 6(Rx) на порту B
     * Включаем тактирование порта, и конфигурируем пины Tx, Rx
     */

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


void setupUart1DmaRx(void) {


    
    /**
     * Канал приема
     */
    DMA_CtrlDataInitTypeDef ctrlRx; // Контрольная структура описания приема-передачи DMA
    ctrlRx.DMA_SourceBaseAddr = (uint32_t) &MDR_UART1->DR; // источник данных
    ctrlRx.DMA_DestBaseAddr = (uint32_t) &buffer_dma_rx; // приемник данных
    ctrlRx.DMA_SourceIncSize = DMA_SourceIncNo; // счетчик данных источника (все чтение из DR регистра)
    ctrlRx.DMA_DestIncSize = DMA_DestIncByte; // тоже счетчик но для данных приемника
    ctrlRx.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // размер элемента пакета данных
    ctrlRx.DMA_CycleSize = BUFFER_SIZE_RX; // Размер пакета данных в MemoryDataSize
    ctrlRx.DMA_NumContinuous = DMA_Transfers_1; //кол-во кусочков переданных за раз
    ctrlRx.DMA_SourceProtCtrl = DMA_SourcePrivileged; // уровень доступа к памяти источника
    ctrlRx.DMA_DestProtCtrl = DMA_DestPrivileged; // уровень доступа к памяти приемника
    ctrlRx.DMA_Mode = DMA_Mode_Basic; // режим тут типа одна передача



    DMA_ChannelInitTypeDef channelRx; // Настройки канала передачи DMA
    DMA_StructInit(&channelRx);
    channelRx.DMA_PriCtrlData = &ctrlRx;
    channelRx.DMA_ProtCtrl = DMA_AHB_Privileged; // уровень доступа к памяти DMA

    DMA_Init(DMA_Channel_UART1_RX, &channelRx);
}

void setupUart1DmaTx(uint8_t count) {
    
        /**
     * Канал отправки
     */
    DMA_CtrlDataInitTypeDef ctrlTx;
    ctrlTx.DMA_SourceBaseAddr = (uint32_t) &buffer_dma_tx[0];
    ctrlTx.DMA_DestBaseAddr = (uint32_t) &MDR_UART1->DR;
    ctrlTx.DMA_SourceIncSize = DMA_SourceIncByte; 
    ctrlTx.DMA_DestIncSize = DMA_DestIncNo;
    ctrlTx.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    ctrlTx.DMA_CycleSize = count;
    ctrlTx.DMA_NumContinuous = DMA_Transfers_1;
    ctrlTx.DMA_SourceProtCtrl = DMA_SourcePrivileged;
    ctrlTx.DMA_DestProtCtrl = DMA_DestPrivileged;
    ctrlTx.DMA_Mode = DMA_Mode_Basic;



    DMA_ChannelInitTypeDef channelTx;
    DMA_StructInit(&channelTx);
    channelTx.DMA_PriCtrlData = &ctrlTx;
    channelTx.DMA_ProtCtrl = DMA_AHB_Privileged;
    
    DMA_Init(DMA_Channel_UART1_TX, &channelTx);
}

void setup(void) {
    initSysTick();
    setupUserLed();

    //Спасибо челику на хабре, что написал что отключенные SSP1, SSP2 будут ложные прерывания давать..
    RST_CLK_PCLKcmd(RST_CLK_PCLK_SSP1, ENABLE);
    RST_CLK_PCLKcmd(RST_CLK_PCLK_SSP2, ENABLE);
    RST_CLK_PCLKcmd(RST_CLK_PCLK_DMA, ENABLE);//тактирование DMA

    setupUART1();
    setupUart1DmaRx();

    UART_DMACmd(MDR_UART1, UART_DMA_RXE, ENABLE);
    UART_Cmd(MDR_UART1, ENABLE); // Включение UART1

    DMA_ClearError();

    setupIT(); //прерывания

}

