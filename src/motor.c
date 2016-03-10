#include "motor.h"
#include "gpio.h"
#include "adc.h"
#include <stm32f4xx.h>


static gpio_pin_t modePin = {GPIOC, 0};

static gpio_pin_t in1Pin[4] = {
	{GPIOC, 1},
	{GPIOC, 2},
	{GPIOC, 3},
	{GPIOC, 4}};

static gpio_pin_t in2Pin[4] = {
	{GPIOC, 6},
	{GPIOC, 7},
	{GPIOC, 8},
	{GPIOC, 9}};

static uint16_t setPower[4] = {0, 0, 0, 0}; //0-1000
static uint16_t dutyCycle[4] = {0, 0, 0, 0}; //0-1000, anything above is interpreted as 1000
static uint8_t mode[4] = {MOTOR_MODE_COAST, MOTOR_MODE_COAST, MOTOR_MODE_COAST, MOTOR_MODE_COAST}; 


void motor_init()
{
	//all motor pins to output
	gpio_pinCfg(modePin, MODE_OUT|OTYPE_PP|SPEED_LOW, 0);
	uint8_t i;
	for(i=0; i<4; i++) gpio_pinCfg(in1Pin[i], MODE_OUT|OTYPE_PP|SPEED_LOW, 0);
	for(i=0; i<4; i++) gpio_pinCfg(in2Pin[i], MODE_OUT|OTYPE_PP|SPEED_LOW, 0);

	gpio_pinSet(modePin, false); //motor driver mode 0

	//timer 3 init
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; //enable clock (84MHz)
	TIM3->PSC = 84; //prescaler = 84 --> 1MHz
	TIM3->ARR = 1000; //auto-reload value = 1000 --> 1kHz PWM frequency
	TIM3->CCMR1 |= (TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE); //channel 1+2 preload enable
	TIM3->CCMR2 |= (TIM_CCMR2_OC3PE | TIM_CCMR2_OC4PE); //channel 3+4 preload enable
	TIM3->DIER |= (TIM_DIER_UIE | TIM_DIER_CC1IE | TIM_DIER_CC2IE | TIM_DIER_CC3IE | TIM_DIER_CC4IE); //overflow & compare interrupts enable
	TIM3->CCR1 = dutyCycle[0]; //motor 0 dutyCycle 0-1000
	TIM3->CCR2 = dutyCycle[1]; //motor 1 dutyCycle 0-1000
	TIM3->CCR3 = dutyCycle[2]; //motor 2 dutyCycle 0-1000
	TIM3->CCR4 = dutyCycle[3]; //motor 3 dutyCycle 0-1000
	TIM3->CR1 |= TIM_CR1_CEN; //enable timer
	NVIC_EnableIRQ(TIM3_IRQn); //enable TIM3 global Interrupt
	NVIC_SetPriority(TIM3_IRQn, 0); //highest interrupt priority
}


void TIM3_IRQHandler(void)
{
	if(TIM3->SR & TIM_SR_CC1IF) //compare 1 interrupt flag, end of motor 0 pwm period
	{
		TIM3->SR &= ~TIM_SR_CC1IF;
		gpio_pinSet(in1Pin[0],false);
		gpio_pinSet(in2Pin[0],false);
	}
	if(TIM3->SR & TIM_SR_CC2IF) //compare 2 interrupt flag, end of motor 1 pwm period
	{
		TIM3->SR &= ~TIM_SR_CC2IF;
		gpio_pinSet(in1Pin[1],false);
		gpio_pinSet(in2Pin[1],false);
	}
	if(TIM3->SR & TIM_SR_CC3IF) //compare 3 interrupt flag, end of motor 2 pwm period
	{
		TIM3->SR &= ~TIM_SR_CC3IF;
		gpio_pinSet(in1Pin[2],false);
		gpio_pinSet(in2Pin[2],false);
	}
	if(TIM3->SR & TIM_SR_CC4IF) //compare 4 interrupt flag, end of motor 3 pwm period
	{
		TIM3->SR &= ~TIM_SR_CC4IF;
		gpio_pinSet(in1Pin[3],false);
		gpio_pinSet(in2Pin[3],false);
	}

	if(TIM3->SR & TIM_SR_UIF) //counter overflow, start of pwm periods
	{
		TIM3->SR &= ~TIM_SR_UIF; //reset interrupt flag

		uint8_t i;
		for(i = 0; i < 4; i++) //for each motor
		{
			motor_updateDutyCycle(i); //TODO: call this somewhere else (less often, maybe with battery voltage smoothing)

			if((mode[i] == MOTOR_MODE_COAST) || (dutyCycle[i] == 0)) //motor off
			{
				gpio_pinSet(in1Pin[i],false);
				gpio_pinSet(in2Pin[i],false);
			}
			else //motor on or breaking
			{
				if((mode[i] == MOTOR_MODE_BREAK) || (mode[i] == MOTOR_MODE_FORWARD)) gpio_pinSet(in1Pin[i],true);
				if((mode[i] == MOTOR_MODE_BREAK) || (mode[i] == MOTOR_MODE_REVERSE)) gpio_pinSet(in2Pin[i],true);
			}
		}
	}
}


void motor_setMode(uint8_t motor, uint8_t motorMode)
{
	if((motor > 3) || (motorMode > 3)) return; 
	mode[motor] = motorMode;
}


void motor_setPower(uint8_t motor, uint16_t power)
{
	if(motor > 3) return;
	if(power > 1000) power = 1000;
	setPower[motor] = power;
	motor_updateDutyCycle(motor);
}


void motor_updateDutyCycle(uint8_t motor) //FIXME: motor has nonlinear response to dutycycle ond/or voltage
{
	dutyCycle[motor] = (uint16_t)(setPower[motor] * MOTOR_MAX_VOLTAGE / adc_getBatteryVoltage() + 0.5); //correct for battery voltage
	if(dutyCycle[motor] >= 1000) dutyCycle[motor] = 1001; //1001 instead of 1000 to not get short pulse in PWM
	TIM3->CCR1 = dutyCycle[0];
	TIM3->CCR2 = dutyCycle[1];
	TIM3->CCR3 = dutyCycle[2];
	TIM3->CCR4 = dutyCycle[3];
}
