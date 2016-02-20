#ifndef UART_H_
#define UART_H_


#include <stdint.h>
#include <stdbool.h>
#include "ringbuffer.h"


//initializes UART1
void uart_init();

//get rx and tx ringbuffer pointers
ringbuffer_t *uart_getRxRingbuffer();
ringbuffer_t *uart_getTxRingbuffer();


#endif /* UART_H_ */
