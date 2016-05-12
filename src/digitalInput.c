#include "digitalInput.h"
#include "gpio.h"

static gpio_pin_t digitalInputPin[16] = {
	{GPIOA, 1},		//digital + analog
	{GPIOA, 2},		//digital + analog
	{GPIOA, 3},		//digital + analog
	{GPIOA, 4},		//digital + analog
	{GPIOA, 5},		//digital + analog
	{GPIOA, 6},		//digital + analog
	{GPIOA, 7},		//digital + analog
	{GPIOB, 0},		//digital + analog
	{GPIOE, 8},		//digital
	{GPIOE, 9}, 	//digital
	{GPIOE, 10}, 	//digital
	{GPIOE, 11}, 	//digital
	{GPIOE, 12}, 	//digital
	{GPIOE, 13}, 	//digital
	{GPIOE, 14}, 	//digital
	{GPIOE, 15}}; 	//digital


void digitalInput_init()
{
	uint8_t i;
	for(i=8; i<16; i++) gpio_pinCfg(digitalInputPin[i], MODE_IN|PULL_PU, 0); //pins to input, with pullup
}


void digitalInput_setPullup(uint8_t input, bool enabled)
{
	if(input >= DIGITAL_COUNT) return;
	if(enabled) gpio_pinCfg(digitalInputPin[input], MODE_IN|PULL_PU, 0);
	else gpio_pinCfg(digitalInputPin[input], MODE_IN|PULL_FL, 0);
}


bool digitalInput_getState(uint8_t input)
{
	if(input >= DIGITAL_COUNT) return 0;
	return gpio_pinGet(digitalInputPin[input]);
}
