#include "acp_handler.h"
#include "acp_opcodes.h"
#include "acp_errorcodes.h"
#include "acp_handler_impl.h"
#include "digitalInput.h"


void acp_handler_digital_valueReq(acp_conn_t conn, uint8_t opcode, size_t payload)
{
	uint8_t buf[3];
	uint8_t len = 0;

	uint8_t port = ringbuffer_pop(conn.rxBuffer);
	if(port >= DIGITAL_COUNT)
	{
		acp_error(conn, ERR_DIG_OOR, opcode);
		return;
	}
	uint8_t value = digitalInput_getState(port);
	buf[len++] = DIGITAL_SENSOR_REPLY;
	buf[len++] = port;
	buf[len++] = value;

	ringbuffer_push_multiple(conn.txBuffer, buf, len);
}


void acp_handler_digital_valueSub(acp_conn_t conn, uint8_t opcode, size_t payload)
{
	//TODO
}


void acp_digital_sendSubUpdate(acp_conn_t conn)
{
	//TODO
}


void acp_handler_digital_pullAct(acp_conn_t conn, uint8_t opcode, size_t payload)
{
	//TODO
}
