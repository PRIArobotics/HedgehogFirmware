#include "servo.h"
#include "gpio.h"
#include <stm32f4xx.h>


static gpio_pin_t servoPin[SERVO_COUNT] = {
	{GPIOC, 7},
	{GPIOC, 6},
	{GPIOD, 15},
	{GPIOD, 14},
	{GPIOD, 13},
	{GPIOD, 12}};

bool servoEnabled[SERVO_COUNT] = {0,0,0,0,0,0};
uint16_t servoPosition[SERVO_COUNT] = {0,0,0,0,0,0};

//TODO: change min and max duty cycle to use full range of motion of the servos
//TODO: maybe use soft-pwm for smoother current draw?
void servo_init()
{
	uint8_t i;
	for(i=0; i<SERVO_COUNT; i++) gpio_pinCfg(servoPin[i], MODE_AF|OTYPE_PP|SPEED_LOW, 2); //servo pins output, AF2

	//Timer 4 Init (servo 2-5)
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; //enable clock (84MHz)
	TIM4->PSC = 42; //prescaler = 42 --> 2MHz
	TIM4->ARR = 40000; //auto-reload value = 40000 --> 50Hz
	TIM4->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1); //channel 1+2 pwm mode 1
	TIM4->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1); //channel 3+4 pwm mode 1
	TIM4->CCMR1 |= (TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE); //channel 1+2 preload enable
	TIM4->CCMR2 |= (TIM_CCMR2_OC3PE | TIM_CCMR2_OC4PE); //channel 3+4 preload enable
	TIM4->CR1 |= TIM_CR1_ARPE; //Auto-reload preload enable
	TIM4->CCR1 = 0; //no pwm output
	TIM4->CCR2 = 0; //no pwm output
	TIM4->CCR3 = 0; //no pwm output
	TIM4->CCR4 = 0; //no pwm output
	TIM4->EGR |= TIM_EGR_UG; //update all registers
	TIM4->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E); //OC1-4 output enable
	TIM4->CR1 |= TIM_CR1_CEN; //enable timer

	//Timer 3 Init (servo 0-1)
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; //enable clock (84MHz)
	TIM3->PSC = 42; //prescaler = 42 --> 2MHz
	TIM3->ARR = 40000; //auto-reload value = 40000 --> 50Hz
	TIM3->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1); //channel 1+2 pwm mode 1
	TIM3->CCMR1 |= (TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE); //channel 1+2 preload enable
	TIM3->CR1 |= TIM_CR1_ARPE; //Auto-reload preload enable
	TIM3->CCR1 = 0; //no pwm output
	TIM3->CCR2 = 0; //no pwm output
	TIM3->EGR |= TIM_EGR_UG; //update all registers
	TIM3->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E); //OC1-2 output enable
	TIM3->CR1 |= TIM_CR1_CEN; //enable timer
}


void servo_update(uint8_t servo)
{
	if(servoEnabled[servo])
	{
		if     (servo == 0) TIM3->CCR2 = 2000 + servoPosition[0];
		else if(servo == 1) TIM3->CCR1 = 2000 + servoPosition[1];
		else if(servo == 2) TIM4->CCR4 = 2000 + servoPosition[2];
		else if(servo == 3) TIM4->CCR3 = 2000 + servoPosition[3];
		else if(servo == 4) TIM4->CCR2 = 2000 + servoPosition[4];
		else if(servo == 5) TIM4->CCR1 = 2000 + servoPosition[5];
	}
	else
	{
		if     (servo == 0) TIM3->CCR2 = 0;
		else if(servo == 1) TIM3->CCR1 = 0;
		else if(servo == 2) TIM4->CCR4 = 0;
		else if(servo == 3) TIM4->CCR3 = 0;
		else if(servo == 4) TIM4->CCR2 = 0;
		else if(servo == 5) TIM4->CCR1 = 0;
	}
}


void servo_setEnabled(uint8_t servo, bool enabled)
{
	if(servo >= SERVO_COUNT) return;
	servoEnabled[servo] = enabled;
	servo_update(servo);
}


void servo_setPosition(uint8_t servo, uint16_t position)
{
	if(servo >= SERVO_COUNT) return;
	servoPosition[servo] = position;
	servo_update(servo);
}
