#include "power.h"
#include "output.h"
#include "gpio.h"
#include "systick.h"
#include "adc.h"


uint64_t button_pressed_timestamp = 0;
bool shutdown = false;

static uint8_t batteryLowCount;
static uint8_t batteryHighCount;
static bool batteryStatus; //false = empty

static gpio_pin_t pin_enable_power_in = {GPIOD,10};
static gpio_pin_t pin_enable_reg_rpi = {GPIOD,8};
static gpio_pin_t pin_enable_reg_ms = {GPIOD,9};
static gpio_pin_t pin_power_button_state = {GPIOD,11};


void power_init()
{
	gpio_pinCfg(pin_enable_power_in, MODE_OUT|OTYPE_PP|SPEED_LOW, 0);
	gpio_pinCfg(pin_enable_reg_rpi, MODE_OUT|OTYPE_PP|SPEED_LOW, 0);
	gpio_pinCfg(pin_enable_reg_ms, MODE_OUT|OTYPE_PP|SPEED_LOW, 0);
	gpio_pinCfg(pin_power_button_state, MODE_IN, 0);

	batteryLowCount = 0;
	batteryHighCount = 0;
	batteryStatus = true;
}

void power_on()
{
	gpio_pinSet(pin_enable_power_in, true);
}

void power_off()
{
	gpio_pinSet(pin_enable_power_in, false);
}

void power_regRpiEnable(bool state)
{
	gpio_pinSet(pin_enable_reg_rpi, state);
}

void power_regMsEnable(bool state)
{
	gpio_pinSet(pin_enable_reg_ms, state);
}

bool power_getButtonState()
{
	return gpio_pinGet(pin_power_button_state);
}

bool power_getShutdown()
{
	return shutdown;
}

void power_update()
{
	//power button monitoring
	if(power_getButtonState())
	{
		if(button_pressed_timestamp == 0)
			button_pressed_timestamp = systick_getUptime();
		else if((systick_getUptime() - button_pressed_timestamp) > systick_timeToTicks(0, 0, 2, 0)) //button was pressed for 2s
			shutdown = true;
	}
	else button_pressed_timestamp = 0;

	//input voltage monitoring TODO: improve
	if(batteryStatus) //battery not empty
	{
		if(adc_getBatteryVoltage() < BATTERY_EMPTY_THRESHOLD)
		{
			batteryLowCount++;
			if(batteryLowCount > 254)
			{
				batteryLowCount = 0;
				batteryStatus = false;
			}
		}
	}
	else //battery empty
	{
		if(adc_getBatteryVoltage() > BATTERY_NOTEMPTY_THRESHOLD)
		{
			batteryHighCount++;
			if(batteryHighCount > 254)
			{
				batteryLowCount = 0;
				batteryStatus = true;
			}
		}
	}
//	if(batteryStatus) speaker(0); //TODO
//	else
//	{
//		speaker(200);
//		if(adc_getBatteryVoltage() < BATTERY_SHUTDOWN_THRESHOLD)
//			shutdown = true;
//	}
}
