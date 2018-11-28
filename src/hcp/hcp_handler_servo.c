#include <stdbool.h>
#include "hcp_handler.h"
#include "hcp_opcodes.h"
#include "ringbuffer.h"
#include "servo.h"


void hcp_handler_servo(hcp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	uint8_t port;
	if(ringbuffer_pop(conn.rxBuffer, &port)) return;

	uint8_t position_h;
	if(ringbuffer_pop(conn.rxBuffer, &position_h)) return;
	uint8_t position_l;
	if(ringbuffer_pop(conn.rxBuffer, &position_l)) return;
	uint16_t position = (position_h << 8) | position_l;
	bool enabled = (position & 0x8000); //get enable bit
	position &= ~0x8000; //remove enable bit

	if(port >= SERVO_COUNT)
	{
		ringbuffer_push(conn.txBuffer, HCP_INVALID_PORT);
		return;
	}
	if((position > SERVO_MAX_ONTIME) || ((position < SERVO_MIN_ONTIME)))
	{
		ringbuffer_push(conn.txBuffer, HCP_INVALID_VALUE);
		return;
	}

	servo_setPosition(port, position);
	servo_setEnabled(port, enabled);

	ringbuffer_push(conn.txBuffer, HCP_OK);
}
