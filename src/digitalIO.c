#include "digitalIO.h"
#include "gpio.h"

static gpio_pin_t digitalPin[DIGITAL_COUNT] = {
	{GPIOD, 0},
	{GPIOD, 1},
	{GPIOD, 2},
	{GPIOD, 3},
	{GPIOD, 4},
	{GPIOD, 5},
	{GPIOD, 6},
	{GPIOD, 7},
	{GPIOE, 15},
	{GPIOE, 14},
	{GPIOE, 13},
	{GPIOE, 12},
	{GPIOE, 11},
	{GPIOE, 10},
	{GPIOE, 0},
	{GPIOE, 1}};


static uint8_t mode[DIGITAL_COUNT] = {
	PIN_MODE_IN_FLOATING,
	PIN_MODE_IN_FLOATING,
	PIN_MODE_IN_FLOATING,
	PIN_MODE_IN_FLOATING,
	PIN_MODE_IN_FLOATING,
	PIN_MODE_IN_FLOATING,
	PIN_MODE_IN_FLOATING,
	PIN_MODE_IN_FLOATING,
	PIN_MODE_IN_FLOATING,
	PIN_MODE_IN_FLOATING,
	PIN_MODE_IN_FLOATING,
	PIN_MODE_IN_FLOATING,
	PIN_MODE_IN_FLOATING,
	PIN_MODE_IN_FLOATING,
	PIN_MODE_IN_FLOATING,
	PIN_MODE_IN_FLOATING};


void digitalIO_init()
{
	uint8_t i;
	for(i=8; i<16; i++) digitalIO_setMode(i, mode[i]);
}


void digitalIO_setMode(uint8_t pin, uint8_t pinMode)
{
	if(pin >= DIGITAL_COUNT) return;

	mode[pin] = pinMode;

	if(pinMode == PIN_MODE_IN_FLOATING) gpio_pinCfg(digitalPin[pin], MODE_IN|PULL_FL, 0);
	else if(pinMode == PIN_MODE_IN_PULLUP) gpio_pinCfg(digitalPin[pin], MODE_IN|PULL_PU, 0);
	else if(pinMode == PIN_MODE_IN_PULLDOWN) gpio_pinCfg(digitalPin[pin], MODE_IN|PULL_PD, 0);
	else if(pinMode == PIN_MODE_OUT) gpio_pinCfg(digitalPin[pin], MODE_OUT|OTYPE_PP|SPEED_LOW, 0);
}


uint8_t digitalIO_getMode(uint8_t pin)
{
	return mode[pin];
}


bool digitalIO_getState(uint8_t pin)
{
	if(pin >= DIGITAL_COUNT) return 0;
	return gpio_pinGet(digitalPin[pin]);
}


void digitalIO_setState(uint8_t pin, bool state)
{
	if(pin >= DIGITAL_COUNT) return;
	return gpio_pinSet(digitalPin[pin], state);
}
