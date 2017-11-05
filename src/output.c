#include "output.h"
#include "gpio.h"
#include <stm32f4xx.h>
#include "systick.h"


static gpio_pin_t pin_led1 = {GPIOA,11};
static gpio_pin_t pin_led2 = {GPIOA,12};
static gpio_pin_t pin_speaker = {GPIOA,0};

static bool buzzer_enabled = false;
static bool short_beep = false;


void output_init()
{
	gpio_pinCfg(pin_led1, MODE_OUT|OTYPE_PP|SPEED_LOW, 0);
	gpio_pinCfg(pin_led2, MODE_OUT|OTYPE_PP|SPEED_LOW, 0);

	gpio_pinCfg(pin_speaker, MODE_OUT|OTYPE_PP|SPEED_LOW, 0); //buzzer pin output
	gpio_pinSet(pin_speaker,false);
}


void led1(bool state)
{
	gpio_pinSet(pin_led1, state);
}


void led2(bool state)
{
	gpio_pinSet(pin_led2, state);
}


void buzzer(bool enabled)
{
	buzzer_enabled = enabled;
}

void buzzer_update(void) //TODO: improve
{
	if(!short_beep)
	{
		if(systick_getUptime() % BUZZER_DELAY == 0)
			gpio_pinSet(pin_speaker,buzzer_enabled);
		if((systick_getUptime() - BUZZER_ON) % BUZZER_DELAY == 0)
			gpio_pinSet(pin_speaker,false);
	}
}

void buzzer_short_beep(void)
{
	short_beep = true;
	gpio_pinSet(pin_speaker,true);
	systick_busyWait(300);
	gpio_pinSet(pin_speaker,false);
	short_beep = false;
}
