#include "output.h"
#include "gpio.h"
#include <stm32f4xx.h>
#include "systick.h"


static gpio_pin_t pin_led_1 = {GPIOB,8};
static gpio_pin_t pin_led_2 = {GPIOB,9};
static gpio_pin_t pin_led_power = {GPIOE,7};
static gpio_pin_t pin_speaker_1 = {GPIOE,8};
static gpio_pin_t pin_speaker_2 = {GPIOE,9};

//static bool buzzer_enabled = false;


void output_init()
{
	gpio_pinCfg(pin_led_1, MODE_OUT|OTYPE_PP|SPEED_LOW, 0);
	gpio_pinCfg(pin_led_2, MODE_OUT|OTYPE_PP|SPEED_LOW, 0);
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


void led1(bool state)
{
	gpio_pinSet(pin_led_1, state);
}


void led2(bool state)
{
	gpio_pinSet(pin_led_2, state);
}

void powerLed(bool state) //TODO: blinking
{
	gpio_pinSet(pin_led_power, state);
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


void output_update(void) //TODO: led blink, speaker stuff
{
	//if(systick_getUptime() % BUZZER_DELAY == 0)
	//	gpio_pinSet(pin_speaker,buzzer_enabled);
	//if((systick_getUptime() - BUZZER_ON) % BUZZER_DELAY == 0)
	//	gpio_pinSet(pin_speaker,false);
}
