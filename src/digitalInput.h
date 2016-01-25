#ifndef DIGITALINPUT_H_
#define DIGITALINPUT_H_


#include <stdint.h>
#include <stdbool.h>


//initializes input pins as inputs without pullup, call gpio_init() first
void digitalInput_init();

//switches pullup on/off, input: digital input number 0...7, enabled: true (pullup on) or false (pullup off / floating)
void digitalInput_setPullup(uint8_t input, bool enabled);

//returns state of input: true (high) or false (low), input: digital input number 0...7
bool digitalInput_getState(uint8_t input);


#endif /* DIGITALINPUT_H_ */