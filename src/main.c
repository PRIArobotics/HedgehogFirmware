#include <stdint.h>
#include "gpio.h"
#include "output.h"
#include "digitalInput.h"
#include "servo.h"
#include "adc.h"
#include "motor.h"
#include "uart.h"

#include "ringbuffer.h"


int main()
{
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

		motor_setPower(0,adc_getAnalogInput(0));
		
		servo_set(0,true,adc_getAnalogInput(0)*2);

		while(ringbuffer_getFilled(uartRxBuffer) > 0)
		{
			uint8_t data = ringbuffer_pop(uartRxBuffer);
			if(ringbuffer_getFree(uartTxBuffer) > 0)
				ringbuffer_push(uartTxBuffer, data);
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
