#include <stdint.h>
#include "systick.h"
#include "power.h"
#include "gpio.h"
#include "digitalIO.h"
#include "output.h"
#include "servo.h"
#include "adc.h"
#include "motor.h"
#include "uart.h"
#include "hcp.h"
#include "imu.h"
//#include <core_cm4.h>


#define SHUTDOWN_TIMEOUT 20


bool rpi_was_active = false;


int main()
{
	//NVIC_SetPriorityGrouping(0); //TODO
	systick_init();
	gpio_init();
	adc_init();
	power_init();
	output_init();

	systick_busyWait(systick_timeToTicks(0, 0, 1, 0));

	power_on();
	powerLed(POWER_LED_MODE_BOOT);

	speaker_startup();

	power_regRpiEnable(true);
	power_regMsEnable(true);

	imu_init();
	digitalIO_init();
	servo_init();
	motor_init();
	uart_init();
	hcp_init();

	while(!power_getShutdown())
	{
		if(power_getRPiActive())
		{
			rpi_was_active = true;
			powerLed(POWER_LED_MODE_ON);
		}
		if(rpi_was_active && (!power_getRPiActive())) break;

		hcp_update();
	}

	hcp_sendShutdown();
	powerLed(POWER_LED_MODE_SHUTDOWN);
	speaker_shutdown();
	power_regMsEnable(false);

	uint64_t timeout = systick_getUptime() + systick_timeToTicks(0, 0, SHUTDOWN_TIMEOUT, 0);
	while((power_getRPiActive()) && (systick_getUptime() < timeout));

	power_regRpiEnable(false);
	power_off();

	return 0;
}
