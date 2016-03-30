#include "acp.h"
#include "uart.h"
#include "acp_opcodes.h"
#include "acp_commands.h"
#include "acp_errorcodes.h"
#include "acp_handler_impl.h"


static uint8_t connectionState;
static uint8_t opcode;
static acp_cmd_t cmd;
static size_t payloadLength;
static acp_conn_t acp_conn;


static void acp_receive(acp_conn_t conn);


void acp_init()
{
	acp_conn.rxBuffer = uart_getRxRingbuffer();
	acp_conn.txBuffer = uart_getTxRingbuffer();
	connectionState = WAIT_OPCODE;
	opcode = 0;
	payloadLength = 0;
	cmd = acp_cmds[ACP_UNUSED];
}


void acp_update()
{
	acp_receive(acp_conn);
	//TODO: send subscription responses
}


static void acp_receive(acp_conn_t conn)
{
	if(connectionState == WAIT_OPCODE)
	{
		if(ringbuffer_getFilled(conn.rxBuffer) < 1) return;
		opcode = ringbuffer_pop(conn.rxBuffer);
		cmd = acp_cmds[opcode];
//#ifdef ACP_UNUSED
		if(cmd.opcode == ACP_UNUSED)
		{
			acp_error(conn, ERR_UN_OPCODE, opcode);
			//error_recover_serial(serial); TODO
			return;
		}
//#endif
		if(!(cmd.flags & ACP_VPL_FLAG))
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
		cmd = acp_cmds[opcode];
		if(cmd.handler != NULL)
		{
			cmd.handler(conn, opcode, payloadLength);
		}
		else
		{
			//TODO somehow handle this... known, but unimplemented opcode
			acp_error(conn, ERR_UNSPECIFIED, opcode);
			//error_recover_serial(serial); TODO
			return;
		}
		//TODO: not needed anymore because max. payloadLength <= 255 ?
		if((cmd.flags & ACP_VPL_FLAG) == 1 && payloadLength == 255)
		{
			connectionState = WAIT_LENGTH;
		}
		else
		{
			connectionState = WAIT_OPCODE;
		}
	}
}
