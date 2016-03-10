#include "uart.h"
#include "gpio.h"
#include <stm32f4xx.h>


static gpio_pin_t pinTxD = {GPIOA, 9};
static gpio_pin_t pinRxD = {GPIOA, 10};


static ringbuffer_t uart_rx_rb;
static ringbuffer_t uart_tx_rb;
static uint8_t rx_buffer[256];
static uint8_t tx_buffer[256];


static void uart_startFifoTransmit();


void uart_init()
{
	gpio_pinCfg(pinTxD, MODE_AF|OTYPE_PP|SPEED_LOW, 7);
	gpio_pinCfg(pinRxD, MODE_AF, 7);

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; //enable UART1 clock (84MHz)
	USART1->BRR = (45<<4) | 4;//Baudrate = 115200, 42MHz / (8 * 115200) = 45.5729, 0.5729 * 7 = ~4 
	USART1->CR1 |= (USART_CR1_TCIE | USART_CR1_RXNEIE); //transmission complete & Rx not emty interrupts enable
	USART1->CR1 |= (USART_CR1_TE | USART_CR1_RE); //enable Tx & Rx
	USART1->CR1 |= USART_CR1_UE; //enable UART
	NVIC_EnableIRQ(USART1_IRQn); //enable UART1 global interrupts
	//NVIC_SetPriority(USART1_IRQn, 0x0B); //TODO: interrupt priorities

	ringbuffer_init(&uart_rx_rb, rx_buffer, sizeof(rx_buffer));
	ringbuffer_init(&uart_tx_rb, tx_buffer, sizeof(tx_buffer));
	ringbuffer_setStartReadFunction(&uart_tx_rb, &uart_startFifoTransmit);
}


void USART1_IRQHandler(void)
{
	if(USART1->SR & USART_SR_TC) //transmission complete
	{
		USART1->SR &= ~USART_SR_TC; //clear interrupt flag
		if(ringbuffer_getFilled(&uart_tx_rb) > 0) //more data to send available
			USART1->DR = ringbuffer_pop(&uart_tx_rb); //send data from buffer
	}

	if(USART1->SR & USART_SR_RXNE) //Rx data register not emty
	{
		USART1->SR &= ~USART_SR_RXNE; //clear interrupt flag
		if(ringbuffer_getFree(&uart_rx_rb) > 0) //free space available
			ringbuffer_push(&uart_rx_rb, USART1->DR); //write new data to buffer
		//else //buffer full //TODO: add buffer overflow error
	}
}


static void uart_startFifoTransmit()
{
	while(!(USART1->SR & USART_SR_TXE)); //wait until Tx data register is emty
	USART1->DR = ringbuffer_pop(&uart_tx_rb); //send first byte to start transmission
}


ringbuffer_t *uart_getRxRingbuffer()
{
	return &uart_rx_rb;
}

ringbuffer_t *uart_getTxRingbuffer()
{
	return &uart_tx_rb;
}
