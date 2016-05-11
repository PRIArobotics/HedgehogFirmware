#ifndef HCP_COMMANDS_H_
#define HCP_COMMANDS_H_


#include <stddef.h>
#include <stdint.h>
#include "hcp.h"


//variable payload flag
#define HCP_VPL_FLAG 0x01

#define HCP_NO_PAYLOAD 0


/**
 * An hcp_handler_func is called with the serial connection receiving the command, the opcode being called, and the
 * payload length as parameters. hcp_handler_func functions are called synchronously.
 * The serial connection's ring buffer's read pointer will be positioned at the first payload byte, i.e. opcode and
 * payload length byte (if any) will already be consumed. The handler is responsible for consuming all payload bytes.
 */
typedef void (*hcp_handler_func)(hcp_conn_t conn, uint8_t opcode, size_t payloadLength);

typedef struct {
	uint8_t opcode; //the command's opcode
	uint8_t flags; //flags regarding the command
	size_t payloadLength; //the command's payload length, or 0 if variable
	hcp_handler_func handler; //the command's handler function
} hcp_cmd_t;

//array of commands, indexed by the opcode
extern hcp_cmd_t hcp_cmds[256];


#endif /* HCP_COMMANDS_H_ */
