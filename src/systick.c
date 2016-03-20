#include "systick.h"
#include "system_stm32f4xx.h"
#include <stm32f4xx.h>
#include <stdbool.h>
#include "motor.h"
#include "adc.h"
#include "output.h"


static volatile uint64_t systick_count = 0;

uint8_t batteryLowCount = 0;
uint8_t batteryHighCount = 0;
bool batteryStatus = true; //false = emty


void systick_init()
{
	SysTick_Config(84000000 / 1000); //1ms period TODO: use SystemCoreClock variable instead of number
	NVIC_SetPriority(SysTick_IRQn, 15); //lowest interrupt priority
	systick_count = 0;
}


uint64_t systick_getUptime()
{
	return systick_count;
}


void systick_busyWait(uint32_t ticks)
{
	uint64_t end = systick_count + ticks;
	while(systick_count < end);
}


uint32_t systick_timeToTicks(uint16_t h, uint8_t m, uint8_t s, uint16_t ms)
{
	return (uint32_t)(3600000*h + 60000*m + 1000*s + ms);
}


void SysTick_Handler()
{
	systick_count++;

	//motor pwm update
	motor_updateDutyCycle(0);
	motor_updateDutyCycle(1);
	motor_updateDutyCycle(2);
	motor_updateDutyCycle(3);

	//battery monitoring TODO: improve, move to separate function
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
}
