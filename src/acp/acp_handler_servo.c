#include "acp_handler.h"
#include "acp_opcodes.h"
#include "acp_errorcodes.h"
#include "acp_handler_impl.h"
#include "ringbuffer.h"
#include "servo.h"

void acp_handler_servo_onOffAct(acp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	uint8_t port = ringbuffer_pop(conn.rxBuffer);
	uint8_t state = ringbuffer_pop(conn.rxBuffer);
	if(port >= SERVO_COUNT)
	{
		acp_error(conn, ERR_SERVO_OOR, opcode);
		return;
	}
	servo_setEnabled(port, (bool)state);
}


void acp_handler_servo_positionAct(acp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	uint8_t port = ringbuffer_pop(conn.rxBuffer);
	uint8_t pos = ringbuffer_pop(conn.rxBuffer);
	if(port >= SERVO_COUNT)
	{
		acp_error(conn, ERR_SERVO_OOR, opcode);
		return;
	}
	servo_setPosition(port, (pos * 2000 / 255));
}


void acp_handler_servo_positionSyncAct(acp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	size_t i;
	for(i = 0; i < payloadLength; i += 2)
	{
		uint8_t port = ringbuffer_peek_relative(conn.rxBuffer, i);
		if(port >= SERVO_COUNT)
		{
			acp_error(conn, ERR_SERVO_OOR, opcode);
			return;
		}
	}
	for(i = 0; i < payloadLength; i += 2)
	{
		uint8_t port = ringbuffer_pop(conn.rxBuffer);
		uint8_t pos = ringbuffer_pop(conn.rxBuffer);
		servo_setPosition(port, pos);
	}
}
