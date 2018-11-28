#include "hcp.h"
#include "uart.h"
#include "hcp_opcodes.h"
#include "hcp_commands.h"
#include "hcp_handler.h"
#include "systick.h"
#include "power.h"


static uint8_t connectionState;
static uint8_t opcode;
static hcp_cmd_t cmd;
static uint8_t payloadLength;
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
		if(ringbuffer_pop(conn.rxBuffer, &opcode)) return; //get opcode if available
		cmd = hcp_cmds[opcode];
#ifdef HCP_UNUSED
		if(cmd.opcode == HCP_UNUSED) //opcode unknown
		{
			ringbuffer_push(conn.txBuffer, HCP_UNKNOWN_OPCODE);
			systick_busyWait(10); //wait 10ms
			ringbuffer_consume(conn.rxBuffer, ringbuffer_getFilled(conn.rxBuffer));
			return;
		}
#endif
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
		if(ringbuffer_pop(conn.rxBuffer, &payloadLength)) return; //get payloadLength if available
		connectionState = WAIT_PAYLOAD;
	}
	if(connectionState == WAIT_PAYLOAD)
	{
		if(ringbuffer_getFilled(conn.rxBuffer) < payloadLength) return;
		if(cmd.handler != NULL)
		{
			cmd.handler(conn, opcode, payloadLength);
		}
		else //opcode not implemented (e.g. reply codes)
		{
			ringbuffer_push(conn.txBuffer, HCP_UNSUPPORTED_OPCODE);
			ringbuffer_consume(conn.rxBuffer, payloadLength);
			return;
		}
		connectionState = WAIT_OPCODE;
	}

	if(power_getEmergencyStopSendFlag())
	{
		ringbuffer_push(conn.txBuffer, HCP_EMERGENCY_STOP);
		power_clearEmergencyStopSendFlag();
	}

}

void hcp_sendShutdown()
{
	ringbuffer_push(conn.txBuffer, HCP_SHUTDOWN);
}
