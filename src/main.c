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
#include "acp.h"
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
	acp_init();

	while(1)
	{
		acp_update();
	}

	return 0;
}
