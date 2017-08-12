#ifndef POWER_H_
#define POWER_H_


#include <stdint.h>
#include <stdbool.h>


#define BATTERY_SHUTDOWN_THRESHOLD 6.0 //battery voltage below which the power gets switched off
#define BATTERY_EMPTY_THRESHOLD 6.4 //battery voltage below which the buzzer starts to beep
#define BATTERY_NOTEMPTY_THRESHOLD  6.6 //battery voltage above which the buzzer stops to beep
#define BATTERY_FULL_VOLTAGE 8.4


void power_init();
void power_on();
void power_off();
void power_regRpiEnable(bool state);
void power_regMsEnable(bool state);
bool power_getButtonState();
bool power_getShutdown();
void power_update();


#endif /* POWER_H_ */
