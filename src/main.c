#include <stdint.h>
#include "gpio.h"
#include "output.h"
#include "digitalInput.h"
#include "servo.h"


int main()
{
	gpio_init();
	output_init();
	digitalInput_init();
	servo_init();


	uint32_t i;

	// led1&2, speaker and servo0-3 test
	while(1)
	{
		led1(true);
		led2(false);
		speaker(500);
		servo_set(0,true,0);
		servo_set(1,true,0);
		servo_set(2,true,0);
		servo_set(3,true,0);
		for (i = 0; i < 10000000; i++);
		led1(false);
		led2(true);
		speaker(2000);
		servo_set(0,true,2000);
		servo_set(1,true,2000);
		servo_set(2,true,2000);
		servo_set(3,true,2000);
		for (i = 0; i < 10000000; i++);
	}

	return 0;
}
