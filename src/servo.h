#ifndef SERVO_H_
#define SERVO_H_


#include <stdint.h>
#include <stdbool.h>


#define SERVO_COUNT 6
#define SERVO_MIN_ONTIME 0
#define SERVO_MAX_ONTIME 40000


//initializes timer 4 and outputs for 4 servos
void servo_init();

//sets servo enabled, servo: 0-3, enabled: true or false
void servo_setEnabled(uint8_t servo, bool enabled);

//sets servo position, servo: 0-3, ontime: 0-40000
void servo_setOntime(uint8_t servo, uint16_t ontime);

//all servos off
void servo_allOff();


#endif /* SERVO_H_ */
