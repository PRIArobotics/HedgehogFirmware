#include <stm32f4xx.h>
 
int main()
{
	/* Enbale GPIOD clock */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	/* Configure GPIOD pin 12 as output */
	GPIOD->MODER |= (1 << (12 << 1));
 
	uint_32t i;

	/* Blink the LED on Servo0 Pin */
	while(1)
	{
		GPIOD->BSRR |= (1 << 12);
		for (i = 0; i < 5000000; i++);
		GPIOD->BSRR |= (1 << (12+16));
		for (i = 0; i < 5000000; i++);
	}

	return 0;
}
