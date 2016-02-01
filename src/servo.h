#ifndef SERVO_H_
#define SERVO_H_


#include <stdint.h>
#include <stdbool.h>


//initializes timer 4 and outputs for 4 servos
void servo_init();

//sets servo enabled and position, servo: 0-3, enabled: trueor false, position: 0-2000
void servo_set(uint8_t servo, bool enabled, uint16_t position);


#endif /* SERVO_H_ */