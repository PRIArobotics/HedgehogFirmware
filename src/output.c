#include "output.h"
#include "gpio.h"
#include <stm32f4xx.h>
#include "systick.h"
#include "power.h"


static gpio_pin_t pin_led_0 = {GPIOB,8};
static gpio_pin_t pin_led_1 = {GPIOB,9};
#ifndef HW_VERSION
	#error No HW_VERSION defined!
#elif ((HW_VERSION == 3) || (HW_VERSION == 4) || (HW_VERSION == 10))
	static gpio_pin_t pin_led_power = {GPIOB,10};
#elif (HW_VERSION == 2)
	static gpio_pin_t pin_led_power = {GPIOE,7};
#else
	#error Defined HW_VERSION is not supported by this firmware!
#endif
static gpio_pin_t pin_speaker_1 = {GPIOE,8};
static gpio_pin_t pin_speaker_2 = {GPIOE,9};

static power_led_mode_t power_led_mode = POWER_LED_MODE_OFF;


void output_init()
{
	gpio_pinCfg(pin_led_0, MODE_OUT|OTYPE_PP|SPEED_LOW, 0);
	gpio_pinCfg(pin_led_1, MODE_OUT|OTYPE_PP|SPEED_LOW, 0);
	gpio_pinCfg(pin_led_power, MODE_OUT|OTYPE_PP|SPEED_LOW, 0);

	gpio_pinCfg(pin_speaker_1, MODE_AF|OTYPE_PP|SPEED_LOW, 1); //speaker pin output, AF1
	gpio_pinCfg(pin_speaker_2, MODE_AF|OTYPE_PP|SPEED_LOW, 1); //speaker pin output, AF1

	//Timer 1 Init
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; //enable clock (42MHz)
	TIM1->PSC = 419; //100kHz after prescaler
	TIM1->CCMR1 |= (TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1M_1); //toggle pin on compare match
	TIM1->CCER |= (TIM_CCER_CC1NE | TIM_CCER_CC1E); //compare 1 outputs enable
	TIM1->BDTR |= TIM_BDTR_MOE; //main output enable
}


void led0(bool state)
{
	gpio_pinSet(pin_led_0, state);
}


void led1(bool state)
{
	gpio_pinSet(pin_led_1, state);
}

void powerLed(uint8_t mode)
{
	power_led_mode = mode;
}


void speaker(uint16_t frequency)
{
	if((frequency < 50) || (frequency > 15000))
	{
		TIM1->CR1 &= ~TIM_CR1_CEN; //disable timer
	}
	else
	{
		TIM1->CR1 &= ~TIM_CR1_CEN; //disable timer
		TIM1->CNT = 0; //reset counter value
		TIM1->ARR = (uint32_t)(100000/frequency+0.5); //set auto-reload value to control frequency
		TIM1->CR1 |= TIM_CR1_CEN; //enable timer
	}
}

void speaker_startup()
{
	speaker(500);
	systick_busyWait(systick_timeToTicks(0, 0, 0, 200)); //wait 0.2s
	speaker(2000);
	systick_busyWait(systick_timeToTicks(0, 0, 0, 100)); //wait 0.1s
	speaker(0);
}

void speaker_shutdown()
{
	speaker(2000);
	systick_busyWait(systick_timeToTicks(0, 0, 0, 200)); //wait 0.2s
	speaker(500);
	systick_busyWait(systick_timeToTicks(0, 0, 0, 100)); //wait 0.1s
	speaker(0);
}


void output_update(void)
{
	if(power_getBatteryStatus() == BATTERY_STATUS_LOW)
	{
		if(systick_getUptime() % SPEAKER_BAT_LOW_INTERVAL == 0)
			speaker(1000);
		if((systick_getUptime() - 100) % SPEAKER_BAT_LOW_INTERVAL == 0)
			speaker(0);
		if((systick_getUptime() - 200) % SPEAKER_BAT_LOW_INTERVAL == 0)
			speaker(1000);
		if((systick_getUptime() - 300) % SPEAKER_BAT_LOW_INTERVAL == 0)
			speaker(0);
		if((systick_getUptime() - 400) % SPEAKER_BAT_LOW_INTERVAL == 0)
			speaker(1000);
		if((systick_getUptime() - 500) % SPEAKER_BAT_LOW_INTERVAL == 0)
			speaker(0);

		if(power_led_mode == POWER_LED_MODE_ON) power_led_mode = POWER_LED_MODE_BLINK;
	}
	else if(power_getBatteryStatus() == BATTERY_STATUS_OK)
	{
		if(power_led_mode == POWER_LED_MODE_BLINK) power_led_mode = POWER_LED_MODE_ON;
	}

	switch(power_led_mode)
	{
		case POWER_LED_MODE_OFF:
			gpio_pinSet(pin_led_power, 0);
			break;
		case POWER_LED_MODE_ON:
			gpio_pinSet(pin_led_power, 1);
			break;
		case POWER_LED_MODE_BLINK:
			if(systick_getUptime() % 500 == 0)
				gpio_pinSet(pin_led_power, 1);
			if(systick_getUptime() % 1000 == 0)
				gpio_pinSet(pin_led_power, 0);
			break;
		case POWER_LED_MODE_BOOT: //TODO
			gpio_pinSet(pin_led_power, 1);
			break;
		case POWER_LED_MODE_SHUTDOWN: //TODO
			gpio_pinSet(pin_led_power, 0);
			break;
	}
}
