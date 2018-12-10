#ifndef MOTOR_H_
#define MOTOR_H_


#include <stdint.h>
#include <stdbool.h>


#define MOTOR_COUNT 4
#define MOTOR_MAX_POWER 1000

#define MOTOR_VELOCITY_P 0.2
#define MOTOR_VELOCITY_I 0.2
#define MOTOR_VELOCITY_D 10.0

#define MOTOR_POSITION_P 10.0
#define MOTOR_POSITION_I 0.1
#define MOTOR_POSITION_D 0.0


//motor configurations
typedef enum motor_type_e
{
	MOTOR_TYPE_DC = 0,
	MOTOR_TYPE_ENC = 1,
	MOTOR_TYPE_STEP = 2
} motor_type_t;

//motor modes
typedef enum motor_mode_e
{
	MOTOR_MODE_POWER = 0,
	MOTOR_MODE_BRAKE = 1,
	MOTOR_MODE_VELOCITY = 2,
	MOTOR_MODE_POSITION = 3
} motor_mode_t;


//initializes motor outputs (uses TIM2), default motor type is DC
void motor_init();

//configures motor as:
//DC motor, motor: 0 to 3, enc_a/enc_b: 0
//DC motor with encoder, motor: 0 to 3, enc_a/b: 0 to 7
//stepper motor, motor: 0/2 (0 configures 0 & 1, 2 configures 2 & 3), enc_a/enc_b: 0
void motor_configure(uint8_t motor, motor_type_t type, uint8_t enc_a, uint8_t enc_b);

//set motor mode and power/speed/position, depending on type:
//DC motor: motorMode: POWER/BRAKE, power: -1000 to 1000
//DC motor with encoder: motorMode: POWER/BRAKE/VELOCITY/POSITION, power: -1000 to 1000 or velocity/position: -32.768 to 32,767
//stepper motor: motorMode: POWER/VELOCITY/POSITION, power: 0 or velocity/position: -32.768 to 32,767
void motor_set(uint8_t motor, motor_mode_t motorMode, int16_t power_velocity_position);

//all motors to 0 power
void motor_allOff();

#endif /* MOTOR_H_ */
