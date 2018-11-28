#include <stdbool.h>
#include "hcp_handler.h"
#include "hcp_opcodes.h"
#include "ringbuffer.h"
#include "motor.h"


void hcp_handler_motor(hcp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	uint8_t port;
	if(ringbuffer_pop(conn.rxBuffer, &port)) return;

	uint8_t mode;
	if(ringbuffer_pop(conn.rxBuffer, &mode)) return;

	uint8_t power_h;
	if(ringbuffer_pop(conn.rxBuffer, &power_h)) return;
	uint8_t power_l;
	if(ringbuffer_pop(conn.rxBuffer, &power_l)) return;
	uint16_t power = (power_h << 8) | power_l;
	bool direction = (power & 0x8000); //get direction bit
	power &= ~0x8000; //remove direction bit

	if(port >= MOTOR_COUNT)
	{
		ringbuffer_push(conn.txBuffer, HCP_INVALID_PORT);
		return;
	}
	if((mode != HCP_MOTOR_MODE_POWER) && (mode != HCP_MOTOR_MODE_BREAK))
	{
		ringbuffer_push(conn.txBuffer, HCP_INVALID_MODE);
		return;
	}
	if(power > MOTOR_MAX_POWER)
	{
		ringbuffer_push(conn.txBuffer, HCP_INVALID_VALUE);
		return;
	}

	if(mode == HCP_MOTOR_MODE_POWER)
	{
		if(direction) motor_setMode(port, MOTOR_MODE_FORWARD);
		else motor_setMode(port, MOTOR_MODE_REVERSE);
		motor_setPower(port, power);
	}
	else if(mode == HCP_MOTOR_MODE_BREAK)
	{
		motor_setMode(port, MOTOR_MODE_BREAK);
		motor_setPower(port, power);
	}

	ringbuffer_push(conn.txBuffer, HCP_OK);
}
