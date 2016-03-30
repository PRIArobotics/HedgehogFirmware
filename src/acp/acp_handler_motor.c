#include "acp_handler.h"
#include "acp_opcodes.h"
#include "acp_errorcodes.h"
#include "acp_handler_impl.h"
#include "motor.h"


void acp_handler_motor_offAct(acp_conn_t conn, uint8_t opcode, size_t payload)
{
	uint8_t port = ringbuffer_pop(conn.rxBuffer);
	if(port >= MOTOR_COUNT)
	{
		acp_error(conn, ERR_MOTOR_OOR, opcode);
		return;
	}
	motor_setMode(port, MOTOR_MODE_COAST);
}


void acp_handler_motor_powerAct(acp_conn_t conn, uint8_t opcode, size_t payload)
{
	uint8_t port = ringbuffer_pop(conn.rxBuffer);
	uint8_t dir = ringbuffer_pop(conn.rxBuffer);
	uint8_t power = ringbuffer_pop(conn.rxBuffer);
	if(port >= MOTOR_COUNT)
	{
		acp_error(conn, ERR_MOTOR_OOR, opcode);
		return;
	}
	motor_setMode(port, (dir? MOTOR_MODE_FORWARD: MOTOR_MODE_REVERSE));
	motor_setPower(port, (power * 1000 / 255));
}


void acp_handler_motor_brakeAct(acp_conn_t conn, uint8_t opcode, size_t payload)
{
	uint8_t port = ringbuffer_pop(conn.rxBuffer);
	if(port >= MOTOR_COUNT)
	{
		acp_error(conn, ERR_MOTOR_OOR, opcode);
		return;
	}
	motor_setMode(port, MOTOR_MODE_BREAK);
}
