#ifndef BATTERY_H_
#define BATTERY_H_

#include <stdint.h>


#define BATTERY_EMTY_THRESHOLD 6.4 //battery voltage below which the buzzer starts to beep
#define BATTERY_NOTEMTY_THRESHOLD  6.6 //battery voltage above which the buzzer stops to beep


//initialize battery monitoring
void battery_init();

//update battery monitoring
void battery_update();


#endif /* BATTERY_H_ */
