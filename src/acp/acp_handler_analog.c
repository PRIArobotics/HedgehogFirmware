#include "acp_handler.h"
#include "acp_opcodes.h"
#include "acp_errorcodes.h"
#include "acp_handler_impl.h"
#include "adc.h"


void acp_handler_analog_valueReq(acp_conn_t conn, uint8_t opcode, size_t payload)
{
	uint8_t buf[4];
	uint8_t len = 0;

	uint8_t port = ringbuffer_pop(conn.rxBuffer);
	if(port >= ANALOG_COUNT)
	{
		acp_error(conn, ERR_ANA_OOR, opcode);
		return;
	}
	uint16_t ana = adc_getAnalogInput(port);
	buf[len++] = ANALOG_SENSOR_REPLY;
	buf[len++] = port;
	buf[len++] = (uint8_t) (ana >> 8);
	buf[len++] = (uint8_t) (ana & 0xFF);

	ringbuffer_push_multiple(conn.txBuffer, buf, len);
}


void acp_handler_analog_valueSub(acp_conn_t conn, uint8_t opcode, size_t payload)
{
	//TODO
}


void acp_analog_sendSubUpdate(acp_conn_t conn)
{
	//TODO
}


void acp_handler_analog_pullAct(acp_conn_t conn, uint8_t opcode, size_t payload)
{
	//TODO
}
