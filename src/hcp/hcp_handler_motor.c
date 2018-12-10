#include <stdbool.h>
#include <stdlib.h>
#include "hcp_handler.h"
#include "hcp_opcodes.h"
#include "ringbuffer.h"
#include "motor.h"


//TODO: better position mode, see hlc implementation
void hcp_handler_motor(hcp_conn_t conn, uint8_t opcode, size_t payloadLength) //TODO: error replies
{
	if(opcode == HCP_MOTOR)
	{
		uint8_t port;
		if(ringbuffer_pop(conn.rxBuffer, &port)) return;

		uint8_t mode;
		if(ringbuffer_pop(conn.rxBuffer, &mode)) return;

		uint8_t pvp_h;
		if(ringbuffer_pop(conn.rxBuffer, &pvp_h)) return;
		uint8_t pvp_l;
		if(ringbuffer_pop(conn.rxBuffer, &pvp_l)) return;
		int16_t pvp = (pvp_h << 8) | pvp_l;

		if(port >= MOTOR_COUNT)
		{
			ringbuffer_push(conn.txBuffer, HCP_INVALID_PORT);
			return;
		}
		if(mode > 3)
		{
			ringbuffer_push(conn.txBuffer, HCP_INVALID_MODE);
			return;
		}
		if((mode == MOTOR_MODE_POWER || mode == MOTOR_MODE_BRAKE) && abs(pvp) > MOTOR_MAX_POWER)
		{
			ringbuffer_push(conn.txBuffer, HCP_INVALID_VALUE);
			return;
		}

		motor_set(port, mode, pvp);
	}
	else if(opcode == HCP_MOTOR_CONFIG_DC)
	{
		uint8_t port;
		if(ringbuffer_pop(conn.rxBuffer, &port)) return;

		if(port >= MOTOR_COUNT)
		{
			ringbuffer_push(conn.txBuffer, HCP_INVALID_PORT);
			return;
		}

		motor_configure(port, MOTOR_TYPE_DC, 0, 0);
	}
	else if(opcode == HCP_MOTOR_CONFIG_ENC)
	{
		uint8_t port;
		if(ringbuffer_pop(conn.rxBuffer, &port)) return;

		uint8_t enc_a;
		if(ringbuffer_pop(conn.rxBuffer, &enc_a)) return;

		uint8_t enc_b;
		if(ringbuffer_pop(conn.rxBuffer, &enc_b)) return;

		if(port >= MOTOR_COUNT || enc_a == enc_b || enc_a > 7 || enc_b > 7)
		{
			ringbuffer_push(conn.txBuffer, HCP_INVALID_PORT);
			return;
		}

		motor_configure(port, MOTOR_TYPE_ENC, enc_a, enc_b);
	}
	else if(opcode == HCP_MOTOR_CONFIG_STEP)
	{
		uint8_t port;
		if(ringbuffer_pop(conn.rxBuffer, &port)) return;

		if(!(port == 0 || port == 2))
		{
			ringbuffer_push(conn.txBuffer, HCP_INVALID_PORT);
			return;
		}

		motor_configure(port, MOTOR_TYPE_STEP, 0, 0);
	}

	ringbuffer_push(conn.txBuffer, HCP_OK);
}
