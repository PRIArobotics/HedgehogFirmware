#include <stdint.h>
#include <stm32f4xx.h>
#include "gpio.h"
#include "digitalInput.h"
 

#define PIN_SERVO0 (gpio_pin_t){GPIOD,12}


int main()
{
	gpio_init();
	digitalInput_init();

	gpio_pinCfg(PIN_SERVO0, MODE_OUT, 0);
 
	uint32_t i;

	/* Blink the LED on Servo0 Pin */
	while(1)
	{
		gpio_pinSet(PIN_SERVO0, true);
		for (i = 0; i < 1000000; i++);
		gpio_pinSet(PIN_SERVO0, false);
		for (i = 0; i < 1000000; i++);
	}

	return 0;
}
