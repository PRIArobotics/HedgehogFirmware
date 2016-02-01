#include "digitalInput.h"
#include "gpio.h"

static gpio_pin_t digitalInputPin[8] = {
	{GPIOE, 8},
	{GPIOE, 9},
	{GPIOE, 10},
	{GPIOE, 11},
	{GPIOE, 12},
	{GPIOE, 13},
	{GPIOE, 14},
	{GPIOE, 15}};


void digitalInput_init()
{
	uint8_t i;
	for(i=0; i<8; i++) gpio_pinCfg(digitalInputPin[i], MODE_IN|PULL_PU, 0); //pins to input, with pullup
}


void digitalInput_setPullup(uint8_t input, bool enabled)
{
	if(input > 7) return;
	if(enabled) gpio_pinCfg(digitalInputPin[input], MODE_IN|PULL_PU, 0);
	else gpio_pinCfg(digitalInputPin[input], MODE_IN|PULL_FL, 0);
}


bool digitalInput_getState(uint8_t input)
{
	if(input > 7) return 0;
	return gpio_pinGet(digitalInputPin[input]);
}