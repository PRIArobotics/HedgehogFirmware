#ifndef SYSTICK_H_
#define SYSTICK_H_


#include <stdint.h>


//initializes systick with 1ms period
void systick_init();

//returns time since controller switched on in ms (not very accurate)
uint64_t systick_getUptime();

//wait certain number of ticks/ms  (not very accurate, max. ~1193h)
void systick_busyWait(uint32_t ticks);

//convert time to ticks (max. ~1193h)
uint32_t systick_timeToTicks(uint16_t h, uint8_t m, uint8_t s, uint16_t ms);


#endif /* SYSTICK_H_ */
