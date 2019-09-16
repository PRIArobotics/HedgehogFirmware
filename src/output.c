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

static bool speaker_playingSequence = false;
static uint16_t speaker_frequency = 0;

void output_init()
{
	gpio_pinCfg(pin_led_0, MODE_OUT|OTYPE_PP|SPEED_LOW, 0);
	gpio_pinCfg(pin_led_1, MODE_OUT|OTYPE_PP|SPEED_LOW, 0);
	gpio_pinCfg(pin_led_power, MODE_OUT|OTYPE_PP|SPEED_LOW, 0);

	gpio_pinCfg(pin_speaker_1, MODE_AF|OTYPE_PP|SPEED_LOW, 1); //speaker pin output, AF1
	gpio_pinCfg(pin_speaker_2, MODE_AF|OTYPE_PP|SPEED_LOW, 1); //speaker pin output, AF1

	//Timer 1 Init (speaker)
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; //enable clock (42MHz)
	TIM1->PSC = 419; //100kHz after prescaler
	TIM1->CCMR1 |= (TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1M_1); //toggle pin on compare match
	TIM1->CCER |= (TIM_CCER_CC1NE | TIM_CCER_CC1E); //compare 1 outputs enable
	TIM1->BDTR |= TIM_BDTR_MOE; //main output enable

	//Timer 5 Init (Power LED PWM)
	gpio_pinSet(pin_led_power, 0);
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN; //enable clock (84MHz)
	TIM5->PSC = 83; //prescaler = 84 --> 1MHz
	TIM5->ARR = 999; //auto-reload value = 999 --> 1kHz PWM frequency
	TIM5->CCMR1 |= TIM_CCMR1_OC1PE; //channel 1 preload enable
	TIM5->DIER |= (TIM_DIER_UIE | TIM_DIER_CC1IE); //overflow & ch1 compare interrupts enable
	TIM5->CCR1 = 0; // duty cycle 0
	TIM5->CR1 |= TIM_CR1_CEN; //enable timer
	NVIC_EnableIRQ(TIM5_IRQn); //enable TIM5 global Interrupt
	NVIC_SetPriority(TIM5_IRQn, 32); //interrupt priority
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


static void speaker(uint16_t frequency)
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

void speaker_setFrequency(uint16_t frequency)
{

	speaker_frequency = frequency;
	if(!speaker_playingSequence) speaker(speaker_frequency);
}

void speaker_startup()
{
	speaker_playingSequence = true;

	speaker(500);
	systick_busyWait(systick_timeToTicks(0, 0, 0, 200)); //wait 0.2s
	speaker(2000);
	systick_busyWait(systick_timeToTicks(0, 0, 0, 100)); //wait 0.1s
	speaker(0);

	speaker_playingSequence = false;
	speaker(speaker_frequency);
}

void speaker_shutdown()
{
	speaker_playingSequence = true;

	speaker(2000);
	systick_busyWait(systick_timeToTicks(0, 0, 0, 200)); //wait 0.2s
	speaker(500);
	systick_busyWait(systick_timeToTicks(0, 0, 0, 100)); //wait 0.1s
	speaker(0);

	speaker_playingSequence = false;
	speaker(speaker_frequency);
}


void output_update(void)
{
	if(power_getBatteryStatus() == BATTERY_STATUS_LOW)
	{
		if(systick_getUptime() % SPEAKER_BAT_LOW_INTERVAL == 0)
		{
			speaker_playingSequence = true;
			speaker(1000);
		}
		if((systick_getUptime() - 100) % SPEAKER_BAT_LOW_INTERVAL == 0)
			speaker(0);
		if((systick_getUptime() - 200) % SPEAKER_BAT_LOW_INTERVAL == 0)
			speaker(1000);
		if((systick_getUptime() - 300) % SPEAKER_BAT_LOW_INTERVAL == 0)
			speaker(0);
		if((systick_getUptime() - 400) % SPEAKER_BAT_LOW_INTERVAL == 0)
			speaker(1000);
		if((systick_getUptime() - 500) % SPEAKER_BAT_LOW_INTERVAL == 0)
		{
			speaker(0);
			speaker_playingSequence = false;
			speaker(speaker_frequency);
		}

		if(power_led_mode == POWER_LED_MODE_ON) power_led_mode = POWER_LED_MODE_BLINK;
	}
	else if(power_getBatteryStatus() == BATTERY_STATUS_OK)
	{
		if(power_led_mode == POWER_LED_MODE_BLINK) power_led_mode = POWER_LED_MODE_ON;
	}

	switch(power_led_mode) //TODO: add gamma correction
	{
		case POWER_LED_MODE_OFF:
			TIM5->CCR1 = 0;
			break;
		case POWER_LED_MODE_ON:
			TIM5->CCR1 = 1000;
			break;
		case POWER_LED_MODE_BLINK:
			if(systick_getUptime() % 500 == 0)
				TIM5->CCR1 = 1000;
			if(systick_getUptime() % 1000 == 0)
				TIM5->CCR1 = 0;
			break;
		case POWER_LED_MODE_BOOT:
			if(TIM5->CCR1 > 999) TIM5->CCR1 = 0;
			else TIM5->CCR1 += 1;
			break;
		case POWER_LED_MODE_SHUTDOWN:
			if(TIM5->CCR1 < 1) TIM5->CCR1 = 1000;
			else TIM5->CCR1 -= 1;
			break;
	}
}


void TIM5_IRQHandler(void)
{
	if(TIM5->SR & TIM_SR_UIF) //counter overflow, start of pwm periods
	{
		TIM5->SR &= ~TIM_SR_UIF; //reset interrupt flag
		if(TIM5->CCR1 > 0) gpio_pinSet(pin_led_power, 1);
	}
	if(TIM5->SR & TIM_SR_CC1IF) //compare 1, end of pwm period
	{
		TIM5->SR &= ~TIM_SR_CC1IF; //reset interrupt flag
		if(TIM5->CCR1 < 1000) gpio_pinSet(pin_led_power, 0);
	}
}

