#ifndef POWER_H_
#define POWER_H_


#include <stdint.h>
#include <stdbool.h>


//voltages in mV
#define BATTERY_VOLTAGE_HYSTERESIS 100
#define BATTERY_EMPTY_THRESHOLD_LIPO 9100 //battery voltage below which the power gets switched off
#define BATTERY_LOW_THRESHOLD_LIPO 9900 //battery voltage below which low battery indication occurs
#define BATTERY_EMPTY_THRESHOLD_LIFE 6000 //battery voltage below which the power gets switched off
#define BATTERY_LOW_THRESHOLD_LIFE 6200 //battery voltage below which low battery indication occurs


typedef enum battery_status_e
{
	BATTERY_STATUS_EMPTY,
	BATTERY_STATUS_LOW,
	BATTERY_STATUS_OK
} battery_status_t;


void power_init();
void power_on();
void power_off();
void power_regRpiEnable(bool state);
void power_regMsEnable(bool state);
bool power_getRegMsPG();
bool power_getButtonState();
bool power_getRPiActive();
bool power_getShutdown();
bool power_getImmidiateShutdown();
bool power_getEmergencyStop();
void power_setEmergencyStop(bool state);
bool power_getEmergencyStopSendFlag();
void power_setEmergencyStopSendFlag(bool state);
battery_status_t power_getBatteryStatus();
void power_update();
uint16_t power_getInputVoltage_mV();


#endif /* POWER_H_ */
