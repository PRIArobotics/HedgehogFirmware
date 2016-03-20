#ifndef GPIO_H_
#define GPIO_H_


#include <stdint.h>
#include <stdbool.h>
#include <stm32f4xx.h>


//Defines to be used with gpio_pinCfg():

//Pin Mode, gets written to MODER
#define MODE_IN		0b00000000 //input (reset state)
#define MODE_OUT	0b00000001 //output
#define MODE_AF		0b00000010 //alternate function
#define MODE_ANA	0b00000011 //analog input
//Pin Output type, gets writtten to OTYPER
#define OTYPE_PP	0b00000000 //push-pull (reset-state)
#define OTYPE_OD	0b00000100 //open-drain
//Pin Speed, gets written to OSPEEDR
#define SPEED_LOW	0b00000000 //low speed (reset-state except for PB3 and PA13)
#define SPEED_MED	0b00001000 //medium speed
#define SPEED_FAST	0b00010000 //fast speed
#define SPEED_HIGH	0b00011000 //high speed (reset-state for PB3 and PA13)
//Pull-Up/Pull-Down, gets written to PUPDR
#define PULL_FL		0b00000000 //floating (reset-state except for PA13, PA14, PA15 and PB4)
#define PULL_PU		0b00100000 //pull-up (reset-state for PA13, PA15 and PB4)
#define PULL_PD		0b01000000 //pull-down (reset-state for PA14)


typedef struct {
	GPIO_TypeDef *port;
	uint8_t  pinNr;
} gpio_pin_t;


//initializes GPIO clocks
void gpio_init();

//configures pin, pin = struct containing port & pinNr, cfg = config (use defines above with | inbetween), af = alternate function 0...15 (ignored if pin not in af mode)
void gpio_pinCfg(gpio_pin_t pin, uint8_t cfg, uint8_t af);

//sets pin to high/low, pin = struct containing port & pinNr, state = true (high) or false (low)
void gpio_pinSet(gpio_pin_t pin, bool state);

//returns pin logic level: true (high) or false (low), pin = struct containing port & pinNr
bool gpio_pinGet(gpio_pin_t pin);


#endif /* GPIO_H_ */
