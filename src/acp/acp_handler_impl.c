#include "acp_handler_impl.h"
#include "acp_opcodes.h"


void acp_error(acp_conn_t conn, uint8_t error_code, uint8_t causing_opc)
{
	uint8_t buf[3];
	uint8_t len = 0;

	buf[len++] = ERROR_ACTION;
	buf[len++] = error_code;
	buf[len++] = causing_opc;

	ringbuffer_push_multiple(conn.txBuffer, buf, len);
}
