#include "hcp_handler.h"
#include "hcp_opcodes.h"
#include "ringbuffer.h"
#include "adc.h"
#include "power.h"
#include "digitalIO.h"


void hcp_handler_analogReq(hcp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	uint8_t port;
	if(ringbuffer_pop(conn.rxBuffer, &port)) return;

	if((port >= ANALOG_COUNT) && (port != HCP_ANALOG_SUPPLY_PORT))
	{
		ringbuffer_push(conn.txBuffer, HCP_INVALID_PORT);
		return;
	}
	if((port < ANALOG_COUNT) && (digitalIO_getMode(port) == PIN_MODE_OUT))
	{
		ringbuffer_push(conn.txBuffer, HCP_INVALID_IO);
		return;
	}

	uint16_t value;
	if(port == HCP_ANALOG_SUPPLY_PORT) value = power_getInputVoltage_mV();
	else value = adc_getAnalogInput(port);

	ringbuffer_push(conn.txBuffer, HCP_ANALOG_REP);
	ringbuffer_push(conn.txBuffer, port);
	ringbuffer_push(conn.txBuffer, (uint8_t)(value >> 8)); //high byte
	ringbuffer_push(conn.txBuffer, (uint8_t)value); //low byte
}
