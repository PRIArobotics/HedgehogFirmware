#ifndef ADC_H_
#define ADC_H_


#include <stdint.h>
#include <stdbool.h>


#define MAX_BATTERY_VOLTAGE 8.0 //battery voltage when adc-input = 3.3V


//inits ADC1 Channel 1-9 for analog inputs and battery voltage
void adc_init();

//enables or disables pullup of analog input, input: 0-7, enabled: true or false
void adc_setPullup(uint8_t input, bool enabled);

//returns 10bit (0-1023) analog input value, input: 0-7
uint16_t adc_getAnalogInput(uint8_t input);

//returns battery voltage
float adc_getBatteryVoltage();


#endif /* ADC_H_ */