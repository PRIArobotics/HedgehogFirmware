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

//TODO: make some variables constant or volatile
//TODO: use enums instead of defines for modes and status returns
//TODO: error checking everywhere the ringbuffer functions are used
//TODO: hcp handlers should use error codes from set-functions
int main()
{
	//NVIC_SetPriorityGrouping(0); //TODO interrupt priorities
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

	//FIXME: remove, testing:
	//motor_configure(0, MOTOR_TYPE_ENC, 0, 1);
	//motor_set(0, MOTOR_MODE_VELOCITY, 100);
	//motor_set(0, MOTOR_MODE_POSITION, 0);

	while(!power_getShutdown() && !power_getImmidiateShutdown())
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
	while((power_getRPiActive()) && (systick_getUptime() < timeout && !power_getImmidiateShutdown())); // wait for active pin to go low or timeout or button kept pressed after beep
	if(rpi_was_active && !power_getImmidiateShutdown()) systick_busyWait(systick_timeToTicks(0, 0, 8, 0)); // wait for RPi fully off

	power_regRpiEnable(false);
	powerLed(POWER_LED_MODE_OFF);
	power_off();

	return 0;
}
