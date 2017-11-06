#include "adc.h"
#include "gpio.h"
#include <stm32f4xx.h>


static gpio_pin_t pin_analog[ANALOG_COUNT] = {
	{GPIOA, 5},
	{GPIOA, 4},
	{GPIOA, 3},
	{GPIOA, 2},
	{GPIOA, 1},
	{GPIOA, 0},
	{GPIOC, 3},
	{GPIOC, 2},
	{GPIOA, 6},
	{GPIOA, 7},
	{GPIOC, 4},
	{GPIOC, 5},
	{GPIOB, 0},
	{GPIOB, 1},
	{GPIOC, 1},
	{GPIOC, 0}};

static uint16_t analogData[ANALOG_COUNT] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


void adc_init()
{
	//all adc pins to analog mode
	uint8_t i;
	for(i=0; i<ANALOG_COUNT; i++) gpio_pinCfg(pin_analog[i], MODE_ANA, 0);

	//ADC1 init
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; //enable clock for ADC1 (84MHz)
	ADC->CCR |= ADC_CCR_ADCPRE | ADC_CCR_VBATE; //prescaler = 8, vbat connected
	ADC1->CR1 &= ~ADC_CR1_RES; //12bit resolution
	ADC1->CR1 |= ADC_CR1_SCAN; //scan mode enabled
	ADC1->CR1 |= ADC_CR1_JAUTO; //automatic injection enabled
	ADC1->CR2 |= ADC_CR2_EOCS; //EOC bit is set at the end of each regular conversion
	ADC1->CR2 |= ADC_CR2_DDS; //DMA requests are issued as long as data are converted and DMA=1
	ADC1->CR2 |= ADC_CR2_DMA; //DMA mode enabled
	ADC1->CR2 |= ADC_CR2_CONT; //continuous mode enabled

	//sample time channel 1-9 = 480 cycles
	ADC1->SMPR2 |= ADC_SMPR2_SMP1
				| ADC_SMPR2_SMP2
				| ADC_SMPR2_SMP3
				| ADC_SMPR2_SMP4
				| ADC_SMPR2_SMP5
				| ADC_SMPR2_SMP6
				| ADC_SMPR2_SMP7
				| ADC_SMPR2_SMP8
				| ADC_SMPR2_SMP9;

	//sample time channel 10-16 & 18 (VBat) = 480 cycles
	ADC1->SMPR1 |= ADC_SMPR1_SMP10
				| ADC_SMPR1_SMP11
				| ADC_SMPR1_SMP12
				| ADC_SMPR1_SMP13
				| ADC_SMPR1_SMP14
				| ADC_SMPR1_SMP15
				| ADC_SMPR1_SMP16
				| ADC_SMPR1_SMP18;

	//regular sequence, channel 1-16
	ADC1->SQR1 = ((15<<20) | (10<<15) | (11<<10) | (9<<5) | (8<<0)); //16 conversions in sequence, conversion 16-13: 10, 11, 9, 8
	ADC1->SQR2 = ((15<<25) | (14<<20) | (7<<15) | (6<<10) | (12<<5) | (13<<0)); //conversion 12-7: 15, 14, 7, 6, 12, 13
	ADC1->SQR3 = ((0<<25) | (1<<20) | (2<<15) | (3<<10) | (4<<5) | (5<<0)); //conversion 6-1: 0, 1, 2, 3, 4, 5

	ADC1->JSQR = (18<<15); //injected sequence, channel 18 (VBat)

	ADC1->CR2 |= ADC_CR2_ADON; //adc enabled

	//DMA2 stream 0 init
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN; //DMA2 clock enabled
	DMA2->LIFCR |= 0b111101; //clear all stream 0 interrupts
	DMA2_Stream0->PAR = (uint32_t)&ADC1->DR; //adc1 data register as peripheral source
	DMA2_Stream0->M0AR = (uint32_t)analogData; //data array as memory destination
	DMA2_Stream0->NDTR = 16; //16 data items
	DMA2_Stream0->CR &= ~DMA_SxCR_CHSEL; //channel 0
	DMA2_Stream0->CR |= DMA_SxCR_PL; //highest priority
	DMA2_Stream0->CR |= DMA_SxCR_MSIZE_0; //16bit memory data size
	DMA2_Stream0->CR |= DMA_SxCR_PSIZE_0; //16bit peripheral data size
	DMA2_Stream0->CR |= DMA_SxCR_MINC; //memory address is incremented by 2bytes after each transfer
	DMA2_Stream0->CR |= DMA_SxCR_CIRC; //circular mode
	DMA2_Stream0->CR |= DMA_SxCR_EN; //stream enabled

	ADC1->CR2 |= ADC_CR2_SWSTART; //start adc conversion
}


uint16_t adc_getAnalogInput(uint8_t input)
{
	if(input >= ANALOG_COUNT) return 0;
	return analogData[input];
}


float adc_getBatteryVoltage() //FIXME reading from JDR crashes mcu
{
	uint16_t value = 500;
	//value = ADC1->JDR4;
	return ((float)(value) * MAX_BATTERY_VOLTAGE / 4095.0);
	return 12.0;
}

uint16_t adc_getBatteryVoltage_mV() //FIXME reading from JDR crashes mcu
{
	//return (uint16_t)((float)(ADC1->JDR4) * (MAX_BATTERY_VOLTAGE * 1000.0) / 4095.0);
	return 12000;
}
