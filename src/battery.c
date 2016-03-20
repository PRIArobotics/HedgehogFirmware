#include "battery.h"
#include <stdbool.h>
#include "motor.h"
#include "adc.h"
#include "output.h"


static uint8_t batteryLowCount;
static uint8_t batteryHighCount;
static bool batteryStatus; //false = emty


void battery_init()
{
	batteryLowCount = 0;
	batteryHighCount = 0;
	batteryStatus = true;
}


void battery_update()
{
	//motor pwm update
	motor_updateDutyCycle(0);
	motor_updateDutyCycle(1);
	motor_updateDutyCycle(2);
	motor_updateDutyCycle(3);

	//battery voltage monitoring beeper TODO: improve
	if(batteryStatus) //battery not emty
	{
		if(adc_getBatteryVoltage() < BATTERY_EMTY_THRESHOLD)
		{
			batteryLowCount++;
			if(batteryLowCount > 254)
			{
				batteryLowCount = 0;
				batteryStatus = false;
			}
		}
	}
	else //battery emty
	{
		if(adc_getBatteryVoltage() > BATTERY_NOTEMTY_THRESHOLD)
		{
			batteryHighCount++;
			if(batteryHighCount > 254)
			{
				batteryLowCount = 0;
				batteryStatus = true;
			}
		}
	}
	if(batteryStatus) buzzer(false);
	else buzzer(true);

	//TODO: ad power-off when battery completely emty
}
