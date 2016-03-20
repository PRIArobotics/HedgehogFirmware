#include "systick.h"
#include <stm32f4xx.h>
#include "battery.h"


static volatile uint64_t systick_count = 0;


void systick_init()
{
	SysTick_Config(84000000 / 1000); //1ms period TODO: use SystemCoreClock variable instead of number
	NVIC_SetPriority(SysTick_IRQn, 15); //lowest interrupt priority
	systick_count = 0;
}


uint64_t systick_getUptime()
{
	return systick_count;
}


void systick_busyWait(uint32_t ticks)
{
	uint64_t end = systick_count + ticks;
	while(systick_count < end);
}


uint32_t systick_timeToTicks(uint16_t h, uint8_t m, uint8_t s, uint16_t ms)
{
	return (uint32_t)(3600000*h + 60000*m + 1000*s + ms);
}


void SysTick_Handler()
{
	systick_count++;
	battery_update();
}
