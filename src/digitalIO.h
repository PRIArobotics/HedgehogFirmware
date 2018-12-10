#ifndef DIGITALIO_H_
#define DIGITALIO_H_


#include <stdint.h>
#include <stdbool.h>


#define DIGITAL_COUNT 16


typedef enum pin_mode_e
{
	PIN_MODE_IN_FLOATING = 0,
	PIN_MODE_IN_PULLUP = 1,
	PIN_MODE_IN_PULLDOWN = 2,
	PIN_MODE_OUT = 3
} pin_mode_t;


//initializes all pins as floating inputs, call gpio_init() first
void digitalIO_init();

//sets pin mode (see defines above), input: digital input number 0...15, pinMode: see defines above
void digitalIO_setMode(uint8_t pin, pin_mode_t pinMode);

//returns pin mode (see defines above), input: digital input number 0...15, pinMode: see defines above
pin_mode_t digitalIO_getMode(uint8_t pin);

//returns state of (input) pin, input: digital input number 0...15
bool digitalIO_getState(uint8_t pin);

//sets state of (output) pin, input: digital input number 0...15, state: true (high) or false (low)
void digitalIO_setState(uint8_t pin, bool state);

//use input for motor encoder
void digitalIO_usePinEnc(uint8_t pin);

//input no longer used for anything
void digitalIO_freePin(uint8_t pin);


#endif /* DIGITALIO_H_ */
