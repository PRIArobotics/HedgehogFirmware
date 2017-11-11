#include <stdint.h>
#include "systick.h"
#include "power.h"
#include "gpio.h"
#include "digitalIO.h"
#include "output.h"
#include "servo.h"
#include "adc.h"
#include "motor.h"
#include "uart.h"
#include "hcp.h"
//#include <core_cm4.h>


int main()
{
	//NVIC_SetPriorityGrouping(0); //TODO
	systick_init();
	gpio_init();
	power_init();
	output_init();

	systick_busyWait(systick_timeToTicks(0, 0, 1, 0)); //wait 1s

	power_on();
	powerLed(true); //TODO: blinking until RPi booted?

	speaker(500);
	systick_busyWait(systick_timeToTicks(0, 0, 0, 200)); //wait 0.2s
	speaker(2000);
	systick_busyWait(systick_timeToTicks(0, 0, 0, 100)); //wait 0.1s
	speaker(0);

	power_regRpiEnable(true);
	power_regMsEnable(true);

	digitalIO_init();
	servo_init();
	adc_init();
	motor_init();
	uart_init();
	hcp_init();

	while(!power_getShutdown())
	{
		power_update();
		output_update();
		hcp_update();
	}

	speaker(2000);
	systick_busyWait(systick_timeToTicks(0, 0, 0, 200)); //wait 0.2s
	speaker(500);
	systick_busyWait(systick_timeToTicks(0, 0, 0, 100)); //wait 0.1s
	speaker(0);

	powerLed(false); //TODO: blinking while RPi still active?
	power_regMsEnable(false);
	//TODO: wait for RPi shutdown?
	power_regRpiEnable(false);
	power_off();

	return 0;
}
