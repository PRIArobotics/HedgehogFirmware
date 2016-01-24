#include <stdint.h>
#include "gpio.h"
#include "output.h"
#include "digitalInput.h"


int main()
{
	gpio_init();
	output_init();
	digitalInput_init();


	uint32_t i;

	/* Blink LED1 and LED2 */
	while(1)
	{
		led1(true);
		led2(false);
		speaker(500);
		for (i = 0; i < 10000000; i++);
		led1(false);
		led2(true);
		speaker(2000);
		for (i = 0; i < 10000000; i++);
	}

	return 0;
}
