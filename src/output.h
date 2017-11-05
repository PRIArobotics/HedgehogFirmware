#ifndef OUTPUT_H_
#define OUTPUT_H_


#include <stdint.h>
#include <stdbool.h>


//times in systicks (ms) for battery empty beeping
#define BUZZER_DELAY 30000
#define BUZZER_ON 2000


//initializes LED and speaker gpios and timer for speaker, call gpio_init() first
void output_init();

//switches LED1 on/off, state: true (on) or false (off)
void led1(bool state);

//switches LED2 on/off, state: true (on) or false (off)
void led2(bool state);

//sets speaker frequency, frequency: 50-15000Hz, anything else-->speaker off
//void speaker(uint16_t frequency);

//switches the piezo-buzzer on/off, state: true (on) or false (off)
void buzzer(bool enabled); 

//call regularly, controls the beeping pattern
void buzzer_update(void);

//beeps for 300ms, blocking
void buzzer_short_beep(void);


#endif /* OUTPUT_H_ */
