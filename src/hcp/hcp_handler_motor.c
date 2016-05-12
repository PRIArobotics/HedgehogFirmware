#include <stdbool.h>
#include "hcp_handler.h"
#include "hcp_opcodes.h"
#include "ringbuffer.h"
#include "motor.h"


#define HCP_MOTOR_MODE_POWER 0x00
#define HCP_MOTOR_MODE_BREAK 0x01


void hcp_handler_motor(hcp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	uint8_t port = ringbuffer_pop(conn.rxBuffer);
	uint8_t mode = ringbuffer_pop(conn.rxBuffer);
	uint16_t power = 0;
	power |= (ringbuffer_pop(conn.rxBuffer) << 8); //high byte
	power |= ringbuffer_pop(conn.rxBuffer); //low byte
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
