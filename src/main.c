#include <stdint.h>
#include "systick.h"
#include "gpio.h"
#include "output.h"
#include "digitalInput.h"
#include "servo.h"
#include "adc.h"
#include "motor.h"
#include "uart.h"

#include "ringbuffer.h"
#include <stdlib.h>

//#include <core_cm4.h>


int main()
{
	//NVIC_SetPriorityGrouping(0);
	systick_init();
	gpio_init();
	output_init();
	digitalInput_init();
	servo_init();
	adc_init();
	motor_init();
	uart_init();


	motor_setMode(0,MOTOR_MODE_FORWARD);

	ringbuffer_t *uartRxBuffer = uart_getRxRingbuffer();
	ringbuffer_t *uartTxBuffer = uart_getTxRingbuffer();

	while(1)
	{
		/*led1(true);
		led2(false);
		//speaker(500);
		servo_set(0,true,0);
		servo_set(1,true,0);
		servo_set(2,true,0);
		servo_set(3,true,0);
		for (i = 0; i < 10000000; i++);
		led1(false);
		led2(true);
		//speaker(2000);
		servo_set(0,true,2000);
		servo_set(1,true,2000);
		servo_set(2,true,2000);
		servo_set(3,true,2000);
		for (i = 0; i < 10000000; i++);*/
		
		if(!digitalInput_getState(0)) buzzer(true);
		else buzzer(false);

		uint16_t analogValue = adc_getAnalogInput(0);

		char str[6];
		itoa(analogValue, str, 10);
		str[4] = '\n';
		str[5] = '\r';
		if(ringbuffer_getFree(uartTxBuffer) > 0)
			ringbuffer_push_multiple(uartTxBuffer, (uint8_t*)str, sizeof(str));
		
		servo_set(0,true,analogValue/2);
		motor_setPower(0,analogValue/4);

		size_t filled = ringbuffer_getFilled(uartRxBuffer);
		if(filled > 5)
		{
			uint8_t data[filled];
			ringbuffer_pop_multiple(uartRxBuffer, data, filled);
			ringbuffer_push_multiple(uartTxBuffer, data, filled);
		}
		
		//servo_set(1,true,adc_getAnalogInput(1));
		//servo_set(2,true,adc_getAnalogInput(2));
		//servo_set(3,true,adc_getAnalogInput(3));
		//speaker(adc_getAnalogInput(4)*20);

		uint32_t i;
		for (i = 0; i < 1000000; i++);
	}

	return 0;
}
