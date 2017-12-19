#include "power.h"
#include "output.h"
#include "gpio.h"
#include "systick.h"
#include "adc.h"


static uint64_t button_pressed_timestamp = 0;
static bool button_initial_press = true;
static bool shutdown = false;
static bool emergency_stop_active = false;
static bool emergency_stop_send_flag = false;

static uint16_t input_voltage_mV = 12000;
static uint8_t batteryStatus = BATTERY_STATUS_OK;

static gpio_pin_t pin_enable_power_in = {GPIOD,10};
static gpio_pin_t pin_enable_reg_rpi = {GPIOD,8};
static gpio_pin_t pin_enable_reg_ms = {GPIOD,9};
static gpio_pin_t pin_pg_reg_ms = {GPIOB,2};
static gpio_pin_t pin_power_button_state = {GPIOD,11};
static gpio_pin_t pin_rpi_active = {GPIOC,10};


void power_init()
{
	gpio_pinCfg(pin_enable_power_in, MODE_OUT|OTYPE_PP|SPEED_LOW, 0);
	gpio_pinCfg(pin_enable_reg_rpi, MODE_OUT|OTYPE_PP|SPEED_LOW, 0);
	gpio_pinCfg(pin_enable_reg_ms, MODE_OUT|OTYPE_PP|SPEED_LOW, 0);
	gpio_pinCfg(pin_pg_reg_ms, MODE_IN, 0);
	gpio_pinCfg(pin_power_button_state, MODE_IN, 0);
	gpio_pinCfg(pin_rpi_active, MODE_IN|PULL_PD, 0);
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

bool power_getRegMsPG()
{
	return gpio_pinGet(pin_pg_reg_ms);
}

bool power_getButtonState()
{
	return gpio_pinGet(pin_power_button_state);
}

bool power_getRPiActive()
{
	return gpio_pinGet(pin_rpi_active);
}

bool power_getShutdown()
{
	return shutdown;
}

bool power_getEmergencyStop()
{
	return emergency_stop_active;
}

void power_clearEmergencyStop()
{
	emergency_stop_active = false;
}

bool power_getEmergencyStopSendFlag()
{
	return emergency_stop_send_flag;
}

void power_clearEmergencyStopSendFlag()
{
	emergency_stop_send_flag = false;
}

uint8_t power_getBatteryStatus()
{
	return batteryStatus;
}

uint16_t power_getInputVoltage_mV()
{
	return input_voltage_mV;
}

static void emergency_stop()
{
	emergency_stop_active = true;
	emergency_stop_send_flag = true;
	motor_allOff();
	servo_allOff();
}

void power_update()
{
	//power button monitoring
	if(power_getButtonState()) //button is pressed
	{
		if(!button_initial_press) //button is not still being held down from switching on
		{
			if(button_pressed_timestamp == 0) //has just started being pressed
			{
				button_pressed_timestamp = systick_getUptime();
				emergency_stop();
			}
			else if((systick_getUptime() - button_pressed_timestamp) > systick_timeToTicks(0, 0, 2, 0)) //button was pressed for 2s
				shutdown = true;
		}
	}
	else
	{
		button_initial_press = false;
		button_pressed_timestamp = 0;
	}

	//input voltage monitoring
	input_voltage_mV = (uint16_t)(input_voltage_mV * 0.9 + adc_getInputVoltage_mV() * 0.1 + 0.5);
	switch(batteryStatus)
	{
		case BATTERY_STATUS_OK:
			if(input_voltage_mV < BATTERY_LOW_THRESHOLD - BATTERY_VOLTAGE_HYSTERESIS) batteryStatus = BATTERY_STATUS_LOW;
			break;
		case BATTERY_STATUS_LOW:
			if(input_voltage_mV > BATTERY_LOW_THRESHOLD + BATTERY_VOLTAGE_HYSTERESIS) batteryStatus = BATTERY_STATUS_OK;
			if(input_voltage_mV < BATTERY_EMPTY_THRESHOLD - BATTERY_VOLTAGE_HYSTERESIS) batteryStatus = BATTERY_STATUS_EMPTY;
			break;
		case BATTERY_STATUS_EMPTY:
			shutdown = true;
			break;
	}
}
