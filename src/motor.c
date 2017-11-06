#include "motor.h"
#include <stm32f4xx.h>
#include "gpio.h"
#include "adc.h"

#include "output.h"


static gpio_pin_t pin_mode_01 = {GPIOC, 13};
static gpio_pin_t pin_mode_23 = {GPIOB, 5};

static gpio_pin_t pin_a[MOTOR_COUNT] = {
	{GPIOC, 9},
	{GPIOC, 11},
	{GPIOA, 8},
	{GPIOB, 10}};

static gpio_pin_t pin_b[MOTOR_COUNT] = {
	{GPIOC, 12},
	{GPIOC, 8},
	{GPIOC, 14},
	{GPIOC, 15}};

static bool pinStates[4][2] = {
	{false, false},
	{true, true},
	{true, false},
	{false, true}};

static uint16_t dutyCycle[MOTOR_COUNT] = {0, 0, 0, 0}; //0-MOTOR_MAX_POWER, anything above is interpreted as MOTOR_MAX_POWER
static uint8_t mode[MOTOR_COUNT] = {MOTOR_MODE_COAST, MOTOR_MODE_COAST, MOTOR_MODE_COAST, MOTOR_MODE_COAST};


void motor_init()
{
	//all motor pins to output
	gpio_pinCfg(pin_mode_01, MODE_OUT|OTYPE_PP|SPEED_LOW, 0);
	gpio_pinCfg(pin_mode_23, MODE_OUT|OTYPE_PP|SPEED_LOW, 0);
	uint8_t motor;
	for(motor=0; motor<MOTOR_COUNT; motor++) gpio_pinCfg(pin_a[motor], MODE_OUT|OTYPE_PP|SPEED_LOW, 0);
	for(motor=0; motor<MOTOR_COUNT; motor++) gpio_pinCfg(pin_b[motor], MODE_OUT|OTYPE_PP|SPEED_LOW, 0);

	gpio_pinSet(pin_mode_01, false); //motor driver mode 0
	gpio_pinSet(pin_mode_23, false); //motor driver mode 0

	//timer 3 init
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; //enable clock (84MHz)
	TIM3->PSC = 84; //prescaler = 84 --> 1MHz
	TIM3->ARR = MOTOR_MAX_POWER - 1; //auto-reload value = 999 --> 1kHz PWM frequency
	TIM3->CCMR1 |= (TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE); //channel 1+2 preload enable
	TIM3->CCMR2 |= (TIM_CCMR2_OC3PE | TIM_CCMR2_OC4PE); //channel 3+4 preload enable
	TIM3->DIER |= (TIM_DIER_UIE | TIM_DIER_CC1IE | TIM_DIER_CC2IE | TIM_DIER_CC3IE | TIM_DIER_CC4IE); //overflow & compare interrupts enable
	TIM3->CCR1 = dutyCycle[0]; //motor 0 dutyCycle 0-MOTOR_MAX_POWER
	TIM3->CCR2 = dutyCycle[1]; //motor 1 dutyCycle 0-MOTOR_MAX_POWER
	TIM3->CCR3 = dutyCycle[2]; //motor 2 dutyCycle 0-MOTOR_MAX_POWER
	TIM3->CCR4 = dutyCycle[3]; //motor 3 dutyCycle 0-MOTOR_MAX_POWER
	TIM3->CR1 |= TIM_CR1_CEN; //enable timer
	NVIC_EnableIRQ(TIM3_IRQn); //enable TIM3 global Interrupt
	NVIC_SetPriority(TIM3_IRQn, 0); //highest interrupt priority
}


void TIM3_IRQHandler(void)
{
	if(TIM3->SR & TIM_SR_UIF) //counter overflow, start of pwm periods
	{
		TIM3->SR &= ~TIM_SR_UIF; //reset interrupt flag

		uint8_t motor;
		for(motor = 0; motor < 4; motor++) //for each motor
		{
			if(dutyCycle[motor] == 0)
			{
				gpio_pinSet(pin_a[motor],false);
				gpio_pinSet(pin_b[motor],false);
			}
			else
			{
				gpio_pinSet(pin_a[motor],pinStates[mode[motor]][0]);
				gpio_pinSet(pin_b[motor],pinStates[mode[motor]][1]);
			}
		}
	}

	if(TIM3->SR & TIM_SR_CC1IF) //compare 1 interrupt flag, end of motor 0 pwm period
	{
		TIM3->SR &= ~TIM_SR_CC1IF;
		if(dutyCycle[0] < MOTOR_MAX_POWER)
		{
			gpio_pinSet(pin_a[0],false);
			gpio_pinSet(pin_b[0],false);
		}
	}
	if(TIM3->SR & TIM_SR_CC2IF) //compare 2 interrupt flag, end of motor 1 pwm period
	{
		TIM3->SR &= ~TIM_SR_CC2IF;
		if(dutyCycle[1] < MOTOR_MAX_POWER)
		{
			gpio_pinSet(pin_a[1],false);
			gpio_pinSet(pin_b[1],false);
		}
	}
	if(TIM3->SR & TIM_SR_CC3IF) //compare 3 interrupt flag, end of motor 2 pwm period
	{
		TIM3->SR &= ~TIM_SR_CC3IF;
		if(dutyCycle[2] < MOTOR_MAX_POWER)
		{
			gpio_pinSet(pin_a[2],false);
			gpio_pinSet(pin_b[2],false);
		}
	}
	if(TIM3->SR & TIM_SR_CC4IF) //compare 4 interrupt flag, end of motor 3 pwm period
	{
		TIM3->SR &= ~TIM_SR_CC4IF;
		if(dutyCycle[3] < MOTOR_MAX_POWER)
		{
			gpio_pinSet(pin_a[3],false);
			gpio_pinSet(pin_b[3],false);
		}
	}
}


void motor_setMode(uint8_t motor, uint8_t motorMode)
{
	if((motor >= MOTOR_COUNT) || (motorMode >= 4)) return;
	mode[motor] = motorMode;
}


void motor_setPower(uint8_t motor, uint16_t power)
{
	if(motor >= MOTOR_COUNT) return;
	if(power > MOTOR_MAX_POWER) power = MOTOR_MAX_POWER;
	dutyCycle[motor] = power;
	TIM3->CCR1 = dutyCycle[0];
	TIM3->CCR2 = dutyCycle[1];
	TIM3->CCR3 = dutyCycle[2];
	TIM3->CCR4 = dutyCycle[3];
}
