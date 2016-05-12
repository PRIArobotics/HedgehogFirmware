#include <stdbool.h>
#include "hcp_handler.h"
#include "hcp_opcodes.h"
#include "ringbuffer.h"
#include "servo.h"


void hcp_handler_servo(hcp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	uint8_t port = ringbuffer_pop(conn.rxBuffer);
	uint16_t position = 0;
	position |= (ringbuffer_pop(conn.rxBuffer) << 8); //high byte
	position |= ringbuffer_pop(conn.rxBuffer); //low byte
	bool enabled = (position & 0x8000); //get enable bit
	position &= ~0x8000; //remove enable bit

	if(port >= SERVO_COUNT)
	{
		ringbuffer_push(conn.txBuffer, HCP_INVALID_PORT);
		return;
	}
	if(position > SERVO_MAX_POSITION)
	{
		ringbuffer_push(conn.txBuffer, HCP_INVALID_VALUE);
		return;
	}

	servo_setPosition(port, position);
	servo_setEnabled(port, enabled);

	ringbuffer_push(conn.txBuffer, HCP_OK);
}
