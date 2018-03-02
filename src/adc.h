#ifndef ADC_H_
#define ADC_H_


#include <stdint.h>
#include <stdbool.h>


#define ANALOG_COUNT 16

#if HW_VERSION==2
	#define MAX_INPUT_VOLTAGE 112200 //input voltage in mV when adc-value = 0xFFFF (voltage divider and 1/4 internal divider)
#else
	#define MAX_INPUT_VOLTAGE 79200 //input voltage in mV when adc-value = 0xFFFF (voltage divider and 1/4 internal divider)
#endif

//init ADC for analog inputs and battery voltage
void adc_init();

//returns 12bit (0-4095) analog input value, input: 0-15
uint16_t adc_getAnalogInput(uint8_t input);

//returns input voltage in mV
uint16_t adc_getInputVoltage_mV();


#endif /* ADC_H_ */
