#include <stdbool.h>
#include "hcp_handler.h"
#include "hcp_opcodes.h"
#include "ringbuffer.h"
#include "servo.h"


void hcp_handler_servo(hcp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	uint8_t port;
	if(ringbuffer_pop(conn.rxBuffer, &port)) return;

	uint8_t ontime_h;
	if(ringbuffer_pop(conn.rxBuffer, &ontime_h)) return;
	uint8_t ontime_l;
	if(ringbuffer_pop(conn.rxBuffer, &ontime_l)) return;
	uint16_t ontime = (ontime_h << 8) | ontime_l;
	bool enabled = (ontime & 0x8000); //get enable bit
	ontime &= ~0x8000; //remove enable bit

	if(port >= SERVO_COUNT)
	{
		ringbuffer_push(conn.txBuffer, HCP_INVALID_PORT);
		return;
	}
	if((ontime > SERVO_MAX_ONTIME) || ((ontime < SERVO_MIN_ONTIME)))
	{
		ringbuffer_push(conn.txBuffer, HCP_INVALID_VALUE);
		return;
	}

	servo_setOntime(port, ontime);
	servo_setEnabled(port, enabled);

	ringbuffer_push(conn.txBuffer, HCP_OK);
}
