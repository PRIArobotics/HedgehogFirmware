#include "hcp_handler.h"
#include "hcp_opcodes.h"
#include "ringbuffer.h"
#include "power.h"

void hcp_handler_emergency(hcp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	power_clearEmergencyStop();
	ringbuffer_push(conn.txBuffer, HCP_OK);
}
