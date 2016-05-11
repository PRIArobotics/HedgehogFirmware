#ifndef HCP_OPCODES_H_
#define HCP_OPCODES_H_


//unused opcodes, if all 256 opcodes are used, then ACP_UNUSED should not be defined
#define HCP_UNUSED			0xFF

//commands
#define HCP_IO_STATE        0x10
#define HCP_ANALOG_REQ      0x20
#define HCP_DIGITAL_REQ     0x30
#define HCP_MOTOR           0x40
#define HCP_SERVO           0x50
#define HCP_SERIAL          0x60

//replies
#define HCP_OK              0x80
#define HCP_INVALID_PORT    0x81
#define HCP_INVALID_IO      0x82
#define HCP_INVALID_MODE    0x83
#define HCP_INVALID_FLAGS   0x84
#define HCP_INVALID_VALUE   0x85
#define HCP_ANALOG_REP      0xA1
#define HCP_DIGITAL_REP     0xB1
#define HCP_SERIAL_UPDATE   0xE1


#endif /* HCP_OPCODES_H_ */
