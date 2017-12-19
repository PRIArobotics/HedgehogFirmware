#include <digitalIO.h>
#include "hcp_handler.h"
#include "hcp_opcodes.h"
#include "ringbuffer.h"


void hcp_handler_digitalReq(hcp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	uint8_t port = ringbuffer_pop(conn.rxBuffer);

	if(port >= DIGITAL_COUNT)
	{
		ringbuffer_push(conn.txBuffer, HCP_INVALID_PORT);
		return;
	}
	if((port < DIGITAL_COUNT) && (digitalIO_getMode(port) == PIN_MODE_OUT))
	{
		ringbuffer_push(conn.txBuffer, HCP_INVALID_IO);
		return;
	}

	ringbuffer_push(conn.txBuffer, HCP_DIGITAL_REP);
	ringbuffer_push(conn.txBuffer, port);
	ringbuffer_push(conn.txBuffer, (uint8_t)digitalIO_getState(port));
}
