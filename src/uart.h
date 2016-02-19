#ifndef UART_H_
#define UART_H_


#include <stdint.h>
#include <stdbool.h>


//initializes UART1
void uart_init();

//wait for emty transmit buffer and then send character
void uart_putChar(uint8_t ch);


#endif /* UART_H_ */