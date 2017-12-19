#ifndef POWER_H_
#define POWER_H_


#include <stdint.h>
#include <stdbool.h>


#define BATTERY_EMPTY_THRESHOLD 9.1 //battery voltage below which the power gets switched off
#define BATTERY_LOW_THRESHOLD 9.9 //battery voltage below which low battery indication occurs
#define BATTERY_VOLTAGE_HYSTERESIS 0.1

#define BATTERY_STATUS_EMPTY 0
#define BATTERY_STATUS_LOW 1
#define BATTERY_STATUS_OK 2


void power_init();
void power_on();
void power_off();
void power_regRpiEnable(bool state);
void power_regMsEnable(bool state);
bool power_getRegMsPG();
bool power_getButtonState();
bool power_getRPiActive();
bool power_getShutdown();
bool power_getEmergencyStop();
void power_clearEmergencyStop();
uint8_t power_getBatteryStatus();
void power_update();


#endif /* POWER_H_ */
