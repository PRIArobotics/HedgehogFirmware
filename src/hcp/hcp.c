#include "hcp.h"
#include "uart.h"
#include "hcp_opcodes.h"
#include "hcp_commands.h"
#include "hcp_handler.h"


static uint8_t connectionState;
static uint8_t opcode;
static hcp_cmd_t cmd;
static size_t payloadLength;
static hcp_conn_t conn;


void hcp_init()
{
	conn.rxBuffer = uart_getRxRingbuffer();
	conn.txBuffer = uart_getTxRingbuffer();
	connectionState = WAIT_OPCODE;
	opcode = 0;
	payloadLength = 0;
}


void hcp_update()
{
	if(connectionState == WAIT_OPCODE)
	{
		if(ringbuffer_getFilled(conn.rxBuffer) < 1) return;
		opcode = ringbuffer_pop(conn.rxBuffer);
		cmd = hcp_cmds[opcode];
//#ifdef HCP_UNUSED
		if(cmd.opcode == HCP_UNUSED)
		{
			//TODO: send unknown opcode error message, what else to do?
			return;
		}
//#endif
		if(!(cmd.flags & HCP_VPL_FLAG))
		{
			payloadLength = cmd.payloadLength;
			connectionState = WAIT_PAYLOAD;
		}
		else
		{
			connectionState = WAIT_LENGTH;
		}

	}
	if(connectionState == WAIT_LENGTH)
	{
		if(ringbuffer_getFilled(conn.rxBuffer) < 1) return;
		payloadLength = ringbuffer_pop(conn.rxBuffer);
		connectionState = WAIT_PAYLOAD;
	}
	if(connectionState == WAIT_PAYLOAD)
	{
		if(ringbuffer_getFilled(conn.rxBuffer) < payloadLength) return;
		if(cmd.handler != NULL)
		{
			cmd.handler(conn, opcode, payloadLength);
		}
		else
		{
			//TODO: send unimplemented opcode error message, what else to do?
			return;
		}
		connectionState = WAIT_OPCODE;
	}

}
