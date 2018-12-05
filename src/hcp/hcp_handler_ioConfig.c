#include "hcp_handler.h"
#include "hcp_opcodes.h"
#include "ringbuffer.h"
#include "digitalIO.h"
#include "output.h"


void hcp_handler_ioConfig(hcp_conn_t conn, uint8_t opcode, size_t payloadLength) //TODO: avoid enc pins getting set as output
{
	uint8_t port;
	if(ringbuffer_pop(conn.rxBuffer, &port)) return;
	uint8_t flags;
	if(ringbuffer_pop(conn.rxBuffer, &flags)) return;

	if((port >= DIGITAL_COUNT) && (port != HCP_DIGITAL_LED0_PORT) && (port != HCP_DIGITAL_LED1_PORT))
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

	if(port == HCP_DIGITAL_LED0_PORT)
	{
		led0(state);
		ringbuffer_push(conn.txBuffer, HCP_OK);
		return;
	}
	if(port == HCP_DIGITAL_LED1_PORT)
	{
		led1(state);
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
