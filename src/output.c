#include "output.h"
#include "gpio.h"
#include <stm32f4xx.h>


static gpio_pin_t pin_led1 = {GPIOA,11};
static gpio_pin_t pin_led2 = {GPIOA,12};
static gpio_pin_t pin_speaker = {GPIOA,0};


void output_init()
{
	gpio_pinCfg(pin_led1, MODE_OUT|OTYPE_PP|SPEED_LOW, 0);
	gpio_pinCfg(pin_led2, MODE_OUT|OTYPE_PP|SPEED_LOW, 0);

	//gpio_pinCfg(pin_speaker, MODE_AF|OTYPE_PP|SPEED_LOW, 1); //speaker pin output, AF1
	gpio_pinCfg(pin_speaker, MODE_OUT|OTYPE_PP|SPEED_LOW, 0); //buzzer pin output
	gpio_pinSet(pin_speaker,false);

	//Timer 2 Init
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; //enable clock (42MHz)
	TIM2->PSC = 0; //no prescaler
	TIM2->CCMR1 |= (TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1M_1); //toggle pin on compare match
	TIM2->CCER |= TIM_CCER_CC1E; //compare 1 output enable
}


void led1(bool state)
{
	gpio_pinSet(pin_led1, state);
}


void led2(bool state)
{
	gpio_pinSet(pin_led2, state);
}


// void speaker(uint16_t frequency)
// {
// 	if((frequency < 50) || (frequency > 15000))
// 	{
// 		TIM2->CR1 &= ~TIM_CR1_CEN; //disable timer 2
// 	}
// 	else
// 	{
// 		TIM2->CR1 &= ~TIM_CR1_CEN; //disable timer 2
// 		TIM2->CNT = 0; //reset counter value
// 		TIM2->ARR = (uint32_t)(42000000/frequency+0.5); //set auto-reload value to control frequency
// 		TIM2->CR1 |= TIM_CR1_CEN; //enable timer 2
// 	}
// }

void buzzer(bool enabled)
{
	gpio_pinSet(pin_speaker,enabled);
}
