#ifndef ACP_OPCODES_H_
#define ACP_OPCODES_H_


//If all 256 opcodes are used, then ACP_UNUSED should not be defined, and
//code that checks for ACP_UNUSED should be disabled by using #ifdef ACP_UNUSED
#define ACP_UNUSED								255 //unused opcodes, reserved for future use

//No Operation
#define NOP										  0
//Analog Sensors
#define ANALOG_SENSOR_REQUEST					 10 //The Request says which ana port must be read for the mission
#define ANALOG_SENSOR_REPLY						 11 //The Answer for the Repl
#define ANALOG_SENSOR_SUBSCRIPTION				 12 //The Subscription says which ports must be updated every 100 ms
#define ANALOG_SENSOR_UPDATE					 13 //The update of the subscripted ports
#define ANALOG_PULLUP_ACTION					 14 //Activate the Pull-Ups on the defined ana-ports
//Digital Sensors
#define DIGITAL_SENSOR_REQUEST					 20 //The Request says which dig port must be read for the mission
#define DIGITAL_SENSOR_REPLY					 21 //The Answer for the Repl
#define DIGITAL_SENSOR_SUBSCRIPTION				 22 //The Subscription says which ports must be updated every 100 ms
#define DIGITAL_SENSOR_UPDATE					 23 //The update of the subscripted ports
#define DIGITAL_PULLUP_ACTION					 24 //Activate the Pull-Ups on the defined dig-ports
#define DIGITAL_OUTPUT_MODE_ACTION				 25
#define DIGITAL_OUTPUT_LEVEL_ACTION				 26
//Motor
#define MOTOR_POWER_ACTION						 30
#define MOTOR_BRAKE_ACTION						 37 //brake the motor
#define MOTOR_OFF_ACTION						 38 //turn motor off
//Servo
#define SERVO_ONOFF_ACTION						 50 //Enable or Disable the Servoports
#define SERVO_POSITION_ACTION					 51 //Move the Servo on a defined position
#define SERVO_POSITION_SYNC_ACTION				 52
//Battery
#define HWC_BATTERY_CHARGE_REQUEST				 60
#define HWC_BATTERY_CHARGE_REPLY				 61
#define HWC_BATTERY_CHARGE_UPDATE				 68
//Others
#define HWC_OFF_ACTION							 90
#define HWC_RESET_ACTION						 91
#define ERROR_ACTION							 96
//Controllers
#define HWC_TYPE_REQUEST						110
#define HWC_TYPE_REPLY							111
//external ports //TODO SPI?
#define EXTERNAL_PORT_CONNECT_ACTION			130
#define EXTERNAL_PORT_SEND_ACTION				131
#define EXTERNAL_PORT_RECEIVE_ACTION			132
//NOP2
#define NOP2									248 //Dirty hack that corrects for garbage sent, baud rate dependent!


#endif /* ACP_OPCODES_H_ */
