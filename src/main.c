#include <stdint.h>
#include "gpio.h"
#include "output.h"
#include "digitalInput.h"
#include "servo.h"
#include "adc.h"


int main()
{
	gpio_init();
	output_init();
	digitalInput_init();
	servo_init();
	adc_init();


	uint32_t i;

	//uint8_t n;
	//for(n=0; n<8; n++) adc_setPullup(n,true);

	// led1&2, speaker and servo0-3 test
	while(1)
	{
		/*led1(true);
		led2(false);
		//speaker(500);
		servo_set(0,true,0);
		servo_set(1,true,0);
		servo_set(2,true,0);
		servo_set(3,true,0);
		for (i = 0; i < 10000000; i++);
		led1(false);
		led2(true);
		//speaker(2000);
		servo_set(0,true,2000);
		servo_set(1,true,2000);
		servo_set(2,true,2000);
		servo_set(3,true,2000);
		for (i = 0; i < 10000000; i++);*/

		servo_set(0,true,adc_getAnalogInput(0));
		servo_set(1,true,adc_getAnalogInput(1));
		servo_set(2,true,adc_getAnalogInput(2));
		servo_set(3,true,adc_getAnalogInput(3));
		//speaker(adc_getAnalogInput(4)*20);
		for (i = 0; i < 100000; i++);
	}

	return 0;
}
