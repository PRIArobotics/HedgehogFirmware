#include <stdint.h>
#include "systick.h"
#include "gpio.h"
#include "output.h"
#include "digitalInput.h"
#include "servo.h"
#include "adc.h"
#include "motor.h"
#include "uart.h"
#include "battery.h"
#include "hcp.h"
//#include <core_cm4.h>


int main()
{
	//NVIC_SetPriorityGrouping(0); //TODO: change this (maybe helpful for fix_me in motor.c)
	gpio_init();
	output_init();
	digitalInput_init();
	servo_init();
	adc_init();
	motor_init();
	uart_init();
	battery_init();
	systick_init();
	hcp_init();

	while(1)
	{
		hcp_update();
	}

	return 0;
}
