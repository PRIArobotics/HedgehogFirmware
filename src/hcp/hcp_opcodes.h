#ifndef HCP_OPCODES_H_
#define HCP_OPCODES_H_


//unused opcodes, if all 256 opcodes are used, then ACP_UNUSED should not be defined
#define HCP_UNUSED				0xFF

//commands
#define HCP_IO_STATE        	0x10
#define HCP_ANALOG_REQ      	0x20
#define HCP_DIGITAL_REQ     	0x30
#define HCP_MOTOR           	0x40
#define HCP_SERVO           	0x50
#define HCP_SERIAL          	0x60

//replies
#define HCP_OK              	0x80
#define HCP_UNKNOWN_OPCODE 		0x81
#define HCP_UNSUPPORTED_OPCODE 	0x82
#define HCP_INVALID_PORT    	0x83
#define HCP_INVALID_IO      	0x84
#define HCP_INVALID_MODE    	0x85
#define HCP_INVALID_FLAGS   	0x86
#define HCP_INVALID_VALUE   	0x87
#define HCP_ANALOG_REP      	0xA1
#define HCP_DIGITAL_REP     	0xB1
#define HCP_SERIAL_UPDATE   	0xE1


//motor modes
#define HCP_MOTOR_MODE_POWER 0x00
#define HCP_MOTOR_MODE_BREAK 0x01


//special analog/digital ports
#define HCP_ANALOG_BATTERY_PORT 0x80
#define HCP_DIGITAL_LED1_PORT 	0x90
#define HCP_DIGITAL_LED2_PORT 	0x91
#define HCP_DIGITAL_BUZZER_PORT 0x92

//SPI PORTS
#define HCP_SPI_PORT_1			0x00
#define HCP_SPI_PORT_2			0x01


#endif /* HCP_OPCODES_H_ */
