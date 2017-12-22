#include "hcp_handler.h"
#include "hcp_opcodes.h"
#include "ringbuffer.h"
#include "digitalIO.h"
#include "output.h"


void hcp_handler_ioState(hcp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	uint8_t port = ringbuffer_pop(conn.rxBuffer);
	uint8_t flags = ringbuffer_pop(conn.rxBuffer);

	if((port >= DIGITAL_COUNT) && (port != HCP_DIGITAL_LED1_PORT) && (port != HCP_DIGITAL_LED2_PORT))
	{
		ringbuffer_push(conn.txBuffer, HCP_INVALID_PORT);
		return;
	}

	bool output = flags & 0x01;
	bool pullup = flags & 0x02;
	bool pulldown = flags & 0x04;
	bool state = flags & 0x08;

	if((flags & 0xF0) || (output && (pullup || pulldown)) || (pullup && pulldown) || (!output && state))
	{
		ringbuffer_push(conn.txBuffer, HCP_INVALID_FLAGS);
		return;
	}

	if(port == HCP_DIGITAL_LED1_PORT)
	{
		led1(state);
		ringbuffer_push(conn.txBuffer, HCP_OK);
		return;
	}
	if(port == HCP_DIGITAL_LED2_PORT)
	{
		led2(state);
		ringbuffer_push(conn.txBuffer, HCP_OK);
		return;
	}

	if(output)
	{
		digitalIO_setMode(port, PIN_MODE_OUT);
		digitalIO_setState(port, state);
	}
	else //input
	{
		if(pullup) digitalIO_setMode(port, PIN_MODE_IN_PULLUP);
		else if(pulldown) digitalIO_setMode(port, PIN_MODE_IN_PULLDOWN);
		else digitalIO_setMode(port, PIN_MODE_IN_FLOATING);
	}

	ringbuffer_push(conn.txBuffer, HCP_OK);
}
