#ifndef POWER_H_
#define POWER_H_


#include <stdint.h>
#include <stdbool.h>


// voltages in mV
// voltages are a bit off (nonlinear behavior?)
// the comments contains the real voltage that the value corresponds to

#define BATTERY_VOLTAGE_HYSTERESIS 100

// battery voltage below which the battery is probably LiFe
// LiPo 3S: 11.1V, LiFe 2S: 6.6V (nominal voltage when fully charged)
#define BATTERY_LIFE_LIPO_LIMIT 6900 // 7.7V

// battery voltage below which low battery indication occurs
#define BATTERY_LIPO_LOW_THRESHOLD 1070 // 10.9V
// battery voltage below which the power gets switched off
#define BATTERY_LIPO_EMPTY_THRESHOLD 9500 // 9.7V

// note that the voltage regulator only goes down to 6V, so we should
// warn and shut down before coming too close to that
#define BATTERY_LIFE_LOW_THRESHOLD 5100 // 6.3V
#define BATTERY_LIFE_EMPTY_THRESHOLD 4900 // 6.1V


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
