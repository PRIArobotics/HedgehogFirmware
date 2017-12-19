#ifndef SERVO_H_
#define SERVO_H_


#include <stdint.h>
#include <stdbool.h>


#define SERVO_COUNT 6
#define SERVO_MAX_POSITION 2000


//initializes timer 4 and outputs for 4 servos
void servo_init();

//sets servo enabled, servo: 0-3, enabled: true or false
void servo_setEnabled(uint8_t servo, bool enabled);

//sets servo position, servo: 0-3, position: 0-2000
void servo_setPosition(uint8_t servo, uint16_t position);

//all servos off
void servo_allOff();


#endif /* SERVO_H_ */
