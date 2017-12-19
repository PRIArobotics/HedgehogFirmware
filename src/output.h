#ifndef OUTPUT_H_
#define OUTPUT_H_


#include <stdint.h>
#include <stdbool.h>


//interval in systicks (ms) for battery low beeping
#define SPEAKER_BAT_LOW_INTERVAL 20000

#define POWER_LED_MODE_OFF 0
#define POWER_LED_MODE_ON 1
#define POWER_LED_MODE_BLINK 2
#define POWER_LED_MODE_BOOT 3
#define POWER_LED_MODE_SHUTDOWN 4

//initializes LED and speaker gpios and timer for speaker, call gpio_init() first
void output_init();

//switches LED1 on/off, state: true (on) or false (off)
void led1(bool state);

//switches LED2 on/off, state: true (on) or false (off)
void led2(bool state);

//controls power LED, mode: see defines above
void powerLed(uint8_t mode);

//sets speaker frequency, frequency: 50-15000Hz, anything else-->speaker off
void speaker(uint16_t frequency);

//play startup sound
void speaker_startup();

//play shutdown sound
void speaker_shutdown();

//call regularly, controls beeping and blinking patterns
void output_update(void);


#endif /* OUTPUT_H_ */
