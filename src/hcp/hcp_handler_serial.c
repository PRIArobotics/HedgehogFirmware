#include "hcp_handler.h"
#include "hcp_opcodes.h"
#include "ringbuffer.h"


void hcp_handler_serial(hcp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	//TODO serial handler
	ringbuffer_consume(conn.rxBuffer, payloadLength);
	ringbuffer_push(conn.txBuffer, HCP_UNSUPPORTED_OPCODE);
}
