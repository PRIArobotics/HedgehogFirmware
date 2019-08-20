#include "hcp_handler.h"
#include "hcp_opcodes.h"
#include "ringbuffer.h"
#include "power.h"

void hcp_handler_emergency(hcp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	if(opcode == HCP_EMERGENCY_ACTION)
	{
		uint8_t state;
		if(ringbuffer_pop(conn.rxBuffer, &state)) return;
		power_setEmergencyStop((bool)state);
		ringbuffer_push(conn.txBuffer, HCP_OK);
	}
	else if(opcode == HCP_EMERGENCY_REQ)
	{
		ringbuffer_push(conn.txBuffer, HCP_EMERGENCY_REP);
		ringbuffer_push(conn.txBuffer, (uint8_t)power_getEmergencyStop());
	}
}
