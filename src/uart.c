#include "uart.h"
#include "gpio.h"
#include <stm32f4xx.h>


static gpio_pin_t pinTxD = {GPIOA, 9};
static gpio_pin_t pinRxD = {GPIOA, 10};


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
}


void USART1_IRQHandler(void)
{
	if(USART1->SR & USART_SR_TC) //transmission complete
	{
		USART1->SR &= ~USART_SR_TC; //clear interrupt flag
	}
	if(USART1->SR & USART_SR_RXNE) //Rx data register not emty
	{
		USART1->SR &= ~USART_SR_RXNE; //clear interrupt flag
		uint8_t data = (uint8_t) USART1->DR; //read data
		uart_putChar(data); //test
	}
}


void uart_putChar(uint8_t ch) //TODO: make sure this doesn't interrupt fifo transmit
{
	while(!(USART1->SR & USART_SR_TXE)); //wait for emty transmit buffer
	USART1->DR = ch; //transmit data
}