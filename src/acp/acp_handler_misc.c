#include "acp_handler.h"
#include "acp.h"
#include "acp_opcodes.h"
#include "acp_errorcodes.h"
#include "acp_handler_impl.h"


void acp_handler_nop(acp_conn_t conn, uint8_t opcode, size_t payload)
{
}


void acp_handler_HWC_offAct(acp_conn_t conn, uint8_t opcode, size_t payload)
{
	//TODO
}


void acp_handler_HWC_resetAct(acp_conn_t conn, uint8_t opcode, size_t payload)
{
	//TODO
}


void acp_handler_HWC_typeReq(acp_conn_t conn, uint8_t opcode, size_t payload)
{
	uint8_t buf[2];
	uint8_t len = 0;

	buf[len++] = HWC_TYPE_REPLY;
	buf[len++] = HWC_TYPE;

	ringbuffer_push_multiple(conn.txBuffer, buf, len);
}


void acp_handler_battery_voltageReq(acp_conn_t conn, uint8_t opcode, size_t payload)
{
	//TODO
}
