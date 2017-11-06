#ifndef ADC_H_
#define ADC_H_


#include <stdint.h>
#include <stdbool.h>


#define ANALOG_COUNT 16

#define MAX_BATTERY_VOLTAGE 112.2 //battery voltage when adc-value = 0xFFFF (voltage divider and 1/4 internal divider)


//init ADC for analog inputs and battery voltage
void adc_init();

//returns 12bit (0-4095) analog input value, input: 0-15
uint16_t adc_getAnalogInput(uint8_t input);

//returns battery voltage in V
float adc_getBatteryVoltage();

//returns battery voltage in mV
uint16_t adc_getBatteryVoltage_mV();


#endif /* ADC_H_ */
