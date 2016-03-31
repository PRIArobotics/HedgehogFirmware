#ifndef ACP_COMMANDS_H_
#define ACP_COMMANDS_H_


#include <stddef.h>
#include <stdint.h>
#include "acp.h"


//variable payload flag
#define ACP_VPL_FLAG 0x01

#define ACP_NO_PAYLOAD 0


/**
 * An acp_handler_func is called with the serial connection receiving the command, the opcode being called, and the
 * payload length as parameters. acp_handler_func functions are called synchronously.
 * The serial connection's ring buffer's read pointer will be positioned at the first payload byte, i.e. opcode and
 * payload length byte (if any) will already be consumed. The handler is responsible for consuming all payload bytes.
 * For variable payload length opcodes, if the given payload length is 255, the function will be called again for the
 * same command, until all of the payload has been consumed. Between such calls, no other acp_handler_func will be invoked.
 * It is the function's responsibility to keep track of command continuation, if needed. As invocations are synchronous,
 * a static variable could be used for this purpose.
 */
typedef void (*acp_handler_func)(acp_conn_t conn, uint8_t opcode, size_t payloadLength);

typedef struct {
	uint8_t opcode; //the command's opcode
	uint8_t flags; //flags regarding the command
	size_t payloadLength; //the command's payload length, or 0 if variable
	acp_handler_func handler; //the command's handler function
} acp_cmd_t;

//array of commands, indexed by the opcode
extern acp_cmd_t acp_cmds[256];


#endif /* ACP_COMMANDS_H_ */
